
#include "Properties.h"

#include <cstdlib>
#include <utility>

#include "StringUtil.h"

using namespace std;

namespace alog {

Properties::Properties()
{
}

Properties::~Properties()
{
}

void Properties::load(istream& in) {
    clear();

    string fullLine;
    string command;
    string leftSide;
    string rightSide;
    char line[1024];
    string::size_type pos = string::npos;

    while (in.getline(line, 1024)) {
        fullLine = line;
        pos = fullLine.find('#');
        if (pos == string::npos) {
            command = fullLine;
        } else if (pos > 0) {
            command = fullLine.substr(0, pos);
        } else {
            continue;
        }

        pos = command.find('=');
        if (pos != string::npos) {
            leftSide = StringUtil::Trim(command.substr(0, pos));
            rightSide = StringUtil::Trim(command.substr(pos + 1, command.size() - pos));
            _substituteVariables(rightSide);
        } else {
            continue;
        }

        // strip off the "alog"
        pos = leftSide.find('.');
        if (leftSide.substr(0, pos) == "alog") {
            leftSide = leftSide.substr(pos + 1);
        }

        insert(value_type(leftSide, rightSide));
    }
}

int Properties::getInt(const string& property, int defaultValue) {
    auto it = find(property);
    if (it != end()) {
        return atoi(it->second.c_str());
    }
    return defaultValue;
}

bool Properties::getBool(const string& property, bool defaultValue) {
    auto it = find(property);
    if (it != end()) {
        return (it->second == "true");
    }
    return defaultValue;
}

string Properties::getString(const string& property, const char* defaultValue) {
    auto it = find(property);
    if (it != end()) {
        return it->second;
    }
    return string(defaultValue);
}

void Properties::_substituteVariables(string& value) {
    string result;

    string::size_type left = 0;
    string::size_type right = value.find("${", left);
    if (string::npos == right) {
        return;
    }

    while (true) {
        result += value.substr(left, right - left);
        if (string::npos == right) {
            break;
        }

        left = right + 2;
        right = value.find('}', left);
        if (string::npos == right) {
            result += value.substr(left - 2);
            break;
        } else {
            const string key = value.substr(left, right - left);
            if (key == "${") {
                result += "${";
            } else {
                char* value = getenv(key.c_str());
                if (value) {
                    result += value;
                } else {
                    auto it = find(key);
                    if (it != end()) {
                        result += (*it).second;
                    }
                }
            }
            left = right + 1;
        }
        right = value.find("${", left);
    }

    value = result;
}

}
