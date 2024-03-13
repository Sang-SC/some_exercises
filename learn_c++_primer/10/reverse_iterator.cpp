#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>

int main()
{
    // 逆序打印 vec 的元素
    std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (auto iter = vec.crbegin(); iter != vec.crend(); ++iter)
    {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;


    // 在一个逗号分隔的列表中，查找最后一个单词并输出
    std::string line = "first,second,third";
    auto comma = std::find(line.crbegin(), line.crend(), ',');
    std::cout << std::string(line.crbegin(), comma) << std::endl;      // 这种方法打印了最后一个单词，但单词是倒序的
    std::cout << std::string(comma.base(), line.cend()) << std::endl;  // 这种方法打印了最后一个单词，且单词是正序的，是通过将反向迭代器转换成普通迭代器


    // 使用 find 在一个 int 的 list 中查找最后一个值为0的元素
    std::list<int> myList = {1, 2, 3, 0, 4, 5, 10, 6, 10, 10, 10};
    // 容器不能直接打印
    std::cout << "myList = ";
    for (const auto& element : myList) {
        std::cout << element << " ";
    }
    std::cout << "\n";
    auto reverseIter = std::find(myList.rbegin(), myList.rend(), 0);
    if (reverseIter != myList.rend())
    {
        std::cout << "找到了 0 最后出现的位置，为 myList 中倒数第 " << std::distance(myList.rbegin(), reverseIter) + 1 << " 个元素" << std::endl;
    } else {
        std::cout << "没找到0" << std::endl;
    }



    return 0;
}