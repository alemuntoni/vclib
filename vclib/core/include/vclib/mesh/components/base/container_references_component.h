// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H

#include "container_component.h"
#include "reference_vector.h"

#include <vclib/mesh/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <limits>
#include <ranges>
#include <type_traits>
#include <vector>

namespace vcl::comp {

/**
 * @brief Base class for all components that store indices representing
 * topological references to other elements in the Mesh (e.g. vertex references,
 * adjacent faces, adjacent edges, adjacent vertices).
 *
 * It extends ContainerComponent by defining the referencesElement method and
 * providing a uniform, cache-friendly implementation of updateReferences for
 * mesh compaction.
 *
 * @tparam COMP_ID: The unique identifier of the component.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 * @tparam REF_ELEM_ID: The ElemId of the referenced elements (e.g.
 * ElemId::VERTEX).
 * @tparam N: The size of the reference container: static (N >= 0) or dynamic (N
 * < 0).
 * @tparam TTVC: True if the component container size is tied to the vertex
 * count.
 */
template<
    uint                   COMP_ID,
    std::unsigned_integral Index,
    uint                   REF_ELEM_ID,
    int                    N    = -1,
    bool                   TTVC = false,
    bool                   OPT  = false>
class ContainerReferencesComponent :
        public ContainerComponent<
            COMP_ID,
            Index,
            N,
            TTVC,
            OPT,
            ReferenceVector<Index, N>>
{
    using Base =
        ContainerComponent<COMP_ID, Index, N, TTVC, OPT, ReferenceVector<Index, N>>;

public:
    using IndexType = Index;

    /**
     * @brief The null/invalid sentinel value for this component's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    template<uint TARGET_ELEM_ID>
    static constexpr bool referencesElement()
    {
        return TARGET_ELEM_ID == REF_ELEM_ID;
    }

protected:
    /**
     * @brief Updates the stored reference indices according to the newIndices
     * mapping after compaction, permutation, or renumbering of the referenced
     * element container.
     *
     * @tparam TARGET_ELEM_ID: The element ID of the referenced elements.
     * @param[in] newIndices: A vector mapping old element indices to new ones.
     */
    template<uint TARGET_ELEM_ID>
    void updateReferences(
        this auto&&                   self,
        const std::vector<IndexType>& newIndices)
    {
        if constexpr (TARGET_ELEM_ID == REF_ELEM_ID) {
            for (auto& ref : Base::container(self)) {
                if (ref != NULL_INDEX) {
                    assert(ref < newIndices.size());
                    ref = newIndices[ref];
                }
            }
        }
    }

    /**
     * @brief Shifts the stored reference indices by offset if they match
     * TARGET_ELEM_ID. Used during append operations.
     *
     * @tparam TARGET_ELEM_ID: The element ID of the referenced elements.
     * @param[in] offset: The offset value to add to the references.
     */
    template<uint TARGET_ELEM_ID>
    void shiftReferences(this auto&& self, IndexType offset)
    {
        if constexpr (TARGET_ELEM_ID == REF_ELEM_ID) {
            if (offset != 0) {
                for (auto& ref : Base::container(self)) {
                    if (ref != NULL_INDEX) {
                        ref += offset;
                    }
                }
            }
        }
    }

    // --- Generic Static Reference / Adjacency Helpers ---

    /**
     * @brief Returns the number of referenced elements stored in this component.
     * @param[in] self: The component / element proxy object.
     * @return The number of referenced elements.
     */
    static uint refCount(const auto& self)
    {
        return Base::container(self).size();
    }

    /**
     * @brief Returns the index of the i-th referenced element.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @return The index in the target element container.
     */
    static IndexType refIndex(const auto& self, uint i)
    {
        assert(i < refCount(self));
        return Base::container(self)[i];
    }

    /**
     * @brief Returns a proxy to the i-th referenced element in the parent mesh.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @return A proxy to the referenced element.
     */
    static decltype(auto) refElement(auto&& self, uint i)
    {
        assert(i < refCount(self));
        return self.parentContainer()
            .parentMesh()
            .template element<REF_ELEM_ID>(Base::container(self)[i]);
    }

    /**
     * @brief Returns the index of the i-th referenced element using modular arithmetic.
     * Supports negative indices.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @return The index in the target element container.
     */
    static IndexType refIndexMod(const auto& self, int i)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        return Base::container(self)[((i % n) + n) % n];
    }

