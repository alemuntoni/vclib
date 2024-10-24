from . import common

def generate_elem_range_concept(element):
    include_file = "mesh/" + element.name + '_range.h'
    module_include_file = "vclib/concepts/ranges/mesh/" + element.name + '_range.h'
    target_file = "include/" + module_include_file
    template_file = "concepts/ranges/mesh/element_range.h"

    common.insert_include_in_file("include/vclib/concepts/ranges/mesh.h", include_file)

    common.insert_include_in_file("src/vclib/concepts/ranges/mesh.ixx", module_include_file, True)

    return common.simple_string_replacement(target_file, template_file, element)
