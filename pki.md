## PKI

### CA 自建 ca

https://www.jianshu.com/p/79c284e826fa
https://www.barretlee.com/blog/2016/04/24/detail-about-ca-and-certs/

1.  生成秘钥(可选，因为后面的步骤也可以自动生成秘钥)
    openssl genrsa

2.  生成 CA 证书请求
    openssl req [-key 使用已生成的秘钥][-keyout 未声明秘钥需自动生成]

3.  自签发 ca 根证书
    openssl ca -selfsign（需使用上述的请求证书）

4.  2,3 两步可以合在一起
    openssl req -new -x509(关键是-x509 生成 509 的根证书)

签发证书也是上述的步骤只是签发证书时不需要-selfsign 选项

### 证书发放

向 CA 请求证书时需要先生成一个 CSR(certificate signing request), 证书签发的请求。包括一些申请者的信息，申请者的公钥，还有 Distinguished Name(专有名称类似于发放者的唯一标示)用来标识证书时发给谁的。

### pem 代表 Privacy-enhanced Electronic Mail 一种文件格式 base64 编码显示

### CA DN distinguished name

*   .cer/.crt 是用于存放证书，它是 2 进制形式存放的，不含私钥。
*   X.509 DER 编码(ASCII)的后缀是： .DER .CER .CRT
*   X.509 PAM 编码(Base64)的后缀是： .PEM .CER .CRT
*   .pem 跟 crt/cer 的区别是它以 Ascii 来表示

### CSR 文件 包含了公钥和标识名称（Distinguished Name）

### [intermediate ca](What is an intermediate certificate?)

### [证书链](what-is-ssl-certificate-chain)

root ca -> intermediate ca -> end user

### [数字证书](http://www.ruanyifeng.com/blog/2011/08/what_is_a_digital_signature.html)

### tsl 认证

*   单向认证时，客户端需要有 server 的根证书来验证 server 的证书是否 ok
*   双向时，服务端也得有客户端的根证书来验证 client 的证书是否 ok

### 私钥使用来解密公钥加密的数据，证书用来验证身份的，而证书又包含公钥，因此证书和私钥永远是一对儿。

流程就是把证书发给对方，对方验证证书是否 ok，然后取出证书中的公钥，用公钥加密数据然后发回来，此时再用私钥解密即可获得数据。
