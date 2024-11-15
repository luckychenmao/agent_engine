#include <gtest/gtest.h>
#include <curl/curl.h>

class gtest_demo_libcurl_multi_test : public ::testing::Test {
protected:
    // 如果需要在每个测试之前/之后执行的代码
    void SetUp() override {
        // 初始化代码，如分配内存或打开文件
    }

    void TearDown() override {
        // 清理代码，如释放内存或关闭文件
    }
};



TEST_F(gtest_demo_libcurl_multi_test, InitTest){
    CURL *eh = curl_easy_init();
    EXPECT_NE(eh, nullptr);
}