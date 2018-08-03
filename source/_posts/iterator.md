---
title: 关闭迭代器是一个抽象漏洞
date: 2018-01-21 17:30:06
tag:
- iterator
- iterable
- js
---
[原文地址](http://raganwald.com/2017/07/22/closing-iterables-is-a-leaky-abstraction.html)

先上图
![](http://exploringjs.com/es6/images/iteration----consumers_sources.jpg)

<!-- more -->

先来普及一下
[抽象漏洞wiki](https://zh.wikipedia.org/zh-hans/%E6%8A%BD%E8%B1%A1%E6%BC%8F%E6%B4%9E%E5%AE%9A%E5%BE%8B) 
[抽象漏洞](http://www.jdon.com/46006)
总结一小下：抽象漏洞告诉我们出来混早晚要还的，无论你怎么去屏蔽复杂，怎么去抽象，总是不完整的总是会有漏洞的。

### 迭代器和可迭代对象的一个概述

在js中，迭代器和可迭代对象是顺序访问数据的抽象层接口。我们可以在数组或者优先级队列中看到他们。

一个迭代器就是一个拥有next方法的对象。当你调用next()的时候，他会返回一个Plain Old Javascript Object。 这个返回值拥有done属性，当done值是false的时候，这个返回值还会有个value属性，数据就是value属性中的值。反而如果done是true，那么就应该不会有value属性。
> 迭代器不可以是async/await等异步方法

迭代器被设计为一个有状态的对象：重复调用next方法通常能获取到一系列的数据直到done为止。
这里有一个栗子: [jsbin](http://jsbin.com/gibadoxato/edit?js,console)
```
const iCountdown = {
  value: 10,
  done: false,
  next() {
    this.done = this.done || this.value < 0;

    if (this.done) {
      return { done: true };
    } else {
      return { done: false, value: this.value-- };
    }
  }
};

iCountdown.next()
  //=> { done: false, value: 10 }

iCountdown.next()
  //=> { done: false, value: 9 }

iCountdown.next()
  //=> { done: false, value: 8 }

// ...

iCountdown.next()
  //=> { done: false, value: 1 }

iCountdown.next()
  //=> { done: true }
```

可迭代对象是一个拥有[Symbol.iterator]方法的对象。当调用这个方法的时候他会返回一个迭代器。
举个栗子：[jsbin](http://jsbin.com/labibimani/edit?js,console)
```
const countdown = {
  [Symbol.iterator]() {
    const iterator = {
      value: 10,
      done: false,
      next() {
        this.done = this.done || this.value < 0;

        if (this.done) {
          return { done: true };
        } else {
          return { done: false, value: this.value-- };
        }
      }
    };

    return iterator;
  }
};
```
我们可以使用for ... of 来对这个对象进行迭代
```
for (const count of countdown) {
  console.log(count);
}
```
或者我们还可以对他进行解构
```
const [ten, nine, eight, ...rest] = countdown;

ten
  //=> 10
nine
  //=> 9
eight
  //=> 8
rest
  //=> [7, 6, 5, 4, 3, 2, 1]
```
接下来让我们看看如何对一个文件进行迭代？
在看之前我们得先想想node有对文件有按行读的同步方法(上面我们提过了，迭代器只能使用同步方法)么？我找了一圈发现了一个异步读的方法，readline，这个包可以对文件流进行异步按行读取。
```
const readline = require('readline');
const fs = require('fs');

const rl = readline.createInterface({
  input: fs.createReadStream('sample.txt'),
  crlfDelay: Infinity
});

rl.on('line', (line) => {
  console.log(`Line from file: ${line}`);
});
```
可以肯定的是node本身没有提供同步的按行读取文件的方法，那么我们就只能利用node的同步读方法来模拟按行读取了。每次读取一部分然后检查换行符，fs.readSync(fd, buffer, offset, length, position)。当然这里已然有一个包了‘n-readlines’。[参见](https://stackoverflow.com/questions/7545147/nodejs-synchronization-read-large-file-line-by-line)

好了这下我们可以看看对文件的按行迭代了
```
const fs = require('fs');
const lineByLine = require('n-readlines');

function lines (path) {
  return {
    [Symbol.iterator]() {
      return {
        done: false,
        fileDescriptor: new lineByLine(path),
        next() {
          if (this.done) return { done: true };
					const line = this.fileDescriptor.next();
					
          this.done = !line;

          if (this.done) {
						this.fileDescriptor.fd && fs.closeSync(this.fileDescriptor.fd);
            return { done: true };
          } else {
            return { done: false, value: line };
          }
        }
      };
    }
  };
}
```
当我们想要对一个文件进行按行迭代时，我们就可以这么使用lines('./README.md');

当我们调用了[Symbol.iterator]()我们就会得到一个文件的迭代器。

如下
```
for (const line of lines('./iter.js')) {
	console.log(line.toString());
}
```
当我们把文件内容都迭代的读取完了以后，我们自然会关闭了文件。

可是若是我们只想读取第一行呢？
```
for (const line of lines('./iter.js')) {
	console.log(line.toString());
	break;
}
```
这样我们就会有一个问题了，就是该如何关闭文件。我们上面的代码是在读完以后回去检查文件是否关闭然后关闭。可是当我们只读取了一行就退出时，我们的文件时未关闭的。。。

这肯定是不好的。并且上面也不是唯一的情况，有时我们可能使用迭代器去管理我们的异步任务，比如我们通过指定端口和其他进程交互，很显然当我们完成了交互时我们会显式的去关闭指定的端口，我们可不想在使用中却被gc给收集了。

通过上面的描述，很明显我们需要一个能显式关闭迭代器的方法，以便迭代器能够释放他们所占用的资源。接下来让我们做一些尝试。

幸运的是，这里确实有这么一种机制来关闭迭代器。他设计的意图就是用来处理那些拥有各种各样的资源的迭代器，好比是文件描述符，一个打开的端口，大量的内存等等。

迭代器需要释放资源这是个问题，js为我们提供的机制来解决这些问题。但是我们依然先采用自己的方式来试试。

我们来看看

```
const countdown = {
  [Symbol.iterator]() {
    const iterator = {
      value: 10,
      done: false,
      next() {
        this.done = this.done || this.value < 0;

        if (this.done) {
          return { done: true };
        } else {
          return { done: false, value: this.value-- };
        }
      },
      return(value) {
        this.done = true;
        if (arguments.length === 1) {
          return { done: true, value };
        } else {
          return { done: true };
        }
      }
    };

    return iterator;
  }
};
```

### 永远都会被调用的return
我们在前面看到了迭代器的主要方法next,其实这里还有一个return方法，它的签名时return(optionalValue),它的使用如下
* 当optionalValue为空时他应当返回{ done: true }， 否则返回{ done: true, value: optionalValue }
* 此后，再调用next方法时迭代器应当永远都返回 { done: true }

我们再回头看看我们的countdown,来实现一下.return
```
const countdown = {
  [Symbol.iterator]() {
    const iterator = {
      value: 10,
      done: false,
      next() {
        this.done = this.done || this.value < 0;

        if (this.done) {
          return { done: true };
        } else {
          return { done: false, value: this.value-- };
        }
      },
      return(value) {
        this.done = true;
        if (arguments.length === 1) {
          return { done: true, value };
        } else {
          return { done: true };
        }
      }
    };

    return iterator;
  }
};
```
这里我们看到了一些重复的逻辑，但他们是有用的，尤其是释放资源时，让我们再整理一下
```
const countdown = {
  [Symbol.iterator]() {
    const iterator = {
      value: 10,
      done: false,
      next() {
        if (this.done) {
          return { done: true };
        } else if (this.value < 0) {
          return this.return();
        } else {
          return { done: false, value: this.value-- };
        }
      },
      return(value) {
        this.done = true;
        if (arguments.length === 1) {
          return { done: true, value };
        } else {
          return { done: true };
        }
      }
    };

    return iterator;
  }
};
```
现在我们可以看到如何编写一个在耗尽整个迭代器之前打破的循环：
```
count iCountdown = countdown[Symbol.iterator]();

while (true) {
  const { done, value: count } = iCountdown.next();

  if (done) break;

  console.log(count);

  if (count === 6) {
    iCountdown.return();
    break;
  }
}
```
调用return可以保证我们的iCountdown能够释放资源。那么如果for...of也是这么做的话(先调用next,最后调用return)那就完美了。
我们可以加一个输出
```
return(value) {
  if (!this.done) {
    console.log('Return to Forever');
    this.done = true;
  }
  if (arguments.length === 1) {
    return { done: true, value };
  } else {
    return { done: true };
  }
}
```
接着可以尝试
```
for (const count of countdown) {
  console.log(count);
  if (count === 6) break;
}
  //=>
    10
    9
    8
    7
    6
    Return to Forever
```
并且
```
const [ten, nine, eight] = countdown;
  //=> Return to Forever
```
整体[jsbin](http://jsbin.com/yatorehivu/edit?js,console)

当我们没有消费整个迭代器时,就是说中途break，js会自动调用return方法[jsbin](http://jsbin.com/nucixineru/edit?js,console)

我们也可以看出来return方法是可选的，若是实现了js就会去自动调用，没有则不回去调用。

### 调用return也不总是那么简单
看了上面，我们就会为我们的可迭代对象实现return方法尤其是那些需要释放资源的迭代对象，以便js为我们自动调用。

来看一个比较棘手的问题，若是我们就像构造一个函数来返回迭代对象的第一个元素(如果有的话)，如下
```
function first (iterable) {
  const [value] = iterable;

  return value;
}
```
解构会替我们把迭代对象的迭代器给关掉。当然如果我们高兴我们也可以自己来手动实现
```
function first (iterable) {
  const iterator = iterable[Symbol.iterator]();
  const { done, value } = iterator.next();

  if (!done) return value;
}
```
但我们可能会忽略了关闭我们所提取的迭代器，所以我们又必须得这么做：
```
function first (iterable) {
  const iterator = iterable[Symbol.iterator]();
  const { done, value } = iterator.next();

  if (typeof iterator.return === 'function') {
    iterator.return();
  }

  if (!done) return value;
}
```
一个很好的启发是，我们可以使用JavaScript的内置功能来关闭从可迭代对象中提取的迭代器。

而我们也知道解构会为我们关闭迭代器。我们也知道打断for ... of 循环也会关闭迭代器， 不管我们是否消费了整个迭代器。

还有上面所说的对于在一个生成器内从for ... of 中yield数据也是成立的。举个栗子，我们可以看看下面的函数mapWith
```
function * mapWith (mapFn, iterable) {
  for (const value of iterable) {
    yield mapFn(value);
  }
}
```
这是个generator函数接收了一个迭代对象作为参数，并且返回一个可迭代对象。当我消耗了返回的迭代对象，那么内部的迭代对象也会被消耗掉，可若是我们中途断了，又会如何呢？[jsbin](http://jsbin.com/xomulexazu/edit?js,console)
```
const countdownInWords = mapWith(n => words[n], countdown);

for (const word of countdownInWords) {
  break;
}
//=> Return to Forever
```
是的完全ok js的内置功能又帮了我们, 而且在这里我们也看到了，生成器的返回值是可以和for...of一起使用的。

但不幸的是我们也不能总是成功

### 更多关于显示关闭迭代器的栗子
zipWith 函数会接受多个可迭代对象，并且把他们'zip'到一起返回，若是把他也写成一个生成器函数，我们是无法依靠js的内置功能去关闭所有的迭代器的。
来看
```
function * zipWith (zipper, ...iterables) {
  const iterators = iterables.map(i => i[Symbol.iterator]());

  while (true) {
    const pairs = iterators.map(j => j.next()),
          dones = pairs.map(p => p.done),
          values = pairs.map(p => p.value);

    if (dones.indexOf(true) >= 0) {
      for (const iterator of iterators) {
        if (typeof iterator.return === 'function') {
          iterator.return();
        }
      }
      return;
    }

    yield zipper(...values);
  }
}
const fewWords = ['alper', 'bethe', 'gamow'];

for (const pair of zipWith((l, r) => [l, r], countdown, fewWords)) {
  //... diddley
}
  //=> Return to Forever
```
这段代码我们使用了显式关闭的方法，当所有迭代对象中的任何一个消耗尽了我们就显示关闭所有的迭代器([jsbin](http://jsbin.com/nomefozagi/edit?js,console))。但是若是我们提前终止了外部循环，sorry是没人来擦屁股的([jsbin](http://jsbin.com/regibijohu/edit?js,console))。
```
const [[firstCount, firstWord]] = zipWith((l, r) => [l, r], countdown, fewWords);
  //=>
```
试过栗子就知道了，没有Return to Forever输出，尽管js的内置功能帮我们把generator返回的迭代器关闭了，但其他的都没有关闭。但是也很明显，我们的迭代器和generator返回的迭代器毛关系都没有，他哪知道啥时候关闭啊。

根据 [jaffathecake](https://www.reddit.com/user/jaffathecake)的建议，我们可以这么来
```
function * zipWith (zipper, ...iterables) {
  const iterators = iterables.map(i => i[Symbol.iterator]());

  try {
    while (true) {
      const pairs = iterators.map(j => j.next()),
            dones = pairs.map(p => p.done),
            values = pairs.map(p => p.value);

      if (dones.indexOf(true) >= 0) {
        for (const iterator of iterators) {
          if (typeof iterator.return === 'function') {
            iterator.return();
          }
        }
        return;
      }

      yield zipper(...values);
    }
  }
  finally {
    for (const iterator of iterators) {
      if (typeof iterator.return === 'function') {
        iterator.return();
      }
    }
  }
}
```
这个时候我们要是关闭了那肯定可以啊，这就是借助了，try/catch/finally, 就不试了，大家可以试试

还有另外一招我们也可以实现上述目的
```
function zipWith (zipper, ...iterables) {
  return {
    [Symbol.iterator]() {
      return {
        done: false,
        iterators: iterables.map(i => i[Symbol.iterator]()),
        zipper,
        next() {
          const pairs = this.iterators.map(j => j.next()),
                dones = pairs.map(p => p.done),
                values = pairs.map(p => p.value);

          if (dones.indexOf(true) >= 0) {
            return this.return();
          } else {
            return { done: false, value: this.zipper(...values) };
          }
        },
        return(optionalValue) {
          if (!this.done) {
            this.done = true;

            for (const iterable of this.iterators) {
              if (typeof iterable.return === 'function') {
                iterable.return();
              }
            }
          }

          if (arguments.length === 1) {
            return { done: true, value:optionalValue };
          } else {
            return { done: true };
          }
        }
      };
    }
  };
}
```
[jsbin](http://jsbin.com/vakodeyawi/edit?js,console), 这相当于显示实现了一个可迭代对象。
无论哪种方式，我们都必须明确地安排这样的事情，当它自己的迭代器关闭时，zipWith关闭所有的迭代器。

* 到了这步我们可以来说说迭代器和生成器了，生成器可以看做对coder友好的迭代器。因为迭代器虽然很强大但是需要我们自己去维护迭代器的状态，而生成器则无须我们来维护状态，他会替我们去维护，而我们只需要告诉生成器如何产生数据就好了。很显然生成器的返回值就是个迭代器无疑了。
* 可迭代对象，可迭代对象就是那些实现了Symbol.iterator这个方法的对象，就是说若是调用Symbol.iterator这个方法会返回一个迭代器。

那么什么语法可以用于迭代器呢？
* for-of 循环， 我们见过了
* spread syntax, 意味着我们可以这么使用
```
const a = { ...iterable }
```
* yield* 我们知道yield* 后面跟的是另一个生成器，事实他还可以跟一个可迭代对象
```
function* gen() {
  yield* ['a', 'b', 'c'];
}

gen().next(); // { value: "a", done: false }
```
* destructuring assignment解构我们也见过了

怎么样突然发现可迭代对象居然这么厉害了吧

总结一小下生成器就是coder友好版的迭代器，而可迭代对象则是实现了Symbol.iterator方法的对象且该方法返回一个迭代器。而且迭代器可以使用的语法，显然生成器的返回值当然也可以哦。

好了我们言归正传，接上话。

### 隐藏的功能
我们已经看到了，迭代器需要关闭。我们还知道迭代器的关闭是不可见的。有个return 方法需要我们去实现，并且需要被调用。但通常我们都是把迭代器和生成器，for ... of循环或者解构一起使用，他们把调用return这件事给我们隐藏了。

这种有意识的设计让我们对迭代器的学习和使用变得容易。当我们看到下面的代码
```
function * take (numberToTake, iterable) {
  const iterator = iterable[Symbol.iterator]();

  for (let i = 0; i < numberToTake; ++i) {
    const { done, value } = iterator.next();
    if (!done) yield value;
  }
}
```
我们可以很快的明白这段代码是要干啥了，但是下面这段代码是不是更好呢:
```
function * take (numberToTake, iterable) {
  let i = 0;

  for (const value of iterable) {
    if (i++ === numberToTake) {
      return;
    } else {
      yield value;
    }
  }
}
```
但这里有一个关于显式还是隐式的永恒争论点
```
function * take (numberToTake, iterable) {
  const iterator = iterable[Symbol.iterator]();

  try {
    for (let i = 0; i < numberToTake; ++i) {
      const { done, value } = iterator.next();
      if (!done) yield value;
    }
  }
  finally {
    if (typeof iterator.return === 'function') {
      iterator.return();
    }
  }
}
```
for ... of 是不是更优雅？若是for(let i = 0; i < numberToTake; ++i)更快呢？ try...finally是不是更好呢？因为他显式的关闭了迭代器。又或者他是不好的因为他引进了额外的代码？

所有的所有我觉得又回到了那句话：写代码写程序没有最好的，都是取舍问题。你们觉得呢？虽然我还做不到，因为取舍很难确定，因为有的时候是自己写的代码总想找点儿理由来维护他。

### chesterton’s 的围栏和抽象漏洞
在改造事物方面，不同于改变事物，有一个简单朴素的原则;这个原则我们或许可以称之为悖论。在这种情况下存在某种制度或定理;为了简单起见，我们可以这么举例，有一个围栏或者大门设置在了路上。更现代的改革者到了这里并且说“我看不见他的作用，把他清理了吧”，对于更有智慧的改革者会说“如果你看不见他的用处，我是不会让你清理的，再去想想吧，等你想明白了，并且告诉我你看到了他的用处，那么我就会允许你把他彻底毁了。” - G.K.Chesterton

...省略一万字看原文吧
总结一小下就是：无论我们是函数式或者是OO，还是其他的，我们都会有像迭代器一样的抽象漏洞。当我们在大多数情况下使用他是好的，当我们到达使用的边缘的时候就会出问题。因此我们需要了解他的底层原理，若是不了解就不会知道到底除了啥问题。

### 结尾
我们有时候还是需要去了解一些底层的问题，否则有时候会真的很无能为力，尤其是出错的时候，我们可以使用抽象去减少一些复杂度，但这不代表我们完全不需要去了解抽象。

[reddi讨论](https://www.reddit.com/r/javascript/comments/6sdmk9/closing_iterables_is_a_leaky_abstraction/)

### 大大的总结一下
第一次听说抽象漏洞，学习了，非不证自明的东西总是会存在漏洞的。也加深了对迭代器，迭代对象，生成器的理解。其实还有一个更重要的就是，资源的关闭，我们的资源是有限的，一定要有收尾操作。

再来列举一下迭代对象可以使用的语法，生成器(准确说他的返回值)也可以哦
* for ... of
* 解构赋值
* rest syntax
* yield *

迭代对象的三个方法
* next
* return (可选)
* throw (可选)

js 会在for... of, 解构赋值，之后为我们自动调用return，当然是在没有完全消费掉数据的时候才会为我们调用。注意释放资源。我们可以利用这个功能。

在使用generator和迭代器的过程中要注意释放资源。能用js内置的最好不能就try/catch/finally