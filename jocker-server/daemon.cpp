#include <iostream>
#include <fstream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 12345; // Replace with your desired port number

void receiveFileandConfig(int clientSocket) {
    std::cout << "\nStarting to receive container files...\n" << std::endl;

    std::cout << "\nReceiving binary filename size..." << std::endl;
    uint64_t binary_name_size;
    recv(clientSocket, &binary_name_size, sizeof(binary_name_size), 0);
    std::cout << "Received binary filename length: " << binary_name_size << std::endl;

    std::cout << "\nReceiving binary filename..." << std::endl;
    std::vector<char> binary_name(binary_name_size);
    recv(clientSocket, binary_name.data(), binary_name_size, 0);
    std::cout << "Received binary filename: " << binary_name.data() << std::endl;

    std::cout << "\nReceiving binary size..." << std::endl;
    uint64_t binary_size;
    recv(clientSocket, &binary_size, sizeof(binary_size), 0);
    std::cout << "Received binary size: " << binary_size << std::endl;

    std::cout << "\nReceiving binary..." << std::endl;
    std::vector<char> binary(binary_size);
    recv(clientSocket, binary.data(), binary_size, 0);
    std::cout << "Received binary." << std::endl;

    std::string config_filename = std::string(binary_name.data()) + ".joker";
    std::cout << "\nConfig filename is: " << config_filename << std::endl;
    std::cout << "Receiving config size..." << std::endl;
    uint64_t binary_config_size;
    recv(clientSocket, &binary_config_size, sizeof(binary_config_size), 0);
    std::cout << "Received config size: " << binary_config_size << std::endl;

    std::cout << "\nReceiving config..." << std::endl;
    std::vector<char> binary_config(binary_config_size);
    recv(clientSocket, binary_config.data(), binary_config_size, 0);
    std::cout << "Received config." << std::endl;

    std::ifstream check_file(binary_name.data());
    if (check_file.good()) {
        std::cerr << "\nError: File with the name " << binary_name.data() << " already exists." << std::endl;
        return;
    }
    check_file.close();

    std::cout << "\nSaving binary file..." << std::endl;
    std::ofstream binary_file(binary_name.data(), std::ios::binary);
    binary_file.write(binary.data(), binary_size);
    binary_file.close();
    std::cout << "Saved binary file to " << binary_name.data() << ".\n" << std::endl;

    std::cout << "\nSaving config file..." << std::endl;
    std::ofstream binary_config_file(config_filename, std::ios::binary);
    binary_config_file.write(binary_config.data(), binary_config_size);
    binary_config_file.close();
    std::cout << "Saved config file to " << config_filename << ".\n" << std::endl;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: Unable to create socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Unable to bind socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error: Unable to listen on socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Daemon listening on port " << PORT << std::endl;

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error: Unable to accept client connection." << std::endl;
            continue;
        }

        receiveFileandConfig(clientSocket);

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
