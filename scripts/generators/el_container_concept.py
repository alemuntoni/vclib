from . import common

def generate_elem_container_concept(element):    
    include_file = "containers/" + element.name + '_container.h'
    include_vclib_file = "vclib/concepts/mesh/" + include_file
    target_file = "include/" + include_vclib_file
    template_file = "concepts/mesh/containers/element_container.h"

    common.insert_include_in_file("include/vclib/concepts/mesh/containers.h", include_file)

    common.insert_include_in_file("src/vclib/concepts/mesh/containers.ixx", include_vclib_file, True)

    return common.simple_string_replacement(target_file, template_file, element)
