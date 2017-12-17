---
title: rxjs原理解析（自建一个demo版rxjs）
tag:
- rxjs
- reactive programming
- Ben Lesh(原文作者)
---

我又要先上图了：
![](https://dab1nmslvvntp.cloudfront.net/wp-content/uploads/2016/02/1455228348streams.png)

[原文地址(english, 需翻墙)](https://medium.com/@benlesh/learning-observable-by-building-observable-d5da57405d87)


# 通过构造一个Observable来学习Observable
很多时候大家都在问我"hot" 和 "cold" observables的区别到底是啥？，或者是一个observable到底是单播还是多播？。人们对于’Rx.Observable‘的内部工作原理似乎是非常迷惑的。当被问到如何描述一个observable的时候，人们经常说的就是这样的， “他是流（streams）”或者是“他是个类似promises的东西”。但事实上，我在很多场合以及一些公开演讲上都有讲过这些东西。

和promise作比较比较是有必要的，但不幸的是，恐怕不会有太大的用处。这两者都是异步原语，并且promises已经被js社区广泛接受和使用了，总体来说这是个好的开始。通过对比promise的‘then’和observable的’subscribe‘,我们可以看到两者在立即执行和延时执行上的区别，还可以看到observable的取消执行和可重用性，当然还有其他很多的东西。通过这种比较的方式学习对于observable的初学者来说是很容易接受的。但是这里有一个问题：就是这两者的不同之处远远大于类似之处。Promises都是多播的，Promise的resolve和reject都是异步的。当大家以处理promise的方式处理observables的时候，大家会发现有时候结果并不像预期的那样。Observables有时候是多播的，有时候又不是，并且通常是异步的。真的，有时候我也在责备自己，因为我有可能再使这种误解被延续。

# Observable仅仅是一个函数，他接受一个observer 并且返回一个函数
若果你想彻底搞懂observable，你可以自己实现一个简单的observable。真的，这并没有听起来那么难。对于一个observable, 当我们去观察他的最小实现时会发现他只是一个拥有特定（specific,具体，指定，特定）目的的函数，而这个函数又有自己特定的类型。（就是一个具有特定目的的特定类型的函数）
1. 结构
    * 函数
    * 接受一个observer(观察者): 一个拥有next, error 以及complete方法的对象
    * 返回一个可取消执行的函数
2. 目的：
    连接一个observer到生产者(产生value的对象)，并且返回一个能够
    取消连接生产者的方法。实际上observer就是一个可以随时传入数据的的监听器处理函数（handler处理函数）
3. 基础实现：
```
/**
 * 一个虚假的data source
*/
class DataSource {
    constructor() {
        let i = 0;
        this._id = setInterval(() => this.emit(i++), 200);
    }
    emit(n) {
        const limit = 10;
        if (this.ondata) {
            this.ondata(n);
        }
        if (n === limit) {
            if (this.oncomplete) {
                this.oncomplete();
            }
            this.destroy();
        }
    }
    
    destroy() {
        clearInterval(this._id);
    }
}

/**
 * 我们的 observable
*/

function MyObservable(observer) {
    const datasource = new DatSource();
    datasource.ondata = (e) => observer.next(e);
    datasource.onerror = (e) => observer.error(err);
    datasource.oncomplete = () => observer.complete();
    return () => {
        datasource.destroy();
    }
}

/**
 * 接下来我们可以使用上面的observable
*/
const unsub = myObservable({
    next(x) { console.log(x); },
    error(err) { console.error(err); },
    complete() { console.log('done'); } 
});
```

[你可以在jsbin上尝试一下](http://jsbin.com/yazedu/1/edit?js,console,output)

正如你看到的一样，他并不复杂，他只是一个简单的契约


# 安全的Observers: 优化我们的Observers
当我们谈论Rxjs或者响应式编程的时候，我们大部分时间把observables放在首位，但事实上observer的实现才是这类响应式编程的核心工作者(workhorse驮马驮东西的马)。Observables是惰性的（inert）他们仅仅是函数，他们就在那里不动一直到你’订阅‘他们，’订阅‘后他就会建立你的observer(就是把observer与producer连接在一起)，至此他们的活就干完了，然后就又变回了原始的状态等着被其他人再次调用, 另一方面observers则是保持在活跃状态，监听着producer的事件。

你可以用一个带有’next‘, 'error'以及’complete‘等方法的js 对象来订阅observable，但实际上这仅仅是个开始。在rxjs5我们提供了一些保证，下面是一些非常重要的保证：

# Observer 保证
1. 若果你传入的oberser没有实现所有的方法，这也是可以的
2. 你不需要在complete和error之后调用next
3. 当你取消订阅以后，任何事件都不会被触发（error, next, or complete）
4. 当调用’complete‘和’error‘的时候需要调用unsubsription
5. 当你的next， complete，error等handlers发生异常的时候，需要调用
    unsubscription来保证没有资源泄露
6. next，error 以及 complete都是可选的

为了达到以上目的，我们需要把你的observer包裹到一个SafeObserver中，这个SafeOberver会强制实现以上保证。为了实现2， 我们需要跟踪是否发生了complete 或者 error。为了实现3，我们需要让我们的SafeObserver知道消费者在什么时候调用了unsubscribe, 等等。

因此如果我们真的想要实现完整的SafeObserver,那将是很庞大的，因此在此文章中就不在具体详述，简要写一下怎么用。具体的实现可以看一下
[jsbin](http://jsbin.com/kezejiy/2/edit?js,console,output)(可惜我可以不在乎23333)
```
/**
 * A contrived data source to use in our "observable"
 * NOTE: this will clearly never error
 */

class DataSource {
  constructor() {
    let i = 0;
    this._id = setInterval(() => this.emit(i++), 200);
  }
  
  emit(n) {
    const limit = 10;
    if (this.ondata) {
      this.ondata(n);
    }
    if (n === limit) {
      if (this.oncomplete) {
        this.oncomplete();
      }
      this.destroy();
    }
  }
  
  destroy() {
    clearInterval(this._id);
  }
}

/**
 * Safe Observer
 */
class SafeObserver {
  constructor(destination) {
    this.destination = destination;
  }
  
  next(value) {
    // only try to next if you're subscribed have a handler
    if (!this.isUnsubscribed && this.destination.next) {
      try {
        this.destination.next(value);
      } catch (err) {
        // if the provided handler errors, teardown resources, then throw
        this.unsubscribe();
        throw err;
      }
    }
  }
  
  error(err) {
    // only try to emit error if you're subscribed and have a handler
    if (!this.isUnsubscribed && this.destination.error) {
      try {
        this.destination.error(err);
      } catch (e2) {
        // if the provided handler errors, teardown resources, then throw
        this.unsubscribe();
        throw e2;
      }
      this.unsubscribe();
    }
  }

  complete() {
    // only try to emit completion if you're subscribed and have a handler
    if (!this.isUnsubscribed && this.destination.complete) {
      try {
        this.destination.complete();
      } catch (err) {
        // if the provided handler errors, teardown resources, then throw
        this.unsubscribe();
        throw err;
      }
      this.unsubscribe();
    }
  }
  
  unsubscribe() {
    this.isUnsubscribed = true;
    if (this.unsub) {
      this.unsub();
    }
  }
}

/**
 * our observable
 */
function myObservable(observer) {
  const safeObserver = new SafeObserver(observer);
  const datasource = new DataSource();
  datasource.ondata = (e) => safeObserver.next(e);
  datasource.onerror = (err) => safeObserver.error(err);
  datasource.oncomplete = () => safeObserver.complete();

  safeObserver.unsub = () => {
    datasource.destroy();
  };

  return safeObserver.unsubscribe.bind(safeObserver);
}


/**
 * now let's use it
 */
const unsub = myObservable({
  next(x) { console.log(x); },
  error(err) { console.error(err); },
  complete() { console.log('done')}
});

/**
 * uncomment to try out unsubscription
 */
// setTimeout(unsub, 500);
```

# Observable的设计: 符合人体工程学的 Observer 安全性

若是我们把observables封装成一个class或者 一个对象，那么我们就可以很方便的把SafeObserver当做匿名的obserers传入（或者是函数就好像rxjs里的签名似的`subscribe(fn, fn, fn)`）并且以更好的符合人体工程学的方式提供给开发者。通过在Observable的’subscribe‘中把SafeObserver以内在的形式创建， Observables 又可以以一种简单的方式来使用了：
```
const myObservable = new Observable((observer) => {
    const datasource = new DataSource();
    datasource.ondata = (e) => observer.next(e);
    datasource.onerror = (err) => observer.error(err);
    datasource.oncomplete = () => observer.complete();
    return () => {
        datasource.destroy();
    };
});

```

你可能已经注意到了这个例子和我们的第一个例子是类似的。但是他更容易阅读和理解。具体实现可见[jsbin](http://jsbin.com/depeka/edit?js,console)但在jsbin里面我们可以看到在 new Observable的时候他把 observable又用safeObservable包裹了一下显然是没有必要的，因为我们在调用myObserable的subscribe的时候已经把observer用safeObservable包装过了
```
class Observable {
  constructor(_subscribe) {  // 我们在new Observable的时候传递的函数其实才是真正的
    // subscribe
    this._subscribe = _subscribe; // 保存起来当我们调用subscribe的时候回来调用他的
  }
  
  subscribe(observer) { // 你看包装过了
    const safeObserver = new SafeObserver(observer);
    return this._subscribe(safeObserver);
  }
}

```

# 操作符：也只是个函数而已
Rxjs中操作就是一个接收源observable，然后返回一个新的observable, 并且在你订阅他（指新的observable）的时候，他(操作符)会去订阅源observable。我们可以实现一个简单的如下：[jsbin](http://jsbin.com/xavaga/2/edit?js,console,output)
```
function map(source, project) {
    return new Observable(observer) => {
        const mapObserver = {
            next: (x) => observer.next(project(x)),
            error: (err) => observer.error(err),
            complete: () => observer.complete()
        };
        return source.subscribe(mapObserver);
    }
}
```
这里最重要的地方是这个操作符做了什么： 当你订阅他所返回的observable的时候，他创建了一个’mapObserver‘去执行工作，并且把'observer'和mapObserver连在了一起。构造操作符的链式调用仅仅是创建了一个模板，用于在订阅时把observes连接在一起。

# 设计Observable: 使操作符更好的链式调用
如果我们把所有的操作符都实现为独立的函数，那么我们的操作符链式调用会很丑陋
```
map(map(myObservable,(x) => x + 1), (x => x + 2)
```
那么我们可以想象一下如果我们来个5，6操作符，那个咋办？基本上时没法使用的了。

我们还可以使用reduce来简化一下具体实现参考[jsbin](http://jsbin.com/vipuqiq/6/edit?js,console,output)
```
pipe(myObservable, map(x => x + 1), map(x => x + 2));

```

理想情况下，我们希望能够使用如下的方式进行链式调用
```
myObservable.map(x => x + 1).map(x => x + 2);
```

幸运的是，我们已经把Observable包装成了一个class, 因此我们可以把操作符作为class的方法实现：[jsbin]()
```
/**
 * Observable basic implementation
 */
class Observable {
  constructor(_subscribe) {
    this._subscribe = _subscribe;
  }
  
  subscribe(observer) {
    const safeObserver = new SafeObserver(observer);
    safeObserver.unsub = this._subscribe(safeObserver);
    return safeObserver.unsubscribe.bind(safeObserver);
  }
}

// 在此看到了map的实现
Observable.prototype.map = function (project) {
  return new Observable((observer) => {
    const mapObserver = {
      next: (x) => observer.next(project(x)),
      error: (err) => observer.error(err),
      complete: () => observer.complete()
    };
    return this.subscribe(mapObserver);
  });
}

```
现在我们就得到了我们想要的语法了。这样做还有一个好处，就是我们子类化一些特定Observable(好比包裹Promise或者需要一些静态数值时)

# 总结：Observables就是一个接收observer作为参数并且返回一个函数的函数
牢记此话，Observables are a function that take an observer and return a function. 不多也不少。如果你写了一个函数接收一个observer然后返回一个函数，那么他是同步的还是异步的呢？都有可能，他是一个函数，任何函数的行为都取决与他是如何实现的。因此在处理Observable时，就把他看作是一个你传入的函数的引用, 没有什么魔法，stateful alien type(有状态的外部类型)。当你在使用操作符的链式调用的时候，你所做的其实就是组合一个函数，建立observers的连接，并将它们连接在一起，以及将数据传递给你的observer。

> 本文中Observable返回都是一个函数，而在Rxjs回哦在那个以及es-observable规范中返回都是Subscription对象，他有一个更好的设计。但在这里这么写保持了文章的简洁性。

### 接下来再贡献一点其他的
* Subject即是observer 又是observable
* Subject内部有存储observers的list, 因此他可以多播给这些observers
* Observables 就是函数用来建立生产者和消费者的监听的函数
* Observables 目前还没有跟踪错误， 可以使用error handler(都得用啊)或者是observeOn(这招经本地验证无效本地是node环境可能有所不同待研究)[jsbin](http://jsbin.com/dolutiz/edit?js,console)
上图
如下不管是mapped新产生的observable还是source都终止了。最安全的做法就是一定要监听error。
```
// This is going to behave strangely
const source$ = Observable.interval(1000).share();
const mapped$ = source$.map(x => {
  if (x === 1) {
    throw new Error('oops');
  }
  return x;
});
source$.subscribe(x => console.log('A', x));
mapped$.subscribe(x => console.log('B', x));
source$.subscribe(x => console.log('C', x));
// "A" 0
// "B" 0
// "C" 0
// "A" 1
// Uncaught Error: "oops"
```
另一个解决的办法就是使用observeOn
```
const source$ = Observable.interval(1000)
  .share()
  .observeOn(Rx.Scheduler.asap); // magic here
```
* 从subject下游抛出的同步错误会杀掉整个subject(尚未验证，不太明白说的是啥 // TODO: 验证 )
* 大神说他自己错了，Promise的错误处理才是个好主意。
* 将来的版本或许会支持error trap(但目前我是5.5.5了，也没支持)
有图有真相，最近的rxjs的一个[issue](https://github.com/Reactive-Extensions/RxJS/issues/1417)的讨论
{% asset_img error_trap.png %}
* 可能像promise那样全是异步的也不是必须的(shrug耸一下肩)。

### 文章评论区精彩内容
* multicast and unicast
单播还是多播取决于你是如何连接producer的，如果你是每次subscribe时就新建一个producer,那么就是单播，否则就是多播，但是这里面还有好多细节的，大致可以这么认为。
