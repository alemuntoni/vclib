// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initVertexReferences(pybind11::class_<ElementType, Options...>& c)
{
    using VertexType = vcl::VertexType<ElementType>;

    namespace py = pybind11;

    static const int N = ElementType::VERTEX_COUNT;

    c.def("vertex_count", [](const ElementType& e) { return e.vertexCount(); });

    c.def("vertex", [](ElementType& e, uint i) -> VertexType {
        return e.vertex(i);
    });
    c.def("vertex_mod", [](ElementType& e, int i) -> VertexType {
        return e.vertexMod(i);
    });

    c.def("set_vertex", [](ElementType& e, uint i, const VertexType& v) {
        e.setVertex(i, v);
    });
    c.def("set_vertex", [](ElementType& e, uint i, uint vi) {
        e.setVertex(i, vi);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, const VertexType& v) {
        e.setVertexMod(i, v);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, uint vi) {
        e.setVertexMod(i, vi);
    });

    c.def("set_vertices", [](ElementType& e, const std::vector<uint>& v) {
        e.setVertices(v);
    });

    c.def("set_vertices", [](ElementType& e, const std::vector<VertexType>& v) {
        e.setVertices(v);
    });

    c.def("contains_vertex", [](const ElementType& e, const VertexType& v) {
        return e.containsVertex(v);
    });
    c.def("contains_vertex", [](const ElementType& e, uint vi) {
        return e.containsVertex(vi);
    });
    c.def("index_of_vertex", [](const ElementType& e, const VertexType& v) {
        return e.indexOfVertex(v);
    });
    c.def("index_of_vertex", [](const ElementType& e, uint vi) {
        return e.indexOfVertex(vi);
    });
    c.def(
        "index_of_edge",
        [](const ElementType& e, const VertexType& v1, const VertexType& v2) {
            return e.indexOfEdge(v1, v2);
        });
    c.def(
        "index_of_edge", [](const ElementType& e, uint vi1, uint vi2) {
            return e.indexOfEdge(vi1, vi2);
        });

    if constexpr (N < 0) {
        c.def("resize_vertices", [](ElementType& e, uint n) {
            e.resizeVertices(n);
        });
        c.def("push_vertex", [](ElementType& e, const VertexType& v) {
            e.pushVertex(v);
        });
        c.def("push_vertex", [](ElementType& e, uint vi) {
            e.pushVertex(vi);
        });
        c.def(
            "insert_vertex",
            [](ElementType& e, uint i, const VertexType& v) {
                e.insertVertex(i, v);
            });
        c.def("insert_vertex", [](ElementType& e, uint i, uint vi) {
            e.insertVertex(i, vi);
        });
        c.def("erase_vertex", [](ElementType& e, uint i) {
            e.eraseVertex(i);
        });
        c.def("clear_vertices", [](ElementType& e) {
            e.clearVertices();
        });
    }

    using VertexView = decltype(std::declval<ElementType&>().vertices());

    if (!registeredTypes.contains(typeid(VertexView))) {
        // inner class that allows to iterate over vertices
        pybind11::class_<VertexView> v(c, "_VertexReferencesRange");
        v.def(
            "__iter__",
            [](VertexView& v) { return py::make_iterator(v.begin(), v.end()); },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(VertexView));
    }

    c.def("vertices", [](ElementType& e) { return e.vertices(); });

    using VertexIndexView =
        decltype(std::declval<const ElementType&>().vertexIndices());

    if (!registeredTypes.contains(typeid(VertexIndexView))) {
        // inner class that allows to iterate over vertex indices
        pybind11::class_<VertexIndexView> vi(c, "_VertexReferencesIndexRange");
        vi.def(
            "__iter__",
            [](VertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(VertexIndexView));
    }

    c.def("vertex_indices", [](const ElementType& e) {
        return e.vertexIndices();
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H
