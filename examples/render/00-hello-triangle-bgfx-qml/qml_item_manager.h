// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef QML_ITEM_MANAGER_H
#define QML_ITEM_MANAGER_H

#include <QImage>
#include <QPainter>
#include <QQuickPaintedItem>
#include <QTimer>

#include <vclib/bgfx/read_from_gpu_buffer.h>
#include <vclib/render/concepts/render_app.h>
#include <vclib/render/window_managers.h>

template<typename DerivedRenderApp>
class QmlItemManager : public QQuickPaintedItem
{
    bool                           mInitialized = false;
    std::string                    mTitle       = "QmlItemManager";
    QImage                         mLastImage;
    QTimer*                        mUpdateTimer;
    vcl::detail::ReadFromGPUBuffer mReader;

public:
    using ParentType = QQuickPaintedItem;

    // Use an existing ID. The Canvas doesn't care as long as windowPtr is null
    static const uint WINDOW_MANAGER_ID = vcl::WindowManagerId::QT_WIDGET;

    QmlItemManager() : QmlItemManager(nullptr) {}

    QmlItemManager(ParentType* parent) : QQuickPaintedItem(parent)
    {
        initItem();
    }

    QmlItemManager(
        const std::string& title,
        vcl::uint          width,
        vcl::uint          height,
        ParentType* parent = nullptr) : QQuickPaintedItem(parent), mTitle(title)
    {
        initItem();
        setWidth(width);
        setHeight(height);
    }

    ~QmlItemManager() override = default;

    const std::string& windowTitle() const { return mTitle; }

    void setWindowTitle(const std::string& title) { mTitle = title; }

    bool isMinimized() const { return false; }

    void setContinuousRedraw(bool enabled)
    {
        if (enabled)
            mUpdateTimer->start(16);
        else
            mUpdateTimer->stop();
    }

    vcl::Point2f dpiScale() const { return vcl::Point2f(1.0f, 1.0f); }

    static void* displayId() { return nullptr; }

    static vcl::NativeWindowHandleType handleType()
    {
        return vcl::NativeWindowHandleType::DEFAULT;
    }

    // Returning nullptr forces vcl::Canvas (BGFX) to create an offscreen
    // framebuffer
    void* winId() { return nullptr; }

    void* windowPtr() { return nullptr; } // used by some drawers

    void paint(QPainter* painter) override
    {
        if (!mLastImage.isNull()) {
            painter->drawImage(boundingRect(), mLastImage);
        }
    }

protected:
    void geometryChange(const QRectF& newGeometry, const QRectF& oldGeometry)
        override
    {
        QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);

        uint w = newGeometry.width();
        uint h = newGeometry.height();

        if (w > 0 && h > 0) {
            if (mInitialized) {
                DerivedRenderApp::WM::resize(derived(), w, h);

                // Re-allocate ReadFromGPUBuffer with new size
                mReader = vcl::detail::ReadFromGPUBuffer(
                    vcl::detail::ReadFromGPUBuffer::Target::COLOR,
                    vcl::Point2<uint>(w, h));
            }
        }
    }

private:
    void initItem()
    {
        static_assert(
            vcl::RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");

        // We will render manually using a timer for simplicity in this example
        // (to simulate a continuous render loop)
        mUpdateTimer = new QTimer(this);
        connect(
            mUpdateTimer,
            &QTimer::timeout,
            this,
            &QmlItemManager::triggerRender);
        mUpdateTimer->start(16); // ~60 FPS
    }

    void triggerRender()
    {
        if (width() <= 0 || height() <= 0)
            return;

        if (!mInitialized) {
            DerivedRenderApp::WM::init(derived());
            DerivedRenderApp::WM::resize(derived(), width(), height());

            // Allocate ReadFromGPUBuffer now that BGFX is initialized
            mReader = vcl::detail::ReadFromGPUBuffer(
                vcl::detail::ReadFromGPUBuffer::Target::COLOR,
                vcl::Point2<uint>(width(), height()));

            mInitialized = true;
        }

        // 1. Ask the reader to prepare for a frame readback
        // Provide a callback to receive the pixels
        mReader.setPendingRead([this](
                                   const vcl::ReadBufferTypes::ReadData& data) {
            const auto& bytes = std::get<vcl::ReadBufferTypes::ByteData>(data);
            if (!bytes.empty()) {
                // Create QImage from the raw RGBA data (bgfx default readback
                // format is usually RGBA8) The image must copy the data because
                // bytes vector is temporary
                QImage img(
                    bytes.data(), width(), height(), QImage::Format_RGBA8888);

                // bgfx textures might be vertically flipped depending on the
                // backend In Vulkan, they usually are not flipped, but we can
                // flip if needed: mLastImage = img.copy().mirrored();
                mLastImage = img.copy();

                // Tell QML to repaint this item using our newly updated
                // mLastImage
                update();
            }
        });

        // 2. Call the RenderApp paint method
        // This will call Canvas::onPaint() -> bgfx::frame(),
        // AND will call onDrawContent of all Drawers.
        // Wait, for offscreen, we must set the view correctly.
        // vclib Canvas sets up the view for the main window, but since our
        // windowPtr is null, it sets up the view for its own offscreen buffer!
        // But the Reader needs the draw calls to go to ITS offscreen buffer!

        // Wait: The HelloTriangleDrawer uses `CanvasType::viewId()`!
        // The reader has its own viewId and framebuffer.
        // Actually, if we use Reader, it wants us to draw into
        // `mReader.viewId()`! But `DerivedRenderApp::WM::paint()` uses
        // `CanvasType::viewId()`.

        // Better: let's modify the reader's view to match, or just manually
        // call the Drawer Since HelloTriangleDrawer has `drawOnView(viewId)`,
        // we can cast and call it directly.
        auto* app    = derived();
        auto* drawer = static_cast<HelloTriangleDrawer<DerivedRenderApp>*>(app);
        drawer->onDrawContent(mReader.viewId());

        // 3. Submit the readback
        mReader.submit();

        // 4. Call bgfx::frame() to actually execute
        uint32_t currentFrame = bgfx::frame();

        // 5. Check if the readback from a previous frame is ready
        mReader.performRead(currentFrame);
    }

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

#endif // QML_ITEM_MANAGER_H
