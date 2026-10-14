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

    return 0;
}
