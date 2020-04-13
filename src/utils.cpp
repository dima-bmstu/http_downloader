#include <map>
#include <algorithm>
#include <iostream>

#include "utils.h"

static const std::string base64_symbols ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const std::map<std::string, std::string> long_to_short_options = {
    {"--output", "-o"},
    {"--help", "-h"}
};

ArgsParser::ArgsParser(int argc, char const **argv) {
    bool found_url = false, was_option = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        bool is_arg_option = (arg.find("-") != arg.npos);
        if (!found_url && !was_option && !is_arg_option) {
            url_arg = arg;
            found_url = true;
            continue;
        }
        if (is_arg_option && long_to_short_options.count(arg)) {
            arg = long_to_short_options.at(arg);
        }
        tokens.push_back(std::move(arg));
        was_option = is_arg_option;
    }
}

std::string_view ArgsParser::url() const {
    return url_arg;
}

std::string_view ArgsParser::option(std::string_view option_name) const {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(tokens.begin(), tokens.end(), option_name);
    if (itr != tokens.end() && ++itr != tokens.end()) {
        return *itr;
    }
    throw std::runtime_error("Cannot find option " + std::string(option_name) + ", use -h to show hint.");
}

bool ArgsParser::is_option_exists(std::string_view option) const {
    return std::find(tokens.begin(), tokens.end(), option) != tokens.end();
}

void print_usage() {
    std::cout << "Usage: load [options...] <url>\n"
              << "-h, --help          This help text\n"
              << "-o, --output <FILE> Write document to FILE" << std::endl;
}

std::string_view filename_from_url(std::string_view url) {
    auto last_separator = url.rfind('/');
    if (last_separator != std::string::npos && last_separator < url.size() - 1)
        return url.substr(last_separator + 1, url.size() - last_separator);
    return "output";
}

std::string base64_encode(std::string_view str) {
    auto bytes_to_encode = str.data();
    auto in_len = str.size();
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_symbols[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_symbols[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}