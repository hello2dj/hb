### golang 的所有的都是 copy 赋值，=，传参，channel 发送等等都是。

### := 这种声明方式若是已经定义了，则不会新建

```
package main

import (
	"fmt"
)

func main() {
	pase_student()
}
type student struct {
	Name string
	Age  int
}

func pase_student() {
	m := make(map[string]*student)
	stus := []student{
		{Name: "zhou", Age: 24},
		{Name: "li", Age: 23},
		{Name: "wang", Age: 22},
	}
	for i, stu := range stus {
		m[stu.Name] = &stu; // 应该改为m[stu.Name] = &stus[i]
	}

	for _, va := range m {
		fmt.Printf("%p \n", va)
	}
}
```

stus 是个 map, stu 并不会每次新生成一个, 其实循环时每次都是相同的 stu, 而且 golang 每次都是 copy 语义，你再看一眼会发现，给 map 赋值的是指针，那就意味着无论你如何改变 map 都会是相同的值。:= 多次声明不会重新定义新的变量， 可见[规范](https://golang.org/ref/spec#Short_variable_declarations)

见下:

```
field1, offset := nextField(str, 0)
field2, offset := nextField(str, offset)  // redeclares offset
a, a := 1, 2                              // illegal: double declaration of a or no new variable if a was declared elsewhere
```

### golang 的字符串是不可变的，要想使用可变字符串可以使用 bytes，或者[]rune 数组, string 类型不可变，他的 slice 也不可变

### 我们可以从关闭的 channel 中读取数据但是为空，就是说若是在 select 语句 case 中从关闭的 channel 是可以的。 [参见](https://play.golang.org/p/zqR533gQiSJ)

```
select {
case <- stopCh:
  fmt.Println("go")
default:
  fmt.Println("come")
}
```

### 在使用 Go channel 的时候，一个适用的原则是不要从接收端关闭 channel，也不要关闭有多个并发发送者的 channel。 [优雅关闭 go channel](https://www.jianshu.com/p/d24dfbb33781)(http://www.tapirgames.com/blog/golang-channel-closing)

channel 的使用，当 channel 是非缓冲的时候他就是阻塞读与写的， 所以使用 channel 的时候要小心同步阻塞，导致死锁 game over

```
ch := make(chan string)
ch <- "23" // 此时就会阻塞
// a <- ch 单写他也是会阻塞的
```

所以要小心不要出现只有读或者只有写的 channel 若是出现的话，分布在不同 goroutine 中时就会导致 golang 死锁，主在等子完成可是子阻塞在了 channel 读或者写上了。

```
package main

import (
    "time"
    "math/rand"
    "sync"
    "log"
    "strconv"
)

func main() {
    rand.Seed(time.Now().UnixNano())
    log.SetFlags(0)

    // ...
    const MaxRandomNumber = 100000
    const NumReceivers = 10
    const NumSenders = 1000

    wgReceivers := sync.WaitGroup{}
    wgReceivers.Add(NumReceivers)

    // ...
    dataCh := make(chan int, 100)
    stopCh := make(chan struct{})
        // stopCh is an additional signal channel.
        // Its sender is the moderator goroutine shown below.
        // Its reveivers are all senders and receivers of dataCh.
    toStop := make(chan string, 1)
        // 设为缓冲1是为了防止moderator还未准备好就停止了
        // the channel toStop is used to notify the moderator
        // to close the additional signal channel (stopCh).
        // Its senders are any senders and receivers of dataCh.
        // Its reveiver is the moderator goroutine shown below.

    var stoppedBy string

    // moderator
    go func() {
        stoppedBy = <- toStop // part of the trick used to notify the moderator
                              // to close the additional signal channel.
        close(stopCh)
    }()

    // senders
    for i := 0; i < NumSenders; i++ {
        go func(id string) {
            for {
                value := rand.Intn(MaxRandomNumber)
                if value == 0 {
                    // here, a trick is used to notify the moderator
                    // to close the additional signal channel.
                    select {
                    case toStop <- "sender#" + id:
                    default:
                    }
                    return
                }

                // the first select here is to try to exit the
                // goroutine as early as possible.
                select {
                case <- stopCh:
                    return
                default:
                }
                // 为什么要在前面在加一个select stopCh呢? 因为若是到了这一步由于select的随机性（此时stopCh和dataCh都处于活跃状态select是随机选取的），有可能会继续发送，而没有选择stopCh
                select {
                case <- stopCh:
                    return
                case dataCh <- value:
                }
            }
        }(strconv.Itoa(i))
    }

    // receivers
    for i := 0; i < NumReceivers; i++ {
        go func(id string) {
            defer wgReceivers.Done()

            for {
                // same as senders, the first select here is to
                // try to exit the goroutine as early as possible.
                select {
                case <- stopCh:
                    return
                default:
                }

                select {
                case <- stopCh:
                    return
                case value := <-dataCh:
                    if value == MaxRandomNumber-1 {
                        // the same trick is used to notify the moderator
                        // to close the additional signal channel.
                        select {
                        case toStop <- "receiver#" + id:
                        default:
                        }
                        return
                    }

                    log.Println(value)
                }
            }
        }(strconv.Itoa(i))
    }

    // ...
    wgReceivers.Wait()
    log.Println("stopped by", stoppedBy)
}
```

### golang make 返回的是值类型，用 slice, map, channel, 并且会给 map 和 slice 预分配空间

### golang slice 语法可以对值也可以对引用

```
a = [1,2,34]
t := a[1:3] // ok
c := (&a)[1:2] // ok
```

### golang 切片的用法是 [start:end] 但不包括 end

### new 返回的是指针类型

### golang 也存在 js 中的那个经典问题就是

```
func main() {
	runtime.GOMAXPROCS(1)
	wg := sync.WaitGroup{}
	wg.Add(20)
	for i := 0; i < 10; i++ {
		go func() {
			fmt.Println("i: ", i)
			wg.Done()
		}()
	}
	for i := 0; i < 10; i++ {
		go func(i int) {
			fmt.Println("i: ", i)
			wg.Done()
		}(i)
	}
	wg.Wait()
}
```

第一个打印的 i 都是 10， 因为他们打印的都是同一个变量 i。

### golang 中 return defer 返回值的顺序

先来假设出结论，帮助大家理解原因：

多个 defer 的执行顺序为“后进先出”；

defer、return、返回值三者的执行逻辑应该是：return 最先执行，return 负责将结果写入返回值中；接着 defer 开始执行一些收尾工作；最后函数携带当前返回值退出。

如何解释两种结果的不同：

上面两段代码的返回结果之所以不同，其实从上面第 2 条结论很好理解。

a()int 函数的返回值没有被提前声名，其值来自于其他变量的赋值，而 defer 中修改的也是其他变量，而非返回值本身，因此函数退出时返回值并没有被改变。

b()(i int) 函数的返回值被提前声名，也就意味着 defer 中是可以调用到真实返回值的，因此 defer 在 return 赋值返回值 i 之后，再一次地修改了 i 的值，最终函数退出后的返回值才会是 defer 修改过的值。

defer 是在函数结束前执行的，当返回值的临时变量赋给外部时才算调用结束吧！a = fn(2) 当把值给了 a 才算结束

> 我们可以这么理解 return 肯定先执行执行的结果就是把返回值计算出来并且赋值给返回值所存在的临时变量, 但我们命名返回值的时候，返回值并不是临时变量而是函数中声明的变量

### 在函数有多个返回值时，只要有一个返回值有指定命名，其他的也必须有命名

### append 只能作用在 slice 上而不能是指针

参见签名

```
func append(slice []Type, elems ...Type) []Type
```

错误的栗子

```
package main

import "fmt"

func main() {
	s1 := []int{1, 2, 3}
	s2 := []int{4, 5}
	s1 = append(s1, s2)
	fmt.Println(s1)
} // 错误因为 append接下来的参数是以一个一个传递的切片中的元素，而不是切片 正确的是 s1 = append(s1, ...s2)
```

### 进行结构体比较时候，只有相同类型的结构体才可以比较，结构体是否相同不但与属性类型个数有关，还与属性顺序相关。并且结构体可比较的前提是结构的属性都可比较。map 和 slice 还有 function 不可比较

就是说属性名字不同，类型不同或者顺序不同都是不能比较的

```
func main() {

	sn1 := struct {
		age  int
		name string
	}{age: 11, name: "qq"}
	sn2 := struct {
		age  int
		name string
	}{age: 11, name: "qq"}

	if sn1 == sn2 {
		fmt.Println("sn1 == sn2")
	}

	sm1 := struct {
		age int
		m   map[string]string
	}{age: 11, m: map[string]string{"a": "1"}}
	sm2 := struct {
		age int
		m   map[string]string
	}{age: 11, m: map[string]string{"a": "1"}}

	if sm1 == sm2 {
		fmt.Println("sm1 == sm2")
	}
}
```

此处 sn1 可以和 sn2 使用==比较但是如下

```
sn3:= struct {
    name string
    age  int
}{age:11,name:"qq"}
```

sn3 就不能比较了

还有上例中，含有不可比较的 map,slice，func 等，所以 sm1 和 sm2 是不可比较的但是我们可以使用 deepEqual 来进行比较

```
if reflect.DeepEqual(sn1, sm) {
    fmt.Println("sn1 ==sm")
}else {
    fmt.Println("sn1 !=sm")
}
```

### := 赋值模式的限制

定义变量同时显式初始化不能提供数据类型只能在函数内部使用

### nil 可以用作 interface、function、pointer、map、slice 和 channel 的“空值”, 是不可以作为其他类型的空值的，比如 String, string 的空值是“”

```
var a chan int
// a 是空值nil
// chan的初始化一定是用make

var a map[int]string
// a 是空值nil
// map初始化一定使用make
// 使用new 生成的map也是nil的map
```

### itoa

```
const (
	x = iota
	y
	z = "zz"
	k
	p = iota
)

func main()  {
	fmt.Println(x,y,z,k,p)
}
// 结果
0
1
zz
zz
4（直接计算当前的值）
```

### golang const 常量可以使用 itoa 赋值， 甚至是自定义类型

[参见](https://segmentfault.com/a/1190000000656284)

若是自定义类型类似于枚举，当我们传递字面量（如：2，3）时也是可以被识别为枚举值的。

*   当在一行声明两个常量 itoa 时，itoa 是只有到了下一行才会增长

```
const (
    Apple, Banana = iota + 1, iota + 2
    Cherimoya, Durian
    Elderberry, Fig
)
// 输出
// Apple: 1
// Banana: 2
// Cherimoya: 2
// Durian: 3
// Elderberry: 3
// Fig: 4
```

*   itoa 从 0 开始增长，当我们们不使用 itoa 时，且赋值一个则所有的都会是这个值

```
const (
  a = "1"
  b
  c
  d
)
```

### goto 不能跳转到其他函数或者内层代码, 只能在本函数以及相同的 scope 内

下面的就是错的

```
package main

func main()  {

    for i:=0;i<10 ;i++  {
    loop:
        println(i)
    }
    goto loop
}
```

### 注意 defintion 和 type alias 的区别 defintion 是定义了一个新的类型，alias 仅仅是个别名

```
package main
import "fmt"

func main()  {
    type MyInt1 int
    type MyInt2 = int
    var i int =9
    var i1 MyInt1 = i // 错误 MyInt1是个新类型
    var i2 MyInt2 = i // yes 是个别名
    fmt.Println(i1,i2)
}
```

### 1. 我们在返回值中定义的命名返回值，可以直接在函数中使用不用再重新定义 2. 内部 scope 中新定义的内部变量是会覆盖掉外部的变量

```
package main

import (
    "errors"
    "fmt"
)

var ErrDidNotWork = errors.New("did not work")

func DoTheThing(reallyDoIt bool) (err error) {
    if reallyDoIt {
        result, err := tryTheThing()
        if err != nil || result != "it worked" {
            err = ErrDidNotWork
        }
        fmt.Println(err)
    }
    return err
}

func tryTheThing() (string,error)  {
    return "",ErrDidNotWork
}

func main() {
    fmt.Println(DoTheThing(true))
    fmt.Println(DoTheThing(false))
}
// 输出两个
<nil>
<nil>
```

改为

```
func DoTheThing(reallyDoIt bool) (err error) {
    var result string
    if reallyDoIt {
        result, err = tryTheThing() // 不要新定义变量
        if err != nil || result != "it worked" {
            err = ErrDidNotWork
        }
    }
    return err
}
```

### panic 仅有最后一个可以被 revover 捕获,panic 中可以传任何值，不仅仅可以传 string

```
func main()  {
    defer func() {
        if err:=recover();err!=nil{
            fmt.Println("++++")
            f:=err.(func()string)
            fmt.Println(err,f(),reflect.TypeOf(err).Kind().String())
        }else {
            fmt.Println("fatal")
        }
    }()

    defer func() {
        // 这里就recover是捕获不到err的，所以这里的err是nil
        if err:=recover();err!=nil{
            fmt.Println("++++")
            f:=err.(func()string)
            fmt.Println(err,f(),reflect.TypeOf(err).Kind().String())
        }else {
            fmt.Println("fatal")
        }
    }()

    defer func() {
        panic(func() string {
            return  "defer panic"
        })
    }()
    panic("panic")
}
```

### 不管运行顺序如何，当参数为函数的时候，要先计算参数的值

### map 引用不存在的 key，不报错

### map 使用 range 遍历顺序问题，并不是录入的顺序，而是随机顺序

### append 函数返回更新后的 slice（长度和容量可能会变），必须重新用 slice 的变量接收，不然无法编译通过

### golang channel 和一个 goroutine 组合起来就是一个 web-worker 的模式，只是我们在 js 里管理的是 web-worker 的句柄，而在 golang 里面我们要管理的是 channel,这里 golang 的一个优势是我可以把多个 channel 和一个 goroutine 绑定，而 web-worker 一个句柄就对应这个一个 web-worker 是不可能多对一的。

### golang channel 的使用方式两种

1.  调用者生成传递个 goroutine。
2.  被调用者生成返回给调用者。

### 内嵌结构体，当内嵌结构体不是指针时，是会有默认值得，这就符合了 golang 得概念，所有的变量都有默认值，指针的默认值是 nil.

```
package main
import "sync"
type A struct {
    sync.Mutex
}

func main() {
    a := A{}
    a.Lock()
    a.Unlock()
    fmt.Println("Mutex a ", a)
}
```

上面代码运行是 ok 的。

### 两个 golang 问题 http://colobu.com/2018/03/08/two-issues-in-go-development/

### golang 包的引入路径 GOPATH/src/...一层一层查找

### bin 是可执行的文件， pkg 是编译生成的.a 文件的存放位置是静态库

### src 是存放源文件的命令

### 安装问题 golang/x/tools > https://github.com/golang/tools

下载放到 src 下的 golang/x/下

### 安装问题 golang/x/net > https://github.com/golang/net

下载放到 src 下的 golang/x/下

### golint go get -u -v github.com/golang/lint/golint

### 编译时的使用第三方源码包 [编译时的探索](https://tonybai.com/2015/03/09/understanding-import-packages/)

1.  在使用第三方包的时候，当源码和.a 均已安装的情况下，编译器链接的是源码
2.  所谓的使用第三方包源码，实际上是链接了以该最新源码编译的临时目录下的.a 文件而已。
3.  标准库在编译时也是必须要源码的。不过与自定义包不同的是，即便你修改了 fmt 包的源码（未重新编译 GO 安装包），用户源码编译时，也不会尝试重新编译 fmt 包的，依旧只是在链接时链接已经编译好的 fmt.a
4.  Go 语言中 import 后面路径中最后的一个元素到底是包名还是路径名？答案是**目录名**。按照 Golang 语言习惯，一个 go package 的所有源文件放在同一个目录下，且该目录名与该包名相同，比如 libproj1/foo 目录下的 package 为 foo，foo1.go、 foo2.go…共同组成 foo package 的源文件。但目录名与包名也可以不同。
5.  一个路径下**不允许有两个包**。

### 同其他语言 nodejs 一样，一个包被导入一次后就会被缓存，再次被导入会从缓存获取

### init 函数的执行顺序

1.  对同一个 go 文件的 init()调用顺序是从上到下的
2.  对同一个 package 中不同文件是按文件名字符串比较“从小到大”顺序调用各文件中的 init()函数,对于
3.  对不同的 package，如果不相互依赖的话，按照 main 包中"先 import 的后调用"的顺序调用其包中的 init()
4.  如果 package 存在依赖，则先调用最早被依赖的 package 中的 init()
5.  最后调用 main 函数

### 关于关闭 channel 有几点需要注意的是：

1.  重复关闭 channel 会导致 panic。
2.  向关闭的 channel 发送数据会 panic。
3.  从关闭的 channel 读数据不会 panic，读出 channel 中已有的数据之后再读就是 channel 类似的默认值，比如 chan int 类型的 channel 关闭之后读取到的值为 0。

### make 和 new

1.  new allocate the memory and return the pointer 分配内存并且赋予零值
2.  make allocate the memory and initialize the memory cause slice, map and channel must be initialized before use
    new 的作用是 初始化 一个指向类型的指针 (_T)， make 的作用是为 slice, map 或者 channel 初始化，并且返回引用 T
    make(T, args)函数的目的与 new(T)不同。它仅仅用于创建 Slice, Map 和 Channel，并且返回类型是 T（不是 T_）的一个初始化的（不是零值）的实例。 这中差别的出现是由于这三种类型实质上是对在使用前必须进行初始化的数据结构的引用。 例如, Slice 是一个 具有三项内容的描述符，包括 指向数据（在一个数组内部）的指针，长度以及容量。在这三项内容被初始化之前，Slice 的值为 nil。对于 Slice，Map 和 Channel， make（）函数初始化了其内部的数据结构，并且准备了将要使用的值。

### 产品中一定不要使用默认的 http.Get

如果你觉得方便，直接使用 http.Get 或者类似的方法发送请求，可能会导致一些问题， 因为这默认是使用 DefaultClient 作为 client:

1.  多 goroutine 共享，这意味着在别处对 DefaultClient 的改动会影响你当前的使用
2.  未设置 connection timeout 和 read/write timeout
3.  默认的 idle connection 等设置可能不满足你的需求

### 检查网络错误或者超时 net.Error 和 err.Timeout() 可以检查是不是超时错误

```
if err, ok := err.(net.Error); ok && err.Timeout() {
    ……
}
```

### golang 命名返回值

```
func test()(a int) {
    return
}
```

实际上 a 相当于已经命名了，相当于

```
func test() int {
    var a int
    return a
}
```

### channel 关闭问题

*   通过 recover 来恢复来恢复
*   通过 Mutex 来保证不会向已关闭的 channel 发送消息
*   通过 sync.Once 来关闭，保证不会重复关闭好的原则是：

1.  不要在接收端关闭 channel, 避免向关闭的 channel 发送消息
2.  不要关闭有多个并发发送者的 channel

### golang 里面所有未赋值的变量都会赋值为默认值，就连内嵌 struct, infterface 也一样

*   内嵌 struct 会变为相应字段的默认值
*   内嵌 interface 为赋值就是 nil, 因为接口变量的默认值就是 nil(但 nil 和 nil 是不同的， 这里是个坑)

### 若是要 marshal 时，不要输出 null 字段或者是为空的字段（结构体），需要把它赋值为指针才可以然后加上 omitempty

https://stackoverflow.com/questions/18088294/how-to-not-marshal-an-empty-struct-into-json-with-go?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa

### http://colobu.com/2017/05/12/call-private-functions-in-other-packages/ 突破 golang 的访问限制

### reflect

*   reflect.TypeOf,是获取类型的元数据

```
reflect.TypeOf(i).Elem().Field(0).Tag //获取定义在struct里面的标签
```

*   reflect.ValueOf,是获取类型的值

```
reflect.ValueOf(i).Elem().Field(0).String() //获取存储在第一个字段里面的值
```

### http://www.01happy.com/p3206/

最后再次重复一遍反射三定律：

*   反射可以将“接口类型变量”转换为“反射类型对象”。
*   反射可以将“反射类型对象”转换为“接口类型变量”。
*   如果要修改“反射类型对象”，其值必须是“可写的”（settable）。

一旦你理解了这些定律，使用反射将会是一件非常简单的事情。它是一件强大的工具，使用时务必谨慎使用，更不要滥用。

### 只有可以 addressable 的变量才可以使用 reflect 进行赋值修改即：

wrong

```
var x float64 = 3.4
v := reflect.ValueOf(x)
v.SetFloat(7.1) // Error: will panic.
```

yes

```
var x float64 = 3.4
p := reflect.ValueOf(&x) // Note: take the address of x.
fmt.Println("type of p:", p.Type())
fmt.Println("settability of p:", p.CanSet())
```

> 存入 map 的是值是不可以 addressable 的 a = map [string]int; &a["s"], 这个很好理解因为，map 回去做冲突处理，地址有可能发生变化。

### [go web 的一些建议](https://medium.com/@matryer/how-i-write-go-http-services-after-seven-years-37c208122831

1.  Shared dependencies are fields of the structure
    通过结构体的字段共享依赖，而不是到处引入（适用于所有的）

2.  I have a single file inside every component called routes.go where all the routing can live:

```
package app
func (s *server) routes() {
    s.router.HandleFunc("/api/", s.handleAPI())
    s.router.HandleFunc("/about", s.handleAbout())
    s.router.HandleFunc("/", s.handleIndex())
}
```

This is handy because most code maintenance starts with a URL and an error report — so one glance at routes.go will direct us where to look.

3.  If a particular handler has a dependency, take it as an argument. 若是有特殊依赖则作为参数传入

4.

### https://github.com/dgryski/go-perfbook/blob/master/performance.md

### https://my.oschina.net/xinxingegeya/blog/729673 golang unsafe 包的 unsafe 特性

### 错误和异常，意料之内的是错误，意料之外的是异常(https://studygolang.com/articles/11753?fr=sidebar)

### 从标准输入读取

1.  os.Stdin 实现了 Reader 接口可以直接 Read

```
s := make([]byte, 10)
n, err := os.Stdin.Read(s)
```

2.  fmt.Scanf() 可以读取 so.Stdin

```
var firstname, secondname string
fmt.Scanln(&FirstName, &SecondNames)    //Scanln 扫描来自标准输入的文本，将空格分隔的值依次存放到后续的参数内，直到碰到换行。
fmt.Scanf("%s %s", &firstName, &lastName)    //Scanf与其类似，除了 Scanf 的第一个参数用作格式字符串，用来决定如何读取。
```

3.  os.Stdin 是个 Reader 但是没有缓冲， 可以使用 bufio.NewReader()生成一个带缓冲的

```
inputReader := bufio.NewReader(os.Stdin)
for {
    input, _ := inputReader.ReadString('\n')
    fmt.Println(input)
}
```


### Reader 接口 
1. Read 方法
也就是说，当 Read 方法返回错误时，不代表没有读取到任何数据。调用者应该处理返回的任何数据，之后才处理可能的错误。
io.EOF 变量的定义：var EOF = errors.New("EOF")，是 error 类型。根据 reader 接口的说明，在 n > 0 且数据被读完了的情况下，返回的 error 有可能是 EOF 也有可能是 nil。

### Writer 接口
1. Write 方法
Write 将 len(p) 个字节从 p 中写入到基本数据流中。它返回从 p 中被写入的字节数 n（0 <= n <= len(p)）以及任何遇到的引起写入提前停止的错误。若 Write 返回的 n < len(p)，它就必须返回一个 非nil 的错误。

### golang [比较行为](https://golang.org/ref/spec#Comparison_operators)
1. 指针只有指针指向的内容是同一个时才相等
2. 接口值可以比较， 但若是接口值得类型相同，但是值却是不可比较的例如slice等，就会报runtime error

Slice, map, and function values are not comparable

### func的receiver是 值时 无论调用者是指针还是值都可以调用func
如下若是使用指针作为receiver，则print 值时无法调用到String
```
type A struct {
	c int
}

func (a A) String() string {
	return "dj"
}

func format() {

	a := &A{c: 22}
	fmt.Println(a)
}
```

### bufio的readSlice返回的是bufio.Reader的里的buf（Reader缓存的slice），因此当再此读取以后返回的slice的值是改变的
> 一般的reader都是没有缓冲的，bufio就是给reader加缓冲的
```
func readSlice() {
	reader := bufio.NewReader(strings.NewReader("http://studygolang.com.\nIt is the home of gophers"))
	line, _ := reader.ReadSlice('\n')
	fmt.Println("the line is ", string(line))
	n, _ := reader.ReadSlice('\n')
	fmt.Println(string(n), string(line))
}
```
最后n和slice的值相同

而ReadBytes和ReadString就不会出现这个问题了，他们都新开辟的空间


如果ReadSlice在找到界定符之前遇到了error，
它就会返回缓存中所有的数据和错误本身（经常是 io.EOF）。
如果在找到界定符之前缓存已经满了，ReadSlice会返回bufio.ErrBufferFull错误。
当且仅当返回的结果（line）没有以界定符结束的时候，ReadSlice返回err != nil，
也就是说，如果ReadSlice返回的结果line不是以界定符delim结尾，那么返回的err也一定不等于nil
（可能是bufio.ErrBufferFull或io.EOF）。
并且当下次在读取时会继续向前
```
func ReadSliceSize() {
	reader := bufio.NewReaderSize(strings.NewReader("http://studygolang.com.\nIt is the home of gophers"), 10)
	line, _ := reader.ReadSlice('\n')
	fmt.Println("the line is ", string(line))
	n, _ := reader.ReadSlice('\n')
	fmt.Println(string(n), string(line))
}
```

然而使用ReadBytes和ReadString就没有这个问题了，bufio.reader会自动为我们处理。

### golang排序
对基本类型排序
1. int, float等
sort包有sort.Ints, sort.Float64s等方法排序，会改变原数组
2. 其他类型则需要实现 sort.Interface接口了 ，如less,等

### 程序中使用time.Time
程序中应使用 Time 类型值来保存和传递时间，而不是指针。就是说，表示时间的变量和字段，应为time.Time类型，而不是*time.Time.类型。一个Time类型值可以被多个go程同时使用

### time 格式化
这是实际开发中常用到的。

1. time.Parse 和 time.ParseInLocation
2. time.Time.Format
解析
对于解析，要特别注意时区问题，否则很容易出 bug。比如：

t, _ := time.Parse("2006-01-02 15:04:05", "2016-06-13 09:14:00")
fmt.Println(time.Now().Sub(t).Hours())
2016-06-13 09:14:00 这个时间可能是参数传递过来的。这段代码的结果跟预期的不一样。

原因是 time.Now() 的时区是 time.Local，而 time.Parse 解析出来的时区却是 time.UTC（可以通过 Time.Location() 函数知道是哪个时区）。在中国，它们相差 8 小时。

所以，一般的，我们应该总是使用 time.ParseInLocation 来解析时间，并给第三个参数传递 time.Local。

### string和number互转
1. ParseBool, ParseFloat, ParseInt, and ParseUint convert strings to values
2. FormatBool, FormatFloat, FormatInt, and FormatUint convert values to strings:

### number和byte (数字和字节互转)


### 想进行地址的加减操作就得使用uintptr

```
bts := [5]byte{2, 3, 4, 5, 6}
fmt.Println(bts)
c := (*[4]byte)(unsafe.Pointer((uintptr(unsafe.Pointer(&bts)) + uintptr(1))))[:]
fmt.Println(c)
```
输出正确，直接使用 slice就错误（bts := []byte{2,3,4,5,6}）
原因是：bts为slice的时候 bts的内部结构是
```
type SliceHeader struct {
        Data uintptr
        Len  int
        Cap  int
}
```
也就是说转化为[]byte以后 只有Data的地址，Len和Cap的值被转化为了字节

### string 和 []byte 无copy转换
// reflect.SliceHeader and reflect.StringHeader
```
type SliceHeader struct {
        Data uintptr
        Len  int
        Cap  int
}

type StringHeader struct {
        Data uintptr
        Len  int
}
```
```
/*
  struct string{
    uint8 *str;
    int len;
  }
  struct []uint8{
    uint8 *array;
    int len;
    int cap;
  }
  uintptr是golang的内置类型，是能存储指针的整型，uintptr的底层类型是int，它和unsafe.Pointer可相互转换。
  但是转换后的string与[]byte共享底层空间，如果修改了[]byte那么string的值也会改变，就违背了string应该是只读的规范了，可能会造成难以预期的影响。
*/
func str2byte(s string) []byte {
    x := (*[2]uintptr)unsafe.Pointer(&s)
    h := [3]uintptr{x[0],x[1],x[1]}
    return *(*[]byte)(unsafe.Pointer(&h))
}
func byte2str(b []byte) string{
    return *(*string)(unsafe.Pointer(&b))
}
```

### golang json问题
1. json tag `json:"name,omitempty,type"` json: "name,[option]"
2. int 为0时，若tag 为omitempty, int会不被序列化出来， omitempty，tag里面加上omitempy，可以在序列化的时候忽略0值或者空值
    解决办法是使用 指针
    ```
    type Test struct {
        String  *string `json:"string,omitempty"`
        Integer *int    `json:"integer,omitempty"`
    }
    ```
3. int8 byte uint8 json序列化的时候 会被当做字符串处理
```
type CA struct {  
    List []uint8  
}  
  
func main() {  
    ca := CA{[]uint8{1,2,3,4,5,6,7,8,9,0}}  
    r, _ := json.Marshal(ca)  
    fmt.Println(string(r)) //{"List":"AQIDBAUGBwgJAA=="}  
}  
```


### 当我们想给某操作加缓冲区时就可以使用bytes.Buffer

### 只有make的map才可以赋值， new的是不行的, var声明的也不行
```
package main

import "fmt"

type Param map[string]int
type Show struct {
	Param
}

func main() {
	s := new(Show)
	fmt.Println("dj ", s.Param)
	s.Param["dengjie"] = 123
	
}
// 报错
```

### 可被json处理都是对外的字段
```
type People struct {
	name string `json:"name"`
}

func main() {
	js := `{
		"name":"11"
	}`
	var p People
	err := json.Unmarshal([]byte(js), &p)
	if err != nil {
		fmt.Println("err: ", err)
		return
	}
	fmt.Println("people: ", p)
}
// {} 因为name是小写开头未暴露
```

### 结构体实现String接口时内部不要调用，fmt的包，会无限递归，因为调用fmt.Println(p)时是会调用 p的String方法

```
type People struct {
	Name string
}

func (p *People) String() string {
	return fmt.Sprintf("print: %v", p)
}

func main() {
	p := &People{}
	p.String()
}
```

### send on closed channel
```
func main() {
	ch := make(chan int, 1000)
	go func() {
		for i := 0; i < 10; i++ {
			ch <- i
		}
	}()
	go func() {
		for {
			a, ok := <-ch
			if !ok {
				fmt.Println("close")
				return
			}
			fmt.Println("a: ", a)
		}
	}()
	close(ch)
	fmt.Println("ok")
	time.Sleep(time.Second * 100)
}
```
### 类型断言
```
type student struct {
	Name string
}

func zhoujielun(v interface{}) {
	switch msg := v.(type) {
	case *student, student:
		msg.Name
	}
}
// 错误 因为 v.(type)返回的是v的类型，是interface{}， 不能操作，v.(student).Name才可以
```

### time.Sleep 是以纳秒 为单位的
```
time.Sleep(time.Second * 100000000000000)
// 一乘就超出了int64的范围
``` 


### golang 面试题 https://zhuanlan.zhihu.com/p/35058068 

### (golang的奇葩1)[https://juejin.im/post/5b5bd2ddf265da0f716c2fea?utm_source=gold_browser_extension]
### (golang奇葩2)[https://mp.weixin.qq.com/s/gjOTdEVyhHAiG4nK_ZrOVw]

### channel 必须使用make初始化

### PS在Tx中唯一绑定一个连接，不会re-prepare。

Tx和statement不能分离，在DB中创建的statement也不能在Tx中使用，因为他们必定不是使用同一个连接使用Tx必须十分小心，例如下面的代码：
```
tx, err := db.Begin()
if err != nil {
    log.Fatal(err)
}
defer tx.Rollback()
stmt, err := tx.Prepare("INSERT INTO foo VALUES (?)")
if err != nil {
    log.Fatal(err)
}
defer stmt.Close() // danger!
for i := 0; i < 10; i++ {
    _, err = stmt.Exec(i)
    if err != nil {
        log.Fatal(err)
    }
}
err = tx.Commit()
if err != nil {
    log.Fatal(err)
}
// stmt.Close() runs here!
```
*sql.Tx一旦释放，连接就回到连接池中，这里stmt在关闭时就无法找到连接。所以必须在Tx commit或rollback之前关闭statement。

> 当使用tx时所有的操作必须在commit或者rollback之前关闭


### tcp 设置超时时间

https://github.com/developer-learning/night-reading-go/blob/master/discuss/2018-06-07-dial-timeout-in-go.md


### 你需要知道的知识点有两个，一是go的参数都是值传递，二是只有用同一把锁才能对某个资源边界进行锁与解锁的操作。

```
type ExecuterList struct {
	sync.Map
	length int
}

func (e ExecuterList) Get(key string) IExecuter {
	value, ok := e.Load(key)
	if !ok {
		return nil
	}
	if value == nil {
		return nil
	}
	res, _ := value.(IExecuter)
	return res
}
```
使用 go tool vet ，出现“Get passes lock by value: ExecuterList contains sync.Map contains sync.Mutex”， 解决方案有两种：

1，sync.Map用指针
```
type X struct {
	*sync.Map
}
```
2， 也可以用 (e *ExecutorList) ,避免锁的复制。

### context.withCancel http://www.zenlife.tk/with-cancel.md

### go 为什么会有nil channel https://lingchao.xin/post/why-are-there-nil-channels-in-go.html


### 当我们需要确保某一个类型实现了某一固定接口时可以在代码顶部这么写
```
var _ HelloServiceInterface = (*HelloServiceClient)(nil) // 确保 HelloServiceClient 实现了 HelloServiceInterface
```
不用但声明，若没有实现编译时就会报错


### copy内置的copy https://golang.org/ref/spec#Appending_and_copying_slices

### https://wuyin.io/2018/03/07/50-shades-of-golang-traps-gotchas-mistakes/

### method的赋值会依然带着接收者，即 a.cMethod() === 'd := a.cMethod \n d()' 怎么样炫酷吧，方法的赋值其实也是带着接收者的
```
package main

type A struct {
	a int
}

func (a *A)set(c int) {
	a.a=c
}

func main() {
	a := A{123}
	c := a.set
	a.set(123)
	c(0)
}
```