    /**
     * @brief Returns a proxy to the i-th referenced element using modular arithmetic.
     * Supports negative indices.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @return A proxy to the referenced element.
     */
    static decltype(auto) refElementMod(auto&& self, int i)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        return refElement(self, ((i % n) + n) % n);
    }

    /**
     * @brief Sets the reference at position i to the given element index.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @param[in] idx: The index of the referenced element.
     */
    static void setRef(auto&& self, uint i, IndexType idx)
    {
        assert(i < refCount(self));
        Base::container(self)[i] = idx;
    }

    /**
     * @brief Sets the reference at position i to the given element proxy or pointer.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @param[in] e: The referenced element proxy or pointer.
     */
    template<typename Elem>
    static void setRef(auto&& self, uint i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        assert(i < refCount(self));
        if constexpr (requires { e.index(); }) {
            Base::container(self)[i] = e.index();
        }
        else if constexpr (std::is_pointer_v<std::decay_t<Elem>>) {
            if (e != nullptr) {
                Base::container(self)[i] = e->index();
            }
            else {
                Base::container(self)[i] = NULL_INDEX;
            }
        }
        else {
            Base::container(self)[i] = static_cast<IndexType>(e);
        }
    }

    /**
     * @brief Sets the reference at position i using modular arithmetic.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @param[in] idx: The index of the referenced element.
     */
    static void setRefMod(auto&& self, int i, IndexType idx)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        setRef(self, ((i % n) + n) % n, idx);
    }

    /**
     * @brief Sets the reference at position i using modular arithmetic.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @param[in] e: The referenced element proxy or pointer.
     */
    template<typename Elem>
    static void setRefMod(auto&& self, int i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        setRef(self, ((i % n) + n) % n, e);
    }

    /**
     * @brief Sets all stored references from an input range of indices, proxies, or pointers.
     * @param[in] self: The component / element proxy object.
     * @param[in] rng: The input range.
     */
    template<std::ranges::input_range Rng>
    static void setRefs(auto&& self, Rng&& rng)
    {
        if constexpr (N >= 0) {
            assert(std::ranges::distance(rng) == N);
        }
        else if constexpr (!TTVC) {
            resizeRefs(self, std::ranges::distance(rng));
        }
        uint i = 0;
        for (auto&& item : rng) {
            setRef(self, i++, item);
        }
    }

    /**
     * @brief Sets all stored references from a parameter pack of indices, proxies, or pointers.
     * @param[in] self: The component / element proxy object.
     * @param[in] args: The references to set.
     */
    template<typename... Args>
    static void setRefs(auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        if constexpr (N < 0 && !TTVC) {
            resizeRefs(self, sizeof...(Args));
        }
        uint i = 0;
        (setRef(self, i++, args), ...);
    }

    /**
     * @brief Checks if this container contains the given referenced element index.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The index to check.
     * @return true if found, false otherwise.
     */
    static bool containsRef(const auto& self, IndexType idx)
    {
        return Base::container(self).contains(idx);
    }

    /**
     * @brief Checks if this container contains the given referenced element proxy or pointer.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy or pointer to check.
     * @return true if found, false otherwise.
     */
    template<typename Elem>
    static bool containsRef(const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            return containsRef(self, e.index());
        }
        else if constexpr (std::is_pointer_v<std::decay_t<Elem>>) {
            if (e != nullptr) {
                return containsRef(self, e->index());
            }
            return containsRef(self, NULL_INDEX);
        }
        else {
            return containsRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Finds the position in this reference container of the given element index.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The index to search for.
     * @return The 0-based position in this container, or UINT_NULL if not found.
     */
    static uint indexOfRef(const auto& self, IndexType idx)
    {
        return Base::container(self).indexOf(idx);
    }

    /**
     * @brief Finds the position in this reference container of the given element proxy or pointer.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy or pointer to search for.
     * @return The 0-based position in this container, or UINT_NULL if not found.
     */
    template<typename Elem>
    static uint indexOfRef(const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            return indexOfRef(self, e.index());
        }
        else if constexpr (std::is_pointer_v<std::decay_t<Elem>>) {
            if (e != nullptr) {
                return indexOfRef(self, e->index());
            }
            return indexOfRef(self, NULL_INDEX);
        }
        else {
            return indexOfRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Resizes the references container to size n.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] n: The new size.
     */
    static void resizeRefs(auto&& self, uint n) requires (N < 0 && !TTVC)
    {
        Base::container(self).resize(n);
    }

    /**
     * @brief Appends a referenced element index to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The index to append.
     */
    static void pushRef(auto&& self, IndexType idx) requires (N < 0 && !TTVC)
    {
        Base::container(self).pushBack(idx);
    }

    /**
     * @brief Appends a referenced element proxy or pointer to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy or pointer to append.
     */
    template<typename Elem>
    static void pushRef(auto&& self, const Elem& e)
        requires (N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            pushRef(self, e.index());
        }
        else if constexpr (std::is_pointer_v<std::decay_t<Elem>>) {
            if (e != nullptr) {
                pushRef(self, e->index());
            }
            else {
                pushRef(self, NULL_INDEX);
            }
        }
        else {
            pushRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Inserts a referenced element index at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to insert at.
     * @param[in] idx: The index to insert.
     */
    static void insertRef(auto&& self, uint i, IndexType idx) requires (N < 0 && !TTVC)
    {
        Base::container(self).insert(i, idx);
    }

    /**
     * @brief Inserts a referenced element proxy or pointer at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to insert at.
     * @param[in] e: The element proxy or pointer to insert.
     */
    template<typename Elem>
    static void insertRef(auto&& self, uint i, const Elem& e)
        requires (N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            insertRef(self, i, e.index());
        }
        else if constexpr (std::is_pointer_v<std::decay_t<Elem>>) {
            if (e != nullptr) {
                insertRef(self, i, e->index());
            }
            else {
                insertRef(self, i, NULL_INDEX);
            }
        }
        else {
            insertRef(self, i, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Erases the reference at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to erase.
     */
    static void eraseRef(auto&& self, uint i) requires (N < 0 && !TTVC)
    {
        Base::container(self).erase(i);
    }

    /**
     * @brief Clears all references in the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     */
    static void clearRefs(auto&& self) requires (N < 0 && !TTVC)
    {
        Base::container(self).clear();
    }

    /**
     * @brief Returns a lightweight view over the indices stored in this container.
     * @param[in] self: The component / element proxy object.
     * @return A View object exposing begin() and end() over the indices.
     */
    static auto refIndices(auto&& self)
    {
        return vcl::View(
            Base::container(self).begin(),
            Base::container(self).end());
    }

    /**
     * @brief Returns a lazy range of element proxies for all referenced elements.
     * @param[in] self: The component / element proxy object.
     * @return A transformed range yielding ElementProxy / ConstElementProxy objects.
     */
    static decltype(auto) refElements(auto&& self)
    {
        auto& m = self.parentContainer().parentMesh();
        return Base::container(self) | std::views::transform([&m](auto idx) {
                   return m.template element<REF_ELEM_ID>(idx);
               });
    }


};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
