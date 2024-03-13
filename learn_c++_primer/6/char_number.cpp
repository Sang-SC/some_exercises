#include <iostream>

using std::cin;
using std::cout;
using std::endl;


/*
* 函数作用：统计自身被调用的次数
*/
unsigned myCnt()
{
    static unsigned cnt = 0;
    ++cnt;
    return cnt;
}


/*
* 主函数
*/
int main()
{
    cout << "请输入字符，按回车继续" << endl;
    char ch;
    while (cin >> ch)
    {
        cout << "您已输入的字符数量为：" << myCnt() << endl;
    }
    return 0;
}