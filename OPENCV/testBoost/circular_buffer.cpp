#include <iostream>
#include "boost/circular_buffer.hpp"

int main() {
    boost::circular_buffer<int> c(10);
    std::cout << c.size() << "\t" << c.capacity() << std::endl;//0   10

    return 0;
}
