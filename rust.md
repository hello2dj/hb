---
title: rust
date: 2018-01-03 20:14:31
tag:
- rust-lang
---
### rust 默认语义是move(也是通过按字节copy实现的)

### rust中的变量是 某块内存的名字， mut 标志的是名字对应的内存区域是否可变
```
let mut x = 123;
let mut y = &mut x;

// &mut x中的mut标志的是x对应的内存区域是否可变，mut y 标志的是 y对应的内存区域是否可变，y的内存区域存储的是x对应内存的地址
```

### rust enum 是不可以使用 == 进行比较的， 其他比较也是不可以的

### rust的所有权问题当一个结构体中的一个字段move出去以后那么整个结构体就不能再使用了，但是，内部字段还可以继续使用
```
#[derive(Debug)]
struct MoveStruct {
  x: Box<i32>,
  y: Box<i64>,
}

fn main() {
  let moveTest = MoveStruct{ x: Box::new(23), y: Box::new(43) };
  let d = moveTest.x;
  let x = moveTest;
  // let c = moveTest.y;
  println!("hello2dj {:?}", x);
}
// 报错
src/main.rs:64:7
   |
63 |   let d = moveTest.x;
   |       - value moved here
64 |   let x = moveTest;
   |       ^ value used here after move
   |
   = note: move occurs because `moveTest.x` has type `std::boxed::Box<i32>`, which does not implement the `Copy` trait
```
但若是不在使用moveTest就可以了
```
  let d = moveTest.x;
  // let x = moveTest;
  let c = moveTest.y;

  ok
```
### rust 的结构体，只要结构体是可变的，那么内部字段就都是可变的，不存在部分可变，部分不可变的情况（RefCell不算）

### rust 在调用*解引用时是这么的步骤
1. a = U;
2. 若是U实现了Deref，则*(a.deref())
3. 若是未实现Deref, 则查看U 是否是&T，即是否是引用类型
4. 若是则返回T
5. 若不是则报错 类似于 type `{integer}` cannot be dereferenced

### rust 的变量一旦被可变借用后，其原始值就不能再用了, 再有其他借用也不可以，无论是可变还是不可变。
```
let mut a = 23;
let c = &mut a;
*c = 234;
println!("{}", a)

// 这是会报错的，因为a已经被可变借用了。

let mut a = 23;
let c = &a;
println!("{}", a)
```

### 总结一下
1. 若是被不可变借用了，那么不管原始变量是否可变，原始变量及其不可变借用，都是只读的。
2. 若是被可变借用了，前提得是原始变量是可变的，此时只能有一个可变借用且原始变量也不可变，并且原始变量也不允许在被借用了，无论可变还是不可变。
3. 可变不共享，共享不可变。

### 结构时也是遵循copy和move,以及借用规则的
```
#[derive(Debug)]
struct A {
    a: i32,
    b: B
}

#[derive(Debug)]
struct B {
    c: i32,
    d: i32
}

fn main() {
    let pair = (0, -2);
    let pa = A{a: 2, b: B {c: 23, d:23}};
    // 试一试 ^ 将不同的值赋给 `pair`
    let A { a:_, b: d } = pa; // 此处相当于move pa.b 给了d
    println!("Tell me about {:?}", pair);
    // match 可以解构一个元组
    match pa {
        // 绑定到第二个元素, 此处也是如此把 pa.d move给了 d
        A{ a: _, b: d} => println!("First is `0` and `y` is `{:?}`", d),
        _      => println!("It doesn't matter what they are"),
        // `_` 表示不将值绑定到变量
    }
    println!("Tell me about {:?}", pa);
}

// 上面编译是会报错的
use partially moved value: pa
```

### rust match 表达式返回最后一个不带分号的表达式的结果
```
let c : i32 = match result {
  Err(err) => {
    2
  }
  Ok(record) => {
    1
  }
};

// 都带上分号则c的类型是();
let c : () = match result {
  Err(err) => {
    2;
  }
  Ok(record) => {
    1;
  }
};

```

