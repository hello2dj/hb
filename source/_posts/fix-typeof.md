---
title: Object.prototype.toString是什么鬼？
date: 2018-04-05
tags:
    - js
---

### 发生了什么问题：js的类型判断

<!-- more -->

我想判断一个变量 a 是什么类型，那么我们会怎么做? typeof a? ok，我们来看看

```
let a = 2;
typeof a; // 'number'

let c = 'as';
typeof c; // 'string'
```

嗯 ：），看着不错哦！似乎我们是解决了，可惜啊，让我们来看一张表

| Type of val                          |       result       |
| ------------------------------------ | :----------------: |
| Undefined                            |    “undefined“     |
| Null                                 |      “object“      |
| Boolean                              |     “boolean“      |
| Number                               |      “number“      |
| NaN                                  |      “object“      |
| String                               |      “string“      |
| Object (native and not callable)     |      “object“      |
| Object (native or host and callable) |     “function“     |
| Object (host and not callable)       | 不同实现有不同表现 |

额 ：（ 看完是不是就有一丝丝不高兴了，因为他并不是很好，因为有些类型他也没办法区分比如 null, Array 他都识别为对象了，不高兴啊！总之在 js 里面进行类型判断是一件很不爽的事情，不像 java 和 golang 这种强类型的语言，他们总是有很明确的方法来判断自己是啥类型因为他们都有自己的反射机制。

### 有没有更好的办法呢：[[class]]

[[class]]是什么呢？ es5 规范里面规定的所有的 js 对象都有的一个内部属性（es 规范使用[[]]表示内部属性），用来表明对象分类的字符串属性，就是说他是用来表示对象类型。对于我们来说，这意味着，所有的内置对象类型都有一个唯一的不可修改的，规范强制规定的值，很明显要是咱们能拿到这个值，对象类型识别问题就解决了。。。

