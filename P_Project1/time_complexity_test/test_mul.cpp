//
// Created by 叶璨铭 on 2021/9/18.
//
#include <iostream>
#include <string>
#include <sstream>
#include "StopWatch.hpp"
#include "../BigInt.hpp"
using namespace std;
//const string tested_path = "pwsh -Command \"../mul.exe"; //pwsh指令
//    command <<tested_path<<" "<<val1<<" "<<val2<<" \"";
//stringstream command;
//command <<tested_path<<" "<<val1<<" "<<val2<<" |out-null \"";
//system(command.str().c_str());
//    cout <<command.str().c_str()<<endl;  //回显 debug
long long time_trail(const string& val1, const string& val2){
    StopWatch sw;
    for (int i = 0; i < 100; ++i) {
        BigInt(val1)*BigInt(val2);
    }
    return sw.elapsedMicroSecond();
}
void doubling_test(int most_testing_n){
    StopWatch sw; //最长测试时间
    for (int i = 1; i<=most_testing_n; i<<=1) {
        stringstream val_str;
        for (int j = 0; j < i; ++j) {
            val_str<<"9";
        }
//        cout<<"("<<i<<", "<<time_trail(val_str.str(), val_str.str())<<")"<<endl;
        cout<<i<<","<<time_trail(val_str.str(), val_str.str())<<endl;
    }
}
int main(int argc, char *argv[]){
//    ios_base::sync_with_stdio(false);
    doubling_test(1048576
    );
//    cout <<time_trail("123", "123")<<endl;
}

//        char* val = new char[i];
//        for (int j = 0; j < i; ++j) {
//            val[j] = '9';
//        }
//        delete val;
//    system( (tested_path+" "+val1+" "+val2+" >nul").c_str() );