### 这是一种方法调用， From::from(type), 调用type的from方法
### rust的方法调用与golang一样，无论方法定义时的接收者是引用还是值，这两个方法都会替我们进行解引用或者引用直到找到符合的方法。

### rust中有一个trait object， Box<Error>, 并且给他实现了
```
impl<'a, E: Error + 'a> From<E> for Box<Error + 'a>
```
这意味着我们都可以把Error类型转换到Box<Error>

### rust的mod也可以以文件的形式组织，当前提是在使用时得先写mod module, 来引入
```
// my.rs
// main.rs
mod my;
....
```

### only auto traits can be used as additional traits in a trait object
```
fn return_closure(idx: Arc<AtomicUsize>) -> Box<FnOnce() -> () + Send + Sized> { // Sized 报错 因为Sized 不是auto traits
  Box::new(move|| {
    idx.fetch_add(10, Ordering::SeqCst); 
  })
}
```

### auto traits 是指编译器会自动实现的trait, 例如Send , Sync


### 当编译器做使用了如下三个规则来推测生命周期之后还有未推测出来的生命周期时就会报错，也就意味着，这些还未推测出来的生命周期需要我们手动声明
1. 每一个函数参数都有自己的生命周期参数，举个栗子就是有一个参数就有一个生命周期参数，有两个就有两个生命周期参数，有n个就有n生命周期参数，以次类推（这里意味着&self和&mut self也有自己的生命周期参数）
```
fn foo<'a>(x: &'a i32)
fn foo<'a, 'b>(x: &'a i32, y: &'b i32)
```
2. 如果只有一个输入的生命周期参数(就是函数参数只有一个生命周期参数1. 只有一个引用参数，2. 所有引用参数的生命周期参数都相同)，那么这个生命周期参数就会赋给所有的输出生命周期参数(就是返回值)
```
fn foo<'a>(x: &'a i32) -> &'a i32

fn foo<'a>(x: &'a i32, x: &'b i32) -> &'a i32
```
3. 若果方法有多个输入生命周期参数，不过其中有一个参数是&self或者&mut self, 那么self的生命周期参数会赋给所有的输出生命周期参数，这使得方法写起来简单些
```
fn foo(&'a self) -> &'a str
```

### 生命周期其实也是种泛型，所以当函数使用生命周期时一定得像使用泛型一样把生命周期写在方法名后面
```
use std::fmt::Display;

fn longest_with_an_announcement<'a, T>(x: &'a str, y: &'a str, ann: T) -> &'a str
  where T: Display
{
  println!("Announcement! {}", ann);
  if x.len() > y.len() {
    x
  } else {
    y
  }
}
```

### 生命周期默认是'static的有
1. &str
2. trait object
3. closure

### lifetime 就是scope,相当于scope的泛型，而我们的泛型相当于自带lifetime
```
struct A<T> { // T可以是&类型，就相当于&'a SomeType,不需要在另外声明
  a: T,
}
```

### 对与结构体的引用，是不予许把引用的字段move出去,简而言之就是引用的内容是不允许move的
cannot move out of borrowed content
```
use std::sync::Arc;
struct A<T> {
  it: Arc<T>, // 这里的T可以是引用那么他的生命周期是如何确定的
  ie: B<T>,
}


struct B<T> {
  it: Arc<T>,
 
}

fn main() {
    let c = 32;
    let d = &A{
        it: Arc::new(23),
        ie: B { it: Arc::new(23) },
    };
    let e = d.it;
    // 会报错的 
   |  let e = d.it;
   |             ^---
   |             |
   |             cannot move out of borrowed content
   |             help: consider using a reference instead: `&d.it`

error: aborting due to previous error

error: Could not compile `playground`.
} 
```
见上面的代码，若d不是引用则可以move，但是move过后的d是不允许在使用，只能再move出去其他字段，若是再使用的话回报use of partially moved value: `d`，这个错。

解决引用无法move字段的方法
1. clone,或者copy,不要使用move
2. std::mem::replace, 可以用一个新值，或者空值来替换掉原始字段值，并返回原始字段值

