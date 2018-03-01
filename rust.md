---
title: rust
date: 2018-01-03 20:14:31
tag:
- rust-lang
---

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

### 生命周期其实也是种泛型，所以当函数使用生命周期时一定得想使用泛型一样把生命周期写在方法名后面
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