# HAR SERVER
---
## 编译依赖
+ gcc
+ 依赖于[Boost.PropertyTree](https://www.boost.org)。
+ 使用[grpc](https://github.com/grpc/grpc)内部通信。  
+ 数据库连接器使用[libpqxx](https://github.com/jtv/libpqxx)。
## 运行依赖
+ python3
+ postgresql
+ rest服务依赖于python包Flask、Flask-RESTful、grpcio、grpcio-status、protobuf
## 测试依赖
+ 测试依赖于[googletest](https://github.com/google/googletest)。
+ python依赖包psycopg2
## 配置文件
配置文件实例如下：  
```
{
  "rpc_server": "127.0.0.1:45211",
  "rest_server": "127.0.0.1:45212",
  "database": {
    "ip": "127.0.0.1",
    "port": 5432,
    "database_name": "test",
    "user_name": "postgres",
    "password": "postgres"
  }
}
```

## 使用方法
```
    git clone https://github.com/QuickRepair/server.git
    cd server
    mkdir build
    cd build
    cmake ..
    make
```