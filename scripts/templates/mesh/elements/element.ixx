%HEADER%

module;

export module vclib.mesh.elements.%EL%;

import vclib.concepts.mesh.elements;
import vclib.mesh.components;
import vclib.mesh.components.parent_mesh_pointer;
import vclib.mesh.elements.element;
import vclib.types;

export {
#include <vclib/mesh/elements/%EL%.h>
#include <vclib/mesh/elements/%EL%_components.h>
}
