#include "util/OptionParser.h"

#include <iostream>
#include <limits>
#include <stdio.h>
#include <utility>

#include "util/CommandLineParameter.h"
#include "util/Log.h"
#include "util/StringUtil.h"

using namespace std;
namespace util {
DECLARE_AND_SETUP_LOGGER(util, OptionParser);

OptionParser::OptionParser(const string &usageDescription) { usage_description_ = usageDescription; }

OptionParser::~OptionParser() {}

void OptionParser::AddOptionInfo(const OptionInfo &optionInfo, const string &shortOpt, const string &longOpt) {
    size_t index = option_infos_.size();
    if (!shortOpt.empty()) {
        short_opt2info_map_.insert(make_pair(shortOpt, index));
    }
    if (!longOpt.empty()) {
        long_opt2info_map_.insert(make_pair(longOpt, index));
    }
    option_infos_.push_back(optionInfo);
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const char *defaultValue) {
    AddOption(shortOpt, longOpt, optName, string(defaultValue));
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const string &defaultValue) {
    OptionInfo optionInfo(optName, kOptString, kStore, false);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
    str_opt_map_[optName] = defaultValue;
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const uint32_t defaultValue) {
    string defaultStringValue = StringUtil::ToString(defaultValue);
    OptionInfo optionInfo(optName, kOptUint32, kStore, false);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
    int_opt_map_[optName] = (int32_t)defaultValue;
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const int32_t defaultValue) {
    string defaultStringValue = StringUtil::ToString(defaultValue);
    OptionInfo optionInfo(optName, kOptInt32, kStore, false);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
    int_opt_map_[optName] = defaultValue;
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const bool defaultValue) {
    string defaultStringValue = StringUtil::ToString(defaultValue);
    OptionInfo optionInfo(optName, kOptBool, kStoreTrue, false);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
    bool_opt_map_[optName] = defaultValue;
}

void OptionParser::AddOption(
    const string &shortOpt, const string &longOpt, const string &optName, const OptionType type, bool isRequired) {
    OptionInfo optionInfo(optName, type, kStore, isRequired);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
}

void OptionParser::AddOption(const string &shortOpt,
                             const string &longOpt,
                             const string &optName,
                             const OptionAction &action,
                             const OptionType type,
                             bool isRequired) {
    OptionInfo optionInfo(optName, type, action, isRequired);
    AddOptionInfo(optionInfo, shortOpt, longOpt);
}

bool OptionParser::ParseArgs(int argc, char **argv) {
    string option;
    for (int i = 0; i < argc; ++i) {
        string optName = string(argv[i]);
        size_t index;
        ShortOpt2InfoMap::const_iterator shortIt = short_opt2info_map_.find(optName);
        if (shortIt == short_opt2info_map_.end()) {
            LongOpt2InfoMap::const_iterator longIt = long_opt2info_map_.find(optName);
            if (longIt == long_opt2info_map_.end()) {
                continue;
            } else {
                index = longIt->second;
            }
        } else {
            index = shortIt->second;
        }
        OptionInfo &info = option_infos_[index];
        if (info.optionType == kOptHelp) {
            cout << usage_description_ << endl;
            return false;
        }

        string optarg;
        if (info.HasValue()) {

            if (i + 1 >= argc) {
                fprintf(stderr, "Option parse error: option [%s] must have value!\n", optName.c_str());
                return false;
            } else {
                ShortOpt2InfoMap::const_iterator shortIt = short_opt2info_map_.find(argv[i + 1]);
                LongOpt2InfoMap::const_iterator longIt = long_opt2info_map_.find(argv[i + 1]);
                if (shortIt != short_opt2info_map_.end() || (longIt != long_opt2info_map_.end())) {
                    fprintf(stderr, "Option parse error: option [%s] must have value!\n", optName.c_str());
                    return false;
                }
                optarg = argv[++i];
            }
        }

        info.isSet = true;
        switch (info.optionType) {
        case kOptString:
            if (!info.isMultiValue) {
                str_opt_map_[info.optionName] = optarg;
            } else {
                multi_value_str_opt_map_[info.optionName].push_back(optarg);
            }
            break;
        case kOptInt32: {
            int32_t intValue;
            if (StringUtil::StrToInt32(optarg.c_str(), intValue) == false) {
                fprintf(stderr,
                        "Option parse error: invalid int32 value[%s] for option [%s]\n",
                        optarg.c_str(),
                        optName.c_str());
                return false;
            }
            int_opt_map_[info.optionName] = intValue;
            break;
        }
        case kOptUint32: {
            int64_t intValue;
            if (StringUtil::StrToInt64(optarg.c_str(), intValue) == false ||
                intValue < numeric_limits<uint32_t>::min() || intValue > numeric_limits<uint32_t>::max()) {
                fprintf(stderr,
                        "Option parse error: invalid uint32 value[%s] for option [%s]\n",
                        optarg.c_str(),
                        optName.c_str());
                return false;
            }
            int_opt_map_[info.optionName] = (int32_t)intValue;
            break;
        }
        case kOptBool:
            bool_opt_map_[info.optionName] = info.optionAction == kStoreTrue ? true : false;
            break;
        default:
            continue;
        }
    }

    if (!IsValidArgs()) {
        return false;
    }
    return true;
}

bool OptionParser::IsValidArgs() {
    for (OptionInfos::const_iterator it = option_infos_.begin(); it != option_infos_.end(); ++it) {
        OptionInfo info = (*it);
        if (info.isRequired && info.isSet == false) {
            fprintf(stderr, "Option parse error: missing required option [%s]\n", info.optionName.c_str());
            return false;
        }
    }

    return true;
}

bool OptionParser::ParseArgs(const string &commandString) {
    CommandLineParameter cmdLinePara(commandString);
    return ParseArgs(cmdLinePara.GetArgc(), cmdLinePara.GetArgv());
}

bool OptionParser::GetOptionValue(const string &optName, string &value) const {
    StrOptMap::const_iterator it = str_opt_map_.find(optName);
    if (it != str_opt_map_.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool OptionParser::GetOptionValue(const string &optName, bool &value) const {
    BoolOptMap::const_iterator it = bool_opt_map_.find(optName);
    if (it != bool_opt_map_.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool OptionParser::GetOptionValue(const string &optName, int32_t &value) const {
    IntOptMap::const_iterator it = int_opt_map_.find(optName);
    if (it != int_opt_map_.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool OptionParser::GetOptionValue(const string &optName, uint32_t &value) const {
    IntOptMap::const_iterator it = int_opt_map_.find(optName);
    if (it != int_opt_map_.end()) {
        value = (uint32_t)it->second;
        return true;
    }
    return false;
}

bool OptionParser::GetOptionValue(const string &optName, vector<string> &valueVec) const {
    MultiValueStrOptMap::const_iterator it = multi_value_str_opt_map_.find(optName);
    if (it != multi_value_str_opt_map_.end()) {
        valueVec.clear();
        valueVec.insert(valueVec.end(), it->second.begin(), it->second.end());
        return true;
    }
    return false;
}

} // namespace util
