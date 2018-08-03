---
title: hyperledger-fabric基础
date: 2018-04-10
tags:
    - fabric
---

<!-- more -->

### [流程问题](http://rdcqii.hundsun.com/portal/article/661.html)

### [关于使用 kafka 实现共识](https://www.jianshu.com/p/f1a671be69a4)

### 重新部署 chaincode 时要删除 chaincode 生成的 docker 镜像

### fabric-ca 就是发证方，类似于银行发行信用卡。http://hyperledger-fabric.readthedocs.io/en/latest/identity/identity.html

```
A PKI is like a card provider – it dispenses many different types of verifiable identities. An MSP, on the other hand, is like the list of card providers accepted by the store – determining which identities are the trusted members (actors) of the store payment network. MSPs turn verifiable identities into the members of a blockchain network.
```

MSPs 把可验证的身份证明集成到了区块链网络的成员中。

### anchor peer

channel 上的其他节点都能够发现他并和他进行交流，并且 channel 上的每个 member 都会有 anchor peers(注意复数可以不只有一个 anchor peer)，这样所有的 peer(注意所有的 peer 就是指所属不同的 memer 的 peer 也是可以相互发现的) 之间可以通过 anchor peer 互相发现。

### 只读的 chaincode 调用客户端一般是不会将其记录进 ledger 的，除非客户端明确要求。

### transaction

我们的 invoke(显然 query 也是 invoke 就意味着 query 也是可以提交到 ledger 的)和 instantiate 都成为 transaction 客户端从背书节点收集返回结果并将其打包提交到 ledger。

### configtx.yml

*   Consortium 联盟

### 系统链码

*   cscc：负责 joinchannel／config update 等
*   escc：负责对传入数据进行签名（msp 管理）
*   lccc：负责 deploy invoke
*   vscc：负责签名验证／策略验证（这里如何进行策略验证？）
*   qscc：负责 ledger 查询

### A principal is described in terms of the MSP that is tasked to validate the identity of the signer and of the role that the signer has within that MSP. Currently, two roles are supported: member and admin. Principals are described as MSP.ROLE, where MSP is the MSP ID that is required, and ROLE is either one of the two strings member and admin. Examples of valid principals are 'Org0.admin' (any administrator of the Org0 MSP) or 'Org1.member' (any member of the Org1 MSP).(http://hyperledger-fabric.readthedocs.io/en/latest/endorsement-policies.html)

### configtx.yml

*   Profiles
*   Organizations
*   Application
*   Orderer
*   Capabilities
*   Resources

### policy [The primary purpose of this document is to explain how policies are defined in and interact with the channel configuratio](http://hyperledger-fabric.readthedocs.io/en/latest/policies.html)

*   UNKNOWN = 0; // Reserved to check for proper initialization
*   SIGNATURE = 1; 使用
*   MSP = 2;
*   IMPLICIT_META = 3; 使用

### MSP OU organizationalUnitName

### msp config

*   a folder admincerts to include PEM files each corresponding to an administrator certificate
*   a folder cacerts to include PEM files each corresponding to a root CA’s certificate
*   (optional) a folder intermediatecerts to include PEM files each corresponding to an intermediate CA’s certificate
*   (optional) a file config.yaml to configure the supported Organizational Units and identity classifications (see respective sections below).
*   (optional) a folder crls to include the considered CRLs
*   a folder keystore to include a PEM file with the node’s signing key; we emphasise that currently RSA keys are not supported
*   a folder signcerts to include a PEM file with the node’s X.509 certificate
*   (optional) a folder tlscacerts to include PEM files each corresponding to a TLS root CA’s certificate
*   (optional) a folder tlsintermediatecerts to include PEM files each corresponding to an intermediate TLS CA’s certificate

#### Unlike today’s systems, where a participant’s private programs are used to update their private ledgers, a blockchain system has shared programs to update shared ledgers.

不像现在的那些系统，参与者用他们私有的程序修改他们私有的账本，而区块链系统则是使用共享的程序修改共享的账本。

