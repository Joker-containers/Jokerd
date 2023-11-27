#include <iostream>
#include <fstream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>

constexpr int PORT = 12345;

void receiveFile(int clientSocket, const std::string& filePath) {
    std::ofstream fileStream(filePath, std::ios::binary);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << filePath << std::endl;
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    size_t totalBytesReceived = 0;

    while ((bytesRead = read(clientSocket, buffer, sizeof(buffer))) > 0) {
        fileStream.write(buffer, bytesRead);
        totalBytesReceived += bytesRead;
    }

    fileStream.close();

    if (bytesRead == -1) {
        std::cerr << "Error occurred during file transfer." << std::endl;
    } else {
        std::cout << "File received: " << filePath << std::endl;
        std::cout << "Size of the file: " << totalBytesReceived << " bytes" << std::endl;
    }
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

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
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

        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error: Unable to accept client connection." << std::endl;
            continue;
        }

//        char filePathBuffer[1024];
//        ssize_t filePathLength = read(clientSocket, filePathBuffer, sizeof(filePathBuffer));
//        if (filePathLength <= 0) {
//            std::cerr << "Error: Unable to read file path from client." << std::endl;
//            close(clientSocket);
//            continue;
//        }
//
//        filePathBuffer[filePathLength] = '\0';
//        std::string filePath(filePathBuffer);
//
//        receiveFile(clientSocket, filePath);
        receiveFile(clientSocket, "saved_file.json");

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
