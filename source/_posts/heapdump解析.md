---
title: memory snapshot的世界
tag:
- chrome 
- memory 
- snapshot
- heapdump
- web
---
### snapshot(v8)
  * 既然要讲v8的snapshot那就得先看看snapshot的表示形式，整个snapshot里的对象是以graph的形式展示的，节点是对象(会以对象的构造函数的形式展示)，边是属性值, 例如
  ```
  class A {
    a() {}
  }
  那么展示就是 A - a(边) -> Function
  ```
  此图是snapshot的json格式图
    {% asset_img snapshot的json格式.png %}
  此图中的索引都是从零开始的
  ```
    // 对应snapshot中的字段, 查找节点i的信息
    var i_type = nodes[i];
    var i_name = strings[nodes[i + 1]];
    var i_id = nodes[i + 2];
    var i_size = nodes[i + 3]
    var i_edge_count = nodes[i + 4];
  ```
  再具体的解释如下:
  {% asset_img nodes属性.png %}
  {% asset_img edges属性.png %}
  {% asset_img 遍历nodes的方式.png %}

  * 那么接下来就要说一下snapshot里面是有哪些类型即节点的类型(当然这些类型都来自v8)[完整的数据类型](https://github.com/v8/v8/blob/master/src/objects.h)，而我们在这里要讲则是在snapshot里面定义的类型(来自v8), 这里我们要区分primitive(包扩三类number, string, symbol,js会替我们auto-boxing)和Object(不要问什么)

     {% asset_img 节点类型.png %} 
    
    从上图我们可以清晰节点的类型一共有13种，hidden类型可能不展示
  * 边的类型(即属性的表现形式)
    {% asset_img 边的类型.png %}
    从上图可以看出共有7种
    * map, array, symbol, object, regexp, number, boolean, string, date，typedarray...这些常见的内建对象我就不解释了，大家都知道
    * 我们在sanpshot中还会经常看见两类string比较生疏的两个(这两类算在pimitive中)， sliced string和 concatenated string
      * 那我们就来看看string，在v8里的形式
          {% asset_img 字符串类型.png %} 
        在这里我们可以清晰的看出来，v8的字符串类型，里面有sliced的解释，是其他字符串的部分引用(因为字面量字符串是不可变的so可以这么办。。)，而cons的就是(a, b),或者嵌套似的((a,b),c)这样的pairs(同样是因为字面量的字符串是不可变的)
  * 节点类型就上面列出的那些，但是根据具体的对象，他们又有好多不同的名字(Gc roots, map / tag, system / ...)
  
### chrome devtools 里的概念
  * #### shallow size:
    是指对象自己本身占用的大小, 不包含引用对象内容的大小
  ```
  const parent = {
    a: 23,
    child: child,
  }
  const child = {
    a: 23,
    c: 12,
  };
  /**
    child的大小就是8B(32为机器)，parent大小就有得讨论了，若不算引用的具体内容大小，parent的大小也是8，而此时就是shallow size。那么包含引用内容呢？
  */
  ```
  *
  * #### retained size: 
    是指不但包含对象自身，还包含该对象所能引用的或者间接引用（parent.child, parent.child.child）
  
  * #### GC roots
    GC roots的概念来自与垃圾回收算法，js的垃圾回收算法是基于根不可达来回收不使用的内存的，就选取某个对象作为初始点，沿着这个对象的引用链往下走，凡是通过这个对象无法访问到(是指通过引用可以获取到对象)的对象就认为是可以被回收的。

    ![](http://upload-images.jianshu.io/upload_images/7901630-a04e0112cb083905.gif?imageMogr2/auto-orient/strip)

    ![](http://fengzii.com/images//2017-07-09-java-basic-five-gc-root-set.png)

    如上图5，6，7就无法被访问到，此时就可以认为5，6，7可以被回收了
    
    1. 那么GC root 只有一个么？No.很明显不可能只有一个，因为有栈变量的存在
    2. 那么有哪些对象可以作为GC root呢， 对于js， 有Global, Window, 栈变量，内建对象等(我们是可以自己扩展内建对象的)
    3. chrome devtools 里面的distance是指什么？ 是指通过多少次引用能访问对此对象，对于Window等就是1
  * 从上面我们可以看出来，所有的对象是以树的形式展示的，devtools中如何显示对象树的？
    对象的保留树
      就像我们前面所说的，堆就是由相互连接的对象构成的网络。在数学的世界中，这种结构称作图或者内存图。一个图是由节点和边构成的，而节点又是由边连接起来的，其中节点和边都有相应的标签。

      * 节点（或者对象）是用创建对象的**构造函数**标记的。
      ```
      这里问题就来了那么js中的对象的构造函数都是啥呢
      const a = 'hello2dj';
      // a 的构造函数是 String, 但对于字面量来说还会有具体的展示类型,这与v8的内部实现相关
      ```
      * 边是用**属性名**来标记的
      对于不同的属性chrome 会标记不同的标识
      对象的属性以及属性值属于不同类型并且有着相应的颜色。每个属性都会有四种类型之一(更详细的上述snapshot里面又讲)：
        * a:property - 有名称的常规属性，通过 .（点）操作符或者 []（方括号）符号来访问，例如 ["foo bar"]；
        * 0:element - 有数字下标的常规属性，使用 []（方括号）来访问。
        * a:context var - 函数上下文中的某个变量，在相应的函数闭包中使用其名字就可以访问。
        * a:system prop - 由 JavaScript 虚拟机添加的属性，在 JavaScript 代码中无法访问。
  * Object count 挡在summary视图模式下查看时，会有这个，按照上述来说对象树的节点是constructor, 属性是边，那么object count 就是通过这个constructor 构造出来的对象实例数量

  * 巧了还有一个我们可以在devtools里经常看到的就是有些对象是黄颜色标识的有些是红色标识的，见图, 图中很明显标识红色和黄色的原因

    {% asset_img fragment.jpg %}

    {% asset_img schema.jpg %}

    * 以黄色突出显示的节点具有 JavaScript 代码对它们的直接引用。 以红色突出显示的节点则没有直接引用。只有属于黄色节点的树时，它们才处于活动状态。 一般而言，您需要将注意力放在黄色节点上。 **修复代码，使黄色节点处于活动状态的时间不长于需要的时间**，您也需要消除属于黄色节点树的红色节点。点击黄色节点对其进行进一步调查。
    * 显然红色节点就是没有js代码直接引用的对象
  * 在summary视图下第一栏是从constructor而这一栏是分两类的
    * 不带()括号的是构造器，下面包含的是用这个构造器生成的对象,这个很好实验的，自己写一个类，实例化一下就可以了
    * 另一类带括号的又有如下区分
      {% asset_img tag对象.png %} 
      见上图，他管()的行为叫tag,那就很明显了，在括号()下面的对象就是全部的这种对象了。
      {% asset_img 随心所欲.png %}

      我错了
      * (string, regexp) 显示的是literal string 即 a='234'中的'23'， regexp类似
      * (num) 显示的是以number对象展示的对象
      * (array) 那些通过数组引用的对象，说白了就是数组对象
        * (code deopt data)[]: v8去优化时的数据
        * []:就是纯数组
        * (object properties)[]:通过对象属性引用的对象
        * (map descriptors)[]: map类型相关，暂时搁置
        * (object elements)[]: 暂时未知
        * (function scope info)[]: 暂时未知
      * (system) 那就是原生代码了
      * (compiled code) 编译过后的代码
      * (closure) 通过闭包引用的对象，但感觉更像是闭包自己
      * (sliced string): 搁置
      * (undefined): 搁置
      * (concatenated string): 搁置
  * 还有一些其他[概念](https://developers.google.com/web/tools/chrome-devtools/memory-problems/memory-101)参见https://developers.google.com/web/tools/chrome-devtools/memory-problems/memory-101

### 看一次对其中两个对象的识别及分析
代码如下，num2是一个Number对象，不知道为啥，我以字面量分配的num1，我没找到。。。(有待继续)
```
num2 = new Number(234);
```
接下来的图示顺序分析的

{% asset_img @28081的引用情况.png %}
{% asset_img @28081的图.png %}
{% asset_img @15319的引用链.png %}

### 常见的内存泄露种类
  * ### 全局变量
  ```
  function globalLeak() {
    bar = 'hello2dj';
  }
  ```
  这里bar没有生命就意味着他被global引用了，那么他就不会被回收
  * ### 被遗忘的计时器或回调函数
  ```
  var someResource = getData();
  setInterval(function() {
      var node = document.getElementById('Node');
      if(node) {
          // 处理 node 和 someResource
          node.innerHTML = JSON.stringify(someResource));
      }
  }, 1000);
  ```
  此例说明：与节点或数据关联的计时器不再需要，node 对象可以删除，整个回调函数也不需要了。可是，计时器回调函数仍然没被回收（计时器停止才会被回收）。同时，someResource 如果存储了大量的数据，也是无法被回收的。

  还有时间监听：
  ```
  var element = document.getElementById('button');
    function onClick(event) {
        element.innerHTML = 'text';
    }
  element.addEventListener('click', onClick);
  ```
  * ### 被引用的dom
  有时，保存 DOM 节点内部数据结构很有用。假如你想快速更新表格的几行内容，把每一行 DOM 存成字典（JSON 键值对）或者数组很有意义。此时，同样的 DOM 元素存在两个引用：一个在 DOM 树中，另一个在字典中。将来你决定删除这些行时，需要把两个引用都清除。
  ```
    var elements = {
      button: document.getElementById('button'),
      image: document.getElementById('image'),
      text: document.getElementById('text')
  };
  function doStuff() {
      image.src = 'http://some.url/image';
      button.click();
      console.log(text.innerHTML);
      // 更多逻辑
  }
  function removeButton() {
      // 按钮是 body 的后代元素
      document.body.removeChild(document.getElementById('button'));
      // 此时，仍旧存在一个全局的 #button 的引用
      // elements 字典。button 元素仍旧在内存中，不能被 GC 回收。
  }
  ```
  此外还要考虑 DOM 树内部或子节点的引用问题。假如你的 JavaScript 代码中保存了表格某一个 <td> 的引用。将来决定删除整个表格的时候，直觉认为 GC 会回收除了已保存的 <td> 以外的其它节点。实际情况并非如此：此 <td> 是表格的子节点，子元素与父元素是引用关系。由于代码保留了 <td> 的引用，导致整个表格仍待在内存中。保存 DOM 元素引用的时候，要小心谨慎。

  * ### 闭包
  这段代码被引用了无数次了来自[meteor](http://info.meteor.com/blog/an-interesting-kind-of-javascript-memory-leak)
  ```
  var theThing = null;
  var replaceThing = function () {
    var originalThing = theThing;
    var unused = function () {
      if (originalThing)
        console.log("hi");
    };
    theThing = {
      longStr: new Array(1000000).join('*'),
      someMethod: function () {
        console.log(someMessage);
      }
    };
  };
  setInterval(replaceThing, 1000);
  ```
  代码片段做了一件事情：每次调用 replaceThing ，theThing 得到一个包含一个大数组和一个新闭包（someMethod）的新对象。同时，变量 unused 是一个引用 originalThing 的闭包（先前的 replaceThing 又调用了 theThing ）。思绪混乱了吗？最重要的事情是，闭包的作用域一旦创建，它们有同样的父级作用域，作用域是共享的。someMethod 可以通过 theThing 使用，someMethod 与 unused 分享闭包作用域，尽管 unused 从未使用，它引用的 originalThing 迫使它保留在内存中（防止被回收）。当这段代码反复运行，就会看到内存占用不断上升，垃圾回收器（GC）并无法降低内存占用。本质上，闭包的链表已经创建，每一个闭包作用域携带一个指向大数组的间接的引用，造成严重的内存泄漏。

  Meteor 的博文 解释了如何修复此种问题。在 replaceThing 的最后添加 originalThing = null 。
  ### 参考
  1. [v8类型(知乎上的一篇)](https://www.zhihu.com/question/62732293)
  2. [v8类型源码注释](https://github.com/v8/v8/blob/master/src/objects.h)
  3. [v8-object-representation](http://jayconrod.com/posts/52/a-tour-of-v8-object-representation)
  4. [js内部编码介绍](http://60sky.com/post/javascript-ucs2-utf16.html)

  5. [v8类型图](http://bespin.cz/~ondras/html/classv8_1_1Value.html)

  6. [snapshot的格式头文件](https://cs.chromium.org/chromium/src/v8/include/v8-profiler.h?sq=package:chromium&dr=C&l=356)

  7. [生成snapshot的cc文件](https://cs.chromium.org/chromium/src/v8/src/profiler/heap-snapshot-generator.cc?q=heap-snapshot-generator&sq=package:chromium&dr=C&l=17)

  8. [heap profiling](https://developer.chrome.com/devtools/docs/heap-profiling)

  9. [snapshot格式](https://github.com/longtian/javascript_performance_measurement/tree/gh-pages/heap_snapshot)

  10. [easy profiling](https://addyosmani.com/blog/taming-the-unicorn-easing-javascript-memory-profiling-in-devtools/)

  11. [常见内存泄露copy于此处](https://jinlong.github.io/2016/05/01/4-Types-of-Memory-Leaks-in-JavaScript-and-How-to-Get-Rid-Of-Them/)