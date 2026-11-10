// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initAdjacentFaces(pybind11::class_<ElementType, Options...>& c)
{
    using FaceType =
        std::remove_cvref_t<decltype(std::declval<ElementType&>().adjFace(0))>;

    namespace py = pybind11;

    static const int  N    = ElementType::ADJ_FACE_COUNT;
    static const bool TTVC =
        comp::ComponentTypeT<CompId::ADJACENT_FACES, ElementType>::
            TIED_TO_VERTEX_COUNT;

    c.def(
        "adj_face_count", [](const ElementType& e) { return e.adjFaceCount(); });

    c.def("adj_face", [](ElementType& e, uint i) -> FaceType {
        return e.adjFace(i);
    });
    c.def("adj_face_mod", [](ElementType& e, int i) -> FaceType {
        return e.adjFaceMod(i);
    });

    c.def("adj_face_index", [](const ElementType& e, uint i) {
        if (i >= e.adjFaceCount()) {
            throw py::index_error();
        }
        return e.adjFaceIndex(i);
    });
    c.def("adj_face_index_mod", [](const ElementType& e, int i) {
        return e.adjFaceIndexMod(i);
    });

    c.def("set_adj_face", [](ElementType& e, uint i, const FaceType& f) {
        e.setAdjFace(i, f);
    });
    c.def("set_adj_face", [](ElementType& e, uint i, uint fi) {
        e.setAdjFace(i, fi);
    });
    c.def("set_adj_face_mod", [](ElementType& e, int i, const FaceType& f) {
        e.setAdjFaceMod(i, f);
    });
    c.def("set_adj_face_mod", [](ElementType& e, int i, uint fi) {
        e.setAdjFaceMod(i, fi);
    });

    c.def("set_adj_faces", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjFaces(v);
    });

    c.def("set_adj_faces", [](ElementType& e, const std::vector<FaceType>& v) {
        e.setAdjFaces(v);
    });

    c.def("contains_adj_face", [](const ElementType& e, const FaceType& f) {
        return e.containsAdjFace(f);
    });
    c.def("contains_adj_face", [](const ElementType& e, uint fi) {
        return e.containsAdjFace(fi);
    });

    c.def("index_of_adj_face", [](const ElementType& e, const FaceType& f) {
        return e.indexOfAdjFace(f);
    });
    c.def("index_of_adj_face", [](const ElementType& e, uint fi) {
        return e.indexOfAdjFace(fi);
    });

    if constexpr (N < 0 && !TTVC) {
        c.def("resize_adj_faces", [](ElementType& e, uint n) {
            e.resizeAdjFaces(n);
        });
        c.def("push_adj_face", [](ElementType& e, const FaceType& f) {
            e.pushAdjFace(f);
        });
        c.def("push_adj_face", [](ElementType& e, uint fi) {
            e.pushAdjFace(fi);
        });
        c.def(
            "insert_adj_face",
            [](ElementType& e, uint i, const FaceType& f) {
                e.insertAdjFace(i, f);
            });
        c.def("insert_adj_face", [](ElementType& e, uint i, uint fi) {
            e.insertAdjFace(i, fi);
        });
        c.def("erase_adj_face", [](ElementType& e, uint i) {
            e.eraseAdjFace(i);
        });
        c.def("clear_adj_faces", [](ElementType& e) {
            e.clearAdjFaces();
        });
    }

    using AdjFaceView = decltype(std::declval<ElementType&>().adjFaces());

    if (!registeredTypes.contains(typeid(AdjFaceView))) {
        // inner class that allows to iterate over adj faces
        pybind11::class_<AdjFaceView> v(c, "_AdjFaceRange");
        v.def(
            "__iter__",
            [](AdjFaceView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const AdjFaceView& v) { return v.size(); });
        v.def("__getitem__", [](AdjFaceView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        v.def("__bool__", [](const AdjFaceView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjFaceView));
    }

    c.def("adj_faces", [](ElementType& e) { return e.adjFaces(); });

    using AdjFaceIndexView =
        decltype(std::declval<const ElementType&>().adjFaceIndices());

    if (!registeredTypes.contains(typeid(AdjFaceIndexView))) {
        // inner class that allows to iterate over adj face indices
        pybind11::class_<AdjFaceIndexView> vi(c, "_AdjFaceIndexRange");
        vi.def(
            "__iter__",
            [](AdjFaceIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        vi.def("__len__", [](const AdjFaceIndexView& v) { return v.size(); });
        vi.def("__getitem__", [](AdjFaceIndexView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        vi.def("__bool__", [](const AdjFaceIndexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjFaceIndexView));
    }

    c.def("adj_face_indices", [](const ElementType& e) {
        return e.adjFaceIndices();
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H
