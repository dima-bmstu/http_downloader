#include "UrlParser.h"
#include <regex>
#include <iostream>

Url UrlParser::parse(std::string_view url) {
    try
    {
        std::regex re("((.*)://)?"        // protocol    (http://)        (1 (2)://)
                      "((.*):(.*)@)?"     // credentials (user:password@) (3 (4):(5)@)
                      "([^/:]+)"          // url         (mail.ru)        (6)
                      "(:(\\d+))?"        // port        (:80)            (7 :(8))
                      "(/([^?#]+))?"      // /path/to/file                (9 /(10))
                      "(\\?([^#]+))?"     // parametets  (?param=value)   (11 ?(12))
                      "(#(.*))?"          // anchor      (#place)         (13 #(14))
                     );        
        std::smatch match;
        std::string url_string(url);
        
        if (std::regex_search(url_string, match, re)) {
            uint16_t port = match.str(8).empty() ? 80 : static_cast<uint16_t>(std::stoi(match.str(8)));
            return Url{.protocol = match.str(2),
                       .host = match.str(6),
                       .port = port,
                       .user = match.str(4),
                       .password = match.str(5),
                       .path = match.str(10),
                       .parameters = match.str(12),
                       .anchor = match.str(14)};
        }
    }
    catch (std::regex_error &e) {
        std::cerr << "Url parser regex error: " << e.what() << std::endl;
    }

    return Url();
}