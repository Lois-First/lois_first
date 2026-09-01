# C++20 Ranges 详解

## 1. Ranges 是什么

Ranges（范围库）是 C++20 对迭代器和算法的一次系统升级。它不是一个新的容器，而是一套描述“可遍历序列”的概念、视图和算法接口：

- **range（范围）**：能够通过 `begin(r)` 和 `end(r)` 取得遍历边界的对象。
- **iterator（迭代器）**：指向元素并负责递增、解引用。
- **sentinel（哨兵）**：表示结束位置，不要求和迭代器是同一种类型。
- **view（视图）**：轻量、通常不拥有数据的范围适配器，按需计算元素。
- **range algorithm**：位于 `std::ranges`，直接接收一个范围，不必手写 `begin/end`。
- **concept（概念）**：在编译期检查范围和元素是否满足算法要求，错误信息比传统模板更明确。

传统写法：

```cpp
#include <algorithm>
#include <concepts>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

std::sort(v.begin(), v.end());
auto it = std::find_if(v.begin(), v.end(), pred);
```

C++20 写法：

```cpp
std::ranges::sort(v);
auto it = std::ranges::find_if(v, pred);
```

## 2. 为什么需要 Ranges

### 2.1 减少边界错误

算法接收一个范围对象，起止边界天然成对，不会误把别的容器的 `end()` 传进来。

### 2.2 算法可组合

`views` 通过 `|` 连接成惰性流水线：过滤、转换、截取可以组合而不创建中间容器。

### 2.3 约束模板接口

`std::ranges::range_value_t<R>`、`std::ranges::range_reference_t<R>` 等工具统一推导范围元素类型；概念约束能在调用点报告“不满足可排序/可迭代”等原因。

### 2.4 支持不同结束类型

输入流、哨兵终止的 C 字符串等序列，不必为“结束位置”构造一个和迭代器同型的值。

## 3. 基本使用

### 3.1 头文件和命名空间

```cpp
#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
namespace rv = std::ranges::views;
```

### 3.2 直接把容器传给算法

```cpp
int count_positive(const std::vector<int> &values) {
    return static_cast<int>(std::ranges::count_if(
        values, [](int x) { return x > 0; }));
}

void normalize(std::vector<int> &values) {
    std::ranges::sort(values);
    auto last = std::ranges::unique(values).begin();
    values.erase(last, values.end());
}
```

更安全、可读的去重写法是：

```cpp
auto last = std::ranges::unique(values).begin();
values.erase(last, values.end());
```

实际项目中不要重复调用可能有副作用的算法；先保存返回结果。

### 3.3 范围算法常用函数

| 函数 | 作用与返回值 |
|---|---|
| `ranges::find(r, value)` | 查找值，返回迭代器 |
| `ranges::find_if(r, pred)` | 按谓词查找 |
| `ranges::contains(r, value)` | **C++23**，C++20 不可用；C++20 用 `find != end` |
| `ranges::count/count_if` | 计数，返回数量 |
| `ranges::all_of/any_of/none_of` | 判断所有/任一/无元素满足谓词 |
| `ranges::copy/copy_if` | 复制到目标输出迭代器 |
| `ranges::transform` | 映射元素，可写入另一个范围 |
| `ranges::remove/remove_if` | 移动待删除元素到逻辑尾，返回 `subrange` |
| `ranges::sort/stable_sort` | 排序，要求随机访问和可排序元素 |
| `ranges::reverse/rotate` | 反转/旋转元素 |
| `ranges::min/max/minmax` | 返回值或迭代器；范围为空时要遵守前置条件 |
| `ranges::lower_bound/upper_bound` | 有序范围二分查找 |
| `ranges::fold_left` | **C++23**；C++20 用 `std::accumulate` |

注意：表中标出的 C++23 函数不能在 C++20 模式下使用。

## 4. Views：惰性范围适配器

View 通常只保存底层范围的引用、迭代器或状态。创建 view 几乎不做工作，遍历时才计算；view 一般不可拷贝底层元素，也不拥有外部容器的生命周期。

### 4.1 `filter`

只暴露满足谓词的元素。

```cpp
std::vector<int> values{1, 2, 3, 4, 5, 6};
auto even = values | rv::filter([](int x) { return x % 2 == 0; });
for (int x : even) std::cout << x << ' '; // 2 4 6
```

