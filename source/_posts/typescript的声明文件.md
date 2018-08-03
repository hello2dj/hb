---
title: typescript的声明文件
date: 2017-12-13 21:58:58
tag:
- typescript
---

{% asset_img ts.png 开篇一图 %}

<!-- more -->

### 开篇扯淡

上次我们谈论协变和逆变时提到了 typescript, ts 本身是 js 的超集，从名字就可以看出来 ts 为 js 带来了类型，那么如何能写好 typescript？答案是从声明文件开始，若是能写的一手好的 ts 的声明文件，那 ts 的书写必然是手到擒来啊！2333333（我们就不在这里再做 ts 的基本语法的普及了详见[handbook](https://www.typescriptlang.org/docs/handbook/basic-types.html)）

### 为什么要有声明文件呢？ts 的声明文件又是什么呢（是以.d.ts 结尾的文件）？

为什么？我们知道 ts 是 js 的超集也是会编译成 js 的，那么已有的 js 代码怎么和现有的 ts 项目融合呢（即在 ts 中引用 js）？因为 js 是弱类型的，是无法推导出变量的类型的，那么 ts 的编译器自然就无法集成 js 的代码了？此时声明文件就起到作用了，声明文件的目的就是告诉 ts 的编译器，我要引用 js 文件了，js 文件的内容有啥，暴露出来的变量叫啥，类型是啥等等，听着怎么那么像是 C/C++的头文件呢？ts 是强类型语言了，每个变量就都会有自己的类型(类型是不可变得哦)，这些类型都是要声明的，还有接口也是要声明的，而 js 代码是没有这些的，所以就需要声明文件了

```
// 1
interface Dog {
  bark: (world: string) => void;
}
const tuDog: Dog = {
  bark: (world: string) {
    console.log('bark');
  }
}

// 2
interface TextSpan {
    start: number;
    end: number;
}

// 3
type Test = (version: number) => void;
```

上面的代码我们定义了两个接口类型，和一个 Test 的函数类型，当我们把这些类型定义都放到一个文件里时就成了一个‘声明文件’了，这里打引号的意思是，正常 ts 之间引用是不用专门抽出来写一成一个文件的(当然你要是乐意那也是可以的)，这个例子就是在说声明文件就是一堆’类型声明‘(这里打引号是因为正式的声明文件里也是会声明变量的)，就好比的 C/C++的头文件

### 举个简单的栗子

1.  新建一个 foo.js

```
// foo.js
module.exports.say = require('./say');
```

2.  新建一个 say.js

```
// say.js 我们在此假设 o 是string
module.exports = function say(o) {
  return o;
}
```

3.  在新建一个 index.ts

```
// index.ts
import * as a from '../foo';  //此时是会报错的，因为没有foo.ts
a.say('123')
```

4.  此时编译 index.ts 是无法通过的, 因为没有 foo.ts
5.  再新建一个 foo.d.ts 文件(需同名，这样 tsc--typescript 的编译器在加载时就会去找相应文件)

```
// 但若是 再加一个foo.d.ts文件
export function say(o: string): string;
```

6.  此时再编译就会通过了，而且若是调用穿得参数不是 string，也是不会通过的

### 上面是通过包的形式引入的,那么全局方式引入的又该如何呢？答案是 declare

1.  新建一个 foo.js

```
// foo.js 我们在此假设 o 是string
global.say = function(o) {
  return o;
};
```

2.  在新建一个 index.ts

```
// index.ts
import * as a from '../foo';  //此时是会报错的，因为没有foo.ts, 并且也没有bark
bark('123')
```

3.  此时编译 index.ts 是无法通过的, 因为没有 foo.ts
4.  再新建一个 foo.d.ts 文件(需同名，这样 tsc--typescript 的编译器在加载时就会去找相应文件)

```
// 但若是 再加一个foo.d.ts文件
delcare function say(o: string): string;
// delcare var say: (o: string) => string;
```

5.  成功 ok

### 通过上面我们可以看出需要通过包引入的我们使用 export, 通过全局似的就可以使用 declare

### declare 的正式说明参见[typescript spec](https://github.com/Microsoft/TypeScript/blob/master/doc/spec.md#12-ambients)

总的来说就是 declare 使声明引用那些已经在代码执行环境中存在的 变量，函数，类，枚举，命名空间，或者 modules, 例如通过 script 标签引入的包。在 declare 命名空间和 modules 时应注意，只能在顶层是使用 declare, 命名空间内部是不允许使用的，命名空间内部的可访问情况可由 export 控制（想让谁可访问就 export 谁）

### 有很多的库都有相应的 ts 声明文件，那么怎么安装呢？

```
npm install --save-dev @types/node(package-name)
```

可以在这里[搜索.d.ts](http://definitelytyped.org/)

### 总结

当然这里仅仅是简单的做了介绍，通过这个简单的介绍我们可以发现，当我们把声明文件写的完整的时候，基本就是 ts 出师的时候了。(举的栗子是两种分开的，那么在我的 node 项目的某个文件中既有 global 形式又有包的导出形式该怎么做呢？我还没写对，有待努力！各位看官要是知道的话忘不吝赐教（dj_amazing@sina.com）)

### 在声明文件中 export **
* 和declare的差不多，import {export的， declare的 } from ''; 都可以被导出， 但是若是使用了 export = something, 那么就只有somethine会被导出了
### declare namespace看做是内部模
块即可，不可作为类型，以及值，只是用来分隔代码

### conditional type 中可以使用 infer 关键字，常规的类型定义中不能使用
推到出 函数返回值类型
```
type ReturnType<T> = T extends (...args: any[]) => infer R ? R : any;
```

普通类型定义 使用infer会报错
```
type ReturnType<T extends (...args: any[]) => infer R> = R;  // Error, not supported
```
参考：

1.  [handbook](https://www.typescriptlang.org/docs/handbook/basic-types.html)

2.  [learn-ts](https://github.com/TypeStrong/learn-typescript)

3.  [DefinitelyTyped](https://github.com/DefinitelyTyped)
