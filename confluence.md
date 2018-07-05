### 破解流程（机器配置 4c8g）(http://www.llbee.com/?p=630)

*   apt-get install postgresql
*   安装之后 /etc/init.d/postgresql start 来启动
*   然后切换到 postgres 用户
*   使用 psql 来来连接数据库， -p 指定端口
    *   创建用户 CREATE USER dengjie WITH PASSWORD 'dengjie';
    *   创建数据库 CREATE DATABASE confluence OWNER dengjie;
    *   授权 GRANT ALL PRIVILEGES ON DATABASE confluence to dengjie;
*   root 用户下启动 atlassian-confluence-6.8.2-x64.bin
*   打开 confluence 网站设置
*   停止 /etc/init.d/confluence stop, 把 {confluence 安装目录}/confluence/WEB-INF/lib/atlassian-extras-decoder-v2-3.2.jar 替换掉
*   重启 /etc/init.d/confluence start
