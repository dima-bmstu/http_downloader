#ifndef DOWNLOADER_URL_PARSER_H
#define DOWNLOADER_URL_PARSER_H

#include "Interfaces.h"
#include "utils.h"


class UrlParser : public ParserInterface <Url, std::string_view> {
public:
    UrlParser() = default;
    Url parse(std::string_view url) override;
    ~UrlParser() = default;
};

#endif //DOWNLOADER_URL_PARSER_H