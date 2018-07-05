
---
title: egg入门
date: 2018-06-05 10:05:27
tags:
    - node
---

# egg是基于koa的企业级web框架

## egg的使用感觉
1. 约定大于配置
2. koa中间件基本无痛使用
3. 可以总结业务通用基础服务（生产基础框架），而不用费心费力再重新写插件啥的
> app -> plugins -> framework (统统一套约定)

> [照着官网总结了一部分详细的还得去官网查询](http://eggjs.org/zh-cn)

### 官方文档进行搭建
1. npm i egg-init -g
2. egg-init egg-example --type=simple
> 关于 type: egg 本身的集成包括了很多的东西，从一个基本的webserver走起，沉淀出plugin, 最后是framework 于是type就很明显了webserver== simple...

### 整体结构 MVC

### 文件夹约定
所有相应的文件必须放在指定目录中

```
egg-project
├── package.json
├── app.js (可选)
├── agent.js (可选)
├── app
|   ├── router.js
│   ├── controller
│   |   └── home.js
│   ├── service (可选)
│   |   └── user.js
│   ├── middleware (可选)
│   |   └── response_time.js
│   ├── schedule (可选)
│   |   └── my_task.js
│   ├── public (可选)
│   |   └── reset.css
│   ├── view (可选)
│   |   └── home.tpl
│   └── extend (可选)
│       ├── helper.js (可选)
│       ├── request.js (可选)
│       ├── response.js (可选)
│       ├── context.js (可选)
│       ├── application.js (可选)
│       └── agent.js (可选)
├── config
|   ├── plugin.js
|   ├── config.default.js
│   ├── config.prod.js
|   ├── config.test.js (可选)
|   ├── config.local.js (可选)
|   └── config.unittest.js (可选)
└── test
    ├── middleware
    |   └── response_time.test.js
    └── controller
        └── home.test.js
```

### [内置对象](http://eggjs.org/zh-cn/basics/objects.html)
1. Application (ctx.app, this.app)
2. Response/Request (ctx.request, ctx.response)
3. Context (this.ctx)
4. Service (this.service)`
5. Helper (this.ctx.helper)
6. Logger (ctx.logger, this.logger（应用打印log）) 还有coreLogger(框架打印log)
7. Config (this.config, app.config)
8. Subscription 订阅发布模型的规范基类

### controller
```
// app/controller/post.js
const Controller = require('egg').Controller;
class PostController extends Controller {
  async create() {
    const { ctx, service } = this;
    const createRule = {
      title: { type: 'string' },
      content: { type: 'string' },
    };
    // 校验参数
    ctx.validate(createRule);
    // 组装参数
    const author = ctx.session.userId;
    const req = Object.assign(ctx.request.body, { author });
    // 调用 Service 进行业务处理
    const res = await service.post.create(req);
    // 设置响应内容和响应状态码
    ctx.body = { id: res.id };
    ctx.status = 201;
  }
}
module.exports = PostController;
```

* this.ctx: 当前请求的上下文 Context 对象的实例，通过它我们可以拿到框架封装好的处理当前请求的各种便捷属性和方法。

* this.app: 当前应用 Application 对象的实例，通过它我们可以拿到框架提供的全局对象和方法。

* this.service：应用定义的 Service，通过它我们可以访问到抽象出的业务层，等价于 this.ctx.service 。

* this.config：应用运行时的配置项。

* this.logger：logger 对象，上面有四个方法（debug，info，warn，error），分别代表打印四个不同级别的日志，使用方法和效果与 context logger 中介绍的一样，但是通过这个 logger 对象记录的日志，在日志前面会加上打印该日志的文件路径，以便快速定位日志打印位置。

### service
```
// app/service/user.js
const Service = require('egg').Service;

class UserService extends Service {
  async find(uid) {
    const user = await this.ctx.db.query('select * from user where uid = ?', uid);
    return user;
  }
}

module.exports = UserService;

```
this对象和controller中能拿到的实例是相同的, service的使用是依据文件名来查找的，this.service.filename.method

service 在使用构造函数时需要传递ctx参数

```
class User extends app.Service {
  // 默认不需要提供构造函数。
  // constructor(ctx) {
  //   super(ctx); 如果需要在构造函数做一些处理，一定要有这句话，才能保证后面 `this.ctx`的使用。
  //   // 就可以直接通过 this.ctx 获取 ctx 了
  //   // 还可以直接通过 this.app 获取 app 了
  // }
}
```

### router
[http://eggjs.org/zh-cn/basics/router.html](http://eggjs.org/zh-cn/basics/router.html)

基本使用
```
// app/router.js
module.exports = app => {
  const { router, controller } = app;
  router.get('/user/:id', controller.user.info);
};
```

### middlerware
基本示例
```
const isJSON = require('koa-is-json');
const zlib = require('zlib');

async function gzip(ctx, next) {
  await next();

  // 后续中间件执行完成后将响应体转换成 gzip
  let body = ctx.body;
  if (!body) return;
  if (isJSON(body)) body = JSON.stringify(body);

  // 设置 gzip body，修正响应头
  const stream = zlib.createGzip();
  stream.end(body);
  ctx.body = stream;
  ctx.set('Content-Encoding', 'gzip');
}
```
使用
1. 全局路由使用
module.exports = {
  // 配置需要的中间件，数组顺序即为中间件的加载顺序
  middleware: [ 'gzip' ],

  // 配置 gzip 中间件的配置
  gzip: {
    threshold: 1024, // 小于 1k 的响应体不压缩
  },
};
2. 某个路由使用
```
module.exports = app => {
  const gzip = app.middleware.gzip({ threshold: 1024 });
  app.router.get('/needgzip', gzip, app.controller.handler);
};
```

3. 框架默认中间件使用
例如，想把某个自定义中间件放到所有中间件之前执行
```
// app.js
module.exports = app => {
  // 在中间件最前面统计请求时间
  app.config.coreMiddleware.unshift('report');
};
```
上面的顺序可随意互换

4. [koa中间件基本可无痛使用](http://eggjs.org/zh-cn/basics/middleware.html#%E4%BD%BF%E7%94%A8-koa-%E7%9A%84%E4%B8%AD%E9%97%B4%E4%BB%B6)

通用配置（无论是框架还是自定义中间件都可以）

1. enable：控制中间件是否开启。
2. match：设置只有符合某些规则的请求才会经过这个中间件。
3. ignore：设置符合某些规则的请求不经过这个中间件。

### 插件（就是个mini的egg应用，只是没有路由和controller罢了）
[http://eggjs.org/zh-cn/advanced/plugin.html](http://eggjs.org/zh-cn/advanced/plugin.html)

可以把我们的通用逻辑沉淀为插件

插件配置
示例
```
exports.mongoose = {
    enable: true,
    package: 'egg-mongoose',
};

exports.session = false;

exports.cors = {
    enable: true,
    package: 'egg-cors',
};

exports.jwt = {
    enable: true,
    package: 'egg-jwt'
};

exports.routerPlus = {
    enable: true,
    package: 'egg-router-plus',
};

```
### 定时任务
[http://eggjs.org/zh-cn/basics/schedule.html](http://eggjs.org/zh-cn/basics/schedule.html)

### 框架扩展
1. context
2. application
3. response
4. request
5. helper
扩展就是说可以通过context等实例直接访问到
例如，我们要增加一个 ctx.foo() 方法：
```
// app/extend/context.js
module.exports = {
  foo(param) {
    // this 就是 ctx 对象，在其中可以调用 ctx 上的其他方法，或访问属性
  },
};
```
特性根据环境进行框架扩展
比如unittest环境app实例有mockXX方法
```
// app/extend/application.unittest.js
module.exports = {
  mockXX(k, v) {
  }
};
```

### 启动自定义(可以执行一些异步初始化的工作)
```
module.exports = app => {
  app.beforeStart(async () => {
    // 应用会等待这个函数执行完成才启动
    app.cities = await app.curl('http://example.com/city.json', {
      method: 'GET',
      dataType: 'json',
    });

    // 也可以通过以下方式来调用 Service
    // const ctx = app.createAnonymousContext();
    // app.cities = await ctx.service.cities.load();
  });
};
```

### 配置
基本默认
```
config
|- config.default.js
|- config.test.js
|- config.prod.js
|- config.unittest.js
`- config.local.js
```
配置加载顺序
-> 插件 config.default.js
-> 框架 config.default.js
-> 应用 config.default.js
-> 插件 config.prod.js
-> 框架 config.prod.js
-> 应用 config.prod.js

配置示例
```
module.exports = appInfo => {
    appInfo;
    const config = exports = {};

    // add your config here
    config.middleware = ['errorhandler', 'auth'];
    config.auth = {
        ignore: []
    };

    // mongoose
    config.mongoose = {
        url: 'mongodb://localhost:27017/trace-to-source',
        options: {},
    };

    // multipart set
    config.multipart = {
        fileSize: '30mb',
        whitelist: [
            '.xlsx',
            '.jpg',
            '.png',
            '.zip',
        ],
    };

    config.security = { // 关闭csrf安全防范(关闭后post delete put请求可以不携带csrf-token)
        domainWhiteList: ['http://47.104.6.246:8090', 'http://localhost:8000', 'https://localhost:8000'],
        csrf: {
            enable: false,
        },
    };

    // cors
    config.cors = {
        credentials: true,
        allowMethods: 'GET,HEAD,PUT,POST,DELETE,PATCH,OPTIONS',
    };

    // logger
    config.logger = {
        buffer: true, // 文件写入时缓存 https://github.com/eggjs/egg-logger/blob/master/lib/egg/loggers.js#L44
        maxDays: 0,
    };

    config.jwt = {
        secret: 'lksjdhglaiejf'
    };

    return config;
};
```

### 运行环境
1. 通过 config/env 文件指定，该文件的内容就是运行环境，如 prod。一般通过构建工具来生成这个文件。
2. 通过 EGG_SERVER_ENV 环境变量指定。
一般使用 EGG_SERVER_ENV指定

自定义环境

比如，要为开发流程增加集成测试环境 SIT。将 EGG_SERVER_ENV 设置成 sit（并建议设置 NODE_ENV = production），启动时会加载 config/config.sit.js，运行环境变量 app.config.env 会被设置成 sit

应用内获取运行环境

框架提供了变量 app.config.env 来表示应用当前的运行环境。

### [测试](http://eggjs.org/zh-cn/core/unittest.html)
ctx测试
```
const assert = require('assert');
const mock = require('egg-mock');

describe('test/controller/home.test.js', () => {
  let app;
  before(() => {
    // 创建当前应用的 app 实例
    app = mock.app();
    // 等待 app 启动成功，才能执行测试用例
    return app.ready();
  });
  it('should mock ctx.user', () => {
        const ctx = app.mockContext({
                user: {
                name: 'fengmk2',
                },
            });
        assert(ctx.user);
        assert(ctx.user.name === 'fengmk2');
    });
});
```
http请求测试(controller 测试)
```
// 使用 async
it('should redirect', async () => {
  await app.httpRequest()
    .get('/')
    .expect(302);
});
```

service 测试
```
describe('get()', () => {
  it('should get exists user', async () => {
    // 创建 ctx
    const ctx = app.mockContext();
    // 通过 ctx 访问到 service.user
    const user = await ctx.service.user.get('fengmk2');
    assert(user);
    assert(user.name === 'fengmk2');
  });

  it('should get null when user not exists', async () => {
    const ctx = app.mockContext();
    const user = await ctx.service.user.get('fengmk1');
    assert(!user);
  });
});
```
[更多测试](http://eggjs.org/zh-cn/core/unittest.html)

### 多进程架构

```
                +--------+          +-------+
                | Master |<-------->| Agent |
                +--------+          +-------+
                ^   ^    ^
               /    |     \
             /      |       \
           /        |         \
         v          v          v
+----------+   +----------+   +----------+
| Worker 1 |   | Worker 2 |   | Worker 3 |
+----------+   +----------+   +----------+
```

启动顺序

```
+---------+           +---------+          +---------+
|  Master |           |  Agent  |          |  Worker |
+---------+           +----+----+          +----+----+
     |      fork agent     |                    |
     +-------------------->|                    |
     |      agent ready    |                    |
     |<--------------------+                    |
     |                     |     fork worker    |
     +----------------------------------------->|
     |     worker ready    |                    |
     |<-----------------------------------------+
     |      Egg ready      |                    |
     +-------------------->|                    |
     |      Egg ready      |                    |
     +----------------------------------------->|
```

消息发送

```
广播消息： agent => all workers
                  +--------+          +-------+
                  | Master |<---------| Agent |
                  +--------+          +-------+
                 /    |     \
                /     |      \
               /      |       \
              /       |        \
             v        v         v
  +----------+   +----------+   +----------+
  | Worker 1 |   | Worker 2 |   | Worker 3 |
  +----------+   +----------+   +----------+

指定接收方： one worker => another worker
                  +--------+          +-------+
                  | Master |----------| Agent |
                  +--------+          +-------+
                 ^    |
     send to    /     |
    worker 2   /      |
              /       |
             /        v
  +----------+   +----------+   +----------+
  | Worker 1 |   | Worker 2 |   | Worker 3 |
  +----------+   +----------+   +----------+
```

### egg-logger 使用
  * 自定义transport时一定要传递level参数
  ```
  class RemoteTransport extends Transport {
    // level 必传
    constructor({ level, app }) {
      super({ level });
      this._level = level;
      this._app = app;
    }
  }
  ```
  * 自定logger在配置文件中必须传file参数, 必须在customLogger下面配置
  ```
  config.customLogger = {
    remoteLogger: {
      file: path.join(appInfo.root, 'logs/remote.log'),
    },
  };
  ```
  * 所有logger的错误error方法都被errorLogger的log方法劫持了，所以当我们使用自定义logger的error方法打印时其实是被errorLogger输出了，
  所以若是自己配置的自定义logger中有其他处理使用error时是不会被触发的

  原因见下
  ```
  /**
   * Add a logger
   * @param {String} name - logger name
   * @param {Logger} logger - Logger instance
   */
  set(name, logger) {
    if (this.has(name)) {
      return;
    }

    // redirect ERROR log to errorLogger, except errorLogger itself
    if (name !== 'errorLogger') {
      logger.redirect('error', this.errorLogger);
    }
    this[name] = logger;
    super.set(name, logger);
  }
  ```
### hack
1. egg-mongoose 里面挂载在app上的mongoose是个conn对象，base才是mongoose实例
2. egg 里面的hack, app下面有个serviceClasses属性里面是所有的service的构造函数，所以在拿不到service实例时，
可以自己new一个，切记要传入ctx对象，可以通过app.createAnonymousContext创建一个匿名ctx
