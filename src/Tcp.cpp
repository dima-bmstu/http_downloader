#include "Tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdexcept>

void Tcp::connect(std::string_view host, uint16_t port) {
    struct hostent *server;
    
    if((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Tcp: could not create socket");
    
    server = gethostbyname(host.data());
    if (server == NULL)
        throw std::runtime_error("Tcp: no such host");

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    
    server_addr.sin_port = htons(port);
    if (::connect(socket_id,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Tcp: Failed to connect to server");
}

void Tcp::write(std::string_view data) {
    auto sended = send(socket_id , data.data() , data.size(), 0);
    if (sended < 0)
        throw std::runtime_error("Tcp: Cannot send data: " + std::string(data));
}

std::string_view Tcp::read(size_t bytes) {
    buffer.resize(bytes);
    auto recv_bytes = recv(socket_id, buffer.data(), bytes, 0);
    if (recv_bytes <= 0)
        throw std::runtime_error("Tcp: something went wrong while read data");
    return std::string_view(buffer).substr(0, recv_bytes);
}
