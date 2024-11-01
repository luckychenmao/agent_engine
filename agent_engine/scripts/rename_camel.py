#!/usr/bin/python3

import os
import re

def camel_to_snake(name):
    """
    Convert camel case string to snake case.
    """
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()

def rename_files_in_directory(directory):
    for root, _, files in os.walk(directory):
        for filename in files:
            # Separate name and extension
            name, ext = os.path.splitext(filename)

            # Convert name to snake_case
            snake_name = camel_to_snake(name)

            # Check and change .cpp to .cc
            if ext == ".cpp":
                ext = ".cc"

            new_filename = snake_name + ext

            # Build full file paths
            old_file = os.path.join(root, filename)
            new_file = os.path.join(root, new_filename)

            # Rename file
            os.rename(old_file, new_file)
            print(f"Renamed: {old_file} -> {new_file}")

if __name__ == "__main__":
    directory = input("Enter the directory path: ")
    rename_files_in_directory(directory)
