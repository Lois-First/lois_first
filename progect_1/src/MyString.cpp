#include "MyString.h"
#include <cstring>
#include <algorithm>
#include <stdexcept>

// ========== 构造函数 ==========

MyString::MyString() : data_(new char[1]), size_(0), capacity_(0) {
    data_[0] = '\0';
}

MyString::MyString(const char* str) {
    if (str == nullptr) {
        data_ = new char[1];
        data_[0] = '\0';
        size_ = 0;
        capacity_ = 0;
    }
    else {
        size_ = std::strlen(str);
        capacity_ = size_;
        data_ = new char[capacity_ + 1];
        std::strcpy(data_, str);
    }
}

MyString::MyString(size_type n, char c) {
    size_ = n;
    capacity_ = n;
    data_ = new char[capacity_ + 1];
    std::fill(data_, data_ + size_, c);
    data_[size_] = '\0';
}

MyString::MyString(const MyString& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new char[capacity_ + 1];
    std::strcpy(data_, other.data_);
}

MyString::MyString(MyString&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// ========== 析构函数 ==========

MyString::~MyString() {
    delete[] data_;
}

// ========== 赋值运算符 ==========

MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        MyString tmp(other);
        std::swap(data_, tmp.data_);
        std::swap(size_, tmp.size_);
        std::swap(capacity_, tmp.capacity_);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

MyString& MyString::operator=(const char* str) {
    MyString tmp(str);
    std::swap(data_, tmp.data_);
    std::swap(size_, tmp.size_);
    std::swap(capacity_, tmp.capacity_);
    return *this;
}

// ========== 容量操作 ==========

MyString::size_type MyString::size() const { return size_; }
MyString::size_type MyString::length() const { return size_; }
MyString::size_type MyString::capacity() const { return capacity_; }
bool MyString::empty() const { return size_ == 0; }

void MyString::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        char* new_data = new char[new_cap + 1];
        std::strcpy(new_data, data_);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }
}

void MyString::resize(size_type new_size, char c) {
    if (new_size > size_) {
        reserve(new_size);
        std::fill(data_ + size_, data_ + new_size, c);
    }
    size_ = new_size;
    data_[size_] = '\0';
}

// ========== 元素访问 ==========

char& MyString::operator[](size_type pos) { return data_[pos]; }
const char& MyString::operator[](size_type pos) const { return data_[pos]; }

char& MyString::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("MyString::at: pos out of range");
    }
    return data_[pos];
}

const char& MyString::at(size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("MyString::at: pos out of range");
    }
    return data_[pos];
}

char& MyString::front() { return data_[0]; }
const char& MyString::front() const { return data_[0]; }

char& MyString::back() { return data_[size_ - 1]; }
const char& MyString::back() const { return data_[size_ - 1]; }

const char* MyString::c_str() const { return data_; }
const char* MyString::data() const { return data_; }

// ========== 迭代器 ==========

MyString::iterator MyString::begin() { return data_; }
MyString::iterator MyString::end() { return data_ + size_; }
MyString::const_iterator MyString::begin() const { return data_; }
MyString::const_iterator MyString::end() const { return data_ + size_; }
MyString::const_iterator MyString::cbegin() const { return data_; }
MyString::const_iterator MyString::cend() const { return data_ + size_; }

// ========== 修改操作 ==========

void MyString::push_back(char c) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = c;
    data_[size_] = '\0';
}

MyString& MyString::append(const MyString& str) {
    reserve(size_ + str.size_);
    std::strcpy(data_ + size_, str.data_);
    size_ += str.size_;
    return *this;
}

MyString& MyString::append(const char* str) {
    size_type len = std::strlen(str);
    reserve(size_ + len);
    std::strcpy(data_ + size_, str);
    size_ += len;
    return *this;
}

MyString& MyString::insert(size_type pos, const MyString& str) {
    if (pos > size_) {
        throw std::out_of_range("MyString::insert: pos out of range");
    }
    reserve(size_ + str.size_);
    // 将 pos 之后的字符后移
    std::memmove(data_ + pos + str.size_, data_ + pos, size_ - pos + 1);
    // 插入新字符
    std::memcpy(data_ + pos, str.data_, str.size_);
    size_ += str.size_;
    return *this;
}

MyString& MyString::erase(size_type pos, size_type len) {
    if (pos > size_) {
        throw std::out_of_range("MyString::erase: pos out of range");
    }
    if (len == npos || pos + len > size_) {
        len = size_ - pos;
    }
    // 将后面的字符前移
    std::memmove(data_ + pos, data_ + pos + len, size_ - pos - len + 1);
    size_ -= len;
    return *this;
}

void MyString::clear() {
    size_ = 0;
    data_[0] = '\0';
}

// ========== 查找 ==========

MyString::size_type MyString::find(const MyString& str, size_type pos) const {
    if (pos > size_) return npos;
    const char* result = std::strstr(data_ + pos, str.data_);
    if (result == nullptr) return npos;
    return static_cast<size_type>(result - data_);
}

MyString::size_type MyString::find(char c, size_type pos) const {
    for (size_type i = pos; i < size_; ++i) {
        if (data_[i] == c) return i;
    }
    return npos;
}

MyString MyString::substr(size_type pos, size_type len) const {
    if (pos > size_) {
        throw std::out_of_range("MyString::substr: pos out of range");
    }
    if (len == npos || pos + len > size_) {
        len = size_ - pos;
    }
    return MyString(data_ + pos, data_ + pos + len);
}

// ========== 运算符重载 ==========

MyString& MyString::operator+=(const MyString& str) {
    return append(str);
}

MyString& MyString::operator+=(const char* str) {
    return append(str);
}

MyString& MyString::operator+=(char c) {
    push_back(c);
    return *this;
}

void MyString::swap(MyString& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

// ========== 非成员运算符 ==========

MyString operator+(const MyString& lhs, const MyString& rhs) {
    MyString result(lhs);
    result += rhs;
    return result;
}

MyString operator+(const MyString& lhs, const char* rhs) {
    MyString result(lhs);
    result += rhs;
    return result;
}

MyString operator+(const char* lhs, const MyString& rhs) {
    MyString result(lhs);
    result += rhs;
    return result;
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    return std::strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
    return !(lhs == rhs);
}

bool operator<(const MyString& lhs, const MyString& rhs) {
    return std::strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator>(const MyString& lhs, const MyString& rhs) {
    return rhs < lhs;
}

bool operator<=(const MyString& lhs, const MyString& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const MyString& lhs, const MyString& rhs) {
    return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
    os << str.c_str();
    return os;
}

std::istream& operator>>(std::istream& is, MyString& str) {
    str.clear();
    char c;
    while (is.get(c) && !std::isspace(static_cast<unsigned char>(c))) {
        str.push_back(c);
    }
    return is;
}

std::istream& getline(std::istream& is, MyString& str, char delim) {
    str.clear();
    char c;
    while (is.get(c) && c != delim) {
        str.push_back(c);
    }
    return is;
}