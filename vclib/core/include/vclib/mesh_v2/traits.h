// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_V2_TRAITS_H
#define VCL_MESH_V2_TRAITS_H

#include <vclib/mesh_v2/containers/base/element_container.h>
#include <vclib/mesh_v2/requirements.h>

#include <concepts>
#include <type_traits>
#include <utility>

/**
 * @defgroup mesh_traits Mesh Type Traits
 * @ingroup mesh
 *
 * @brief Type traits for extracting element and component types from Meshes,
 * Containers, and Element proxies using C++20 concepts without ambiguity.
 */

namespace vcl::meshv2 {

namespace detail {

// ============================================================================
// VertexTypeHelper
// ============================================================================

template<typename T>
struct VertexTypeHelper
{
};

// 1. MeshConcept
template<MeshConcept M>
struct VertexTypeHelper<M>
{
    using type = typename M::Vertex;
};

// 2. VertexContainerConcept
template<VertexContainerConcept C>
struct VertexTypeHelper<C>
{
    using type = typename C::ElementType;
};

// 3. FaceConcept (guaranteed to have .vertex(0) by FaceConcept)
template<FaceConcept F>
struct VertexTypeHelper<F>
{
    using type = std::remove_cvref_t<decltype(std::declval<F&>().vertex(0))>;
};

// 4. EdgeConcept (guaranteed to have .vertex(0) by EdgeConcept)
template<EdgeConcept E>
struct VertexTypeHelper<E>
{
    using type = std::remove_cvref_t<decltype(std::declval<E&>().vertex(0))>;
};

// 5. WedgeConcept (guaranteed to have .vertex() by WedgeConcept)
template<WedgeConcept W>
struct VertexTypeHelper<W>
{
    using type = std::remove_cvref_t<decltype(std::declval<W&>().vertex())>;
};

// 6. VertexConcept
template<VertexConcept V>
struct VertexTypeHelper<V>
{
    using type = V;
};

// ============================================================================
// FaceTypeHelper
// ============================================================================

template<typename T>
struct FaceTypeHelper
{
};

// 1. FaceMeshConcept
template<FaceMeshConcept M>
struct FaceTypeHelper<M>
{
    using type = typename M::Face;
};

// 2. FaceContainerConcept
template<FaceContainerConcept C>
struct FaceTypeHelper<C>
{
    using type = typename C::ElementType;
};

// 3. WedgeConcept (guaranteed to have .parentFace() by WedgeConcept)
template<WedgeConcept W>
struct FaceTypeHelper<W>
{
    using type = std::remove_cvref_t<decltype(std::declval<W&>().parentFace())>;
};

// 4. FaceConcept
template<FaceConcept F>
struct FaceTypeHelper<F>
{
    using type = F;
};

// ============================================================================
// EdgeTypeHelper
// ============================================================================

template<typename T>
struct EdgeTypeHelper
{
};

// 1. EdgeMeshConcept
template<EdgeMeshConcept M>
struct EdgeTypeHelper<M>
{
    using type = typename M::Edge;
};

// 2. EdgeContainerConcept
template<EdgeContainerConcept C>
struct EdgeTypeHelper<C>
{
    using type = typename C::ElementType;
};

// 3. EdgeConcept
template<EdgeConcept E>
struct EdgeTypeHelper<E>
{
    using type = E;
};

// ============================================================================
// WedgeTypeHelper
// ============================================================================

template<typename T>
struct WedgeTypeHelper
{
};

// 1. FaceConcept (guaranteed to have .wedge(0) by FaceConcept)
template<FaceConcept F>
struct WedgeTypeHelper<F>
{
    using type = std::remove_cvref_t<decltype(std::declval<F&>().wedge(0))>;
};

// ============================================================================
// IndexTypeHelper
// ============================================================================

template<typename T>
struct IndexTypeHelper
{
};

template<typename T>
requires requires { typename T::IndexType; }
struct IndexTypeHelper<T>
{
    using type = typename T::IndexType;
};

// ============================================================================
// PositionTypeHelper
// ============================================================================

template<typename T>
struct PositionTypeHelper
{
};

// 1. VertexConcept (guaranteed to have Position by VertexConcept)
template<VertexConcept V>
struct PositionTypeHelper<V>
{
    using type = std::remove_cvref_t<decltype(std::declval<V&>().position())>;
};

// 2. VertexContainerConcept
template<VertexContainerConcept C>
struct PositionTypeHelper<C>
{
    using type = typename PositionTypeHelper<typename C::ElementType>::type;
};

// 3. MeshConcept
template<MeshConcept M>
struct PositionTypeHelper<M>
{
    using type = typename PositionTypeHelper<typename M::Vertex>::type;
};

// ============================================================================
// ScalarTypeHelper
// ============================================================================

template<typename T>
struct ScalarTypeHelper
{
};

// 1. Types directly exposing ScalarType (e.g. Point3f, Point3d)
template<typename T>
requires (
    !MeshConcept<T> && !VertexConcept<T> &&
    requires { typename T::ScalarType; })
struct ScalarTypeHelper<T>
{
    using type = typename T::ScalarType;
};

// 2. VertexConcept (scalar of the vertex position)
template<VertexConcept V>
struct ScalarTypeHelper<V>
{
    using type = typename PositionTypeHelper<V>::type::ScalarType;
};

// 3. MeshConcept (scalar of the mesh vertices)
template<MeshConcept M>
struct ScalarTypeHelper<M>
{
    using type = typename ScalarTypeHelper<typename M::Vertex>::type;
};

// ============================================================================
// NormalTypeHelper
// ============================================================================

template<typename T>
struct NormalTypeHelper
{
};

// Supported on atomic proxies/elements possessing a normal (Vertex, Face,
// WedgeProxy), but explicitly excluded on MeshConcept to prevent ambiguity
// between vertex and face normals.
template<typename T>
requires (!MeshConcept<T> && HasNormal<T>)
struct NormalTypeHelper<T>
{
    using type = std::remove_cvref_t<decltype(std::declval<T&>().normal())>;
};

// ============================================================================
// TexCoordTypeHelper
// ============================================================================

template<typename T>
struct TexCoordTypeHelper
{
};

// Supported on atomic proxies possessing texture coordinates (Vertex,
// WedgeProxy), but excluded on MeshConcept and FaceConcept (since Face itself
// has no UVs, only its wedges do).
template<typename T>
requires (!MeshConcept<T> && !FaceConcept<T> && HasTexCoord<T>)
struct TexCoordTypeHelper<T>
{
    using type = std::remove_cvref_t<decltype(std::declval<T&>().texCoord())>;
};

} // namespace detail

// ============================================================================
// Public Type Traits Aliases
// ============================================================================

/**
 * @brief Extracts the Vertex proxy type from a Mesh, a Face, an Edge, a Wedge,
 * or a Vertex itself.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using VertexType =
    typename detail::VertexTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the Face proxy type from a FaceMesh, a Wedge, or a Face
 * itself.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using FaceType = typename detail::FaceTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the Edge proxy type from an EdgeMesh or an Edge itself.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using EdgeType = typename detail::EdgeTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the Wedge proxy type from a Face.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using WedgeType =
    typename detail::WedgeTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the index type (e.g. uint, uint64_t) from an Element,
 * Container, or Mesh.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using IndexType =
    typename detail::IndexTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the 3D position type (e.g. Point3f, Point3d) from a Vertex or
 * a Mesh.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using PositionType =
    typename detail::PositionTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the fundamental scalar type (e.g. float, double) from a
 * Vertex position, a Mesh, or a Point type.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
using ScalarType =
    typename detail::ScalarTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the normal vector type from a Vertex, a Face with face
 * normal, or a Wedge proxy. Not available on Mesh to avoid ambiguity between
 * vertex and face normals.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
requires (!MeshConcept<std::remove_cvref_t<T>> &&
          HasNormal<std::remove_cvref_t<T>>)
using NormalType =
    typename detail::NormalTypeHelper<std::remove_cvref_t<T>>::type;

/**
 * @brief Extracts the texture coordinates type from a Vertex or a Wedge proxy.
 * Not available on Face or Mesh to avoid ambiguity.
 * @tparam T: The type to query (cv-ref stripped automatically).
 * @ingroup mesh_traits
 */
template<typename T>
requires (!MeshConcept<std::remove_cvref_t<T>> &&
          !FaceConcept<std::remove_cvref_t<T>> &&
          HasTexCoord<std::remove_cvref_t<T>>)
using TexCoordType =
    typename detail::TexCoordTypeHelper<std::remove_cvref_t<T>>::type;

} // namespace vcl::meshv2

#endif // VCL_MESH_V2_TRAITS_H
