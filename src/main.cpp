
#include <maycloud/netdaemon.h>
#include <maycloud/asyncwebserver.h>

/**
* Пример главной страницы
*/
void mainPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	const char *data = "<p>Your AsyncWebServer example is working.</p><ul>"
	"<li><a href=\"/auth\">Authentication example</a></li>"
	"<li><a href=\"/stat.json\">JSON page example</a></li>"
	"<li><a href=\"/index.php?foo=42\">GET variables example</a></li>"
	"</ul>";
	response->setSimpleHtmlPage("Hello, world!", data);
}

/**
* Пример страницы, защищённой HTTP-авторизацией
*/
void protectedPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	//AsyncWebServer *server = (AsyncWebServer *) userdata;
	if(!request->authenticateUser(response, "admin", "123456", "Restricted area!")) return;
	response->setBody("Auth passed!");
}

/**
* Пример страницы, возвращающей данные в JSON
*/
void statPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	response->setContentType("application/json");
	response->setBody("{\"foo\": 42}");
}

/**
* Пример страницы, возвращающей данные в JSON
*/
void getMethodPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	if(request->getVariableExists("foo")) {
		std::string foo = request->get("foo");
		if(foo.empty()) foo = "&lt;an empty string&gt;";
		response->setSimpleHtmlPage("GET var example", "The value of <b>foo</b> is: " + foo);
	} else {
		response->setSimpleHtmlPage("GET var example", "The variable <b>foo</b> is not set");
	}
}

/**
* Точка входа
*/
int main(int argc, char *argv[]) {

	// Создаём объект демона
	NetDaemon daemon(100, 1024);

	// Наш веб-сервер
	AsyncWebServer server(&daemon);

	// Назначаем 9095 порт TCP
	server.bind(9095);

	// Начинаем слушать заданный порт с размером очереди входящих соединений = 10
	server.listen(10);

	// Добавляем сервер в демона
	daemon.addObject(&server);

	// Сопоставляем URL-адресам их обработчики (хэндлеры)
	server.get("/", mainPage, (void *) &server);
	server.get("/auth", protectedPage, (void *) &server);
	server.get("/stat.json", statPage, (void *) &server);
	server.get("/index.php", getMethodPage, (void *) &server);

	// Просто выведем подсказку
	std::cout << "Listening on http://127.0.0.1:9095/" << std::endl;

	// Запускаем цикл обработки событий
	daemon.run();

	// Выход из сервера
	return 0;
}
