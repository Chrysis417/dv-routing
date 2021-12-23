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
![1](https://user-images.githubusercontent.com/62326963/147217852-a337b6fc-0ed6-4a6c-939f-727f32baea32.png)
![2](https://user-images.githubusercontent.com/62326963/147217857-69e06cdd-9810-4f40-a5d0-dcdc3b8113c8.png)
![3](https://user-images.githubusercontent.com/62326963/147217859-51f582e7-0bb1-44b0-b095-41d6443a3c92.png)
![4](https://user-images.githubusercontent.com/62326963/147217860-829e7bfe-7b92-4c16-b449-8c98b942f7ec.png)
