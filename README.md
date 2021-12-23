# dv-routing
计算机网络实验项目：dv-routing算法模拟
每一个运行该程序的进程作为一个路由器，向其邻居周期性(每2秒)发送自己的路由表，每个路由器根据接收到的路由表以及已知路由表计算到其他路由器的最短距离
每过5秒，路由器将5秒内未收到信息的邻居标记为不可达(即将cost设为一个很大的数)
使用UDP进行通信
## 编译
clang++-11 -fdiagnostics-color=always -g *.cpp -o main -pthread

## 运行
开启多个终端(具体数目由初始化数据nodeaddr.txt与topology.txt决定。本例中为6个)，分别执行
./main nodeA nodeaddr.txt topology.txt
./main nodeB nodeaddr.txt topology.txt
...
./main nodeF nodeaddr.txt topology.txt

运行过程中，输入disp指令展示当前路由表

## 运行结果
