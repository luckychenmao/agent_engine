#include "util/CommandLineParameter.h"

#include <iosfwd>
#include <stdint.h>
#include <string.h>

#include "util/Log.h"
#include "util/StringTokenizer.h"

namespace util {
DECLARE_AND_SETUP_LOGGER(util, CommandLineParameter);

using namespace std;

CommandLineParameter::CommandLineParameter(const string &cmd) {
    // tokenize the cmd
    StringTokenizer st(cmd, " ", StringTokenizer::kTokenTrim | StringTokenizer::kTokenIgnoreEmpty);

    // copy tokenized cmd into argv
    argc_ = st.GetNumTokens();
    ;
    argv_ = new char *[argc_];
    for (int32_t i = 0; i < argc_; ++i) {
        int32_t size = st[i].length() + 1;
        argv_[i] = new char[size];
        strncpy(argv_[i], st[i].c_str(), size);
        LOG(TRACE3, "argv[%d]: %s", i, argv_[i]);
    }
}

CommandLineParameter::~CommandLineParameter() {
    for (int32_t i = 0; i < argc_; ++i) {
        delete[] argv_[i];
        argv_[i] = NULL;
    }
    delete[] argv_;
    argv_ = NULL;
}

} // namespace util
