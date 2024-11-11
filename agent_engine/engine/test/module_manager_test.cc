#include <gtest/gtest.h>

#include "engine/module_manager.h"

class ModuleManagerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ModuleManagerTest, TestInit) {
    ModuleManager module_manager;
}