### 4.2 `transform`

按函数将每个元素映射为另一个值；结果通常是代理引用，不能假设它是容器。

```cpp
auto squares = values | rv::transform([](int x) { return x * x; });
```

### 4.3 `take`、`drop`、`take_while`、`drop_while`

```cpp
auto first_three = values | rv::take(3);
auto after_two = values | rv::drop(2);
auto prefix = values | rv::take_while([](int x) { return x < 5; });
```

### 4.4 `reverse`、`common`、`common` 的用途

`views::reverse(r)` 反向遍历，要求底层范围满足双向遍历；`views::common(r)` 把不同类型的迭代器/哨兵包装成同类型迭代器，方便传给旧式算法。

```cpp
auto reversed = values | rv::reverse;
std::sort(rv::common(reversed).begin(), rv::common(reversed).end());
```

一般优先使用 `std::ranges` 算法，只有必须调用旧接口时才需要 `common`。

### 4.5 `keys`、`values`、`elements`

从 pair-like 元素中投影某个成员，常用于 map。

```cpp
std::map<std::string, int> scores{{"A", 1}, {"B", 2}};
for (const auto &key : scores | rv::keys) std::cout << key;
for (int score : scores | rv::values) std::cout << score;
```

### 4.6 `iota`、`istream`、`single`、`empty`

```cpp
auto numbers = rv::iota(1, 10);       // 1..9，惰性生成
auto input = rv::istream<int>(std::cin); // 从流逐个读取
auto one = rv::single(42);
auto none = rv::empty<int>;
```

`iota(value)` 的单参数版本是无限范围，必须搭配 `take` 等终止适配器。

### 4.7 `split` 与 `join`

`views::split(delim)` 将字符序列拆成子范围；`views::join` 把嵌套范围展平一层。

```cpp
std::string text = "red,green,blue";
for (auto part : text | rv::split(',')) {
    for (char ch : part) std::cout << ch;
    std::cout << '\n';
}
```

`split` 的元素是子范围，不是 `std::string`；需要拥有字符串时应显式构造 `std::string(part.begin(), part.end())`。

### 4.8 `filter` 与 `transform` 管道

```cpp
auto result = values
    | rv::filter([](int x) { return x % 2 == 0; })
    | rv::transform([](int x) { return x * 10; })
    | rv::take(2);
for (int x : result) std::cout << x << ' '; // 20 40
```

管道不产生 `vector`。若 API 需要拥有结果，显式收集：

```cpp
std::vector<int> materialize(std::ranges::input_range auto r) {
    return {r.begin(), r.end()};
}
```

## 5. Concepts：范围接口的约束

### 5.1 常用范围概念

| 概念 | 要求 |
|---|---|
| `range<R>` | `begin/end` 可用 |
| `input_range<R>` | 可单次读取遍历 |
| `forward_range<R>` | 可多次遍历 |
| `bidirectional_range<R>` | 支持 `--it` |
| `random_access_range<R>` | 支持跳跃和下标差值 |
| `contiguous_range<R>` | 元素连续存储 |
| `common_range<R>` | `begin` 与 `end` 类型相同 |
| `sized_range<R>` | 可 O(1) 得到大小 |
| `borrowed_range<R>` | 从临时范围取得的迭代器仍可安全使用 |
| `view<R>` | 轻量、可复制的范围对象 |

### 5.2 用概念约束自己的函数

```cpp
template<std::ranges::input_range R>
requires std::integral<std::ranges::range_value_t<R>>
auto sum(R &&r) {
    using value_type = std::ranges::range_value_t<R>;
    value_type total{};
    for (auto &&value : r) total += value;
    return total;
}
```

也可以写成简写模板：

```cpp
auto print_all(std::ranges::input_range auto &&r) {
    for (auto &&x : r) std::cout << x << ' ';
}
```

## 6. Range algorithms 的投影（projection）

许多算法最后一个可调用参数是投影函数。它允许按对象的某个字段排序/查找，而不必为每次比较手写 Lambda。

```cpp
struct User { std::string name; int age; };
std::vector<User> users{{"Tom", 30}, {"Ann", 20}};

std::ranges::sort(users, std::ranges::less{}, &User::age);
auto it = std::ranges::find(users, 30, &User::age);
```