### 关于generic reference 的问题[stackoverflow](https://stackoverflow.com/questions/36759472/rust-constraints-for-generic-type-reference)
通过他我们知道了，当我们的trait写是self时，那我们的self的类型就是实现的self,比如我们的trait A接收的是self参数，然后我们给i32实现了trait A, 那么接收者就必须得是i32, 再好比我们给&'a i32实现了trait A，那么接收者就的是& i32。总结就是：trait 给谁实现了比如B, 那么方法的self的类型就是B。
```
trait Operation {
    fn operation(self);
}
impl Operation for u32 {
    fn operation(&self){ // 这里就会报错因为接受者参数写成了&self, 而不是self,这里的self就是u32
        self;
    }
}

// 调用的时候就可以使用
let a = 32u32;
a.operation();

// 甚至使用&来调用
let a = 32u32;
let d = &a;
d.operation();
```
通过上面的栗子，我们可以看到，rust在我们进行方法的调用的时候回自动给我们解引用，但当我们的类型是不可copy的时候，自动解引用就会触发move，那么就会报错了,见下
```
trait Operation {
    fn operation(self);
}

struct Container<T> {
    x:T
}

impl<T> Operation for Container<T> {
    fn operation(self){
        println!("dengjie");
        self;
    }
}



fn main() {
    let a = Container {
        x: 32u32,
    };
    let d = &a;
    d.operation(); // 报错我们不可以把借用的内容move出去， 除非我们Container实现了Copy.
    // cannot move out of borrowed content
    
}
```

### 当我们使用&和&mut时我们完全可以把他们变为* const和* mut 来使用unsafe代码来修改数据，但是unsafe代码一定得留神

### Box<T>类型和其他结构体类型不同，其他结构体其中字段被move出去以后，那么整个结构体不可再move但是其中的其他字段还是可以继续使用，Box则不同，一个字段被move以后其他字段也不能使用了。
原因是 Box是分配在堆上的空间，若是分别move，会造成堆空间的重复释放，而struct的栈上空间就不存在这个问题


### for syntax
https://stackoverflow.com/questions/35592750/how-does-for-syntax-differ-from-a-regular-lifetime-bound


### lifetime的省略机制
1. 每个带生命周期参数的输入参数，每个对应不同的生命周期参数；
2. 如果只有一个输入参数带生命周期参数，那么返回值的生命周期被指定为这个参数；
3. 如果有多个输入参数带生命周期参数，但其中有 &self、&mut self，那么返回值的生命周期被指定为这个参数；
4. 以上都不满足，就不能自动补全返回值的生命周期参数。

### trait, impl trait, &trait, Box<trait>
https://joshleeb.com/posts/rust-traits-and-trait-objects/


### rust 关于方法调用的dref和ref
https://stackoverflow.com/questions/28519997/what-are-rusts-exact-auto-dereferencing-rules/28552082

定义方法实现时 方噶的receiver只能是self, &self, &mut self, 若是其他的没有self receiver 则没有限制， 含有self的只能是上面三种形式
1. self,
2. &self,
3. &mut self,

调用时rust会自动添加ref或者dref来进行适配
```

struct A {
    a: i32    
}

impl A {
    fn say(&self) {
        println!("{:?}", self.a);
    }
}

fn main() {
    let c = &&&A {a : 234};
    c.say();
}
```
如上我们给A 定义了一个&self,但我们的调用确是&&&A, rust 会自行derf到&A

当然 A和会继承Deref的方法，因为rust在调用方法时会自动去适配Deref A的

对一个不能copy的Struct既有self又有&self调用是会报错的
##### 总结一下
1. rust会自动添加&
2. rust会自动添加*
3. rust会自动调用Deref
4. 对与调用实现的方法来说只要能通过上述方法从调用者a匹配到receiver self就可以

### str Vs String
https://stackoverflow.com/questions/24158114/what-are-the-differences-between-rusts-string-and-str/24159933#24159933

