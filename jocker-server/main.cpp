#include "daemon.h"

//void runtime() {
//    ns_options ns_opt = ns_options();
//    std::string uts_ns_name = "uts_test";
//    ns_opt.add_namespace(UTS, uts_ns_name);
//
//    std::string bin_path = "ls";
//
//    std::vector<std::string> bin_args = {};
//
//    std::string container_name = "FIRST CONTAINER";
//
//    container_options opt = container_options(ns_opt, {}, bin_args, bin_path, container_name);
//
//    ns_pool pool {};
//
//    ns_conf_repository repo = ns_conf_repository();
//
//    std::string uts_hostname = "TESTMACHINE";
//    uts_ns_config conf = uts_ns_config(uts_hostname);
//    repo.uts_ns_configs[uts_ns_name] = conf;
//
//    auto res = d_resources(pool, repo);
//    container c = container(opt, res);
//    sleep(5);
//}

void runtime(Daemon &daemon) {
    while (true) {
        daemon.get_request_type();
        daemon.execute_request();
    }
}


int main(int argc, char *argv[]) {
    Daemon daemon(10110, "./DAEMON.log");
    try {
        runtime(daemon);
    } catch (std::exception& e) {
        daemon.log_message(e.what(), true);
    }
}
