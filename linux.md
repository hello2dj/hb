### root 账号拥有所有权限，是删不掉的

### lsof -iTCP -sTCP:LISTEN -nP

mac 下查看进程占用端口的情况

### netstat -p | grep ''

linux 下的

### iostate 查看磁盘情况, perf, perf-stat

### 一次 docker 问题的定位

### 执行程序时找不到程序
1. 查看是否安装了程序
2. 若是sudo无法找到程序，得看PATH的指定
3. 总结就是要看能执行时 的PATH和无法执行时的PATH区别


### ln 目标是有文件创建软链会报错加-f， 若是文件夹或是软链的文件夹，则会在其目录内创建
1. -s 是创建软链接
2. -f 若是已有改软链或者是文件删除再建
3. -n 若软链到的本省就是一个软链，1. 若无-n参数，若是文件夹则会在其目录内创建一个同名软链，有n则会报错

### 文件权限
![](http://man.linuxde.net/wp-content/uploads/2013/11/chmod.gif)

```
d rwx rwx rwx 
```
1. 第一位标识改文件的类别，文件夹or软连接或其他
2. 2~4 标识文件的拥有者对该文件的权限
3. 5~6 拥有者所属用户组对该文件的权限
4. 7~8 其他用户对文件的权限


### 查看cpu数目
# 总核数 = 物理CPU个数 X 每颗物理CPU的核数 
# 总逻辑CPU数 = 物理CPU个数 X 每颗物理CPU的核数 X 超线程数

# 查看物理CPU个数
cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l

# 查看每个物理CPU中core的个数(即核数)
cat /proc/cpuinfo| grep "cpu cores"| uniq

# 查看逻辑CPU的个数
cat /proc/cpuinfo| grep "processor"| wc -l