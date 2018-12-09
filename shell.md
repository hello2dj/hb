### shell 脚本
查看文件数量
```
for i in /*; do echo $i; find $i | wc -l; done
```