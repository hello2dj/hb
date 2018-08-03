---
title: Rxjs之不要到处都是取消订阅
tag:
- rxjs
- reactive programming
- Ben Lesh(原文作者)
---

我又要先上图了：
![](https://dab1nmslvvntp.cloudfront.net/wp-content/uploads/2016/02/1455228348streams.png)

[原文地址(english, 需翻墙)](https://medium.com/@benlesh/rxjs-dont-unsubscribe-6753ed4fda87)

<!-- more -->

### Rxjs: 不要 取消订阅！
好吧，我说的不要调用太多的取消的订阅

在我们的使用过程中我们可能会有很多的Observables, 那么这多的Observables该如何管理呢，有的人就会管理所有的subscription,然后一个一个的取消订阅，这样是不优雅的。当你管理了太多的subscriptions时那可能意味着你是必须要管理他们，而不是在充分利用Rx的强大。

### 我们来看一个不太好的管理方式
```
class MyGenericComponent extends SomeFrameworkComponent {
 updateData(data) {
  // do something framework-specific to update your component here.
 }

 onMount() {
  this.dataSub = this.getData()
   .subscribe(data => this.updateData(data));

  const cancelBtn = this.element.querySelector(‘.cancel-button’);
  const rangeSelector = this.element.querySelector(‘.rangeSelector’);

  this.cancelSub = Observable.fromEvent(cancelBtn, ‘click’)
   .subscribe(() => {
    this.dataSub.unsubscribe();
   });

  this.rangeSub = Observable.fromEvent(rangeSelector, ‘change’)
   .map(e => e.target.value)
   .subscribe((value) => {
    if (+value > 500) {
      this.dataSub.unsubscribe();
    }
   });
 }

 onUnmount() {
  this.dataSub.unsubscribe();
  this.cancelSub.unsubscribe();
  this.rangeSub.unsubscribe();
 }
}
```
我们从dataSub订阅数据直到发生了button click或者是range范围变化为止。就意味着当这两想发生时我们就会停止数据的订阅。我们可以看到上面管理了3个subscripitions，很复杂。
但其实我们可以把这些subscripition组合到一个subscripition里面来管理。
### 使用takeUntil来管理你的subscripition
```
class MyGenericComponent extends SomeFrameworkComponent {
 updateData(data) {
  // do something framework-specific to update your component here.
 }

 onMount() {
   const data$ = this.getData();
   const cancelBtn = this.element.querySelector(‘.cancel-button’);
   const rangeSelector = this.element.querySelector(‘.rangeSelector’);
   const cancel$ = Observable.fromEvent(cancelBtn, 'click');
   const range$ = Observable.fromEvent(rangeSelector, 'change').map(e => e.target.value);
   
   const stop$ = Observable.merge(cancel$, range$.filter(x => x > 500))
   this.subscription = data$.takeUntil(stop$).subscribe(data => this.updateData(data));
 }

 onUnmount() {
  this.subscription.unsubscribe();
 }
}
```
可以看到我们使用一个stop$就管理了所有的subscripitions。
优点：
1. 很明显我们减少了代码量
2. 这种方式给了我们一个可以complete observable的方法，当takeUntil条件满足后是会触发complete的，而单纯的调用unsubcribe只是不再订阅了，而source并不一定就结束了，这样就不会触发complete的
3. 最后一个就是你把所有的东西都聚合在了一起，因为上述代码我们可以看到只有一个subscribe和一个unsubscribe, 而Observable，只有在调用了subscribe后才开始执行，这意味你把中心点都给聚合了，代码逻辑的开始和结束被归一了，代码会很清晰。

### 其他的操作符
* take(n)
* takeWhile(predicate): 只有predicate返回true,才会触发数据，当是false时就结束了
* first()
* first(predicate): 过滤所有的数据只有第一个满足predicate的数据会被触发，然后就结束了。

### 总结：使用takeUntil和takeWhile或者其他的
当你看到多个subscripitions的时候就应当尝试使用takeUntil来管理它们。
* 更多的可组合性
* 当你结束一个stream的时候会触发complete
* 更少的代码
* 更少需要管理的东西
* 更少的subscribe调用
