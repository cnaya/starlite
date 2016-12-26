# starlite=星空博客
---
基于webcppd的Markdown博客程序

![](https://raw.githubusercontent.com/webcpp/starlite/master/assets/blog/starlite-log.png)

## 依赖
- webcppd

- 其他
  - centos
    - sudo yum -y install opencv-devel cryptopp-devel qrencode-devel

  - ubuntu 
    - sudo apt-get -y install libopencv-dev libcrypto++-dev libqrencode-dev

## 安装

`make && sudo make install && sudo systemctl restart webcppd`

## 特性
 - 前端
   - Jquery
   - Bootstrap
   - Layer
   - Editor.md

 - 后端
   - C++

 -  模板引擎
    - mustache

 - 数据库
    - MariaDB/MySQL
       - MySQL v5.6+用户可以直接把`db/db.sql`中的`ENGINE=Mroonga`改为`ENGINE=INNODB`。
       - MariaDB用户需安装`Mroonga`存储引擎以支持全文搜索，具体请参考：[https://mariadb.com/kb/en/mariadb/about-mroonga/](https://mariadb.com/kb/en/mariadb/about-mroonga/)
 
