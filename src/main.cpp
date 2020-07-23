
#include <maycloud/netdaemon.h>
#include <maycloud/asyncwebserver.h>

void protectedPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	//AsyncWebServer *server = (AsyncWebServer *) userdata;
	if(!request->authenticateUser(response, "admin", "123456", "Restricted area!")) return;
	response->setBody("Auth passed!");
}

void mainPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	response->setSimpleHtmlPage("Hello, world!", "<p>Your AsyncWebServer example is working.</p><ul><li><a href=\"/auth\">Authentication example</a></li><li><a href=\"/stat.json\">JSON page example</a></li></ul>");
}

void statPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	response->setContentType("application/json");
	response->setBody("{\"foo\": 42}");
}

int main(int argc, char *argv[]) {
	NetDaemon daemon(100, 1024);
	AsyncWebServer *server = new AsyncWebServer(&daemon);
	server->bind(9095);
	server->listen(10);
	daemon.addObject(server);

	server->get("/", mainPage, (void *) server);
	server->get("/auth", protectedPage, (void *) server);
	server->get("/stat.json", statPage, (void *) server);

	daemon.run();
	return 0;
}
