#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>
using namespace rapidjson;
class gtest_demo_rapidjson_test : public ::testing::Test {
protected:
    // 如果需要在每个测试之前/之后执行的代码
    void SetUp() override {
        // 初始化代码，如分配内存或打开文件
    }

    void TearDown() override {
        // 清理代码，如释放内存或关闭文件
    }
};



TEST_F(gtest_demo_rapidjson_test, InitTest){
// JSON 字符串
    const char* json = R"({
        "id":"202411151229466580e54c15474c16",
        "created":1731644986,
        "model":"glm-4-flash",
        "choices":[{"index":0,"finish_reason":"stop","delta":{"role":"assistant","content":""}}],
        "usage":{"prompt_tokens":33,"completion_tokens":10,"total_tokens":43}
    })";

     // 创建一个 RapidJSON document 对象
    rapidjson::Document doc;
    
    // 解析 JSON 字符串
    if (doc.Parse(json).HasParseError()) {
        std::cerr << "Error parsing JSON!" << std::endl;
        FAIL();
    }

    // 访问 JSON 中的数据
    std::cout << "ID: " << doc["id"].GetString() << std::endl;
    std::cout << "Created: " << doc["created"].GetInt() << std::endl;
    std::cout << "Model: " << doc["model"].GetString() << std::endl;

    // 遍历 choices 数组
    const rapidjson::Value& choices = doc["choices"];
    for (SizeType i = 0; i < choices.Size(); i++) {
        const rapidjson::Value& choice = choices[i];
        std::cout << "Choice " << i << " Index: " << choice["index"].GetInt() << std::endl;
        std::cout << "Finish Reason: " << choice["finish_reason"].GetString() << std::endl;
        const rapidjson::Value& delta = choice["delta"];
        std::cout << "Role: " << delta["role"].GetString() << std::endl;
        std::cout << "Content: " << delta["content"].GetString() << std::endl;
    }

    // 访问 usage 对象
    const rapidjson::Value& usage = doc["usage"];
    std::cout << "Prompt Tokens: " << usage["prompt_tokens"].GetInt() << std::endl;
    std::cout << "Completion Tokens: " << usage["completion_tokens"].GetInt() << std::endl;
    std::cout << "Total Tokens: " << usage["total_tokens"].GetInt() << std::endl;

    EXPECT_EQ(usage["total_tokens"].GetInt(), 43);
}