// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_lines.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/editors/editor.h>
#include <vclib/render/settings/show_normals_editor_settings.h>

#include <nlohmann/json.hpp>

namespace vcl {

template<typename ViewerDrawer>
class ShowNormalsEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    std::vector<DrawableLines> mVertexNormalsLines;
    std::vector<DrawableLines> mFaceNormalsLines;

    ShowNormalsEditorSettings mSettings;

public:
    ShowNormalsEditorBGFX() = default;

    std::string name() const override { return "Show Normals"; }

    void onViewerSet() override
    {
        Base::viewerRegisterGlobalAction(
            "Toggle Show Normals",
            {Key::N, {KeyModifier::NO_MODIFIER}},
            [this]() {
                this->setActive(!this->isActive());
            });
    }

    ShowNormalsEditorSettings& settings() override { return mSettings; }

    const ShowNormalsEditorSettings& settings() const override
    {
        return mSettings;
    }

    void loadSettings(const nlohmann::json& j) override
    {
        mSettings.loadSettings(j);
    }

    void saveSettings(nlohmann::json& j) const override
    {
        mSettings.saveSettings(j);
    }

    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    void refresh() override
    {
        mVertexNormalsLines.clear();
        mFaceNormalsLines.clear();

        for (const auto& drawable : *Base::drawList()) {
            const AbstractDrawableMesh* m =
                dynamic_cast<const AbstractDrawableMesh*>(drawable.get());
            if (m) {
                const auto& provider = m->meshProvider();

                double diag       = m->boundingBox().diagonal();
                double lineLength = diag * mSettings.normalLengthRatio;

                // Vertex Normals
                mVertexNormalsLines.push_back(
                    vertexNormalLines(provider, lineLength));

                // Face Normals
                mFaceNormalsLines.push_back(
                    faceNormalLines(provider, lineLength));
            }
            else {
                mVertexNormalsLines.push_back(DrawableLines());
                mFaceNormalsLines.push_back(DrawableLines());
            }
        }
        Base::viewerUpdate();
    }

    void refreshSettings() override
    {
        for (auto& l : mVertexNormalsLines) {
            l.setGeneralColor(mSettings.vertexNormalColor);
            l.setWidth(mSettings.thickness);
        }
        for (auto& l : mFaceNormalsLines) {
            l.setGeneralColor(mSettings.faceNormalColor);
            l.setWidth(mSettings.thickness);
        }
        Base::viewerUpdate();
    }

    void drawContent(uint viewId) override
    {
        using enum EditorSettings::EditMode;

        DrawObjectSettings settings;
        settings.viewId = viewId;

        if (mSettings.editMode == NONE)
            return;

        if (mSettings.editMode == CURRENT_OBJECT) {
            uint id = Base::drawList()->selectedObjectId();
            if (id < mVertexNormalsLines.size()) {
                if (mSettings.showVertexNormals)
                    mVertexNormalsLines[id].draw(settings);
                if (mSettings.showFaceNormals)
                    mFaceNormalsLines[id].draw(settings);
            }
        }
        else {
            for (uint i = 0; i < mVertexNormalsLines.size(); ++i) {
                bool show = (mSettings.editMode == VISIBLE_OBJECTS) ?
                                Base::drawList()->at(i)->isVisible() :
                                true;

                if (show) {
                    if (mSettings.showVertexNormals)
                        mVertexNormalsLines[i].draw(settings);
                    if (mSettings.showFaceNormals)
                        mFaceNormalsLines[i].draw(settings);
                }
            }
        }
    }

private:
    DrawableLines vertexNormalLines(
        const AbstractMeshProvider& provider,
        double                      lineLength)
    {
        DrawableLines vLines;
        if (provider.hasVertexNormals()) {
            std::vector<vcl::Point3d> pts;
            std::vector<uint>         ids;
            uint                      vCount = provider.vertexCount();
            vcl::Matrix44d            T      = provider.transformMatrix();

            for (uint i = 0; i < vCount; ++i) {
                vcl::Point3d pos = provider.vertexPosition(i);
                vcl::Point3d n   = provider.vertexNormal(i);
                if (n.squaredNorm() > 0) {
                    pos *= T;
                    n = vcl::multiplyNormalByMatrix(n, T);
                    n.normalize();
                    uint sId = pts.size();
                    pts.push_back(pos);
                    pts.push_back(pos + n * lineLength);
                    ids.push_back(sId);
                    ids.push_back(sId + 1);
                }
            }
            vLines.setVertices(pts);
            vLines.setIndices(ids);
            vLines.setGeneralColor(mSettings.vertexNormalColor);
            vLines.setWidth(mSettings.thickness);
            vLines.setTopology(DrawableLines::Topology::LINES);
        }

        return vLines;
    }

    DrawableLines faceNormalLines(
        const AbstractMeshProvider& provider,
        double                      lineLength)
    {
        DrawableLines fLines;
        if (provider.hasFaceNormals()) {
            std::vector<vcl::Point3d> pts;
            std::vector<uint>         ids;
            uint                      fCount = provider.faceCount();
            vcl::Matrix44d            T      = provider.transformMatrix();

            for (uint i = 0; i < fCount; ++i) {
                vcl::Point3d barycenter = provider.faceBarycenter(i);
                vcl::Point3d n          = provider.faceNormal(i);
                if (n.squaredNorm() > 0) {
                    barycenter *= T;
                    n = vcl::multiplyNormalByMatrix(n, T);
                    n.normalize();
                    uint sId = pts.size();
                    pts.push_back(barycenter);
                    pts.push_back(barycenter + n * lineLength);
                    ids.push_back(sId);
                    ids.push_back(sId + 1);
                }
            }
            fLines.setVertices(pts);
            fLines.setIndices(ids);
            fLines.setGeneralColor(mSettings.faceNormalColor);
            fLines.setWidth(mSettings.thickness);
            fLines.setTopology(DrawableLines::Topology::LINES);
        }

        return fLines;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SHOW_NORMALS_EDITOR_BGFX_H
