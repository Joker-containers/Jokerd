#include <iostream>
#include <sys/wait.h>
#include <err.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

constexpr size_t CHILD_STACK_SIZE = 1024*1024; // Megabyte /* Stack size for cloned child */

static int              /* Start function for cloned child */
child_function(void *arg)
{
    struct utsname uts;

    /* Change hostname in UTS namespace of child. */

    if (sethostname(static_cast<char *>(arg), strlen(static_cast<char *>(arg))) == -1)
        err(EXIT_FAILURE, "sethostname");

    /* Retrieve and display hostname. */

    if (uname(&uts) == -1)
        err(EXIT_FAILURE, "uname");
    printf("uts.nodename in child:  %s\n", uts.nodename);

    /* Keep the namespace open for a while, by sleeping.
       This allows some experimentation--for example, another
       process might join the namespace. */

    sleep(5);

    // TODO: make wrapper function for this stuff
    char **new_argv = new char *[2];
    char *progname_copy = new char [strlen(static_cast<char *>(arg)) + 1];
    strcpy(progname_copy, static_cast<char *>(arg));
    new_argv[0] = progname_copy;
    new_argv[1] = nullptr;

    auto error = execvp(static_cast<char *>(arg), new_argv); // TODO: handle the errors
    perror("execvp");
}

void create_container(char *progname){
    // Creates container.
    // First, a cgroup directory should be created and set up.
    // Then, call clone(). Clone unshares his namespaces, while parent sets them up.
    // Finally, after some kind of message about setup finish, child does exec()
    // From that moment it is the requested program launched in the container
    // Probably this method will delegate all the work to the container's constructor
    pid_t           pid;
    struct utsname  uts;

    {
        // That's just a temporary area of memory reserved for child process' initial stack, before it gets cloned()
        // (actually don't know if it works the way I think yet)
        char child_stack[CHILD_STACK_SIZE];
        void            *stack;         /* Start of stack buffer */
        void            *stackTop;      /* End of stack buffer */

//        std::cout << static_cast<void*>(&child_stack) << std::endl;
//        std::cout << static_cast<void*>(&child_stack[CHILD_STACK_SIZE]) << std::endl;

        stack = static_cast<void *>(&child_stack);
        stackTop = static_cast<void *>(&child_stack[CHILD_STACK_SIZE]); /* Assume stack grows downward */

        /* Create child that has its own UTS namespace;
           child commences execution in childFunc(). */

        pid = clone(child_function, &stackTop, CLONE_NEWUTS | SIGCHLD, progname);
        if (pid == -1)
            err(EXIT_FAILURE, "clone"); // TODO: handle this
        printf("clone() returned %jd\n", (intmax_t) pid);

        /* Parent falls through to here */

        sleep(1);           /* Give child time to change its hostname */

        /* Display hostname in parent's UTS namespace. This will be
           different from hostname in child's UTS namespace. */

        if (uname(&uts) == -1)
            err(EXIT_FAILURE, "uname"); // TODO: handle this
        printf("uts.nodename in parent: %s\n", uts.nodename);

        if (waitpid(pid, NULL, 0) == -1)    /* Wait for child */
            err(EXIT_FAILURE, "waitpid"); // TODO: handle this
        printf("child has terminated\n");
    }
}

void list_containers(){
    // Lists containers and relevant info, probably by some internal method of container cluster.
}

void get_container_info(){
    // Provides a detailed info about the specified container
}

void remove_container(){
    // Kills processes inside the container and releases all the resources. Clears related directories etc.
}

int main(int argc, char *argv[]) {
    // First thing main does: checks whether cgroup "Jocker" directory exists, creates if not.
    // Then, waiting commands from cli (using sockets?)
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <child-hostname>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    create_container(argv[1]);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
