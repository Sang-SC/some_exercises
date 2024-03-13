#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
* 函数作用：将 string 按字典序排列，并删除重复单词
*/
void elimDups(vector<string> &words)
{
    // 按字典序排序words，以便查找重复单词
    sort(words.begin(), words.end());
    // unique算法消除重复元素
    auto end_unique = unique(words.begin(), words.end());
    // 调整容器大小以匹配消除重复元素后的大小
    words.erase(end_unique, words.end());
}


/*
* 函数作用：根据给定的 ctr 值，判断 word 是否应该是复数形式
*/
string make_plural(size_t ctr, const string & word, const string &ending)
{
    return (ctr > 1) ? word + ending : word;
}


void biggies(vector<string> &words, vector<string>::size_type sz)
{
    elimDups(words);
    // 按长度排序，长度相同的单词维持字典序
    stable_sort(words.begin(), words.end(), [](const string &a, const string &b){ return a.size() < b.size();});  
    // 获取一个迭代器，指向第一个满足 size >= sz 的元素的数据
    auto wc = find_if(words.begin(), words.end(), [sz](const string &a){ return a.size() >= sz;});
    // 计算满足 size >= sz 的元素的数目
    auto count = words.end() - wc;
    cout << count << " " << make_plural(count, "word", "s") << " of length " << sz << " or longer" << endl;
    // 打印长度大于等于给定值的单词，每个单词后面接一个空格
    for_each(wc, words.end(), [](const string &s){cout << s << " ";});
    cout << endl;
}

int main()
{
    vector<string> v1;
    v1 = {"the", "quick", "quick", "red1", "fox", "jumps", "over", "the", "slow", "red", "turtle", "quick", "quick2"};
    biggies(v1, 3);
    return 0;
}