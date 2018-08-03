---
title: 欢乐的使用rxjs-promise-async-await
tag:
- rxjs
- reactive programming
- Ben Lesh(原文作者)
---
先上图：
![](https://dab1nmslvvntp.cloudfront.net/wp-content/uploads/2016/02/1455228348streams.png)

[原文地址(english, 需翻墙)](https://medium.com/@benlesh/rxjs-observable-interop-with-promises-and-async-await-bebb05306875)

<!-- more -->

### Rxjs observable和promise以及Async-Await的互相操作
无论何时我都会被问到一个问题那就是如何再使用rxjs的时候使用promise和async和await呢？或者什么时候不能混合使用？我也说过几个不要同时使用的栗子。rxjs从一开始就可以很好的和promise一起使用。希望这篇文章能够很好的阐述一下。

### 如果他接受Observable, 那么他就可以接受promise
举个栗子，假如你在使用switchMap, 那么你就可以像返回一个Observable一样返回一个Promise。就像下面这样 [jsbin](http://jsbin.com/xevozit/edit?js,console)：
```
// An observable that emits 10 multiples of 100 every 1 second
const source$ = Observable.interval(1000)
  .take(10)
  .map(x => x * 100);
/**
 * returns a promise that waits `ms` milliseconds and emits "done"
 */
function promiseDelay(ms) {
  return new Promise(resolve => {
    setTimeout(() => resolve('done'), ms);
  });
}

// using it in a switchMap
source$.switchMap(x => promiseDelay(x)) // works
  .subscribe(x => console.log(x)); 

source$.switchMap(promiseDelay) // just a little more terse
  .subscribe(x => console.log(x)); 

// or takeUntil
source$.takeUntil(doAsyncThing('hi')) // totally works
  .subscribe(x => console.log(x))

// or weird stuff you want to do like
Observable.of(promiseDelay(100), promiseDelay(10000)).mergeAll()
  .subscribe(x => console.log(x))
```
经验证确实工作的很好

### 使用defer函数可以让返回promise的函数可以重新执行化
如果你的函数返回一个promise你可以使用Observable.defer包裹他，就可以使得他在发生错误是可以进行重试[jsbin](http://jsbin.com/dogabof/edit?js,console)。
Observable.defer: Returns an observable sequence that invokes the specified factory function whenever a new observer subscribes.
```
function getErroringPromise() {
  console.log('getErroringPromise called');
  return Promise.reject(new Error('sad'));
}

Observable.defer(getErroringPromise)
  .retry(3)
  .subscribe(x => console.log);

// logs "getErroringPromise called" 4 times (once + 3 retries), then errors
```
经验证确实工作的很好

### 使用defer来封装async-await
defer是一个强有力的工具，你也可以用它来封装async-await函数
```
Observable.defer(async function() {
  const a = await promiseDelay(1000).then(() => 1);
  const b = a + await promiseDelay(1000).then(() => 2);
  return a + b + await promiseDelay(1000).then(() => 3);
})
.subscribe(x => console.log(x)) // logs 7
```
这个没有jsbin尝试失败了呃，因为不支持async-await,估计是我的姿势错了,但我在本地试了ok。

### 用forEach订阅一个Observable, 然后来创建使用async-await的函数来并发执行任务。
forEach 介绍：
{% asset_img forEach.png %}
```
const click$ = Observable.fromEvent(button, 'clicks');
/**
 * Waits for 10 clicks of the button
 * then posts a timestamp of the tenth click to an endpoint
 * using fetch
 */
async function doWork() {
  await click$.take(10)
    .forEach((_, i) => console.log(`click ${i + 1}`));
  return await fetch(
    'notify/tenclicks',
    { method: 'POST', body: Date.now() }
  );
}
```
经本地验证很ok

### 使用toPromise()和async/await来把最后一个订阅值返回为一个Promise
事实上toPromise比较怪异因为他并不是rxjs规范所定义的操作符，只是我们提供了而已。而且toPromise只会把最后一个值使用promise进行包装，那就意味着，若是Observable一直不触发complete那么这个promise就永远不会resolve。
> toPromise是一个反模式，只有在需要promise的时候才使用，不要乱用，比如await
```

const source$ = Observable.interval(1000).take(3); // 0, 1, 2
// waits 3 seconds, then logs "2".
// because the observable takes 3 seconds to complete, and 
// the interval emits incremented numbers starting at 0
async function test() {
  console.log(await source$.toPromise());
}
```
经本地验证很ok。

### 总结：Observable 和 Promise能很好的一起使用
基本上如果你的目的就是active programming那么就应当使用Rxjs.Observable。但为了符合人体工程学，我们还是提供了和Promise的互操作，谁让他这么流行呢！其实当我们在async/await中使用forEach的是后会带来更多的可能性。