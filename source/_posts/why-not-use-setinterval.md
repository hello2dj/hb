---
title: 为什么不要使用setInterval?
date: 2018-04-02
tag:
- js
---

<!-- more -->

> 最近我遇到了一个情况，就是需要在一个特定的时间间隔后调用一个函数。好比说每隔十秒就发送一个 ajax 请求。显然，最好的选项似乎是 setInterval, 但是我被分分钟搞得灰头土脸。

为了了解为甚么 setInterval 是邪恶的，那么我们得先搞清楚一个事实那就是 js 是单线程的，也就是同一时间只能执行一个操作。

如果我们的函数的执行时间都比咱么的延时时间还长（好比 ajax call）, 我们就会发现我们的 setInterval 就不会准时触发了，事实上 js 的定时器就是这么无奈，他的定时总是不准时。

```
const fakeCallToServer = () {
    setTimeou(() => {
        console.log('returning from server', new Date().toLocaleTimeString());
    }, 4000)
}

setInterval(function(){

    let insideSetInterval = new Date().toLocaleTimeString();

    console.log('insideSetInterval', insideSetInterval);

    fakeCallToServer();
}, 2000);

//insideSetInterval 14:13:47
//insideSetInterval 14:13:49
//insideSetInterval 14:13:51
//returning from server 14:13:51
//insideSetInterval 14:13:53
//returning from server 14:13:53
//insideSetInterval 14:13:55
//returning from server 14:13:55
```

> 尝试一下上面的代码

就像你看到的一样 setInterval 中的 console.log 一直在输出 ajax calls, 根本就不会理会之前的请求是否返回或是其他的情况。这样就会在后端 server 囤积大量的请求。

现在咱们再来试试同步操作：

```
var counter = 0;

var fakeTimeIntensiveOperation = function() {

    for(var i =0; i< 50000000; i++) {
        document.getElementById('random');
    }

    let insideTimeTakingFunction  = new Date().toLocaleTimeString();

    console.log('insideTimeTakingFunction', insideTimeTakingFunction);
}



var timer = setInterval(function(){

    let insideSetInterval = new Date().toLocaleTimeString();

    console.log('insideSetInterval', insideSetInterval);

    counter++;
    if(counter == 1){
        fakeTimeIntensiveOperation();
    }

    if (counter >= 5) {
       clearInterval(timer);
    }
}, 1000);

//insideSetInterval 13:50:53
//insideTimeTakingFunction 13:50:55
//insideSetInterval 13:50:55 <---- not called after 1s
//insideSetInterval 13:50:56
//insideSetInterval 13:50:57
//insideSetInterval 13:50:58
```

可以很明显的看到，当 setInterval 遇到了 cpu 敏感的操作时，setInterval 就完全乱了节奏。

### 结论

异步操作时，setInterval 有可能会堆积很多的异步操作，而同步操作则会打乱 setInterval 的节奏。并且当函数报错时 setInterval 也是不会停止的，但其实可以使用 clearInterval 在停止 setInterval。我们可以使用 setTimeout 递归调用来模拟 setInterval 这可以让我们更好的控制代码。
