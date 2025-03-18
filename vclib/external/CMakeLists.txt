#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

cmake_minimum_required(VERSION 3.24)
project(vclib-external)

# 3rd party libraries
message(STATUS "VCLib-External: 3rd party libraries")
add_subdirectory(3rdparty)

### vclib-core target
set(VCLIB_EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

file(GLOB_RECURSE MODULES CONFIGURE_DEPENDS "src/*.ixx")

set(VCLIB_EXTERNAL_TARGET_LINK_TYPE INTERFACE) # header only library

if (VCLIB_BUILD_CXX20_MODULES)
    add_library(${PROJECT_NAME})

    set(VCLIB_EXTERNAL_TARGET_LINK_TYPE PUBLIC)

    target_sources(${PROJECT_NAME}
      PUBLIC
        FILE_SET CXX_MODULES FILES ${MODULES}
    )

    target_compile_definitions(${PROJECT_NAME} PUBLIC VCLIB_WITH_MODULES)

    # avoid warnings when using modules
    # todo: remove this when modules are and header units stable and supported by all compilers
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "/wd5244;")
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wno-include-angled-in-module-purview)
    endif()
else()
    add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
endif()

add_library(vclib::external ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${VCLIB_EXTERNAL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES} vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})
