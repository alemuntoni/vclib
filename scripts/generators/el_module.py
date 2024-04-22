from . import common

def generate_elem_module(element):
    target_file = "src/vclib/mesh/elements/" + element.name + '.ixx'
    template_file = "mesh/elements/element.ixx"

    return common.simple_string_replacement(target_file, template_file, element)
