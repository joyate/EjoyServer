# EjoyServer

基于boost asio 开发的一个服务器，异步处理消息。简单易用


#### 框架特点

* 统一的第三方数据缓冲层接口封装
* 消息自动注册
* 简洁的消息处理机制
* 统一的消息定义格式
* 完全的异步通信


#### 插件接口

* 第三方库，统一通过插件的形式载入，支持dll/so载入第三方插件，目前已封装好如下第三方插件<br>
  * mysql库封装
  * hiredis库封装
  
* 命令行参数解析 <br>
  * boost::program_options
  
  
* 网络层 <br>
  * boost::asio
  
  
* 同步锁 <br>
  * boost::mutex <br>
  * boost::mutex::scoped_lock <br>
  * boost::condition_variable_any <br>
  
  
* 多线程分级日志系统 <br>


* 对象池 <br>
  * boost::pool <br>
  
  
  
#### 有待改善

* 目前服务器架构适用于弱交互的游戏类型，对于强交互的支持比较弱
* 在网络消息缓冲目前采用的是全局缓冲池，有一定的性能损失，后续考虑环形缓冲
* 目前程序只在window下编译测试通过，linux下还没有编译测试
