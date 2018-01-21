### mage 目录结构
mygame/
  lib/
    modules/
      players/    -- the name of our module
        index.js  -- the entry point of the module
        usercommands/
          login.js  -- the user command we use to login
* modules下面可以有多个，但是command是必须交usercommands的, 在indexjs文件中可以注册module级别的方法

### modules 内置的modules有些事需要配置的，比如auth
```
module:
    auth:
        // this should point to the topic you created
        topic: auth
        // configure how user passwords are stored, the values below are the
        // recommended default, see the module's README.md for more details
        // about available hash types
        hash:
            type: pbkdf2
            algorithm: sha256
            iterations: 10000
```

### http的请求数据是string,以换行符作为分隔，且第一个得是数组，是用来作为header的
```
[]
{"username": "test","password": "secret"}
```
不能有其他的数据，他是以"\n"进行split,然后json.parse的
### mage返回的id是uuid

### auth module 负责，注册登录，state负责一次请求的上下文，有state,req,res等

### 再state中设置acl, 再方法上就可以校验了

### Archivist来作为数据存储的抽象，topic就对应表