### crystal
1. is_a? 是用来判断类型的
```
if c.is_a?(String)
```

2. Tuple, Hash 都是使用['ele']来访问的，没有'.'的访问方式
```
hash = {'c' => 3, 'd': 4}
hash['c']
```

3. responds_to?(method_name(tuple))是用来判断某个变量是否拥有某一方法，类实例属性不可以直接调用得先赋值一个变量
```
if (a = @a).responds_to?(:abs)
```

4. a.nil? 判断a是否为nil

5. case语句可以用来比值，也可以用来判断类型
```
a = 3
case a
when 3, 5
# doSomething
when 6
# doing
else: 
end

case a
when String
# do puts
when Int32
# do puts
```

6. block 和 proc
  ### block
  1. method() do ... end, 这个最左匹配
    ```
    foo bar do
      something
    end

    # The above is the same as
    foo(bar) do
      something
    end
    ```
  2. {...}， 这个是最右匹配
  ```
  foo bar { something }

  # The above is the same as

  foo(bar { something })
  ```

  3. method(..., &block) ,可以前面传参数最后一个传block, 这是为了更明显，其实也可以不写
  ```
  def tw(a : Int32, &bl)
    puts a
    yield
  end
  ```
  4. block也可以传递参数
  ```
  tw 2 do |arg| 
    arg + 3
  end

  def tw(a : Int32, &bl)
    puts a
    yield 23
  end 
  ```
  5. 但我们的blocK 只有一个参数时可以如下
  ```
  puts(call &.+(3))

  def call(&bl) 
    yield 4
  end
  ```
  > 每个函数只要写了yield那就是可以在调用的时候加上 block
  6. A block can be captured and turned into a Proc, 但必须写明参数以及返回值，当然既没有参数也没有返回值得block就不用写了
  ```
  def int_to_int(&block : Int32 -> Int32)
    block
  end

  proc = int_to_int { |x| x + 1 }
  proc.call(1) #=> 2
  ```
  ### proc 可以捕获外部变量
  ```
  -> (arg) {}
  ```
  1. proc是不能传给block的, 必须对proc 使用&
  ```
  def capture(&block)
    block
  end

  def twice
    yield
    yield
  end

  proc = capture { puts "Hello" }
  twice &proc

  twice &->{ puts "Hello" }

  def say_hello
    puts "Hello"
  end

  twice &->say_hello

  # twice proc error
  ```

  ### captured block（&block这中形式拿到的block） 以及 proc都可以捕获外部变量


  ### 关于实例变量的获取与赋值
  1. 赋值时一定要带前缀 @
  2. 获取时只要不歧义，就可以不带@前缀
  ```
  class Person
    getter age
    def initialize(@name : String)
      @age = 0
    end

    def age
      puts "call me"
      @age  ## 不带 @ 就会报错
    end

    #def age
    # age   ## 此处歧义到底要调谁啊？
    #end
    
    def become_older
      age  ## 此处的age 调用的就是age method 当注释掉age method后调用的就是age属性了
    end
  end

  puts Person.new("dengjie").age, Person.new("deji").become_older
  ```
  > 对于crystal 的class或者struct本省就不可以直接访问属性，只有设置getter和setter才可以，不像java，只要是public的属性就可以访问
7. crystal 的class， struct, module都可以扩充定义
```
module A
	def self.c
		23
	end
end

module A
	def self.d
		234
	end
end

# A 就有了c和d两个方法
```

8. crystal 中的所有的都是对象我也可以直接在A(上面的module)上直接定义方法
```
def A.tell
	23
end
```

8. If you want a method to only accept the type Int32 (not instances of it), you use .class
```
def foo(x : Int32.class)
end

foo Int32  # OK
foo String # Error
foo 13 # Error
```
9. @name 是类实例变量， @@是类变量， struct和class的区别是 struct栈分配，class是堆分配, class可以 '<' 就是include 实例方法, 而struct则只有abstract struct 才可以 继承

10. tuple 必须初始化，且不可修改

11. 正常函数式没有全局变量，或者说函数不能捕获外部变量，proc可以，但T.class还不能作为proc的参数类型

12. module里的方法是不可以调用的，除非是定义为self.method,或者extend self
```
  Modules serve two purposes:

  as namespaces for defining other types, methods and constants
  as partial types that can be mixed in other types
```

13. crystal 的module 和class是有路径的如下
```
module A::B
  VERSION = "123"
  def print
    puts "print"
  end
end


class A::C
  include B
  def tell
    puts VERSION
  end
end

d = A::C.new
d.tell
```

> A包下的一级路径访问时不需要再用前缀了, 比如 include B 就不需要 include A::B

14. crystal 也和go一样也有select, channel
```
timeout_channel = Channel(Nil).new
timeout_channel2 = Channel(Nil).new
timeout = 6;
timeout2 = 3;

spawn do
  sleep timeout.not_nil!
  timeout_channel.send nil
end

spawn do
  sleep timeout2.not_nil!
  timeout_channel2.send nil
end

select
when timeout_channel.receive
  raise Exception.new "DENGJIE"
when timeout_channel2.receive
  puts "dengjie"
end
```

15. crystal 里面只有module才可以被include或者是extend, include是实例扩展，extend是类扩展, 但扩展后的类型依然可以是用 is_a? 来判断类型

16. use inherit will get Class method and Instance method but include can only get Instance method and extend will get Class method [](https://stackoverflow.com/questions/1282864/ruby-inheritance-vs-mixins) this can also adapt crystal

17. module 前缀声明class以及 constant 在整个module内可见
```
module A::E
end

module A
  class B < E
  end
end
// compile ok
```
声明必须前置否则报错

###  如下可以 给函数的参数命名 'dj adsf', dj 是对外的名字， adsf 是对内的名字
```
    def query_one?(dj adsf : Class)
      p adsf
    end

    query_one? dj String
```

###  how to check if a block supplied https://stackoverflow.com/questions/39190854/crystal-how-to-check-if-the-block-argument-is-given-inside-the-function


### 不允许使用系统定义的抽象类(Object, Value, Struct, Reference, Class...)作为类属性类型以及变量类型，但可以说作为参数类型, 以及作为泛型参数也不可以
```
class A
  @a : Object
  def initialize(@a) # wrong
  end
end

a : Object # wrong

def say(a : Object) # yes
end
```

### 定义的方法可以转换成proc
```
def say(s: Int32)
s
end

c = ->say(Int32)  # 类推 ->say(String, Int32)
c.call(23)  
```

### block的写法
```
[2,3,4].map { |a| 
  a.as(Int16)
  a
}
## 等于下面的写法
[2,3,4].map do |a|
  a.as(Int16)
end
```


### method_missing 宏相当于proxy

### ensure 和 return  || 类似于golang的return和defer 都是return 现将返回值写上，然后ensure清理
```
cc = ->() {
  mu.lock
  data = Raft::ABytes.new
  begin
    return data
  ensure
    data = Raft::ABytes.new 3, 3
    pp "dd"
    mu.unlock
  end
}
```


### 抽象类的抽象方法必须实现, 抽象方法只能在抽象类中实现，且抽象方法不能实例化
### module中抽象方法可以不实现，但调用会报错