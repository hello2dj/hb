### 生成BFC
* 根元素

* float属性不为none

* position为absolute或fixed

* display为inline-block, table-cell, table-caption, flex, inline-flex

* overflow不为visible
### BFC 布局规则
* 内部的Box会在垂直方向，一个接一个地放置。

* Box垂直方向的距离由margin决定。属于同一个BFC的两个相邻Box的margin会发生重叠

* 每个元素的左外边缘（margin-left)， 与包含块的左边（contain box left）相接触(对于从左往右的格式化，否则相反)。即使存在浮动也是如此。除非这个元素自己形成了一个新的BFC。

* BFC的区域不会与float box重叠。

* BFC就是页面上的一个隔离的独立容器，容器里面的子元素不会影响到外面的元素。反之也如此。

* 计算BFC的高度时，浮动元素也参与计算

### IFC 布局规则
* 框会从包含块的顶部开始，一个接一个地水平摆放。
* 摆放这些框的时候，它们在水平方向上的外边距、边框、内边距所占用的空间都会被考虑在内。在垂直方向上，这些框可能会以不同形式来对齐：它们可能会把底部或顶部对齐，也可能把其内部的文本基线对齐。能把在一行上的框都完全包含进去的一个矩形区域，被称为该行的行框。水平的margin、padding、border有效，垂直无效。不能指定宽高。
* 行框的宽度是由包含块和存在的浮动来决定。行框的高度由行高计算这一章所描述的规则来决定。

### IFC 结论
* 一个line box总是足够高对于包含在它内的所有盒子。然后，它也许比包含在它内最高的盒子高。(比如，盒子对齐导致基线提高了)。
* 当盒子B的高度比包含它的line box的高度低，在line box内的B的垂值对齐线通过’vertical align’属性决定。当几个行内级盒子在一个单独的line box内不能很好的水平放置，则他们被分配成了2个或者更多的垂直重叠的line boxs.因此,一个段落是很多个line boxs的垂直叠加。Line boxs被叠加没有垂直方向上的分离(特殊情况除外)，并且他们也不重叠。
* 通常，line box的左边缘挨着它的包含块的左边缘，右边缘挨着它的包含块的右边缘。然而，浮动盒子也许会在包含块边缘和line box边缘之间。因此，尽管line boxs在同样的行内格式上下文中通常都有相同的宽度(就是他的包含块的宽度)，但是水平方向上的空间因为浮动被减少了，它的宽度也会变得复杂。Line boxs在同样的行内格式上下文中通常在高度上是多样的(比如，一行也许包含了一个最高的图片然后其他的也可以仅仅只包含文字)
* 当在一行中行内级盒子的总宽度比包含他们的line box的宽度小，他们的在line box中的水平放置位置由’text align’属性决定。如果属性是’justify’，用户代理可能会拉伸空间和文字在inline boxs内。
* 当一个行内盒子超过了line box的宽度，则它被分割成几个盒子并且这些盒子被分配成几个横穿过的line boxs。如果一个行内盒子不能被分割。则行内盒子溢出line box。
* 当一个行内盒子被分割，分割发生则margins,borders,和padding便没有了视觉效果。
* 在同样的line box内的行内盒子也许会被分割成几个盒子因为双向的文字。Line boxs在行内格式上下文中档需要包含行内级内容时被创造。Line boxs包含没有文字，没有空格，没有带着margins,padding和borders，以及没有其他在流中的内容(比如图片，行内盒子和行内表格)，也不会以新起一行结尾。对于在他们内的任何盒子的位置都以他们决定并且必须将他们视作没有高度的line boxs。

### IFC的css

* font-size
* line-height
* height
* vertical-aligin

