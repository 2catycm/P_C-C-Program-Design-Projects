#include <iostream>
#include "BigInt.hpp"
int main(int argc, char** argv){
    using namespace std;
    ios_base::sync_with_stdio(false);
    BigInt x = 1;
    for (int i = 0; i < 1000; i++)
    {
        x*=(i+1);
    }
    cout<<x;
    return 0;
}