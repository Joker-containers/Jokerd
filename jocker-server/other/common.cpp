#include "common.h"
#include "syscall_wrapper.h"

const size_t BUFFER_SIZE = 1024;


void deleteCharPtrArray(char **av) {
    if (av) {
        for (size_t i = 0; av[i]; ++i) {
            delete[] av[i];
        }
        delete[] av;
    }
}

std::unique_ptr<char *[], void (*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens) {
    std::unique_ptr<char *[], void (*)(char **)> ptr(new char *[tokens.size() + 1], deleteCharPtrArray);
    ptr[tokens.size()] = nullptr;
    for (size_t i = 0; i < tokens.size(); ++i) {
        ptr[i] = new char[tokens[i].length() + 1];
        std::strcpy(ptr[i], tokens[i].c_str());
    }
    return ptr;
}

int write_to_file(const std::string &path, const std::string &content) {
    raii_fd fd = raii_fd(syscall_wrapper(open, "open", path.c_str(), O_RDWR));
    off_t content_ptr = 0;
    while (true) {
        content_ptr += syscall_wrapper(write, "write", fd.fd, content.c_str() + content_ptr,
                                       content.size() - content_ptr);
        if (content_ptr == content.size()) {
            break;
        }
    }
    return 0;
}


void recv_all(int socket, void *buffer, size_t size, int flags) {
    ssize_t total_bytes_read = 0;
    ssize_t bytes_read;
    auto current_buf_ptr = static_cast<char *>(buffer);

    while (size > 0) {
        bytes_read = syscall_wrapper(recv, "recv", socket, current_buf_ptr + total_bytes_read, size, flags);
        total_bytes_read += bytes_read;
        size -= bytes_read;
    }
}

void send_all(int socket, void *buffer, size_t size, int flags) {
    ssize_t total_bytes_send = 0;
    ssize_t bytes_send;
    auto current_buf_ptr = static_cast<char *>(buffer);

    while (size > 0) {
        bytes_send = send(socket, current_buf_ptr + total_bytes_send, size, flags);
        total_bytes_send += bytes_send;
        size -= bytes_send;
    }
}

std::pair<std::string, std::string> parse_variable(const std::string &line){
    auto const delimiter_pos = line.find_first_of(':');
    if (delimiter_pos == std::string::npos){
        throw std::runtime_error("Invalid formatting!");
    }
    std::string property = line.substr(0, delimiter_pos);
    std::string valur = line.substr(delimiter_pos + 1, line.size() - 1);
}
