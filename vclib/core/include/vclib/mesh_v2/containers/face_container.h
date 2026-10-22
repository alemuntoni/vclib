// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H
#define VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H

#include <vclib/mesh_v2/components/base/component_predicates.h>
#include <vclib/mesh_v2/containers/base/element_container.h>
#include <vclib/mesh_v2/elements/face.h>
#include <vclib/mesh_v2/elements/face_components.h>

#include <vclib/algorithms/core.h>
#include <vclib/base.h>

namespace vcl::meshv2 {

/**
 * @brief FaceContainer stores the per-face data in a Structure of Arrays (SoA).
 *
 * It accepts the Face component list (satisfying HasRequiredFaceComponents),
 * instantiates the underlying storage and exposes the Face element view.
 *
 * @tparam CompList The face components type (e.g. FaceComponents<BitFlags,
 * TriangleVertexReferences<uint>>).
 * @tparam Index The index type (default uint).
 * @tparam ParentMesh The parent mesh type (default void).
 *
 * @ingroup containers
 */
template<
    HasRequiredFaceComponents CompList,
    std::unsigned_integral    Index = uint,
    typename ParentMesh             = void>
class FaceContainer :
        public ElementContainer<
            typename comp::detail::ComponentListHelper<CompList>::type,
            ParentMesh,
            Index,
            Face>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Face>;

protected:
    using Base::vector;

public:
    using Components     = typename Base::Components;
    using FaceComponents = CompList;
    using IndexType      = Index;
    using Face           = typename Base::ProxyType;
    using ConstFace      = typename Base::ConstProxyType;
    using FaceType_      = Face;
    using ConstFaceType  = ConstFace;

    template<typename NewParent>
    using Rebind = FaceContainer<CompList, Index, NewParent>;

    FaceContainer() { Base::template enable<CompId::VERTEX_REFERENCES>(); }

    Face face(Index i) { return Base::element(i); }

    ConstFace face(Index i) const { return Base::element(i); }

    std::size_t faceCount() const { return Base::elementCount(); }

    std::size_t faceContainerSize() const
    {
        return Base::elementContainerSize();
    }

    std::size_t deletedFaceCount() const { return Base::deletedElementCount(); }

    /**
     * @brief Adds a new face into the Face container, returning the index of
     * the added face.
     *
     * @return The index of the new face.
     */
    Index addFace() { return Base::addElement(); }

    /**
     * @brief Adds a new face with the given vertices or vertex indices into the
     * Face container, returning the index of the added face.
     *
     * @param[in] args: The vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<typename... Args>
    Index addFace(const Args&... args) requires (
        sizeof...(Args) >= 3 &&
        (sizeof...(Args) == Face::VERTEX_COUNT || Face::VERTEX_COUNT < 0))
    {
        Index fid = addFace();
        face(fid).setVertices(args...);
        return fid;
    }

    /**
     * @brief Adds a new face with the vertices or vertex indices contained in
     * the given range, returning the index of the added face.
     *
     * @param[in] rng: The range of vertices or vertex indices of the new face.
     * @return The index of the new face.
     */
    template<std::ranges::input_range Rng>
    Index addFace(Rng&& rng)
    {
        Index fid = addFace();
        face(fid).setVertices(std::forward<Rng>(rng));
        return fid;
    }

    /**
     * @brief Adds an arbitrary number of n faces, returning the index of the
     * first added face.
     *
     * @param[in] n: the number of faces to add to the container.
     * @return The index of the first added face.
     */
    Index addFaces(std::size_t n) { return Base::addElements(n); }

    /**
     * @brief Adds an arbitrary number of faces with the vertices or vertex
     * indices contained in the given range of ranges, returning the index of
     * the first added face.
     *
     * @param[in] rng: A range of ranges containing the vertices or vertex
     * indices for each face.
     * @return The index of the first added face.
     */
    template<std::ranges::input_range Rng>
    requires std::ranges::input_range<std::ranges::range_value_t<Rng>>
    Index addFaces(Rng&& rng)
    {
        Index baseId = faceContainerSize();
        if constexpr (std::ranges::sized_range<Rng>) {
            reserveFaces(baseId + std::ranges::size(rng));
        }
        for (auto&& f : rng) {
            addFace(f);
        }
        return baseId;
    }

