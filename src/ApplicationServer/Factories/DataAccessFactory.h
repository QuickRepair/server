//
// Created by gaojian on 2019/9/14.
//

#ifndef HARSERVER_DATAACCESSFACTORY_H
#define HARSERVER_DATAACCESSFACTORY_H


#include <memory>

class DataAccess;
class Configure;

class DataAccessFactory {
public:
	static std::unique_ptr<DataAccess> getDataAccessObject(Configure &configure);
};


#endif //HARSERVER_DATAACCESSFACTORY_H
