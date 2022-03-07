#include "../src/FloatMatrix.hpp"
using SUSTech::CSE::FloatMatrix;
int main(int argc, char const *argv[])
{
    FloatMatrix a(3,4,1);
    // FloatMatrix b(3,4,2);
    // FloatMatrix c = a + b;
    // c.print();
    // std::cout<<std::endl;
    // FloatMatrix d = a*2;
    // d.print();
    // std::cout<<std::endl;

    // a.print();
    // std::cout<<std::endl;

    a = a;

    return 0;
}
