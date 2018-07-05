### sql 语句就好比pipeline似的，上一个的执行结果喂给下个语句（select 语句永远在最后）
### 使用汉语时需要使用''单引号
### select 不但可以使用列名还可以写常量比如“商品”等字符串。
```
select "商品" as 'name', id from product;
```

### 在 SELECT 语句中使用 DISTINCT 可以删除重复行, DISTINCT 关键字只能用在第一个列名之前
```
select distinct product_id from product;
or 
select distinct product_id, purchase_price from product;
// 这是会按照product_id, 和purchase_price进行合并去重的
// 因此如下写是错误的
select product_id, distinct purchase_price from product; // error
```

### WHERE 子句要紧跟在 FROM 子句之后。

### 单行注释
书写在“--”之后，只能写在同一行
### 多行注释
/* */类c

### select 字句也可以使用算数运算，比如2倍价钱， 常用的4则运算，当查询结果是null时，那所有的运算结果都是null
```
select purchase_price * 2 as 'purchase_price2' from product;
```

### 运算就是这样以'行'为单位执行的, 查出一行执行一行

### SELECT 子句中可以使用常数或者表达式

### 比较运算符在where字句中的有 = 相等， <> 不等 >=, <=, <, >

### WHERE 子句的条件表达式中也可以使用计算表达式 比如查询卖出价比买入价多600的商品,相当与在找单品利润大于600的
```
select product_id, product_name, product_type from product
where sale_price - purchase_price > 600
```

### insert into table_name values ('field', ...), ('field_value', ...) ...;插入多行数据

### 字符串类型的数据原则上按照字典顺序进行排序，不能与数字的大小顺序混淆说白了，就是ascii的大小

### 对null 不能使用比较运算符，sql 有专门的is null 和is not null来判断
```
select product_name from product
where purchase_price is null;
```


### 逻辑运算符，not, and, or
* 售价不大于1000的
```
select product_name from product
where not sale_price > 1000;
```
* and 条件都满足, 售价大于100的衣服
```
select product_name from product
where sale_price > 100 and product_type = '衣服'
```

### AND 运算符的优先级高于 OR 运算符。想要优先执行 OR 运算符时可以使用括号

### count聚合函数应用在具体列时，不会对null进行计算就是不包括null
COUNT 函数的结果根据参数的不同而不同。COUNT(*) 会得到包含 NULL 的数据 行数，而 COUNT(< 列名 >) 会得到 NULL 之外的数据行数。
```
select count(purchase_price) from product;
// 假设一共有8行2行purchase_price是null，则上面的输出是6
```

### 当select字句中使用了聚合函数后，则整个select语句中只能使用聚合函数，或者常量而不允许在直接使用列名了。

### 所有的聚合函数，如果以列名为参数，那么在计算之前就已经把 NULL 排除在外了，聚合函数会将 NULL 排除在外。但 COUNT(*) 例外，并不会排除 NULL。
### 由于有上述原因所以在我们计算平均值时avg，也会出问题，他会把null也排除，那么平均值得分母也就少了。

### MAX/MIN 函数几乎适用于所有数据类型的列。SUM/AVG 函数只适用于数值类型的列。

### 想要计算值的种类时，可以在 COUNT 函数的参数中使用 DISTINCT。
```
select count (distinct product_type) form product
```

### 所有的聚合函数都可以使用 DISTINCT, 在聚合函数的参数中使用 DISTINCT，可以删除重复数据。

### 使用聚合函数和GROUP BY子句时需要注意以下4点。 GROUP BY子句也和SELECT子句一样， 可以通过逗号分隔指定多列。GROUP BY 就像是切分表的一把刀。
> 此外，GROUP BY子句的书写位置也有严格要求，一定要写在 FROM 语句之后(如果有 WHERE 子句的话需要写在 WHERE 子句之后)。

1. 聚合函数只能写在SELECT子句之中
2. GROUP BY子句中不能使用SELECT子句中列的别名 
3. GROUP BY子句的聚合结果是无序的
4. WHERE子句中不能使用聚合函数

