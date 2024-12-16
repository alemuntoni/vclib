module;

#include <iostream>

export module test_module; // module declaration

export void hello()       // export declaration
{
    std::cout << "Hello world!\n";
}
