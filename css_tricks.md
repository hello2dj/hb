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

### 层叠水平和z-index
需要注意的是，诸位千万不要把层叠水平和CSS的z-index属性混为一谈。没错，某些情况下z-index确实可以影响层叠水平，但是，只限于定位元素以及flex盒子的孩子元素；而层叠水平所有的元素都存在

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

### 层叠上下文实践
##### 层叠上下文之间的顺序
> 在此注意：z-index只影响flex和传统定位元素

0. 新的层叠上下文比较很简单就是谁后谁大，但带有z-index的传统定位元素比较吊，可以使用z-index来提升或者降低，其他途径创建的新的层叠上下文相当于是传统定位元素的z-index:0的等级
1. z-index只影响传统定位元素创建的层叠上下文的层叠顺序
```
.a{
    position: absolute;
    z-index: 1;  // 起效
}
```
.b {
    opacity: 0.8;
    z-index: 2; // 无
}

2. 谁后谁大： 针对传统定位元素且z-index相等以及其他途径创建的层叠上下文
```
<div style="postion: absolute;">
</div>
<div style="opacity: 0.8">
</div>
```
如上opacity会覆盖在absolute之上(我们可以把其他途径创建的层叠上下文看做是和z-index=0的传统定位元素)， 但是若是如下
```
<div style="postion: absolute; z-index:1">
</div>
<div style="opacity: 0.8">
</div>
```
此时absolute就会在opacity之上，但是若是给opacity 加上z-index: 23，会毫无作用。

3. 当非兄弟元素元素(都是新层叠上下文)比较时
    1. 若两个非兄弟元素的兄弟祖元素是非新的层叠上下文时，直接比较即可
    ```
    <div class="a">
        <div class="a1" />
    </div>
    <div class="b" />
    ```
    比较a > a1 和 b时 a是非新层叠上下文这直接比较a1和b

    ```
    <div class="a">
        <div class="a1" />
    </div>
    <div class="b">
        <div class="b1" />
    </div>
    ```
    比较a1和b1时，若a,b 是非层叠上下文直接比较a1,b1
    2. 若两个非兄弟元素的兄弟祖元素是新的层叠上下文时, 直接比较租元素即可

层叠上下文的创建
1. 皇亲国戚派：页面根元素天生具有层叠上下文，称之为“根层叠上下文”。
2. 科考入选派：z-index值为数值的定位元素的传统层叠上下文。
3. 其他当官途径：其他CSS3属性。
> 参照层叠水平的图片，所讲的都是同一个层叠上下文内的层叠水平
1. 

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

clientX:跟 screenX 相比就是将参照点改成了浏览器内容区域的左上角，

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

### animation-fill-mode : none | forwards | backwards | both;
1. forwards: 当动画完成后，保持最后一个属性值（在最后一个关键帧中定义）
2. backwards: 在 animation-delay 所指定的一段时间内，在动画显示之前，应用开始属性值（在第一个关键帧中定义） backwards：在动画等待的那段时间内，元素的样式将设置为动画第一帧的样式著作权归作者所有。

3. both: 向前和向后填充模式都被应用

### 动画
https://www.w3cplus.com/css3/css-animation-fill-mode-property.html

animation-delay不仅可以延时动画，还可以抵消动画
比如2s的动画，你设置animation-delay: -1s; 那就会只剩下1秒的动画


### event高度
1. clientX/Y 相对于浏览器可视区域，可视区左上角是(0,0)
2. screenX/Y 相对于涉笔屏幕而言，左上角是(0,0)
3. pageX/Y 相对于整个网页就是就是正个文档的而言，整个网页的左上角是(0,0)
> client只是占用了screen的一部分，因为浏览器的有上下边栏，缩小等等， 而client也只是显示了整个网页的一部分，因为网页会很宽，很长
4. offsetX/Y 相对于事件源

### [手机网站的几点注意](https://www.haorooms.com/post/phone_web)
1. 图片模糊，因为大家都在用devicePixelRatio > 1, 因此图片使用2倍的会好一些，background-size: contain
2. canvas 加载图片     https://stackoverflow.com/questions/10925047/benefit-to-use-canvas-instead-of-img
3. retina 1px
Element{
    border-width: thin;
}

### meta viewport 标签中的width 使用来设置layout viewport的
1. layout viewport 
    布局视口是用来布局网页的。
2. visual viewport
    视觉视口是设备屏幕
3. ideal viewport
    他是类似于布局视口，但是宽度和visual viewport相同, 完美的viewport就是这个，既可以布局又可以完美展示，不需要缩放

