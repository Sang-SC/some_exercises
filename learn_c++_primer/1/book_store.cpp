/*
* 读入书本数据（格式为 ISBN 码、数量、单价），输出统计结果（格式为： ISBN 码、总数量、总收入、总平均价格）
* 利用文件重定向，从文件中读取数据：./out <./data/book_sales
*/

#include <iostream>
#include "include/Sales_item.h"
int main()
{
    Sales_item total;  // 保存和的变量
    // 读入第一条交易记录，确保有数据处理
    if (std::cin >> total) {
        Sales_item trans;
        while (std::cin >> trans) {
            if (total.isbn() == trans.isbn())
                total += trans;
            else {
                std::cout << total << std::endl;
                total = trans;
            }
        }
        std::cout << total << std::endl;
    } else {
        std::cerr << "No data?!" << std::endl;
        return -1;
    }
    return 0;
}