//
// Created by Nazar Demchuk on 06.01.2024.
//

#ifndef JOCKER_SERVER_DAEMON_H
#define JOCKER_SERVER_DAEMON_H

extern const int PORT;

void logMessage(const std::string& message, bool to_cerr);

void receiveFileandConfig(int client_socket);

void sendLogs(int client_socket);

void processClientRequest(int client_socket);

void setupServer();

#endif //JOCKER_SERVER_DAEMON_H
