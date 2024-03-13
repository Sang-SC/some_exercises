/*
* 文件功能：使用 ofstream（文件输出流）打开简单的二进制文件（Word文件这种不行，需要借助其他方法）以进行写入，然后使用 ifstream（文件输入流）打开文件以进行读取
*/
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // 创建一个二进制文件输出流对象，打开文件
    ofstream outfile("example.bin", ios::binary);

    // 写入数据到文件
    int data[] = {1, 2, 3, 4, 5};
    outfile.write((char*)data, sizeof(data));

    // 关闭文件输出流
    outfile.close();

    // 创建一个二进制文件输入流对象，打开文件
    ifstream infile("example.bin", ios::binary);

    // 从文件中读取数据
    int read_data[5];
    infile.read((char*)read_data, sizeof(read_data));

    // 输出读取到的数据
    for (int i = 0; i < 5; i++) {
        cout << read_data[i] << " ";
    }
    cout << endl;

    // 关闭文件输入流
    infile.close();

    return 0;
}