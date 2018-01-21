---
title: node，setImmeidate, setTimeout, nextTick你真的了解么？
date: 2018-01-10 20:27:18
tag:
- node
- setTimeout
- setInterval
- setImmediate
- process.nextTick
---

![](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQ9leykiCi69dcz6PgtAdOLnqaQG934yIqpSitu_3YCoQ3eDxKpZQ)

### 先来温习一下event loop中的几个phase可参见我的上篇文章[libuv概览](https://hello2dj.github.io/2018/01/10/libuv%E6%A6%82%E8%A7%88/)
  * timers: 这个phase是来检查定时器是否到期的，并执行的
  * poll(I/O): 这个是用来监听fd的事件的，比如socket的可读，可写，文件的可读可等
  * check: 当事件循环被I/O阻塞结束之后立刻调用check handle的回调。本质上他是和prepare phase(在循环被I/O阻塞之前（right before the loop will block for I/O）prepare回调就会立即被调用) 相反的。

### 为什么会强调上述几个阶段呢？可以参见一下[node官网对于event loop的解释](https://nodejs.org/en/docs/guides/event-loop-timers-and-nexttick/)
1. node的定时器setTimeout,setInterval是在timers 阶段检查是否过期执行的
2. node的setImmediate 是在check阶段执行的
3. 其他的一些监听文件描述符而执行的cb是在poll(I/O)阶段执行的

### 其实我们可以总解一下js代码的执行时间有哪些？
1. event loop 开始之前的代码
2. event loop 中各个phase执行的代码
也就是说node的js代码会在两种情况下执行(纯属自己的看法)

