#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


/*
* 主函数
*/
int main()
{
    vector<int> vInt;
    int i;
    char cont = 'y';
    while (cin >> i)
    {
        vInt.push_back(i);
        cout << "是否继续输入（y or n）：";
        cin >> cont;
        if (cont != 'y' && cont != 'Y')
        {
            break;
        }
    }

    for (auto mem : vInt)
    {
        cout << mem << " ";
    }
    cout << endl;
    return 0;
}
