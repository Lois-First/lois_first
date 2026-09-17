#include "MyString.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    // 1. 构造函数测试
    cout << "=== 构造函数测试 ===" << endl;
    MyString s1;
    MyString s2("Hello");
    MyString s3(5, 'A');
    MyString s4(s2);
    MyString s5(std::move(s2));
    MyString s6(s3.begin(), s3.end());

    cout << "s1: \"" << s1 << "\"" << endl;
    cout << "s3: \"" << s3 << "\"" << endl;
    cout << "s4: \"" << s4 << "\"" << endl;
    cout << "s5: \"" << s5 << "\"" << endl;
    cout << "s6: \"" << s6 << "\"" << endl;

    // 2. 赋值测试
    cout << "\n=== 赋值测试 ===" << endl;
    MyString a, b, c;
    a = "Hello";
    b = a;
    c = std::move(a);
    cout << "a: \"" << a << "\"" << endl;
    cout << "b: \"" << b << "\"" << endl;
    cout << "c: \"" << c << "\"" << endl;

    // 3. 容量测试
    cout << "\n=== 容量测试 ===" << endl;
    MyString cap("Hello");
    cout << "size: " << cap.size() << endl;
    cout << "capacity: " << cap.capacity() << endl;
    cout << "empty: " << cap.empty() << endl;

    cap.reserve(100);
    cout << "reserve(100) 后 capacity: " << cap.capacity() << endl;

    cap.resize(10, '!');
    cout << "resize(10, '!') 后: \"" << cap << "\"" << endl;

    // 4. 访问测试
    cout << "\n=== 访问测试 ===" << endl;
    MyString acc("World");
    cout << "acc[0]: " << acc[0] << endl;
    cout << "acc.at(1): " << acc.at(1) << endl;
    cout << "acc.front(): " << acc.front() << endl;
    cout << "acc.back(): " << acc.back() << endl;
    cout << "acc.c_str(): " << acc.c_str() << endl;

    // 5. 修改测试
    cout << "\n=== 修改测试 ===" << endl;
    MyString mod("Hi");
    mod.push_back('!');
    cout << "push_back('!'): \"" << mod << "\"" << endl;

    mod.append(" there");
    cout << "append(\" there\"): \"" << mod << "\"" << endl;

    mod.insert(2, " C++");
    cout << "insert(2, \" C++\"): \"" << mod << "\"" << endl;

    mod.erase(0, 3);
    cout << "erase(0, 3): \"" << mod << "\"" << endl;

    // 6. 查找测试
    cout << "\n=== 查找测试 ===" << endl;
    MyString findStr("Hello World Hello");
    cout << "find(\"World\"): " << findStr.find("World") << endl;
    cout << "find('o'): " << findStr.find('o') << endl;
    cout << "substr(6, 5): \"" << findStr.substr(6, 5) << "\"" << endl;

    // 7. 运算符测试
    cout << "\n=== 运算符测试 ===" << endl;
    MyString op1("Hello"), op2(" World");
    MyString op3 = op1 + op2;
    cout << "op1 + op2: \"" << op3 << "\"" << endl;

    op3 += "!";
    cout << "op3 += \"!\": \"" << op3 << "\"" << endl;

    cout << "op1 == op2: " << (op1 == op2) << endl;
    cout << "op1 < op2: " << (op1 < op2) << endl;

    // 8. 迭代器测试
    cout << "\n=== 迭代器测试 ===" << endl;
    MyString iter("Hello");
    cout << "遍历: ";
    for (auto it = iter.begin(); it != iter.end(); ++it) {
        cout << *it;
    }
    cout << endl;

    // 9. 范围 for 循环
    cout << "范围 for: ";
    for (char c : iter) {
        cout << c;
    }
    cout << endl;

    // 10. 输入测试
    cout << "\n=== 输入测试 ===" << endl;
    MyString input;
    cout << "请输入一个单词: ";
    cin >> input;
    cout << "你输入了: \"" << input << "\"" << endl;

    cin.ignore();  // 清除缓冲区中的换行符
    MyString line;
    cout << "请输入一行文字: ";
    getline(cin, line);
    cout << "你输入了: \"" << line << "\"" << endl;

    return 0;
}