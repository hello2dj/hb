---
title: go map实战
date: 2018-05-10
tags:
    - go map
---

### go maps 实践

<!-- more-->

* 简介
在cs中hash table是一种经常使用的数据结构。许多hash table的实现都拥有很多的属性。但总的来说，他们都会提供快速查询，添加以及删除等功能。go 提供了一个内置的实现了hash table的map 类型。

* 声明和初始化
go map类型的签名如下
```
map[KeyType]ValueType
```
KeyType要求是能够[comparable](https://golang.org/ref/spec#Comparison_operators)的类型, 而ValueType则可以是任意的类型，甚至是另一个map

如下一个key为string,value是int的map
```
var m map[string]int
```

map类型也是引用类型，就像指针或者切片一样，因此上面的声明的m是nil;我们还没有实例化map。一个值是nil的map再读取的时候就像是个空map你啥都读不到
```
value, ok := m["c"]
// value is 0, ok is false
```
但是写入nil map是会报错的。千万记得不要这么做要记得初始化map，请使用内置的make function(make 专门用来分配内存，初始化map, slice, channel)
```
m = make(map[string]int)
```
make会分配内存并且初始化然后返回一个map值，注意make不生成指针new才是返回指针但是new只分配内存，而不初始化。make的实现底层是基于的c的实现，本文只关注怎么使用，就不分析他的实现了。

* 使用maps
go 提供了便捷的语法来操作map例如赋值
```
m["route"] = 66
```
加下来取值
```
i := m["route"]
```

若是我们取得值不存在那么我们取到的会是值相应类型的默认值（zero value）。在我们的例子中我们读到的就是0：
```
j := m["root"]
// j == 0
```
内置的len函数可以得到map中的元素个数
```
n := len(m)
// n == 1
```
内置的delete函数是用来删除map中的元素
```
delete(m, "route")
```
delete没有返回值，并且若是删除的key不存在则啥都不处理

还有一种读取的语法如下
```
i, ok := m["route"]
```
这个语法是：i取得是m中route对应的数据，若是不存在route对应的数据则i会是对应类型的零值，而ok代表的是route在m是否存在，false即是不存在也就是说没有读到i值。

当我们只是为了验证是否存在相应的key时可以使用下划线来忽略key对应的数据
```
_, ok := m["route"]
```

为了遍历map，我们可以使用range关键字
```
for key, value := range m {
    fmt.Println("Key:", key, "Value:", value)
}
```

若是不使用make,我们也可以使用map的字面量来初始化一个map
```
commits := map[string]int {
    "rsc": 3711,
    "r", 2138,
}
```
我们还可以初始化一个空map，和使用make是一样的
```
var m = map[string]int{}
```

* 对默认值的利用
当我们读取的key不存在时返回默认值有的是很方便的。

比如，一个值为bool类型的map就可以看做是一个set类型的数据结构（要知道，布尔类型的默认值是false）。这个例子遍历一个linked list of nodes, 并且打印他们的值。他使用值类型是Node 指针的map来检测list是否有环。
```
type Node struct {
    Next *Node
    Value interface{}
}
var first *Node

visited := make(map[*Node]bool)
for n := first; n != nil; n = n.Next {
    if visited[n] {
        fmt.Println("cycle detected")
        break
    }
    visited[n] = true
    fmt.Println(n.Value)
}
```
若Node n已经被访问过了，则visited[n]的值是true, 若值是false则说明Node n没有被访问过。我们不需要再用其他数据来判断node在map中的存在性，map默认值已经帮我们处理了。

另一个有用的例子是slices的map, 我们知道当我们想一个nil slice append数据的时候是会分配新的内存的。因此当我们向slice的map中append 数据的时候，是不需要检查key是否存在的。可以看看下面的例子
```
type Person struct {
    Name string
    Likes []string
}

var people []*Person

likes := make(map[string][]*Person)
for _, p := range people {
    for _, l := range p.Likes {
        likes[l] = append(likes[l], p)
    }
}
```

我们开一个打印出喜欢cheese的人：
```
for _, p := range likes["cheese"] {
    fmt.Println(p.Name, "likes cheese.")
}
```

打印出有多少人喜欢bacon
```
fmt.Println(len(likes["bacon"]), " people like bacon.")
```
> range 和 len都把nil slice当做长度是0的slice, 因此我们最后两个数据是不会出错的。

* key的类型
前面提到过了，就是map的keys必须是可比较的。语言规范已经详细定义了[可比较](http://golang.org/ref/spec#Comparison_operators)。 总的来说可比较的类型就是boolean, numeric, string, pointer, channel 以及接口类型，还有只包含上述类型的结构体和数据。不在上述范围的类型有map, slice 和 functions; 这些类型是不能用==，也不能当做map的keys的。

很明显strings， ints, 以及一些其他的类型可以做key,但是结构体就有点而出乎意料了。
让我们看这个
```
hits := make(map[string]map[string]int)
```
这是一个页面访问的次数的map，key对应二级url
```
n := hits["/doc/"]["au"]
```
但我们这么访问是错的，因为map是需要实例化的，我们可以这么读，但是当我们添加的时就会有问题，我们需要去初始化内部的map。如下
```
func add(m map[string]map[string]int, path, country string) {
    mm, ok := m[path]
    if !ok {
        mm = make(map[string]int)
        m[path] = mm
    }
    mm[country]++
}
add(hits, "/doc/", "au")
```
但是我们可以采用另一种设计如下
```
type Key struct {
    Path, Country string
}

hits := make(map[Key]int)
```
此时我们可以一步添加
```
hits[Key{"/", "vn"}]++
```
另外读取也是非常方便的
```
n := hits[Key{"/ref/spec/", "ch"}]
```

* 并发
Maps不是并发安全的：当我们同时读写时map的行为是未定义的。通常我们可以使用sync.RWMutex来保护map

看个例子
```
var counter = struct {
    sync.RWMutex
    m map[string]int
}{m: make(map[string]int)}
```

读取的时候就可以使用读锁
```
couter.RLock()
n := counter.m["some_key"]
counter.RUnlock()
fmt.Println("some_key:", n)
```
写时用写锁
```
couter.Lock()
counter.m["some_key"]++
counter.Unlock()
```
> sync.Mutex第一次被使用后，千万不可以复制，要传指针。因为sync.Mutex是结构体而非指针数据，接下来回来一篇文章分析一下的。

* 迭代顺序
map的迭代顺序是不包证的
```
import "sort"

var m map[int]string
var keys []int
for k := range m {
    keys = append(keys, k)
}

sort.Ints(keys)
for _, k := range keys {
    fmt.Println("Key:", k, "value:", m[k])
}
```

* 注意点
map element是不可以addressable的意味着, 其实是可以预料的，因为map会扩容，那么扩容后map元素是否还在原地址就不一定了，所以&map["x"]这个操作是不被允许的。
```
type data struct {
    name string
}
var a = map[string]data {"x": {"one"}}
m["x"].name = "two" // error
```
错误的，除非他的类型是指针
```
type data struct {  
    name string
}

func main() {  
    m := map[string]*data {"x":{"one"}}
    m["x"].name = "two" //ok
    fmt.Println(m["x"]) //prints: &{two}
}
```
但是要注意不要写入空的了指针，是会panic的
```
package main

type data struct {  
    name string
}

func main() {  
    m := map[string]*data {"x":{"one"}}
    m["z"].name = "what?" //???
}
```
很显然，指针的默认值是nil,当然无法访问nil的name了。


但是slice element就可以addressable
```
type data struct {  
    name string
}

func main() {  
    s := []data {{"one"}}
    s[0].name = "two" //ok
    fmt.Println(s)    //prints: [{two}]
}
```