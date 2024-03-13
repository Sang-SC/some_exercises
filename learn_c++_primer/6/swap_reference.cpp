#include <iostream>

using std::cin;
using std::cout;
using std::endl;

/*
* 函数功能：交换两个数
*/
void mySwap(int &i, int &j)
{
    int tem = i;
    i = j;
    j = tem;
}


/*
* 函数功能：主函数
*/
int main()
{
    int a = 123, b = 789;
    cout << "交换前：a = " << a << ", b = " << b << endl;
    mySwap(a, b);
    cout << "交换后：a = " << a << ", b = " << b << endl;
}