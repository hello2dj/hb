---
title: css 变量(慎重，有大图)
date: 2018-04-06 10:05:27
tags:
    - css
---

[原文地址](https://medium.freecodecamp.org/everything-you-need-to-know-about-css-variables-c74d922ea855)

![](https://cdn-images-1.medium.com/max/2000/1*Im5WsB6Y7CubjWRx9hH7Gg.png)

> 本文是 CSS Variables 的第一章([作者的新书](https://gumroad.com/l/lwaUh))

大多数的语言都有变量，可是 css 里面是没有的（话说 css 是编程语言么？）

但是像一些 css 预处理器如 sass 等都会提供对变量的支持

值得高兴的是 css 现在也开始支持变量了([can i use](https://caniuse.com/#search=css%20variables))。

这篇文章里我会介绍一下 css variables 的使用，让我们 css 更利于维护吧！

### 我们会学到什么？

学习基础知识是非常好玩的，尤其是当我们用这些知识构造一个真实的 app。

#### Project 1: 使用 css Variables 创建一个可变的组件

![](https://cdn-images-1.medium.com/max/1600/1*qElS3I43_SdpdRA8-m2iew.gif)

[Codepen](https://codepen.io/ohansemmanuel/full/PQYzvv/)

#### Project2: 使用 CSS Variables 改变主题

![](https://cdn-images-1.medium.com/max/1600/1*r2TrlsC-gWRD5Hu6Tp2gjQ.gif)

[Codepen](https://codepen.io/ohansemmanuel/full/xYKgwE/)

#### Project3: 创建 CSS Variable booth

![](https://cdn-images-1.medium.com/max/1600/1*E6H-wT6a0BDR9OJK7Z0dTA.gif)

[Codepen](https://codepen.io/ohansemmanuel/full/EoBLgd/)

### 为什么变量这么重要呢？

这里有几个原因大家可以一起看看

1.  代码可阅读性增强，这个很明显，不会看到的都是字面量比如 #0f0f0f 等等，而可以看到的是 bg-color

2.  修改也变得容易了，好比我们的盒子高度都是 40px，我想都修改为 42px，我就不需要一个一个去改了。。。

3.  快速定位错误，你写一堆的 42px, 手一抖写了个 41px 咋办? 还有其他的由于不小心敲错的错误等等。。。

我们可以看到 css variables 的 可读性，可维护性都是很好的。

### 定义 css variables

在 css 中 css variables 是以--开头的‘property’, 如下

```
/* can you spot the variable here ? */
.block {
    color: #8cacea;
    --color: blue;
}
```

![CSS Variables are also called “Custom Properties”](https://cdn-images-1.medium.com/max/1600/0*2Pl5qBF8DCTGL_np.png)

### css variables 的作用域

我们都知道在 js 里面，变量都是有作用域的。在 css 里也是一样的。

```
:root {
    --main-color: red;
}
```

:root 选择器允许我们指定 DOM 中最顶级的元素或者是 document tree

因此我们在这个范围指定的 css variables 就是在全局范围内都可以使用的了。
![Local and Globally scoped variables](https://cdn-images-1.medium.com/max/1600/0*GLjARI5CCGA3xJAx.png)

### 举个 🌰1：

假设你想创建一个 css variable 来存储主题的主色

1.  先创建:root 选择器来指定‘全局’变量

```
:root {

}
```

2.  定义变量

```
:root {
    --primary-color: red
}
```

一定要记住 css variable 必须以‘--’开头 eg: --color

### 使用 css variables

一旦一个变量被定义了且初始化了，你就可以使用了。

这里有一些小陷阱。

如果你用过 css 预处理器，那么你就知道他们的变量的使用方式

```
$font-size: 20px;

.test {
    font-size: $font-size
}
```

但是 css variables 的使用时不同的， 你需要是用 var()来使用变量。如下：

```
:root {
    --font-size: 20px
}

.test {
    font-size: var(--font-size, 30px)
}
```

**注意其中的第二个参数，是的 var 允许设置默认值，若是--font-size 未定义他就会使用默认值**

![Remember to use the var function](https://cdn-images-1.medium.com/max/1600/0*Gv8Nci9VTrJBxpBe.png)

另一个需要注意的地方是，css variables 不想预处理器里的变量那样，可以用在很多的地方以及进行数学运算，css variables 值允许作为属性值来使用。

```
/*this is wrong*/
.margin {
    --side: margin-top;
    var(--side): 20px;
}
```

![The declaration is thrown away as a syntax error for having an invalid property name](https://cdn-images-1.medium.com/max/1600/0*vtIhP9EGm_vTxeio.png)

你也不可以进行数学运算，除非使用 calc()

```
/*this is wrong*/
.margin {
    --space: 20px * 2;
    font-size: var(--space); // not 40px
}
```

而应该使用下面这种方式

```
.margin {
    --space: calc(20px * 2);
    font-size: var(--space); /*equals 40px*/
}
```

### 值得一提的地方

这里还有一些其他需要值得注意的地方

1.  #### 自定义属性（css variables）就是普通的属性，因此你可以在任何元素中声明他们可以在 p, section, aside, root 设置伪元素中使用。

![They behave like normal properties](https://cdn-images-1.medium.com/max/1600/0*plpQVof3v3JrzC1P.png)

2.  #### css variables 也拥有正常继承和层叠规则就像其他属性似的（例如 color 属性可以被继承似的）

```
div {
    --color: red;
}

div.test {
    color: var(--color);
}
div.ew {
    color: var(--color);
}
```

--color 会被 divs 所继承。
![](https://cdn-images-1.medium.com/max/1600/0*GNSU5IDdk7dx3B8t.png)

3.  #### css variables 也可以使用在媒体查询里面或者其他条件规则
    例如

```
:root {
    --gutter: 10px
}
@media screen and (min-width: 768px) {
    --gutter: 30px;
}
```

![Useful bit for responsive design](https://cdn-images-1.medium.com/max/1600/0*qmsVGjnWjLCKfyvt.png)

4.  #### css variables 还可以在 html style 属性里面使用

```
<!--HTML-->
<html style="--color: red">

<!--CSS-->
body {
  color: var(--color)
}
```

![Set variables inline](https://cdn-images-1.medium.com/max/1600/0*EQiFgdDyNBQ1AfDk.png)
还有就是 css variables 是大小写敏感的

```
/*these are two different variables*/
:root {
 --color: blue;
--COLOR: red;
}
```

5.  #### 多次声明

多次声明同一个 variable

如下

```
/*define the variables*/
:root { --color: blue; }
div { --color: green; }
#alert { --color: red; }

/*use the variable */
* { color: var(--color); }
```

然后看看下面这写元素的颜色吧

```
<p>What's my color?</p>
<div>and me?</div>
<div id='alert'>
  What's my color too?
  <p>color?</p>
</div>
```

第一个 p 的颜色是 blue, 因为 p 没有直接的--color 定义，因此继承了:root 内的--color 定义。

第一个 div 是绿色的 因为 div 内有直接的--color 定义

id 是 alert 的 div 是 red，因为#alert 也有直接的--color 定义，按照 css 选择器的权重来看也是 red，因为 id 选择器是仅次于内联 和 important 的, so...

最后在#alert 里的 p 也是红色的，因为 p 继承了#alert 的--color 属性

![The solution to the Quiz](https://cdn-images-1.medium.com/max/1600/1*lGioVJqkKo0N91R9eMvywQ.png)

6.  #### 循环依赖的解析

看下面

1.  当一个变量依赖他自己时使用 var()来引用

```
:root {
    --m: var(--m)
}

body {
    margin: var(--m)
}
```

2.  当多个变量相互依赖

```
:root {
  --one: calc(var(--two) + 10px);
  --two: calc(var(--one) - 10px);
}
```

注意千万不要创建循环依赖发，循环依赖是不 ok 的。

7.  #### 使用无效的变量会发什么什么？

语法错误会被丢弃，那么无效的 var()会发生什么？

看下面：

```
:root {
    --color: 20px;
}
p { background-color: red; }
p { background-color: var(--color); }
```

![](https://cdn-images-1.medium.com/max/1600/0*fa59XRLGKo5Rsqm4.png)

--color 会被替换为 20px, 然而 20px 这个值对于 background-color 是个无效的值，并且他是非继承属性，因此它的值会被设置为 initial: transparent。尴尬了吧。

![](https://cdn-images-1.medium.com/max/1600/0*uVic7R1o96n-T1l5.png)

接下来高能了，上面那个呢是给设置成了 initial 值，那么要是不用 css 变量直接写个 20px 呢，这个属性会被丢弃的，啥都木有
![](https://cdn-images-1.medium.com/max/1600/0*9HzCVQdyvqeo5dZq.png)

可以再 dev-tools 里面验证一下就得到了，直接写错是无效的。

> 在 chrome65 下验证过了，确实是的

8.  #### 设置数值请注意--单一实体

见下：

```
font-size: 20px;
```

20px 就是一个单一实体，是个整体

当使用 css variables 时一定要注意，如下

```
:root {
    --size: 20
}
div {
    font-size: var(--size)px /*wrong*/
}
```

如上的代码你可能想要的结果是 20px,可惜你错了， 浏览器会把他解析为 20 px;

请注意 20 和 px 中间有空格哦！说白了就无法生效

如果一定要这么用的话就得这么用 calc(var(--size) \* 1px), ok!

### 下面来一点儿实操

创建颜色不同的 button

![](https://cdn-images-1.medium.com/max/1600/1*qElS3I43_SdpdRA8-m2iew.gif)

可以看到他们的不同是 border-color 和 background-color，那么我们会怎么做呢？

如下

```
<button class="btn">Hello</button>
<button class="btn red">Hello</button>
```

.btn 会包含基本的 style,如下

```
.btn {
    padding: 2rem 4rem;
    border: 2px solid black;
    background: transparent;
    font-size: 0.6em;
    border-radius: 2px;
}

/*on hover */
.btn:hover {
  cursor: pointer;
  background: black;
  color: white;
}
```

那么那些可变的因素在哪呢？

```
.btn.red {
    border-color: red
}
.btn.red:hover {
    background: red
}
```

接下来我们用 css 变量来改写一下

```
.btn {
   padding: 2rem 4rem;
   border: 2px solid var(--color, black);
   background: transparent;
   font-size: 0.6em;
   border-radius: 2px;
 }
 /*on hover*/
 .btn:hover {
  cursor: pointer;
   background: var(--color, black);
   color: white;
 }
```

var 第二个参数在上面我们说过了是 default value

接下来看

```
.btn.red {
    --color: red
}
```

此时再来看看比较
![Without CSS Variables VS with CSS Variables](https://cdn-images-1.medium.com/max/1600/1*bdT9ITBx1wpXjLOYoWBI7w.png)

如果你有更多的颜色的 button,如下

![See the difference??](https://cdn-images-1.medium.com/max/1600/1*erZb3Z5FtTIR8EV9fl0QOA.png)

### 总结

其实变量这个东西是个语言都支持了，为啥 css 没有呢，在我看来因为是一开始就没把 css 当做编程语言来用。

其实 web 也是 GUI 编程，我们纵观一下 GUI 编程，其实一开始都是直接语言去控制的，比如 QT, 所有的图形都是 C++去操作的，无论是按钮啊，tabel 啊都是的。可是人们就发现 UI 是可以和逻辑分开的。于是就出了可视化的 GUI 编程，妥妥拽拽啥的，最后是用 XML 来表达的，android,ios 都是差不多的。

那 web 呢，我在我的一片文章里说过了，一开始 web 就不是打算来进行 UI 的，他只是像做一些文档展示的工作，可是社会是前进的，web 越来越火，对页面样式的控制也是极度强烈，但一开始都是一些简单的控制，一点一点才复杂化的，从简单开始 css 就出现了，在我看来 css 就好比是 GUI 中的 XML，但是用他们总是会有限制的。

因此 css 的出现就好比是 XML 我只需要配置就好了，那么复杂干什么！！！要啥变量啊！要啥变量！！

可随着 web 的功能性越来越强他需要 GUI 的能力，于是出现了 canvas, css variables, 硬件加速等等。

web 的发展已经完全超出了他最初的目的，只要性能问题解决了，往后哪有 GUI 编程啊，都是 web(虽然 web 也可以看做 GUI，但他的限制还是太多了)。

写到这其实我觉 GUI 编程或者说 application 的编程需要新的语言来解决一些问题了，无论是包袱太多还是什么其他问题，web 不应该再用 js 了(可惜不现实)，因为 js 的缺点确实多，历史包袱多，很难再有大的进化了。GUI 也不应该再用 C#, Java, C++了，他们都带繁重了。我看到了 Dart, 虽然出了很久了，可惜不火，dart 是新时代设计的语言，经过深思熟虑，不想 js 10 天就出来。

新的时代就应该用新的语言。当然这很难...
