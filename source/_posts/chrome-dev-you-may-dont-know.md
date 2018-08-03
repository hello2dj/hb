---
title: chrome-dev-you-may-dont-know
date: 2018-056 20:24:58
tags: 
    - js
    - chrome devtools
---

## chrome devtools 中的一些实用技巧

<!-- more -->

### [在元素面板中拖拽元素](#在元素面板中拖拽元素-1)

### [在控制台中引用元素面板中选中的元素](#控制台中引用元素面板中选中的元素)

### [在控制台中使用上次操作的结果](#在控制台中使用上次操作的结果-1)

### [修改元素的状态以及添加 css](#修改元素的状态以及添加-css-1)

### [查找 css 属性定义的位置](#查找-css-属性定义的位置-1)

### [将修改的 css 样式保存到文件](#将修改的-css-样式保存到文件-1)

### [对单个元素的截图](#对单个元素的截图-1)

### [在控制台中使用 shift-enter(连续多行输入执行)](#在控制台中使用-shift-enter-连续多行输入执行-1)

### [清理控制台](#清理控制台-1)

### [跳转...](#跳转...-1)

### [监听表达式](#监听表达式-1)

### [XHR/FETCH 调试](#XHR-FETCH-调试-1)

### [DOM 修改的调试](#DOM-修改的调试-1)

-----------------------------------------------(华丽的分割线)

#### 在元素面板中拖拽元素

在元素面板中我们是可以拖拽任意元素到页面中其他位置的(这个大家应该都知道。。。)

![](https://d33wubrfki0l68.cloudfront.net/1f404b89ca124b8a7c11d2552f893189aa6ef9a8/ee86f/chrome-devtools-tips/drag-and-drop.gif)

#### 控制台中引用元素面板中选中的元素

在元素面板中选中元素，然后就可以在控制台中使用 $0 引用它，神奇了

![](https://d33wubrfki0l68.cloudfront.net/6aad4fd3fcf776853af0f751467abb4ca3e36036/cfa4b/chrome-devtools-tips/reference-elements.gif)

> 大吃一惊还有这等操作(管用)

#### 在控制台中使用上次操作的结果

在控制台中输入 $\_ 引用上次操作结果

![](https://d33wubrfki0l68.cloudfront.net/3145f6a4bfc316b1217a2559dceeef253f09f76a/ad0d6/chrome-devtools-tips/use-last-result.gif)

> 他要$?就更神奇了（管用）

#### 修改元素的状态以及添加 css

这个大家都知道，直接截图就好了

![](https://d33wubrfki0l68.cloudfront.net/e373ea01bf1ef9a452670843aea2bfa47a1ab56d/1e648/chrome-devtools-tips/add-css.gif)

在看下一个

![](https://d33wubrfki0l68.cloudfront.net/fe7535cfa76aa552810a1c3f51b922702040615f/12d68/chrome-devtools-tips/element-state.png)

#### 查找 css 属性定义的位置

使用 cmd-click(ctrl-click on windows) 组合点击一个元素面板中的 css 属性就会跳转到 Source panel 中(连续两连点击才可以哦)
![](https://d33wubrfki0l68.cloudfront.net/2e004309ab05ae39cf1f22fbb2bf52b50a323d3a/7479c/chrome-devtools-tips/find-where-css-defined.gif)

> 不错不错（管用）

#### 将修改的 css 样式保存到文件

在元素面板右侧修改样式，然后使用上面方法跳 source panel 中，就会看到 css 源文件，右击文件名字，save as 保存到本地

![](https://d33wubrfki0l68.cloudfront.net/755e747fc0664ea8ad18147f12a76111f0cf37fd/b5a38/chrome-devtools-tips/save-modified-css.gif)

#### 对单个元素的截图

选中一个元素然后按组合键 cmd-shift-p(or crtl-shift-p on windows)打开命令菜单(command menu), 接着输入 screenshot 选择 Capture node screenshot

![](https://d33wubrfki0l68.cloudfront.net/58cfa50406939395e2959604c235ccb7ccab88ed/cef7d/chrome-devtools-tips/screenshot-node.gif)

> 有意思的功能, 可惜我试验失败了。。。（66.0.3359.139chrome 版本，大家可以试试），其实里面的 screenshot 还有另外两个 capture full siz screenshot(全截，我也失败了)， capture screenshot 接的是当文档展示的区域我成功了

#### 在控制台中使用 shift-enter 连续多行输入执行

多行输入执行，啥也不说了，有用（管用）

![](https://d33wubrfki0l68.cloudfront.net/c312ca53496081f552859fac2f18e1558bb90228/b5e1a/chrome-devtools-tips/multiple-lines-commands.gif)

> 每次换行使用 shit+enter 哦

#### 清理控制台

清理使用 ctrl-l or cmd-k(管用)

#### 跳转...

在 source panel 中

*   cmd-o(ctrl-o on windows) 展示当前页面加载的所有文件
*   cmd-shift-o(ctrl-shift-o in windows) 展示当前文件的符号表（属性，函数以及类）

*   ctrl-g 调到当前文件的指定行

![](https://d33wubrfki0l68.cloudfront.net/055919bd29f5e9ff0c5f551d01e36d1e06db0f14/e666b/chrome-devtools-tips/files-list.png)

> 亲测管用

#### 监听表达式

添加一个表达式到 debug session 中，不用手动计算了

![](https://d33wubrfki0l68.cloudfront.net/d08eb9dda2e3841be48d5ff2169bd745c65909b9/db264/chrome-devtools-tips/watch-expressions.gif)

> 管用， 在 source panel 中，若是没有 consle 面板，右键-> evaluate in console 即可

#### XHR/FETCH 调试

可以在 source panel 中的右侧看到 XHR/FETCH breakpoints 中添加断点，不知道请 ajax 啥时候发的，没问题，他可以帮你。。。

![](https://d33wubrfki0l68.cloudfront.net/3a70cac4f7fef4d7caa5ee0335573bbad49a1151/e6aae/chrome-devtools-tips/xhr-fetch-breakpoints.png)

> 并且我还在下面发现了 Event Listener Breakpoint,而它可以带来的断点位置是在是太多了，比如： 动画（animation）, Canvas, Clipboard, DOM Mutaion, keyboard, 哎呀太多了，大家可以亲自试一试

#### DOM 修改的调试

右键选中的元素，选择 break on 展开后会有 subtree modifications, 就是说如果有脚本修改了子元素，断点就会触发，还有 atrribute modification, 以及 node removal

![](https://d33wubrfki0l68.cloudfront.net/79a8a06d060fdf6dd28a3bc496f556449e758e99/a6ce1/chrome-devtools-tips/break-subtree-modifications.png)

#### $$ 相当于 document.querySelectorAll()

### 文章尾部福利赠送

cmd-shfit-p 打开的命令行里面的有用命令(还有更多的等待大家一起探索)

1.  show layers 查看当前页面的渲染情况，合成层([关于合成层看淘宝](http://taobaofed.org/blog/2016/04/25/performance-composite/))的状况，绘制的具体范围，等等检查性能一绝
2.  show perfomance monitor 顾名思义
3.  show frame per second(FPS) meter 顾名思义
4.  show paint flashing rectangles 展示当前页面重绘的区域
