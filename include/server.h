#ifndef LOCAL_SERVER_H_
#define LOCAL_SERVER_H_

#include <SmingCore.h>

class c_localServer{
public:
	c_localServer();
	~c_localServer();

	void init();
    
    HttpServer server;
};

#endif //LOCAL_SERVER_H_