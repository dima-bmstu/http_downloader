#ifndef DOWNLOADER_INTERFACES_H
#define DOWNLOADER_INTERFACES_H

#include <string>

inline constexpr size_t defaut_read_bytes = 4 * 1024;

class SaverInterface {
public:
    virtual void open(std::string_view name) = 0;
    virtual void save(std::string_view data) = 0;
    virtual void close() = 0;
    virtual ~SaverInterface() = default;
};

template <class To, class ...From>
class ParserInterface {
public:
    virtual To parse(From...) = 0;
    virtual ~ParserInterface() = default;
};

class TcpInterface {
public:
    virtual void connect(std::string_view host, uint16_t port) = 0;
    virtual void write(std::string_view) = 0;
    virtual std::string_view read(size_t bytes = defaut_read_bytes) = 0;
    virtual ~TcpInterface() = default;
};

#endif //DOWNLOADER_INTERFACES_H