/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/bgfx/primitives/lines/gpu_generated_lines.h>

namespace vcl::detail {

GPUGeneratedLines::GPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(vertCoords, vertNormals, vertColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(
        pointsSize, vertexCoords, vertexNormals, vertexColors, lineColors);
}

GPUGeneratedLines::GPUGeneratedLines(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors) : GPUGeneratedLines()
{
    assert(bgfx::isValid(mComputeVerticesPH));
    setPoints(
        pointsSize,
        vertexCoords,
        lineIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void GPUGeneratedLines::swap(GPUGeneratedLines& other)
{
    using std::swap;

    swap(mCustomIndicesUH, other.mCustomIndicesUH);

    swap(mVertices, other.mVertices);
    swap(mIndices, other.mIndices);
}

void GPUGeneratedLines::draw(uint viewId) const
{
    mVertices.bind(0);
    mIndices.bind();
    bgfx::setState(linesDrawState());
    bgfx::submit(viewId, mLinesPH);
}

void GPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    std::vector<uint> emptyIndices;
    setPoints(
        false, vertCoords, emptyIndices, vertNormals, vertColors, lineColors);
}

void GPUGeneratedLines::setPoints(
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    setPoints(
        true, vertCoords, lineIndices, vertNormals, vertColors, lineColors);
}

void GPUGeneratedLines::setPoints(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    IndexBuffer emptyIndices;
    setPoints(
        pointsSize,
        vertexCoords,
        emptyIndices,
        vertexNormals,
        vertexColors,
        lineColors);
}

void GPUGeneratedLines::setPoints(
    const uint          nPoints,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    if (nPoints > 1) {
        allocateVertexAndIndexBuffer(nPoints);
        generateVertexAndIndexBuffer(
            nPoints,
            vertexCoords,
            lineIndices,
            vertexNormals,
            vertexColors,
            lineColors);
    }
    else {
        mVertices.destroy();
        mIndices.destroy();
    }
}

void GPUGeneratedLines::setPoints(
    bool                      setLineIndices,
    const std::vector<float>& vertCoords,
    const std::vector<uint>&  lineIndices,
    const std::vector<float>& vertNormals,
    const std::vector<uint>&  vertColors,
    const std::vector<uint>&  lineColors)
{
    assert(vertCoords.size() % 3 == 0);
    assert(lineIndices.size() % 2 == 0);

    const bool setColors     = vertColors.size() != 0;
    const bool setNormals    = vertNormals.size() != 0;
    const bool setLineColors = lineColors.size() != 0;

    const uint nPoints =
        setLineIndices ? lineIndices.size() : vertCoords.size() / 3;

    assert(!setColors || vertCoords.size() == vertColors.size() * 3);
    assert(!setNormals || vertCoords.size() == vertNormals.size());
    assert(!setLineColors || vertColors.size() == lineColors.size() * 2);

    if (nPoints > 1) {
        IndexBuffer  lineIndicesBuffer;
        VertexBuffer vertexNormalsBuffer;
        VertexBuffer vertexColorsBuffer;
        VertexBuffer lineColorsBuffer;


        VertexBuffer vertexCoordsBuffer = allocateVertexCoords(vertCoords);

        if (setLineIndices)
            lineIndicesBuffer = allocateLineIndices(lineIndices);
        if (setNormals)
            vertexNormalsBuffer = allocateVertexNormals(vertNormals);
        if (setColors)
            vertexColorsBuffer = allocateVertexColors(vertColors);
        if (setLineColors)
            lineColorsBuffer = allocateLineColors(lineColors);

        allocateVertexAndIndexBuffer(nPoints);
        generateVertexAndIndexBuffer(
            nPoints,
            vertexCoordsBuffer,
            lineIndicesBuffer,
            vertexNormalsBuffer,
            vertexColorsBuffer,
            lineColorsBuffer);
    }
    else {
        mVertices.destroy();
        mIndices.destroy();
    }
}

VertexBuffer GPUGeneratedLines::allocateVertexCoords(
    const std::vector<float>& coords)
{
    VertexBuffer coordsBuffer;

    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(coords.size());

    std::copy(coords.begin(), coords.end(), buffer);

    coordsBuffer.createForCompute(
        buffer,
        coords.size() / 3,
        bgfx::Attrib::Position,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn);

    return coordsBuffer;
}

IndexBuffer GPUGeneratedLines::allocateLineIndices(
    const std::vector<uint>& indices)
{
    IndexBuffer indicesBuffer;

    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(indices.size());

    std::copy(indices.begin(), indices.end(), buffer);

    indicesBuffer.createForCompute(
        buffer,
        indices.size(),
        PrimitiveType::UINT,
        bgfx::Access::Read,
        releaseFn);

    return indicesBuffer;
}

VertexBuffer GPUGeneratedLines::allocateVertexNormals(
    const std::vector<float>& normals)
{
    VertexBuffer normalsBuffer;

    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<float>(normals.size());

    std::copy(normals.begin(), normals.end(), buffer);

    normalsBuffer.createForCompute(
        buffer,
        normals.size() / 3,
        bgfx::Attrib::Normal,
        3,
        PrimitiveType::FLOAT,
        false,
        bgfx::Access::Read,
        releaseFn);

    return normalsBuffer;
}

VertexBuffer GPUGeneratedLines::allocateVertexColors(
    const std::vector<uint>& vertColors)
{
    VertexBuffer vertColorsBuffer;

    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(vertColors.size());

    std::copy(vertColors.begin(), vertColors.end(), buffer);

    vertColorsBuffer.createForCompute(
        buffer,
        vertColors.size(),
        bgfx::Attrib::Color0,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn);

    return vertColorsBuffer;
}

VertexBuffer GPUGeneratedLines::allocateLineColors(
    const std::vector<uint>& lineColors)
{
    VertexBuffer lineColorsBuffer;

    auto [buffer, releaseFn] =
        linesGetAllocatedBufferAndReleaseFn<uint>(lineColors.size());

    std::copy(lineColors.begin(), lineColors.end(), buffer);

    lineColorsBuffer.createForCompute(
        buffer,
        lineColors.size(),
        bgfx::Attrib::Color1,
        4,
        PrimitiveType::UCHAR,
        true,
        bgfx::Access::Read,
        releaseFn);

    return lineColorsBuffer;
}

void GPUGeneratedLines::allocateVertexAndIndexBuffer(const uint pointsSize)
{
    bgfx::VertexLayout layout;
    layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color2, 4, bgfx::AttribType::Uint8, true)
        .end();

    const uint szv = (pointsSize / 2) * 4 * 15;

    mVertices.create(
        bgfx::makeRef(nullptr, sizeof(float) * szv),
        layout,
        BGFX_BUFFER_COMPUTE_READ_WRITE,
        true);

    const uint szi = (pointsSize / 2) * 6;

    mIndices.create(
        bgfx::makeRef(nullptr, sizeof(uint) * szi),
        BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32,
        true);
}

void GPUGeneratedLines::generateVertexAndIndexBuffer(
    const uint          pointsSize,
    const VertexBuffer& vertexCoords,
    const IndexBuffer&  lineIndices,
    const VertexBuffer& vertexNormals,
    const VertexBuffer& vertexColors,
    const VertexBuffer& lineColors)
{
    float data[] = {
        static_cast<float>(lineIndices.isValid()),
        static_cast<float>(vertexNormals.isValid()),
        static_cast<float>(vertexColors.isValid()),
        static_cast<float>(lineColors.isValid())};

    mCustomIndicesUH.bind(data);
    vertexCoords.bind(0);

    if (lineIndices.isValid())
        lineIndices.bind(1);

    if (vertexNormals.isValid())
        vertexNormals.bind(2);

    if (vertexColors.isValid())
        vertexColors.bind(3);

    if (lineColors.isValid())
        lineColors.bind(4);

    mVertices.bind(5, bgfx::Access::Write);
    mIndices.bind(6, bgfx::Access::Write);

    bgfx::dispatch(0, mComputeVerticesPH, (pointsSize / 2), 1, 1);

    mVertices.setCompute(false);
    mIndices.setCompute(false);
}

} // namespace vcl::detail
