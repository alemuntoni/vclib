// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
#define VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H

#include "container_component.h"
#include "reference_vector.h"

#include <vclib/mesh_v2/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <limits>
#include <ranges>
#include <vector>

namespace vcl::meshv2 {

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
                if (ref != std::numeric_limits<IndexType>::max()) {
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
                    if (ref != std::numeric_limits<IndexType>::max()) {
                        ref += offset;
                    }
                }
            }
        }
    }

    // --- High-level Element Proxy Access (via Parent Mesh) ---

    decltype(auto) referencedElement(this auto&& self, uint i)
    {
        return self.parentContainer()
            .parentMesh()
            .template element<REF_ELEM_ID>(Base::container(self)[i]);
    }

    decltype(auto) referencedElements(this auto&& self)
    {
        auto& m = self.parentContainer().parentMesh();
        return Base::container(self) | std::views::transform([&m](auto idx) {
                   return m.template element<REF_ELEM_ID>(idx);
               });
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
