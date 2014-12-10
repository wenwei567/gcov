#include <iostream>
#include <cstdlib>

#include "my_strace/my_strace.hh"

static void usage()
{
    std::cout << "Usage: ./my_gcov --level{1,2,3,4} /path/to/binary [args]";
    std::cout << std::endl;

    std::exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        usage();

    std::string level_str = argv[1];
    std::string bin_path = argv[2];

    if (level_str == "--level1")
        my_strace(bin_path, argv);

    else
        std::cout << "Not implemented yet, sorry." << std::endl;
}