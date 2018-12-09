### [progressive load data use web workers](https://davidea.st/articles/firebase-bundle-size?utm_source=mybridge&utm_medium=blog&utm_campaign=read_more)

### webpack工程化实践
https://www.cnblogs.com/ihardcoder/p/5623411.html， hash, chunkHash, contentHash

### gulp是task runner, 通俗一点可以理解为打包工具
### webpack 是module工具解决的是前端模块化问题

### 工程化
1. 脚手架，减少模板代码
2. 构建
    1. API和编程规范
    2. ES6与babel
    3. CSS预编译和PostCSS
        1. css的缺陷
        2. css sprites
    4. 模块化开发(webpack)
3. 本地开发
    1. 本地服务器
    2. 动态构建
        1. webpack-dev-server
    3. mock服务
4. 部署
    1. 部署流程
    2. 代码审查
    3. 安全
    5. 静态资源缓存问题
5. 工作流
    1. 本地工作流
    2. 云平台工作流（各种部署工具，webhook, jekins）

### localStorage跨域
可以使用postMessage和iframe

### cookie跨域
jsonp, CORS跨域资源共享, window.name(iframe), nginx 反向代理

### 部署
1. 配置超长时间的本地缓存 —— 节省带宽，提高性能
2. 采用内容摘要作为缓存更新依据 —— 精确的缓存控制
3. 静态资源CDN部署 —— 优化网络请求
4. 更资源发布路径实现非覆盖式发布 —— 平滑升级

### 图片无限加载 https://mp.weixin.qq.com/s/uc6ZQx0KRZfqJoT_VzUyFg

### 滚动事件 https://www.cnblogs.com/coco1s/p/5499469.html