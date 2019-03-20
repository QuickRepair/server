# HAR SERVER

#依赖库
使用Microsoft的cpprest(casablanca)作为网络库。  
数据库连接器使用mariadb-connector-c，该库在cmake配置中默认存放于/usr/local/mariadb/，下载地址 https://downloads.mariadb.com/Connectors/c  
配置文件结构如下：  
{
	"listen_ip": "http://×.×.×.×",
	"listen_port": 12345,
	"database": {
		"ip": "×.×.×.×",
		"port": 3306,
		"database_name": "×××",
		"user_name": "×××",
		"password": "×××"
	}
}