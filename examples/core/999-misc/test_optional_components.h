// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_OPTIONAL_COMPONENTS_H
#define TEST_OPTIONAL_COMPONENTS_H

#include "types.h"

inline void testEnableDisable()
{
    MyMesh mesh;
    // By default optional components are disabled
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((!mesh.isEnabled<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // Enable normal
    mesh.enable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // Disable normal
    mesh.disable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // Enable all
    mesh.enableAll();
    assert((mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((mesh.isEnabled<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));

    // Disable all
    mesh.disableAll();
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    assert((!mesh.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::COLOR>()));
    assert((!mesh.isEnabled<meshv2::ElemId::FACE, meshv2::CompId::COLOR>()));
}

inline void testEnableSameOptionalComponentsOf()
{
    MyMesh meshWithNormals;
    meshWithNormals.enable<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>();
    assert((meshWithNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    MyMesh meshWithoutNormals;
    assert((!meshWithoutNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // 1. Mesh-level enableSameOptionalComponentsOf
    meshWithoutNormals.enableSameOptionalComponentsOf(meshWithNormals);
    assert((meshWithoutNormals.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));

    // 2. Test with a mesh that does not have the component (remains disabled)
    SimpleMesh simpleMesh;
    MyMesh meshTestedAgainstSimple;
    assert((!meshTestedAgainstSimple.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
    meshTestedAgainstSimple.enableSameOptionalComponentsOf(simpleMesh);
    assert((!meshTestedAgainstSimple.isEnabled<meshv2::ElemId::VERTEX, meshv2::CompId::NORMAL>()));
}

inline void testOptionalComponents()
{
    std::println("--- Testing Optional Components & enableSameOptionalComponentsOf ---");
    testEnableDisable();
    testEnableSameOptionalComponentsOf();
    std::println("Optional components verified successfully!\n");
}

#endif // TEST_OPTIONAL_COMPONENTS_H
