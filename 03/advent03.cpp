#include <spiralsquare.hpp>

#include <iostream>


int main()
{
    int const input = 312051;

    Field f(getRing(input));
    f.fillField();
    std::cout << "First result is " << f.getDistanceFromCenter(input) << std::endl;

    for(int dim=3; ; dim+=2) {
        Field f2(dim);
        int result2 = f2.fillComplicated(input);
        if(result2) {
            std::cout << "Second result is " << result2 << std::endl;
            break;
        }
    }

    return 0;
}
