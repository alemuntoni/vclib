// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initAdjacentVertices(pybind11::class_<ElementType, Options...>& c)
{
    using VertexType =
        std::remove_cvref_t<decltype(std::declval<ElementType&>().adjVertex(0))>;

    namespace py = pybind11;

    c.def(
        "adj_vertex_count",
        [](const ElementType& e) { return e.adjVertexCount(); });

    c.def("adj_vertex", [](ElementType& e, uint i) -> VertexType {
        return e.adjVertex(i);
    });
    c.def("adj_vertex_mod", [](ElementType& e, int i) -> VertexType {
        return e.adjVertexMod(i);
    });

    c.def("set_adj_vertex", [](ElementType& e, uint i, const VertexType& v) {
        e.setAdjVertex(i, v);
    });
    c.def("set_adj_vertex", [](ElementType& e, uint i, uint vi) {
        e.setAdjVertex(i, vi);
    });
    c.def("set_adj_vertex_mod", [](ElementType& e, int i, const VertexType& v) {
        e.setAdjVertexMod(i, v);
    });
    c.def("set_adj_vertex_mod", [](ElementType& e, int i, uint vi) {
        e.setAdjVertexMod(i, vi);
    });

    c.def("set_adj_vertices", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjVertices(v);
    });

    c.def(
        "set_adj_vertices",
        [](ElementType& e, const std::vector<VertexType>& v) {
            e.setAdjVertices(v);
        });

    c.def("contains_adj_vertex", [](const ElementType& e, const VertexType& v) {
        return e.containsAdjVertex(v);
    });
    c.def("contains_adj_vertex", [](const ElementType& e, uint vi) {
        return e.containsAdjVertex(vi);
    });

    c.def("index_of_adj_vertex", [](const ElementType& e, const VertexType& v) {
        return e.indexOfAdjVertex(v);
    });
    c.def("index_of_adj_vertex", [](const ElementType& e, uint vi) {
        return e.indexOfAdjVertex(vi);
    });

    c.def("resize_adj_vertices", [](ElementType& e, uint n) {
        e.resizeAdjVertices(n);
    });
    c.def("push_adj_vertex", [](ElementType& e, const VertexType& v) {
        e.pushAdjVertex(v);
    });
    c.def("push_adj_vertex", [](ElementType& e, uint vi) {
        e.pushAdjVertex(vi);
    });
    c.def(
        "insert_adj_vertex",
        [](ElementType& e, uint i, const VertexType& v) {
            e.insertAdjVertex(i, v);
        });
    c.def("insert_adj_vertex", [](ElementType& e, uint i, uint vi) {
        e.insertAdjVertex(i, vi);
    });
    c.def("erase_adj_vertex", [](ElementType& e, uint i) {
        e.eraseAdjVertex(i);
    });
    c.def("clear_adj_vertices", [](ElementType& e) {
        e.clearAdjVertices();
    });

    using AdjVertexView = decltype(std::declval<ElementType&>().adjVertices());

    if (!registeredTypes.contains(typeid(AdjVertexView))) {
        // inner class that allows to iterate over adj vertices
        pybind11::class_<AdjVertexView> v(c, "_AdjVertexRange");
        v.def(
            "__iter__",
            [](AdjVertexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(AdjVertexView));
    }

    c.def("adj_vertices", [](ElementType& e) { return e.adjVertices(); });

    using AdjVertexIndexView =
        decltype(std::declval<const ElementType&>().adjVertexIndices());

    if (!registeredTypes.contains(typeid(AdjVertexIndexView))) {
        // inner class that allows to iterate over adj vertex indices
        pybind11::class_<AdjVertexIndexView> vi(c, "_AdjVertexIndexRange");
        vi.def(
            "__iter__",
            [](AdjVertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(AdjVertexIndexView));
    }

    c.def("adj_vertex_indices", [](const ElementType& e) {
        return e.adjVertexIndices();
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H
