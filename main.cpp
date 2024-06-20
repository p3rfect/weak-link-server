#include "Services/ServerManager.h"

int main() {
    ServerManager server;
    server.start_server(8080);
    return 0;
}
