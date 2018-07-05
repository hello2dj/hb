### fork库中的master不做任何操作,本地master会又集中库自动同步
### 本地库中的master是需要靠手动同步fork库中的master分支来保持更新
### 集中库由分支与分支之间的操作，都是通过提pr操作的，fork库不存在分支与分支之间的合并操作

### 所有的开发都是从fork库中master中创建的分支，而不是其他,因此若是master不能同步更新那么开发的topic就是不干净的了。

### topic 分支是在fork库中创建，然后pull 到本地开发，然后push到fork库的topic，在提pr到集中库的master

### release 是从集中库的master中创建的，因为topic提到了集中库的master，测试也是测得集中库的master, 因此bug是从这里发现的。
### release 修改完以后push到fork库中的release，再提pr到集中库的release上，再提pr到production，上线，再合并会到集中库的master中。

### release属于bug修复分支

### hotfix是由研发经理在集中库中的production中创建的分支

# 1. 开发都是在fork库的master中获取代码
# 2. release, hotfix都是在集中库中创建，release是根据master, hotfix是根据production