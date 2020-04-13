#include <iostream>
#include <iomanip>
#include <sstream>

#include "HttpDownloader.h"
#include "utils.h"

static constexpr char end_of_header[] = "\r\n\r\n";
static constexpr char content_length[] = "Content-Length: ";

HttpDownloader::HttpDownloader(std::unique_ptr<SaverInterface>&& saver,
                               std::unique_ptr<TcpInterface>&& tcp,
                               std::shared_ptr<ParserInterface<Url, std::string_view> > parser) 
    : saver(std::move(saver)), tcp(std::move(tcp)), url_parser(parser) {}

void HttpDownloader::download(std::string_view string_url, std::string_view filename) {
    if (filename.empty())
        thow_exception("filename is empty");
    
    Url url = url_parser->parse(string_url);
    if (url.host.empty())
        thow_exception("url is empty");

    if (url.protocol != "http")
        thow_exception("protocol " + url.protocol + " is not supported");
    
    tcp->connect(url.host, url.port);
    std::cout << "Connection established" << std::endl;

    tcp->write(make_request(url));
    while (!response_header_received)
        append_to_header(tcp->read());
    
    saver->open(filename);
    uint32_t received_bytes = 0;
    if (!file_chunk.empty()) {
        saver->save(file_chunk);
        received_bytes += file_chunk.size();
    }

    while(received_bytes < file_size) {
        auto data = tcp->read();
        saver->save(data);
        received_bytes += data.size();
        print_progress(received_bytes);
    }

    saver->close();
    std::cout << "\nFile downloaded and saved to file " << filename << std::endl;
}

std::string HttpDownloader::make_request(const Url& url) {
    std::stringstream stream;
    stream << "GET /" << url.path << (url.parameters.empty() ? "" : "?" + url.parameters) << " HTTP/1.1\r\n"
           << "Host: " << url.host << "\r\n";
    if(!url.user.empty()) {
        stream << "Authorization: Basic " << base64_encode(url.user + ":" + url.password) << "\r\n";
    }
    stream << "User-Agent: downloader\r\n\r\n";
    return stream.str();
}

constexpr size_t HttpDownloader::string_size(std::string_view str) {
    return str.size();
}

void HttpDownloader::append_to_header(std::string_view data) {
    response_header.append(data);
    size_t header_end_position = response_header.find(end_of_header);
    if (header_end_position == response_header.npos)
        return;

    size_t length_posistion = response_header.find(content_length);
    if (length_posistion == response_header.npos)
        thow_exception("Cannot find Content-Length in header");

    file_size = std::stol(response_header.substr(length_posistion + string_size(content_length),
                                                     response_header.find("\r\n", length_posistion)));
    
    if (file_size == 0)
        thow_exception("Wrong Content-Length");

    response_header_received = true;
    file_chunk = response_header.substr(header_end_position + string_size(end_of_header));
}

void HttpDownloader::print_progress(size_t received) {
    std::cout << "\rReceived " << std::fixed << std::setprecision(0) << (static_cast<double>(received)/file_size)*100 << "% "
              << "(" << received << " bytes from " << file_size << ")" << std::flush;
}

void HttpDownloader::thow_exception(std::string error) {
    throw std::runtime_error("HttpDownloader: " + error);
}