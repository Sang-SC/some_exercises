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
    string line;
    cout << "请输入字符串，可以包含空格，文件结束符结束：";
    while (getline(cin, line))
    {
        cout << line << endl;
        cout << "请输入字符串，可以包含空格，文件结束符结束：";
    }
    return 0;
}