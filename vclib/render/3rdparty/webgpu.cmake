#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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

if (VCLIB_ALLOW_DOWNLOAD_WEBGPU)
    set (WEBGPU_BACKEND "WGPU")

    add_subdirectory(webgpu)

    add_library(vclib-3rd-webgpu INTERFACE)
    target_link_libraries(vclib-3rd-webgpu INTERFACE webgpu)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-webgpu)
else()
    message(FATAL_ERROR
        "webgpu is required - set VCLIB_ALLOW_DOWNLOAD_WEBGPU to ON.")
endif()
