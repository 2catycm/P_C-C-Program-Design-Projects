//
// Created by 叶璨铭 on 2021/9/25.
//
#include <iostream>
using std::string;
using std::cout;
template <typename T>
inline T const& Max (T const& a, T const& b)
{
    return a < b ? b:a;
}
struct intstring{
    int i;
    string s;
};
template<typename E> int IdontknowType(){
//    cout<<E;
    cout<<"鬼知道这是什么类型哟"<<std::endl;
}
template<typename E> IdontknowType(E type){ //不写返回类型，自动变成返回int
//    cout<<E;
    cout<<type<<std::endl;
}
int main(int argc, char **argv){
    //第一点，没有控制类型
    intstring not_compareble_a = {1,"哈哈哈"};
    intstring not_compareble_b = {2,"嘿嘿，小猫雨伞"};
//    cout<<
//    Max(not_compareble_a,not_compareble_b);// 翻车了吧，

    //第二点，能不能自动推断类型传递？
//    IdontknowType();

    IdontknowType(1.0);
}