### koa也可以有plugin
```
app.context.pluginName = plugin
```
那么就可以通过ctx.pluginName来访问

原理是每次请求来的ctx都是以app.context为原型创建的， ctx = Object.create(app.context)

### koa router params参数的解析是由koa-router来实现的