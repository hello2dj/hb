---
title: js中的委托vs继承
date: 2018-04-05
tag:
- js
---

曾经 James Gosling（java 之父） 回答过一个问题，那就是如果让他从头重写一遍 java，那会有什么不同的地方？他回答说那就是不要继承只有委托。

<!-- more -->

使用继承来重用代码就好比是你想要一个塑料碗于是你定了一个外卖似的。事实上当你按照教课书似的来使用继承，你回发现你的整个继承架构是那么的脆弱和随意。子类会一直拥有它根本不需要的方法或者属性，这就违背了最小知识原则。

而委托则是通过运行时在具体的实例上下文中进行函数调用来实现代码重用的。如下：

```
Object.prototype.toString.call({});
```

在 js 中同 call 和 apply 我们可以很轻松的实现委托。

### 自定义函数委托

假设我们的有一款绘画 app, 我们需要一个矩形对象。我们先用如下的方法实现：

```
const Rectangle = function(left, top, length, width, options) {
    this.left = left;
    this.top = top;
    this.length = length;
    this.width = width;
    if (options) {
        this.color = options.color;
        this.border = options.border;
        this.opacity = options.opacity;
        //... etc.
    }
}

const myRectangle = new Rectangle(10, 10, 30, 20, {color:'#FAFAFA', opacity:0.7});
```

我们也需要知道两个矩形是否重叠，因此有如下的方法：

```
Rectangle.prototype.overlaps = function(another) {
    const r1x1 = this.left,
        r1x2 = this.left + this.width,
        r1y1 = this.top,
        r1y2 = this.top + this.height,
        r2x1 = another.left,
        r2x2 = another.left + another.width,
        r2y1 = another.top,
        r2y2 = another.top + another.height;

    return (r1x2 >= r2x1) && (r1y2 >= r2y1) && (r1x1 <= r2x2) && (r1y1 <= r2y2);
}

myRectangle.overlaps(myOtherRectangle);
```

现在假设我们的 app 有一个用来渲染 dashlets 的 dashbord。我们也需要判断这些 dashlets 是否重叠。因此我们可以使用继承，然后让他们继承自 Rectangle。但其实我们也可以使用委托来实现

```
Rectangle.prototype.overlaps.call(dashlet1, dashlet2);
```

完整的如下：

```
const Rectangle = function(left, top, length, width, options) {
    //whatever...
}

Rectangle.prototype.overlaps = function(another) {
    const r1x1 = this.left,
        r1x2 = this.left + this.width,
        r1y1 = this.top,
        r1y2 = this.top + this.height,
        r2x1 = another.left,
        r2x2 = another.left + another.width,
        r2y1 = another.top,
        r2y2 = another.top + another.height;

    return (r1x2 >= r2x1) && (r1y2 >= r2y1) && (r1x1 <= r2x2) && (r1y1 <= r2y2));
}

Rectangle.prototype.overlaps.call(
    {left: 10, top: 10, width 12, height: 6},
    {left: 8, top: 15, width 9, height: 16});
//true  
Rectangle.prototype.overlaps.call(
    {left: 10, top: 10, width 12, height: 6},
    {left: 8, top: 25, width 9, height: 16});
//false;
```

### 泛型函数（generic）

那么内置函数是否都可以像上面那么使用么？不幸的是许多内置函数都不行，若是 this 不是指定的类型就会抛出 TypeError。

```
Date.prototype.getMilliseconds.apply({year:2010});
//TypeError: Date.prototype.getMilliseconds called on incompatible Object
```

幸运的是 es5 规范明确的描述了 generic functions 的概念。泛型函数是指那些允许 this 的类型是任意类型的函数。举个例子：

```
const hasNumbers = ''.search.call(['a', 'b', 'c'], /[0-9]/) > -1
```

### 委托，组合，继承

继承链太长，继承结构机会很脆弱，就会给子孙添加很多不必要的东西。能用委托和组合就不要使用继承。
