---
title: mirror-git
date: 2018-03-05 22:00:59
tags:
    - git mirror
---

#### 镜像代码库的方法

我希望这篇文章能够清晰的描述一下如何镜像 git 仓库---即我有一个 github 仓库 example.git, 还有一个 sourceforge 仓库 example.git, 我希望这两个仓库的代码可以镜像同步，就是我无论更新哪一个都会同步到另一个仓库上去。

##### 建立一个本地的仓库，并且设置 upstream(就是添加 remote)。重点：这得是个 bare 仓库

```
cd /srv/gitsync
git clone --bare git@github.com:[account]/[repository].git
mv [repository].git [repository]
```

接下来，我们删除 origin 'remote'，并且配置两个远程仓库为 upstream 就是添加 remote,看代码更详细：

```
cd [repository]
git remote remove origin
git remote add github  git@github.com:[account]/[repository].git
git remote add sourceforge  ssh://[account]@git.code.sf.net/p/[repository]/code
```

镜像意味着无论哪个仓库有更新都会同步到另外一个仓库上去。因此我们应当有个 webhook 来触发我们镜像脚本。每个代码管理都会提供这样的 hook 的。

##### 镜像脚本我们使用如下的代码来实现真正的同步逻辑。

```
#!/bin/bash

function sync_repo {
    cd /srv/gitsync
    cd $1
    echo $1
    # fetch all known remotes 获取所有remote的代码更新
    git fetch --all -p
    # 我们可以看同步就在下面进行
    # push branches from sourceforge to github and via versa. 先同步sourceforge 到github. 注意 push的目的remote是github, 源remote是在refs/remote中指定的sourcefore
    git push github "refs/remotes/sourceforge/*:refs/heads/*"
    # 同理推送 github上的代码更新到sourceforge
    git push sourceforge "refs/remotes/github/*:refs/heads/*"
}

# 处理参数，进入指定的bare仓库进行推送
cd /srv/gitsync
if [ "$1" == "" ]; then
    # no command line parameters, print help message
    echo "gitsync [report]|--all"
elif [ "$1" == "--all" ]; then
    # "--all": for all known repositories
    for D in *; do
    if [ -d "${D}" ]; then
            sync_repo $D
            cd /srv/gitsync
    fi
    done
elif [ -d "$1" ]; then
    # sync only the specified repository
    sync_repo $1
else echo "gitsync [report]|--all"
fi
```

我们可以看到在 sync_repo 函数中我们把 github 和 sourceforge 进行了互相推送。当然你还可以进行更多的映射。

就像我们上面讨论的，我门使用 webhooks 来触发我们的 mirror 行为。可以使用如下的 php 脚本

```
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    die('POST required');
}

if (!isset($_REQUEST['repository'])) {
    die('repository not specified');
}

// validate repository name to prevent injection and traversing attacks
$repo = $_REQUEST['repository'];
if (!preg_match('/^[a-zA-Z0-9]+$/', $repo)) {
    die('invalid repository name');
}

header('HTTP/1.0 204 Found');
system('sudo -Hu gitsync /usr/local/bin/gitsync '.$repo);
```

##### 安全

*   我们必须的校验仓库名称，以防止 shell 命令注入，git 命令注入，以及目录遍历等非法行为。

*   上面的代码使用了 sudo 的权限，显然 webserver 不应该拥有可访问仓库的权限。

*   大部分代码管理都支持 webhook 验证，但是我么上面的例子没有使用验证，那意味着任何人都可以触发我们的同步行为。

##### 删除分支这里还没有好的方法可以让我们同步删除分支的行为。但是我们还是通过我们脚本以 push 的方法来进行删除。且看下面的方法

```
git branch -d [branchName]
git push github --delete [branchName]
git push sourceforge --delete [branchName]
```

虽然麻烦但是我们还是实现了：)。
