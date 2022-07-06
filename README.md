# chatserver
基于muduo库的，工作在nginx tcp负载均衡环境，MySQL支持，redis，的集群聊天服务器和客户端源码



#文件组成
bin: 可执行文件
include: 头文件
src: cpp文件
thirdpatry: 第三方头文件
test: muduo库和json的测试



#编译
cd build
rm -rf *
cmake ..
make



#环境配置
事先配置好所需第三方库
db.hpp中将数据库配置信息更改为本机设置