    std::vector<Index> compactFaces() { return Base::compactElements(); }

    void resizeFaces(std::size_t n) { Base::resizeElements(n); }

    void reserveFaces(std::size_t capacity) { Base::reserveElements(capacity); }

    void clearFaces() { Base::clearElements(); }

    void deleteFace(Index i) { Base::deleteElement(i); }

    void deleteFace(const Face& f) { Base::deleteElement(f.index()); }

    auto faces(bool jumpDeleted = true) { return Base::elements(jumpDeleted); }

    auto faces(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    // --- Custom Components ---

    /**
     * @brief Checks if Faces have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name The name of the custom component to check.
     * @return `true` if the Face Element has a custom component with the given
     * name.
     */
    bool hasPerFaceCustomComponent(const std::string& name) const
        requires face::HasCustomComponents<Components>
    {
        return Base::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the Face Element.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @return A vector of strings representing all the names of the custom
     * components.
     */
    std::vector<std::string> perFaceCustomComponentNames() const
        requires face::HasCustomComponents<Components>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Face Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K The type of the custom component to check.
     * @param[in] name The name of the custom component to check.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return `true` if the custom component is of the same type of the
     * template argument.
     */
    template<typename K>
    bool isPerFaceCustomComponentOfType(const std::string& name) const
        requires face::HasCustomComponents<Components>
    {
        return Base::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the Face
     * Element having the given input name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name The name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index perFaceCustomComponentType(const std::string& name) const
        requires face::HasCustomComponents<Components>
    {
        return Base::elemComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Face Element having the same type of the given
     * template argument type of this function.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K The type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> perFaceCustomComponentNamesOfType() const
        requires face::HasCustomComponents<Components>
    {
        return Base::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K to the Face, having the given
     * name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K The type of the custom component added to the Face.
     * @param[in] name The name of the custom component added to the Face.
     */
    template<typename K>
    void addPerFaceCustomComponent(const std::string& name)
        requires face::HasCustomComponents<Components>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Face
     * Element.
     *
     * The function does nothing if the custom component does not exists.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @param[in] name The name of the custom component that will be removed
     * from the Face.
     */
    void deletePerFaceCustomComponent(const std::string& name)
        requires face::HasCustomComponents<Components>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K The type of the custom component on which return the handle.
     * @param[in] name Name of the custom component on which return the handle.
     * @return A vector handle that allows to access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> perFaceCustomComponentVectorHandle(
        const std::string& name)
        requires face::HasCustomComponents<Components>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * @note This function is available only if the Face Element has the
     * CustomComponents Component.
     *
     * @tparam K The type of the custom component on which return the handle.
     * @param[in] name Name of the custom component on which return the handle.
     * @return A const vector handle that allows to access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> perFaceCustomComponentVectorHandle(
        const std::string& name) const
        requires face::HasCustomComponents<Components>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-face custom components of type K stored in
     * this container to the given output stream.
     *
     * @tparam K The type of the custom components to serialize.
     * @param os The output stream to write to.
     */
    template<typename K>
    void serializePerFaceCustomComponentsOfType(std::ostream& os) const
        requires face::HasCustomComponents<Components>
    {
        Base::template serializeElemCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all per-face custom components of type K from the
     * given input stream and stores them into this container.
     *
     * @tparam K The type of the custom components to deserialize.
     * @param is The input stream to read from.
     */
    template<typename K>
    void deserializePerFaceCustomComponentsOfType(std::istream& is)
        requires face::HasCustomComponents<Components>
    {
        Base::template deserializeElemCustomComponentsOfType<K>(is);
    }

protected:
    /**
     * @brief Imports all faces from the corresponding container of the
     * other mesh into this container.
     *
     * If importing into a triangle mesh from a mesh with non-triangle faces
     * (e.g. quad or polygonal), the faces are triangulated using ear-cutting.
     *
     * @param[in] m The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        Base::importFrom(m);
        manageImportTriFromPoly(m);
    }

    /**
     * @brief Manages conversion from polygonal or n-gon faces to triangles
     * when importing into a triangle mesh.
     *
     * Non-triangular faces are triangulated using the ear-cutting algorithm.
     * The first triangle occupies the original face index, and additional
     * triangles are appended to the container.
     */
    template<typename OtherMesh>
    void manageImportTriFromPoly(const OtherMesh& m)
    {
        if constexpr (OtherMesh::template hasContainerOf<ElemId::FACE>()) {
            using OtherFace = OtherMesh::ConstFace;

            // if this is not a triangle mesh nor a polygon mesh (meaning that
            // we can't control the number of vertex pointers in this mesh), and
            // this mesh does not have the same number of vertex pointers of the
            // other, it means that we don't know how to convert these type of
            // meshes (e.g. we don't know how to convert a polygon mesh into a
            // quad mesh, or convert a quad mesh into a pentagonal mesh...)
            static_assert(
                !(Face::VERTEX_COUNT != 3 && Face::VERTEX_COUNT > 0 &&
                  Face::VERTEX_COUNT != OtherFace::VERTEX_COUNT),
                "Cannot import from that type of Mesh. Don't know how to "
                "convert faces.");

            // we need to manage conversion from poly or faces with cardinality
            // > 3 (e.g. quads) to triangle meshes. In this case, we triangulate
            // the polygon using the earcut algorithm.
            if constexpr (
                Face::VERTEX_COUNT == 3 &&
                (OtherFace::VERTEX_COUNT > 3 || OtherFace::VERTEX_COUNT < 0)) {
                for (const auto& mf : m.faces()) {
                    // if the current face has the same number of vertices of
                    // this mf face (3), then the vertex indices have been
                    // correctly imported from the component importFrom
                    // function. The component importFrom function does
                    // nothing when importing from a face with at least 4
                    // vertices
                    if (mf.vertexCount() != 3) {
                        // triangulate mf; the first triangle of the
                        // triangulation will be this->face(m.index(mf)); the
                        // other triangles will be added at the end of the
                        // container

                        // TODO: use vcl::views for mesh v2 when will be
                        // implemented
                        auto polyPositions =
                            mf.vertexIndices() |
                            std::views::transform([&m](auto vi) {
                                return m.vertex(vi).position();
                            });

                        std::vector<uint> tris = vcl::earCut(polyPositions);

                        if (tris.size() >= 3) {
                            Face f = face(mf.index());
                            importTriCorners(f, mf, tris, 0);

                            // number of other faces to add
                            uint  nf      = tris.size() / 3 - 1;
                            Index baseFid = addFaces(nf);

                            uint t = 3; // index that cycles into tris
                            for (uint k = 0; k < nf; ++k, t += 3) {
                                Face fk = face(baseFid + k);
                                fk.importFrom(mf, false);
                                importTriCorners(fk, mf, tris, t);
                            }
                        }
                    }
                }
            }
        }
    }

private:
    template<typename OtherFace>
    static void importTriCorners(
        Face&                    f,
        const OtherFace&         mf,
        const std::vector<uint>& tris,
        uint                     basetri)
    {
        for (uint j = 0; j < 3; ++j) {
            f.setVertex(j, mf.vertexIndex(tris[basetri + j]));
            // TODO: Handle wedge components (e.g. WedgeColors, WedgeTexCoords)
            // once implemented in mesh_v2:
            // f.wedgeColor(j) = mf.wedgeColor(tris[basetri + j]);
            // f.wedgeTexCoord(j) = mf.wedgeTexCoord(tris[basetri + j]);
        }
    }
};

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_CONTAINERS_FACE_CONTAINER_H
