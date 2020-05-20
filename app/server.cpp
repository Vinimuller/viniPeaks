#include <server.h>

void onFile(HttpRequest& request, HttpResponse& response);
void onIndex(HttpRequest& request, HttpResponse& response);

c_localServer::c_localServer()
{
	Serial.println("\n=== SERVER CONSTRUCTOR ===");
	
    server.listen(80);
	server.paths.set("/", onIndex);
	server.paths.setDefault(onFile);
}

c_localServer::~c_localServer()
{

}

void c_localServer::init()
{   
    server.listen(80);
	server.paths.set("/", onIndex);
	server.paths.setDefault(onFile);
}

void onIndex(HttpRequest& request, HttpResponse& response)
{
	response.sendFile("index.html");
}

void onFile(HttpRequest& request, HttpResponse& response)
{
	String file = request.uri.getRelativePath();
	if(file[0] == '/')
		file = file.substring(1);

	if(file[0] == '.')
    {
        response.setAllowCrossDomainOrigin("*");
		response.code = HTTP_STATUS_FORBIDDEN;
    }
	else {
		response.setCache(86400, true); // It's important to use cache for better performance.
        response.setAllowCrossDomainOrigin("*");
		response.sendFile(file);
	}
}