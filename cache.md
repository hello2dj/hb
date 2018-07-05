### https://juejin.im/entry/5afe6e08f265da0b8455aa0c?utm_source=gold_browser_extension

### https://coolshell.cn/articles/17416.html

cache aside 策略，先写数据库在删除，针对删除失败可以使用消息队列重试机制，这种方式不一致的情况为
--> A 来写更新
--> B 来查找，没有数据，读数据库
--> A 更新完毕
--> A 设置缓存失效
--> B 设置缓存

1.  业务侵入性强方式：业务代码重试机制
2.  业余侵入性不强方式：监听 oplog, binlog 等，取出 key 来删除操作

Write Behind Caching Pattern， 只更新缓存，然后异步刷到数据库中但是，其带来的问题是，数据不是强一致性的，而且可能会丢失（我们知道 Unix/Linux 非正常关机会导致数据丢失，就是因为这个事），在软件设计上，我们基本上不可能做出一个没有缺陷的设计，就像算法设计中的时间换空间，空间换时间一个道理，有时候，强一致性和高性能，高可用和高性性是有冲突的。软件设计从来都是取舍 Trade-Off。

![见图](https://coolshell.cn/wp-content/uploads/2016/07/Write-back_with_write-allocation.png)


https://yuerblog.cc/2018/06/18/redis-mysql-sync-tips/