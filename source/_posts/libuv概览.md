---
title: libuv设计概览
date: 2018-01-10 10:29:28
tag:
- node
- libuv
---

先上图

![](http://7xnyi3.dl1.z0.glb.clouddn.com/2017-07-10-libuv-libuv.png)

<!-- more -->

### 设计概览
libuv是一个跨平台的异步事件库。
libuv对于不同的I/O轮询机制提供了很多的抽象。例如：handle和stream是对socket和其他实体的高等抽象。还提供了跨平台的文件I / O和线程功能。以及一些其他的功能。
此处图一张：
![](http://docs.libuv.org/en/v1.x/_images/architecture.png)

### handles 和requests
libuv给用户提供了两个抽象用于和事件循环进行交互： handle和request。
handle表示长期存在的对象，当它激活时，它是用来处理特定的操作。例如：
  * 当激活时，prepare handle 会在每次循环时调用一次他的回调。
  * 当每次有新的连接到来时tcp handle就会调用他的连接回调。

request是用来表示短期存在的操作。这些操作可以通过一个handle来执行：写request通常是用来在一个handle上写数据；或者是独立于handle的： getaddrinfo rquest就不需要一个handle可以直接在事件循环上执行。

### I/O 循环
I/O(或者)循环是libuv的核心。它用来建立所有的I/O操作的内容，并且这意味着I/O 循环是一个单线程的。当然当我们在多个线程上运行的时候就可以跑多个事件循环了。libuv事件循环（或涉及循环或handle的任何其他API）就不是线程安全的，除非另有说明。事件循环遵循常见的单线程异步I/O方法：所有（网络）I/O在非阻塞套接字上执行，使用给定平台上可用的最佳机制：epoll on linux, kqueue on OSX其他等等。作为循环迭代的一部分，循环将阻塞等待已经添加到轮询器的套接字上的I/O活动，并且将触发回调来指示套接字条件（可读，可写的挂断），因此句柄可以读，写或执行期望的I/O操作。
见图
![](http://docs.libuv.org/en/v1.x/_images/loop_iteration.png)

1. loop的now会被更新，在循环开始前，循环会缓存当前的时间以减少和和时间相关的系统调用。
2. 如果循环处于活动状态，则开始迭代，否则循环将立即退出。 那么，什么时候循环被认为是活着的呢？ 如果一个循环有处于激活状态或者被ref的handle，或者活动的请求或者正在被关闭的句柄，它就被认为是活着的。
3. 运行(due)到期的定时器。所有活跃的的定时器如果他们的时间在now之前就会运行他们的cb。

4. 处于等待的回调被调用。虽然所有的I/O回调都会在循环之后立即调用，但是还有这样的一些例子，比如一个回调被延迟到下一此循环了。因此如果上次循环有延时的I/O回调会在这个时间点调用。
5. Idle handle callbacks are called. Despite the unfortunate name, idle handles are run on every loop iteration, if they are active.
6. prepare handle 回调被调用。在循环被I/O阻塞之前（right before the loop will block for I/O）prepare回调就会立即被调用。
7. 轮询超时时间计算。在循环被阻塞之前计算他会被阻塞多久。这里有一些计算的规则：
  * 如果UV_RUN_NOWAIT标志为true则timeout是0。
  * 若果循环将要停止(uv_stop())，timeout是0。
  * 若果没有处于活跃的handle或者request，timeout是0。
  * 若果有idle handles处于活跃状态，timeout是0。
  * 若果有等待被关闭的handle,timeout是0。
  * 若果上述条件没一个满足的，就挑选一个最近的定时器的时间。如果连timer都没有那么timeout 是无限的(infinity)。
8. 循环被I/O阻塞。在这一步，循环会被阻塞一段时间而这个时间就是上一步计算来的。所有的相关handles都会在给定的文件描述符上等待读或写事件若是有就调用他们的回调。
9. Check handle的回调被调用。当事件循环被I/O阻塞结束之后立刻调用check handle的回调。本质上他是和prepare handle 相反的。
10. 关闭回调被调用。如果一个handle呗uv_close()关掉了，此时就会在这个点调用关闭的回调。
11. 在UV_RUN_ONCE的情况下的特殊情况，他意味着进一步的处理。比如在循环被I/O阻塞时没有回调被触发，但是已经过去一段时间了，有可能会有定时器到期，那么在进行一次定时器的回调就可以被调用了。
12. 循环结束。当循环是在UV_RUN_NOWAIT或者UV_RUN_ONCE模式下运行的，那么循环将结束。uv_run会返回。如果实在UV_RUN_DEFAULT模式下运行的，若是他还处于alive状态那么循环会继续，否则也会结束。
> 重要提示：libuv使用线程池来使异步文件I/O操作成为可能，但是网络I/O总是在单个线程（每个循环的线程）中执行。

### 文件I/O
与网络I/O不同，没有libuv可以依赖的特定于平台的文件I/O原语，因此当前的方法是在线程池中运行阻塞文件I/O操作。

libuv目前使用一个全局的线程池，所有的循环都可以和其交互。有如下3种操作使用的是线程池:
1. 文件系统的操作
2. DNS的功能（getaddrinfo and getnameinfo）
3. 用户通过uv_queue_work指定的代码
> 线程池的大小是相当受限制的。

### 参考
1. [为什么使用thread pool来处理文件I/O](http://blog.libtorrent.org/2012/10/asynchronous-disk-io/)
2. [关于怎么使用libuv](http://luohaha.github.io/Chinese-uvbook/source/introduction.html)

### libuv 的队列操作(好久没看c的神操作了，在libuv中我又看到了)
** 2. 定义基本操作 **
```
#define QUEUE_NEXT(q)       (*(QUEUE **) &((*(q))[0]))
#define QUEUE_PREV(q)       (*(QUEUE **) &((*(q))[1]))
```
数组的第0个表示下一个，1表示上一个。
这里使用(*(QUEUE **) &((*(q))[0]))这么复杂的表达是有两个原因。一个是转成左值（这里指的左值是指从数组元素的赋值的角度去看，就是指能不能给数组元素赋值），另一个是保存类型信息。

这样会丢失类型信息
```
#define QUEUE_NEXT(q)          ((*(q))[0])  // 这个是数组的0号元素
```

这样不是左值
```
#define QUEUE_PREV(q)       ((QUEUE *) ((*(q))[1])) // QUEUE* 是个数组指针非左值
```

```
q是 QUEUE * 这个是数组指针非左值
*(q) 是QUEUE
(*(q))[0]是数组第一个元素的值
&((*(q))[0])第一个元素的地址 （非左值）
(QUEUE **) &((*(q))[0]) 还是数组第一个元素之的地址但是这次保存了类型
* (QUEUE **) &((*(q))[0]) 是第一个元素的值得地址即保存了类型又是左值
```

** 3. 取值 **
这个队列的实现和数据无关，所以宏里面看不到data的定义，是不是很神奇，像在c++这种面向对象的语言中，我们一般通过迭代器来实现操作和数据的分离，而c语言可以用很巧妙的方式去高效的实现哦。

```
#define QUEUE_DATA(ptr, type, field)                                          \
  ((type *) ((char *) (ptr) - ((char *) &((type *) 0)->field)))
```

((char *) &((type *) 0)->field))是拿到偏移量。为什么这样就可以拿到偏移量？其实很好理解，把0当做其实地址，取field的地址，就是偏移量啦。

[解释出处](https://www.jianshu.com/p/6373de1e117d)
[另一篇解释](http://masutangu.com/2016/10/libuv-source-code/)
[还有一篇参考](http://gngshn.github.io/2017/09/01/libuv-%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90-1-event-loop%E7%9A%84%E5%88%9D%E5%A7%8B%E5%8C%96/)
