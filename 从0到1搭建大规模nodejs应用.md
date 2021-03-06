一切从需求开始 首屏优先

* ### 为什么使用框架 
  * #### 效率 
  * #### 安全
  * #### 成本

* ### 框架选择
  * #### 尽量不要有升级的坑
  * #### 有人维护
  * #### 最好文档全
  * #### 使用量多
  * #### 团队有人能carry

* ### 私有npm库 cnpm
  * #### 安全 
  * #### 网络 
  * #### 速度 
  * #### 稳定 
  * #### 内部模块

* ### 内部系统打通
  * #### 文件系统
  * #### 权限系统
  * #### 缓存系统
  * #### 监控系统

* ### 部署(自动化部署优点)
  * #### 权限
  * #### 规范
  * #### 策略
  * #### 查询状态
  * #### 自动切换
  * #### 多机并发
  * #### 智能监控
  * #### 快速回滚

  * #### 主要环节
    * ##### 文件拉取
    * ##### 执行发布前置脚本
    * ##### 环境设置
    * ##### 文件分发
    * ##### 执行发布后置脚本

  * #### 主要技术
    * ##### web操作
    * ##### git管理
    * ##### rsync同步文件

  * #### 发布要谨慎
    * ##### 分组部署
    * ##### 灰度发布
    * ##### 发布完检测
    * ##### 周五不发布

* ### 监控
  * #### 需求
    * ##### 掌握服务器状态
    * ##### 及时发送警告
    * ##### AB test
    * ##### 故障定位
  * #### 监控简介
    * ##### 数据收集（cpu, 硬盘，网络）
    * ##### 数据传输 发送至接收模块 日志分类， 格式处理， 存储
    * ##### 数据存储 redis, mysql Hbase
    * ##### 数据分析 处理 分析 报警
    * ##### 前台展示 查询， 展示
  * #### tips
    * ##### 美好一天从看监控开始
    * ##### 熟悉应用负载
    * ##### 掌握应用的临界值
    * ##### 掌握流量峰谷的规律
    * ##### 区分日志与警告
    * ##### 对警告保持敏感(关闭不处理的报警使得对警告敏感，防止频繁接受报警而产生对报警信息的不敏感)

* ### 增强代码健壮性
  * #### 输入检验
  * ##### 增加uncaughtExeption
  * ##### try-catch
  * ##### 及时清理代码
  * ##### 单元测试
* ### HAProxy + Nginx + pm2 + cluster，部署以及高可用
  * #### HAProxy 最前层负载均衡
  * #### 多机房部署
  * #### 业务无单节点（不要某一个业务只有一个机器处理）
  * #### 返回码的监控
  * #### 单机故障处理与剔除
  * #### 业务节点的无状态化
  * #### 统一错误页
  * #### 充分利用流量
  * #### 本地内存缓存（LRU）&redis缓存
  * #### CDN兜底
  * #### 高负载自动保护（页面渲染是CPU密集，可以设置阈值，若达到阈值就重定向请求到CDN,自动保护系统）
  * #### 手动按比例切流量
  * #### 偶发性错误，自动跳转到CDN
  * #### nginx转发， 若node服务整个都挂了，可以在nginx位置自动重定向（有服务注册，当node服务都挂了，那网关层可定知道整个node服务挂了，此时就可以请求其他可用的了）
  * #### 域名切换，若机房都挂了，则切换整个服务域名，就是整个服务切换（直接启用备用域名服务）
  * #### 应急响应预案
  * #### 单元测试
  * #### 回归测试
  * #### 压力测试（要有压测计划）
    * ##### 掌握系统的极限
    * ##### 流量生成尽可能模拟真实用户
    * ##### 环境尽可能与正式环境一致（机器配置，所在机房）
    * ##### 全链路压测（应用不是孤立的，带着上下游一起压测）
  * #### 浏览器 -> 用户访问 -> CDN -> 回源 -> 备份机 ->(定时备份) -> 页面渲染服务(总结一句就是多备份)
  
* ### 安全
  * #### http头部安全设置 [helmet](https://helmetjs.github.io/)
  * #### 敏感信息外泄
    * ##### 内部api不要公布
    * ##### ...
  * #### 谨慎操作cookie
  * #### 防xss攻击
  * #### 确保依赖库安全
    * #### 本身没有安全漏洞
    * #### 升级安全
    * #### ...

* ### 开发及打包工具
  * #### node-app-config
  * #### 重复工作自动化

* ### tips
  * #### 先抗住再优化
  * #### 用数据说话
  * #### 持续优化
  * #### 缓存的坑