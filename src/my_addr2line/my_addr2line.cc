#include "../elf.hh"

#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <unistd.h>

#include <cstring>
#include <iostream>

#include <errno.h>

static void trace_child(pid_t pid_child, char** argv)
{
    Elf elf(argv[2]);
    elf.parse_dwarf();

    int status = 0;
    struct user_regs_struct user_regs;

    wait(&status);

    while (true)
    {
        ptrace(PTRACE_SINGLESTEP, pid_child, 0, 0);

        wait(&status);

        if (WIFEXITED(status))
            break;

        ptrace(PTRACE_GETREGS, pid_child, 0, &user_regs);

        elf.addr2line(user_regs);
    }
}

void my_addr2line(char** argv)
{
    pid_t pid_child = fork();

    if (pid_child == -1)
    {
        std::cerr << "Error forking" << std::endl;
        std::exit(1);
    }

    else if (pid_child == 0)
    {
        ptrace(PTRACE_TRACEME);

        char* bin_argv[32];
        std::memset(bin_argv, 0, sizeof (char*) * 32);
        bin_argv[0] = argv[2];

        for (int i = 3, j = 1; i < 31 && argv[i]; ++i, ++j)
            bin_argv[j] = argv[i];

        execvp(argv[2], bin_argv);
    }

    else
        trace_child(pid_child, argv);
}
