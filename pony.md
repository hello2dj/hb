```
class Box[A: Stringable]
    fun string[B: I32 = A](a': B) => 
        a'
```
[B: I32 = A]的写法是A必须是I32的subtype


### recover 中只能访问sendable
如果访问(其实是赋值行为 assign)，ref, trn, box 都会变成tag


### tag trait
```
trait tag HasName
  fun ref name(x: String) => None

class A is HasName
    var b: String = ""
    fun ref name(x: String) =>
        b = x

actor Main
  new create(env: Env) =>
    let a: A = A // ok
    let a: HasName = A // wrong
    a.name("dgnei")
```
因为HasName的要求是tag，而hasName的要求是ref，所以挂了，而第一种声明方式就是可以的

### 相等
1. is 判断 是否是同一个元素
2. ==(实现eq 方法即可) 判断数据是否相等

还有个interface, pony并不判断元素是否实现了Equatable接口只是直接调用了eq 方法（使用‘==’时），其他传参时若是约定了Equatable接口 那就必须得实现了
```
interface Equatable[A: Equatable[A] #read]
  fun eq(that: box->A): Bool => this is that
  fun ne(that: box->A): Bool => not eq(that)

class B
    fun say(x: Equatable[I32]): Bool => // 此时传入的x就必须实现Equatable接口了
```

