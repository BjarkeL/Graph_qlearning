//#include "graph.h"
//#include "qlearning.h"
#include <iostream>
#include <iomanip>
#include "dynamicRegister.h"

int main() {

    DynReg test;
    DynReg test2;

    for (int i = 0; i < 15; i++) {
        test.add(1);
    }
    for (int i = 0; i < 15; i++) {
        test2.add(1);
    }

    std::cout << "Size of register: " << test.getSize() << std::endl;

    test2.clear(14);

    for (int i = 0; i < 15; i++) {
        std::cout << "Line: " << std::setw(2) << i << ": a * " << test.read(i) << ", b * " << test2.read(i) << std::endl;
    }

    std::cout << "a equal b? " << bool(test == test2) << std::endl;

    return 0;
}