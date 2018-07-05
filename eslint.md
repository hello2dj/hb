### 当我们使用mocha 或者其他测试库时，会报describe等未定义此时
如下配置，其他也是
```
env:
    mocha: true
```

### rules配置
```
rules:
    indent:
        - error
        - 4
```

### async/await 不识别
如下配置
```
parserOptions:
    ecmaVersion: 9
```