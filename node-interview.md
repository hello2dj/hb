### 类型判断 === 和 ==
null == undefined true

### 作用域词法作用域

### js 中什么类型是引用传递, 什么类型是值传递? 如何将值类型的变量以引用的方式传递?
```
function changeStuff(a, b, c)
{
  a = a * 10;
  b.item = "changed";
  c = {item: "changed"};
}

var num = 10;
var obj1 = {item: "unchanged"};
var obj2 = {item: "unchanged"};

changeStuff(num, obj1, obj2);

console.log(num);
console.log(obj1.item);    
console.log(obj2.item);

// 10
// changed
// unchanged
```
按引用传递，意味着无论如何改变都会映射到原始值，就是说好比 c就完全等于obj2，无论是对c赋值，还是修改属性都会映射到obj2， 也就是说你打印c的地址就是obj2的地址

安值传递就是说，按copy赋值，完全不影响原始值

我认为golang和js都是按值传递 --- call by sharing

### JavaScript 中不同类型以及不同环境下变量的内存都是何时释放?
1. 引用类型在没有引用之后就会被回收（根不可达）
2. 值类型要是被闭包引用，那也会等到闭包被回收后才会被回收，非闭包情况下，新生代切换就会被回收
3. 
```
let arr = [];
while(true)
  arr.push(1);
```
爆栈

```
let arr = [];
while(true)
  arr.push();
```
不爆，push没有参数时，array不会增长

```
function out() {
  const bigData = new Buffer(100);
  inner = function () {
    void bigData;
  }
}
```
闭包引用，so爆栈

4. 不同数据类型的存储位置
5. 新生代 copy-swap,  老生代mark-gc

### es6 新特性
Map, Set
new Set(...array)

### 如何在不重启 node 进程的情况下热更新一个 js/json 文件? 这个问题本身是否有问题? 
清楚require.cache, json文件， 视具体情况还可以用 VM 模块重新执行

### 模块机制
其实官方文档已经说得很清楚了, 每个 node 进程只有一个 VM 的上下文, 不会跟浏览器相差多少, 模块机制在文档中也描述的非常清楚了:
```
function require(...) {
  var module = { exports: {} };
  ((module, exports) => {
    // Your module code here. In this example, define a function.
    function some_func() {};
    exports = some_func;
    // At this point, exports is no longer a shortcut to module.exports, and
    // this module will still export an empty default object.
    module.exports = some_func;
    // At this point, the module will now export some_func, instead of the
    // default object.
  })(module, module.exports);
  return module.exports;
}
```

> 如果 a.js require 了 b.js, 那么在 b 中定义全局变量 t = 111 能否在 a 中直接打印出来? 否

> a.js 和 b.js 两个文件互相 require 是否会死循环? 双方是否能导出变量? 如何从设计上避免这种问题?

不会, 先执行的导出空对象, 通过导出工厂函数让对方从函数去拿比较好避免. 模块在导出的只是 var module = { exports: {} }; 中的 exports, 以从 a.js 启动为例, a.js 还没执行完 exports 就是 {} 在 b.js 的开头拿到的就是 {} 而已.

> require的实现原理

### 那么为什么 Node.js 不给每一个.js文件以独立的上下文来避免作用域被污染
独立上下文和作用域是不是一回事呢？ 作用域是指全局作用域么？还是指什么作用域？

1. 那么为什么 Node.js 不给每个 js 模块文件分配单独的 v8::Context 呢？

我觉得 Node.js 现在这个做法还挺自然的吧，一是确实有使用全局变量的场景存在，二是 runInNewContext() 的性能真的慢几个数量级。

