// 文件编译方法：g++ -o out add_Sales_item.cpp -I ./include

#include <iostream>
#include "include/Sales_item.h"

int main()
{
    Sales_item item1, item2;
    std::cin >> item1 >> item2;
    std::cout << item1 + item2 << std::endl;
    return 0;
}