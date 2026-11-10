// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

namespace detail {

template<ElemId ELEM_ID, CompId COMP_ID, MeshConcept MeshType, typename... Options>
void addOptionalComponentFunctions(
    pybind11::class_<MeshType, Options...>& c,
    const std::string&                      name,
    const std::string&                      compName)
{
    if constexpr (MeshType::template hasElementOptionalComponent<
                      ELEM_ID,
                      COMP_ID>()) {
        c.def(
            ("is_per_" + name + "_" + compName + "_enabled").c_str(),
            [](const MeshType& t) {
                return t
                    .template isElementComponentAvailable<ELEM_ID, COMP_ID>();
            });
        c.def(("enable_per_" + name + "_" + compName).c_str(), [](MeshType& t) {
            t.template enableElementComponent<ELEM_ID, COMP_ID>();
        });
        c.def(
            ("disable_per_" + name + "_" + compName).c_str(), [](MeshType& t) {
                t.template disableElementComponent<ELEM_ID, COMP_ID>();
            });
    }
}

} // namespace detail

template<ElementConcept Element, MeshConcept MeshType, typename... Options>
void initContainer(
    pybind11::class_<MeshType, Options...>& c,
    const std::string&                      name,
    std::string                             namePlural = "")
{
    namespace py = pybind11;

    static const ElemId ELEM_ID = Element::ELEMENT_ID;

    std::string capitalName = name;
    capitalName[0]          = std::toupper(capitalName[0]);

    if (namePlural.empty())
        namePlural = name + "s";

    c.def("index", [](MeshType&, const Element& e) -> uint {
        return e.index();
    });

    c.def(
        name.c_str(),
        [](MeshType& t, uint i) -> Element {
            return t.template element<ELEM_ID>(i);
        });

    c.def((name + "_count").c_str(), [](const MeshType& t) {
        return t.template count<ELEM_ID>();
    });
    c.def((name + "_container_size").c_str(), [](const MeshType& t) {
        return t.template containerSize<ELEM_ID>();
    });
    c.def(("deleted_" + name + "_count").c_str(), [](const MeshType& t) {
        return t.template deletedCount<ELEM_ID>();
    });

    c.def(("add_" + name).c_str(), [](MeshType& t) {
        return t.template add<ELEM_ID>();
    });

    c.def(("add_" + namePlural).c_str(), [](MeshType& t, uint n) {
        return t.template add<ELEM_ID>(n);
    });

    c.def(("clear_" + namePlural).c_str(), [](MeshType& t) {
        t.template clearElements<ELEM_ID>();
    });
    c.def(("resize_" + namePlural).c_str(), [](MeshType& t, uint n) {
        t.template resize<ELEM_ID>(n);
    });
    c.def(("reserve_" + namePlural).c_str(), [](MeshType& t, uint n) {
        t.template reserve<ELEM_ID>(n);
    });

    c.def(("compact_" + namePlural).c_str(), [](MeshType& t) {
        return t.template compactElements<ELEM_ID>();
    });

    c.def(("compact_" + name + "_indices").c_str(), [](const MeshType& t) {
        return t.template compactIndices<ELEM_ID>();
    });

    c.def(("delete_" + name).c_str(), [](MeshType& t, uint i) {
        t.template deleteElement<ELEM_ID>(i);
    });
    c.def(("delete_" + name).c_str(), [](MeshType& t, const Element& e) {
        t.template deleteElement<ELEM_ID>(e.index());
    });

    using ElemView =
        decltype(std::declval<MeshType&>().template elements<ELEM_ID>(true));

    if (!registeredTypes.contains(typeid(ElemView))) {
        // inner class that allows to iterate over elements
        pybind11::class_<ElemView> v(c, ("_" + capitalName + "Range").c_str());

        v.def(
            "__iter__",
            [](ElemView& t) { return py::make_iterator(t.begin(), t.end()); },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const ElemView& t) { return t.size(); });

        v.def("__getitem__", [](ElemView& t, int i) {
            int s = static_cast<int>(t.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return t[static_cast<uint>(i)];
        });

        v.def("__bool__", [](const ElemView& t) { return !t.empty(); });

        registeredTypes.insert(typeid(ElemView));
    }

    c.def(
        namePlural.c_str(),
        [](MeshType& t, bool jumpDeleted) {
            return t.template elements<ELEM_ID>(jumpDeleted);
        },
        py::arg("jump_deleted") = true);

    // optional components

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_EDGES>(
        c, name, "adjacent_edges");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_FACES>(
        c, name, "adjacent_faces");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::ADJACENT_VERTICES>(
        c, name, "adjacent_vertices");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::COLOR>(
        c, name, "color");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::MATERIAL_INDEX>(
        c, name, "material_index");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::NORMAL>(
        c, name, "normal");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::PRINCIPAL_CURVATURE>(
        c, name, "principal_curvature");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::QUALITY>(
        c, name, "quality");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::TANGENT>(
        c, name, "tangent");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::TEX_COORD>(
        c, name, "tex_coord");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_COLORS>(
        c, name, "wedge_colors");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_NORMALS>(
        c, name, "wedge_normals");

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::WEDGE_TEX_COORDS>(
        c, name, "wedge_tex_coords");
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_CONTAINER_H
