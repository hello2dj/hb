### porto buffer
1. option: 用来指定一些的特定处理，可以在proto文件顶层使用，字段上面使用，各个地方都能用， 比如
    * option java_package = "com.example.foo"; 指定生成的java 包
    * packed 如果在一个repeated 的字段上标明，则会使用一种更紧凑的编码方式
    ```
    repeated int32 samples = 4 [packed=true];
    ```



access_token:08560ed0833b523bc199f0505b933a4e
session_key:08560ed0833b523bc199f0505b933a4e
app_key:17JuniorStu
sig:e3e3d4fc996ff83abbca1501cec51ba3