---
title: Golang 高效的错误处理
tag:
- golang 
- error
---
> [原文地址](https://morsmachine.dk/error-handling)

![](https://ideassea.com/content/images/2017/07/golang-error-handling-1-1.jpg)
#### 简介
golang 的错误处理方式一直是他遭受抨击的一个原因之一。探查每一个的错误然后处理确实是一个艰巨的任务，这里有几招可以让你减少错误的处理方式（处理错误的方法）。

### 错误处理靠前
当我们写golang的时候倾向于
```
f, err := os.Open(path)
if err != nil {
  // handle error
}
// do stuff
```
而不是
```
f, err := os.Open(path)
if err == nil {
  // do stuff
}
// handle error
```
这种方式可以让我们的正常处理方式一路看到底，而不是if之后是错误

### 定义属于自己的错误
处理错误的第一步得是知道错误是啥，如果你的package发生了错误，那么你的用户一定对错误的原因很感兴趣。要做到让你的user知道错误是啥，你只需要实现error interface, 如下就可以了
```
type Error string
func (e Error) Error() string { return string(e) }
```
这样你的用户就可以通过类型断言来判断是否是你的错误
```
result, err := yourpackage.Foo()
if err, ok := err.(yourpackage.Error); ok {
  // use tp to handle err
}
```
你也可以暴露一个结构体的error给你的用户
```
type OpenError struct {
  File *File
  Error string
}
func (oe *OpenError) Error() string {
  // format error string here
}

func ParseFiles(files []*File) error {
  for _, f := range files {
    err := f.parse()
    if err != nil {
      return &OpenError{
        File: f,
        Error: err.Error(),
      }
    }
  }
}
```
通过这种方式，你的用户就可以分辨具体是哪个文件解析失败了
但是当你包裹错误的时候你也应当注意，因为包裹一个error，会丢失一些信息，就好比上面那个你已经丢失了err的类型而只剩下了err里的信息
```
var c net.Conn
f, err := DownloadFile(e, path)
switch e := err.(type) {
  default:
    // this will get executed if err == nil
  case net.Error:
    // close connection, not valid anymore
    c.Close()
    return e
  case error:
    // if err is non-nil
    return err
}
// do other things
```
见上如果此时你包裹了net.Error, 那么这段代码就不会看到net.Error这个错误了，而只能是一段错误信息，此时就无法具体区分错误了（见前一段的包裹方式丢失了具体的错误类型）
一个好的处理方式是尽量不要包裹你调用的其他包自己产生的错误，因为用户可能更关心他们产生的错误而不是你的。

### 把错误当做状态
有时候你可能想要持有一个错误而不是抛出，不管你是打算随后上报或者是你知道这个错误很快就会再次出现

这种情况的一个栗子就是bufio这个包。当bufio.Reader遇到一个错误，他会持有这个错误一直到buffer空为止，只有此时他才会上报这个错误。
另一种栗子就是使用go/loader。当使用参数调用遇到错误时，他会持有这个错误，因为有很大的概率他会再次使用相同的参数又调用一遍

### 使用函数去避免重复
如果你有一段错误处理的代码是重复的，你可以把他提出来做一个函数
```
func handleError(c net.Conn, err error){
  // repeated error handling
}
func DoStuff(c net.Conn) error{
  f, err := downloadFile(c, path)
  if err != nil {
    handeError(c, err)
    return err
  }
  f, err := doOtherThing(c)
  if err != nil {
    handleError(c, err)
    return err
  }
}
```
一个替换方案是
```
func handeError(c net.Conn, err error) {
  if err == nil {
    return
  }
  // repeat err handling
}

func DoStuff(c net.Conn) error {
  // defer func(){ handleError(c, err) }()
  // 这是原文在的位置，但明显有错误啊，err未定义啊
  f, err := downloadFile(c, path)
  defer func(){ handleError(c, err) }()// 所以我挪到这里了，但效果是否ok，暂未验证 
  if err != nil {
    return err
  }
  f, err := doOtherThing(c)
  if err != nil
}
```

ps: 一个golang官网的一段翻译
### [Why is my nil error value not equal to nil?](https://golang.org/doc/faq#nil_error) 为什么我的nil error 不等于nil呢？
先上代码
```
func returnError() error{
  var p *MyError = nil
  if bad() {
    p = ErrBad
  }
  return p // Will always return a non-nil error
}
```
如上这里的p明明是nil值为啥返回后就不是了呢？这里涉及到另外一个问题就是interface值，我们知道error是一个interface，而MyError是一个struct, 就是要把struct值赋值给interface,在golang里interface值是包括两个的一个type值，一个value值，只有当两者都是nil的时候interface值才是nil, 上述代码，很明显虽然value是nil,可是类型还在啊，就是说type不是你nil而是MyError, 改进如下
```
func returnsError() error {
  if bad() {
    return ErrBad
  }
  return nil
}
```