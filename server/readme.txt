1.逻辑线程不能直接操作数据库,不能随便包含DB_Operator.h
2.不能随便包含Msg_Define.h
3.模块头文件不能再包含其他头文件
4.模块cpp需要的头文件在Logic_Module_Head.h / Scene_Module_Head.h,不能再包含其他头文件
5.迭代器内erase
6.慎用指针

---流水记录方法---  
1.建立数据库表配置在 SVN 上 JD02\trunk\requirement\配置文件\策划配制\M-MYSQL初始化配置.xls
2.点击 M-MYSQL初始化配置.bat 生成配置并更新到SVN上
3.代码中使用宏定义:MSG_STREAM(FMT,...);
	例如: MSG_STREAM("`表名`(`字段1`, `字段二`) VALUES (%d,%d)", 1, 2);

---Linux下Eclipse SVN 重新输入用户名密码方法---  
eclipse/configuration/org.eclipse.core.runtime/.keyring
~/.subversion

--record --gate --logic --chat --scene --name=all --index=0

---设置时间--- 
date -s "2015-02-14 13:52:00"
ntpdate asia.pool.ntp.org

---改时间后编译问题--- 
find . -type f | xargs -n 5 touch

---centos最小化安装问题问题--- 
yum groupinstall "Development Libraries" "Development Tools"
yum groupinstall "Desktop"
yum groupinstall "X Window System"
yum install *fonts*

---wireshark
yum install wireshark
yum install wireshark-gnome

---eclipse svn
name:subclipse
http://subclipse.tigris.org/update_1.8.x
