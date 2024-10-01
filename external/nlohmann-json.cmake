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

set(VCLIB_NLOHMANN_JSON_DIR ${CMAKE_CURRENT_LIST_DIR}/nlohmann-json-3.11.3)

if (VCLIB_ALLOW_BUNDLED_NLOHMANN_JSON AND
        EXISTS ${VCLIB_NLOHMANN_JSON_DIR}/include/nlohmann/json.hpp)
    message(STATUS "- Nlohmann JSON - using bundled source")
else()
    message(
        FATAL_ERROR
        "Nlohmann JSON is required -
        VCLIB_ALLOW_BUNDLED_NLOHMANN_JSON must be enabled and found.")
endif()

add_library(vclib-external-nlohmann-json INTERFACE)

target_include_directories(vclib-external-nlohmann-json INTERFACE ${VCLIB_NLOHMANN_JSON_DIR}/include)

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-nlohmann-json)
