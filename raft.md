[一个演示 raft 的代码](http://thesecretlivesofdata.com/raft/)

### leader, follower, candidate

### log replication
1. leader 接收一个set x <- 5的命令，不提交写入log
2. 广播给各个follower, follower copy the log entry to 自己本地的log, 然后各个followers 答复同意
3. leader 确定 majority follower回答已写入log, 然后 leader apply log, 即设置 x = 5
4. leader 广播给各个followers 这个entry 可以commited
5. cluster 达到一致

### leader election
1. 有两个timeout
1.1 election timeout 选举timeout
* 每个节点上来都认为自己是follower， 然后随机选择一个timeout(150ms-300ms)之间
* 某个节点定时到时间后提升自己为candidate(候选者)， 发起一轮新的election term
* 然后将自己的term加一, 给自己投票, 发送request vote 给其他节点
* 其他节点收到request vote, 判断自己是否已经发起了request vote 若没有则投票给收到的请求指定的节点。接着重置自己的定时器（就是在开起一个election timer）, 把自己变为follower, 将term加1 or 设置为 请求中的新的term?
* 一旦一个candidate接收到了大部分节点的投票，就成为leader, 接着发送 append entries messages 给他的followers
* append entries message 作为 heartbeat message, 每个一个heartbeat timeout 发送一次。 follower收到append entries 给leader 返回一个响应。
> leader 和followers 之间是有心跳的，append entries message就是心跳信息。
* 上面的term 会保持一致，直到某一个follower节点不在接收心跳包，或者leader不在发送心跳包， 那么大家就又会变为candidate重新选举。（判断依据就是follower节点的接收心跳包的定时器超时，或者leader没有接收到某一个节点的心跳包相应数据）
* 接着就会再发起一轮leader election， 若原来有3个节点，down掉一个，只剩2个就完蛋了，没有大多数一说了啊！
>>>>>>>>>>>>>>>>
* 来看看4个节点的leader选举
* 同时有两个节点的election timer到期，则两个节点都会发起，request vote
* 那么假设两个candidate都收到了2个投票，也就是说大家都是2，那么这两个candidate就会从新变成follower开启下一轮leader election
> 这里隐藏了3个点： ####1###. 成为candidate后，就是先发一个request vote, candidate 节点不会给其他candidate节点投票，只会给自己投票，也就是说上面的那两个candidate最多收到三个投票（因为有两个，follower，还有自己）####2####. follower 收到request vote时只会给term大于自己的request vote 投票，小于的不会投票，或者已经投过票的就不会再给其他人投票，都会投反对票的。####3#### 一但一个节点收到了大多数节点的request vote，自己是leader, 好比上面的4个节点的例子，若是其中一个candidate（A）的投票都快于另一个candidate（B）节点的request vote,那么A就会收到3个节点的投票，那他就会认为自己是leader, 而另外一个B 没有收到，那就会把自己在变为follower，重启定时器？还是什么其他操作

2. log replication
* a client send a change to the leader
* leader append this change to local log then 在下一次心跳把改变发给followers
* 当leader收到大多数followers的心跳回应以后就提交这次改变，接着回应给客户端
> 细节和上面提到的一致

3. 网络分区
> 假设我们有5个节点 a,b,c,d,e; a,b 和c,d,e分区了
* 因此，我们有两个分区了，c,d,e有一个leader（e, 选举出来的）， a,b 有原来的leader b
* 假设来了两个客户端都要去修改数据c1, c2
    * c1 改变b: set 3, b发送改变给a, a,b 这里无法收到大多数的同意，2个节点1：1无法完成大多数
    * c2 改变e： set 8，e可以收到大多数，因为3个收到其他两个就可以了
* 接下来网络分区没了，合起来了
    * 原来的leader b 还要发送心跳给其他节点，但是收到了更高的election term，因此a,b 就会抛弃他们还没有提交的log, 然后去追赶新leader的log, 这个肯定是新leader发送了，append logs给a和b,要不然a和b也不知道该追赶到哪。

### candidate 状态
当一个候选人等待别人的选票时，它有可能会收到来自其他服务器发来的声明其为领导人的 AppendEntries RPC。如果这个领导人的任期（包含在它的 RPC 中）比当前候选人的当前任期要大，则候选人认为该领导人合法，并且转换自己的状态为追随者。如果在这个 RPC 中的任期小于候选人的当前任期，则候选人会拒绝此次 RPC， 继续保持候选人状态。

### append entries
follower 会把与leader 冲突的条目删除，然后复制leader的log条目
```
leader: [1] -> [1] -> [3] -> [3] -> [4] -> [5]

follower: [1] -> [1] -> [2] -> [2] -> [4]
```
如上面的follower 就会把  [2] -> [2] -> [4] 都丢弃，然后复制leader的[3] -> [3] -> [4] -> [5]

如上面的原因，因此也会出现丢失的情况，
```
leader: [1] -> [3]

follower: [1] -> [1] -> 2] -> [2] -> [4]
```
按照上面的复制方法 [1] -> [2] -> [2] -> [4]就会被丢弃，可实际情况是 他们已经被大多数节点接受了， 于是就出了问题了。

### append entries 的避免方法
避免上述问题方法是，再选举时，candidate 发出的request vote请求包含了当前自己的最新的logIndex和logTerm, 若是候选人的日志没有自己的新，那就不会给他投票

### broadcast < electionTimeout <<< MTBF
就是leader 发送心跳信息的时间要小于选举超时时间一个量级，这样可以保证那些

### 当leader 没有收到大多数的commit reply 请求时, leader没有操作，不会commit client的数据


### leader 只允许主节点提交包含当前term的日志