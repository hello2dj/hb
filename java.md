### java函数的异常一定要被处理或者声明然后继续抛出

### java 基本类型图int, float等有装箱和解箱的操作，基本类型装箱后和装箱前是相等的（==等号判断）
```
Integer a = 23;
int b = 23;

bool isEqual = a == b;
// true
```

### java equals 是比较引用地址是否是同一个的
```
Integer a = 23;
int b = 23;
a.equals(b);
```


### java 的class的加载时机是在 第一次使用时加载

### java innerclass 也可以是static的
### java 的innerclass 可以通过 OuterClass.this.XXX 来访问外部的属性，甚至若是补充突可以直接调用
```
package com.hello2dj;

interface Selector { boolean end(); Object current(); void next();
}
public class Sequence {
    private Object[] o; private int next = 0; public Sequence(int size) {
    o = new Object[size]; }
    public void add(Object x) { if(next < o.length) {
        o[next] = x;
        next++; }
    }
    private class SSelector implements Selector {
        int i = 0;
        public boolean end() {
            return i == o.length; }
        public Object current() { return o[i];
        }
        public void next() {
            System.out.println(Sequence.this.o[0]); // 访问外部
            if(i < o.length) i++; }
    }
    public Selector getSelector() {
        return new SSelector(); }
}
```

### java static 的innerClass 是只能访问static的


### 继承innerclass
```
//: InheritInner.java
// Inheriting an inner class
class WithInner { 
    class Inner {}
}
public class InheritInner extends WithInner.Inner {
    //! InheritInner() {} // Won't compile 
InheritInner(WithInner wi) {
    wi.super(); 
}
public static void main(String args[]) { 
    WithInner wi = new WithInner(); 
    InheritInner ii = new InheritInner(wi);
}
} ///:~
```
可以继承innerclass, 但是无法使用默认构造器，必须传入innerclass的outerclass并将其实例化，如上：


### innerclass也可以被overriding
```
//: BigEgg.java
// "Overriding" an inner class has no effect
class Egg { 
    class Yolk {
        Yolk() { 
            System.out.println("Egg.Yolk()");
        } 
    }
    Yolk y;
    Egg() {
        System.out.println("New Egg()"); 
        y = new Yolk();
    } 
}
public class BigEgg extends Egg { 
    class Yolk {
        Yolk() { System.out.println("BigEgg.Yolk()");
        } 
    }
    public static void main(String args[]) { 
        new BigEgg();
    }
} ///:~

// New Egg() 
// Egg.Yolk()

```

### java里面的constructor是父类的先执行，完后再回执行子类

### java 内部类
Nested classes are divided into two categories: static and non-static. Nested classes that are declared static are called static nested classes. Non-static nested classes are called inner classes.

从字面上看，一个被称为静态嵌套类，一个被称为内部类。从字面的角度解释是这样的：什么是嵌套？嵌套就是我跟你没关系，自己可以完全独立存在，但是我就想借你的壳用一下，来隐藏一下我自己（真TM猥琐）。什么是内部？内部就是我是你的一部分，我了解你，我知道你的全部，没有你就没有我。（所以内部类对象是以外部类对象存在为前提的）

静态内部类的作用：只是为了降低包的深度，方便类的使用，静态内部类适用于包含类当中，但又不依赖与外在的类，不用使用外在类的非静态属性和方法，只是为了方便管理类结构而定义。在创建静态内部类的时候，不需要外部类对象的引用。

-------------------------------------------------------------------------------------------------------------------------------------
非静态内部类有一个很大的优点：可以自由使用外部类的所有变量和方法


###  exception guidelines
Use exceptions to
1. Fix the problem and call the method (which caused the exception) again.
2. Patch things up and continue without retrying the method.
3. Calculate some alternative result instead of what the method was supposed to produce.
4. Do whatever you can in the current context and rethrow the same exception to a higher context.
5. Do whatever you can in the current context and throw a different exception to a higher context.
6. Terminate the program.
7. Simplify. If your exception scheme makes things more complicated, then it is painful and annoying to use.
8. Make your library and program safer. This is a short-term investment (for debugging) and a long-term investment (for application robustness)

### io 流
![](http://dl2.iteye.com/upload/attachment/0107/5080/673c01ca-bdb6-3ad3-9495-8eae4085abf2.png)

io流：
1. 字节流
    1. InputStream
        1. FileInputStream
        2. FilterInputStream
            1. BufferedInputStream
            2. DataInputStream
        3. ObjectInputStream
        4. ByteArrayInputStream
        5. PipedInputStream
    2. OutputStream
        1. FileOutputStream
        2. FilterOutputStream
            1. BufferedOutputStream
            2. DataOutputStream
            3. PrintStream
        3. ObjectOutputStream
        4. ByteArrayOutputStream
        5. PipedOutputStream 
2. 字符流
    1. Reader
        1. BufferedReader
        2. InputStreamReader
            1. FileReader
        3. StringReader
        4. CharArrayReader
    2. Writer
        1. BufferedWriter
        2. OutputStreamWriter
            3. FileWriter
        3. PrintWriter
        4. StringWriter

### java annotation只能是基本类型以及基本类型额数组

### 
1. == : 它的作用是判断两个对象的地址是不是相等。即，判断两个对象是不是同一个对象。(基本数据类型==比较的是值，引用数据类型==比较的是内存地址)

2. equals() : 它的作用也是判断两个对象是否相等。但它一般有两种使用情况：

    1. 情况1：类没有覆盖 equals() 方法。则通过 equals() 比较该类的两个对象时，等价于通过“==”比较这两个对象。
    2. 情况2：类覆盖了 equals() 方法。一般，我们都覆盖 equals() 方法来两个对象的内容相等；若它们的内容相等，则返回 true (即，认为这两个对象相等)。

### final关键字主要用在三个地方：变量、方法、类。

1. 对于一个final变量，如果是基本数据类型的变量，则其数值一旦在初始化之后便不能更改；如果是引用类型的变量，则在对其初始化之后便不能再让其指向另一个对象。
2. 当用final修饰一个类时，表明这个类不能被继承。final类中的所有成员方法都会被隐式地指定为final方法。
3. 使用final方法的原因有两个。第一个原因是把方法锁定，以防任何继承类修改它的含义；第二个原因是效率。在早期的Java实现版本中，会将final方法转为内嵌调用。但是如果方法过于庞大，可能看不到内嵌调用带来的任何性能提升（现在的Java版本已经不需要使用final方法进行这些优化了）。类中所有的private方法都隐式地指定为fianl

### java启动参数 http://hellojava.info/?p=323 && java 常见错误排查 https://www.slideshare.net/BlueDavy/java-33483237