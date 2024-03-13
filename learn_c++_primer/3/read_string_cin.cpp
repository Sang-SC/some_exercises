#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;


/*
* 主函数
*/
int main()
{
    string word;
    cout << "请输入字符串，不可以包含空格，文件结束符结束：";
    while (cin >> word)
    {
        cout << word << endl;
        cout << "请输入字符串，不可以包含空格，文件结束符结束：";
    }
    return 0;
}