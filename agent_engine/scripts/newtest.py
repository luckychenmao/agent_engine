#!/usr/bin/python3

import os
import sys
import re

def camel_to_snake(name):
    # 在大写字母前添加下划线，并将其转换为小写
    s1 = re.sub('([a-z0-9])([A-Z])', r'\1_\2', name)
    # 全部转换为小写
    return s1.lower()

def create_test():
    if len(sys.argv) != 2:
        print("Usage: newtest.py ClassName")
        return

    class_name = sys.argv[1]
    current_dir = os.path.basename(os.getcwd())
    test_dir = "test"
    test_filename = f"{camel_to_snake(class_name)}_test.cc"
    test_file_path = os.path.join(test_dir, test_filename)

    if not os.path.exists(test_dir):
        os.makedirs(test_dir)

    if os.path.exists(test_file_path):
        print(f"Error: {test_file_path} already exists.")
        return

    # Content for MyClassTest.cc
    test_content = f"""#include "{current_dir}/{camel_to_snake(class_name)}.h"

#include <gtest/gtest.h>

class {class_name}Test : public ::testing::Test {{
protected:
    void SetUp() override {{}}
    void TearDown() override {{}}
}};

"""

    with open(test_file_path, 'w') as test_file:
        test_file.write(test_content)

    print(f"{test_filename} has been created in {test_dir}.")

if __name__ == "__main__":
    create_test()
