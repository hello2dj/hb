### 检查是否已有docker用户组
```
cat /etc/group | grep docker
```
### 将现有用户加入docker 组
```
sudo gpasswd -a ${USER} docker
```
### 重启docker服务
```
sudo service docker restart
```

### 退出当前用户，重新登录

### docker的远程镜像名字得和本地一致且以url为前缀
比如docker push docker.sensoro.com/library/ai-server，那么推送的地址是docker.sensoro.com/library/ 我们的镜像想要叫ai-server。 但是本地在build时，要用docker build ./ -t docker.sensoro.com/library/ai-server tag必须是这个。

### docker compose 
* networks 负责创建一个网络 [见](https://docs.docker.com/compose/compose-file/)
* sysctls 负责内核参数 [见](https://docs.docker.com/compose/compose-file/)

* ulimits

### swarm 
1. docker swarm init
2. 根据上条命令输出执行即可