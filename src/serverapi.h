#ifndef _SERVER_H_
#define _SERVER_H_
#include "mem.h"

class ServerAPI {
public:
    ServerAPI(const ServerConfig &config);
    ~ServerAPI();

    void loop();
};

#endif