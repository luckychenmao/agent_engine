#!/usr/bin/python3

import os
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: new_class.py ClassName")
        return

    class_name = sys.argv[1]
    current_dir = os.path.basename(os.getcwd())

    h_filename = f"{class_name}.h"
    cpp_filename = f"{class_name}.cpp"

    if os.path.exists(h_filename) or os.path.exists(cpp_filename):
        print(f"Error: {h_filename} or {cpp_filename} already exists.")
        return

    h_content = f"""#pragma once

#include "util/Log.h"

namespace {current_dir} {{

class {class_name} {{
public:
    {class_name}();
    ~{class_name}();
    {class_name}(const {class_name} &) = delete;
    {class_name} &operator=(const {class_name} &) = delete;

public:
private:
    LOG_DECLARE();

}};

}} // namespace {current_dir}
"""

    cpp_content = f"""#include "{current_dir}/{class_name}.h"

namespace {current_dir} {{
LOG_SETUP({current_dir}, {class_name});

{class_name}::{class_name}() {{}}
{class_name}::~{class_name}() {{}}

}} // namespace {current_dir}
"""

    with open(h_filename, 'w') as h_file:
        h_file.write(h_content)

    with open(cpp_filename, 'w') as cpp_file:
        cpp_file.write(cpp_content)

    print(f"{h_filename} and {cpp_filename} have been created.")

if __name__ == "__main__":
    main()
