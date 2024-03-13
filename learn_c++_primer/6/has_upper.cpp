#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*
* 函数功能：判断是否含有大写字母
*/
bool hasUpper(const string& str)
{
    for (auto c : str)
    {
        if (isupper(c))
        {
            return true;
        }
    }
    // 如果上面没有return，即没有大写字母，则return false
    return false;
}


/*
* 函数功能：将所有大写字母转换成小写
*/
void changeToLower(string str)
{
    for (auto &c : str)
    {
        c = tolower(c);
    }
}


/*
* 主函数
*/
int main()
{
    cout << "请输入一个字符串：";
    string str;
    cin >> str;
    if (hasUpper(str))
    {
        changeToLower(str);
        cout << "该字符串含有大写字母，全部转换为小写后结果为：" << str << endl;
    } else {
        cout << "该字符串不含大写字母，无需转换" << endl;
    }
    return 0;
}