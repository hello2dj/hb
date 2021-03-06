---
title: software啊！你的命名为何这么难
date: 2018-01-01 12:25:35
tag:
- software
- naming
---
### 软件是如此的复杂之命名(我们只谈语义问题不谈逻辑问题)

<!-- more -->

让机器懂的代码很简单，让人懂的代码就很难了。 其实代码也是在写作（虽然也大不相同），我现在发现能将自然科学和人文科学完美结合的就是计算机科学了。因为一段正确的代码需要正确的算法逻辑甚至是数学证明，事实上计算机科学发展的基础就是数。可是代码追求的不仅仅是正确，还得是可维护的，就是说是人可以读懂的。那么这时候就要看你的英语功底了，其实也不尽然，但是良好英语功底得是有的，只要你想写出大家可以看懂的代码（此处省略一万字别较真...）。
关于代码写作，大部分不是在写完整的句子，反而大部分是在写短语，是在给事物进行命名处理。而是否写的好，这里有两方面的问题：

1. 大家的英语都是very good, 但这里有个问题, 就是这段代码里的命名是否真正的体现了代码的核心含义就很重要了。举个简单栗子, 在一个业务场景中有这样一个问题 ‘If your customer just considers “order” an “order” that has been approved, don’t call “order” to a non approved one in your code, call it “nonApprovedOrder”’ 翻译一下就是你的客户只会‘订购’那些被批准了的‘订单’，那么你在一段代码里出现了order这个变量，此时就会很模糊了，模糊的关键就在于order到底是被批准了，还是未批准呢？可能你会说order对象有个属性来表示是否批准了。当然这是可以的，但若是我直接使用nonApprovedOrder是否会更清晰呢！这里只是举个简单例子，也并不代表一定得这样，其实order下的属性判断我觉得也是可以的，这里只是为了说明一个问题，能让人一目了然的命名才是更合适的。
2. 对于非英语母语的半吊子英语的人来说，用对词语就很关键了。但对像我这种半吊子的人来说这里除了用对以外还有一个不可谓不重要的陷阱，那就是关于大家所学英语的共识。比如我知道一个单词a,并且我认为他的意思就是a,可是若是对另一个人来说他所认识的a代表的是b，那此时对于这个变量a,双发就会产生歧义了，甚至会惹得其他人说这是什么意思，怎么可以这么用。我举这个栗子就是要说明，一个公司最好有自己的命名规范甚至是自己的业务词语汇总。能减少不必要的麻烦。

看了上面两个问题就会发现，你写了一个正确代码，但却不一定写得出可读的代码。接下给大家看一些关于命名原则

1. 变量的名字尽可能的具有具体的意义，而不是过于泛化

好的栗子：daysDateRange, flightNumber, carColor
坏的栗子：days,dRange,temp,data, aux
> 关于这个原则，是视具体环境而变化的，比如data这个命名，有的时候我需要的就是data啊，越往底层走就越会出现像data这种抽象的命名，越往业务逻辑层走就越会出现像carColor这种具体的命名

2. 尽量短一些（毕竟我们不是在写以字数多就是好的文章）

坏的栗子：howLonDoesItTakeToOpenTheDoor, howBigIsTheMaterial...
好的栗子：timeToOpenTheDoor, MaterialSize

3. 用缩写没问题，关键得有注释，或者是大家都知道的通用缩写

4. 在需要的时候适当使用[匈牙利标记法](https://zh.wikipedia.org/wiki/%E5%8C%88%E7%89%99%E5%88%A9%E5%91%BD%E5%90%8D%E6%B3%95)。
举个栗子： 你需要根据苹果的颜色来区分是否成熟 你可这么来命名
red_apples, yellow_apples ...
> 这条待定

5. 保持一致性
如果采用驼峰式就都使用驼峰式

6. 把业务领域的术语对应到你所熟知的命名上去
栗子： If your customer just considers “order” an “order” that has been approved, don’t call “order” to a non approved one in your code, call it “nonApprovedOrder” 同上

7. 黄金条例： 花一些时间再命名上（我觉得这条是最靠谱的，我也要贯彻执行）
当你近乎不加思索的使用一个变量命名的时候，你基本上就已经选择了一个不好的命名。
反之，你总是在向着好的命名前进

### 那怎么判断我们的命名是合适呢？当我们的命名ok时代码应当呈现以下几个特征
* easy to read
* easy to use
* it looks simple
* lean
* is straight forward
但这并不代表我们有了好的命名就会写出可读的代码，好的命名只是我们写出可读的代码的第一步，但若是第一步都迈不出去，那可就惨了。。。

### 当然一次性就改好了是在太困难了，那我们以前的旧代码怎么办？
一句话对我们的代码持续性重构，以及持续性的测试。因此我们需要大量的测试用例，是的，测试用例。

### 最后一个真的是太重要了。

参考：
[blog](http://www.makinggoodsoftware.com/)