#### hyperledger 是由他们的参与者管理他们的交易

participants manage their transactions

#### hyperledger 不是一个公开的任何人都可以进行交易的系统，他是需要登录管理的，依靠 Membership Service Provider（MSP）来管理。

#### 多 channel 机制可以对参与者的信息进行隔离管理。

#### hyperledger fabric 账本子系统包括 world state 和 transaction log 两部分。

*   world state 用来描述某一指定时间点时 ledger 的状态。他是 ledger 的数据库。
*   transaction log 记录了所有导致 world state 状态发生改变的 transaction

#### smart contracts

只能合约是以 chaincode 的形式体现的，当外部 application 想和 blockchain 交互时都是通过 chaincode, chaincode 和 blockchain 交互则是通过 world state 来进行的。并不是直接和 transaction log 交互的。

#### 区域内信任即私链

#### The ledger is the sequenced, tamper-resistant record of all state transitions in the fabric

防篡改记录

#### features

每一个 channel 有一个 ledger, 对于参与到 channel 的 peer 他们都是这个 channel 的 member 并且会有一份 ledger 的 copy

*   查询更新可以基于 key, 范围查询，组合 key 查询
*   只读的查询可以提供丰富的查询语句(world state 使用 CouchDB)
*   当启用数据来源功能时，可以查询某一 key 的历史数据
*   交易包括了 chaincode read set 和 write set 中 key/value 的版本号
*   transaction 包含所有 peer 的签名，并且提交给 ordering service
*   transaction 在 blocks 中是有序的
*   peers 使用背书策略验证交易并执行这些策略
*   采用多版本管理，执行 chaincode 操作时会验证在执行时间内状态是否被改变过
*   交易一旦通过并且提交，则不允许在被改变
*   一个 channel 的 ledger 会有一个配置块来定以策略，acl 以及其他信息
*   channel 有 MSP 来

### msp 目录

*   组织
    *   ca 组织的根证书和对应的私钥
    *   msp 代表该组织的身份信息
        *   admincerts 组织管理员的身份验证证书，被根证书签名
        *   cacerts 组织的根证书
        *   tlscacerts 用于 tls 的 ca 证书，自签名
    *   peers
        *   peer0 第一个 peer 的信息
            *   msp
                *   admincerts 组织管理员的身份验证证书。peer 将基于这些证书来验证交易签署者是否为管理员
                *   keystore 本节点自己的身份私钥，用来签名
                *   signcerts 验证本节点签名的证书，被组织更证书签名
    *   users
        *   存放属于该组织用户的实体

### configtx.yml

*   Consortiums Order 所服务的联盟列表
*   Consortium 该应用通道锁关联的联盟的名称

### MSP 分为 local msp 和 channel msp

*   local msp 就是给 peer, orderer and users 使用的只针对其自身
*   channel msp 则是 channel 使用的多个组织都回去使用的，每个 peer 节点都会有一个 copy

### identity

*   cert
*   public key
*   IdentityIdentifier: { msg.name, 以及一个唯一 id }

一个区块链网络拥有不同的参与者，包括 peer, orderer, client applications, administrators 等等。每一个参与者都会有一个身份证（Identity）以 x.509 数字证书的形式封装。这些身份证很重要，因为他决定了你对于整个区块链网络资源的权限。hyperledger 使用参与者身份证中的一些属性来确定权限，hyperledger 给他们定了一个特殊的名字：principal。Pricipals 就好比是 userIDs 或者是 groupIDs,但是他更灵活，因为他可以包含很多关于参与者身份的属性。当我们说 principals 的时候，我们就是在参与者在系统中的决定参与者权限的身份属性。这些属性通常是 organization, organizational unit, role 或者就是参与者的身份证。

