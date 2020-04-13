#ifndef DOWNLOADER_URILS_H
#define DOWNLOADER_URILS_H

#include <string>
#include <vector>

struct Url {
    std::string protocol;
    std::string host;
    uint16_t port = 0;
    std::string user;
    std::string password;
    std::string path;
    std::string parameters;
    std::string anchor;
};

class ArgsParser {
    public:
        ArgsParser (int argc, char const **argv);
        std::string_view url() const;
        std::string_view option(std::string_view option_name) const;
        bool is_option_exists(std::string_view option) const;

    private:
        std::vector<std::string> tokens;
        std::string url_arg;
};

void print_usage();
std::string_view filename_from_url(std::string_view url);
std::string base64_encode(std::string_view str);

#endif //DOWNLOADER_URILS_H