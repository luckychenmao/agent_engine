#pragma once

#include <string>

namespace util {

class FileUtil {
public:
    FileUtil();
    ~FileUtil();
    FileUtil(const FileUtil &) = delete;
    FileUtil &operator=(const FileUtil &) = delete;

public:
    static bool IsExist(const std::string &filePath);
    static bool ReadFile(const std::string &filePath, std::string &content);
};

} // namespace util
