# 现代 C++ 字符串实现的两种方案

> 本文档讲解项目中 `include/string_cpp_1.h`（+ `src/common/string_cpp_1.cpp`）与 `include/string_cpp_2.h`（+ `src/common/string_cpp_2.cpp`）两套现代 C++ 字符串方案。
> 面向对现代 C++（C++20）语法不太熟悉的读者，第 1 章为前置语法讲解，之后逐 API 分析两套方案。

---

## 目录

1. [前置语法知识](#1-前置语法知识)
2. [方案一：ModernString1 —— 基于 SSO 的统一 String 类](#2-方案一modernstring1--基于-sso-的统一-string-类)
3. [方案二：ModernString2 —— Concept + 模板泛型](#3-方案二modernstring2--concept--模板泛型)
4. [两种方案对比](#4-两种方案对比)
5. [API 速查表](#5-api-速查表)
6. [使用示例](#6-使用示例)
7. [复杂度与注意事项](#7-复杂度与注意事项)

---

## 1. 前置语法知识

以下概念全部取自这两个文件中用到的语法，逐条讲解。

### 1.1 运算符重载（Operator Overloading）

**含义**：C++ 允许你为一个自定义类型定义 `+ - * / [] == <<` 等运算符的行为，方式是定义一个名字叫 `operator 符号` 的函数。这样写 `s1 + s2`、`str[i]` 就变成了对自定义函数的调用。

本项目中出现的运算符：

| 运算符 | 声明（节选） | 作用 |
|---|---|---|
| `operator[]` | `char& operator[](std::size_t i) noexcept` | 下标访问：让 `str[0]` 合法。返回**引用**意味着可以写 `str[0] = 'A'` |
| `operator=` | `String& operator=(const String& other)` | 赋值：深拷贝/移动右侧对象 |
| `operator+=` | `String& operator+=(std::string_view sv)` | 就地追加 |
| `operator+` | `friend String operator+(const String& lhs, std::string_view rhs)` | 拼接，返回**新对象**（不修改 lhs） |
| `operator==` | `bool operator==(const String& other) const noexcept` | 相等比较 |
| `operator<=>` | `auto operator<=>(const String& other) const noexcept` | **C++20 三路比较（宇宙飞船运算符）** |
| `operator<<` | `std::ostream& operator<<(std::ostream& os, const String& str)` | 输出：让 `std::cout << s` 合法 |
| `operator string_view()` | `operator std::string_view() const noexcept` | **类型转换运算符**：对象可隐式转换为 `std::string_view` |

几个要点：

- **为什么要重载运算符**：C++ 内置运算符只对内置类型（`int`、指针等）有效。想让自定义类也支持 `[]`、`+`、`==`、`cout <<` 这种自然写法，就必须重载。
- **`operator[]` 返回引用**：`char&` 表示返回的是字符串内部的一个字符的"别名"，因此可以赋值（左值）。若只返回 `char`，`str[0] = 'A'` 会编译失败。
- **`operator+` 为何写成 `friend` 自由函数**：普通成员函数形式是 `lhs.operator+(rhs)`，左操作数只能是类自身；自由函数可以更灵活地控制左右操作数类型，且 `friend` 让自由函数能访问私有成员。本项目里它其实只用到公有的 `+=` 和拷贝构造，`friend` 是锦上添花。
- **`operator+` 与 `operator+=` 的分工**：`+=` 修改自身、复用已有容量；`+` 先拷贝一份再 `+=`，返回新对象。这是 STL 的经典做法，避免重复实现逻辑。
- **类型转换运算符**：`operator std::string_view() const` 允许 `String` 被隐式转换成 `std::string_view`。例如 `std::string_view(str)`、`sv1.compare(sv2)` 里传 `String` 对象时都能自动转换。它是本项目一切泛型操作的基础。
- **`operator<=>`（C++20 三路比较）**：一次比较得到三种关系（小于/等于/大于）。只要定义了它和 `operator==`，C++ 编译器就能**自动改写**出 `<`、`<=`、`>`、`>=`。所以即使代码里没写 `operator<`，写 `if (s1 < s2)` 也是合法的——编译器把它改写为 `(s1 <=> s2) < 0`。

### 1.2 移动语义（Move Semantics）

**背景**：拷贝（copy）是深拷贝——把数据完整复制一份，源对象不变。但有些情况下对象马上就要消亡（如函数返回的临时对象、`std::move(a)` 后的对象），深拷贝纯属浪费，尤其是复制一块堆内存。

**移动（move）**：不复制数据，而是把资源（如指向堆内存的指针）**偷过来**，再把源对象置为空（但仍处于合法可析构的状态）。移动构造/移动赋值的形参写法是 `T&&`（右值引用），表示"这个对象是临时值，允许你洗劫它"。

本项目中的移动构造：

```cpp
String::String(String&& other) noexcept {
    m_length  = other.m_length;
    m_capacity = other.m_capacity;
    if (other.is_sso()) {
        // SSO 模式：缓冲区嵌在对象内部，无法"偷"，只能整块拷走（≤16 字节，很便宜）
        std::memcpy(m_storage.sso_buf, other.m_storage.sso_buf, m_length + 1);
    } else {
        // 堆模式：把堆指针偷过来
        m_storage.heap = other.m_storage.heap;
        other.m_capacity = SSO_CAPACITY;
        other.m_storage.sso_buf[0] = '\0';
    }
    other.m_length = 0;   // 源对象被置空
}
```

**为什么移动函数都要标 `noexcept`**：以 `std::vector<String>` 为例，扩容时要搬移元素。如果移动构造**保证不抛异常**（`noexcept`），vector 就直接移动元素（O(1)）；如果可能抛异常，vector 为了异常安全只能退化为拷贝（O(n)），性能大幅下降。这是约定俗成的规则：移动操作必须 `noexcept`。

**拷贝与移动的触发时机**（简化版）：

```cpp
String a("hello");
String b = a;          // a 是左值（有名对象）→ 拷贝构造
String c = std::move(a); // std::move 把 a 强转为右值 → 移动构造（a 被清空）
String d = String("x");  // 临时对象 → 移动构造（或直接被省略）
```

### 1.3 `[[nodiscard]]` 属性

**含义**：修饰函数的返回值，告诉编译器"这个返回值不允许被丢弃"。若调用者忽略返回值，编译器发出警告。

本项目用法：

```cpp
[[nodiscard]] std::size_t length() const noexcept;
[[nodiscard]] std::size_t find(std::string_view target, std::size_t pos = 0) const noexcept;
[[nodiscard]] String substr(std::size_t pos = 0, std::size_t len = npos) const;
```

**为什么加**：`find` 返回查找位置，如果你写了 `s.find("x");` 却不接住结果，多半是忘了用——编译器警告提醒你。`substr` 返回一个新串，丢弃它没意义。`length`/`empty`/`data` 同理。这是 C++17 引入的"静态检查"手段。

### 1.4 `constexpr` 与 `static constexpr`

**含义**：`constexpr` 表示"编译期就能算出结果的常量"。`static` 表示它是类（或命名空间）的静态成员，通过 `类名::常量名` 访问，而不是每个对象一份。

```cpp
static constexpr std::size_t npos = static_cast<std::size_t>(-1);
```

这里 `static_cast<std::size_t>(-1)` 把 `-1`（int）强制转换成 `size_t`（无符号），结果就是 **size_t 能表示的最大值**。`npos` 的语义是"不存在 / 到末尾"：比如 `find` 找不到时返回 `npos`，`substr` 的 `len = npos` 表示"一直截到串尾"。

### 1.5 `noexcept`

**含义**：承诺函数"不会抛异常"。若在 `noexcept` 函数里真的抛了异常，程序直接调用 `std::terminate` 终止（而不是正常传播异常）。

本项目标 `noexcept` 的都是"只操作成员、不分配内存、不调用可能抛异常的函数"的方法：`length()`、`data()`、`empty()`、`operator[]`、`operator string_view()`、移动构造/移动赋值等。这是把承诺写清楚，让编译器能做更多优化。

### 1.6 模板（Templates）

**含义**：让"类型/值"作为参数，写一次代码，为多种类型生成多个版本。

本项目用到两种：

**函数模板**——`Index` 可同时接受 `StaticString` 和 `DynamicString`：

```cpp
template <StringConcept S1, StringConcept S2>
std::size_t Index(const S1& str, const S2& target, std::size_t pos = 0) noexcept;
```

当调用 `Index(a, b)` 时，编译器根据实参类型自动推导出 `S1`、`S2`（这叫**模板实参推导**）。

**类模板**——`StaticString` 把"容量 N"作为**非类型模板参数**（一个值，不是类型）：

```cpp
template <std::size_t N = 255>
class StaticString { char m_data[N + 1]; ... };
```

`StaticString<64> s;` 会生成一个内部缓冲区恰好 64+1 字节的类。默认参数 `= 255` 表示写 `StaticString<>` 时用 255。

**默认模板参数**：

```cpp
template <typename ResultString = DynamicString, StringConcept S>
ResultString SubString(const S& src, std::size_t pos, std::size_t len = npos);
```

`ResultString` 默认是 `DynamicString`，调用时可显式指定，如 `SubString<StaticString<32>>(...)`。

### 1.7 C++20 概念（Concepts）

**要解决的问题**：C++11 起模板报错晦涩难懂（几千行错误信息）。C++20 的 **concept** 给模板参数加上"该参数必须满足什么要求"的约束，不满足直接给出清晰报错。

定义：

```cpp
template <typename T>
concept StringConcept = requires(const T& s) {
    { s.length() } -> std::same_as<std::size_t>;
    { s.data() }   -> std::same_as<const char*>;
    { s.empty() }  -> std::same_as<bool>;
    { static_cast<std::string_view>(s) } -> std::same_as<std::string_view>;
};
```

逐句解读：

- `requires(const T& s)`：引入一个"要求块"，`s` 是一个测试用的 `const T&` 假想对象。
- `{ s.length() } -> std::same_as<std::size_t>`：**表达式 `s.length()` 必须能编译通过，且它的类型必须恰好是 `std::size_t`**。`std::same_as<A>` 是"类型完全相等"的约束（`{ 表达式 } -> 约束` 是 C++20 的新语法，等于把约束套用在表达式的类型上）。
- 四条要求合起来就是：**一个类型 T 只要"有 `length()` 返回 `size_t`、`data()` 返回 `const char*`、`empty()` 返回 `bool`、能转成 `string_view`"，就算满足 `StringConcept`**。

有趣推论：`std::string`、`std::string_view` 也全部满足 `StringConcept`。也就是说 `Index(std::string("a"), std::string_view("b"))` 也能编译通过——这就是"泛型"的威力。

使用方式：`template <StringConcept S1, StringConcept S2>` 表示 S1、S2 必须满足 `StringConcept`。违反时报错会直接说"类型不满足 StringConcept"。

### 1.8 `std::string_view`

**含义**：**不拥有**数据的字符串视图，内部就是"一个 `const char*` 指针 + 一个长度"。拷贝 `string_view` 只是拷贝指针和长度，O(1)，不分配内存。

```cpp
std::string_view sv = str;   // 借用 str 的缓冲区，str 负责内存
sv.substr(...); sv.find(...); sv.compare(...);  // 只读操作都免费
```

本项目把 `string_view` 当作"统一的中转格式"：`Index`/`StrCompare`/`SubString`/`Concat` 先把各类字符串转成 `string_view`，再调用标准库算法。

**⚠️ 生命周期陷阱**：`string_view` 自己不持有数据。如果它借用的那个字符串被销毁、或被修改而触发重新分配，视图就悬空了（dangling）。详见第 7 章。

### 1.9 `union` 联合体（SSO 的核心存储）

**含义**：联合体里所有成员**共享同一块内存**。同一时刻只能用一个成员（"激活的"成员）。

```cpp
union Storage {
    char sso_buf[SSO_CAPACITY + 1];   // 16 个字节，内嵌在对象里
    LongBuffer heap;                   // 一个指针 + 一个 size_t（8+8 字节）
} m_storage;
```

因为两者重叠，对象大小是"最大的成员"（16 字节），而不是两者之和。短串时用 `sso_buf`（数据在对象内部，零堆分配）；长串时用 `heap`（存指向堆内存的指针）。

**如何知道当前用的是哪个成员**？靠对象外的另一个字段 `m_capacity` 做"标签"：

```cpp
bool is_sso() const noexcept { return m_capacity <= SSO_CAPACITY; }
```

- `m_capacity == 15`（SSO_CAPACITY）→ 当前用 `sso_buf`；
- `m_capacity > 15` → 当前用 `heap`。

这就是经典的 **tagged union（带标签的联合体）** 技巧。注意：读"非激活成员"在 C++ 标准里属于未定义行为（工程实践中广泛使用且通常没问题），本项目只在对应模式下读写对应成员，写法是安全的。

### 1.10 委托构造（Delegating Constructor）

**含义**：一个构造函数在初始化列表里调用同类的另一个构造函数，避免重复代码。

本项目的经典写法：

```cpp
// const char* 版：先转成 string_view，再委托给 string_view 版
String::String(const char* str)
    : String(str ? std::string_view(str) : std::string_view()) {}

// 拷贝构造：利用类型转换运算符把 other 变成视图，再委托给 string_view 版（= 重新做一份拷贝）
String::String(const String& other)
    : String(std::string_view(other)) {}
```

`str ? ... : ...` 是三目运算符：`str` 非空用 `string_view(str)`，是 `nullptr` 则给空视图（**空指针保护**）。

---

## 2. 方案一：ModernString1 —— 基于 SSO 的统一 String 类

### 2.1 设计思想

对外**只有一个类** `ModernString1::String`。用户在栈上创建对象，至于数据是存在对象内部的缓冲区还是堆上，**由类自己根据长度决定**，用户无感知：

- 长度 **≤ 15** → 数据存在对象内部的 `char sso_buf[16]`（**零堆分配**，极快）；
- 长度 **> 15** → 数据存在堆上，对象里只存一个指针。

这种技术叫 **SSO（Small String Optimization，小字符串优化）**，`std::string` 在主流编译器中就是这么实现的。它的好处是：大量短字符串场景（如字典键、标签）没有堆分配，同时长字符串也不浪费对象内存。

### 2.2 内存布局

对象由三部分组成：

```text
union Storage {                    ┌────────────────────────┐
    char sso_buf[16];              │  sso_buf[16]  或  heap  │  ← 16 字节（重叠）
    LongBuffer{ char* ptr;         │                         │
                 size_t cap; };    │                         │
} m_storage;                       └────────────────────────┘
std::size_t m_length;              ← 当前长度（8 字节）
std::size_t m_capacity;            ← 容量，兼作"标签"（8 字节）
```

- SSO 模式：`m_capacity = 15`，数据在 `sso_buf[0..m_length]`，`sso_buf[m_length] = '\0'`。
- 堆模式：`m_capacity = 分配的长度`（或扩容后的大小），`m_storage.heap.ptr` 指向堆缓冲，`heap.cap` 记录分配容量。
- 两种模式下，`data()` 返回的缓冲区**都以 `'\0'` 结尾**，可直接当 C 字符串用。

### 2.3 生命周期管理（构造 / 析构 / 拷贝 / 移动 / 赋值）

| 操作 | 实现要点 | 复杂度 |
|---|---|---|
| 默认构造 | `m_length=0, m_capacity=SSO_CAPACITY, sso_buf[0]='\0'`，空串、SSO 模式 | O(1) |
| `String(const char*)` | 委托给 string_view 构造；`nullptr` 保护 | O(n) |
| `String(string_view)` | 核心构造：按长度分流 SSO / 堆 | O(n) |
| 拷贝构造 | 委托给 string_view 构造，等于重新做一份 | O(n) |
| 移动构造 | SSO 则整块拷贝缓冲；堆则偷指针，源对象置空 | O(1) |
| 析构 | 若 `!is_sso()` 则 `delete[] heap.ptr` | O(1) |
| 拷贝赋值 | 自赋值保护；`*this = string_view(other)` | O(n) |
| 移动赋值 | 先释放自己已有的堆内存，再偷 | O(1) |
| 赋值 string_view | 容量够就原地覆盖；不够则释放旧的、按精确长度新分配 | O(n) |

细节：

- **构造分流**：`m_length <= SSO_CAPACITY` → 栈内 `memcpy`；否则 `new char[m_capacity + 1]`（**容量=长度**，精确分配）。
- **移动后的源对象**：`m_length = 0`，`m_capacity` 重置为 `SSO_CAPACITY`，缓冲区写 `'\0'`。它处于"合法但为空"的状态，仍可被继续使用或析构。
- **赋值复用容量**：`operator=(string_view)` 里，`sv.length() <= m_capacity` 就直接在现有缓冲里覆盖，**不重新分配**。这也是为什么拷贝赋值要委托给它——拷贝已有对象时往往容量够用。
- **堆模式一旦开启就不会缩回 SSO**：`m_capacity` 只会增大或保持，不会因为字符串变短而释放堆内存。（这是 `std::string` 同款行为：释放内存很贵，宁可留着复用。）

### 2.4 各 API 详解

#### 常量

- **`static constexpr std::size_t npos`**：`size_t(-1)`，即最大无符号值，表示"未找到 / 到末尾"。用法：`s.find("x")` 的返回、`substr` 的 `len` 默认值。
- **`static constexpr std::size_t SSO_CAPACITY = 15`**：SSO 阈值。小于等于 15 个字符走栈内缓冲区（缓冲区开 16 字节，留一位给 `'\0'`）。

#### 容量与访问

- **`std::size_t length() const noexcept`** → 字符数（不含结尾 `'\0'`）。`[[nodiscard]]`。
- **`std::size_t capacity() const noexcept`** → 容量。SSO 模式下恒等于 15。
- **`bool empty() const noexcept`** → `m_length == 0`。`[[nodiscard]]`。
- **`const char* data() const noexcept`** → 指向内部缓冲区，**保证以 `'\0'` 结尾**。SSO 返回 `sso_buf`，堆模式返回 `heap.ptr`。`[[nodiscard]]`。
- **`char& operator[](std::size_t index) noexcept`** / **`const char& operator[](std::size_t index) const noexcept`** → 下标访问，可读可写（非 const 版）。**不做越界检查**，`index >= length()` 是未定义行为。
- **`operator std::string_view() const noexcept`** → 隐式转视图，`string_view(data(), length())`。几乎所有算法都靠它。

#### 基本操作

- **`String substr(std::size_t pos = 0, std::size_t len = npos) const`** → 取子串。
  - `pos >= m_length` → 返回空串；
  - `actual_len = min(len, m_length - pos)`：`len` 超出剩余长度会自动截断；
  - 通过 `String(string_view(data()+pos, actual_len))` 返回新对象。
  - 注意：`std::string::substr` 在 `pos > size()` 时抛 `out_of_range`，这里改为返回空串，语义更宽松。
- **`std::size_t find(std::string_view target, std::size_t pos = 0) const noexcept`** → 从 `pos` 起查找子串，找到返回起始下标，找不到返回 `npos`。直接调用 `std::string_view::find`。

#### 拼接

- **`String& operator+=(std::string_view sv)`** → 就地追加。
  - 容量够：直接在 `data()+m_length` 处 `memcpy` 追加，更新长度；
  - 容量不够：`new_cap = max(m_capacity * 2, new_len)`（**成倍扩容**，保证均摊 O(1)），分配新缓冲、拷贝旧数据+新数据，再释放旧缓冲。
- **`friend String operator+(const String& lhs, std::string_view rhs)`** → `lhs` 拷贝一份，`+= rhs`，返回新对象。`lhs` 本身不变。支持 `s + "abc"`（`const char*` 隐式转 `string_view`）。

#### 比较

- **`auto operator<=>(const String& other) const noexcept`** → 三路比较，返回自动推导的比较序（此处为 `std::strong_ordering`）。基于 `std::string_view` 的比较。定义它之后，`< <= > >=` 都能用。
- **`bool operator==(const String& other) const noexcept`** → 相等判断，基于 `string_view` 逐字节比较。

#### 输出

- **`std::ostream& operator<<(std::ostream& os, const String& str)`** → `os << std::string_view(str)`，让 `std::cout << s` 工作。返回 `os` 是为了支持链式 `cout << a << b`。

---

## 3. 方案二：ModernString2 —— Concept + 模板泛型

### 3.1 设计思想

**与方案一相反的路线**：不把静态/动态存储揉进一个类，而是**物理隔离**成两个类，再用泛型算法把它们统一起来：

- **`StaticString<N>`**：固定长度栈缓冲区（模板参数 N 决定大小），**永不堆分配**，适合嵌入式/无动态内存环境；
- **`DynamicString`**：堆分配，可增长；
- 两者之上的四个**自由函数**（`Index` / `StrCompare` / `SubString` / `Concat`）用 `StringConcept` 约束，对**任意满足该概念的类型**一视同仁。

好处是**开闭原则**：以后想加第三种字符串类型（比如 UTF-8 串、大对象串），只要让它满足 `StringConcept`，四个算法直接能用，无需改动算法。方案一是"一个类全家桶"，方案二是"几个小类 + 通用算法"。

### 3.2 `StringConcept` 概念

```cpp
template <typename T>
concept StringConcept = requires(const T& s) {
    { s.length() } -> std::same_as<std::size_t>;
    { s.data() }   -> std::same_as<const char*>;
    { s.empty() }  -> std::same_as<bool>;
    { static_cast<std::string_view>(s) } -> std::same_as<std::string_view>;
};
```

详见 [1.7](#17-c20-概念concepts)。补充一点：因为要求的是 `const T&`，所以 **只读** 操作即可满足；写操作（如 `assign`、`operator[]` 非 const 版）不在概念要求内，算法也只用只读接口。

### 3.3 `StaticString<N>`

模板类，非类型参数 `N`（默认 255）决定容量。

| 成员 | 说明 |
|---|---|
| `StaticString() noexcept` | 空串：`m_data[0] = '\0'`，`m_length=0` |
| `StaticString(std::string_view sv)` | 构造时 `assign(sv)` |
| `bool assign(std::string_view sv)` | 拷贝到内部缓冲；`m_length = min(sv.length(), N)`。**若源串超长则截断，返回 `false` 表示未完整拷贝**；完整则返回 `true` |
| `std::size_t length() const noexcept` | `m_length` |
| `std::size_t capacity() const noexcept` | 恒等于 `N`（固定） |
| `bool empty() const noexcept` | `m_length == 0` |
| `const char* data() const noexcept` | `m_data`（固定栈缓冲） |
| `char& operator[](std::size_t i) noexcept` | 可写下标，不检查越界 |
| `const char& operator[](std::size_t i) const noexcept` | 只读下标 |
| `operator std::string_view() const noexcept` | 转视图 `string_view(m_data, m_length)` |

注意：这个类**没有**自定义析构/拷贝/移动——成员只有 `char[N+1]` 和 `size_t`，编译器自动生成的都是正确的（按值拷贝即可）。它**从不分配、从不释放**，生命周期极简。

### 3.4 `DynamicString`

堆分配，完整的 RAII 五件套（构造/析构/拷贝/移动/赋值）。

| 成员 | 说明 |
|---|---|
| `DynamicString() noexcept` | `m_data=nullptr, m_length=0`，空串 |
| `DynamicString(std::string_view sv)` | `assign(sv)`（实现见 .cpp） |
| `DynamicString(const DynamicString&)` | 拷贝构造，委托 `string_view` 构造 |
| `DynamicString(DynamicString&&) noexcept` | 移动构造：偷指针，源对象 `nullptr` |
| `~DynamicString()` | `delete[] m_data`（`delete[]` 空指针是合法的） |
| `operator=(const DynamicString&)` | 拷贝赋值，自赋值保护 |
| `operator=(DynamicString&&) noexcept` | 移动赋值：先释放自己的旧内存，再偷 |
| `bool assign(std::string_view sv)` | **先 `delete[] m_data`**，再按 `sv.length()` 精确分配新缓冲；长度 0 时置 `nullptr`。恒返回 `true` |
| `length() / empty()` | 同前 |
| `const char* data() const noexcept` | `m_data ? m_data : ""`——**空指针时返回空串字面量**，避免悬空指针传给 `memcpy`/字符串函数 |
| `operator[]`（两个版本） | 下标访问，不检查越界 |
| `operator string_view() const noexcept` | `string_view(data(), m_length)` |

注意两点：
- **没有 `capacity()`、没有 `operator+=`**：每次 `assign` 都先释放再按精确长度分配，从不预分配。因此若用它频繁"逐个字符累加"，性能是 O(n²)（方案一的 `+=` 有成倍扩容，是 O(n) 均摊）。设计如此：它只承担"持有一段数据"的职责。
- **`assign` 返回 `bool`**：与 `StaticString::assign` 的 `bool` 签名保持一致（接口统一），只是它永远不会失败返回 `true`。

### 3.5 四个泛型算法

它们都不关心具体类型，只要求实参满足 `StringConcept`。所有算法先把参数转成 `string_view` 再操作。

#### `Index` —— 泛型查找

```cpp
template <StringConcept S1, StringConcept S2>
std::size_t Index(const S1& str, const S2& target, std::size_t pos = 0) noexcept {
    return std::string_view(str).find(std::string_view(target), pos);
}
```

- 在 `str` 中从 `pos` 起查找 `target`，返回起始下标，找不到返回 `npos`。
- `S1`、`S2` 可以是 `StaticString`、`DynamicString`，甚至是 `std::string`。**跨类型查找**（比如在 `StaticString` 里找 `DynamicString`）同样可行。

#### `StrCompare` —— 泛型比较

```cpp
template <StringConcept S1, StringConcept S2>
int StrCompare(const S1& s1, const S2& s2) noexcept {
    return std::string_view(s1).compare(std::string_view(s2));
}
```

- 返回 `int`：`< 0` 表示 `s1 < s2`；`0` 相等；`> 0` 表示 `s1 > s2`（字典序）。
- 对应 `strcmp` 的 C 风格语义，返回 `int` 而非 `bool`/`ordering`，便于作为排序比较器。

#### `SubString` —— 泛型截取

```cpp
template <typename ResultString = DynamicString, StringConcept S>
ResultString SubString(const S& src, std::size_t pos, std::size_t len = npos) {
    std::string_view sv(src);
    if (pos >= sv.length()) return ResultString();
    return ResultString(sv.substr(pos, len));
}
```

- **结果类型也是模板参数**，默认 `DynamicString`。想要栈结果就写 `SubString<StaticString<32>>(...)`。
- `pos >= length` → 返回空结果；否则取 `sv.substr(pos, len)`（`len = npos` 表示到末尾）构造结果。
- ⚠️ 若 `ResultString` 是 `StaticString<N>` 且子串超过 N，会被**静默截断**（`assign` 返回 `false` 但这里没检查）。

#### `Concat` —— 泛型拼接

```cpp
template <typename ResultString = DynamicString, StringConcept S1, StringConcept S2>
ResultString Concat(const S1& s1, const S2& s2) {
    std::string_view sv1(s1), sv2(s2);
    char* buf = new char[sv1.length() + sv2.length()];   // 临时缓冲（注意：没有 +1 存 '\0'）
    std::memcpy(buf, sv1.data(), sv1.length());
    std::memcpy(buf + sv1.length(), sv2.data(), sv2.length());
    ResultString res(std::string_view(buf, sv1.length() + sv2.length()));
    delete[] buf;   // 结果已深拷贝进自己的存储，临时缓冲可安全释放
    return res;
}
```

- 把两段拼进一个临时堆缓冲，构造结果（结果会**深拷贝**进自己的存储），再释放临时缓冲。
- **成本较高**：1 次临时分配 + 2 次 `memcpy`，加上结果构造再分配/再拷贝一次。换成方案一的 `operator+=`（成倍扩容、原地追加）会更省。这是方案二"通用性优先、性能靠后"的取舍。
- 临时缓冲没写 `'\0'` 是安全的：`string_view` 按长度拷贝，结果类会自行补 `'\0'`。**不要**试图把这个 `buf` 当作 C 字符串用（它不结尾）。

---

## 4. 两种方案对比

| 维度 | 方案一 ModernString1::String | 方案二 ModernString2 |
|---|---|---|
| **设计哲学** | 单类全家桶：SSO + 堆自动切换，用户无感知 | 物理隔离：`StaticString`（栈）/ `DynamicString`（堆）+ 泛型算法统一 |
| **内存模型** | `union`：≤15 字符用对象内 16 字节缓冲，超过用堆指针 | 类模板：固定 `N+1` 字节栈缓冲；堆类：指针+长度 |
| **对外接口风格** | 成员函数 + 运算符重载（`+`、`+=`、`[]`、`<=>`、`==`、`<<`） | 最小成员 + 自由函数（`Index`/`StrCompare`/`SubString`/`Concat`） |
| **泛化能力** | 封闭：所有操作绑定在 String 类内部 | 开放：满足 `StringConcept` 的任何类型（含 `std::string`）都能套用算法 |
| **结果类型可定制** | 否（`substr`/`operator+` 只能返回 String） | 是（`SubString<X>` / `Concat<X>` 指定结果类型） |
| **短字符串性能** | SSO：零堆分配，极快 | `StaticString`：零堆分配且无析构；`DynamicString` 即使很短也堆分配 |
| **长字符串 / 频繁拼接** | `operator+=` 成倍扩容，均摊 O(1)/字符，推荐 | `Concat` 每次 2 次分配，`DynamicString` 无预分配，不推荐频繁拼接 |
| **内存可控性** | 由长度自动决定，用户不可控 | 用户显式选择栈（`StaticString<N>`）或堆（`DynamicString`） |
| **异常安全性** | 构造失败可能抛 `bad_alloc`，默认 ctor/`length` 等不抛 | 同左；`assign` 超长（对 `StaticString`）是截断而非报错 |
| **适用场景** | 通用字符串，追求好用、写法自然 | 需要"零分配"的嵌入场景，或需要多种串类型共用的算法库 |
| **依赖的 C++ 特性** | C++20（`<=>`）、union、移动语义 | C++20 concepts、类模板、默认模板参数 |

---

## 5. API 速查表

### ModernString1::String（`string_cpp_1.h`）

| 接口 | 签名 | 行为 |
|---|---|---|
| 常量 | `static constexpr npos` | 未找到/到末尾的哨兵 |
| 常量 | `static constexpr SSO_CAPACITY = 15` | SSO 阈值 |
| 构造 | `String() noexcept` | 空串（SSO） |
| 构造 | `String(const char*)` | 空指针安全 |
| 构造 | `String(std::string_view)` | 核心构造，自动分流 |
| 构造/析构 | 拷贝、移动、`~String()` | RAII 完整 |
| 赋值 | `operator=(String / String&& / string_view)` | 三种赋值 |
| 容量 | `length() / capacity() / empty() / data()` | 均 `[[nodiscard]]` |
| 访问 | `operator[](size_t)` ×2 | 可写/只读，不越界检查 |
| 子串 | `substr(pos=0, len=npos)` | 越界返空串，len 自动截断 |
| 查找 | `find(target, pos=0)` | 找不到返 `npos` |
| 拼接 | `operator+=` / `friend operator+` | 就地追加 / 返回新对象 |
| 比较 | `operator<=>` / `operator==` | C++20 三路比较，自动派生 `<` 等 |
| 转换 | `operator string_view()` | 隐式转换 |
| 输出 | `friend ostream& operator<<` | `cout << s` |

### ModernString2（`string_cpp_2.h`）

| 接口 | 签名 | 行为 |
|---|---|---|
| 常量 | 命名空间级 `npos` | 未找到/到末尾 |
| 概念 | `concept StringConcept` | 约束"可转 string_view 的字符串类型" |
| 类模板 | `StaticString<N=255>` | 固定栈缓冲，永不分配；`assign` 超长截断返 `false` |
| 类 | `DynamicString` | 堆缓冲，RAII 完整；`assign` 精确分配 |
| 算法 | `Index(str, target, pos=0)` | 泛型查找 |
| 算法 | `StrCompare(s1, s2)` | 泛型比较（`<0 / 0 / >0`） |
| 算法 | `SubString<Result=DynamicString>(src, pos, len=npos)` | 泛型截取，可指定结果类型 |
| 算法 | `Concat<Result=DynamicString>(s1, s2)` | 泛型拼接，可指定结果类型 |

---

## 6. 使用示例

### 方案一

```cpp
#include "string_cpp_1.h"
using ModernString1::String;

String a;                              // 空串
String b("hello");                     // SSO，数据在对象内，零堆分配
String c("a string longer than fifteen bytes"); // 堆分配
String d = b;                          // 拷贝构造
String e = std::move(c);               // 移动构造，c 被置空

b += " world";                         // 追加，容量够则原地
b += c;                                // 容量不够 → 成倍扩容
String f = b + "!";                    // 拼接返回新对象，b 不变

std::size_t p = b.find("world");       // 找到返回下标
String sub = b.substr(0, 5);           // 前 5 个字符
b[0] = 'H';                            // 可写下标

if (b == d) { /* ... */ }              // operator==
if (b < d)  { /* ... */ }              // 由 operator<=> 自动改写而来

std::cout << b << std::endl;           // operator<<

// 注意：String 可隐式转 string_view，因此可以这样用
std::string_view v = b;
std::size_t again = v.find("lo");
```

### 方案二

```cpp
#include "string_cpp_2.h"
using namespace ModernString2;

// 两种类型可以混用
StaticString<64> s1("hello");          // 栈上固定 64 字节，零分配
DynamicString  d1("dynamic");          // 堆

StaticString<32> s2("world");

auto pos  = Index(s1, "lo");           // 泛型查找
int  cmp  = StrCompare(s1, d1);        // 跨类型比较（栈 vs 堆）
auto part = SubString(d1, 1, 3);       // 结果默认 DynamicString
auto seg  = SubString<StaticString<16>>(d1, 1, 3); // 结果指定为栈串

auto concat1 = Concat(s1, s2);         // 默认结果 DynamicString
auto concat2 = Concat<StaticString<128>>(s1, d1);  // 指定栈结果

// StaticString 超长赋值会截断，assign 返回 false 提示
bool ok = StaticString<8> tmp;  tmp.assign("too long for this buffer");
// ok == false，tmp 里是 "too lon"

// 概念是开放的：std::string 也能直接进算法
#include <string>
std::string st = "std string";
auto pos2 = Index(st, "string");       // 满足 StringConcept，照样能用
```

---

## 7. 复杂度与注意事项

### 7.1 各操作时间复杂度

| 操作 | 复杂度 | 说明 |
|---|---|---|
| `length / capacity / empty / data / operator[]` | **O(1)** | 直接读成员 / 取下标 |
| `String(string_view)` 构造、`assign`、拷贝构造 | **O(n)** | 一次 `memcpy` |
| 移动构造 / 移动赋值 | **O(1)** | 偷指针 |
| `substr` / `SubString` | **O(len)** | 只拷贝截取段 |
| `find` / `Index` | **O(n·m)** 最坏 | 委托 `string_view::find` |
| `StrCompare` | **O(min(n,m))** | 逐字节 |
| `operator+=`（方案一） | **均摊 O(n)** | 成倍扩容，均摊 O(1)/字符 |
| `operator+`（方案一） | **O(n+m)** | 拷贝 + 追加 |
| `Concat`（方案二） | **O(n+m)** | 另加 2 次分配 + 2 次拷贝（常数因子大） |
| 方案二 `DynamicString` 反复 `assign` | **O(n²) 总量** | 每次先释放再精确分配，无扩容复用 |

### 7.2 注意事项与潜在坑点

1. **SSO 拷贝的含义**：短串的"拷贝"是把对象内 16 字节缓冲整体搬走（`memcpy(m_length + 1)`），它仍是**独立的副本**，不是共享数据——源和目的互不影响。这正是方案一 `copy ctor` 委托给 `string_view` 构造的原因：等于重新做一份完整拷贝，语义安全。

2. **移动后的对象仍可使用**：移动（`std::move` 之后）的源对象是"合法但空"的：`m_length = 0`、`m_capacity = 15`、缓冲区 `'\0'`。可以继续给它赋值、追加、析构，但不要再假设里面有旧数据。

3. **`operator[]` 不做越界检查**：`str[str.length()]` 会写穿到结尾 `'\0'` 或越界（未定义行为）。需要安全访问请先检查 `length()`。

4. **`string_view` 悬空陷阱**：`string_view` 不拥有数据。以下情况会悬空：
   - 视图借用的字符串对象被析构；
   - 字符串被**重新分配**（如 `operator+=` 扩容、`assign` 到更大、`Concat` 的临时缓冲）。**不要在字符串可能被修改/重分配后继续使用旧视图**。

5. **方案二 `Concat` 的临时缓冲**：`new char[len1 + len2]` 没有留 `'\0'` 位置。它只被当作带长度的视图使用，随后深拷贝进结果、再释放，是安全的；但**不要**把它当 C 字符串（`printf("%s")`、`strlen`）用。

6. **方案二 `StaticString` 静默截断**：`assign`、`SubString`、`Concat` 的结果若超过 `N`，会被**静默截断**——`assign` 的 `bool` 返回值其实给出了提示，但构造/算法代码里都没检查。需要严格语义时，请主动检查 `assign` 返回值或确认长度在 `N` 内。

7. **自赋值与自我引用**：
   - 方案一的拷贝/移动赋值都做了 `this != &other` 保护；
   - **但若传入的视图别名了对象自身的内存**（如 `s = std::string_view(s.data() + 1, ...)` 或 `s += s`），两个 `memcpy` 源/目的重叠是**未定义行为**（`memcpy` 不允许重叠，重叠应换 `memmove`）。正常外部用法不会触发。

8. **`data()` 一定以 `'\0'` 结尾**：两种方案都保证（构造/赋值时写 `m_data[m_length] = '\0'`，`DynamicString` 空指针时返回 `""`）。可安全传给 C 风格接口。

9. **异常安全**：堆分配可能抛 `std::bad_alloc`。标了 `noexcept` 的函数（`length`、`data`、移动操作等）内部不做分配，保证不抛；构造/`assign`/`+=` 未标 `noexcept`，可能抛异常。`StaticString` 全流程零分配，天然不抛。

10. **方案二的"精确分配"策略**：`DynamicString::assign` 每次按精确长度 `new char[len+1]` 并先 `delete[]` 旧的。若用它频繁变长地拼接，会不断分配/释放——这是它**没有** `operator+=` 的原因。频繁拼接请用方案一的 `String`。

11. **`Concat` 的代价**：无论结果多短，都会先 `new` 一个临时缓冲再拷贝，短串拼接（本可走 SSO）也付出堆分配。追求性能的拼接路径应选方案一 `operator+=`/`operator+`。

---

*本文档基于 `include/string_cpp_1.h`、`src/common/string_cpp_1.cpp`、`include/string_cpp_2.h`、`src/common/string_cpp_2.cpp` 撰写。*
