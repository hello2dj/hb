---
title: Oberservable之“hot”和“cold”的区别
tag:
- rxjs
- reactive programming
- Ben Lesh(原文作者)
---

我又要先上图了：
![](https://dab1nmslvvntp.cloudfront.net/wp-content/uploads/2016/02/1455228348streams.png)

[原文地址(english, 需翻墙)](https://medium.com/@benlesh/hot-vs-cold-observables-f8094ed53339)

# TL;DR: 当你不想一次又一次的创建你的producer时，你需要一个HOT observable

### COLD 就是你的Observable创建了生产者（就是当你创建observable时创建producer）
```
// COLD
const cold = new Observable((observer) => {
  const producer = new Producer();
  // have observer listen to producer here
});
```

### HOT 就是你的observable 关闭你的生产者
```
// HOT
const producer = new Producer();
cosnt hot = new Observable((observer) => {
  // have observer listen to producer here
})
```

### 继续深入
我的上篇[文章](https://medium.com/@benlesh/learning-observable-by-building-observable-d5da57405d87)介绍了observables就是函数。那篇文章的目的是解开observerable的神秘，但是没有深入那个困扰大家的问题：HOT VS COLD

### Observables are just functions!
Observables 就是一个绑定observer和producer的函数。是的，那就是全部。他其实不需要创建producer,他们仅仅是建立observer对producer的监听，并且返回一个函数可以用来移除监听器。对obserable的调用就好像是在调用一个函数，并且传递一个observer

### 那么什么是producer？
生产者就是你的observable的数据来源。他可能是一个web socket, 或者是一个DOM event, 又或者是一个迭代器，甚至有可能是对一个数组的循环。总之，他可以是任何东西，可以用来获取数据并且传递给`observer.next(value)`。

### Cold observables: 生产者在**内部**创建
如果一个observable的producer是在subscribe时创建并且激活的那么他就是 ‘cold’。这意味着，如果observables是函数，那么生产者就是在调用这个函数是创建和激活的。

1. creates the producer
2. activates the producer
3. start listening to the producer
4. unicast

下面这个栗子就是‘cold’, 因为他对websocket的监听是在你subscribe observable时建立的：
```
const source = new Observable((observer) => {
  const socket = new WebSocket('ws://someurl');
  socket.addEventListener('message', (e) => observer.next(e));
  return () => socket.close();
});
```
因此任何subscribe source的对象，都会有自己的WebSocket的实例，并且当他unsubscribe的时候，他将会关闭那个socket。这意味着我们source仅仅只是unicast,因为这个生产者仅仅只能给一个监听者发送数据。[这里有一个简单的栗子jsbin](http://jsbin.com/wabuguy/1/edit?js,output)

### HOT observables: 生产者是在**外部**创建的
如果一个observable的生产者是在**订阅之外**(就是不是在订阅时产生的行为)创建或者是激活的那么他就是'hot'。
1. 共享对生产者的引用
2. 开始监听生产者
3. 多播(通常是)
如果我们把上面的栗子中对于WebSocket的创建挪到obserable的外面，那么他就是hot了
```
const socket = new WebSocket('ws://someurl');
const source = new Observable((observer) => {
  socket.addEventListener('message', (e) => observer.next(e));
});
```
现在任何订阅`source`的对象都共享一个相同的WebSocket实例。他将会高效的多播数据到所有的订阅者。但是这里还有个小问题：我们的obserable没有了对socket的取消逻辑。这意味着一旦发生了错误或者结束后，甚至是取消订阅，我们都无法关闭socket。所以我们真正想要的是让我们的'cold' observable 变成hot。这里有个例子就没有取消的逻辑[jsbin](Here is a JSBin showing this basic concept.)

### 我们为什么需要 ’hot‘ observable?
从第一个cold observable的列子我可以看到如果所有的observable都是cold是会有一些问题的。比如，你不止一次的订阅一个observable,但是他有可能每次都创建了一些稀缺资源，好比 web socket connection, 可事实是你并不想创建很多的web socket连接。事实上你可能很容易的对一个observable创建很多的订阅,并且是在你没意识的情况下。假设我们需要从web socket的订阅中过滤出奇数和偶数，于是最终我们可能写出如下的代码：
```
source.filter(x => x % 2 === 0)
  .subscribe(x => console.log('even', x));
source.filter(x => x % 2 ==== 1)
  .subscribe(x => console.log('odd', x));
```
### Rx 的subjects
在我们把'cold' observable 变得 ’hot‘之前，我们需要先介绍一个新的类型：Rx Subject。他有如下特性：
1. 他是一个obserable.他的结构类似一个observable, 并且用相同的操作符
2. 他是一个observer, 他是一个鸭子类型的observer。当他订阅一个observable是，他会像一个observer那样把数据传递给’next‘方法
3. 他是多播的。所有通过’subscribe‘犯法订阅的observers都会加入到一个内部的observers list
4. 当他结束的时候就是真的结束了（意味着结束后不可以在重用）。无论是发生了unsubscribe（这我试出来了见下代码）或者complete, 亦或者是发生了error之后，Subjects是不可以再重用的。（When it’s done, it’s done. Subjects cannot be reused after they’re unsubscribed, completed or errored.
）
```
// The death of a Subject
const subject = new Subject();
subject.subscribe(x => console.log(x));
subject.next(1);    // 1
subject.next(2);    // 2
subject.complete();
subject.next(3);    // silently ignored
subject.unsubscribe();
subject.next(4);    // Unhandled ObjectUnsubscribedError
```
5. 当你通过他自身传递数据时，他会进行#2，就是obser那一套。又若是你通过next传递数据，他又会表现出他observable的一面。

之所以称之为’subject‘是因为上述第三点。 在四人帮的设计模式里，’Subjects‘是一个拥有’addObserver‘方法的类。在这个例子里面，’addObserver‘方法是’subscribe‘，这里有一个[jsbin](http://jsbin.com/muziva/1/edit?js,output)的例子：
```
const { Subject } = Rx;

const subject = new Subject();

// you can subscribe to them like any other observable

subject.subscribe(x => console.log('one', x), err => console.error('one', err));
subject.subscribe(x => console.log('two', x), err => console.error('two', err));
subject.subscribe(x => console.log('three', x), err => console.error('three', err));


// and you can next values into subjects.
// NOTICE: each value is sent to *all* subscribers. This is the multicast nature of subjects.

subject.next(1);
subject.next(2);
subject.next(3);

// An error will also be sent to all subscribers
subject.error(new Error('bad'));


// NOTICE: once it's errored or completed, you can't send new values into it
try {
  subject.next(4); //throws ObjectUnsubscribedError
} catch (err) {
  console.error('oops', err);
}
```

### 使一个 ’COLD‘ Observable 'HOT'
当我们使用了Subject，我们可以使用一些函数式编程方法来使一个 ’cold‘ Observable 'hot':
```
function makeHot(cold) {
  const subject = new Subject();
  cold.subscribe(subject);
  return new Observable((observer) => subject.subscribe(observer));
}

// 整体流程就是：observer -> subject -> cold(依次订阅下一个observable)
```
我们的makeHot方法可以接收任何的cold observable并且可以通过创建一个共享的subject。这里有个例子[jsbin](http://jsbin.com/ketodu/1/edit?js,output)

但是我们仍然有一些小的问题，因为，我们仍然没有跟踪我们源observable的subscription(就是cancel函数)，因此当我们想要取消的时候，我们该怎么取消呢？我们可以通过添加引用计数来解决这个问题：
```
function makeHotRefCounted(cold) {
  const subject = new Subject();
  const mainSub = cold.subscribe(subject);
  let refs = 0;
  return new Observable((observer) => {
    refs++;
    let sub = subject.subscribe(observer);
    return () => {
      refs--;
      if (refs === 0) mainSub.unsubscribe();
      sub.unsubscribe();
    }
  });
}
```
这样我们就有了unsubscribe函数了。[jsbin](http://jsbin.com/lubata/1/edit?js,output)

### 在Rxjs中，使用’publish()‘或者’share()‘
你应当使用publish或者share而不是上面造的makeHot。 有很多种方法可以是cold变为hot, 并且在Rx中有很多种高效简洁的方法来实现。

在rxjs 5中，share操作符，可以是cold变成hot，以及使用引用计数的observable。并且这个observable还可以重试当他失败或者成功。因为当他错误，完成或者取消订阅以后，subjects就不可以重新使用了，于是`share()`操作符会重新回收死掉的subjects并且在生成一个新的subject,使得我们可以重新订阅。
这里有一个栗子[jsbin](http://jsbin.com/mexuma/1/edit?js,output)
经过尝试错误，取消订阅是可以的重新订阅的。
# The “Warm” Observable
Given everything stated above, one might be able to see how an Observable, being that it’s just a function, could actually be both “hot” and “cold”. Perhaps it observes two producers? One it creates and one it closes over? That’s probably bad juju, but there are rare cases where it might be necessary. A multiplexed web socket for example, must share a socket, but send its own subscription and filter out a data stream.

### 温暖的 observable自己看吧

### ’hot‘和’cold‘都是针对生产者来说的
当你是用shared 引用来关闭producer,那么他是hot。 如果你是在你的observable中创建生产者那么他是cold,若是两者都做，那么我猜他是’warm‘吧！

> hot Observable通常是多播的，但若是producer一次只提供一个监听器数据，此时再说他是多播的就会有些模糊了。