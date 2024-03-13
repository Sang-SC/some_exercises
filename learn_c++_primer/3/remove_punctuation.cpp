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
    string str;
    cout << "输入一个字符串，程序将剔除其中的标点符号：";
    getline(cin, str);
    // cin >> str;
    for (auto c : str)
    {
        if (!ispunct(c))
        {
            cout << c;
        }
    }
    cout << endl;
    return 0;
}