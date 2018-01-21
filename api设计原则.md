api 设计原则http://www.oschina.net/translate/secrets-of-awesome-javascript-api-design

* 简单
简单是指什么是我们一看就能知道这个api是干啥的甚至都能让人猜到的实现是什么。比如getElementById, 一看就知道是通过id来获取元素。
* 和谐一致（统一）
统一和协调的目的是让API新手感觉熟悉和舒服。通过虽然功能不同，但是语法相同或相似，使API变得熟悉，大大减轻了开发者使用新工具的负担。简单点儿说就是api的参数类型种类尽量一致，比如分页使用page和offset，那么就应当保持所有的分页或者或者无论是啥样的分页都是page和offset。再比如api语法一致和谐好比dom操作，getElementById, or getElementsByTag, 看起来就很一致的api
* 平衡
在色彩搭配里来说就是色彩之间的平衡不会太过于突兀，那放到API里来说，我不太理解。
一个系统的api看起来应当是和谐，这里的和谐是指api与api之间不会有太大的冲突，不会有太多的冗余，看起来也应当不会是一个奇形怪状就是说东一个西一个完全靠随机添加。
* 相称性
是指api的表面与他的能力范围相匹配，我觉得更是单一职责，符合单一职责的api应该就是相称的。好比getElementById,但是你却返回了一组元素，就是不相称的
* 重点突出
一套api中应当有自己的核心对象，应当适当的突出好比jquery中就是$('.a')选中的对象。这个很好理解，好比是canvas api,那么canvas对象就是重点。