1. context 有不同的含义execution context 之间不是隔离的，v8::Context 之间是隔离的。
2. 独立的 execution context 不能阻止 scope 被污染。
3. 一个 v8::Context 实例的全局 scope 被污染了，不会影响到另一个的 v8::Context 实例。
4. Node.js 默认不会给每个 js 模块文件分配单独的 v8::Context 。
5. 在 Node.js 的早期版本里有一个环境变量能改变这个行为，后来这个环境变量也没有了。 [详见](https://github.com/nodejs/node/blob/f19e9b6a7603c852bc9cec9516c0fe70fc8db173/lib/module.js#L394)

https://www.zhihu.com/question/62131680

### Event
1. emit是同步方法

> The EventListener calls all listeners synchronously in the order in which they were registered. This is important to ensure the proper sequencing of events and to avoid race conditions or logic errors.

> 若是异步，比如emit('a')事件， 那么监听者是异步执行，若是此时再一次emit('a')事件，多次emit同一个事件，那么执行顺序该是什么呢？这就是问题

```
const EventEmitter = require('events');

let emitter = new EventEmitter();

emitter.on('myEvent', () => {
  console.log('hi');
  emitter.emit('myEvent');
});

emitter.emit('myEvent');
```
死循环，emit是同步触发
```
const EventEmitter = require('events');

let emitter = new EventEmitter();

emitter.on('myEvent', function sth () {
  emitter.on('myEvent', sth);
  console.log('hi');
});

emitter.emit('myEvent');
```
不会，on加入的listeners和emit触发的listeners不是同一个，后者是前者copy

> 使用 emitter 处理问题可以处理比较复杂的状态场景, 比如 TCP 的复杂状态机, 做多项异步操作的时候每一步都可能报错, 这个时候 .emit 错误并且执行某些 .once 的操作可以将你从泥沼中拯救出来. 
> 另外可以注意一下的是, 有些同学喜欢用 emitter 来监控某些类的状态, 但是在这些类释放的时候可能会忘记释放 emitter, 而这些类的内部可能持有该 emitter 的 listener 的引用从而导致内存泄漏.

> 有这样一个场景, 你在线上使用 koa 搭建了一个网站, 这个网站项目中有一个你同事写的接口 A, 而 A 接口中在特殊情况下会变成死循环. 那么首先问题是, 如果触发了这个死循环, 会对网站造成什么影响?

真个网站都没响应了呗

> 如何实现一个异步的 reduce? (注:不是异步完了之后同步 reduce)

需要了解 reduce 的情况, 是第 n 个与 n+1 的结果异步处理完之后, 在用新的结果与第 n+2 个元素继续依次异步下去. 不贴答案, 期待诸君的版本.
> 带一个next方法把值传回去

### Timers
在笔者这里将 Node.js 中的异步简单的划分为两种, 硬异步和软异步.

硬异步是指由于 IO 操作或者外部调用走 libuv 而需要异步的情况. 当然, 也存在 readFileSync, execSync 等例外情况, 不过 node 由于是单线程的, 所以如果常规业务在普通时段执行可能比较耗时同步的 IO 操作会使得其执行过程中其他的所有操作都不能响应, 有点作死的感觉. 不过在启动/初始化以及一些工具脚本的应用场景下是完全没问题的. 而一般的场景下 IO 操作都是需要异步的.

软异步是指, 通过 setTimeout 等方式来实现的异步. 关于 nextTick, setTimeout 以及 setImmediate 三者的区别参见该帖

browser tasks, queue, microtasks
* In summary:
    1. Tasks execute in order, and the browser may render between them
    2. Microtasks execute in order, and are executed:
        1. after every callback, as long as no other JavaScript is mid-execution
        2. at the end of each task


### 进程
process.nextTick 并不属于 Event loop 中的某一个阶段, 而是在 Event loop 的每一个阶段结束后, 直接执行 nextTickQueue 中插入的 "Tick", 并且直到整个 Queue 处理完. 所以面试时又有可以问的问题了, 递归调用 process.nextTick 会怎么样? 死掉

> 进程的当前工作目录是什么? 有什么作用?

当前进程启动的目录, 通过 process.cwd() 获取当前工作目录 (current working directory), 通常是命令行启动的时候所在的目录 (也可以在启动时指定), 文件操作等使用相对路径的时候会相对当前工作目录来获取文件.

一些获取配置的第三方模块就是通过你的当前目录来找配置文件的. 所以如果你错误的目录启动脚本, 可能没法得到正确的结果. 在程序中可以通过 process.chdir() 来改变当前的工作目录.

> child_process.fork 与 POSIX 的 fork 有什么区别?
Node.js 的 child_process.fork() 在 Unix 上的实现最终调用了 POSIX fork(2), 而 POSIX 的 fork 需要手动管理子进程的资源释放 (waitpid), child_process.fork 则不用关心这个问题, Node.js 会自动释放, 并且可以在 option 中选择父进程死后是否允许子进程存活.

* spawn() 启动一个子进程来执行命令
    * options.detached 父进程死后是否允许子进程存活
    * options.stdio 指定子进程的三个标准流
* spawnSync() 同步版的 spawn, 可指定超时, 返回的对象可获得子进程的情况
* exec() 启动一个子进程来执行命令, 带回调参数获知子进程的情况, 可指定进程运行的超时时间
* execSync() 同步版的 exec(), 可指定超时, 返回子进程的输出 (stdout)
* execFile() 启动一个子进程来执行一个可执行文件, 可指定进程运行的超时时间
* execFileSync() 同步版的 execFile(), 返回子进程的输出, 如何超时或者 exit code 不为 0, 会直接 throw Error
* fork() 加强版的 spawn(), 返回值是 ChildProcess 对象可以与子进程交互

常见会问的面试题, 如 child.kill 与 child.send 的区别. 二者一个是基于信号系统, 一个是基于 IPC.

> 父进程或子进程的死亡是否会影响对方? 什么是孤儿进程?

子进程死亡不会影响父进程, 不过子进程死亡时（线程组的最后一个线程，通常是“领头”线程死亡时），会向它的父进程发送死亡信号. 反之父进程死亡, 一般情况下子进程也会随之死亡, 但如果此时子进程处于可运行态、僵死状态等等的话, 子进程将被进程1（init 进程）收养，从而成为孤儿进程. 另外, 子进程死亡的时候（处于“终止状态”），父进程没有及时调用 wait() 或 waitpid() 来返回死亡进程的相关信息，此时子进程还有一个 PCB 残留在进程表中，被称作僵尸进程.

### Cluster

Cluster 是常见的 Node.js 利用多核的办法. 它是基于 child_process.fork() 实现的, 所以 cluster 产生的进程之间是通过 IPC 来通信的, 并且它也没有拷贝父进程的空间, 而是通过加入 cluster.isMaster 这个标识, 来区分父进程以及子进程, 达到类似 POSIX 的 fork 的效果.
```
const cluster = require('cluster');            // | | 
const http = require('http');                  // | | 
const numCPUs = require('os').cpus().length;   // | |    都执行了
                                               // | | 
if (cluster.isMaster) {                        // |-|-----------------
  // Fork workers.                             //   | 
  for (var i = 0; i < numCPUs; i++) {          //   | 
    cluster.fork();                            //   | 
  }                                            //   | 仅父进程执行 (a.js)
  cluster.on('exit', (worker) => {             //   | 
    console.log(`${worker.process.pid} died`); //   | 
  });                                          //   |
} else {                                       // |-------------------
  // Workers can share any TCP connection      // | 
  // In this case it is an HTTP server         // | 
  http.createServer((req, res) => {            // | 
    res.writeHead(200);                        // |   仅子进程执行 (b.js)
    res.end('hello world\n');                  // | 
  }).listen(8000);                             // | 
}                                              // |-------------------
                                               // | |
console.log('hello');                          // | |    都执行了
```

### How It Works


worker 进程是由 child_process.fork() 方法创建的, 所以可以通过 IPC 在主进程和子进程之间相互传递服务器句柄.

cluster 模块提供了两种分发连接的方式.

第一种方式 (默认方式, 不适用于 windows), 通过时间片轮转法（round-robin）分发连接. 主进程监听端口, 接收到新连接之后, 通过时间片轮转法来决定将接收到的客户端的 socket 句柄传递给指定的 worker 处理. 至于每个连接由哪个 worker 来处理, 完全由内置的循环算法决定.

第二种方式是由主进程创建 socket 监听端口后, 将 socket 句柄直接分发给相应的 worker, 然后当连接进来时, 就直接由相应的 worker 来接收连接并处理.

使用第二种方式时理论上性能应该较高, 但是在时间上存在负载不均衡的问题, 比如通常 70% 的连接仅被 8 个进程中的 2 个处理, 而其他进程比较清闲.

### 进程IPC

IPC (Inter-process communication) 进程间通信技术. 常见的进程间通信技术列表如下:

类型	无连接	可靠	流控制	优先级
普通PIPE	N	Y	Y	N
命名PIPE   N	Y	Y	N
消息队列    N	Y	Y	N
信号量      N	Y	Y	Y
共享存储	N	Y	Y	Y
UNIX流SOCKET	N	Y	Y	N
UNIX数据包SOCKET	Y	Y	N	N

Node.js 中的 IPC 通信是由 libuv 通过管道技术实现的, 在 windows 下由命名管道（named pipe）实现也就是上表中的最后第二个, *nix 系统则采用 UDS (Unix Domain Socket) 实现.

普通的 socket 是为网络通讯设计的, 而网络本身是不可靠的, 而为 IPC 设计的 socket 则不然, 因为默认本地的网络环境是可靠的, 所以可以简化大量不必要的 encode/decode 以及计算校验等, 得到效率更高的 UDS 通信.

> 在 IPC 通道建立之前, 父进程与子进程是怎么通信的? 如果没有通信, 那 IPC 是怎么建立的?

这个问题也挺简单, 只是个思路的问题. 在通过 child_process 建立子进程的时候, 是可以指定子进程的 env (环境变量) 的. 所以 Node.js 在启动子进程的时候, 主进程先建立 IPC 频道, 然后将 IPC 频道的 fd (文件描述符) 通过环境变量 (NODE_CHANNEL_FD) 的方式传递给子进程, 然后子进程通过 fd 连上 IPC 与父进程建立连接.

最后于进程间通信 (IPC) 的问题, 一般不会直接问 IPC 的实现, 而是会问什么情况下需要 IPC, 以及使用 IPC 处理过什么业务场景等.

### 守护进程

node中守护进程创建
1. 创建一个进程A。
2. 在进程A中创建进程B，我们可以使用fork方式，或者其他方法。
3. 对进程B执行 setsid 方法。
4. 进程A退出，进程B由init进程接管。此时进程B为守护进程。

```
const spawn = require('child_process').spawn;

const child = spawn(process.argv[0], ['child_program.js'], {
  detached: true,
  stdio: 'ignore'
});

child.unref();
```

### Buffer
Buffer 是 Node.js 中用于处理二进制数据的类, 其中与 IO 相关的操作 (网络/文件等) 均基于 Buffer. Buffer 类的实例非常类似整数数组, 但其大小是固定不变的, 并且其内存在 V8 堆栈外分配原始内存空间. Buffer 类的实例创建之后, 其所占用的内存大小就不能再进行调整.

在 Node.js v6.x 之后 new Buffer() 接口开始被废弃, 理由是参数类型不同会返回不同类型的 Buffer 对象, 所以当开发者没有正确校验参数或没有正确初始化 Buffer 对象的内容时, 以及不了解的情况下初始化 就会在不经意间向代码中引入安全性和可靠性问题.

### TypedArray
Node.js 的 Buffer 在 ES6 增加了 TypedArray 类型之后, 修改了原来的 Buffer 的实现, 选择基于 TypedArray 中 Uint8Array 来实现, 从而提升了一波性能.

```
const arr = new Uint16Array(2);
arr[0] = 5000;
arr[1] = 4000;

const buf1 = Buffer.from(arr); // 拷贝了该 buffer
const buf2 = Buffer.from(arr.buffer); // 与该数组共享了内存

console.log(buf1);
// 输出: <Buffer 88 a0>, 拷贝的 buffer 只有两个元素
console.log(buf2);
// 输出: <Buffer 88 13 a0 0f>

arr[1] = 6000;
console.log(buf1);
// 输出: <Buffer 88 a0>
console.log(buf2);
// 输出: <Buffer 88 13 70 17>
```

### String Decoder
字符串解码器 (String Decoder) 是一个用于将 Buffer 拿来 decode 到 string 的模块, 是作为 Buffer.toString 的一个补充, 它支持多字节 UTF-8 和 UTF-16 字符. 例如
```
const StringDecoder = require('string_decoder').StringDecoder;
const decoder = new StringDecoder('utf8');

const cent = Buffer.from([0xC2, 0xA2]);
console.log(decoder.write(cent)); // ¢

const euro = Buffer.from([0xE2, 0x82, 0xAC]);
console.log(decoder.write(euro)); // €
```

### console.log
1. Writes may be synchronous depending on what the stream is connected to and whether the system is Windows or POSIX:
2. Files: synchronous on Windows and POSIX
3. TTYs (Terminals): asynchronous on Windows, synchronous on POSIX
4. Pipes (and sockets): synchronous on Windows, asynchronous on POSIX


### 同步获取用户输入
```
/*
 * http://stackoverflow.com/questions/3430939/node-js-readsync-from-stdin
 * @mklement0
 */
var fs = require('fs');

var BUFSIZE = 256;
var buf = new Buffer(BUFSIZE);
var bytesRead;

module.exports = function() {
  var fd = ('win32' === process.platform) ? process.stdin.fd : fs.openSync('/dev/stdin', 'rs');
  bytesRead = 0;

  try {
    bytesRead = fs.readSync(fd, buf, 0, BUFSIZE);
  } catch (e) {
    if (e.code === 'EAGAIN') { // 'resource temporarily unavailable'
      // Happens on OS X 10.8.3 (not Windows 7!), if there's no
      // stdin input - typically when invoking a script without any
      // input (for interactive stdin input).
      // If you were to just continue, you'd create a tight loop.
      console.error('ERROR: interactive stdin input not supported.');
      process.exit(1);
    } else if (e.code === 'EOF') {
      // Happens on Windows 7, but not OS X 10.8.3:
      // simply signals the end of *piped* stdin input.
      return '';
    }
    throw e; // unexpected exception
  }

  if (bytesRead === 0) {
    // No more stdin input available.
    // OS X 10.8.3: regardless of input method, this is how the end 
    //   of input is signaled.
    // Windows 7: this is how the end of input is signaled for
    //   *interactive* stdin input.
    return '';
  }
  // Process the chunk read.

  var content = buf.toString('utf8', 0, bytesRead - 1);

  return content;
};
```

### Readline
readline 模块提供了一个用于从 Readble 的 stream (例如 process.stdin) 中一次读取一行的接口. 当然你也可以用来读取文件或者 net, http 的 stream, 比如:
```
const readline = require('readline');
const fs = require('fs');

const rl = readline.createInterface({
  input: fs.createReadStream('sample.txt')
});

rl.on('line', (line) => {
  console.log(`Line from file: ${line}`);
});
```

### Net

* 粘包
默认情况下, TCP 连接会启用延迟传送算法 (Nagle 算法), 在数据发送之前缓存他们. 如果短时间有多个数据发送, 会缓冲到一起作一次发送 (缓冲大小见 socket.bufferSize), 这样可以减少 IO 消耗提高性能.

如果是传输文件的话, 那么根本不用处理粘包的问题, 来一个包拼一个包就好了. 但是如果是多条消息, 或者是别的用途的数据那么久需要处理粘包.


可以参见网上流传比较广的一个例子, 连续调用两次 send 分别发送两段数据 data1 和 data2, 在接收端有以下几种常见的情况:

A. 先接收到 data1, 然后接收到 data2 .
B. 先接收到 data1 的部分数据, 然后接收到 data1 余下的部分以及 data2 的全部.
C. 先接收到了 data1 的全部数据和 data2 的部分数据, 然后接收到了 data2 的余下的数据.
D. 一次性接收到了 data1 和 data2 的全部数据.

其中的 BCD 就是我们常见的粘包的情况. 而对于处理粘包的问题, 常见的解决方案有:

1. 多次发送之前间隔一个等待时间
2. 关闭 Nagle 算法
3. 进行封包/拆包

* 方案1

只需要等上一段时间再进行下一次 send 就好, 适用于交互频率特别低的场景. 缺点也很明显, 对于比较频繁的场景而言传输效率实在太低. 不过几乎不用做什么处理.

* 方案2

关闭 Nagle 算法, 在 Node.js 中你可以通过 socket.setNoDelay() 方法来关闭 Nagle 算法, 让每一次 send 都不缓冲直接发送.

该方法比较适用于每次发送的数据都比较大 (但不是文件那么大), 并且频率不是特别高的场景. 如果是每次发送的数据量比较小, 并且频率特别高的, 关闭 Nagle 纯属自废武功.

另外, 该方法不适用于网络较差的情况, 因为 Nagle 算法是在服务端进行的包合并情况, 但是如果短时间内客户端的网络情况不好, 或者应用层由于某些原因不能及时将 TCP 的数据 recv, 就会造成多个包在客户端缓冲从而粘包的情况. (如果是在稳定的机房内部通信那么这个概率是比较小可以选择忽略的)

* 方案3

封包/拆包是目前业内常见的解决方案了. 即给每个数据包在发送之前, 于其前/后放一些有特征的数据, 然后收到数据的时候根据特征数据分割出来各个数据包.

### TCP

TCP是为流量设计的（每秒内可以传输多少KB的数据），讲究的是充分利用带宽。而 KCP是为流速设计的（单个数据包从一端发送到一端需要多少时间），以10%-20%带宽浪费的代价换取了比 TCP快30%-40%的传输速度。TCP信道是一条流速很慢，但每秒流量很大的大运河，而KCP是水流湍急的小激流。KCP有正常模式和快速模式两种，通过以下策略达到提高流速的结果：

* 可靠传输

> window 是否设置的越大越好?

类似木桶理论, 一个木桶能装多少水, 是由最短的那块木板决定的. 一个 TCP 连接的 window 是由该连接中间一连串设备中 window 最小的那一个设备决定的.

* backlog

![](https://github.com/ElemeFE/node-interview/raw/master/assets/socket-backlog.png)

backlog是来设置accept queue的长度的。另外提一句, 这个 backlog 是通过系统指定时是通过 somaxconn 参数来指定 accept queue 的. 而 tcp_max_syn_backlog 参数指定的是 SYN queue 的长度.

* TCP状态机
![](https://github.com/ElemeFE/node-interview/raw/master/assets/tcpfsm.png)

> TIME_WAIT 是什么情况? 出现过多的 TIME_WAIT 可能是什么原因?

TIME_WAIT 是连接的某一方 (可能是服务端也可能是客户端) 主动断开连接时, 四次挥手等待被断开的一方是否收到最后一次挥手 (ACK) 的状态. 如果在等待时间中, 再次收到第三次挥手 (FIN) 表示对方没收到最后一次挥手, 这时要再 ACK 一次. 这个等待的作用是避免出现连接混用的情况 (prevent potential overlap with new connections see TCP Connection Termination for more).

出现大量的 TIME_WAIT 比较常见的情况是, 并发量大, 服务器在短时间断开了大量连接. 对应 HTTP server 的情况可能是没开启 keepAlive. 如果有开 keepAlive, 一般是等待客户端自己主动断开, 那么TIME_WAIT 就只存在客户端, 而服务端则是 CLOSE_WAIT 的状态, 如果服务端出现大量 CLOSE_WAIT, 意味着当前服务端建立的连接大面积的被断开, 可能是目标服务集群重启之类.

### UDP
> TCP/UDP 的区别? UDP 有粘包吗?

协议	连接性	双工性	可靠性	有序性	有界性	拥塞控制	传输速度	量级	头部大小
TCP	面向连接(Connection oriented)	全双工(1:1)	可靠(重传机制)	有序 (通过SYN排序)	无, 有粘包情况	有	慢	低	20~60字节
UDP	无连接(Connection less)	n:m	不可靠(丢包后数据丢失)	无序	有消息边界, 无粘包	无	快	高	8字节

> UDP socket 支持 n 对 m 的连接状态, 在官方文档中有写到在 dgram.createSocket(options[, callback]) 中的 option 可以指定 reuseAddr 即 SO_REUSEADDR标志. 通过 SO_REUSEADDR 可以简单的实现 n 对 m 的多播特性 (不过仅在支持多播的系统上才有).


### HTTP
> cookie 与 session 的区别? 服务端如何清除 cookie?

主要区别在于, session 存在服务端, cookie 存在客户端. session 比 cookie 更安全. 而且 cookie 不一定一直能用 (可能被浏览器关掉). 服务端可以通过设置 cookie 的值为空并设置一个及时的 expires 来清除存在客户端上的 cookie.

> 什么是跨域请求? 如何允许跨域?

出于安全考虑, 默认情况下使用 XMLHttpRequest 和 Fetch 发起 HTTP 请求必须遵守同源策略, 即只能向相同 host 请求 (host = hostname : port) 注[1]. 向不同 host 的请求被称作跨域请求 (cross-origin HTTP request). 可以通过设置 CORS headers 即 Access-Control-Allow- 系列来允许跨域. 例如:
```
location ~* ^/(?:v1|_) {
  if ($request_method = OPTIONS) { return 200 ''; }
  header_filter_by_lua '
    ngx.header["Access-Control-Allow-Origin"] = ngx.var.http_origin; # 这样相当于允许所有来源了
    ngx.header["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, PATCH, OPTIONS";
    ngx.header["Access-Control-Allow-Credentials"] = "true";
    ngx.header["Access-Control-Allow-Headers"] = "Content-Type";
  ';
  proxy_pass http://localhost:3001;
}
```
> 同源除了相同 host 也包括相同协议. 所以即使 host 相同, 从 HTTP 到 HTTPS 也属于跨域

### socket hang up
hang up 有挂断的意思, socket hang up 也可以理解为 socket 被挂断. 在 Node.js 中当你要 response 一个请求的时候, 发现该这个 socket 已经被 "挂断", 就会就会报 socket hang up 错误.

典型的情况是用户使用浏览器, 请求的时间有点长, 然后用户简单的按了一下 F5 刷新页面. 这个操作会让浏览器取消之前的请求, 然后导致服务端 throw 了一个 socket hang up.
[overflow](https://stackoverflow.com/questions/16995184/nodejs-what-does-socket-hang-up-actually-mean)

### DNS
> DNS 模块中 .lookup 与 .resolve 的区别?
当你要解析一个域名的 ip 时, 通过 .lookup 查询直接调用 getaddrinfo 来拿取地址, 速度很快, 但是如果本地的 hosts 文件被修改了, .lookup 就会拿 hosts 文件中的地方, 而 .resolve 依旧是外部正常的地址.

由于 .lookup 是同步的, 所以如果由于什么不可控的原因导致 getaddrinfo 缓慢或者阻塞是会影响整个 Node 进程的, 参见[文档](https://nodejs.org/dist/latest-v6.x/docs/api/dns.html#dns_dns_lookup).

### MQ
比较适合业务解耦/广播/限流/削锋等场景

### 调整node v8的内存使用上限
用--max-old-space-size 和 --max-new-space-size 来设置 v8 使用内存的上限

### 这题有陷阱！在类Unix系统中你不应该尝试去监听80端口，因为这需要超级用户权限。 因此不推荐让你的应用直接监听这个端口。