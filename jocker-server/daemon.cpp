#include <iostream>
#include <fstream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <condition_variable>

const int PORT = 12345;
std::condition_variable condVar;
std::mutex condVarMutex;
std::string logsMessage;

void logMessage(const std::string& message, bool tocerr = false) {
    if (tocerr) {
        std::cerr << message << std::endl;
    }
    else {
        std::cout << message << std::endl;
    }
    std::ofstream logsFile("containers.log", std::ios::app);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

    logsFile << message << std::endl;
    logsFile.close();
}

void sendLogs(int clientSocket) {
    while (true) {
        std::unique_lock<std::mutex> lock(condVarMutex);
        condVar.wait(lock, [] { return !logsMessage.empty(); });

        if (logsMessage == "success") {
            std::ifstream logsFile("containers.log");
            if (!logsFile.is_open()) {
                std::cerr << "Error: Unable to open logs file." << std::endl;
                return;
            }

            std::string logsContent((std::istreambuf_iterator<char>(logsFile)), std::istreambuf_iterator<char>());
            logsFile.close();

            send(clientSocket, logsContent.c_str(), logsContent.size(), 0);

            logsMessage.clear();
        }
    }
}

void checkForLogsRequest(int clientSocket) {
    std::vector<char> message(10);
    recv(clientSocket, message.data(), message.size(), 0);
    std::string clientRequest(message.data());

    if (clientRequest == "get logs") {
        {
            std::lock_guard<std::mutex> lock(condVarMutex);
            logsMessage = "success";
        }
        condVar.notify_one();
    }
}

void receiveFileandConfig(int clientSocket) {
    std::ofstream logsFile("containers.log", std::ios::app);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

//    std::cout << "\nStarting to receive container files...\n" << std::endl;
    logMessage("\nStarting to receive container files...");

//    std::cout << "\nReceiving binary filename size..." << std::endl;
    logMessage("\nReceiving binary filename size...");
    uint64_t binary_name_size;
    recv(clientSocket, &binary_name_size, sizeof(binary_name_size), 0);
//    std::cout << "Received binary filename length: " << binary_name_size << std::endl;
    logMessage("Received binary filename length: " + std::to_string(binary_name_size));

//    std::cout << "\nReceiving binary filename..." << std::endl;
    logMessage("\nReceiving binary filename...");
    std::vector<char> binary_name(binary_name_size);
    recv(clientSocket, binary_name.data(), binary_name_size, 0);
//    std::cout << "Received binary filename: " << binary_name.data() << std::endl;
    logMessage("Received binary filename: " + std::string(binary_name.data()));

//    std::cout << "\nReceiving binary size..." << std::endl;
    logMessage("\nReceiving binary size...");
    uint64_t binary_size;
    recv(clientSocket, &binary_size, sizeof(binary_size), 0);
//    std::cout << "Received binary size: " << binary_size << std::endl;
    logMessage("Received binary size: " + std::to_string(binary_size));

//    std::cout << "\nReceiving binary..." << std::endl;
    logMessage("\nReceiving binary...");
    std::vector<char> binary(binary_size);
    recv(clientSocket, binary.data(), binary_size, 0);
//    std::cout << "Received binary." << std::endl;
    logMessage("Received binary.");

    std::string config_filename = std::string(binary_name.data()) + ".joker";
//    std::cout << "\nConfig filename is: " << config_filename << std::endl;
//    std::cout << "Receiving config size..." << std::endl;
    logMessage("\nConfig filename is: " + config_filename);
    logMessage("Receiving config size...");
    uint64_t binary_config_size;
    recv(clientSocket, &binary_config_size, sizeof(binary_config_size), 0);
//    std::cout << "Received config size: " << binary_config_size << std::endl;
    logMessage("Received config size: " + std::to_string(binary_config_size));

//    std::cout << "\nReceiving config..." << std::endl;
    logMessage("\nReceiving config...");
    std::vector<char> binary_config(binary_config_size);
    recv(clientSocket, binary_config.data(), binary_config_size, 0);
//    std::cout << "Received config." << std::endl;
    logMessage("Received config.");

    std::ifstream check_file(binary_name.data());
    if (check_file.good()) {
//        std::cerr << "\nError: File with the name " << binary_name.data() << " already exists." << std::endl;
        logMessage("\nError: File with the name " + std::string(binary_name.data()) + " already exists.", true);
        return;
    }
    check_file.close();

//    std::cout << "\nSaving binary file..." << std::endl;
    logMessage("\nSaving binary file...");
    std::ofstream binary_file(binary_name.data(), std::ios::binary);
    binary_file.write(binary.data(), binary_size);
    binary_file.close();
//    std::cout << "Saved binary file to " << binary_name.data() << ".\n" << std::endl;
    logMessage("Saved binary file to " + std::string(binary_name.data()) + ".\n");

//    std::cout << "\nSaving config file..." << std::endl;
    logMessage("\nSaving config file...");
    std::ofstream binary_config_file(config_filename, std::ios::binary);
    binary_config_file.write(binary_config.data(), binary_config_size);
    binary_config_file.close();
//    std::cout << "Saved config file to " << config_filename << ".\n" << std::endl;
    logMessage("Saved config file to " + config_filename + ".\n");
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
//        std::cerr << "Error: Unable to create socket." << std::endl;
        logMessage("Error: Unable to create socket.", true);
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
//        std::cerr << "Error: Unable to bind socket." << std::endl;
        logMessage("Error: Unable to bind socket.", true);
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
//        std::cerr << "Error: Unable to listen on socket." << std::endl;
        logMessage("Error: Unable to listen on socket.", true);
        close(serverSocket);
        return 1;
    }

//    std::cout << "Daemon listening on port " << PORT << std::endl;
    logMessage("Daemon listening on port " + std::to_string(PORT));

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
//            std::cerr << "Error: Unable to accept client connection." << std::endl;
            logMessage("Error: Unable to accept client connection.", true);
            continue;
        }

        receiveFileandConfig(clientSocket);

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

