#include "util/time_utility.h"

#include <gtest/gtest.h>

namespace util {

class TimeUtilityTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TimeUtilityTest, simpleTest) {
    EXPECT_EQ(1, TimeUtility::UsToMs(1000));
    EXPECT_EQ(1000, TimeUtility::MsToUs(1));
    EXPECT_EQ(1, TimeUtility::MsToSec(1000));
    EXPECT_EQ(1000, TimeUtility::SecToMs(1));
    EXPECT_EQ(1, TimeUtility::UsToSec(1000000));
    EXPECT_EQ(1000000, TimeUtility::SecToUs(1));
}

} // namespace util
