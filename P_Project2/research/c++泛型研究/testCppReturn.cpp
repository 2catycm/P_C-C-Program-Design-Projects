#include <cstdio>
#include <stdexcept>
struct A{
    int a;
    int b;
};
A fun(){
    return A{1,2};
    return {1,2};//注意，两个return是OK的，编译器根本不理你
//    return {1,2};//都可以
}
int main(int argc, char **argv)
{
    A a = fun();
    printf("a = %d\n", a.a);
    printf("b = %d\n", a.b);
}