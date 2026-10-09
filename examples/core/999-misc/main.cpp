// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>
#include <vclib/mesh_v2/components/base/base.h>
#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/element_container.h>
#include <vclib/space/core.h>

using namespace vcl;

// Fake test components
namespace comp_test {

template<typename P>
struct Position : public meshv2::Component<meshv2::CompId::POSITION, P>
{
};

template<typename N>
struct Normal : public meshv2::Component<meshv2::CompId::NORMAL, N>
{
};

using Position3f = Position<Point3f>;
using Normal3f   = Normal<Point3f>;

} // namespace comp_test

int main()
{
    // TypeWrapper of components
    using MyComponents =
        TypeWrapper<comp_test::Position3f, comp_test::Normal3f>;

    // A generic element container
    using MyContainer = meshv2::ElementContainer<MyComponents>;

    MyContainer container;

    // enable components trough COMP_ID
    container.template enable<meshv2::CompId::POSITION>();
    container.template enable<meshv2::CompId::NORMAL>();

    container.resize(10);

    // get values trough COMP_ID!
    auto& posVec = container.template get<meshv2::CompId::POSITION>();
    posVec[0]    = Point3f(1.0f, 2.0f, 3.0f);

    auto& normVec = container.template get<meshv2::CompId::NORMAL>();
    normVec[0]    = Point3f(0.0f, 1.0f, 0.0f);

    std::cout << "Position 0: " << posVec[0].x() << ", " << posVec[0].y()
              << ", " << posVec[0].z() << "\n";
    std::cout << "Normal 0:   " << normVec[0].x() << ", " << normVec[0].y()
              << ", " << normVec[0].z() << "\n";
    std::cout << "Container size: " << container.size() << "\n";

    return 0;
}
