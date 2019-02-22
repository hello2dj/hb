---
title: 说说到底啥是reactive programming
tag:
- rxjs
- cyclejs
- reactive programming
---

我又要先上图了：
![](http://img.blog.csdn.net/20160311112349604)



<!-- more -->

# 响应式编程
### 先来看看rxjs介绍的reactive programming
|  | Producer | Consumer |
| ------| ------ | ------ |
| Pull | Passive: produces data when requested. | Active: decides when data is requested.
 |
| Push | Active: produces data at its own pace. | Passive: reacts to received data. |
我们可以看到，在push系统中，consumer要做的就是要决定对接受到的数据做出如何的响应。而在pull系统中consumer要做的是决定什么时候去获取数据。可能ractive programming 就是从消费者的角度来定义的吧。消费者只需要对数据做出响应即可。

显然在pull的系统中，我们需要确定什么时候获取到数据，producer端是被动的，就好比前端和后端，后端就是被动。
那么push系统呢？是有producer来决定什么时候产生数据给Consumer的，而consumer是不关心什么时候拿到数据（典型的订阅模式啊）。
其实前端后端是pull，但我们也在经常性的使用push，那就是promise，当你调用promise后你是不知道什么时候才会拿得到数据的。可是promise是单值的push系统。
而rxjs带来了一个新的push系统，多值可取消的push系统（但rxjs不像promise那样全是异步的，rxjs可异步可同步）。

> push 也有push的弊端，那就是backpressure问题

### 我们再来看看cyclejs里的介绍
![](https://cycle.js.org/img/passive-foo-bar.svg)
```
// Inside module Foo
function onNetworkRequest() {
  // ...
  // 此时Foo玩去可以好好
  CCTV.incrementCounter();
  // ...
}
```
![](https://cycle.js.org/img/passive-foo-bar.svg)
此时箭头的生命是由箭尾决定的

是由Foo来控制什么时候调用Bar,此时控制权在Foo,我们需要向Foo提供对外响应, Bar的内部状态是由外部来修改的

![](https://cycle.js.org/img/reactive-foo-bar.svg)
```
Foo.addOnNetworkRequestListener(() => { // 事件一来我大CCTV就可以主动控制了， Foo也不知道
// 我大CCTV
  self.incrementCounter(); // self is CCTV
});
```
可事实是Bar的状态完全可以是一个内部状态，而这个内部状态只需要根据外部事件的来决定做出如何的响应(reactive 的Bar)。

换句话说就是Bar的内部状态是要随着外部状态的改变而做出响应，但是什么响应就是Bar内部状态的实现了，若是按照第一种方式来看，我们就把Bar对事件的实现暴露给了Foo（举个栗子好比是新闻联播，他要去记录老外那里发生了啥事儿，但我们的新闻联播的播放室会告诉老外我们要怎么播放，要播放什么了么？这就是典型响应式啊，老外发生了事情，cctv收到了事件，然后播放给国内人民）,当我们这么做了以后，Bar只需要关心自己的实现就好了，完全对自己负责就可以了

### 响应式一个最大的卖点就是构建对自我负责的模块就好了，而不是去或者不需要去改变外部或者外来的状态。
另一个好处就是关注点分离，各自关心自己的该关心的事情就好了。

### react的stateful Component也在做这件事(自扫门前雪)，他的Component,就是一个纯粹的自负责组件,即使是传递props
也是，因为props是提前定义的也好比是监听机制只不过是没有起一个on....Listener罢了

### 响应式编程依赖了那些编程原则呢？
1. 迪米特原则：最小消息原则，知道的人越少越好
2. 依赖反转原则
3. 单一职责原则

### 原则综合
1. 单一责任原则：尽量保证一个类只会因为一个原因发生变化，当变化多于一个时，就需要分解这个类。否则将会因为内部存在过多的依赖而变得难以维护。

2. 开放封闭原则：这个是我们最常使用的，具体可以体现在属性私有、方法公开这一点上。开放封闭原则讲究拥抱扩展、封闭修改。

3. 里氏替换原则：保证每一个子类都能够直接替换其父类，满足is-A的关系。

4. 依赖倒置原则：高层次的模块不应该依赖于低层次的模块，二者应该依赖于抽象。抽象不应该依赖于细节，细节应该依赖于抽象。

5. 接口分离原则：当一个用户需要多个接口的时候，尽量将每一个接口分离出来，而不是将多个接口放在一个类中，包含所有的接口

6. 迪米特法则：如果两个类之间并不需要直接通信，那么就不应该让这两个类相互作用。如果其中一个类需要调用另一个类的某一个方法的话，可以通过另外的类来转发调用，降低类与类之间的耦合。

### 再一次来说说Rxjs, 为什么要用他，他其实总结了出了很多我们在处理问题时的抽象，好比map, each, flatMap,感觉像是lodash了，事实是我们使用lodash的链式调用也可处理很多问题并且使用更少的代码以及更加简洁（我们可以filter.some.map.reduce,语义也更加清晰）而不是更多的for循环

参考：
1. [The introduction to Reactive Programming you've been missing](https://github.com/hehonghui/android-tech-frontier/tree/master/androidweekly/%E9%82%A3%E4%BA%9B%E5%B9%B4%E6%88%91%E4%BB%AC%E9%94%99%E8%BF%87%E7%9A%84%E5%93%8D%E5%BA%94%E5%BC%8F%E7%BC%96%E7%A8%8B)
2. [vedio](https://egghead.io/lessons/rxjs-understand-reactive-programming-using-rxjs)
3. [rxjs](http://reactivex.io/rxjs/manual/overview.html#pull-versus-push)
4. [响应式宣言](https://www.reactivemanifesto.org/zh-CN)
> 从响应式宣言中我们可以看到，不仅编程是可以响应式的，整个系统都应该是响应式的。