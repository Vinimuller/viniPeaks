#ifndef LOCAL_SERVER_H_
#define LOCAL_SERVER_H_

#include <SmingCore.h>

#define DNS_DOMAIN 	"vinipeaks.com"
#define DNS_PORT 	53

class c_localServer{
public:
	c_localServer();
	~c_localServer();

	void init();
    
    HttpServer server;
	DnsServer dnsServer;
};

#endif //LOCAL_SERVER_H_