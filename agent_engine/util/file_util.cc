#include "util/file_util.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace util {

FileUtil::FileUtil() {}
FileUtil::~FileUtil() {}

bool FileUtil::IsExist(const std::string &file_path) { return std::filesystem::exists(file_path); }

bool FileUtil::ReadFile(const std::string &file_path, std::string &content) {
    if (!IsExist(file_path)) {
        return false;
    }

    std::ifstream file(file_path, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    auto fileSize = file.tellg();
    if (fileSize == -1) {
        return false;
    }

    content.reserve(fileSize);
    file.seekg(0, std::ios::beg);
    content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return true;
}

} // namespace util
