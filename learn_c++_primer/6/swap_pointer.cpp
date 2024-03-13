#include <iostream>

using std::cin;
using std::cout;
using std::endl;

/*
* 函数功能：交换两个数
*/
void mySwap(int *p, int *q)
{
    int tem = *p;
    *p = *q;
    *q = tem;
}


/*
* 函数功能：主函数
*/
int main()
{
    int a = 123, b = 789;
    int *p = &a, *q = &b;
    cout << "交换前：a = " << a << ", b = " << b << endl;
    mySwap(p, q);
    cout << "交换后：a = " << a << ", b = " << b << endl;
    return 0;
}