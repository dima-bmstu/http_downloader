#include "FileSaver.h"

void FileSaver::open(std::string_view file_name) {
    file.open(file_name.data());
    if (file.is_open())
        std::runtime_error("FileSaver: cannot open file");
}

void FileSaver::save(std::string_view data) {
    if (!file.is_open())
        std::runtime_error("FileSaver: cannot write data, file is not open");
    file << data;
}

void FileSaver::close() {
    file.close();
}

FileSaver::~FileSaver() {
    if (file.is_open())
        file.close();
}