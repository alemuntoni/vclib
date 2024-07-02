/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_TEXTURE_IMAGES_H
#define VCL_MESH_COMPONENTS_TEXTURE_IMAGES_H

#ifndef VCLIB_WITH_MODULES
#include <string>
#include <vector>

#include <vclib/concepts/mesh/components/texture_images.h>
#include <vclib/concepts/mesh/components/texture_paths.h>
#include <vclib/space/texture.h>
#include <vclib/types/view.h>

#include "bases/component.h"
#endif

namespace vcl::comp {

namespace detail {

struct TData
{
    std::vector<vcl::Texture> textures;
    std::string               meshPath;
};

} // namespace detail

/**
 * @brief The TextureImages class represents a component that stores the
 * textures used by a mesh. This component makes sense only if it is used by
 * meshes, and therefore it cannot be stored vertically or be optional (no
 * template arguments are needed for these options).
 *
 * The TextureImages component stores a vector of textures that are composed by
 * an image and a path relative to the mesh path. The mesh path is stored as
 * well, and it is used to construct the absolute paths of the textures.
 *
 * The member functions of this class will be available in the instance of any
 * Mesh that will contain this component.
 *
 * For example, if you have a Mesh `m` with the TextureImages component, you'll
 * be able to access to this component member functions from `m`:
 *
 * @code{.cpp}
 * for (const auto& texture : m.textures()) {
 *     // do something with texture
 * }
 * @endcode
 *
 * @ingroup components
 */
class TextureImages :
        public Component<
            TextureImages,
            CompId::TEXTURE_IMAGES,
            detail::TData,
            void,
            false,
            false>
{
    using Base = Component<
        TextureImages,
        CompId::TEXTURE_IMAGES,
        detail::TData,
        void,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Texture.
     */
    using TextureType = vcl::Texture;

    // iterators
    using TextureIterator      = std::vector<vcl::Texture>::iterator;
    using ConstTextureIterator = std::vector<vcl::Texture>::const_iterator;

    /* Constructors */

    /**
     * @brief Initializes the component with an empty vector of textures
     * and an empty string as mesh base path.
     */
    TextureImages() = default;

    /* Member functions */

    /**
     * @brief Returns the number of texture of the mesh.
     */
    uint textureNumber() const { return texs().size(); }

    /**
     * @brief Returns the path of the i-th texture of the mesh. The path of the
     * texture is relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    const vcl::Texture& texture(uint i) const { return texs()[i]; }

    /**
     * @brief Returns a reference to the i-th texture of the mesh.
     * The path of the texture is relative to the mesh base path.
     * @param[in] i: The index of the texture.
     */
    vcl::Texture& texture(uint i) { return texs()[i]; }

    /**
     * @brief Returns the mesh base path.
     */
    const std::string& meshBasePath() const { return Base::data().meshPath; }

    /**
     * @brief Returns a reference to the mesh base path.
     */
    std::string& meshBasePath() { return Base::data().meshPath; }

    /**
     * @brief Clears the vector of textures.
     */
    void clearTextures() { texs().clear(); }

    /**
     * @brief Adds a texture path to the vector of textures. The image of the
     * texture is left empty.
     * @param[in] textPath: The texture path to add.
     */
    void pushTexture(const std::string& textPath)
    {
        texs().push_back(vcl::Texture());
        texs().back().path() = textPath;
    }

    /**
     * @brief Adds a texture to the vector of textures.
     * @param[in] texture: The texture to add.
     */
    void pushTexture(const vcl::Texture& texture) { texs().push_back(texture); }

    /**
     * @brief Returns an iterator to the beginning of the vector of textures.
     * @return an iterator to the beginning of the vector of textures.
     */
    TextureIterator textureBegin() { return texs().begin(); }

    /**
     * @brief Returns an iterator to the end of the vector of textures.
     * @return an iterator to the end of the vector of textures.
     */
    TextureIterator textureEnd() { return texs().end(); }

    /**
     * @brief Returns a const iterator to the beginning of the vector of
     * textures.
     * @return a const iterator to the beginning of the vector of textures.
     */
    ConstTextureIterator textureBegin() const { return texs().begin(); }

    /**
     * @brief Returns a const iterator to the end of the vector of textures.
     * @return a const iterator to the end of the vector of textures.
     */
    ConstTextureIterator textureEnd() const { return texs().end(); }

    /**
     * @brief Returns a lightweigth view object that stores the begin and end
     * iterators of the vector of textures. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (auto& texture : m.texturess()) {
     *     // Do something with texture
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of textures.
     */
    View<TextureIterator> textures()
    {
        return View(textureBegin(), textureEnd());
    }

    /**
     * @brief Returns a lightweigth const view object that stores the begin and
     * end iterators of the vector of textures. The view object exposes the
     * iterators trough the `begin()` and `end()` member functions, and
     * therefore the returned object can be used in range-based for loops:
     *
     * @code{.cpp}
     * for (const auto& texture : m.textures()) {
     *     // Do something with texture
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over the vector of textures.
     */
    View<ConstTextureIterator> textures() const
    {
        return View(textureBegin(), textureEnd());
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        if constexpr (HasTextureImages<Element>) {
            texs()         = e.texs();
            meshBasePath() = e.meshBasePath();
        }
        if constexpr (HasTexturePaths<Element>) {
            texs().clear();
            for (const auto& tpath : e.texturePaths()) {
                pushTexture(tpath);
            }
            meshBasePath() = e.meshBasePath();
        }
    }

    void serialize(std::ostream& os) const
    {
        vcl::serialize(os, texs());
        vcl::serialize(os, meshBasePath());
    }

    void deserialize(std::istream& is)
    {
        vcl::deserialize(is, texs());
        vcl::deserialize(is, meshBasePath());
    }

private:
    // members that allow to access the data
    std::vector<vcl::Texture>& texs() { return Base::data().textures; }

    const std::vector<vcl::Texture>& texs() const
    {
        return Base::data().textures;
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_TEXTURE_PATHS_H
