---
title: css的一些基础知识
date: 2018-05-07
tags:
    - css
---

css 基础知识
> 随时会更新

<!-- more -->

### 生成 BFC

*   根元素

*   float 属性不为 none

*   position 为 absolute 或 fixed

*   display 为 inline-block, table-cell, table-caption, flex, inline-flex

*   overflow 不为 visible

记忆规则： absolute, fixed, inline-block, overflow

### BFC 布局规则

*   非 BFC 盒子内的第一个元素的 margin-top 设置为正值是不会撑开父元素的，而是紧贴着把父元素的兄弟元素给撑开。
*   内部的 Box 会在垂直方向，一个接一个地放置。

*   Box 垂直方向的距离由 margin 决定。属于同一个 BFC 的两个相邻 Box 的 margin 会发生重叠

*   每个元素的左外边缘（margin-left)， 与包含块的左边（contain box left）相接触(对于从左往右的格式化，否则相反)。即使存在浮动也是如此。除非这个元素自己形成了一个新的 BFC。

*   BFC 的区域不会与 float box 重叠。(不会被兄弟float元素覆盖)

*   BFC 就是页面上的一个隔离的独立容器，容器里面的子元素不会影响到外面的元素。反之也如此。

*   计算 BFC 的高度时，浮动元素也参与计算

### IFC 布局规则

*   框会从包含块的顶部开始，一个接一个地水平摆放。
*   摆放这些框的时候，它们在水平方向上的外边距、边框、内边距所占用的空间都会被考虑在内。在垂直方向上，这些框可能会以不同形式来对齐：它们可能会把底部或顶部对齐，也可能把其内部的文本基线对齐。能把在一行上的框都完全包含进去的一个矩形区域，被称为该行的行框。水平的 margin、padding、border 有效，垂直无效。不能指定宽高。
*   行框的宽度是由包含块和存在的浮动来决定。行框的高度由行高计算这一章所描述的规则来决定。

### IFC 结论

*   一个 line box 总是足够高对于包含在它内的所有盒子。然后，它也许比包含在它内最高的盒子高。(比如，盒子对齐导致基线提高了)。
*   当盒子 B 的高度比包含它的 line box 的高度低，在 line box 内的 B 的垂值对齐线通过’vertical align’属性决定。当几个行内级盒子在一个单独的 line box 内不能很好的水平放置，则他们被分配成了 2 个或者更多的垂直重叠的 line boxs.因此,一个段落是很多个 line boxs 的垂直叠加。Line boxs 被叠加没有垂直方向上的分离(特殊情况除外)，并且他们也不重叠。
*   通常，line box 的左边缘挨着它的包含块的左边缘，右边缘挨着它的包含块的右边缘。然而，浮动盒子也许会在包含块边缘和 line box 边缘之间。因此，尽管 line boxs 在同样的行内格式上下文中通常都有相同的宽度(就是他的包含块的宽度)，但是水平方向上的空间因为浮动被减少了，它的宽度也会变得复杂。Line boxs 在同样的行内格式上下文中通常在高度上是多样的(比如，一行也许包含了一个最高的图片然后其他的也可以仅仅只包含文字)
*   当在一行中行内级盒子的总宽度比包含他们的 line box 的宽度小，他们的在 line box 中的水平放置位置由’text align’属性决定。如果属性是’justify’，用户代理可能会拉伸空间和文字在 inline boxs 内。
*   当一个行内盒子超过了 line box 的宽度，则它被分割成几个盒子并且这些盒子被分配成几个横穿过的 line boxs。如果一个行内盒子不能被分割。则行内盒子溢出 line box。
*   当一个行内盒子被分割，分割发生则 margins,borders,和 padding 便没有了视觉效果。
*   在同样的 line box 内的行内盒子也许会被分割成几个盒子因为双向的文字。Line boxs 在行内格式上下文中档需要包含行内级内容时被创造。Line boxs 包含没有文字，没有空格，没有带着 margins,padding 和 borders，以及没有其他在流中的内容(比如图片，行内盒子和行内表格)，也不会以新起一行结尾。对于在他们内的任何盒子的位置都以他们决定并且必须将他们视作没有高度的 line boxs。

### IFC 的 css

*   font-size
*   line-height
*   height
*   vertical-aligin

### block ele 块级元素

> a block-level box is also a block container box. A block container box either contains only block-level boxes or establishes an inline formatting context and thus contains only inline-level boxes. Not all block container boxes are block-level boxes: non-replaced inline blocks and non-replaced table cells are block containers but not block-level boxes. Block-level boxes that are also block containers are called block boxes.
> 解释一下 块级盒子也是一个块级包含块盒子， 他要么只包含块级盒子要么建立一个 IFC 并且只包含行级盒子。并不是所有的块级包含快盒子都是块级盒子，也有不可替换的 inline-block 或者其他的。

