#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstddef>

class MyString {
public:
    // ========== 类型别名 ==========
    using size_type = std::size_t;
    using iterator = char*;
    using const_iterator = const char*;

    static const size_type npos = static_cast<size_type>(-1);

    // ========== 构造函数 ==========
    MyString();                                          // 默认构造
    MyString(const char* str);                           // C字符串构造
    MyString(size_type n, char c);                       // 填充构造
    MyString(const MyString& other);                     // 拷贝构造
    MyString(MyString&& other) noexcept;                 // 移动构造

    // 迭代器范围构造
    template<typename InputIt>
    MyString(InputIt first, InputIt last);

    // ========== 析构函数 ==========
    ~MyString();

    // ========== 赋值运算符 ==========
    MyString& operator=(const MyString& other);          // 拷贝赋值
    MyString& operator=(MyString&& other) noexcept;      // 移动赋值
    MyString& operator=(const char* str);                // C字符串赋值

    // ========== 容量操作 ==========
    size_type size() const;
    size_type length() const;
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type new_cap);
    void resize(size_type new_size, char c = '\0');

    // ========== 元素访问 ==========
    char& operator[](size_type pos);
    const char& operator[](size_type pos) const;
    char& at(size_type pos);
    const char& at(size_type pos) const;
    char& front();
    const char& front() const;
    char& back();
    const char& back() const;
    const char* c_str() const;
    const char* data() const;

    // ========== 迭代器 ==========
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    // ========== 修改操作 ==========
    void push_back(char c);
    MyString& append(const MyString& str);
    MyString& append(const char* str);
    MyString& insert(size_type pos, const MyString& str);
    MyString& erase(size_type pos = 0, size_type len = npos);
    void clear();

    // ========== 查找 ==========
    size_type find(const MyString& str, size_type pos = 0) const;
    size_type find(char c, size_type pos = 0) const;
    MyString substr(size_type pos = 0, size_type len = npos) const;

    // ========== 运算符重载 ==========
    MyString& operator+=(const MyString& str);
    MyString& operator+=(const char* str);
    MyString& operator+=(char c);
    void swap(MyString& other) noexcept;

private:
    char* data_;          // 字符数据
    size_type size_;      // 当前长度
    size_type capacity_;  // 当前容量
};

// ========== 非成员运算符 ==========
MyString operator+(const MyString& lhs, const MyString& rhs);
MyString operator+(const MyString& lhs, const char* rhs);
MyString operator+(const char* lhs, const MyString& rhs);

bool operator==(const MyString& lhs, const MyString& rhs);
bool operator!=(const MyString& lhs, const MyString& rhs);
bool operator<(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& os, const MyString& str);
std::istream& operator>>(std::istream& is, MyString& str);
std::istream& getline(std::istream& is, MyString& str, char delim = '\n');

// ========== 模板成员函数实现（需在头文件中） ==========
template<typename InputIt>
MyString::MyString(InputIt first, InputIt last) {
    size_ = static_cast<size_type>(std::distance(first, last));
    capacity_ = size_;
    data_ = new char[capacity_ + 1];
    std::copy(first, last, data_);
    data_[size_] = '\0';
}

#endif // MYSTRING_H