### 接下来就是解析一下他们的执行顺序问题
1. setTimeout(n) VS setImmediate
> setImmediate 一定会在本轮event loop内的check 阶段执行，若是check阶段已经错过了，那就只能在下一轮的check阶段执行了。
参见[上篇文章]((https://hello2dj.github.io/2018/01/10/libuv%E6%A6%82%E8%A7%88/))，见下图
```
   ┌───────────────────────┐
┌─>│        timers         │
│  └──────────┬────────────┘
│  ┌──────────┴────────────┐
│  │     I/O callbacks     │
│  └──────────┬────────────┘
│  ┌──────────┴────────────┐
│  │     idle, prepare     │
│  └──────────┬────────────┘      ┌───────────────┐
│  ┌──────────┴────────────┐      │   incoming:   │
│  │         poll          │<─────┤  connections, │
│  └──────────┬────────────┘      │   data, etc.  │
│  ┌──────────┴────────────┐      └───────────────┘
│  │        check          │
│  └──────────┬────────────┘
│  ┌──────────┴────────────┐
└──┤    close callbacks    │
   └───────────────────────┘
```
可以看出来在同一次event loop内，timers阶段是在check阶段之前的,也就是说若是在同一个event loop内既有定时器到期又有setImmediate那肯定是setTimeout先执行。若不在一个event loop内那就不好说了，要具体情况具体分析了。
那么setTimeout(n) 和 setImmediate的执行顺序到底是啥？
  * 当setTimeout与setImmediate在同一时间执行且是在event loop之前执行的设置代码, 又且 n <= t(t是当执行event loop时setTimeout刚好过期的那个时间)
  此时就相当于 在第一次event loop内既有setTimeout 过期又有 setImmediate，显然 setTimeout先执行。
  ```
  // test.js
  setTimeout(() => {
    console.log('setTimeout');
  }, 1)
  setImmediate(() => {
    console.log('setImmediate');
  });

  // 假设我的t 是2，那么输出就是
  # setTimeout
  # setImmediate
  ```
  * 当setTimeout与setImmediate在同一时间执行且是在event loop之前执行的设置代码, 又且 n > t(t是当执行event loop时setTimeout刚好过期的那个时间)
  此时就相当于 在第一次event loop内只有 setImmediate，setTimeout的过期只有在接下来的event loop内被检查到了显然 setImmediate先执行。
  代码同上，把n 换成 大于2的时间。
  > t的抉择就完全取决于机器的性能了
  而且在此处还有一个问题就是node中setTimeout最小时间1毫秒，见源码如下
  ```
  //lib/internal/timers.js~line#34
  after *= 1; // coalesce to number or NaN
  if (!(after >= 1 && after <= TIMEOUT_MAX)) {
    if (after > TIMEOUT_MAX) {
      process.emitWarning(`${after} does not fit into` +
                          ' a 32-bit signed integer.' +
                          '\nTimeout duration was set to 1.',
                          'TimeoutOverflowWarning');
    }
    after = 1; // schedule on next tick, follows browser behavior
  }
  ```
  也就是说你设置0，最下也是1，那么就是说若你的机器性能很好，在1毫秒之前就开始执行event loop了，那么将会永远只看到setImmediate在setTimeout之前执行了。
  * 前面说的都是在event loop之前设置，那么在event loop的期间执行js代码时设置的呢（我们只说同时设置的执行顺序）？(参见我前面的关于js代码的执行时机解释)
    * timers阶段设置
    ```
    setTimeout(() => { // 1
      console.log('外层timeout');
      setTimeout(() => { // 2
        console.log('set timeout in timeout');
      });
      setImmediate(() => { // 3
        console.log('set immediate in timeout');
      });
    });
    ```
    1的回调是在timers阶段执行的，而setTimeout内部的定时器一定是不可能在本次event loop的timers阶段执行的(分析后面再说)，那就是说2的回调一定是在本次event loop之后的某次loop中的timers阶段执行的，可我们也要注意，本次loop的check阶段还没执行，因此就很明显了，3的回调会在本次loop的check阶段执行，因此得到总结**在timer阶段设置的setTimeout和setImmediate一定是setImmediate先执行**。
    * I/O callbacks, idle, prepare, poll, 同理这些阶段也一定是setImmediate先执行，我们最常处于的阶段可能就是poll阶段了，因为这是描述符事件回调的触发阶段，在这个阶段，本轮loop的timers阶段已经执行过了，所以setTimeout,一定是后面的loop timers阶段执行，而本轮loop的check阶段还没有执行，因此在这些阶段设置的setImmediate可以在本轮loop得到执行，所以**在timer阶段之后，check阶段之前设置的setTimeout和setImmediate一定是setImmediate先执行**。
    ```
    const fs = require('fs');

    fs.readFile('./test.js', () => {
      setTimeout(() => {
        console.log('set timeout in poll phase');
      });
      setImmediate(() => {
        console.log('set immediate in poll phase');
      });
    });
    ```
    * check阶段，在本轮设置的setTimeout和setImmediate执行顺序与在event loop执行之前设置的是一样的，执行顺序不定取决于设置的超时时间t(原因同在event loop执行之前设置的是一样，甚至可以说两者就是可以看做是等同的)。因为在这一阶段设置的setImmediate是不会在本轮check阶段执行的，同理setTimeout也是没办法在本轮执行的。

    总的可以用一个流程图来概括
    {% asset_img timerheimmediate.png timer & check phase %}

  ### 接下来 process.nextTick VS setImmediate
  事实上这两者是没有可比的
  1. process.nextTick的实现是基于v8 MicroTask(是在当前js call stack 中没有可执行代码才会执行的队列,低于js call stack 代码，但高于事件循环，浏览器中也是如此[可参见](https://jakearchibald.com/2015/tasks-microtasks-queues-and-schedules/))机制的。不属于event loop(v8 microtask是怎么和node结合起来的还没看明白失败啊)
  但是可以确定是nextTick走的是v8的mircotasks机制，且在当前js calk stack 结束后event loop 继续进行之前调用，也就是说要是写一个递归nextTick调用会把整个node阻塞掉
  ```
  setImmediate(() => {
    console.log('immediate');
  });
  function a() {
    process.nextTick(() => {
      console.log('set nextTick');
      a();
    });
  }
  a();
  ```
  可以试一下上面的代码, 你会发现这就是无限调用了。还有我们所熟知的promise也走的是v8 microtask机制(并且Promise使用时v8实现的promise)。那么同理，promise的then的回调和nextTick中的回调也是由v8 microtask机制来确定执行的，也是与event loop 无关的。当然这在promiseA+规范里面也是有相应描述的。为了测试我们可以执行以下代码
  ```
  const promise = Promise.resolve(234)

  setImmediate(() => {
    console.log('immediate');
  });

  testPromise();

  function testPromise() {
    promise = promise.then(() => {
      console.log('promise');
      testPromise();
    });
  }
  ```
  是的结果和执行pross.nextTick是一样的event loop被阻塞了。

  在源代码里我们也可以看到见下
  主要代码在/lib/internal/procss/next_tick.js(这里有太多的逻辑没搞明白只是知道了一个大概)

  {% asset_img tickQueue.png tick queue %}

  我们所创建的nextTick都是由这个全局的NextTickQueue来管理的，当我们执行nextTick,就push进去一个TickObject
  
  {% asset_img tickObject.png tick object %}

  执行nextTick的逻辑如下
  {% asset_img nextTick.png nextTick %}

  接下来就是触发nextTickQueue里面的tickObject的执行了
  {% asset_img tickCb.png tick queue handle %}

  在接下来就是设置_tickCallback（_tickDomainCallback是使用了Domain的版本）这个回调的执行时机了
  {% asset_img setUpNextTick.png setUpNextTick %}

  失败的地方就来了，我跟到C++代码里面后就完全没找到_tickCallback的具体执行时机的设置了，而且这里面也有太多的逻辑了，完全不知道是在干啥，还有待慢慢揭秘求高手。

  2. setImmediate是基于libuv的event loop的。

  到了这里我可以知道了nextTick一定是先执行的（同时设置）

### 总结一下
  1. 当同时设置nextTick, setImmediate, setTimeout时一定是nextTick先执行，nextTick不属于event loop属于v8的micro tasks
  2. setImmediate, setTimeout是属于event loop的，但是执行的阶段不同。
  3. nextTick的promise的回调执行是在event loop继续执行之前的，也就是说他们的调用是会阻塞event loop的。也就是说在使用nextTick和promise编写递归调用或者大循环时要小心阻塞event loop
  4. 在setImmediate, setTimeout设置再次设置自己时，一定不会再本次loop中执行的
  5. 在浏览器中我猜测setTimeout和promise, MicroTasks也是这样的

### 最后再来点儿，对于setTimeout和setImmediate的代码分析，来具体解释为什么4成立
  * setTimeout
  当我们执行setTimeout时，node会创建一个Timeout对象来存储
  {% asset_img timeout.png setTimeout %}

  Timeout具体属性见下

  {% asset_img timeoutobj.png Timeout %}

  其中_idleStart很重要，是指这个定时器的起始时间，比如在10秒的时候设置了一个40秒的定时器，那么到期的时候就检查这个now - _idleStart 是否大于定时的40秒，而这个时间应当是程序启动后经过的毫秒数。（纯属个人猜测）
  生成了定时器对象后，怎么组织管理就是个问题了，定时器在node中是以对象加链表来组织的，相同时间的定时器会被放到同一个链表中，如都是定时的40毫秒，但设置的时间不同，那么他们就会被放到同一个list中,见下图
  ```
  // ╔════ > Object Map
  // ║
  // ╠══
  // ║ refedLists: { '40': { }, '320': { etc } } (keys of millisecond duration)
  // ╚══          ┌─────────┘
  //              │
  // ╔══          │
  // ║ TimersList { _idleNext: { }, _idlePrev: (self), _timer: (TimerWrap) }
  // ║         ┌────────────────┘
  // ║    ╔══  │                              ^
  // ║    ║    { _idleNext: { },  _idlePrev: { }, _onTimeout: (callback) }
  // ║    ║      ┌───────────┘
  // ║    ║      │                                  ^
  // ║    ║      { _idleNext: { etc },  _idlePrev: { }, _onTimeout: (callback) }
  // ╠══  ╠══
  // ║    ║
  // ║    ╚════ >  Actual JavaScript timeouts
  // ║
  // ╚════ > Linked List
  ```
  插入的流程如下
  {% asset_img insert.png insert %}
  初始化TimerList的如下
  {% asset_img timerlist.png timer list %}
  在初始化一个TimersList时就会以他所属的过期的时间设置一个libuv的定时器，到期后处理自己这个list中node定时器，若是，还有未到期的，那么就继续设置libuv的定时器
  接下来就是定时器到期后怎么处理了
  {% asset_img timeouecb.png timer outdate %}
  上面我的截图里说的语句不通了激动了，从上面可以看到，我们新设置的相同的定时器尤其是针对setTimeout(0)(虽然我们不可能有零这种情况),其实我想说的是，假设我们设置了两个1msecs的定时器，见代码吧
  ```
  setTimeout(() => { // 1
    console.log(1);
    setTimeout(() => { // 2
      console.log(2);
    });
    setImmediate(() => { // 3
      console.log(3);
    })
  })
  ```
  上述代码1，2的过期时间都是1，所以他们在同一个timerList中，还有我们看到前面的描述，当我们是while去处理timerlist的，根据前面的讲述我们知道3一定比2先输出，但是我们是while处理timerlist的，为什么没有判断2过期呢，我就发现了，因为判断过期取的是now - timer._idleStart, 而这个now是在定时器cb执之前取的，而timer._idleStart是在setTimeout时设置的，那就意味着，2的_idleStart 一定比1的到期是去的now 大，那就很明显了，无论如何都是是无法判断2过期(在本次loop期间判断2过期)了，即使是下面的代码也不行
  ```
   setTimeout(() => { // 1
    console.log(1);
    setTimeout(() => { // 2
      console.log(2);
    });
    for(let i = 0; i < 100e100; i ++) { // 1msec的时间绝对有了，那么是不是在本轮loop就可以判断timer过期呢，不会
      let c = 0;
    }
    setImmediate(() => { // 3
      console.log(3);
    })
  })
  ```
  定时器的整体组织方式就是为了方便管理，减少底层真实定时器的使用。

  * setImmediate
  那么setImmediate内设置setImmediate呢？
  看代码吧，immediate是通过一个全局的list来管理的
  {% asset_img immediatelist.png immediate list %}

  调用setImmediate
  {% asset_img newimme.png Immediate %}

  Immediate构造函数的处理

  {% asset_img appendimme.png append Immediate %}

  加下来就是处理 check阶段处理immediate了

  {% asset_img processimme.png 才处理immediate list %}

  上图解释的很清楚了，这就告诉了我们另一个问题，在同一时期设置的setImmeidate会放到同一个队列，并且在一次loop check阶段就把所有的immediate回调给执行了。

  ```
  setTimeout(() => { // 1
    console.log(1);
    setTimeout(() => { // 2
      console.log(2);
    });

    for(let i = 0; i < 10e10; i++) {
      let c = 0;
    } // 上面的那个肯定过期了
    for(let i = 0; i < 100; i++>) {
      setImmediate(() => { // 3
        console.log(3);
      })
    }
  })
  ```
  上面的代码我们的2在1的回调执行之后一定是过期的，那么若是同一时间设置的setImmediate不会在同一个loop的check阶段那么，我们的2输出之后就一定会有3，可以执行一下是没有的，也就证明了我们上面的源码分析是正确的。

  ### 最终总结
  打完收工，源码阅读很考验啊，应当先知道代码的最终功能是什么？你想要知道的问题是什么？然后再去跟代码，一定要先找到函数入口，在就是一步一步调试是很有用的，等着看看怎么调试v8以及node的c++代码势要把microtasks也高明白了。


  ### 参考
  1. [task, micro tasks](https://jakearchibald.com/2015/tasks-microtasks-queues-and-schedules/)
  2. [node source code](https://github.com/nodejs/node)
  3. [event loop](https://github.com/creeperyang/blog/issues/26)