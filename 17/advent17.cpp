#include <spin_lock.hpp>

#include <algorithm>
#include <iostream>

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    int const input = 356;

    auto const [b, i] = spin(input, 2017);

    std::cout << "First result is " << b[i+1] << std::endl;

    std::cout << "Second result is " << spin_after_0(input, 50'000'000) << std::endl;
    return 0;
}
