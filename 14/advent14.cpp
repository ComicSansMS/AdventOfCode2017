#include <disk_defrag.hpp>


#include <iostream>

int main(int argc, char* argv[])
{
    char const* input = "hwlqcszp";
    if(argc == 2) {
        input = argv[1];
    }

    Grid g(input);

    std::cout << "First result is " << g.count() << std::endl;
    std::cout << "Second result is " << g.countRegions() << std::endl;

    return 0;
}
