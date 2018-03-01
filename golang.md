### golang 的所有的都是copy赋值，=，传参，channel发送等等都是。

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
stus是个map, stu并不会每次新生成一个, 其实循环时每次都是相同的stu, 而且golang每次都是copy语义，你再看一眼会发现，给map赋值的是指针，那就意味着无论你如何改变map都会是相同的值。:= 多次声明不会重新定义新的变量， 可见[规范](https://golang.org/ref/spec#Short_variable_declarations)

见下:
```
field1, offset := nextField(str, 0)
field2, offset := nextField(str, offset)  // redeclares offset
a, a := 1, 2                              // illegal: double declaration of a or no new variable if a was declared elsewhere
```

### golang的字符串是不可变的，要想使用可变字符串可以使用bytes，或者[]rune数组, string类型不可变，他的slice也不可变

### 我们可以从关闭的channel中读取数据但是为空，就是说若是在select语句case中从关闭的channel是可以的。 [参见](https://play.golang.org/p/zqR533gQiSJ)
```
select {
case <- stopCh:
  fmt.Println("go")
default: 
  fmt.Println("come")
}
```

### 在使用Go channel的时候，一个适用的原则是不要从接收端关闭channel，也不要关闭有多个并发发送者的channel。 [优雅关闭go channel](https://www.jianshu.com/p/d24dfbb33781)
channel的使用，当channel是非缓冲的时候他就是阻塞读与写的， 所以使用channel的时候要小心同步阻塞，导致死锁game over
```
ch := make(chan string)
ch <- "23" // 此时就会阻塞
// a <- ch 单写他也是会阻塞的
```
所以要小心不要出现只有读或者只有写的channel 若是出现的话，分布在不同goroutine中时就会导致golang死锁，主在等子完成可是子阻塞在了channel读或者写上了。
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

### golang make 返回的是值类型，用slice, map, channel, 并且会给map 和slice预分配空间

### golang slice语法可以对值也可以对引用
```
a = [1,2,34]
t := a[1:3] // ok
c := (&a)[1:2] // ok
```

### golang 切片的用法是 [start:end] 但不包括end

### new 返回的是指针类型

### golang 也存在js中的那个经典问题就是
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
第一个打印的i都是10， 因为他们打印的都是同一个变量i。

### golang 中return defer 返回值的顺序
先来假设出结论，帮助大家理解原因：

多个defer的执行顺序为“后进先出”；

defer、return、返回值三者的执行逻辑应该是：return最先执行，return负责将结果写入返回值中；接着defer开始执行一些收尾工作；最后函数携带当前返回值退出。

如何解释两种结果的不同：

上面两段代码的返回结果之所以不同，其实从上面第2条结论很好理解。

a()int 函数的返回值没有被提前声名，其值来自于其他变量的赋值，而defer中修改的也是其他变量，而非返回值本身，因此函数退出时返回值并没有被改变。

b()(i int) 函数的返回值被提前声名，也就意味着defer中是可以调用到真实返回值的，因此defer在return赋值返回值 i 之后，再一次地修改了 i 的值，最终函数退出后的返回值才会是defer修改过的值。

defer是在函数结束前执行的，当返回值的临时变量赋给外部时才算调用结束吧！a = fn(2) 当把值给了a才算结束

> 我们可以这么理解return肯定先执行执行的结果就是把返回值计算出来并且赋值给返回值所存在的临时变量, 但我们命名返回值的时候，返回值并不是临时变量而是函数中声明的变量

### 在函数有多个返回值时，只要有一个返回值有指定命名，其他的也必须有命名

### append 只能作用在slice上而不能是指针
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

### 进行结构体比较时候，只有相同类型的结构体才可以比较，结构体是否相同不但与属性类型个数有关，还与属性顺序相关。并且结构体可比较的前提是结构的属性都可比较。map和slice还有function不可比较
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
此处sn1可以和sn2 使用==比较
但是如下
```
sn3:= struct {
    name string
    age  int
}{age:11,name:"qq"}
```
sn3就不能比较了

还有上例中，含有不可比较的map,slice，func等，所以sm1和sm2是不可比较的
但是我们可以使用deepEqual来进行比较
```
if reflect.DeepEqual(sn1, sm) {
    fmt.Println("sn1 ==sm")
}else {
    fmt.Println("sn1 !=sm")
}
```

### := 赋值模式的限制
定义变量同时显式初始化
不能提供数据类型
只能在函数内部使用

### nil 可以用作 interface、function、pointer、map、slice 和 channel 的“空值”, 是不可以作为其他类型的空值的，比如String, string 的空值是“”
```
var a chan int
// a 是空值nil
// chan的初始化一定是用make

var a map[int]string
// a 是空值nil
// map初始化一定使用make
// 使用new 生成的map也是nil的map
```


###  itoa
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

### golang const 常量可以使用itoa赋值， 甚至是自定义类型
[参见](https://segmentfault.com/a/1190000000656284)

若是自定义类型类似于枚举，当我们传递字面量（如：2，3）时也是可以被识别为枚举值的。

* 当在一行声明两个常量itoa时，itoa是只有到了下一行才会增长
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
* itoa 从0开始增长，当我们们不使用itoa时，且赋值一个则所有的都会是这个值
```
const (
  a = "1"
  b
  c
  d
)
```
### goto不能跳转到其他函数或者内层代码, 只能在本函数以及相同的scope内
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

### 注意defintion和type alias的区别 defintion是定义了一个新的类型，alias仅仅是个别名
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
### 1. 我们在返回值中定义的命名返回值，可以直接在函数中使用不用再重新定义 2. 内部scope中新定义的内部变量是会覆盖掉外部的变量
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
### panic仅有最后一个可以被revover捕获,panic中可以传任何值，不仅仅可以传string
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

### map引用不存在的key，不报错

### map使用range遍历顺序问题，并不是录入的顺序，而是随机顺序

### append函数返回更新后的slice（长度和容量可能会变），必须重新用slice的变量接收，不然无法编译通过

### golang channel和一个goroutine组合起来就是一个web-worker的模式，只是我们在js里管理的是web-worker的句柄，而在golang 里面我们要管理的是channel,这里golang的一个优势是我可以把多个channel和一个goroutine绑定，而web-worker一个句柄就对应这个一个web-worker是不可能多对一的。

### golang channel的使用方式两种
1. 调用者生成传递个goroutine。
2. 被调用者生成返回给调用者。