### 使用rust要牢记
1. Box<T>的中的字段若move了其中一个字段，则意味着整个Box不能再被使用了，再次部分move也是不允许的。
2. 普通Struct是可以多个字段被move的，但整体Struct就不能再使用了
```
struct A {
    a: Box<i32>,
    b: Box<String>,
}

fn test() {
    let a = A{a..., b...};
    let d = a.a; // ok
    let e = &a; // error
    let c = a.b; // ok

    let a = Box::new(A{a.., b...});
    let d = a.a; // ok; a partial moved
    let e = a.b; // used moved value a;
}
```

3. mem::replace可以用新的指替换旧值，但没有发生任何借用或者move(用一个新值替换旧值，并且返回旧值)
这意味着我们若是想使用某个借用的内容值可以使用mem::replace
```
struct A {
    a: Box<i32>,
}

fn (c: &mut A) {
    let e = mem::replace(&mut c.a, Box::new(0)) // error, 不允许move 借用的content
}
```
4. 借用也是不允许move其中的字段的
```
struct A {
    a: Box<i32>,
}

fn (c: &A) {
    let e = c.a // error, 不允许move 借用的content
}
```

# *** 强调
1. 借用不允许move字段，可以使用replace::mem来进行或者clone
2. Box只允许move一个字段，且原始值不可以在使用
3. Struct 可以move多个字段，但原始值不可再使用


### 在rust中想要保持多个& mut 即多个可变引用就得是用* mut unsafe原始指针了，或者RefCell<> 


### 内部可变性的类型有Mutex(这个的本质是所有权都归了他管他自然可以返回), Cell, RefCell
1. 单线程内可以使用cell和RefCell
2. 多线程就得使用Mutex

### rust 字符串拼接为什么需要借用
https://zhuanlan.zhihu.com/p/24486743

### 如何获取struct中的多字段 &
不写这些getter，直接访问field。

这种办法是可以通过的。原因在于：Rust对于内部不同的访问路径是会分开记录borrow的，所以不会有任何问题。


### move also a byte-copy  but it is a semantic copy
证明move是copy的代码
https://play.rust-lang.org/?gist=2aafa34d69af1bdb04b0299d2dfb4f87&version=stable&mode=debug&edition=2015

```
#[derive(Debug)]
struct A<'a> {
    array: [i32; 4],
    a: &'a i32,
}

fn main() {

    let c = [2i32, 3,4,5];
    let mut a = A{array: c, a: &c[0]};
    let mut d = a;
    d.array[0] = 1234;
    // *(d.a) = 12455;
    println!("{:?}, {:?}", d.a, d);
}
// 结果
// 2, A { array: [1234, 3, 4, 5], a: 2 }
```

### [trait objects lifetime](https://doc.rust-lang.org/book/second-edition/ch19-02-advanced-lifetimes.html?highlight=traits,object#inference-of-trait-object-lifetimes)
1. The default lifetime of a trait object is 'static
2. With &'a Trait or &'a mut Trait, the default lifetime of the trait object is 'a.
3. With a single T: 'a clause, the default lifetime of the trait object is 'a.
4. With multiple clauses like T: 'a, there is no default lifetime; we must be explicit.

* trait objects are "&Foo or Box<Foo>"
cause trait objects的default lifetime is 'static

### object safe
recap 
Whew! As we can see, almost all of these rules talk about Self. A good intuition is “except in special circumstances, if your trait’s method uses Self, it is not object-safe.”


### [why Box<Trait> need lifetime](https://morestina.net/blog/793/closure-lifetimes-in-rust)

trait object's default lifetime is 'static, the captures must be 'static, otherwise there will be invalidate reference.

```
fn crash_rust() {
    let mut p = Processor::new();
    {
        let s = "hi".to_string();
        p.set_callback(|| println!("{}", s.len()));
    }
    // access to destroyed "s"!
    p.invoke();
}
```
when we call invoke, the s has alreay been release.

### 不带分号的句子只是返回到上一层并不是函数调用返回
```
match a {
  1 => { 32; }
  _ => { false }
  // _ => { return flase; }
}
```
此时就会报错因为1对应的分支match返回 （），而_对应的分支返回false; 是编译不过的。


