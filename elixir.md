### keywords key 必须是原子，并且 key 可以不是唯一的,并且顺序是固定的， 两种形式

```
[foo: "dje", bar: "d", foo: "adsf"]
[{:foo, "dje"}, {:bar: "d"}, {:foo, "adsf"}]
```

### map 的键值可以是任意类型，并且没有排序，定义形式

```
%{:foo => "bar", "hello" => :world}
```

1.  添加重复的 key 值会覆盖之前的
2.  当 key 值只有原子时
    1.  可以使用：代替=>
    2.  map 有自己的获取元素的方法 a.foo
    3.  map 有自己的更新方法 %{map | foo: "bz"}

1. 可以有Map Module进行操作
2. elixir中不可以有大写开头的变量，那都是Module


### 元组
元组是不可变的
```
{3.14, :pie, "Apple"}
```

因此常作为返回值

### keyword
关键字列表（keywords）和映射（maps）是 Elixir 中两个相关的集合。Elixir 的关键字列表是一种特殊的列表：列表里的内容是二元元组，并且二元组的第一个元素必须是原子。它和列表的行为完全一致
```
[{:foo, "bar"}, {:hello, "world"}] == [foo: "bar", hello: "world"]
```
键（key）都是原子（atom）
键（key）是有序的（定义后，顺序不会改变）
键（key）不是唯一的

因为这些原因，关键字列表最常见的用法是作为参数传递给函数

### 映射
Elixir 的映射（maps）是键值对结构的第一选择，和关键字列表（keywords）不同，映射允许任意类型的数据作为键，而且数据并不严格排序。你可以使用 %{} 来定义映射：
```
iex> map = %{:foo => "bar", "hello" => :world}
%{:foo => "bar", "hello" => :world}
iex> map[:foo]
"bar"
iex> map["hello"]
:world
```

从 Elixir 1.2 版本开始，变量也可以作为映射的键（key）
```
iex> key = "hello"
"hello"
iex> %{key => "world"}
%{"hello" => "world"}
```

如果重复的键添加到映射中，后面的值会覆盖之前的值：
```
iex> %{:foo => "bar", :foo => "hello world"}
%{foo: "hello world"}
```

从上面的运行结果中，你或许发现了一些不同：存储键只有原子的映射，可以不用 =>，直接使用关键字列表的语法：

```
iex> %{foo: "bar", hello: "world"}
%{foo: "bar", hello: "world"}

iex> %{foo: "bar", hello: "world"} == %{:foo => "bar", :hello => "world"}
true
```

```
%{foo: :hello, "hello": :world} == %{foo: :hello, hello: :world}
%% hello 和 foo都会变成atom

%{foo: :hello, "hello"=> :world} %% error 只要有一个用了=> 语法就必须得都用 =>
```

map也有 ‘.’ 获取方法就是 key值是原子的
```
iex(31)> a = %{:foo => "bar", "hello" => :world}
%{:foo => "bar", "hello" => :world}
iex(32)> a.foo
"bar"
iex(33)> a.hello
** (KeyError) key :hello not found in: %{:foo => "bar", "hello" => :world}
```

map 有自己的更新方法 %{map | foo: "bz"}

### 模式匹配 https://elixirschool.com/zh-hans/lessons/basics/pattern-matching/#%E5%8C%B9%E9%85%8D%E6%93%8D%E4%BD%9C%E7%AC%A6

1. 函数参数也可以模式匹配，在其他语言中参数必须是形参，但在elixir中可以有如下的函数定义
```
def hello("dengjie"), do: "dengj"
```
2. 匿名函数的调用
```
sum = fn (a, b) -> "dj" end
sum.(2, 3)
```
### 函数 https://elixirschool.com/zh-hans/lessons/basics/functions/

1. 参数默认值 a \\ 1
2. 函数简写 def a, do: 23