viewport meta 中的width即设置layout的跨度，设置为device-width就达到了ideal viewport

### clientTop是指边框， clientWidth= width + padding
### offsetWidth= width + padding + border

### offsetParent 是指元素的定位父元素，若没有就是body

### offsetLeft = offsetParent.margin-left + offsetParent.padding-left + offsetParent.border-left + 元素自身的margin-left

### scrollHeight 当有滚动轴时是 实际内容的高度 + padding, 没有的时候就是width + padding

### 浏览器可视区域高度
window.innerHeight || document.documentElement.clientHeight || document.body.clientHeight

整个文档内容高度 document.body.scrollHeight
整个文档上滑高度 document.body.scrollTop

### 元素进入可视区域
rect = element.getBoundingClientRect();

完全进入可视区域
0 <= rect.top <= 浏览器可视区域高度

rect.top是相对于浏览器可视区域的，当top<0 说明元素上滑超出了可视区域，当top>浏览器可视区域高度，说明元素下滑超出了可视区域


### 对于一个元素来说
1. element.style中的值都是内联设置的，若是内联没有设置则是取不到的
```
<div></div>
// div.style.height === undefined

<div style="height: 200px"></div>
// div.style.height === 200px
```

2. scrollHeight是获取元素内容的实际高度，是大于等于元素的高度，因为内容可能会很长
```
<div class="ab">
  <div class="b">
  </div>
</div>

.ab {
  height: 100px;
  width: 300px;
  overflow: auto;
  }

.b {
  height: 500px;
  background: green;
}
```
$('.ab').height === 100
$('.ab').scrollHeight === 500
$('.ab').clientHeight === 100

3. clientHeight 通常指padding + 元素高度(而不是内容高度)
4. offsetHeight 通常指border + padding + 元素高度(而不是内容高度)
5. offsetWidth 当有滚动轴的时候是会包含滚动轴的宽度
> 因此可以使用el.offsetWidth - el.clientWidth 来获取滚动轴的宽度

### mac下滚动轴不占宽度和高度

### -webkit-font-smoothing属性 字体锯齿问题

### 如何修改chrome记住密码后自动填充表单的黄色背景？
```
input:-webkit-autofill {
background-color: #FAFFBD;
background-image: none;
color: #000;
}
```

### input [type=search] 搜索框右侧小图标如何美化？ 
input[type="search"]::-webkit-search-cancel-button{ -webkit-appearance: none;height: 15px; width: 15px;border-radius: 8px;background:url("images/searchicon.png") no-repeat 0 0;background-size: 15px 15px;}

### 实现点击文字，文字后面radio选中效果
label 的for属性后面跟着input的id,就可以点击label，来触发input效果了，大家可以试一试！
```
<form>  <label for="male">Male</label>
 <input type="radio" name="sex" id="male" />
 <br />  <label for="female">Female</label>
 <input type="radio" name="sex" id="female" />
</form>
```

### 3.合法么
```
3.toString()
3..toString()
3...toString()
// error, '3', error
```
因为在 js 中 1.1, 1., .1 都是合法的数字. 那么在解析 3.toString 的时候这个 . 到底是属于这个数字还是函数调用呢? 只能是数字, 因为3.合法啊!

> 都来自https://www.haorooms.com/post/qd_ghfx

### [层叠上下文另一篇](http://www.cnblogs.com/pssp/p/5948356.html)

### [flex guide](https://css-tricks.com/snippets/css/a-guide-to-flexbox/)

### flex 有个order属性，正值，flex会按照order大小进行布局，而非是给定的顺序
### flex nowrap 会按照父元素的宽度平均分给flex-item，flex-item可以给定宽度，但是宽度最大不能超过 pw/sn(parent width / son number)
### flex的子元素可以赋值高度，若不赋予高度则是flex-container的高度（及时是inline元素也一样）

https://marina-ferreira.github.io/projects/js/memory-game/

### fixed布局带有input的坑(http://efe.baidu.com/blog/mobile-fixed-layout/)

### Element.scrollTop 属性可以获取或设置一个元素的内容垂直滚动的像素数。

一个元素的 scrollTop 值是这个元素的顶部到它的最顶部可见内容（的顶部）的距离的度量。当一个元素的内容没有产生垂直方向的滚动条，那么它的 scrollTop 值为0。

### https://www.w3.org/TR/css-flexbox-1/#item-margins flex items没有margin-collapse

### http://eux.baidu.com/blog/fe/link-preload-%E6%A0%87%E7%AD%BE preload