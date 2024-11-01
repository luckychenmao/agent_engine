#pragma once

#include <string>

namespace util {

class CommandLineParameter {
public:
    CommandLineParameter(const std::string &cmd);
    ~CommandLineParameter();

public:
    inline int GetArgc() const { return argc_; };
    inline char **GetArgv() const { return argv_; };

private:
    int argc_;
    char **argv_;
};

} // namespace util
