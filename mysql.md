* count 统计所有的行数包扩null;
* count(expression) 统计列值得结果数，不包含null

### 高挂mysql 3招 https://mp.weixin.qq.com/s?__biz=MzI4NTA1MDEwNg==&mid=2650755337&idx=1&sn=6a46822acc666a55dacf34b8cbaf1db4

### mysql 优化 https://dbaplus.cn/news-11-432-1.html

### mysql 的主从可以搭建为树状机构，即从mysql的主也有可能是一个从mysql
```
m- s - s -- 
    \       \
      s      s-s
```


常见sql语句优化https://mp.weixin.qq.com/s/q4ETFySWjdzHJ-ExzRjrzQ


### mysql分片
https://blog.csdn.net/chexlong/article/details/52614719

http://www.cnblogs.com/langtianya/p/4997768.html

http://blog.51yip.com/mysql/949.html

https://www.scienjus.com/database-sharding-review/



### mysql 中的隐式转换 https://www.cnblogs.com/rollenholt/p/5442825.html
1. 当把字符串转为数字的时候，其实是从左边开始处理的。
    1. 如果字符串的第一个字符就是非数字的字符，那么转换为数字就是0
    2. 如果字符串以数字开头
        1. 如果字符串中都是数字，那么转换为数字就是整个字符串对应的数字
        2. 如果字符串中存在非数字，那么转换为的数字就是开头的那些数字对应的值

2. mysql 中true 就是1, false 就是 0

3. 数字传字符串直接 就是数字的字符串 如：123 => '123'


### mysql 优化 https://blog.devopszen.com/mysql

### mysql int(11)值溢出，最大值为2147483647;在超过的就会截断为2147483647；

### http://www.ywnds.com/?p=9337 关于show processlist

### http://www.ywnds.com/?cat=31

### https://blog.csdn.net/fdipzone/article/details/80144166
查看所有数据库容量大小
```
select 
table_schema as '数据库',
sum(table_rows) as '记录数',
sum(truncate(data_length/1024/1024, 2)) as '数据容量(MB)',
sum(truncate(index_length/1024/1024, 2)) as '索引容量(MB)'
from information_schema.tables
group by table_schema
order by sum(data_length) desc, sum(index_length) desc;
```

查看所有数据库各表容量大小
```
select 
table_schema as '数据库',
table_name as '表名',
table_rows as '记录数',
truncate(data_length/1024/1024, 2) as '数据容量(MB)',
truncate(index_length/1024/1024, 2) as '索引容量(MB)'
from information_schema.tables
order by data_length desc, index_length desc;
```