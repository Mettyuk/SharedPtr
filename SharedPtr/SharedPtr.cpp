#include <iostream>
#include "SharedPtr.hpp"
#include <memory>

int main()
{
    SharedPtr<int> cigany;
    cigany.MakeShared(5);

    cigany.Reset(new int(10));
    std::cout << "asd";
}
