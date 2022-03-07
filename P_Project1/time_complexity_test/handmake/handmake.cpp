//
// Created by 叶璨铭 on 2021/9/19.
//
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
string trivial(const string& integerA, const string& integerB){
    stringstream result;
    vector<int> data(integerA.length()+integerB.length()+1);
    for (int i = 0; i < integerA.length(); ++i) {
        for (int j = 0; j < integerB.length(); ++j) {
            data[i+j-1]+=(integerA[integerA.length()-i-1]-'0')*(integerB[integerA.length()-j-1]-'0');
        }
    }
    int carry = 0;
    int i;
    for (i = 0; i < data.size(); ++i) {
        data[i] += carry;
        carry = data[i]/10;
        data[i] %=10;
//        result<<data[i];
    }
    if (carry!=0) {
        data.push_back(carry);
//        result<<data[i];
    }
    for (int j = 0; j < data.size(); ++j) {
        result<<data[data.size()-j-1];
    }
    return result.str();
}
int main(){
    string integerA, integerB;
    cin >> integerA >> integerB;
    cout<<trivial(integerA, integerB)<<endl;
}