*   解释 1 只包含块级元素或者是创建一个 IFC
    1.  只包含块级元素,就是说若是一个块级盒子里面包含了一个块级元素，那么它里面就只有块级元素！你在骗我吧，你看我是可以这么写的？
    ```
    <div>
      <p>I love you</p>
      <span>真逗我不love你</span>
      那我来爱你吧
    <div>
    ```
    问：你看我这个 div 里不止有 p,还有 span 还有 text,你咋说？答：没错但是你看[规范](https://www.w3.org/TR/CSS2/visuren.html#block-level) 9.2.1.1 Anonymous block boxes
    就是说像你这种情况他是会生成一个匿名块级盒子的，那么你的代码就是这样的了
    ```
     <div>
      <p>I love you</p>
      <anonymous-block-boxes><span>真逗我不love你</span>
      那我来爱你吧</anonymous-block-boxes>
    <div>
    ```
    我觉得这也是合理的就像规范里说的，这种方式也易于实现，要不浏览器的实现不得费劲啊。这样规定就意味着，块级元素只包含块级元素或者是创建一个 IFC
    IFC 的栗子
    ```
    <div>i love you</div>
    ```
    但事实上，这里不止创建 IFC 还有一个这个 Anonymous inline boxes，因为 html 里所有的东西都是被盒子包裹的文本也不例外，你没写盒子，并不代表就没有，其实是有匿名盒子生成的。

### tricks

*   [两栏布局](https://segmentfault.com/a/1190000010698609)

*   [分栏高度自动相等](http://www.zhangxinxu.com/wordpress/2010/03/%E7%BA%AFcss%E5%AE%9E%E7%8E%B0%E4%BE%A7%E8%BE%B9%E6%A0%8F%E5%88%86%E6%A0%8F%E9%AB%98%E5%BA%A6%E8%87%AA%E5%8A%A8%E7%9B%B8%E7%AD%89/)

    *   使用 margin-bottom 负边距，padding-bottom 正边距, 原理是 padding-bottom 足够大让人误以为你的高度是在自动增加而实际是你增加的高度不能超过 padding-bottom 的值，否则就会看到白框了，margin-bottom 负边距是为了让后面的元素能够上移到 content 元素的后面，而不是被 padding 给撑开。

*   [水平垂直居中的方式](http://louiszhai.github.io/2016/03/12/css-center/)

*   [层叠上下文](http://www.zhangxinxu.com/wordpress/2016/01/understand-css-stacking-context-order-z-index/) 背景<布局<内容

### 层叠准则： 务必牢记的层叠准则

下面这两个是层叠领域的黄金准则。当元素发生层叠的时候，其覆盖关系遵循下面 2 个准则：

*   谁大谁上：当具有明显的层叠水平标示的时候，如识别的 z-indx 值，在同一个层叠上下文领域，层叠水平值大的那一个覆盖小的那一个。通俗讲就是官大的压死官小的。
*   后来居上：当元素的层叠水平一致、层叠顺序相同的时候，在 DOM 流中处于后面的元素会覆盖前面的元素。

### 层叠上下文的特性

层叠上下文元素有如下特性：

1.  层叠上下文的层叠水平要比普通元素高（原因后面会说明）；
2.  层叠上下文可以阻断元素的混合模式（见此文第二部分说明）；
3.  层叠上下文可以嵌套，内部层叠上下文及其所有子元素均受制于外部的层叠上下文。
4.  每个层叠上下文和兄弟元素独立，也就是当进行层叠变化或渲染的时候，只需要考虑后代元素。
5.  每个层叠上下文是自成体系的，当元素发生层叠的时候，整个元素被认为是在父层叠上下文的层叠顺序中。

### 翻译成真实世界语言就是：

1.  当官的比老百姓更有机会面见圣上；
2.  领导下去考察，会被当地官员阻隔只看到繁荣看不到真实民情；
3.  一个家里，爸爸可以当官，孩子也是可以同时当官的。但是，孩子这个官要受爸爸控制。
4.  自己当官，兄弟不占光。有什么福利或者变故只会影响自己的孩子们。
5.  每个当官的都有属于自己的小团体，当家眷管家发生摩擦磕碰的时候（包括和其他官员的家眷管家），都是要优先看当官的也就是主子的脸色。

### 层叠上下文的创建

1.  根元素（很厉害的，本身是 BFC,还是具有层叠上下文）
2.  定位元素对于包含有 position:relative/position:absolute 的定位元素，以及 FireFox/IE 浏览器（不包括 Chrome 等 webkit 内核浏览器）（目前，也就是 2016 年初是这样）下含有 position:fixed 声明的定位元素，当其 z-index 值不是 auto 的时候，会创建层叠上下文。典型的栗子，猜猜谁上谁下

```
<div style="position:relative; z-index:auto;">
    <img src="mm1.jpg" style="position:absolute; z-index:2;">    <-- 横妹子 -->
</div>
<div style="position:relative; z-index:auto;">
    <img src="mm2.jpg" style="position:relative; z-index:1;">    <-- 竖妹子 -->
</div>
```

还有这个

```
<div style="position:relative; z-index:auto;">
    <img src="mm1.jpg" style="position:absolute; z-index:2;">    <-- 横妹子 -->
</div>
<div style="position:relative; z-index:auto;">
    <img src="mm2.jpg" style="position:relative; z-index:1;">    <-- 竖妹子 -->
</div>
```

3.  CSS3 与新时代的层叠上下文
1.  z-index 值不为 auto 的 flex 项(父元素 display:flex|inline-flex).
    注意，这里的规则有些负责复杂。要满足两个条件才能形成层叠上下文：条件 1 是父级需要是 display:flex 或者 display:inline-flex 水平，条件 2 是子元素的 z-index 不是 auto，必须是数值。此时，这个子元素为层叠上下文元素，没错，注意了，是子元素，不是 flex 父级元素。
1.  元素的 opacity 值不是 1.
1.  元素的 transform 值不是 none.
1.  元素 mix-blend-mode 值不是 normal.
1.  元素的 filter 值不是 none.
1.  元素的 isolation 值是 isolate.
1.  will-change 指定的属性值为上面任意一个。
1.  元素的-webkit-overflow-scrolling 设为 touch.

再具体的请参见原文

### 同时设置 margin-left 和 margin-right [参见](https://segmentfault.com/a/1190000007184954)

更正： 第二种情况中若是没有设置宽度也是和第一种情况一样，设置负值会增加相应方向的宽度。

### 内容的包裹性，如何让你的盒子的宽度自适应内容呢？

1.  行内元素肯定是可以的可是我们没办法控制行内元素的高度等属性
2.  float, inline-block，其实还有 position:absolute
    但是这真的可以么？也不是[参见](http://jsbin.com/mamigevolo/2/edit?html,css,output)
    为什么还那么宽呢？因为我们里面放了两个 inline 元素，而第二个太长有放不下，于是挪到下一行了，但他占的位置依然在。so...

### float 的一些[规则](http://dev.dafan.info/detail/513780?p=)

[另一个](https://zhanglun.github.io/2014/10/15/%E8%A7%84%E8%8C%83%E4%B9%8B%E8%B7%AF-%E9%87%8D%E6%96%B0%E8%AE%A4%E8%AF%86%E4%BA%86Float/)
还有鑫空间的不贴了，一搜就有

### [宽度计算法则](http://f2ecouple.github.io/2014/03/16/css%E5%AE%BD%E5%BA%A6%E8%AE%A1%E7%AE%97%E7%AE%97%E6%B3%95/)

### position：absolute 的定位参考是包含块，若是没有定位为非 static 的父元素，则是依据的是 viewport 定位（是根元素的包含块）。

### 基本上来说，reflow 有如下的几个原因：

*   Initial。网页初始化的时候。
*   Incremental。一些 Javascript 在操作 DOM Tree 时。
*   Resize。其些元件的尺寸变了。
*   StyleChange。如果 CSS 的属性发生变化了。
*   Dirty。几个 Incremental 的 reflow 发生在同一个 frame 的子树上。我们来看一个示例：

```
var bstyle = document.body.style; // cache
bstyle.padding = "20px"; // reflow, repaint
bstyle.border = "10px solid red"; //  再一次的 reflow 和 repaint

bstyle.color = "blue"; // repaint
bstyle.backgroundColor = "#fad"; // repaint

bstyle.fontSize = "2em"; // reflow, repaint

// new DOM element - reflow, repaint
document.body.appendChild(document.createTextNode('dude!'));
```

当然，我们的浏览器是聪明的，它不会像上面那样，你每改一次样式，它就 reflow 或 repaint 一次。一般来说，浏览器会把这样的操作积攒一批，然后做一次 reflow，这又叫异步 reflow 或增量异步 reflow。但是有些情况浏览器是不会这么做的，比如：resize 窗口，改变了页面默认的字体，等。对于这些操作，浏览器会马上进行 reflow。

但是有些时候，我们的脚本会阻止浏览器这么干，比如：如果我们请求下面的一些 DOM 值：

*   offsetTop(定位距离+margin-top), offsetLeft, offsetWidth(dom 对象的可见宽度包括滚动条等), offsetHeight
*   scrollTop(顶部已经滚动的距离)/Left/Width(元素完整的高度和宽度包括 overflow: hidden 的部分)/Height
*   clientTop(就是 border-top)/Left/Width(dom 内容的宽度)/Height
*   IE 中的 getComputedStyle(), 或 currentStyle

因为，如果我们的程序需要这些值，那么浏览器需要返回最新的值，而这样一样会 flush 出去一些样式的改变，从而造成频繁的 reflow/repaint。

### 各种 top,height(http://www.cnblogs.com/gagarinwjj/p/conflict_client_offset_scroll.html， https://github.com/pramper/Blog/issues/10)

*   offet 的 top, left 就是距离定位系统的父元素的距离(比如是相对父元素的定位那么算出来的就是距离父元素的距离)

### event 对象中有

offsetX/Y, clientX/Y, pageX/Y, screenX/Y 等。

screenX:鼠标位置相对于用户屏幕水平偏移量，而 screenY 也就是垂直方向的，此时的参照点也就是原点是屏幕的左上角。

clientX:跟 screenX 相比就是将参照点改成了浏览器内容区域的左上角，该参照点会随之滚动条的移动而移动。

pageX：参照点也是浏览器内容区域的左上角，但它不会随着滚动条而变动

### flex 布局（https://zhuanlan.zhihu.com/p/25303493）

*   当设置了 display: flex 时， 子元素的 float、clear、vertical-align 都会失效
*   并且当设置了 flex-basis 时 width 设置就不起作用了

### getComputedStyle 可以获取伪类元素的样式

### 可以通过 parentNode 来获取 node 的 parent 元素

### offsetParent (https://www.cnblogs.com/xiaohuochai/p/5828369.html) 是指元素的定位父元素（只有有 position 不为 static 才能成为 offsetParent 元素， 若是没有 position 定位的父级元素那么 offsetParent 就是 body, 而 fixed 元素的 offsetParent 是 null）

offsetParent 是用来计算 offsetTop 等值 的。他和 parentNode 是不同的，parentNode 父亲节点 HTML 结构层级关系中的上一级元素。因为所有的元素最后计算偏移都是和 body 元素的偏移。

### .offsetTop/offsetLeft:当前元素距离父级参照物上/左边距偏移量, 我们要注意 offsetTop 可不是 scrollTop, 一个偏移距离，一个是滚动距离

->offset():等同于 jQuery 中的 offset 方法，实现获取页面中任意一个元素距离 body 的偏移（包含左偏移和上偏移），不管当前元素的父级参照物是谁。
->获取的结果是一个对象{left:距离 body 的左偏移量，top:距离 body 上偏移}
->在标准的 ie8 浏览器中，我们使用 offsetLeft/offsetTop 其实是把父级参照物的边框也计算在内了

```
function offset(curEle){
var totalLeft = null,totalTop = null,par = curEle.offsetParent;
//首先把自己本身的进行累加
totalLeft += curEle.offsetLeft;
totalTop += curEle.offsetTop;

    //只要没有找到body，我们就把父级参照物的边框和偏移量累加
    while(par){
                if(navigator.userAgent.indexOf("MSIE 8.0") === -1){
                    //不是标准的ie8浏览器，才进行边框累加
                    //累加父级参照物边框
                    totalLeft += par.clientLeft;
                    totalTop += par.clientTop;
                }
            //累加父级参照物本身的偏移
            totalLeft += par.offsetLeft;
            totalTop += par.offsetTop;
            par = par.offsetParent;
    }
    return {left:totalLeft,top:totalTop};

}
console.log(offset(box).top);
```

### getBoundingClientRect

DOMRect 对象包含了一组用于描述边框的只读属性——left、top、right 和 bottom，单位为像素。除了 width 和 height 外的属性都是相对于视口(viewPort)的左上角位置而言的。

### getClientRects, 对于 块级元素 来说，这两个其实没有什么区别的，关键是对于 内联元素 这两个有明显的区别。简单的说就是 内联元素 不在一行的时候每行都会产生一个矩形范围，而 getBoundingClientRect 并不会。

getClientRects 返回的是数组，对于行内元素及其有用，尤其是当行内元素不在一行的时候

[宽度计算](http://f2ecouple.github.io/2014/03/16/css%E5%AE%BD%E5%BA%A6%E8%AE%A1%E7%AE%97%E7%AE%97%E6%B3%95/)

### transform-origin 以及 transform： scale 的妙用

transform-orgin 默认是 50%， 50%

中间向两端延伸动画

```
div {

    position: absolute;

    width: 200px;

    height: 60px;

}



div::before {

    content: "";

    position: absolute;

    left: 0;

    bottom: 0;

    width: 200px;

    height: 2px;

    background: deeppink;

    transition: transform .5s;

    transform: scaleX(0);

}



div:hover::before {

    transform: scaleX(1);

}
```

从左侧出来再右侧消失的动画

```
div {

    position: absolute;

    width: 200px;

    height: 60px;

}



div::before {

    content: "";

    position: absolute;

    left: 0;

    bottom: 0;

    width: 200px;

    height: 2px;

    background: deeppink;

    transition: transform .5s;

    transform: scaleX(0);

    transform-origin: 100% 0;

}



div:hover::before {

    transform: scaleX(1);

    transform-origin: 0 0;

}
```

结合 transform-orgin 和 scale 可以做出很多初始位置与结束位置不同的动画
[demo](https://codepen.io/Chokcoco/pen/ELxmwE)

### 使用 css 接收页面点击事件

1.  :target 是 CSS3 新增的一个伪类，可用于选取当前活动的目标元素。当然 URL 末尾带有锚名称 #，就可以指向文档内某个具体的元素。这个被链接的元素就是目标元素(target element)。它需要一个 id 去匹配文档中的 target 。

```
<ul class='nav'>
    <li><a href="#content1">列表1</a></li>
    <li><a href="#content2">列表2</a></li>
</ul>
<div id="content1">列表1内容:123456</div>
<div id="content2">列表2内容:abcdefgkijkl</div>
```

的那我们点击 a 标签时，如下的类选择器就会被触发

```
#content1:target,
#content2:target {
    color: black;
}
```

2.  input[radio|checkbox] 点击可以触发 inpu:check 选择器， <label for=""> for 指定 input id 可以绑定 input,就是说点击 label 就相当于点击 input

```
<div class="container">
    <input class="nav1" id="li1" type="radio" name="nav">
    <input class="nav2" id="li2" type="radio" name="nav">
    <ul class='nav'>
        <li class='active'><label for="li1">列表1</label></li>
        <li><label for="li2">列表2</label></li>
    </ul>
    <div class="content">
        <div class="content1">列表1内容:123456</div>
        <div class="content1">列表2内容:abcdefgkijkl</div>
    </div>
</div>
```

### 多个元素的边界线问题，最后一个不要

消失的边界线问题，《css 设计指南》看到过，有一种巧妙的方法。
li+li{border-left: 1px solid #000;} // 单行 li

### 字体定义顺序是一门学问，通常而言，我们定义字体的时候，会定义多个字体或字体系列。举个栗子：

body {
font-family: tahoma, arial, 'Hiragino Sans GB', '\5b8b\4f53', sans-serif;
}
别看短短 5 个字体名，其实其中门道很深。解释一下：

1.  使用 tahoma 作为首选的西文字体，小字号下结构清晰端整、阅读辨识容易；
2.  用户电脑未预装 tohoma，则选择 arial 作为替代的西文字体，覆盖 windows 和 MAC OS；
3.  Hiragino Sans GB 为冬青黑体，首选的中文字体，保证了 MAC 用户的观看体验；
4.  Windows 下没有预装冬青黑体，则使用 '\5b8b\4f53' 宋体为替代的中文字体方案，小字号下有着不错的效果；
5.  最后使用无衬线系列字体 sans-serif 结尾，保证旧版本操作系统用户能选中一款电脑预装的无衬线字体，向下兼容。嗯，其实上面的 font-family 就是淘宝首页 body 的字体定义，非常的规范，每一个字体的定义都有它的意义。综上，总结一下，我觉得字体 font-family 定义的原则大概遵循：

1、兼顾中西中文或者西文（英文）都要考虑到。

2、西文在前，中文在后由于大部分中文字体也是带有英文部分的，但是英文部分又不怎么好看，同理英文字体中大多不包含中文。

所以通常会先进行英文字体的声明，选择最优的英文字体，这样不会影响到中文字体的选择，中文字体声明则紧随其次。

3、兼顾多操作系统选择字体的时候要考虑多操作系统。例如 MAC OS 下的很多中文字体在 Windows 都没有预装，为了保证 MAC 用户的体验，在定义中文字体的时候，先定义 MAC 用户的中文字体，再定义 Windows 用户的中文字体；

4、兼顾旧操作系统，以字体族系列 serif 和 sans-serif 结尾当使用一些非常新的字体时，要考虑向下兼容，兼顾到一些极旧的操作系统，使用字体族系列 serif 和 sans-serif 结尾总归是不错的选择。

### hover 的使用

[hover](http://linxz.github.io/CSS_Skills/demo/other/about_hover_pseudo_class.html#demo2_1_a_2)

### 当 button 被点击时的状态变化 初始 -> hover(直到鼠标离开都是 hover) -> focus(点击后获取焦点，再点击其他或者 tab 键时会失去 focus) -> active(点击松开后就失去了)， 因此 active 应当放到最后，否则会被作用时长比他长的样式覆盖，比如 hover 的样式

### css loader 简单实现，border-radius, boder-top-color: 透明

```
.loader {

    display: none;
    width: 50px;
    height: 50px;
    border: 4px solid #fff;
    border-top-color: transparent;
    border-radius: 50%;
    margin: 0 auto;
    animation: spin 400ms linear infinite;

}
```

### 居中的方式
http://www.html-js.com/article/4613

### [图片预览的方式](https://www.cnblogs.com/rubylouvre/p/4597344.html)
基本原则就是造一个img标签

### [domReady](https://www.cnblogs.com/rubylouvre/p/4536334.html)

### 两栏布局
```
.wrapper {
    display: flex;
    justify-content: space-between;
}
.left {
    background: blue;
    width: 200px;
    height: 400px;
}
.right {
    width: calc(100% - 200px);
    height: 400px;
    background: green;
}
```

```
.left {
    background: blue;
    float: left;
    width: 200px;
    height: 400px;
}
.right {
    width: calc(100% - 200px);
    height: 400px;
    background: green;
    display: inline-block;
}
```
### [CSS深入理解流体特性和BFC特性下多栏自适应布局](http://www.zhangxinxu.com/wordpress/2015/02/css-deep-understand-flow-bfc-column-two-auto-layout/)

### waterfall 布局
1. 我先入为主以为waterfall 布局就是得横向一个一个布局的，但是若是所有的宽度一致的话，我们可以认为是按照列布局的啊！！！
```
<div class="1">
    <div class="ele">
    </div>
    <div class="ele">
    </div>
    <div class="ele">
    </div>
</div>
<div class="2">
    <div class="ele">
    </div>
    <div class="ele">
    </div>
</div>
<div class="3">
    <div class="ele">
    </div>
    <div class="ele">
    </div>
</div>
<div class="4">
    <div class="ele">
    </div>
    <div class="ele">
    </div>
    <div class="ele">
    </div>
</div>
```
如上我们就把一个一个的横向布局计算absolute位置，变为了，多列等宽布局（只需检测是否要改变列数）每列内部以此顺序布局即可。

[参见]()

### 流体特性
块状水平元素，如div元素（下同），在默认情况下（非浮动、绝对定位等），水平方向会自动填满外部的容器；如果有margin-left/margin-right, padding-left/padding-right, border-left-width/border-right-width等，实际内容区域会响应变窄。
对于浏览器的元素布局来说最基础的就是一个一个的横向排列，放不下了就另换一行继续进行。

### [包含块](https://www.w3.org/TR/CSS2/visudet.html#containing-block-details) （http://w3help.org/zh-cn/kb/008/）
Notably, a containing block is not a box (it is a rectangle), however it is often derived from the dimensions of a box

值得注意的是，一个包含块不是一个盒子（它是一个矩形），然而它通常是从盒子的尺寸派生的

### containing block chain
A sequence of successive containing blocks that form an ancestor-descendant chain through the containing block relation. For example, an inline box’s containing block is the content box of its closest block container ancestor; if that block container is an in-flow block, then its containing block is formed by its parent block container; if that grandparent block container is absolutely positioned, then its containing block is the padding edges of its closest positioned ancestor (not necessarily its parent), and so on up to the initial containing block.

1. position: static/relative 找的是最近的block containers, block container 是包含IFC或者是BFC的

### CSS3 transform使position:fixed元素absolute化实例页面
http://www.zhangxinxu.com/study/201505/css3-transform-position-fixed-to-absolute.html

### 宽度计算的另一种和containing block有关
'margin-left' + 'border-left-width' + 'padding-left' + 'width' + 'padding-right' + 'border-right-width' + 'margin-right' = width of containing block

其中margin-left/width/margin-right可为auto，且具有以下规则：

1. 若width为auto，则其他设置为auto的属性的实际值为0，并让width的实际值满足等式；
2. 若width为数值，而margin-left/right均为auto，且除marin-left/right外其他属性值总和小于containing block的宽度，那么margin-left == margin-right == ('border-left-width' + 'padding-left' + 'width' + 'padding-right' + 'border-right-width')/2；否则margin-left == margin-right == 0.

### 尺寸
相对定位元素的尺寸，会保持它在常规流中的尺寸。包括换行以及原来为它保留的位置。

定位及计算偏移后的值

'left' 和 'right' 的特性值

对于一个相对定位的元素，'left' 和 'right' 会水平的位移框而不会改变它的大小。'left' 会将框向右移动，'right' 会将框向左移动。 由于 'left' 或者 'right' 不会造成框被拆分或者拉伸，所以，计算后的值( computed value )总是：left = -right。

1. 'left' 和 'right' 的设定值都是 "auto"

如果 'left' 和 'right' 的值都是 "auto" （它们的初始值），计算后的值( computed value )为 0（例如，框区留在其原来的位置）。

2. 'left' 或 'right' 其一的设定值为 "auto"

如果 left 为 ‘auto’，计算后的值(computed value)为 right 的负值（例如，框区根据 right 值向左移）。 如果 right 被指定为 ‘auto’，其计算后的值(computed value)为 left 值的负值。

示例代码：
```
<div style="width:20px; height:20px; background-color:red; position:relative; left:100px;"></div>
```

上述代码中，DIV 元素是相对定位的元素，它的 'left' 值是 "100px"， 'right' 没有设置，默认为 "auto"，那么，'right' 特性计算后的值应该是 -left，即 "right:-100px"。

3. 'left' 和 'right' 设定值都不是 "auto"

如果 'left' 和 'right' 都不是 "auto"，那么定位就显得很牵强，其中一个不得不被舍弃。如果包含块的 'direction' 属性是 "ltr"， 那么 'left' 将获胜，'right' 值变成 -left。如果包含块的 'direction' 属性是 ‘rtl’，那么 'right' 获胜，'left' 值将被忽略。

示例代码：
```
<div style="width:100px; height:100px; overflow:auto; border:1px solid blue;">
    <div style="width:20px; height:20px; background-color:red; position:relative; left:60px; right:60px;"></div>
</div>
```
最后，'left' 应该比较强悍才对。

### float
http://efe.baidu.com/blog/float/

1. float时 position是fixed和absolute则浮动相对于失效
2. 其他float有效，position失效


浮动元素的特点
1. 元素被视作块级元素，相当于display设置为“block”；

2. 元素具备包裹性，会根据它所包含的元素实现宽度、高度自适应；

3. 浮动元素前后的 ###*块级兄弟元素*### 忽视浮动元素的而占据它的位置，并且元素会处在浮动元素的下层（并且无法通过z-index属性改变他们的层叠位置），但它的内部文字和其他行内元素都会环绕浮动元素；

4. 浮动元素前后的行内元素环绕浮动元素排列；

5. 浮动元素之前的元素如果也是浮动元素，且方向相同，它会紧跟在它们后面；父元素宽度不够，换行展示；

6. 浮动元素之间的水平间距不会重叠；

7. 当包含元素中只有浮动元素时，包含元素将会高度塌陷；

8. 浮动元素的父元素的非浮动兄弟元素，忽视浮动元素存在，覆盖浮动元素；

9. 浮动元素的父元素的浮动兄弟元素，会跟随浮动元素布局，仿佛处在同一父元素中。

浮动对兄弟元素的影响

Since a float is not in the flow, non-positioned block boxes created before and after the float box flow vertically as if the float did not exist. However, the current and subsequent line boxes created next to the float are shortened as necessary to make room for the margin box of the float.

块级非定位元素自动排列就好像没有浮动元素似的，行内元素则需要为浮动元素腾出位置(对于层级上下文来说，浮动元素高于块级盒子)

浮动元素之间不重叠；尽可能像边缘漂浮，但不越界。

那么第八条、第九条为什么？看CSS标准中的下面的描述：

References to other elements in these rules refer only to other elements in the same block formatting context as the float.

也就是说，float对同一个BFC内的元素有效。如果父元素没有触发生成新的BFC，那么父元素的兄弟元素都算是跟父元素中的元素处于同一BFC，也就会受浮动的影响，并且行为规则与同处于同一个父元素之中的元素的规则相同：块级元素重叠(和float元素重叠)；行内元素环绕；浮动元素跟随。
```
<div id="a">
  <div id="b">
  </div>
</div>

<div id="c">
</div>

#a {
  width: 300px;

  background: blue;

}

#b {
  width: 200px;
  height: 10px;
  background: red;
  float: left;
}

#c {
  width: 200px;
  height: 100px;
  background: green;

}
```

正是因为浮动元素的这三条特点，因此，在使用了浮动元素以后，通常都要做“清除浮动“或”闭合浮动“的操作，来避免浮动元素对其他元素的影响


### flexbox
flex: flex-grow(相对于其他元素的增长比例) flex-shrink(相对于其他元素的缩小比例) flex-baisis(基础大小)

1. flexbox 无法使用float
2. 其他定位元素照用
3. overflow 可以使用
4. flex-container 回创建一个containing-block
5. vertical-align无法使用在flex-item


### block container
Except for table boxes, which are described in a later chapter, and replaced elements, a block-level box is also a block container box. A block container box either contains only block-level boxes or establishes an inline formatting context and thus contains only inline-level boxes. Not all block container boxes are block-level boxes: non-replaced inline blocks and non-replaced table cells are block containers but not block-level boxes. Block-level boxes that are also block containers are called block boxes.

块级盒子都是block container, 但block container不都是块级盒子，还有非替换行内块级盒子等

### indeterminate 属性 只能通过js操作，半赋值
https://imququ.com/post/native-tri-state-checkbox.html


### [svg 多色图标](https://juejin.im/post/5a8409e06fb9a063342672b6) https://css-tricks.com/icon-fonts-vs-svg/

### [css hack各个浏览器的方法](https://github.com/pod4g/tool/wiki/CSS-HACK)

### 对内 css api

### css in js

props(react 社区的 style components):

1.  local namespace & scoped
2.  Smarter critical css extraction // 更精细的加载 css
3.  Dynamic styles
4.  Shared style values
5.  Manageable API(对外提供了可控的 api)

cons:

1.  runtime 级别的无法使用现有的静态 css 处理器
2.  重新开发工具链

### 对外的 api，扩展某一个组件

1.  css in js: 预定义样式（className）, 以 enum 方式开放， 就可以修改这些 className 的样式
2.  css out of js: 让使用者写 css 样式，这样就无法修改 htmL 结构

### 全局 样式

### 样式代码尽可能少的 hard code

### 互操作的 api

### web 本身是开放的

### BEM & atomic

### css hover 子元素时 父元素也处于 hover 状态

典型的就是菜单，当我们 hover 父元素显示出菜单然后挪到子元素时子元素依然在（采用父元素 hover 子元素 来显示）反例是当你把菜单子元素挪出去放到作为父元素的兄弟元素然后继续使用 hover 显示，你在去挪动就会发现兄弟元素不会再显示了。
()[https://codepen.io/dengshen/pen/erGyaP]

### mouseout/mouseleave

mouseout 当从父元素移到子元素时即使没有出父元素也会触发 mouseout
mousleave 即使从父元素移到子元素时 在子元素上移动也不会触发 mouseleave

### 视差滚动

1.  background-attachment: fixed 滚动背景固定不跟随滚动，改变 background-position 来滚动
2.  onscroll: 改变各个元素的 style.top 来滚动（同上 chrome 会有跳动，因为 chrome 对滚动做了优化，很多次滚动才触发一次 scroll 事件, 相当于多帧时间只有一帧动画）
3.  mousewheel： 同上

### 滚动性能优化(https://www.cnblogs.com/coco1s/p/5499469.html)

### img 标签多三像素的问题（https://github.com/muwenzi/Program-Blog/issues/121）

原因是因为 img 标签是 inline 元素对齐方式是 base-line 和文字的对齐方式一致， 因此设置 font-size: 0

### getBoundingClientRect() 来获取页面元素的位置（https://juejin.im/entry/59c1fd23f265da06594316a9）

使用

1.  获取元素在视窗的位置

```
let box = document.getElementId('box')
const rect = box.getBoundingClientRect()
rect.left 就是元素左边距距离视窗的距离
rect.top 就是元素上边距距离视窗的距离
```

2.  获取元素在页面的位置

```
let box = document.getElementId('box')
const rect = box.getBoundingClientRect()
rect.left + document.documentElement.scrollLeft
rect.top + document.docuemntElement.scrollTop
```

3.  判断元素是否在可见区域

```
function isElementInViewport (el) {
    var rect = el.getBoundingClientRect();
    return (
        rect.top >= 0 &&
        rect.left >= 0 &&
        rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) && /_or(window.height() _/
        rect.right <= (window.innerWidth || document.documentElement.clientWidth) /_or $(window.width() _/
    );
}
```

### 获取元素的各种高度(https://blog.csdn.net/woxueliuyun/article/details/8638427)

*   #### clientHeight

    大部分浏览器对 clientHeight 都没有什么异议，认为是元素可视区域的高度，也就是说元素或窗口中可以看到内容的这个区域的高度，即然是指可看到内容的区域，滚动条不算在内。但要注意 padding 是算在内。其计算方式为 clientHeight = topPadding + bottomPadding+ height - 水平滚动条高度。

*   #### offsetHeight

    在 IE6，IE7，IE8， IE9 以及最新的的 FF, Chrome 中，对于一般元素，都是 offsetHeight = padding + height + border = clientHeight + 滚动条 + 边框

*   ### scrollHeight

    scrollHeight 的争议比较大，有些浏览器认为 scrollHeight 可以小于 clientHeight，有些认为 scrollHeight 至少应该等于 clientHeight。但有一点是一样的，就是 scrollHeight >= topPadding + bottomPadding + 内容 margin box 的高度。

    在浏览器中的区别在于：
    IE6、IE7 认为 scrollHeight 是内容高度，可以小于 clientHeight。

    FF 认为 scrollHeight 是内容高度，不过最小值是 clientHeight。

    注： 以上都是对于一般元素而方言的，body 和 documentElement 的 clientHeight, offsetHeight 和 scrollHeight 在各个浏览器中的计算方式又不同。

    在所有的浏览器中，如果你想获取视窗可见部分的高度，应该使用 documentElement.clientHeight，因为 body.clientHeight 是由它的内容决定的。

### viewport 通常被称作视口（视窗 ），是指设备的屏幕上能用来显示网页的那一块区域

### 如何获取设备像素（屏幕尺寸）？

    通常我们可以从 BOM(Browser Object Model) 中通过 screen.width/screen.height 获取。

### 如何获取窗口尺寸？

    如果你想知道用户访问的页面中有多少空间可以用来 CSS 布局，那么你需要获取浏览器窗口的内部尺寸。可以通过 window.innerWidth/window.innerHeight 来获取这些尺寸。注意度量的宽度和高度是包括滚动条的。它们也被视为内部窗口的一部分。（这大部分是因为历史原因造成的。）

### 如何获取 HTML 文档的尺寸？

    可见宽度：document.documentElement.clientWidth

    实际宽度：document.documentElement.offsetWidth

    实际上，document.documentElement 指的是 <html> 元素：即任何 HTML 文档的根元素。

### layout viewport 和 visual viewport

    当我们比较移动浏览器和桌面浏览器的时候，它们最显而易见的不同就是屏幕尺寸。当我们打开一个未针对移动端做任何 CSS 适配的页面时，我们会不由的慨叹：viewport 太窄了！viewport 并不能按照写给桌面浏览器的 CSS 正确布局。明显的解决方案是使 viewport 变宽一些。聪明的人们想到了一个解决办法：把 viewport 分成两部分：visual viewport 和 layout viewport。

    两个 viewport 都是以 CSS 像素度量的。但是当进行缩放（如果你放大，屏幕上的 CSS 像素会变少）的时候，visual viewport 的尺寸会发生变化，layout viewport 的尺寸仍然跟之前的一样。

    George Cummins 在 Stack Overflow 上对基本概念给出了最佳解释：

    把 layout viewport 想像成为一张不会变更大小或者形状的大图。现在想像你有一个小一些的框架，你通过它来看这张大图。（译者：可以理解为「管中窥豹」）这个小框架的周围被不透明的材料所环绕，这掩盖了你所有的视线，只留这张大图的一部分给你。你通过这个框架所能看到的大图的部分就是 visual viewport。当你保持框架（缩小）来看整个图片的时候，你可以不用管大图，或者你可以靠近一些（放大）只看局部。你也可以改变框架的方向，但是大图（layout
    viewport）的大小和形状永远不会变。

    我们工作中所谓的 CSS 布局，尤其是百分比宽度，是以 layout viewport 做为参照系来计算的，它被认为要比 visual viewport 宽。即：<html> 元素在初始情况下用的是 layout viewport 的宽度，这使得站点布局的行为与其在桌面浏览器上的一样。

    layout viewport 有多宽？每个浏览器都不一样。Safari iPhone 为 980px，Opera 为 850px，Android WebKit 800px，最后 IE 为 974px。

### 如何获取两个 viewport 的宽度？如果理解了上面讲述的内容，不难理解下面获取 viewport 的方式：

1.  layout viewport: document.documentElement.clientWidth/document.documentElement.clientHeight

2.  visual viewport: window.innerWidth/window.innerHeight

### canvas [适配retina](http://www.dengzhr.com/frontend/html/1050)