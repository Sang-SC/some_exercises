#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;


/*
* 主函数
*/
int main(int argc, char **argv)
{
    string str;
    for (int i = 1; i != argc; ++i)
    {
        str += argv[i];
    }
    cout << str << endl;
    return 0;
}