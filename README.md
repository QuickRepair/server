# HAR SERVER
---
## 依赖库
+ 依赖于[Boost.PropertyTree](https://www.boost.org)。
+ 使用[cpprest(casablanca)](https://github.com/microsoft/cpprestsdk)作为网络库。  
+ 数据库连接器使用[libpqxx](https://github.com/jtv/libpqxx)。
+ 测试依赖于[googletest](https://github.com/google/googletest)，设置CMakeLists.txt中变量_TEST_ENV为ON以启动测试。

## 配置文件
配置文件结构如下：  
```
{
	"listen_ip": "http://*.*.*.*",
	"listen_port": 12345,
	"database": {
		"ip": "*.*.*.*",
		"port": 3306,
		"database_name": "***",
		"user_name": "***",
		"password": "***"
	}
}
```
