简单的聊天室程序
---   
> 客户端功能：
> 1.从标准输入终端读入用户数据，并将用户数据写入管道发送至服务器
> 2.向标准输出终端打印收到的服务器数据

> 服务器功能 ：服务器使用poll同时监听socket 和 连接socket
> 1.接收客户端数据
> 2.把收到的数据发送给每一个登陆到该服务器的客户端

**使用方法**  

*编译*

> g++ -o client chatRoom_Client.cpp
> g++ -o server chatRoom_Server.cpp
*先运行服务器程序*   

>./server -ip -port -userlimit   

*在多个终端运行客户端程序*  

>./client ip -port   

#效果展示   
![avatar](G:\VMware workstation\Share\GCodeline\ChattingRoom\datum\ChattingRoom.png)

参考资料: Linux高性能服务器编程 - 游双 