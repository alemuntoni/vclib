// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <print>

#include "test_concepts.h"
#include "test_navigation.h"
#include "test_container_proxy.h"
#include "test_const_correctness.h"
#include "test_vertex_references.h"
#include "test_lifetime.h"
#include "test_deletion_compaction.h"
#include "test_optional_components.h"
#include "test_add_elements.h"
#include "test_import_from.h"
#include "test_append.h"
#include "test_custom_components.h"
#include "test_views.h"

int main()
{
    testConcepts();
    testNavigation();
    testContainerProxy();
    testConstCorrectness();
    testVertexReferences();
    testMeshLifetime();
    testDeletionCompaction();
    testOptionalComponents();
    testAddElements();
    testImportFrom();
    testAppend();
    testCustomComponents();
    testViews();

    std::println("All mesh tests passed successfully!");
    return 0;
}
