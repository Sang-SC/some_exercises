#include <iostream>

using std::cin;
using std::cout;
using std::endl;

/*
* 函数功能：计算阶乘
*/
int fact(int n)
{
    int n_fact = 1;
    for (int i = n; i > 1; --i)
    {
        n_fact = n_fact * i;
    }
    return n_fact;
}


/*
* 主函数
*/
int main()
{
    int n;
    int n_fact;
    cout << "请输入一个正整数：";
    cin >> n;
    cout << "该数字的阶乘为：" << fact(n) << endl;;
}