### rust macro 输入的是token串， 输出的是AST, https://danielkeep.github.io/tlborm/book

As previously mentioned, macro processing in Rust happens after the construction of the AST. As such, the syntax used to invoke a macro must be a proper part of the language's syntax. 

1. ident: 标识符，用来表示函数或变量名 如 a, b, foo等等 'an identifier. Examples: x; foo.'
2. expr: 表达式 a + b, a * b, foo(23)等, '2 + 2; if true then { 1 } else { 2 }; f(42)' 'an expression. Examples: 2 + 2; if true then { 1 } else { 2 }; f(42).'

3. block: 代码块，用花括号包起来的多个语句 {}, () 'a brace-delimited sequence of statements. Example: { log(error, "hi"); return 12; }.'
4. pat: 模式，普通模式匹配（非宏本身的模式）中的模式，例如 Some(t), (3, 'a', _), let, match, 'Some(t); (17, 'a'); _.' 'a pattern. Examples: Some(t); (17, 'a'); _.'
5. path: 路径，注意这里不是操作系统中的文件路径，而是用双冒号分隔的限定名(qualified name)，如 std::cmp::PartialOrd 'a qualified name. Example: T::SpecialA.'
6. tt(token tree): 单个语法树 （可以是任意的rust语法）'a single token tree.'
7. ty(type): 类型，语义层面的类型，如 i32, char 'a type. Examples: i32; Vec<(char, String)>; &T.'
9. item: 条目，function, struct, module,  'fn foo() { }; struct Bar;' 'an item. Examples: fn foo() { }; struct Bar;.'
10. meta: 元条目 如#[...] and #![...], 'cfg(target_os = "windows")' 'a "meta item", as found in attributes. Example: cfg(target_os = "windows").'
11. stmt: 单条语句，如 let a = 42; 'a single statement. Example: let x = 3.'

> expr: variables must be followed by one of: => , ;
> ty and path variables must be followed by one of: => , : = > as
> pat variables must be followed by one of: => , =
> Other variables may be followed by any token.

> item: anything.
block: anything.
stmt: => , ;
pat: => , = if in
expr: => , ;
ty: , => : = > ; as
ident: anything.
path: , => : = > ; as
meta: anything.
tt: anything.

### rust的宏匹配以后再有其他宏进行匹配时传入的是AST node无法再进行细致的token匹配，因此在模块化的宏是困难的。
```
 recognise_tree!(expand_to_larch!());
```
这里expand_to_larch也是个宏 可是对于宏来书输入就是token tree， 因此是不会展开expand_to_larch的;

https://danielkeep.github.io/tlborm/book/mbe-min-captures-and-expansion-redux.html
By parsing the input into an AST node, the substituted result becomes un-destructible
当我们把token tree 解析成AST node以后就无法再结构进行匹配了，因为AST node是个完整的语法，破坏了就没有语义了， 而token 还没有进行语义分析呢

词法解析器 -> token tree
语法解析器 -> AST node

```
macro_rules! capture_then_what_is {
    (#[$m:meta]) => {what_is!(#[$m])};
}

macro_rules! what_is {
    (#[no_mangle]) => {"no_mangle attribute"};
    (#[inline]) => {"inline attribute"};
    ($($tts:tt)*) => {concat!("something else (", stringify!($($tts)*), ")")};
}

fn main() {
    println!(
        "{}\n{}\n{}\n{}",
        what_is!(#[no_mangle]),
        what_is!(#[inline]),
        capture_then_what_is!(#[no_mangle]),
        capture_then_what_is!(#[inline]),
    );
}
```

### macro 匹配时是token tree匹配的因此无论写什么都可以因为他只一个token stream，还没有解析，但进入到macro扩展的阶段就是AST node了，这时匹配到的肯定都是合法的rust syntax了。
1. The input to every macro is a single non-leaf token tree.
2. Macros (really, syntax extensions in general) are parsed as part of the abstract syntax tree.
3. There are multiple kinds of syntax extension in Rust. We will only be talking about macros defined by the macro_rules! construct.
4. Just because you see something of the form $name! $arg, doesn't mean it's actually a macro; it might be another kind of syntax extension.

