#include <iostream>
#include <gmpxx.h>
//#include <gmp.h>
int main(int argc, char *argv[]){
    //preparation
    using namespace std;
    ios_base::sync_with_stdio(false);//make cin\cout faster //we don't need printf\scanf here.
    //we only want 2 arguments, or no argument from the command line.
    if (argc > 3){
        cout << "too many arguments." << endl;
        return -1;
    }
    mpz_class  integerA, integerB;
    switch (argc){
        case 2:
            cout << "too few argument." << endl;
            return -1;
        case 1:
            //if there is no argument, we ask for two integers.
            cout << "Please input two integers" << endl;
            cin >> integerA >> integerB;
            break;
        default://equally case 3
            //first convert c-style strings to cpp standard strings,
            //then we use strings to construct big integers.
            integerA = string(argv[1]);
            integerB = string(argv[2]);
            break;
    }
    cout <<integerA<<" * "<<integerB<<" = "<<integerA*integerB << endl;  // operator * has been overrided in class BigInt
    return 0;
}