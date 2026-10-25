// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_MATERIAL_INDEX_H
#define VCL_MESH_V2_COMPONENTS_MATERIAL_INDEX_H

#include "base/component_predicates.h"

#include <vclib/space/core.h>

namespace vcl::meshv2::comp {

/**
 * @brief The MaterialIndex class represents an unsigned integer index that
 * refers to a Material in the Mesh Materials component.
 *
 * It is typically part of an Element (e.g. Vertex, Face).
 *
 * For example, if you have a Face Element `f` with the MaterialIndex component,
 * you can access and modify its material index:
 *
 * @code{.cpp}
 * f.materialIndex() = 0;
 * auto mi = f.materialIndex();
 * @endcode
 *
 * @tparam Index: The unsigned integral type used to store the material index
 * (default: ushort / uint16_t).
 * @tparam OPT: If true, the component is optional (runtime enable/disable).
 *
 * @ingroup components
 */
template<std::unsigned_integral Index = ushort, bool OPT = false>
class MaterialIndex : public Component<CompId::MATERIAL_INDEX, Index, OPT>
{
    using Base = Component<CompId::MATERIAL_INDEX, Index, OPT>;

public:
    /**
     * @brief Expose the type of the material index.
     */
    using MaterialIndexType = Index;

    /**
     * @brief The null/unassigned sentinel value for material index.
     */
    static constexpr MaterialIndexType NULL_MATERIAL_INDEX =
        vcl::NULL_INDEX<MaterialIndexType>;

    /* Constructors */

    /**
     * @brief Default constructor.
     */
    MaterialIndex() = default;

    /**
     * @brief Returns the default initial value for MaterialIndex
     * (NULL_MATERIAL_INDEX).
     * @return NULL_MATERIAL_INDEX.
     */
    static constexpr MaterialIndexType defaultValue()
    {
        return NULL_MATERIAL_INDEX;
    }

    /* Member functions */

    /**
     * @brief Returns a reference to the material index of the element.
     * Propagates constness automatically from the caller (proxy).
     * @return A reference to the material index.
     */
    decltype(auto) materialIndex(this auto&& self)
    {
        return Base::compData(self);
    }

protected:
    template<typename OtherElement>
    void importFrom(this auto&& self, const OtherElement& other, bool = true);
};

/* Concepts */

/**
 * @brief A concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * MaterialIndex component (COMP_ID = CompId::MATERIAL_INDEX).
 *
 * @tparam T: The type to be tested for conformity to HasMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasMaterialIndex = HasComponent<CompId::MATERIAL_INDEX, T>;

/**
 * @brief A concept that checks whether a type T has the MaterialIndex component
 * and it is optional.
 *
 * @tparam T: The type to be tested for conformity to HasOptionalMaterialIndex.
 *
 * @ingroup components_concepts
 */
template<typename T>
concept HasOptionalMaterialIndex =
    HasMaterialIndex<T> && HasOptionalComponent<CompId::MATERIAL_INDEX, T>;

/* Detector function to check if a class has MaterialIndex available */

/**
 * @brief Checks if the given Element has MaterialIndex component available.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has MaterialIndex component available, `false`
 * otherwise.
 */
inline bool isMaterialIndexAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::MATERIAL_INDEX>(element);
}

/* Implementation of member functions */

template<std::unsigned_integral Index, bool OPT>
template<typename OtherElement>
void MaterialIndex<Index, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool)
{
    if constexpr (HasMaterialIndex<OtherElement>) {
        if (isMaterialIndexAvailableOn(other)) {
            self.materialIndex() =
                static_cast<MaterialIndexType>(other.materialIndex());
        }
    }
}

/* Specialization Aliases */

template<std::unsigned_integral Index = ushort>
using OptionalMaterialIndex = MaterialIndex<Index, true>;

using MaterialIndexus         = MaterialIndex<ushort>;
using MaterialIndexu          = MaterialIndex<uint>;
using OptionalMaterialIndexus = OptionalMaterialIndex<ushort>;
using OptionalMaterialIndexu  = OptionalMaterialIndex<uint>;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_MATERIAL_INDEX_H