### 被宏匹配过的token tree中的匹配项就是一个整体(匹配中的匹配项)，不能再拆卡匹配了，也就是说嵌套宏是无法进行再次拆开匹配的功能，只有token tree 才能拆开匹配
就好比 下面的 '3 + 5', 再直接用match_tokens 匹配，那还能拆开为a + b, 但经过 capture_then_match_tokens 匹配后就是个整体的expr，再去用match_tokens进行匹配也是不行的。

```
macro_rules! capture_then_match_tokens {
    ($e:expr) => {match_tokens!($e)};
}

macro_rules! match_tokens {
    ($a:tt + $b:tt) =>  { {println!("{:}, {:}", stringify!($a), stringify!($b)); "got an addition"} };
    (($i:ident)) => { { println!("{:}", stringify!($i)); "got an identifier" }};
    ($($other:tt)*) => { { println!("{:}", stringify!($($other)*)); "got something else"} };
}

fn main() {
    println!("{}\n{}\n{}\n",
        match_tokens!((caravan)),
        match_tokens!(3 + 6),
        match_tokens!(5));
    println!("{}\n{}\n{}",
        capture_then_match_tokens!((caravan)),
        capture_then_match_tokens!(3 + 6),
        capture_then_match_tokens!(5));
}
```

### rust match 若只是匹配没有结构行为则不会move或者引用
```
#[derive(Clone)]
pub enum Entry<K,V> {
    Empty,
    Full(K, V, u64),
    Ghost(K, u64),
}

impl<K,V> Entry<K,V> {
    #[inline]
    #[allow(dead_code)]
    pub fn is_empty(&self) -> bool {
        match *self {
            Entry::Empty => true,
            _ => false
        }
    }
    pub fn is_full(&self) -> bool {
        match *self {
            Entry::Full(..) => true,
            _ => false
        }
    }
    pub fn is_ghost(&self) -> bool {
        match *self {
            Entry::Ghost(..) => true,
            _ => false
        }
    }
    pub fn matches<Q: PartialEq<K>>(&self, key: &Q, hash: u64) -> bool {
        match *self {
            Entry::Empty => true,
            Entry::Full(ref k, _, h) | Entry::Ghost(ref k, h) => hash == h && key == k,
        }
    }
    pub fn key(&self) -> Option<&K> {
        match *self {
            Entry::Full(ref k, _, _) => Some(k),
            _ => None,
        }
    }
    pub fn value(&self) -> Option<&V> {
        match *self {
            Entry::Full(_, ref v, _) => Some(v),
            _ => None,
        }
    }
}
```
当我们调用is_empty, self并不会move因为他只是匹配看看self是不是Entry::Empty类型，is_full也是，他是匹配的Entry::Full(..)类型，并没有move，等, Entry::Full(..) === Entry::Full(_,_,_),一个都不move或者引用，只有使用了self里的字段才会move或者引用