投影先应用，再交给比较器或谓词：`comp(proj(element))`。成员指针、Lambda 和普通函数都可以作为投影。

## 7. 自定义 Range

### 7.1 最小要求

只要 `std::ranges::begin(x)` 和 `std::ranges::end(x)` 可用，类型就可能是 range。最简单方式是在类中提供成员 `begin/end`。

```cpp
class IntBuffer {
public:
    int *begin() { return data_; }
    int *end() { return data_ + 3; }
    const int *begin() const { return data_; }
    const int *end() const { return data_ + 3; }
private:
    int data_[3]{1, 2, 3};
};
```

### 7.2 哨兵

迭代器和哨兵只需支持 `iterator == sentinel` 的比较。这样可表达以 `\0` 结束的字符串或流结束状态。

```cpp
struct cstr_sentinel {};
struct cstr_iterator {
    const char *p;
    char operator*() const { return *p; }
    cstr_iterator &operator++() { ++p; return *this; }
};
bool operator==(cstr_iterator it, cstr_sentinel) { return *it.p == '\0'; }
bool operator==(cstr_sentinel s, cstr_iterator it) { return it == s; }
```

完整自定义范围还应满足对应 iterator concept 的递增、解引用、比较和 `iter_move` 等要求；复杂类型建议参考标准库 iterator customization 规则并配合 `static_assert(std::ranges::range<T>)` 验证。

## 8. 生命周期、悬空迭代器与常见陷阱

### 8.1 View 不拥有数据

```cpp
auto bad() {
    return std::string{"temporary"} | rv::drop(1); // view 指向即将销毁的 string
}
```

返回 view 前必须保证底层数据仍存活；需要跨作用域保存时，返回拥有数据的容器，或把容器移动进 `views::all` 支持的 owning view（仅在明确设计生命周期时使用）。

### 8.2 临时范围与 `borrowed_range`

从临时容器调用范围算法时，算法通常返回 `std::ranges::dangling`，防止误用已经失效的迭代器。`std::ranges::borrowed_range<R>` 表示即使 `R` 是右值，返回的迭代器也安全（例如 `std::span`、`subrange`）。

```cpp
auto result = std::ranges::find(std::vector<int>{1, 2, 3}, 2);
// result 是 dangling，不能解引用
```

### 8.3 代理引用

`vector<bool>`、`zip` 类视图（C++23）等元素可能是代理对象而非真正的 `T&`。泛型代码应使用 `range_reference_t<R>`，不要写死 `T&`。

### 8.4 其他陷阱

- 无限 view 必须用 `take`、`take_while` 等方式截断，否则遍历不会结束。
- `filter` 视图的谓词应保持稳定；遍历期间改变底层元素使元素是否满足条件发生变化会导致难以理解的结果。
- view 的多次遍历能力取决于底层范围；输入范围只能遍历一次。
- `views::transform` 的结果不能自动当成 `vector` 传给需要连续存储的 API。
- C++20 没有 `views::zip`、`views::enumerate`、`ranges::to`、`fold_left`；这些是 C++23 能力，需使用手写循环或第三方库替代。

## 9. 与传统算法的迁移步骤

1. 把 `container.begin(), container.end()` 改为直接传 `container`。
2. 把多个预处理循环改成 `filter | transform | take` 管道。
3. 用 `ranges::sort(range, comparator, projection)` 表达按字段排序。
4. 对接旧 API 时使用 `std::ranges::begin/end` 或 `views::common`。
5. 检查返回迭代器是否可能为 `dangling`，确认 view 的底层对象生命周期。
6. 用概念约束公共模板接口，并用 `static_assert` 验证自定义范围模型。

## 10. 一页速查

```cpp
template<std::ranges::input_range R>
auto positive_squares(R &&r) {
    return std::forward<R>(r)
        | std::views::filter([](auto x) { return x > 0; })
        | std::views::transform([](auto x) { return x * x; });
}

int main() {
    std::vector<int> data{-2, 0, 3, 4};
    for (auto x : positive_squares(data)) std::cout << x << ' '; // 9 16
    std::ranges::sort(data);
}
```

核心原则是：范围描述“从哪里到哪里”，view 描述“如何观察”，算法描述“对观察到的元素做什么”，concept 则在编译期验证三者是否兼容。
