/*
* 文件功能：测试三种插入迭代器
*/


#include <iostream>
#include <vector>
#include <list>
#include <iterator> // 包含插入迭代器所需的头文件
#include <algorithm>

int main() {
    // ---------------插入迭代器类型一---------------
    std::vector<int> vec;  // vector支持push_back操作
    std::back_insert_iterator<std::vector<int>> backIter(vec);
    *backIter = 10;
    ++backIter;
    *backIter = 20;
    ++backIter;
    *backIter = 30;
    ++backIter;
    *backIter = 40;
    ++backIter;

    // 容器不能直接打印
    std::cout << "vec = ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // ---------------插入迭代器类型二---------------
    std::list<int> myList;  // vector不支持push_front操作，因此也不能使用front_insert_iterator，list支持push_front 
    std::front_insert_iterator<std::list<int>> frontIter(myList); 
    *frontIter = 10;
    ++frontIter;
    *frontIter = 20;
    ++frontIter; 
    *frontIter = 30;
    ++frontIter;
    *frontIter = 40;
    ++frontIter;

    // 容器不能直接打印
    std::cout << "myList = ";
    for (const auto& element : myList) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    // ---------------插入迭代器类型三---------------
    std::insert_iterator<std::vector<int>> insertIter(vec, vec.begin() + 2);  // vector支持insert操作
    *insertIter = 100;
    ++insertIter;
    *insertIter = 200;
    ++insertIter;

    // 容器不能直接打印
    std::cout << "vec = ";
    for (const auto& element : vec) {
        std::cout << element << " ";
    }
    std::cout << "\n";

    return 0;
}