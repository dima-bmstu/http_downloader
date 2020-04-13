#ifndef DOWNLOADER_FILE_SAVER_H
#define DOWNLOADER_FILE_SAVER_H

#include <fstream>

#include "Interfaces.h"

class FileSaver : public SaverInterface {
public:
    FileSaver() = default;
    void open(std::string_view file_name) override;
    void save(std::string_view data) override;
    void close() override;
    ~FileSaver();

private:
    std::ofstream file;
};

#endif //DOWNLOADER_FILE_SAVER_H