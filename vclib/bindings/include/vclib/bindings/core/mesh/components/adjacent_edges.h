// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_EDGES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initAdjacentEdges(pybind11::class_<ElementType, Options...>& c)
{
    using EdgeType =
        std::remove_cvref_t<decltype(std::declval<ElementType&>().adjEdge(0))>;

    namespace py = pybind11;

    static const int  N    = ElementType::ADJ_EDGE_COUNT;
    static const bool TTVC =
        comp::ComponentTypeT<CompId::ADJACENT_EDGES, ElementType>::
            TIED_TO_VERTEX_COUNT;

    c.def(
        "adj_edge_count", [](const ElementType& e) { return e.adjEdgeCount(); });

    c.def("adj_edge", [](ElementType& e, uint i) -> EdgeType {
        if (i >= e.adjEdgeCount()) {
            throw py::index_error();
        }
        return e.adjEdge(i);
    });
    c.def("adj_edge_mod", [](ElementType& e, int i) -> EdgeType {
        return e.adjEdgeMod(i);
    });

    c.def("adj_edge_index", [](const ElementType& e, uint i) {
        if (i >= e.adjEdgeCount()) {
            throw py::index_error();
        }
        return e.adjEdgeIndex(i);
    });
    c.def("adj_edge_index_mod", [](const ElementType& e, int i) {
        return e.adjEdgeIndexMod(i);
    });

    c.def("set_adj_edge", [](ElementType& e, uint i, const EdgeType& ed) {
        if (i >= e.adjEdgeCount()) {
            throw py::index_error();
        }
        e.setAdjEdge(i, ed);
    });
    c.def("set_adj_edge", [](ElementType& e, uint i, uint ei) {
        if (i >= e.adjEdgeCount()) {
            throw py::index_error();
        }
        e.setAdjEdge(i, ei);
    });
    c.def("set_adj_edge", [](ElementType& e, uint i, py::none) {
        if (i >= e.adjEdgeCount()) {
            throw py::index_error();
        }
        e.setAdjEdge(i, EdgeType::NULL_INDEX);
    });
    c.def("set_adj_edge_mod", [](ElementType& e, int i, const EdgeType& ed) {
        e.setAdjEdgeMod(i, ed);
    });
    c.def("set_adj_edge_mod", [](ElementType& e, int i, uint ei) {
        e.setAdjEdgeMod(i, ei);
    });
    c.def("set_adj_edge_mod", [](ElementType& e, int i, py::none) {
        e.setAdjEdgeMod(i, EdgeType::NULL_INDEX);
    });

    c.def("set_adj_edges", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjEdges(v);
    });

    c.def("set_adj_edges", [](ElementType& e, const std::vector<EdgeType>& v) {
        e.setAdjEdges(v);
    });

    c.def("contains_adj_edge", [](const ElementType& e, const EdgeType& ed) {
        return e.containsAdjEdge(ed);
    });
    c.def("contains_adj_edge", [](const ElementType& e, uint ei) {
        return e.containsAdjEdge(ei);
    });

    c.def("index_of_adj_edge", [](const ElementType& e, const EdgeType& ed) {
        return e.indexOfAdjEdge(ed);
    });
    c.def("index_of_adj_edge", [](const ElementType& e, uint ei) {
        return e.indexOfAdjEdge(ei);
    });

    if constexpr (N < 0 && !TTVC) {
        c.def("resize_adj_edges", [](ElementType& e, uint n) {
            e.resizeAdjEdges(n);
        });
        c.def("push_adj_edge", [](ElementType& e, const EdgeType& ed) {
            e.pushAdjEdge(ed);
        });
        c.def("push_adj_edge", [](ElementType& e, uint ei) {
            e.pushAdjEdge(ei);
        });
        c.def(
            "insert_adj_edge",
            [](ElementType& e, uint i, const EdgeType& ed) {
                e.insertAdjEdge(i, ed);
            });
        c.def("insert_adj_edge", [](ElementType& e, uint i, uint ei) {
            e.insertAdjEdge(i, ei);
        });
        c.def("erase_adj_edge", [](ElementType& e, uint i) {
            e.eraseAdjEdge(i);
        });
        c.def("clear_adj_edges", [](ElementType& e) {
            e.clearAdjEdges();
        });
    }

    using AdjEdgeView = decltype(std::declval<ElementType&>().adjEdges());

    if (!registeredTypes.contains(typeid(AdjEdgeView))) {
        // inner class that allows to iterate over adj edges
        pybind11::class_<AdjEdgeView> v(c, "_AdjEdgeRange");
        v.def(
            "__iter__",
            [](AdjEdgeView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const AdjEdgeView& v) { return v.size(); });
        v.def("__getitem__", [](AdjEdgeView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        v.def("__bool__", [](const AdjEdgeView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjEdgeView));
    }

    c.def("adj_edges", [](ElementType& e) { return e.adjEdges(); });

    using AdjEdgeIndexView =
        decltype(std::declval<const ElementType&>().adjEdgeIndices());

    if (!registeredTypes.contains(typeid(AdjEdgeIndexView))) {
        // inner class that allows to iterate over adj edge indices
        pybind11::class_<AdjEdgeIndexView> vi(c, "_AdjEdgeIndexRange");
        vi.def(
            "__iter__",
            [](AdjEdgeIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        vi.def("__len__", [](const AdjEdgeIndexView& v) { return v.size(); });
        vi.def("__getitem__", [](AdjEdgeIndexView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        vi.def("__bool__", [](const AdjEdgeIndexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjEdgeIndexView));
    }

    c.def("adj_edge_indices", [](const ElementType& e) {
        return e.adjEdgeIndices();
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_EDGES_H
