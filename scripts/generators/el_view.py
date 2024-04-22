from . import common

def generate_elem_view(element):    
    include_file = "elements/" + element.name + '.h'
    module_include_file = "vclib/views/mesh/" + include_file
    target_file = "include/" + module_include_file
    template_file = "views/mesh/element.h"

    common.insert_include_in_file("include/vclib/views/mesh/elements.h", include_file)

    common.insert_include_in_file("src/vclib/views/mesh/elements.ixx", module_include_file, True)

    return common.simple_string_replacement(target_file, template_file, element)
