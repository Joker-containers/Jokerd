#include "daemon.h"

void runtime(Daemon &daemon) {
    while (true) {
        if (daemon.get_request_type()){
            daemon.execute_request();
        }
        else {
            daemon.reconnect();
        }
    }
}


int main(int argc, char *argv[]) {
    Daemon daemon(10101, "./DAEMON.log");
    try {
        runtime(daemon);
    } catch (std::exception& e) {
        daemon.log_message(e.what(), true);
    }
}
