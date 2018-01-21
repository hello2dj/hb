---
title: rocksdb与LSM-Tree
tag:
- rocksdb
- LSM-Tree
---

![](http://rocksdb.org/static/logo.svg)

今天看了几篇非常棒的文章，列出来与大家共享
[rocksdb与LSM-Tree](http://blog.mrriddler.com/2017/05/30/LSM-Tree%E4%B8%8ERocksDB/)

[leveldb简介](https://soulmachine.gitbooks.io/system-design/content/cn/key-value-store.html)

[关于leveldb的进一步解析](http://bean-li.github.io/categories/)

从这些文章中大致了解了leveldb为何称之为leveldb以及其内部工作原理的大致了解，值得一读再读。对于其转换随机读写为顺序读写的方式值得深思，条条大路通罗马，有的时候是思想狭隘限制我们的生活与工作，多看，多想，多思考~~