#include "util/TimeUtility.h"

#include <gtest/gtest.h>

namespace util {

class TimeUtilityTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TimeUtilityTest, simpleTest) {
    EXPECT_EQ(1, TimeUtility::us2ms(1000));
    EXPECT_EQ(1000, TimeUtility::ms2us(1));
    EXPECT_EQ(1, TimeUtility::ms2sec(1000));
    EXPECT_EQ(1000, TimeUtility::sec2ms(1));
    EXPECT_EQ(1, TimeUtility::us2sec(1000000));
    EXPECT_EQ(1000000, TimeUtility::sec2us(1));
}

} // namespace util
