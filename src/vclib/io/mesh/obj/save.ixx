/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

module;

#include <fstream>
#include <map>
#include <string>
#include <vector>

export module vclib.io.mesh.obj.save;

import vclib.concepts;
import vclib.concepts.mesh.elements.element;
import vclib.io.file_info;
import vclib.io.mesh.obj.material;
import vclib.io.write;
import vclib.mesh.utils.mesh_info;
import vclib.misc;
import vclib.types;

export {
#include <vclib/io/mesh/obj/save.h>
}

// for some reason, clang does not compile the internal calls of the == operator
// between std::map iterators, and therefore it is not exported.
// Adding this operation forces the compilation inside this module.
// Without this, on clang you get an unresolved external symbol error every time
// you try to use this module, and the solution would be to `#include <map>`
// along with `import vclib;` in the client code.
// note: these dummy objects are not exported
// TODO: test if this is still necessary when clang gets updated.
std::map<vcl::detail::ObjMaterial, std::string>
    dummyMap;

bool dummyB = dummyMap.find(vcl::detail::ObjMaterial()) == dummyMap.end();
