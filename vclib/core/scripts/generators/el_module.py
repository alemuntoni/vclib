from . import common

import bisect

def generate_elem_module(element):
    target_file = "src/vclib/mesh/elements/" + element.name + '.ixx'
    template_file = "mesh/elements/element.ixx"

    elem_module_file = common.simple_string_replacement(target_file, template_file, element)

    # Read in the file
    with open('../src/vclib/mesh/elements.ixx', 'r') as file :
        elements_file = file.read()

    start = 'export import'
    start_index = elements_file.index(start)
    end_index = elements_file.rfind('export import')
    end_index = elements_file.index('\n', end_index)

    imports = elements_file[start_index + len(start):(end_index)].split('\n')

    item = 'export import vclib.mesh.elements.' + element.name + ';'

    pos = bisect.bisect_left(imports, item)
    if pos == len(imports) or imports[pos] != item:
        bisect.insort(imports, item)

    files_string ='\n'.join(imports)

    elements_file = elements_file[:start_index + len(start)] + files_string + elements_file[end_index:]

    with open("../src/vclib/mesh/elements.ixx", 'w') as file:
        file.write(elements_file)

    return elem_module_file
