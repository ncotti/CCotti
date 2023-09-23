#ifndef H_HTTP_SERVER
#define H_HTTP_SERVER

#include "server.h"
#include "tools.h"
#include "sig.h"
#include "sem.h"
#include "shared_memory.h"
#include <string.h>
#include <time.h>
#include "http_types.h"

#define DEFAULT_BACKLOG 2
#define DEFAULT_MAX_CLIENTS 1000
#define DEFAULT_SENSOR_PERIOD 1000
#define DEFAULT_SAMPLES_MOVING_AVERAGE_FILTER 5
#define SERVER_ROOT "web"
#define REQUEST_SIZE 10000
#define RESPONSE_SIZE 100000
#define CONFIG_FILE_PATH_SIZE 256
#define SERVER_NAME "Cotti_server"

typedef struct serverData {
    int backlog;
    int max_clients;
    int sensor_period;
    int samples_moving_average_filter;
    int client_count;
} serverData;

class HttpServer: public Server {
protected:
    static bool flag_update_conf;
    SharedMemory<serverData> shm;
    char config_file[CONFIG_FILE_PATH_SIZE];

    void on_start(void) override;
    void on_accept(Socket& socket) override;
    static void sigusr1_handler(int signal);    // Updated text file
    void response(Socket& socket, HttpResponse res);
    int request(Socket& socket, HttpRequest* req);
    HttpResponse not_found(void);
    void update_configuration(void);

public:
    HttpServer(const char* ip=NULL, const char* port="http", const char* config_file="config.cfg");
};

#endif // H_HTTP_SERVER