此时 Object.prototype.toString 就要登场了。让我们看看 [es5 规范](http://lzw.me/pages/ecmascript/#304)里对于他的描述

1.  如果 this 的值是 undefined, 返回 "[object Undefined]".
2.  如果 this 的值是 null, 返回 "[object Null]".
3.  令 O 为以 this 作为参数调用 ToObject 的结果 .
4.  令 class 为 O 的 [[Class]] 内部属性的值 .
5.  返回三个字符串 "[object ", class, and "]" 连起来的字符串 .

> ToObject 就是把 this 转换为对象，比如 4-> Number, '' -> String 等等

简单描述一下就是 toString 的调用会返回如下的格式

```
[object [[class]]]
```

看到没，通过他就可以拿到对象的内部[[class]]属性了。但这是时候有人就会说了，你瞅瞅下面的

```
[1,2,3].toString(); //"1, 2, 3"

(new Date).toString(); //"Sat Aug 06 2011 16:29:13 GMT-0700 (PDT)"

/a-z/.toString(); //"/a-z/"
```

你看，你看，还是不行啊！

上面现象的原因在于，打部分的内置对象都重写了 Object.prototype.toString 函数。比如[Number](http://lzw.me/pages/ecmascript/#426)。

显然直接调用时不行的，但**坏就坏在我们在用 js, 好也好在我们在用 js**, 估计大家都想到了，我们还有两个方法 call 和 apply， 如下

```
Object.prototype.toString.call([1,2,3]); //"[object Array]"

Object.prototype.toString.call(new Date); //"[object Date]"

Object.prototype.toString.call(/a-z/); //"[object RegExp]"
```

> js 这门语言本身就积累了太多的弊端了，好**好在用的人多，坏也坏在用的人多** 用的人多语言本身才能发展的好，用的人多反而也会让 js 本身的发展瞻前顾后，就好比 c++似的。就 cs 发展到现在，我觉得其实是需要新的 GUI 语言的出现或者说专注于 application 的语言，比如我最近看了 Dart 就不错，语法合适，没有很多奇怪的问题，可惜也没有太好的发展，这个可以再开一篇来讲了。（劣币驱逐良币么？）

### 更好的[[class]]获取封装

来，咱们封装一个 toType 吧！

```
const toType = function(obj) {
  return ({}).toString.call(obj).match(/\s([a-zA-Z]+)/)[1].toLowerCase()
}
```

> 为什么要用({}).toString.call,在我看了就是少写几个字符而已，但其实若是我调用 toType 的次数太多，({})这种方式我觉得也不行啊，因为每次调用都要再创建一个对象啊！

我们来试试这个函数吧

```
toType({a: 4}); //"object"
toType([1, 2, 3]); //"array"
(function() {console.log(toType(arguments))})(); //arguments
toType(new ReferenceError); //"error"
toType(new Date); //"date"
toType(/a-z/); //"regexp"
toType(Math); //"math"
toType(JSON); //"json"
toType(new Number(4)); //"number"
toType(new String("abc")); //"string"
toType(new Boolean(true)); //"boolean"
```

我们再来看看 typeof

```
typeof {a: 4}; //"object"
typeof [1, 2, 3]; //"object"
(function() {console.log(typeof arguments)})(); //object
typeof new ReferenceError; //"object"
typeof new Date; //"object"
typeof /a-z/; //"object"
typeof Math; //"object"
typeof JSON; //"object"
typeof new Number(4); //"object"
typeof new String("abc"); //"object"
typeof new Boolean(true); //"object"
```

> 其实 typeof 也可以用函数调用的方法来书写， typeof(a)，大家可以试试是 ok 的。

### 那么 instanceof 呢？

instanceof 又是什么呢？简单的说是用来测试一个对象是否是一个类的是实例（按照面向对象的说法）。他的实现方式是检测第二个输入参数的 prototype 是否在第一个输入参数的原型链上出现过(第二个参数必须是一个 constructor)，比如：

```
let a = [1,2,3];
a.__proto__; // []
Array.prototype; // []和上面的那个是同一个对象
a instanceof Array; // true

a.__proto__ = {}
a instanceof Array; // false
```

有几个关于 instanceof 的点需要我们注意

1.  有一些内置对象是没有相应的 constructor 的，比如 Math, JSON and arguments, 因此他们是没办法使用 instanceof 的
2.  当我们使用 iframe 的时候，就会产生多个上下文，此时就没办法保证 instanceof 的准确行了

```
const iFrame = document.createElement('IFRAME');
document.body.appendChild(iFrame);

const IFrameArray = window.frames[1].Array;
const array = new IFrameArray();

array instanceof Array; //false
array instanceof IFrameArray; //true;
```

### 在 node 里面就应该到此为止了，但是对于浏览器来说还没有结束呢！因为不同的浏览器还有各自的宿主对象呢？

很可惜的是我们的 toType 对他们并不管用：（, 比如：

```
toType(window);
//"global" (Chrome) "domwindow" (Safari) "window" (FF/IE9) "object" (IE7/IE8)

toType(document);
//"htmldocument" (Chrome/FF/Safari) "document" (IE9) "object" (IE7/IE8)

toType(document.createElement('a'));
//"htmlanchorelement" (Chrome/FF/Safari/IE) "object" (IE7/IE8)

toType(alert);
//"function" (Chrome/FF/Safari/IE9) "object" (IE7/IE8)
```

其实对于 Dom 对象来说使用 nodeType 来判断是最好的了，兼容性也 ok。

```
function isElement(obj) {
  return obj.nodeType;
}
```

> 但这其实是一个 duck-typing, 因此也是没有保证的。

### 到此总结一下

即使我们做了上面那么多的是事情依然不能够保证完美，因为 js 太灵活了。比如我们可以重写了 Object.prototype.toString 的实现，那么我们所提供的稍微靠谱的解决方案就 GG 了，因此我们最好不要随意去重写原生的 prototype 的方法。请避免把 toType 用到 host object 的判断以及要检查是否传入了未定义的变量。
