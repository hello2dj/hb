---
title: node源码系列1
date: 2018-08-02 18:50:27
tags: nodejs
---

v10.7.0中的一段代码fixed_queue.js

<!-- more -->

纯贴一波代码，这段代码/internal/process/fixed_queue.js, 使用在/internal/process/next_tick.js中，用来管理添加的next_tick的回调函数，在早期版本中使用的array来管理，在某一期就换成了这个，我测试性能，他俩不在一个数量级（其实都不用测试因为fixed_queue，本身用的array），array明显快于fixed_queue, 但我也没有找到替换的理由。这个数据够很有意思

1. 他相当于是一个一个的ringbuffer 连接起来的单向链表
2. 使用ringbuffer是为了更好更高效的利用内存(这也是我推测的替换的一个原因)， 因为以前用的是是一个大的array，当next_tick过多就会出现一个array占用过多内存，对gc不友好，小块小块的操作也利于对象的回收
> 这里又个我们上了一个生动的课程就是分而治之，既然大块内存不利于开辟与操作，有浪费的可能，那就每一次拿一小块，就像是node的事件循环，他并不是把所有的事件统统放到一个queue中，而是做了拆分，根据事件的种类进行拆分，再说一一个就是go的channel，若是所有的数据都通过一个channel进行传递，也是可以，但当量大的时候返回会很弱， 但是拆分过细也不行，就好比go的channel他的调度能力也是有限的，不是无穷无尽的，当达到10w是就会出现饿死的goroutine。 返回来看我们的fixed_queue, 他的每个ringbuffer 设置的大小是2048，而不是10，因为若是太小，反而有加剧了gc的工作，因为要过于频繁的gc。
3. 为什么大小是2048， 我们以往在实现ringbuffer的时候都是通过 mod来进行判断，在这里我们有学习了， 当Size为2的n次方时我们还可以通过是用 (Size - 1) & x来取模
```
Size = 8;
Size - 1 = b111;
显然 n 无论是多少和111相与得到的值都是在（0, 7）内正好是8的模
```

```
'use strict';

// Currently optimal queue size, tested on V8 6.0 - 6.6. Must be power of two.
const kSize = 2048;
const kMask = kSize - 1;

// The FixedQueue is implemented as a singly-linked list of fixed-size
// circular buffers. It looks something like this:
//
//  head                                                       tail
//    |                                                          |
//    v                                                          v
// +-----------+ <-----\       +-----------+ <------\         +-----------+
// |  [null]   |        \----- |   next    |         \------- |   next    |
// +-----------+               +-----------+                  +-----------+
// |   item    | <-- bottom    |   item    | <-- bottom       |  [empty]  |
// |   item    |               |   item    |                  |  [empty]  |
// |   item    |               |   item    |                  |  [empty]  |
// |   item    |               |   item    |                  |  [empty]  |
// |   item    |               |   item    |       bottom --> |   item    |
// |   item    |               |   item    |                  |   item    |
// |    ...    |               |    ...    |                  |    ...    |
// |   item    |               |   item    |                  |   item    |
// |   item    |               |   item    |                  |   item    |
// |  [empty]  | <-- top       |   item    |                  |   item    |
// |  [empty]  |               |   item    |                  |   item    |
// |  [empty]  |               |  [empty]  | <-- top  top --> |  [empty]  |
// +-----------+               +-----------+                  +-----------+
//
// Or, if there is only one circular buffer, it looks something
// like either of these:
//
//  head   tail                                 head   tail
//    |     |                                     |     |
//    v     v                                     v     v
// +-----------+                               +-----------+
// |  [null]   |                               |  [null]   |
// +-----------+                               +-----------+
// |  [empty]  |                               |   item    |
// |  [empty]  |                               |   item    |
// |   item    | <-- bottom            top --> |  [empty]  |
// |   item    |                               |  [empty]  |
// |  [empty]  | <-- top            bottom --> |   item    |
// |  [empty]  |                               |   item    |
// +-----------+                               +-----------+
//
// Adding a value means moving `top` forward by one, removing means
// moving `bottom` forward by one. After reaching the end, the queue
// wraps around.
//
// When `top === bottom` the current queue is empty and when
// `top + 1 === bottom` it's full. This wastes a single space of storage
// but allows much quicker checks.

const FixedCircularBuffer = class FixedCircularBuffer {
    constructor() {
        this.bottom = 0;
        this.top = 0;
        this.list = new Array(kSize);
        this.next = null;
    }

    isEmpty() {
        return this.top === this.bottom;
    }

    isFull() {
        return ((this.top + 1) & kMask) === this.bottom;
    }

    push(data) {
        this.list[ this.top ] = data;
        this.top = (this.top + 1) & kMask;
    }

    shift() {
        const nextItem = this.list[ this.bottom ];
        if (nextItem === undefined)
            return null;
        this.list[ this.bottom ] = undefined;
        this.bottom = (this.bottom + 1) & kMask;
        return nextItem;
    }
};

class FixedQueue {
    constructor() {
        this.head = this.tail = new FixedCircularBuffer();
    }

    isEmpty() {
        return this.head.isEmpty();
    }

    push(data) {
        if (this.head.isFull()) {
            // Head is full: Creates a new queue, sets the old queue's `.next` to it,
            // and sets it as the new main queue.
            this.head = this.head.next = new FixedCircularBuffer();
        }
        this.head.push(data);
    }

    shift() {
        const { tail } = this;
        const next = tail.shift();
        if (tail.isEmpty() && tail.next !== null) {
            // If there is another queue, it forms the new tail.
            this.tail = tail.next;
        }
        return next;
    }
};
```