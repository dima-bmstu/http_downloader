#include <iostream>

#include "UrlParser.h"
#include "FileSaver.h"
#include "Tcp.h"
#include "HttpDownloader.h"
#include "utils.h"

int main(int argc, char const **argv)
{
    try {
        ArgsParser input(argc, argv);
        std::string output_filename(filename_from_url(input.url()));
        
        if (input.is_option_exists("-h")) {
            print_usage();
            return 0;
        }
        if (input.is_option_exists("-o")) {
            output_filename = input.option("-o");
        }

        HttpDownloader downloader(std::unique_ptr<SaverInterface>(new FileSaver()),
                                  std::unique_ptr<TcpInterface>(new Tcp()),
                                  std::shared_ptr<ParserInterface<Url, std::string_view> > (new UrlParser()));
        
        downloader.download(input.url(), output_filename);
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " <<  e.what() << std::endl;
    }
    
    return 0;
}
