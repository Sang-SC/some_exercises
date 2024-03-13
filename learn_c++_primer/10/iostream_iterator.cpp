/*
* 文件功能：测试 iostream 迭代器
*/
#include <iterator>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> vec;

    // 从标准输入中读取整数，并将其存储到容器
    std::istream_iterator<int> inputIterator(std::cin);
    std::istream_iterator<int> endIterator;      // 默认初始化迭代器，可以当作后置迭代器使用

    while (inputIterator != endIterator) {
        vec.push_back(*inputIterator);
        ++inputIterator;
    }

    // 输出容器内容
    std::ostream_iterator<int> outputIterator(std::cout, "; ");
    std::copy(vec.begin(), vec.end(), outputIterator);  // 将源容器中的元素复制到目标容器中
    std::cout << std::endl;

    return 0;
}