还有参与者的身份证得是合法的被验证过的，并且是来自被系统所信任的权威组织。MSP(memebership service provider)就是用来验证 Identity 的是否是被信任的，更详细的说就是，MSP 是用来表示组织中成员关系规则的模块。MSP 定义了组织中成员身份（Identity）是否有效的规则。默认的 MSP 使用 X.509 证书来作为身份证，并且整个体系结构采用传统的 PKI 模型。

Identity 是身份证，CA 是发放身份证的，MSP 是验证身份证的。

#### msp 最多包含 9 项

*   root CAs(cacerts): 包含根证书，不只一个，可以有多个，对具体的 node 来说可能只有一个，就是 orgs 的根证书而对于一些 orgs 来说则有可能有很多个根证书，他标识了哪些 CAs 是属于相同的 orgs。（必有）
*   Intermediate CAs(intermediatecerts): 同上中间级的 CA
*   Organizational Units (config.yaml): 是用来严格指定 orgs 成员，若是没有则所有可以通过根 CA 认证的都被认为是相同组织的。

    > 是需要在根证书中有体现的, 也就是在下发证书时是要表明你是属于哪个 ou 的, 这里还有问题就是可以根据具体的 node 的类型来设置 ou, 只有两类： client 和 peer

    ````
        OrganizationalUnitIdentifiers:

            Certificate: "cacerts/cacert.pem"
            OrganizationalUnitIdentifier: "COP"

        NodeOUs:

            Enable: false

            ClientOUIdentifier: # if Certificate is empty, then the certifier identifier will not be enforced

                Certificate: "cacerts/cacert.pem"
                OrganizationalUnitIdentifier: "OU_client"

            PeerOUIdentifier:
                Certificate: "cacerts/cacert.pem"
                OrganizationalUnitIdentifier: "OU_peer"
        ```
    ````

*   Administrators(admincerts): 管理的证书（必选）
*   Revoked Certificates(crls): 被撤销的证书
*   Node Identity(signcerts): 自身的身份识别证书，被组织根证书签过名的（必选）
*   keystore(keystore): 上述证书身份认证证书的私钥（必选）
*   tls root CA(tlscacerts): tls 链接使用的证书（必选）
*   TLS Intermediate CA(tlsintermediatecerts):未见到是 tls 中间层的证书

### 资源权限的确认是用 principal 来确定

一共有以下几类 pricinpal

*   MSPPrincipal_ROLE：角色类, 参与者在网络的角色

    *   admin
    *   member
    *   peer
    *   client

*   MSPPrincipal_ORGANIZATION_UNIT：组织类，参与者在网络中的组织

*   MSPPrincipal_IDENTITY：身份证类，具体到某一个参与者的权限设置

### application 与 fabric 的交互

app 要和 fabric 交互是需要通过 peer 的

1.  connect to peer
2.  invoke chaincode(proposal)
    1.  peer invokes chaincode with proposal
    2.  chaincode generates query or update proposal response
3.  proposal response
4.  request that transaction is ordered
    1.  transactions sent to peers in blocks( from order to peers)
    2.  peer updates ledger using transaction blocks
5.  ledger update event

> 通过以上我们得出 我们的客户端想要和 fabric 交互是先连接到 peer, 可 MSP 还记得否，因此 app 必须得有和 MSP 同一个 CA 下发的证书。

> 还有我们知道同一个组织的 peer 的证书是被同一个根证书所信任的，意味着，同一个组织间的 peer 的才可以进行 gossip 通信。不同组织间的通信通过 anchor peer 进行。

### configtxgen

configtx.yml

#### 顶层

*   Profiles map[string]\*Profile： 包括 order 以及 channel 配置
*   Organizations []\*Organization：各个组织的配置（公共配置）

    *   Name string `yaml:"Name"`
    *   ID string `yaml:"ID"`
    *   MSPDir string `yaml:"MSPDir"`
    *   MSPType string `yaml:"MSPType"`
    *   AdminPrincipal string `yaml:"AdminPrincipal"`
        *   AdminRoleAdminPrincipal = "Role.ADMIN"
        *   MemberRoleAdminPrincipal = "Role.MEMBER"
    *   AnchorPeers []\*AnchorPeer `yaml:"AnchorPeers"`

*   Application \*Application：应用通道的配置（公共配置）
*   Orderer \*Orderer：order 配置（公共配置）
*   Capabilities map[string]map[string]bool （未知）(个人理解绝得他应该是指定 channel 或者其他实体对某项功能是否支持)
*   Resources（未知）：权限判定
    *   目前看到 Toplevel 有 resource, 还有 application 有 resource, 同以前讲的路径权限类似， /channel/application/admins

configtxgen 工具生成 tx 文件依靠的就是 profile 中的配置项。

#### policy 规则

*   Policy_UNKNOWN Policy_PolicyType = 0
*   Policy_SIGNATURE Policy_PolicyType = 1： n_out_of 规则
*   Policy_MSP Policy_PolicyType = 2：就是 msp 证书验证规则（这里又有 MSPPrincipal role 角色规则判定）
*   Policy_IMPLICIT_META Policy_PolicyType = 3：路径判定规则

    比如：Admins 意味着只有某个实体或者组织的管理员才能操作
    /channel/order/Admins 意味只有 channel 中的 order 的管理员才能操作

> ModPolicy 是指对策略的修改权限

### 国家部门规定药企需要自建追溯系统

### 医药（溯源），供应链金融

### 群托管的公共交换协议

### 专业投资者（专业投资者可以做一些投资策略的事情但也以区块链的形式存储）与普通投资者的交互平台（Dao 以太坊）

*   发基金
*   购买

### 安全的智能合约

### 形式化验证技术

*   定理证明（定理库）
*   模型检测（模型爆炸，程序太长）

### hyperledger 的密码算法应用场景

*   数字证书做身份认证

### client 和 peers 之间的节点的交易数据是会被加密的，order 是无发探查到 client 提交的交易信息

### bccsp 统一密码服务入口

接口描述

*   秘钥声明周期管理
    *   keyGen
    *   秘钥派生（通过一个秘钥生成另一个秘钥）
    *   秘钥导入
    *   获取秘钥
*   秘钥操作

    *   验签
    *   签名
    *   hash

实现方案

*   software
*   pkcs11 硬件（保证私钥不出设备）
*   plugin(自实现 bccsp)

### msp

*   Indentity 接口， 同上所描述的身份
*   可以从 channel 中获取其他 org 的 msg 信息

### 国密算法（是国内一些领域的准入门槛）

*   sm2 椭圆曲线算法
*   sm3 hash 算法
*   sm4 对称算法

### 可以通过扩展 fabric 的 sw 来增加国密算法

### 还可以使用硬件的方法实现

### 还有 plugin 的方式实现 bccsp 的实现

### 国密算法加入-msp 的支持

*   go 的运行时环境改造
*   go lib 层更改
*   重新实现 msp
*   也实现 msp 的插件形式

### 国密算法的其他支持

*   工具链支持

*   ca 支持

*   sdk 支持

### msp 还可以根据 x.509 的属性来判断

*   1.1.0 中 增加了一个新的属性，可以在 chaincode 中调用接口来获取属性

### cryptogen 的工具是没有中间 CA 的

### CA

*   推荐加上中间 CA
*   使用 fabric-ca
*   使用 可信第三方权威 CA 作为根 CA， fabric-ca 作为中间 CA
*   全部使用外部 CA,应当支持 fabric 所支持的 rest api，以及 x.509 的实现，添加属性等

### 用户操作

*   regist 注册
*   enroll 登录（登录后需要把返回的 secret 等信息保存在本地）
*   reenroll(快到期的是后)
*   revoke(吊销证书， 需要把吊销的证书放到 msp 的目录下)

### msp 分为

*   localmsp
*   channelmsp

### msp 与 创世块

*   各个组织的 msp 信息都会被打倒创世块中

### idmex 是另一种 bccsp 的实现

### 添加 admins 需要在 fabric-ca 中注册一个新的用户，在把他的证书 copy 到 msp 的 amdincerts 中

### 1.1.0 中 Identity 增加了很多借口

### transientMap 是可以被 chaincode 的代码访问到的，可以提供一些加密秘钥或者是其他的一些东西，但他们都不会记录到 ledger 的。

### 登录过期问题

用户登录以后缺少检查登录过期的问题

### 背书策略有如下三类，在创建 channel 的时候就确定了

*   role: memeber, admin
*   ou: 部门名称
*   indenti

### 初始块解析出来以后可以看到很多的策略权限，但是目前还没看到可以有设置修改的位置

### 所有的镜像版本号得一致若是 1.1.0 的那就都得是 1.1.0 的，比如 fabric-ccenv

### 客户端链码安装时 go 版本是会去 GOPATH 下面去找的

### history 查询需要开启 history db

### 一次 chaincode 调用，可以调用多次 putState

```
putState('k1', '1');
putState('k2', '2');
putState('k3', '3');
```

### 链码（不同 channel 之间也可以）之间可以互相调用使用 invokeChaincode 但是只能是只读的。

### kafka 排序最少需要 4 个节

*   https://zhuanlan.zhihu.com/p/25358
*   https://www.jianshu.com/p/bd00f24a4771
*   https://docs.google.com/document/d/1vNMaM7XhOlu9tB_10dKnlrhy5d7b1u8lSY8a-kVjCO4/edit

### ca-server 需要先初始化， 再启动

### 实例化 chaincode 时可以指定，背书策略

如下，指定 admin 和 member 然后指定由谁来背书

```
'endorsement-policy': {
    identities: [
        { role: { name: 'member', mspId: ORGS['org1'].mspid } },
        { role: { name: 'member', mspId: ORGS['org2'].mspid } },
        { role: { name: 'admin', mspId: ORGS['org1'].mspid } }
    ],
    policy: {
        '1-of': [
            // { 'signed-by': 2},
            // { '2-of': [{ 'signed-by': 0}, { 'signed-by': 1 }]}
            { 'signed-by': 0 },
            { 'signed-by': 1 }
        ]
    }
}
```

### 没有加入 channel 的 peer 是可以安装 chaincode 的但是无法实例化指定 channel 的 chaincode。

### peer 的配置

*   CORE_VM_DOCKER_HOSTCONFIG_NETWORKMODE=fixtures_default（当前目录名\_network 名字） 很重要关系到 chaincode 容器是否能链接到相应的 peer

### fabric 网络中加入新的 peer，可以不需要再重新配置 config, 只需要相应的 org 向他颁发证书即可

1.  msp
    *   admincerts 是 org 的 cert
    *   cacerts 是 org 的 ca 的 cacerts（最顶层的根证书）
    *   peer 自己的证书由 org 的 ca 签发
    *   还有对应的私钥
    *   tlscacerts 是 org 的 tlscacert(最顶层的根证书)
2.  tls
    *   ca.crt 由 org 的 tlscacert 颁发
    *   server.crt 由 ca.crt 颁发

### 颁发的 x509 证书的域名是要和节点的域名相匹配

例如我给 order 节点颁发的是 example.com 这个域名那么，order 机器对外的域名就得是 example 的子域名，若 order 对外的是 ip，则证书失效，证书里面必须体现 ip 才可以。

### hyperledger 的 gossip 协议 [简介](https://zhuanlan.zhihu.com/p/27989809)

1.  当我们配置 peer.gossip.orgLeader = true， 那么每个节点启动后都会去和 orderer 连接并接收消息
2.  当我们配置 peer.gossip.userLeaderElection = true, 那么组织内配的节点启动后， 会参与 leader 的选举，选出一个 leader 和 orderer 进行通信，其他节点则是通过 leader 节点同步小心。

上面两项属于 bootstrap 配置，与 anchor peer 无关，anchor peer 是用来和其他组织进行通信的接口
