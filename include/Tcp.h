#ifndef DOWNLOADER_TCP_H
#define DOWNLOADER_TCP_H

#include "Interfaces.h"
#include <netinet/in.h>

class Tcp : public TcpInterface {
public:
    Tcp() = default;
    void connect(std::string_view host, uint16_t port) override;
    void write(std::string_view data) override;
    std::string_view read(size_t bytes = defaut_read_bytes) override;

private:
    int socket_id = -1;
    struct sockaddr_in server_addr;
    std::string buffer;
};

#endif //DOWNLOADER_TCP_H