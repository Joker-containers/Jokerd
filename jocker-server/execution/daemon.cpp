//
// Created by Nazar Demchuk on 06.01.2024.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <mutex>
#include "daemon.h"

const int PORT = 12345;

void logMessage(const std::string& message, bool to_cerr = false) {

    if (to_cerr) {
        std::cerr << message << std::endl;
    } else {
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

void receiveFileandConfig(int client_socket) {
    std::ofstream logsFile("containers.log", std::ios::app);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

    logMessage("Starting to receive container files...");

    logMessage("\nReceiving binary filename size...");
    uint64_t binary_name_size;
    recv(client_socket, &binary_name_size, sizeof(binary_name_size), 0);
    logMessage("Received binary filename length: " + std::to_string(binary_name_size));

    logMessage("\nReceiving binary filename...");
    std::vector<char> binary_name(binary_name_size);
    recv(client_socket, binary_name.data(), binary_name_size, 0);
    logMessage("Received binary filename: " + std::string(binary_name.data()));

    logMessage("\nReceiving binary size...");
    uint64_t binary_size;
    recv(client_socket, &binary_size, sizeof(binary_size), 0);
    logMessage("Received binary size: " + std::to_string(binary_size));

    logMessage("\nReceiving binary...");
    std::vector<char> binary(binary_size);
    recv(client_socket, binary.data(), binary_size, 0);
    logMessage("Received binary.");

    std::string config_filename = std::string(binary_name.data()) + ".joker";
    logMessage("\nConfig filename is: " + config_filename);
    logMessage("Receiving config size...");
    uint64_t binary_config_size;
    recv(client_socket, &binary_config_size, sizeof(binary_config_size), 0);
    logMessage("Received config size: " + std::to_string(binary_config_size));

    logMessage("\nReceiving config...");
    std::vector<char> binary_config(binary_config_size);
    recv(client_socket, binary_config.data(), binary_config_size, 0);
    logMessage("Received config.");

    std::ifstream check_file(binary_name.data());
    if (check_file.good()) {
        logMessage("\nError: File with the name " + std::string(binary_name.data()) + " already exists.", true);
        return;
    }
    check_file.close();

    logMessage("\nSaving binary file...");
    std::ofstream binary_file(binary_name.data(), std::ios::binary);
    binary_file.write(binary.data(), binary_size);
    binary_file.close();
    logMessage("Saved binary file to " + std::string(binary_name.data()) + ".\n");

    logMessage("\nSaving config file...");
    std::ofstream binary_config_file(config_filename, std::ios::binary);
    binary_config_file.write(binary_config.data(), binary_config_size);
    binary_config_file.close();
    logMessage("Saved config file to " + config_filename + ".\n");
}

void sendLogs(int client_socket) {
    std::ifstream logsFile("containers.log", std::ios::binary);
    if (!logsFile.is_open()) {
        std::cerr << "Error: Unable to open logs file." << std::endl;
        return;
    }

    std::vector<char> logsContent((std::istreambuf_iterator<char>(logsFile)),
                                  std::istreambuf_iterator<char>());

    uint64_t logsSize = logsContent.size();
    send(client_socket, logsContent.data(), logsSize, 0);

    logsFile.close();
}

void processClientRequest(int client_socket) {
    char firstByte;
    recv(client_socket, &firstByte, sizeof(firstByte), 0);

    if (firstByte == 0) {
        receiveFileandConfig(client_socket);
    } else if (firstByte == 1 || firstByte == 2) {
        sendLogs(client_socket);
    }
}

void setupServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        logMessage("Error: Unable to create socket.", true);
        exit(1);
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
        logMessage("Error: Unable to bind socket.", true);
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 5) == -1) {
        logMessage("Error: Unable to listen on socket.", true);
        close(serverSocket);
        exit(1);
    }

    logMessage("Daemon listening on port " + std::to_string(PORT));

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            logMessage("Error: Unable to accept client connection.", true);
            continue;
        }

        processClientRequest(clientSocket);

        close(clientSocket);
    }

    close(serverSocket);
}
