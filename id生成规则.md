### 背景
防止订单Id号泄露每日流水，暴露商业机密；需要对订单Id号进行相应的处理，但是订单号的生成又需要

### 满足以下条件
* 唯一性
* 语义性
* 考虑分库分表的情况能快速路由到相应的表
* 长度

### 大厂的生成策略

#	平台	rule	来源
1	大众点评	时间戳+用户标识码+随机数	大众点评订单系统分库分表实践
2	美团团购	单表自增Id * 100 + 买家Id后2位	美团团购订单系统优化记
3	淘宝	发号器Id + 买家Id后4位	淘宝在线交易数据演变
其他策略: 生产乱序码和真实的orderId关联

### [id生成规则讨论](https://www.php1.cn/Content/JieXiGeDaDianZiShangWuWangZhanDingDanHaoDeShengChengFangShi.html)

### [leaf美团leaf生成讲解](https://tech.meituan.com/MT_Leaf.html)