#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "engine/base_module.h"
#include "engine/module_manager.h"

using namespace engine;

class MockBaseModule : public BaseModule {
public:
    MOCK_METHOD(bool, DoInit, (), (override));
    MOCK_METHOD(bool, DoLoad, (), (override));
    MOCK_METHOD(bool, DoUnload, (), (override));
};

class BaseModuleTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BaseModuleTest, TestInitFailed) {
    MockBaseModule module;
    EXPECT_FALSE(module.Init(nullptr));
    auto manager = std::make_unique<ModuleManager>();
    EXPECT_CALL(module, DoInit()).WillOnce(testing::Return(false));
    EXPECT_FALSE(module.Init(manager.get()));
}

TEST_F(BaseModuleTest, TestInitSucc) {
    auto manager = std::make_unique<ModuleManager>();
    MockBaseModule module;
    EXPECT_CALL(module, DoInit()).WillOnce(testing::Return(true));
    EXPECT_TRUE(module.Init(manager.get()));
}

TEST_F(BaseModuleTest, TestLoadFailed) {
    auto manager = std::make_unique<ModuleManager>();
    MockBaseModule module;
    EXPECT_FALSE(module.loaded_);
    module.loaded_ = true;
    EXPECT_TRUE(module.Load());
    module.loaded_ = false;
    EXPECT_CALL(module, DoLoad()).WillOnce(testing::Return(false));
    EXPECT_FALSE(module.Load());
}

TEST_F(BaseModuleTest, TestLoadSucc) {
    auto manager = std::make_unique<ModuleManager>();
    MockBaseModule module;
    EXPECT_FALSE(module.loaded_);
    module.loaded_ = true;
    EXPECT_TRUE(module.Load());
    module.loaded_ = false;
    EXPECT_CALL(module, DoLoad()).WillOnce(testing::Return(true));
    EXPECT_TRUE(module.Load());
    EXPECT_TRUE(module.loaded_);
}

TEST_F(BaseModuleTest, TestUnloadFailed) {
    auto manager = std::make_unique<ModuleManager>();
    MockBaseModule module;
    EXPECT_FALSE(module.loaded_);
    EXPECT_TRUE(module.Unload());
    module.loaded_ = true;
    EXPECT_CALL(module, DoUnload()).WillOnce(testing::Return(false));
    EXPECT_FALSE(module.Unload());
}

TEST_F(BaseModuleTest, TestUnloadSucc) {
    auto manager = std::make_unique<ModuleManager>();
    MockBaseModule module;
    module.loaded_ = true;
    EXPECT_CALL(module, DoUnload()).WillOnce(testing::Return(true));
    EXPECT_TRUE(module.Unload());
    EXPECT_FALSE(module.loaded_);
}

TEST_F(BaseModuleTest, TestBeDepended) {
    MockBaseModule module;
    EXPECT_EQ(0, module.be_depended_modules_.size());
    auto module1 = std::make_shared<MockBaseModule>();
    module1->SetName("module1");
    auto module2 = std::make_shared<MockBaseModule>();
    module2->SetName("module2");
    module.BeDepended("module1", module1);
    module.BeDepended("module2", module2);
    EXPECT_EQ(2, module.be_depended_modules_.size());
}

TEST_F(BaseModuleTest, TestClear) {
    MockBaseModule module;
    EXPECT_EQ(0, module.be_depended_modules_.size());
    auto module1 = std::make_shared<MockBaseModule>();
    module1->SetName("module1");
    auto module2 = std::make_shared<MockBaseModule>();
    module2->SetName("module2");
    module.BeDepended("module1", module1);
    module.BeDepended("module2", module2);
    EXPECT_EQ(2, module.be_depended_modules_.size());
    module.Clear();
    EXPECT_EQ(0, module.be_depended_modules_.size());
}
