// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_COMPONENT_VIEW_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_COMPONENT_VIEW_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace vcl::bind {

template<CompId COMP_ID, typename Container, typename ClassType>
void initComponentView(ClassType& scope, const std::string& className)
{
    namespace py = pybind11;

    using ViewType = vcl::mesh::ComponentView<COMP_ID, Container>;

    if (registeredTypes.contains(typeid(ViewType))) {
        return;
    }

    // If VERTEX_REFERENCES, register the underlying ReferenceVector type if not already registered
    if constexpr (COMP_ID == CompId::VERTEX_REFERENCES) {
        using RefVec = typename ViewType::RawDataType;
        using Index = typename Container::IndexType;
        if (!registeredTypes.contains(typeid(RefVec))) {
            py::class_<RefVec> rv(scope, "_ReferenceVector");
            rv.def("__len__", [](const RefVec& r) { return r.size(); });
            rv.def("__getitem__", [](const RefVec& r, int i) {
                int s = static_cast<int>(r.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                return r[static_cast<uint>(i)];
            });
            rv.def(
                "__iter__",
                [](const RefVec& r) {
                    return py::make_iterator(r.begin(), r.end());
                },
                py::keep_alive<0, 1>());
            rv.def("__repr__", [](const RefVec& r) {
                std::string s = "[";
                for (std::size_t i = 0; i < r.size(); ++i) {
                    if (i > 0) {
                        s += ", ";
                    }
                    s += std::to_string(r[i]);
                }
                s += "]";
                return s;
            });
            rv.def("__eq__", [](const RefVec& r, const std::vector<Index>& other) {
                if (r.size() != other.size()) {
                    return false;
                }
                for (std::size_t i = 0; i < r.size(); ++i) {
                    if (r[i] != other[i]) {
                        return false;
                    }
                }
                return true;
            });
            rv.def("__eq__", [](const RefVec& r, const RefVec& other) {
                if (r.size() != other.size()) {
                    return false;
                }
                for (std::size_t i = 0; i < r.size(); ++i) {
                    if (r[i] != other[i]) {
                        return false;
                    }
                }
                return true;
            });
            registeredTypes.insert(typeid(RefVec));
        }
    }

    // Register class with buffer_protocol support
    py::class_<ViewType> cv(scope, className.c_str(), py::buffer_protocol());

    cv.def("__len__", [](const ViewType& self) { return self.size(); });

    cv.def(
        "__getitem__",
        [](ViewType& self, int i) -> decltype(auto) {
            int s = static_cast<int>(self.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return self[static_cast<uint>(i)];
        },
        py::return_value_policy::reference_internal);

    cv.def(
        "__iter__",
        [](ViewType& self) {
            return py::make_iterator(self.begin(), self.end());
        },
        py::keep_alive<0, 1>());

    cv.def("__bool__", [](const ViewType& self) { return !self.empty(); });

    cv.def("is_available", [](const ViewType& self) {
        return self.isAvailable();
    });

    cv.def("is_compact", [](const ViewType& self) {
        return self.isCompact();
    });

    auto registerBufferAndArray = [&](auto bufferGetter) {
        cv.def_buffer(bufferGetter);

        cv.def(
            "__array__",
            [bufferGetter](py::object self, py::object /*dtype*/, py::object /*copy*/) {
                auto& view = self.cast<ViewType&>();
                py::buffer_info info = bufferGetter(view);
                return py::array(info, self);
            },
            py::arg("dtype") = py::none(),
            py::arg("copy") = py::none());

        cv.def("to_numpy", [bufferGetter](py::object self) {
            auto& view = self.cast<ViewType&>();
            py::buffer_info info = bufferGetter(view);
            return py::array(info, self);
        });
    };

    // Buffer protocol definition with non-compact safety check
    if constexpr (COMP_ID == CompId::POSITION || COMP_ID == CompId::NORMAL) {
        registerBufferAndArray([](ViewType& self) -> py::buffer_info {
            if (!self.isAvailable()) {
                throw std::runtime_error("Component is not available/enabled.");
            }
            if (!self.isCompact()) {
                throw std::runtime_error(
                    "Cannot access buffer protocol on a non-compact mesh. "
                    "The mesh contains deleted elements. Please call mesh.compact() first.");
            }
            using Scalar = typename ViewType::RawDataType::ScalarType;
            void* ptr = (self.empty() || self.data() == nullptr) ?
                            nullptr :
                            static_cast<void*>(self.data());
            return py::buffer_info(
                ptr,
                sizeof(Scalar),
                py::format_descriptor<Scalar>::format(),
                2,
                { self.size(), std::size_t(3) },
                { sizeof(Scalar) * 3, sizeof(Scalar) }
            );
        });
    }
    else if constexpr (COMP_ID == CompId::COLOR) {
        registerBufferAndArray([](ViewType& self) -> py::buffer_info {
            if (!self.isAvailable()) {
                throw std::runtime_error("Component is not available/enabled.");
            }
            if (!self.isCompact()) {
                throw std::runtime_error(
                    "Cannot access buffer protocol on a non-compact mesh. "
                    "The mesh contains deleted elements. Please call mesh.compact() first.");
            }
            void* ptr = (self.empty() || self.data() == nullptr) ?
                            nullptr :
                            static_cast<void*>(self.data());
            return py::buffer_info(
                ptr,
                sizeof(uint8_t),
                py::format_descriptor<uint8_t>::format(),
                2,
                { self.size(), std::size_t(4) },
                { sizeof(uint8_t) * 4, sizeof(uint8_t) }
            );
        });
    }
    else if constexpr (COMP_ID == CompId::TEX_COORD) {
        registerBufferAndArray([](ViewType& self) -> py::buffer_info {
            if (!self.isAvailable()) {
                throw std::runtime_error("Component is not available/enabled.");
            }
            if (!self.isCompact()) {
                throw std::runtime_error(
                    "Cannot access buffer protocol on a non-compact mesh. "
                    "The mesh contains deleted elements. Please call mesh.compact() first.");
            }
            using Scalar = typename ViewType::RawDataType::ScalarType;
            void* ptr = (self.empty() || self.data() == nullptr) ?
                            nullptr :
                            static_cast<void*>(self.data());
            return py::buffer_info(
                ptr,
                sizeof(Scalar),
                py::format_descriptor<Scalar>::format(),
                2,
                { self.size(), std::size_t(2) },
                { sizeof(Scalar) * 2, sizeof(Scalar) }
            );
        });
    }
    else if constexpr (COMP_ID == CompId::QUALITY) {
        registerBufferAndArray([](ViewType& self) -> py::buffer_info {
            if (!self.isAvailable()) {
                throw std::runtime_error("Component is not available/enabled.");
            }
            if (!self.isCompact()) {
                throw std::runtime_error(
                    "Cannot access buffer protocol on a non-compact mesh. "
                    "The mesh contains deleted elements. Please call mesh.compact() first.");
            }
            using Scalar = typename ViewType::RawDataType;
            void* ptr = (self.empty() || self.data() == nullptr) ?
                            nullptr :
                            static_cast<void*>(self.data());
            return py::buffer_info(
                ptr,
                sizeof(Scalar),
                py::format_descriptor<Scalar>::format(),
                1,
                { self.size() },
                { sizeof(Scalar) }
            );
        });
    }
    else if constexpr (COMP_ID == CompId::MATERIAL_INDEX) {
        registerBufferAndArray([](ViewType& self) -> py::buffer_info {
            if (!self.isAvailable()) {
                throw std::runtime_error("Component is not available/enabled.");
            }
            if (!self.isCompact()) {
                throw std::runtime_error(
                    "Cannot access buffer protocol on a non-compact mesh. "
                    "The mesh contains deleted elements. Please call mesh.compact() first.");
            }
            using Scalar = typename ViewType::RawDataType;
            void* ptr = (self.empty() || self.data() == nullptr) ?
                            nullptr :
                            static_cast<void*>(self.data());
            return py::buffer_info(
                ptr,
                sizeof(Scalar),
                py::format_descriptor<Scalar>::format(),
                1,
                { self.size() },
                { sizeof(Scalar) }
            );
        });
    }
    else if constexpr (COMP_ID == CompId::VERTEX_REFERENCES) {
        if constexpr (comp::HasStaticVertexReferences<Container>) {
            registerBufferAndArray([](ViewType& self) -> py::buffer_info {
                if (!self.isAvailable()) {
                    throw std::runtime_error("Component is not available/enabled.");
                }
                if (!self.isCompact()) {
                    throw std::runtime_error(
                        "Cannot access buffer protocol on a non-compact mesh. "
                        "The mesh contains deleted elements. Please call mesh.compact() first.");
                }
                using Index = typename Container::IndexType;
                constexpr std::size_t N = comp::ComponentTypeT<CompId::VERTEX_REFERENCES, Container>::VERTEX_COUNT;
                void* ptr = (self.empty() || self.data() == nullptr) ?
                                nullptr :
                                static_cast<void*>(self.data());
                return py::buffer_info(
                    ptr,
                    sizeof(Index),
                    py::format_descriptor<Index>::format(),
                    2,
                    { self.size(), N },
                    { sizeof(Index) * N, sizeof(Index) }
                );
            });
        }
    }

    registeredTypes.insert(typeid(ViewType));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_COMPONENT_VIEW_H
