#include "include/fact_head.hpp"

/*
* 主函数
*/
int main()
{
    int n;
    int n_fact;
    cout << "请输入一个正整数：";
    cin >> n;
    cout << "该数字的阶乘为：" << fact(n) << endl;
}