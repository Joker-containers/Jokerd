//#include <iostream>
//#include <sys/wait.h>
//#include <err.h>
//#include <sched.h>
//#include <signal.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/utsname.h>
//#include <unistd.h>
//#include <sys/cdefs.h>
//#include <sys/types.h>
//#include <sys/mount.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <string>
//#include <fstream>
//#include <sys/mount.h>
//
//constexpr size_t CHILD_STACK_SIZE = 1024*1024; // Megabyte /* Stack size for cloned child */
//const std::string CGROUP_DIR = "/sys/fs/cgroup/cpu"; // Cgroup path
//
//static int              /* Start function for cloned child */
//child_function(void *arg)
//{
//    struct utsname uts;
//
//    /* Change hostname in UTS namespace of child. */
//
//    if (sethostname(static_cast<char *>(arg), strlen(static_cast<char *>(arg))) == -1)
//        err(EXIT_FAILURE, "sethostname");
//
//    /* Retrieve and display hostname. */
//
//    if (uname(&uts) == -1)
//        err(EXIT_FAILURE, "uname");
//    printf("uts.nodename in child:  %s\n", uts.nodename);
//
//    /* Keep the namespace open for a while, by sleeping.
//       This allows some experimentation--for example, another
//       process might join the namespace. */
//
//    sleep(5);
//
//    // TODO: make wrapper function for this stuff
//    char **new_argv = new char *[2];
//    char *progname_copy = new char [strlen(static_cast<char *>(arg)) + 1];
//    strcpy(progname_copy, static_cast<char *>(arg));
//    new_argv[0] = progname_copy;
//    new_argv[1] = nullptr;
//
//    auto error = execvp(static_cast<char *>(arg), new_argv); // TODO: handle the errors
//    perror("execvp");
//    return 0;
//}
//
//void create_container(char *progname){
//    // Creates container.
//    // First, a cgroup directory should be created and set up.
//    // Then, call clone(). Clone unshares his namespaces, while parent sets them up.
//    // Finally, after some kind of message about setup finish, child does exec()
//    // From that moment it is the requested program launched in the container
//    // Probably this method will delegate all the work to the container's constructor
//    pid_t           pid;
//    struct utsname  uts;
//
//    {
//        //Or before utsname???
//        //Adjust parameters of container
//        std::string cpu_quota_path = CGROUP_DIR + "/jocker" + "/cpu.cfs_quota_us";
//        std::ofstream cpu_quota_file(cpu_quota_path);
//        if (!cpu_quota_file) {
//            perror("opening cpu quota file_name");
//            exit(EXIT_FAILURE);
//        }
//        cpu_quota_file << 50000; // Should be as container argument
//        cpu_quota_file.close();
//
//
//        // That's just a temporary area of memory reserved for child process' initial stack, before it gets cloned()
//        // (actually don't know if it works the way I think yet)
//        char child_stack[CHILD_STACK_SIZE];
//        void            *stack;         /* Start of stack buffer */
//        void            *stackTop;      /* End of stack buffer */
//
////        std::cout << static_cast<void*>(&child_stack) << std::endl;
////        std::cout << static_cast<void*>(&child_stack[CHILD_STACK_SIZE]) << std::endl;
//
//        stack = static_cast<void *>(&child_stack);
//        stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */
//
//        /* Create child that has its own UTS namespace;
//           child commences execution in childFunc(). */
//
//        pid = clone(child_function, &stackTop, CLONE_NEWUTS | SIGCHLD, progname);
//        if (pid == -1)
//            err(EXIT_FAILURE, "clone"); // TODO: handle this
//        printf("clone() returned %jd\n", (intmax_t) pid);
//
//        // Add child process to container
//        std::string tasks_path = CGROUP_DIR + "/jocker" + "/tasks";
//        std::ofstream tasks_file(tasks_path);
//        if (!tasks_file) {
//            perror("opening tasks file_name");
//            exit(EXIT_FAILURE);
//        }
//        tasks_file << pid;
//        // And $$ too???
//        tasks_file.close();
//
//        /* Parent falls through to here */
//
//        sleep(1);           /* Give child time to change its hostname */
//
//        /* Display hostname in parent's UTS namespace. This will be
//           different from hostname in child's UTS namespace. */
//
//        if (uname(&uts) == -1)
//            err(EXIT_FAILURE, "uname"); // TODO: handle this
//        printf("uts.nodename in parent: %s\n", uts.nodename);
//
//        if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
//            err(EXIT_FAILURE, "waitpid"); // TODO: handle this
//        printf("child has terminated\n");
//    }
//}
//
//void list_containers(){
//    // Lists containers and relevant info, probably by some internal method of container cluster.
//}
//
//void get_container_info(){
//    // Provides a detailed info about the specified container
//}
//
//void remove_container(){
//    // Kills processes inside the container and releases all the resources. Clears related directories etc.
//}

// MAIN
    // First thing main does: checks whether cgroup "Jocker" directory exists, creates if not.
    // Then, waiting commands from cli (using sockets?)
//    if (argc < 2) {
//        fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
//        exit(EXIT_SUCCESS);
//    }
//
//    // Mount against cgroup fs
//    if (mkdir(CGROUP_DIR.c_str(), 0755) != -1 && errno != EEXIST) {
//        perror("mkdir");
//        exit(EXIT_FAILURE);
//    }
//    mount("none", CGROUP_DIR.c_str(), NULL, MS_BIND, NULL);
//
//
//    // Create a cgroup directory for the container
//    std::string container_dir = CGROUP_DIR + "/jocker";
//    if (mkdir(container_dir.c_str(), 0755) != -1 && errno != EEXIST) {
//        perror("mkdir");
//        exit(EXIT_FAILURE);
//    }
//
//    create_container(argv[1]);
//    return 0;

#include "container.h"

int main(int argc, char *argv[]) {
    ns_options ns_opt = ns_options();
    std::string uts_ns_name = "uts_test";
    ns_opt.add_namespace(UTS, uts_ns_name);

    std::string bin_path = "ls";

    std::vector<std::string> bin_args = {};

    std::string container_name = "FIRST CONTAINER";

    container_options opt = container_options(ns_opt, {}, bin_args, bin_path, container_name);

    ns_pool pool {};

    ns_conf_repository repo = ns_conf_repository();

    std::string uts_hostname = "TESTMACHINE";
    uts_ns_config conf = uts_ns_config(uts_hostname);
    repo.uts_ns_configs[uts_ns_name] = conf;

    auto res = d_resources(pool, repo);
    container c = container(opt, res);
}
