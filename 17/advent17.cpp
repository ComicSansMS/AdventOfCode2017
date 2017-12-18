#include <spin_lock.hpp>

#include <algorithm>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    int input = 356;
    if(argc == 2) {
        input = std::atoi(argv[1]);
    }

    auto const [b, i] = spin(input, 2017);

    std::cout << "First result is " << b[i+1] << std::endl;

    std::cout << "Second result is " << spin_after_0(input, 50'000'000) << std::endl;
    return 0;
}
