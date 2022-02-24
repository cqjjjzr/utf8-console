#include <iostream>
#include <iomanip>
#include <string>

#include "utf8_console.h"
using namespace std;

int main()
{
    initialize_utf8_console();

    std::string test_str1 = "测试 ｱｲｳｴｵ 😅🤣";
    std::string test_str2 = "テスト Россия";
    cout << "Test UTF-8 aligned output: " << endl
         << right << setw_u8(30, test_str1) << test_str1 << endl
         << right << setw_u8(30, test_str2) << test_str2 << endl
         << endl;

    cout << "Test UTF-8 input: Use EOF as end." << endl;
    std::string str;
    while (getline(cin, str))
        cout << str << endl;
    return 0;
}