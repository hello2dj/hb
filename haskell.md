### 函数调用拥有最高优先级， 例如如下两句等效
```
ghci> succ 9 + max 5 4 + 1   
16   
ghci> (succ 9) + (max 5 4) + 1   
16 
```

### 默认左结合（一堆函数调用时），. 相当于compose函数组合 优先级是9（最高）， $ 组合参数右结合 优先级是0（最低）
```
f $ g $ h x = f (g (h x))
```

### 函数调用时最高的，甚至高于.
```
a = (5 +)
a . a 4 // 报错 <=> a . (a 4)
(a . a) 4 // ok
```
如下替换也是可以的
```
a $ a 4
```
因为haskell调用默认是左结合，那就而 $ 运算符 是右结合的，为什么因为 a $ a 4这是个中缀写法相当于自带左右括号（a） $ (a 4)

就是说$相当于把 表达式分为前后两部分带括号的

### Functor
```
fmap = (a -> b) -> F a -> F b
<$> = fmap
```

### [有关data 定义的类型解释](http://rwh.readthedocs.io/en/latest/chp/3.html#id16)
我们可以使用data来定义记录类型
```
data BookInfo = Book Int String [String]
                deriving (Show)

// 我们也可以这么来定义记录类型
data BookInfo = Book {
  id:: Int,
  name:: String,
  address:: String,
}
```
接下来我们就可以使用属性访问器来访问BookInfo中的内容了
例如: 
```
a = BookInfo {
      id = 271828
    , name = "dengjie"
    , address = "Jane Q. Citizen"
  }
id a // 得到的就是271828
name a // 就是"dengjie"
address a // Jane Q. Citizen
```
可以看到属性访问器就是 data 定义类型时的字段名字
其实属性访问器就是结构与模式匹配,我们可以这么来写
```
id :: BookInfo -> Int
id (BookInfo id _ _ ) = id

// 其他类似
```

### Applicative: Functor
pure :: a -> A a
<*> = A (a -> b) -> A a -> A b

### 我们所处理的数据其实都是在上文中的，好比对象的属性等，这些模式可以使我们轻松的操作上下文中的数据
1. Functor ：使用 fmap 应用一个函数到一个上下文中的值；
2. Applicative ：使用 <*> 应用一个上下文中的函数到一个上下文中的值；
3. Monad ：使用 >>= 应用一个接收一个普通值但是返回一个在上下文中的值的函数到一个上下文中的值。

### monad
return :: a -> m a
(>>=) :: m a -> (a -> m b) -> m b
(>>) :: m a -> m b -> m b
x >> y = x >>= \_ -> y

### [关于state Monad的解释](https://en.wikibooks.org/wiki/Haskell/Understanding_monads/State)
let 定义变量 in 变量使用
let a = 12 in print a

### [关于monad transformer的理解](https://zh.wikibooks.org/zh-hans/Haskell/Monad_transformers)

### 在我看来monad就是处理上下文中内容的一种模式，或者说是一套规范的处理流程。

### 在haskell lift是提升的意思

### [关于类型类](http://rwh.readthedocs.io/en/latest/chp/6.html)

