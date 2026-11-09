// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initWedgeTexCoords(pybind11::class_<ElementType, Options...>& c)
{
    using WedgeTexCoordType = TexCoordd;

    namespace py = pybind11;

    c.def(
        "wedge_tex_coord",
        [](ElementType& e, uint i) -> auto& {
            return e.wedge(i).texCoord();
        },
        py::return_value_policy::reference);
    c.def(
        "wedge_tex_coord_mod",
        [](ElementType& e, int i) -> auto& {
            const int n = e.vertexCount();
            return e.wedge(((i % n) + n) % n).texCoord();
        },
        py::return_value_policy::reference);

    c.def(
        "set_wedge_tex_coord",
        [](ElementType& e, uint i, const WedgeTexCoordType& w) {
            e.wedge(i).texCoord() = w;
        });

    c.def(
        "set_wedge_tex_coord_mod",
        [](ElementType& e, int i, const WedgeTexCoordType& w) {
            const int n = e.vertexCount();
            e.wedge(((i % n) + n) % n).texCoord() = w;
        });

    c.def(
        "set_wedge_tex_coords",
        [](ElementType& e, const std::vector<WedgeTexCoordType>& v) {
            for (uint i = 0; i < std::min<uint>(e.vertexCount(), v.size());
                 ++i) {
                e.wedge(i).texCoord() = v[i];
            }
        });

    using WedgeTexCoordsView =
        decltype(std::declval<ElementType&>().wedges().texCoords());

    if (!registeredTypes.contains(typeid(WedgeTexCoordsView))) {
        // inner class that allows to iterate over wedge tex coords
        pybind11::class_<WedgeTexCoordsView> v(c, "_WedgeTexCoordsRange");
        v.def(
            "__iter__",
            [](WedgeTexCoordsView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(WedgeTexCoordsView));
    }

    c.def("wedge_tex_coords", [](ElementType& e) {
        return e.wedges().texCoords();
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
