from . import common
import bisect

def replace(cmake_file_string, list, start):
    end = ')'

    start_index = cmake_file_string.index(start)
    end_index = cmake_file_string.index(end, start_index + len(start))

    files = cmake_file_string[start_index + len(start):(end_index - 1)].split('\n')

    if list is not None:
        for new_header in list:
            item = '\t' + new_header
            pos = bisect.bisect_left(files, item)
            if pos == len(files) or files[pos] != item:
                bisect.insort(files, item)

        files_string ='\n'.join(files) + '\n'

        cmake_file_string = cmake_file_string[:start_index + len(start)] + files_string + cmake_file_string[end_index:]
    
    return cmake_file_string


def replace_headers(cmake_file_string, list):
    start = 'set(HEADERS\n'
    return replace(cmake_file_string, list, start)

def replace_modules(cmake_file_string, list):
    start = 'set(MODULES\n'
    return replace(cmake_file_string, list, start)


def update_cmake_file(file_list = None):
    target_file = "../CMakeLists.txt"

    # take from file_list all names that end with '.ixx' and put them in module_list
    module_list = [file for file in file_list if file.endswith('.ixx')]

    # remove from file_list all the names that end with '.ixx'
    file_list = [file for file in file_list if not file.endswith('.ixx')]

    # Read in the file
    with open(target_file, 'r') as file :
        cmake_file = file.read()

    cmake_file = replace_headers(cmake_file, file_list)
    cmake_file = replace_modules(cmake_file, module_list)

    with open(target_file, 'w') as file:
        file.write(cmake_file)