### 聚合键中包含 NULL 时，在结果中会以“不确定”行(空行)的形式表现出来。
### GROUP BY和WHERE并用时SELECT语句的执行顺序
FROM → WHERE → GROUP BY → SELECT

### 使用聚合函数与group by字句时常见的错误
1. 在select 语句中写了多余的列
在使用了聚合函数，以及group by字句时，select字句中的列是有严格限制的
    * 常数
    * 聚合函数
    * group by 字句中指定的列名（就是聚合键）
> 使用 GROUP BY 子句时，SELECT 子句中不能出现聚合键之外的列名。
2. 使用 GROUP BY 子句时，SELECT 子句中不能出现聚合键之外的列名。
3. GROUP BY子句的结果能排序吗? 答案随机的， GROUP BY 子句结果的显示是无序的。
4. 在 WHERE 子句中使用聚合函数, where 字句中是不能使用字句的。

### 只有SELECT子句和HAVING子句(以及ORDER BY子句)中能够使用聚合函数， where是不对分组数据处理的where处理的是单行数据。

### 对聚合结果操作，指定条件having 子句
### HAVING 子句要写在 GROUP BY 子句之后
### having 子句与select子句类似只能包含以下
1. 常数
2. 聚合函数
3. group by 字句中指定的列名（就是聚合键）
```
select product_type, count(*) from product
group by product_type
having product_name = '圆柱笔'
//  error 错误
```

### 在where字句中和having字句中都可以出现的条件是聚合键所对应的条件
* WHERE 子句 = 指定行所对应的条件
* HAVING 子句 = 指定组所对应的条件
但应当符合上述的要求，聚合键的条件写在where子句会更好

### where字句中的条件执行速度会比having子句的执行性能好一些

### ORDER BY 子句通常写在 SELECT 语句的末尾。

### 排序键中包含 NULL 时，会在开头或末尾进行汇总。

### 带有order by 子句的整体执行流程
FROM→WHERE→GROUP BY→HAVING→SELECT→ORDER BY


### 在 ORDER BY 子句中可以使用 SELECT 子句中定义的别名。


### 在 ORDER BY 子句中可以使用 SELECT 子句中未使用的列和聚合函数

### 在 ORDER BY 子句中可以使用 SELECT 子句中使用的列和聚合函数

### 定义视图时不要使用ORDER BY子句

# 子查询
###  一言以蔽之，子查询就是一次性视图(SELECT语句)。与视图不同，子查 询在 SELECT 语句执行完毕之后就会消失

### 子查询作为内层查询会首先执行。

# 标量子查询
### 标量子查询就是返回单一值的子查询。
标量子查询有一个特殊的限制，那就是必须而且只能返回 1 行 1
列的结果，也就是返回表中某一行的某一列的值，例如“10”或者“东京都” 这样的值。

### 标量子查询的书写位置并不仅仅局限于 WHERE 子句中，通常任何可 以使用单一值的位置都可以使用。也就是说，能够使用常数或者列名的 地方，无论是 SELECT 子句、GROUP BY 子句、HAVING 子句，还是 ORDER BY子句，几乎所有的地方都可以使用。

### 使用标量子查询时的注意事项，那就是该子查询 绝对不能返回多行结果

### where 子句是一行一行处理数据的，所以不能使用聚合函数，WHERE 子句中不能使用聚合函数

# 关联子查询是指内部自查使用了外包部查询的变量
###  在细分的组内进行比较时，需要使用关联子查询。

### 关联子查询也是用来对集合进行切分的 作用group by 类似

### 别名的使用’内部可以看到外部，而外部看不到内部‘

# select 子句中若是出现了，聚合函数则其他列就不能使用，除非这个列是聚合件

## exist 谓词
### 通常指定关联子查询作为 EXIST 的参数

## \dt 显示表详细信息


## 关于数据库join的问题
1. 数据库join
2. service应用层join
https://www.zhihu.com/question/68258877

## 重代码和重sql的两种编码方式
https://zhuanlan.zhihu.com/p/36585028 
