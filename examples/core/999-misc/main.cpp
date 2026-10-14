// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <iostream>
#include <vclib/mesh_v2/components/base/base.h>
#include <vclib/mesh_v2/components/base/component.h>
#include <vclib/mesh_v2/components/bit_flags.h>
#include <vclib/mesh_v2/element_container.h>
#include <vclib/mesh_v2/element_proxy.h>
#include <vclib/space/core.h>

using namespace vcl;

// Fake test components
namespace comp_test {

template<typename P>
struct Position : public meshv2::Component<meshv2::CompId::POSITION, P>
{
    decltype(auto) position(this auto&& self)
    {
        return self.container().template get<meshv2::CompId::POSITION>()[self.index()];
    }
};

template<typename N>
struct Normal : public meshv2::Component<meshv2::CompId::NORMAL, N>
{
    decltype(auto) normal(this auto&& self)
    {
        return self.container().template get<meshv2::CompId::NORMAL>()[self.index()];
    }
};

using Position3f = Position<Point3f>;
using Normal3f   = Normal<Point3f>;

} // namespace comp_test

int main()
{
    // TypeWrapper of components: BitFlags is MANDATORY!
    using MyComponents = TypeWrapper<
        meshv2::comp::BitFlags,
        comp_test::Position3f,
        comp_test::Normal3f>;

    // A generic element container
    using MyContainer = meshv2::ElementContainer<MyComponents>;

    MyContainer container;

    // enable components trough COMP_ID
    container.template enable<meshv2::CompId::POSITION>();
    container.template enable<meshv2::CompId::NORMAL>();

    container.resize(10);

    // ElementProxy to access the data of the container
    using MyElementProxy = meshv2::ElementProxy<MyContainer, MyComponents>;
    MyElementProxy element0(container, 0);

    // Use the proxy to access and modify the data of the container
    if (element0) {
        element0.position() = Point3f(1.0f, 2.0f, 3.0f);
        element0.normal()   = Point3f(0.0f, 1.0f, 0.0f);

        std::cout << "Position of element 0 read from proxy: "
                  << element0.position().x() << ", " << element0.position().y()
                  << ", " << element0.position().z() << "\n";
    }

    // ConstElementProxy to access the data of the container in a const way
    using MyConstProxy = meshv2::ElementProxy<const MyContainer, MyComponents>;
    MyConstProxy constElement0 = element0;

    const MyConstProxy& constElement01 = element0;

    // constElement0.position() = Point3f(0.0f, 0.0f, 0.0f); // build error
    // constElement01.position() = Point3f(0.0f, 0.0f, 0.0f); // build error

    std::cout << "Position 0 (from ConstProxy): "
              << constElement0.position().x() << ", "
              << constElement0.position().y() << ", "
              << constElement0.position().z() << "\n";

    std::cout << "Position 0 (from const Proxy&): "
              << constElement01.position().x() << ", "
              << constElement01.position().y() << ", "
              << constElement01.position().z() << "\n";

    std::cout << "Normal 0:   " << element0.normal().x() << ", "
              << element0.normal().y() << ", " 
              << element0.normal().z() << "\n";
              
    std::cout << "element0 is valid: " << (element0 ? "yes" : "no") << "\n";
    std::cout << "Container size: " << container.size() << "\n";

    // an invalid proxy (index out of bounds)
    MyElementProxy invalidElement;
    std::cout << "invalidElement is valid: " << (invalidElement ? "yes" : "no")
              << "\n";

    // Test IndexType deduction: default is uint
    static_assert(std::is_same_v<MyElementProxy::IndexType, uint>);

    // Test customized 64-bit container
    using MyBigContainer = meshv2::ElementContainer<MyComponents, uint64_t>;
    using MyBigElementProxy = meshv2::ElementProxy<MyBigContainer, MyComponents>;
    static_assert(std::is_same_v<MyBigElementProxy::IndexType, uint64_t>);

    MyBigContainer bigContainer;
    bigContainer.template enable<meshv2::CompId::POSITION>();
    bigContainer.resize(1);
    MyBigElementProxy bigElement(bigContainer, 0);
    bigElement.position() = Point3f(4.0f, 5.0f, 6.0f);
    std::cout << "BigContainer (uint64_t index) Element 0 position: "
              << bigElement.position().x() << ", "
              << bigElement.position().y() << ", "
              << bigElement.position().z() << "\n";

    // ==========================================
    // Phase 4: BitFlags and Iterators with skip
    // ==========================================
    std::cout << "\n--- Testing Phase 4: BitFlags & Iterators ---\n";
    MyContainer testContainer;
    testContainer.template enable<meshv2::CompId::POSITION>();
    testContainer.resize(5);

    for (std::size_t i = 0; i < testContainer.size(); ++i) {
        testContainer[i].position() = Point3f(static_cast<float>(i), 0.0f, 0.0f);
    }

    // Delete elements 1 and 3
    testContainer[1].setDeleted(true);
    testContainer.deleteElement(3);

    std::cout << "Physical size: " << testContainer.size() << "\n";
    std::cout << "Non-deleted count: " << testContainer.elementCount() << "\n";
    std::cout << "Deleted count: " << testContainer.deletedElementCount() << "\n";

    // Range-based for loop: automatically skips deleted elements (1 and 3)
    std::cout << "Iterating over non-deleted elements (range-based for loop):\n";
    for (auto elem : testContainer) {
        std::cout << "  Visible element " << elem.index()
                  << " pos.x = " << elem.position().x()
                  << " deleted: " << (elem.deleted() ? "yes" : "no") << "\n";
    }

    // Iterator over ALL elements including deleted
    std::cout << "Iterating over ALL elements (jumpDeleted = false):\n";
    for (auto it = testContainer.begin(false); it != testContainer.end(); ++it) {
        std::cout << "  Element " << it->index()
                  << " pos.x = " << it->position().x()
                  << " deleted: " << (it->deleted() ? "yes" : "no") << "\n";
    }

    // Undelete element 1
    testContainer[1].setDeleted(false);
    std::cout << "After undeleting element 1, non-deleted count: "
              << testContainer.elementCount() << "\n";

    return 0;
}
