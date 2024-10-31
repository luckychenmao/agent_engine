#!/usr/bin/python3

import os
import sys

def create_test():
    if len(sys.argv) != 2:
        print("Usage: newtest.py ClassName")
        return

    class_name = sys.argv[1]
    current_dir = os.path.basename(os.getcwd())
    test_dir = "test"
    test_filename = f"{class_name}Test.cpp"
    test_file_path = os.path.join(test_dir, test_filename)

    if not os.path.exists(test_dir):
        os.makedirs(test_dir)

    if os.path.exists(test_file_path):
        print(f"Error: {test_file_path} already exists.")
        return

    # Content for MyClassTest.cpp
    test_content = f"""#include "{current_dir}/{class_name}.h"

#include <gtest/gtest.h>

namespace {current_dir} {{

class {class_name}Test : public ::testing::Test {{
protected:
    void SetUp() override {{}}
    void TearDown() override {{}}
}};

}} // namespace {current_dir}
"""

    with open(test_file_path, 'w') as test_file:
        test_file.write(test_content)

    print(f"{test_filename} has been created in {test_dir}.")

if __name__ == "__main__":
    create_test()
