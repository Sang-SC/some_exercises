/*
* 文件功能：使用 ofstream（文件输出流）打开文本文件以进行写入，然后使用 ifstream（文件输入流）打开文件以进行读取
*/

#include <iostream>
#include <fstream>
// #include <string>

int main()
{
    std::string filename = "fstream_demo.txt";

    // 打开文件，没有文件则会创建一个
    std::ofstream outfile(filename);
    
    // std::ofstream outfile(filename, std::ofstream::app);  // 增加选项app，则不会清空原有内容

    if (!outfile)
    {
        std::cerr << "无法打开文件 " << filename << " 进行写入！" << std::endl;
        return 1;
    }

    outfile << "Hello world!" << std::endl;
    outfile << "This is an example file." << std::endl;

    outfile.close();

    std::ifstream infile(filename);

    if (!infile)
    {
        std::cerr << "无法打开文件 " << filename << " 进行读取！" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }
 
    infile.close();

    return 0;
}