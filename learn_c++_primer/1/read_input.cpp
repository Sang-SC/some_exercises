#include <iostream>

int main()
{
    int sum = 0, value = 0;
    // 持续读取数据，遇到文件结束符（Linux 下是 ctrl + D），或无效输入时结束
    while (std::cin >> value)
    {
        sum += value;
    }
    std::cout << "Sum is " << sum << std::endl;
    return 0;
}