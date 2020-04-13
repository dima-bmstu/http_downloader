#ifndef DOWNLOADER_HTTP_H
#define DOWNLOADER_HTTP_H

#include "Interfaces.h"
#include "utils.h"
#include "memory"

class HttpDownloader {
public:
    HttpDownloader(std::unique_ptr<SaverInterface>&& saver,
                   std::unique_ptr<TcpInterface>&& tcp,
                   std::shared_ptr<ParserInterface<Url, std::string_view> > parser);
    void download(std::string_view string_url, std::string_view filename);

private:
    std::string make_request(const Url& url);
    void append_to_header(std::string_view data);
    constexpr size_t string_size(std::string_view str);
    void print_progress(size_t received);
    void thow_exception(std::string error);

    std::unique_ptr<SaverInterface> saver;
    std::unique_ptr<TcpInterface> tcp;
    std::shared_ptr<ParserInterface<Url, std::string_view> > url_parser;
    
    size_t file_size = 0;
    bool response_header_received = false;
    std::string response_header;
    std::string file_chunk;
};

#endif //DOWNLOADER_HTTP_H