// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_COMPONENTS_BIT_FLAGS_H
#define VCL_MESH_V2_COMPONENTS_BIT_FLAGS_H

#include <vclib/mesh_v2/components/base/component_predicates.h>

#include <vclib/space/core.h>

#include <cassert>

namespace vcl::meshv2::comp {

class BitFlags : public Component<CompId::BIT_FLAGS, BitSet<uint>>
{
public:
    enum {
        DELETED  = 0, // bit 0
        SELECTED = 1, // bit 1
        BORDER   = 2, // bit 2
        VISITED  = 3  // bit 3
    };

    static constexpr uint FIRST_USER_BIT = 4;
    static constexpr uint USER_BIT_COUNT = sizeof(uint) * 8 - FIRST_USER_BIT;

protected:
    decltype(auto) bitFlags(this auto&& self)
    {
        return self.container().template get<CompId::BIT_FLAGS>()[self.index()];
    }

public:
    bool deleted(this auto&& self)
    {
        if (!self.isValid()) return false;
        return self.bitFlags()[DELETED];
    }

    bool isDeleted(this auto&& self)
    {
        return self.deleted();
    }

    decltype(auto) selected(this auto&& self)
    {
        return self.bitFlags()[SELECTED];
    }

    decltype(auto) onBorder(this auto&& self)
    {
        return self.bitFlags()[BORDER];
    }

    decltype(auto) visited(this auto&& self)
    {
        return self.bitFlags()[VISITED];
    }

    decltype(auto) userBit(this auto&& self, uint bit)
    {
        assert(bit < USER_BIT_COUNT);
        return self.bitFlags()[bit + FIRST_USER_BIT];
    }

    void resetBitFlags(this auto&& self)
    {
        bool isD = self.deleted();
        self.bitFlags().reset();
        self.bitFlags()[DELETED] = isD;
    }
};

/**
 * @brief Concept that checks whether a type T (which can be a TypeWrapper of
 * components, an ElementProxy, an ElementContainer, or a Mesh) has the
 * BitFlags component (COMP_ID = CompId::BIT_FLAGS).
 */
template<typename T>
concept HasBitFlags = HasComponent<CompId::BIT_FLAGS, T>;

} // namespace vcl::meshv2::comp

#endif // VCL_MESH_V2_COMPONENTS_BIT_FLAGS_H
