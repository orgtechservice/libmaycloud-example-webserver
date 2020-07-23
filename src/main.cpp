
#include <maycloud/netdaemon.h>
#include <maycloud/asyncwebserver.h>

/**
* Пример главной страницы
*/
void mainPage(HttpRequest *request, HttpResponse *response, void *userdata) {
	response->setSimpleHtmlPage("Hello, world!", "<p>Your AsyncWebServer example is working.</p><ul><li><a href=\"/auth\">Authentication example</a></li><li><a href=\"/stat.json\">JSON page example</a></li></ul>");
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
* Точка входа
*/
int main(int argc, char *argv[]) {

	// Создаём объект демона
	NetDaemon daemon(100, 1024);

	// Наш веб-сервер
	AsyncWebServer *server = new AsyncWebServer(&daemon);

	// Назначаем 9095 порт TCP
	server->bind(9095);

	// Начинаем слушать заданный порт с размером очереди входящих соединений = 10
	server->listen(10);

	// Добавляем сервер в демона
	daemon.addObject(server);

	// Сопоставляем URL-адресам их обработчики (хэндлеры)
	server->get("/", mainPage, (void *) server);
	server->get("/auth", protectedPage, (void *) server);
	server->get("/stat.json", statPage, (void *) server);

	// Просто выведем подсказку
	std::cout << "Listening on http://127.0.0.1:9095/" << std::endl;

	// Запускаем цикл обработки событий
	daemon.run();

	// Выход из сервера
	return 0;
}
