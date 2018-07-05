### https://github.com/fastify/fastify/issues/805

关于处理stream的

### fastify 的hooks中不予许自己调用reply.send,等方法，只能reply.res.end（node自己的response的方法集）

### setErrHandler增加全局错误处理

### fastify的register会做一个封装，如下，外层的和register内部是隔离的意味着你在内层设置的属性外层路由是访问不到了，一种继承关系，内部可以访问外部，外部访问不到内部。

### fastify的middleware没有封装send, 等方法，如果你要是非得中途返回数据就得用reply下面的res（node的response来手动处理了）