### [rust macro](http://blog.luxko.site/tlborm-chinese/book/pat-incremental-tt-munchers.html)
1. 注意关键字，self即使关键字又可以是ident
2. 宏一旦考试匹配就无法[停止或者回溯](http://blog.luxko.site/tlborm-chinese/book/mbe-min-captures-and-expansion-redux.html), 因此在写匹配时应当把最具体的写到最前面，就好比java异常捕获似的
```
macro_rules! dead_rule {
    ($e:expr) => { ... };
    ($i:ident +) => { ... };
}
```
> 考虑当以dead_rule!(x+)形式调用此宏时，将会发生什么。解析器将从第一条规则开始试图进行匹配：它试图将输入解析为一个表达式；第一个标记(x)作为表达式是有效的，第二个标记——作为二元加的节点——在表达式中也是有效的。

至此，由于输入中并不包含二元加的右手侧元素，你可能会以为，分析器将会放弃尝试这一规则，转而尝试下一条规则。实则不然：分析器将会panic并终止整个编译过程，返回一个语法错误。

由于分析器的这一特点，下面这点尤为重要：一般而言，在书写宏规则时，应从最具体的开始写起，依次写至最不具体的。

2. rust的宏的替换不是基于token的
```
macro_rules! capture_expr_then_stringify {
    ($e:expr) => {
        stringify!($e)
    };
}

fn main() {
    println!("{:?}", stringify!(dummy(2 * (1 + (3)))));
    println!("{:?}", capture_expr_then_stringify!(dummy(2 * (1 + (3)))));
}

> "dummy ( 2 * ( 1 + ( 3 ) ) )"
> "dummy(2 * (1 + (3)))"
```
3. token的解析和AST的解析
> 个人理解token tree的解析是中序 而 AST的解析是先序
```
«a» «+» «b» «+» «(   )» «+» «e»
          ╭────────┴──────────╮
           «c» «+» «d» «[   ]»
                        ╭─┴─╮
                         «0»


              ┌─────────┐
              │ BinOp   │
              │ op: Add │
            ┌╴│ lhs: ◌  │
┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
│ Var     │╶┘ └─────────┘ └╴│ BinOp   │
│ name: a │                 │ op: Add │
└─────────┘               ┌╴│ lhs: ◌  │
              ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
              │ Var     │╶┘ └─────────┘ └╴│ BinOp   │
              │ name: b │                 │ op: Add │
              └─────────┘               ┌╴│ lhs: ◌  │
                            ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
                            │ BinOp   │╶┘ └─────────┘ └╴│ Var     │
                            │ op: Add │                 │ name: e │
                          ┌╴│ lhs: ◌  │                 └─────────┘
              ┌─────────┐ │ │ rhs: ◌  │╶┐ ┌─────────┐
              │ Var     │╶┘ └─────────┘ └╴│ Index   │
              │ name: c │               ┌╴│ arr: ◌  │
              └─────────┘   ┌─────────┐ │ │ ind: ◌  │╶┐ ┌─────────┐
                            │ Var     │╶┘ └─────────┘ └╴│ LitInt  │
                            │ name: d │                 │ val: 0  │
                            └─────────┘                 └─────────┘
```
从上面可以看出token自左向右中序，AST先根 先序

4. 宏可以出现的位置
    1. 宏能在如下位置出现：
        1. 模式(pattern)中
        2. 语句(statement)中
        3. 表达式(expression)中
        4. 条目(item)中
        5. impl 块中
    2. 一些并不支持的位置包括：
        1. 标识符(identifier)中
        2. match臂中
        3. 结构体的字段中
        4. 类型中
绝对没有任何在上述位置以外的地方使用宏的可能。
> 当出现在表达式中时，macro展开也必须是表达式，若是单个表达式则一层{} or [] 即可，若是多个则必须再嵌套一层{} 来表明是表达式
```
macro_rules! foo {
    () => {{
        ...
    }}
}
```

5. macro的输入是single token tree即匹配的也是single token tree， 输出是AST, 也就是说不可以把一个一个宏的展开结果输入给另一个宏

6. rust中所有的宏的最终展开必须是一个完整的，有效的语法元素（比如表达式，条目等等）[](http://blog.luxko.site/tlborm-chinese/book/pat-push-down-accumulation.html)

如书中的例子
```
e, init_array!(@accum 2, e) // 这就是个残缺的 因此不合法, e, 加个啥？
```
7. Token trees are somewhere between tokens and the AST. Firstly, almost all tokens are also token trees; more specifically, they are leaves. There is one other kind of thing that can be a token tree leaf, but we will come back to that later.

The only basic tokens that are not leaves are the "grouping" tokens: (...), [...], and {...}. These three are the interior nodes of token trees, and what give them their structure. To give a concrete example, this sequence of tokens:

```
a + b + (c + d[0]) + e
```
按照上面所说的 所有的tokens都是token trees 大部分也都是叶子，只有 (). {}. []不是，因此 上面的那个表达式一共有 7 个 single token tree;
a, +, b, +, (c + d[0]), +, e
> 怎么推断的呢？ $(($t:tt)*) 有多少个$t就有多少个 single token tree

### [rust 中的vector之一种值] // 待理解