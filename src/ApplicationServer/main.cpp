#include <iostream>
#include "DataSource/DataSource.hpp"
#include "Configuration/Configure.h"
#include "RPC/RPCServer.h"
//#include "DataSource/DataSource.hpp"
#include "Factories/DataAccessFactory.h"

using std::runtime_error;			using std::cerr;
using std::endl;


int main()
{
	try
	{
		Configure configure;
		DataSource::initDataAccessObject(DataAccessFactory::getDataAccessObject(configure));
		RPCServer rpcServer(configure.RPCAddress());
		rpcServer.run();
	}
	catch(runtime_error &e)
	{
		cerr << e.what() << endl;
	}

	return 0;
}
