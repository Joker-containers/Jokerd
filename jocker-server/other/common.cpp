#include "common.h"
#include "syscall_wrapper.h"

void deleteCharPtrArray(char **av) {
    if (av) {
        for (size_t i = 0; av[i]; ++i) {
            delete[] av[i];
        }
        delete[] av;
    }
}

std::unique_ptr<char *[], void(*)(char **)> createCharPtrArray(const std::vector<std::string> &tokens) {
    std::unique_ptr<char *[], void(*)(char **)> ptr(new char *[tokens.size() + 1], deleteCharPtrArray);
    ptr[tokens.size()] = nullptr;
    for (size_t i = 0; i < tokens.size(); ++i) {
        ptr[i] = new char[tokens[i].length() + 1];
        std::strcpy(ptr[i], tokens[i].c_str());
    }
    return ptr;
}

struct raii_fd{
    int fd;

    explicit raii_fd(int fd): fd(fd){};
    ~raii_fd(){
        syscall_wrapper(close, "close", fd);
    }
};

int write_to_file(const std::string &path, const std::string &content){
    raii_fd fd = raii_fd(syscall_wrapper(open, "open", path.c_str(), O_RDWR));
    off_t content_ptr = 0;
    while (true){
        content_ptr += syscall_wrapper(write, "write", fd.fd, content.c_str() + content_ptr, content.size() - content_ptr);
        if (content_ptr == content.size()){
            break;
        }
    }
    return 0;
}