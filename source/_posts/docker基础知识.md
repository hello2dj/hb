---
title: docker相关的一些使用总结
date: 2018-06-01
tags:
---
 
> 随时更新

## 解决docker sudo问题

### 检查是否已有 docker 用户组

```
cat /etc/group | grep docker
```

### 将现有用户加入 docker 组

```
sudo gpasswd -a ${USER} docker
```

### 重启 docker 服务

```
sudo service docker restart
```

### 退出当前用户，重新登录

## docker 的远程镜像名字得和本地一致且以 url 为前缀

比如 docker push docker.sensoro.com/library/ai-server，那么推送的地址是 docker.sensoro.com/library/ 我们的镜像想要叫 ai-server。 但是本地在 build 时，要用 docker build ./ -t docker.sensoro.com/library/ai-server tag 必须是这个。

## docker compose

*   networks 负责创建一个网络 [见](https://docs.docker.com/compose/compose-file/)
*   sysctls 负责内核参数 [见](https://docs.docker.com/compose/compose-file/)

*   ulimits

## swarm

1.  docker swarm init
2.  根据上条命令输出执行即可

## [docker daemon api 链接](https://www.jianshu.com/p/7ba1a93e6de4)

## docker 在配置为 host 网络模式时，是不需要映射端口的，因为 docker 容器的网络（ip）就是 host（宿主）的网络 IP，并没有进行隔离

## 网络模式

1.  host 模式

    众所周知，Docker 使用了 Linux 的 Namespaces 技术来进行资源隔离，如 PID Namespace 隔离进程，Mount Namespace 隔离文件系统，Network Namespace 隔离网络等。一个 Network Namespace 提供了一份独立的网络环境，包括网卡、路由、Iptable 规则等都与其他的 Network Namespace 隔离。一个 Docker 容器一般会分配一个独立的 Network Namespace。但如果启动容器的时候使用 host 模式，那么这个容器将不会获得一个独立的 Network Namespace，而是和宿主机共用一个 Network Namespace。容器将不会虚拟出自己的网卡，配置自己的 IP 等，而是使用宿主机的 IP 和端口。例如，我们在 10.10.101.105/24 的机器上用 host 模式启动一个含有 web 应用的 Docker 容器，监听 tcp80 端口。当我们在容器中执行任何类似 ifconfig 命令查看网络环境时，看到的都是宿主机上的信息。而外界访问容器中的应用，则直接使用 10.10.101.105:80 即可，不用任何 NAT 转换，就如直接跑在宿主机中一样。但是，容器的其他方面，如文件系统、进程列表等还是和宿主机隔离的。

2.  container 模式

    在理解了 host 模式后，这个模式也就好理解了。这个模式指定新创建的容器和已经存在的一个容器共享一个 Network Namespace，而不是和宿主机共享。新创建的容器不会创建自己的网卡，配置自己的 IP，而是和一个指定的容器共享 IP、端口范围等。同样，两个容器除了网络方面，其他的如文件系统、进程列表等还是隔离的。两个容器的进程可以通过 lo 网卡设备通信。

3.  none 模式

    这个模式和前两个不同。在这种模式下，Docker 容器拥有自己的 Network Namespace，但是，并不为 Docker 容器进行任何网络配置。也就是说，这个 Docker 容器没有网卡、IP、路由等信息。需要我们自己为 Docker 容器添加网卡、配置 IP 等。

4.  bridge 模式

    bridge 模式是 Docker 默认的网络设置，此模式会为每一个容器分配 Network Namespace、设置 IP 等，并将一个主机上的 Docker 容器连接到一个虚拟网桥上。下面着重介绍一下此模式。