### block ele 块级元素
> a block-level box is also a block container box. A block container box either contains only block-level boxes or establishes an inline formatting context and thus contains only inline-level boxes. Not all block container boxes are block-level boxes: non-replaced inline blocks and non-replaced table cells are block containers but not block-level boxes. Block-level boxes that are also block containers are called block boxes.
解释一下 块级盒子也是一个块级包含块盒子， 他要么只包含块级盒子要么建立一个IFC并且只包含行级盒子。并不是所有的块级包含快盒子都是块级盒子，也有不可替换的inline-block或者其他的。
* 解释1 只包含块级元素或者是创建一个IFC
  1. 只包含块级元素,就是说若是一个块级盒子里面包含了一个块级元素，那么它里面就只有块级元素！你在骗我吧，你看我是可以这么写的？
  ```
  <div>
    <p>I love you</p>
    <span>真逗我不love你</span>
    那我来爱你吧
  <div>
  ```
  问：你看我这个div里不止有p,还有span还有text,你咋说？
  答：没错但是你看[规范](https://www.w3.org/TR/CSS2/visuren.html#block-level) 9.2.1.1 Anonymous block boxes
  就是说像你这种情况他是会生成一个匿名块级盒子的，那么你的代码就是这样的了
  ```
   <div>
    <p>I love you</p>
    <anonymous-block-boxes><span>真逗我不love你</span>
    那我来爱你吧</anonymous-block-boxes>
  <div>
  ```
  我觉得这也是合理的就像规范里说的，这种方式也易于实现，要不浏览器的实现不得费劲啊。
  这样规定就意味着，块级元素只包含块级元素或者是创建一个IFC
  IFC的栗子
  ```
  <div>i love you</div>
  ```
  但事实上，这里不止创建IFC还有一个这个Anonymous inline boxes，因为html里所有的东西都是被盒子包裹的文本也不例外，你没写盒子，并不代表就没有，其实是有匿名盒子生成的。

### tricks
* [两栏布局](https://segmentfault.com/a/1190000010698609)

* [分栏高度自动相等](http://www.zhangxinxu.com/wordpress/2010/03/%E7%BA%AFcss%E5%AE%9E%E7%8E%B0%E4%BE%A7%E8%BE%B9%E6%A0%8F%E5%88%86%E6%A0%8F%E9%AB%98%E5%BA%A6%E8%87%AA%E5%8A%A8%E7%9B%B8%E7%AD%89/)

* [水平垂直居中的方式](http://louiszhai.github.io/2016/03/12/css-center/)

* [层叠上下文](http://www.zhangxinxu.com/wordpress/2016/01/understand-css-stacking-context-order-z-index/) 背景<布局<内容

### 层叠准则： 务必牢记的层叠准则
下面这两个是层叠领域的黄金准则。当元素发生层叠的时候，其覆盖关系遵循下面2个准则：
* 谁大谁上：当具有明显的层叠水平标示的时候，如识别的z-indx值，在同一个层叠上下文领域，层叠水平值大的那一个覆盖小的那一个。通俗讲就是官大的压死官小的。
* 后来居上：当元素的层叠水平一致、层叠顺序相同的时候，在DOM流中处于后面的元素会覆盖前面的元素。

### 层叠上下文的特性
层叠上下文元素有如下特性：

1. 层叠上下文的层叠水平要比普通元素高（原因后面会说明）；
2. 层叠上下文可以阻断元素的混合模式（见此文第二部分说明）；
3. 层叠上下文可以嵌套，内部层叠上下文及其所有子元素均受制于外部的层叠上下文。
4. 每个层叠上下文和兄弟元素独立，也就是当进行层叠变化或渲染的时候，只需要考虑后代元素。
5. 每个层叠上下文是自成体系的，当元素发生层叠的时候，整个元素被认为是在父层叠上下文的层叠顺序中。
### 翻译成真实世界语言就是：

1. 当官的比老百姓更有机会面见圣上；
2. 领导下去考察，会被当地官员阻隔只看到繁荣看不到真实民情；
3. 一个家里，爸爸可以当官，孩子也是可以同时当官的。但是，孩子这个官要受爸爸控制。
4. 自己当官，兄弟不占光。有什么福利或者变故只会影响自己的孩子们。
5. 每个当官的都有属于自己的小团体，当家眷管家发生摩擦磕碰的时候（包括和其他官员的家眷管家），都是要优先看当官的也就是主子的脸色。

### 层叠上下文的创建
1. 根元素（很厉害的，本身是BFC,还是具有层叠上下文）
2. 定位元素
对于包含有position:relative/position:absolute的定位元素，以及FireFox/IE浏览器（不包括Chrome等webkit内核浏览器）（目前，也就是2016年初是这样）下含有position:fixed声明的定位元素，当其z-index值不是auto的时候，会创建层叠上下文。
典型的栗子，猜猜谁上谁下
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
3. CSS3与新时代的层叠上下文
  1. z-index值不为auto的flex项(父元素display:flex|inline-flex).
  注意，这里的规则有些负责复杂。要满足两个条件才能形成层叠上下文：条件1是父级需要是display:flex或者display:inline-flex水平，条件2是子元素的z-index不是auto，必须是数值。此时，这个子元素为层叠上下文元素，没错，注意了，是子元素，不是flex父级元素。
  2. 元素的opacity值不是1.
  3. 元素的transform值不是none.
  4. 元素mix-blend-mode值不是normal.
  5. 元素的filter值不是none.
  6. 元素的isolation值是isolate.
  7. will-change指定的属性值为上面任意一个。
  8. 元素的-webkit-overflow-scrolling设为touch.

  再具体的请参见原文

  ### 同时设置margin-left和margin-right [参见](https://segmentfault.com/a/1190000007184954)

  更正： 第二种情况中若是没有设置宽度也是和第一种情况一样，设置负值会增加相应方向的宽度。

  ### 内容的包裹性，如何让你的盒子的宽度自适应内容呢？
  1. 行内元素肯定是可以的可是我们没办法控制行内元素的高度等属性
  2. float, inline-block，其实还有position:absolute
  但是这真的可以么？也不是[参见](http://jsbin.com/mamigevolo/2/edit?html,css,output)
  为什么还那么宽呢？因为我们里面放了两个inline元素，而第二个太长有放不下，于是挪到下一行了，但他占的位置依然在。so...


  ### float的一些[规则](http://dev.dafan.info/detail/513780?p=)
  [另一个](https://zhanglun.github.io/2014/10/15/%E8%A7%84%E8%8C%83%E4%B9%8B%E8%B7%AF-%E9%87%8D%E6%96%B0%E8%AE%A4%E8%AF%86%E4%BA%86Float/)
  还有鑫空间的不贴了，一搜就有


  ### [宽度计算法则](http://f2ecouple.github.io/2014/03/16/css%E5%AE%BD%E5%BA%A6%E8%AE%A1%E7%AE%97%E7%AE%97%E6%B3%95/)