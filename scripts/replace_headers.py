import os
import glob
import re

def replace_first_between(s, start, end, replacement):
    try:
        start_index = s.index(start)
        end_index = s.index(end, start_index + len(start))
        return s[:start_index] + replacement + s[end_index + len(end):]
    except ValueError:
        return s

def replace_header(file_path, header_string):
    # Read in the file
    with open(file_path, 'r') as file :
        file_string = file.read()

    file_string = replace_first_between(file_string, '/*', '*/', header_string)

    with open(file_path, 'w') as file:
        file.write(file_string)

def replace_headers_in_dir(folder_path):
    # Read in the file
    with open('templates/header.txt', 'r') as file :
        header_string = file.read()

    for file_path in glob.glob(os.path.join(folder_path, '*')):
        if os.path.isdir(file_path):
            replace_headers_in_dir(file_path)
        elif file_path.endswith(('.h', '.cpp')):
            replace_header(file_path, header_string)

if __name__ == "__main__":
    replace_headers_in_dir('../include/vclib/')