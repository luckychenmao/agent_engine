#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <stddef.h>
#include <string>
#include <vector>

namespace util {

// Note! this is not multi-thread safe!
class OptionParser {
public:
    enum OptionType {
        kOptString = 0,
        kOptInt32,
        kOptUint32,
        kOptBool,
        kOptHelp,
    };
    enum OptionAction {
        kStore,
        kStoreTrue,
        kStoreFalse,
    };

public:
    typedef std::map<std::string, std::string> StrOptMap;
    typedef std::map<std::string, bool> BoolOptMap;
    typedef std::map<std::string, int32_t> IntOptMap;
    typedef std::map<std::string, std::vector<std::string>> MultiValueStrOptMap;

public:
    OptionParser(const std::string &usageDescription = "");
    ~OptionParser();

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const OptionType type = kOptString,
                   bool isRequired = false);

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const OptionAction &action,
                   const OptionType type = kOptBool,
                   bool isRequired = false);

    // the following functions are for optional options
    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const char *defaultValue);

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const std::string &defaultValue);

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const uint32_t defaultValue);

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const int32_t defaultValue);

    void AddOption(const std::string &shortOpt,
                   const std::string &longOpt,
                   const std::string &optName,
                   const bool defaultValue);

    bool ParseArgs(int argc, char **argv);
    bool ParseArgs(const std::string &commandString);
    bool GetOptionValue(const std::string &optName, std::string &value) const;
    bool GetOptionValue(const std::string &optName, bool &value) const;
    bool GetOptionValue(const std::string &optName, int32_t &value) const;
    bool GetOptionValue(const std::string &optName, uint32_t &value) const;
    bool GetOptionValue(const std::string &optName, std::vector<std::string> &valueVec) const;

private:
    class OptionInfo {
    public:
        OptionInfo(const std::string &optionName,
                   OptionType type,
                   OptionAction action,
                   bool required,
                   const std::string &defaultValue = "",
                   bool multiValue = false)
            : optionType(type)
            , optionAction(action)
            , optionName(optionName)
            , isRequired(required)
            , isMultiValue(multiValue)

        {
            isSet = false;
        }
        ~OptionInfo() {}
        bool HasValue() { return optionAction == kStore; }

    public:
        OptionType optionType;
        OptionAction optionAction;
        std::string optionName;
        bool isRequired;
        bool isMultiValue;
        bool isSet;
    };

private:
    void AddOptionInfo(const OptionInfo &optionInfo, const std::string &shortOpt, const std::string &longOpt);

    // results:
private:
    StrOptMap str_opt_map_;
    IntOptMap int_opt_map_;
    BoolOptMap bool_opt_map_;
    MultiValueStrOptMap multi_value_str_opt_map_;
    std::string usage_description_;

    // infos:
private:
    typedef std::map<std::string, size_t> ShortOpt2InfoMap;
    typedef std::map<std::string, size_t> LongOpt2InfoMap;
    typedef std::vector<OptionInfo> OptionInfos;
    ShortOpt2InfoMap short_opt2info_map_;
    LongOpt2InfoMap long_opt2info_map_;
    OptionInfos option_infos_;

private:
    bool IsValidArgs();

private:
    friend class OptionParserTest;
};

} // namespace util
