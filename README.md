# FiveChessServer

大学生创新项目：网络智能五子棋博弈系统的研究与实现

项目说明：实现了C/S架构的五子棋游戏，支持双人对弈、人机对弈等功能

服务器端代码开源：https://github.com/FateSolo/FiveChessServer

客户端代码开源：https://github.com/FateSolo/FiveChessClient

本篇文章是对服务器端程序的简要说明。

### 1. 项目开发环境：
* Ubuntu 14.04.3
* Pycharm 5.0.3
* Python 2.7.6
* G++ 4.8.4
* MySQL 5.5

### 2. 总体说明：
* 主要使用Python实现后台逻辑功能

* 应用了基于事件驱动的Twisted作为服务器框架

* 与五子棋AI相关的模块则使用C++编写，由G++编译成动态链接库(.so)，Python使用ctypes调用动态库内的函数

* 使用MySQLdb完成对MySQL数据库的操作

### 3. 五子棋AI相关模块说明：
* 核心AI：FiveChessAI.so (负极大值算法 + AlphaBeta剪枝 + 历史启发 + 置换表 + 限制搜索边框)

* 判断胜负：FiveChessRule.so

* 连五威胁评判：ThreatJudgement.so

* 具体源码存放在 FiveChessAI/ 文件夹内

### 4. 运行环境部署：
这里以Ubuntu Server 14.04为例

1、首先使用浏览器登录你自己的Github账号，将github.com/FateSolo下的FiveChessServer项目Fork到你自己的账号下(如果可以从其他地方获取到源文件，例如大创光盘，那么请直接从第8步开始)

2、在Ubuntu的终端下安装git

        $ sudo apt-get install git

3、配置你的git

    	$ git config --global user.name "在这里填写你的用户名"
    	$ git config --global user.email "在这里填写你的邮箱"

4、在Ubuntu的终端下安装ssh

    	$ sudo apt-get install ssh

5、生成密钥

    	$ ssh-keygen -t rsa -C "你的邮箱"

6、找到当前路径下的隐藏文件.ssh，复制id_rsa.pub文件中的内容，到Github网页上进入Settings，找到SSH Keys将复制的密钥加入

7、成功后可以从你的Github中克隆项目到本地了(注意将name替换成你的用户名)

    	$ git clone git@github.com:name/FiveChessServer.git

8、安装setuptools

    	$ wget "https://pypi.python.org/packages/source/s/setuptools/setuptools-12.0.3.tar.gz#md5=f07e4b0f4c1c9368fcd980d888b29a65" --no-check-certificate
    	$ tar -zxvf setuptools-12.0.3.tar.gz
    	$ cd setuptools=12.0.3
    	$ sudo python setup.py install

9、安装pip

    	$ wget "https://pypi.python.org/packages/source/p/pip/pip-1.5.4.tar.gz#md5=834b2904f92d46aaa333267fb1c922bb" --no-check-certificate
    	$ tar -xzvf pip-1.5.4.tar.gz
    	$ cd pip-1.5.4
    	$ sudo python setup.py install

10、安装MySQL

    	$ sudo apt-get install mysql-client mysql-server

11、建数据库、表

    	$ mysql -u root -p
    	mysql> CREATE SCHEMA `FiveChess` DEFAULT CHARACTER SET utf8 ;
    	mysql> CREATE TABLE `FiveChess`.`User` (`UserName` VARCHAR(10) NOT NULL,`PassWord` VARCHAR(10) NOT NULL,`NickName` VARCHAR(20) NOT NULL,`Win` INT NULL DEFAULT 0,`Lose` INT NULL DEFAULT 0,`Draw` INT NULL DEFAULT 0,PRIMARY KEY (`UserName`));

12、安装环境依赖

    	$ sudo apt-get update
    	$ sudo apt-get install gcc libmysqlclient-dev python-dev

13、安装MySQLdb模块

    	$ sudo pip install MySQL-Python

14、安装Twisted

    	$ sudo pip install twisted

15、以守护进程方式运行FiveChessServer.py

    	$ cd FiveChessServer
    	$ sudo twistd -y FiveChessServer.py

16、验证是否成功运行(查看是否拥有名为twistd的进程)

    	$ ps -e

------

作者 [@FateSolo]<br>
2016 年 2月 6日
