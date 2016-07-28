/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSurface_Base.h"
#include "SkImagePriv.h"
#include "SkCanvas.h"
#include "SkGpuDevice.h"

class SkSurface_Gpu : public SkSurface_Base {
public:
    SK_DECLARE_INST_COUNT(SkSurface_Gpu)

    SkSurface_Gpu(GrRenderTarget*, bool cached, const SkSurfaceProps*, bool doClear);
    virtual ~SkSurface_Gpu();

    virtual SkCanvas* onNewCanvas() SK_OVERRIDE;
    virtual SkSurface* onNewSurface(const SkImageInfo&) SK_OVERRIDE;
    virtual SkImage* onNewImageSnapshot() SK_OVERRIDE;
    virtual void onDraw(SkCanvas*, SkScalar x, SkScalar y,
                        const SkPaint*) SK_OVERRIDE;
    virtual void onCopyOnWrite(ContentChangeMode) SK_OVERRIDE;
    virtual void onDiscard() SK_OVERRIDE;

private:
    SkGpuDevice* fDevice;

    typedef SkSurface_Base INHERITED;
};

///////////////////////////////////////////////////////////////////////////////

SkSurface_Gpu::SkSurface_Gpu(GrRenderTarget* renderTarget, bool cached, const SkSurfaceProps* props,
                             bool doClear)
        : INHERITED(renderTarget->width(), renderTarget->height(), props)
{
    int deviceFlags = 0;
    deviceFlags |= cached ? SkGpuDevice::kCached_Flag : 0;
    deviceFlags |= this->props().isUseDistanceFieldFonts() ? SkGpuDevice::kDFFonts_Flag : 0;
    fDevice = SkGpuDevice::Create(renderTarget, this->props(), deviceFlags);

    if (kRGB_565_GrPixelConfig != renderTarget->config() && doClear) {
        fDevice->clear(0x0);
    }
}

SkSurface_Gpu::~SkSurface_Gpu() {
    SkSafeUnref(fDevice);
}

SkCanvas* SkSurface_Gpu::onNewCanvas() {
    SkCanvas::InitFlags flags = SkCanvas::kDefault_InitFlags;
#if defined(COBALT)
    // After a conversation with Brian Salomon from Skia, we have determined
    // that despite the comment below, it is safe to uncomment this, and it
    // increases performance by avoiding software rasterization for the current
    // clip region.
    flags = SkCanvas::kConservativeRasterClip_InitFlag;
#else
    // When we think this works...
//    flags |= SkCanvas::kConservativeRasterClip_InitFlag;
#endif

    return SkNEW_ARGS(SkCanvas, (fDevice, &this->props(), flags));
}

SkSurface* SkSurface_Gpu::onNewSurface(const SkImageInfo& info) {
    GrRenderTarget* rt = fDevice->accessRenderTarget();
    int sampleCount = rt->numSamples();
    return SkSurface::NewRenderTarget(fDevice->context(), info, sampleCount, &this->props());
}

SkImage* SkSurface_Gpu::onNewImageSnapshot() {
    return SkImage::NewTexture(fDevice->accessBitmap(false));
}

void SkSurface_Gpu::onDraw(SkCanvas* canvas, SkScalar x, SkScalar y,
                              const SkPaint* paint) {
    canvas->drawBitmap(fDevice->accessBitmap(false), x, y, paint);
}

// Create a new SkGpuDevice and, if necessary, copy the contents of the old
// device into it. Note that this flushes the SkGpuDevice but
// doesn't force an OpenGL flush.
void SkSurface_Gpu::onCopyOnWrite(ContentChangeMode mode) {
    GrRenderTarget* rt = fDevice->accessRenderTarget();
    // are we sharing our render target with the image?
    SkASSERT(this->getCachedImage());
    if (rt->asTexture() == SkTextureImageGetTexture(this->getCachedImage())) {
        // We call createCompatibleDevice because it uses the texture cache. This isn't
        // necessarily correct (http://skbug.com/2252), but never using the cache causes
        // a Chromium regression. (http://crbug.com/344020)
        SkGpuDevice* newDevice = static_cast<SkGpuDevice*>(
            fDevice->createCompatibleDevice(fDevice->imageInfo()));
        SkAutoTUnref<SkGpuDevice> aurd(newDevice);
        if (kRetain_ContentChangeMode == mode) {
            fDevice->context()->copyTexture(rt->asTexture(), newDevice->accessRenderTarget());
        }
        SkASSERT(this->getCachedCanvas());
        SkASSERT(this->getCachedCanvas()->getDevice() == fDevice);

        this->getCachedCanvas()->setRootDevice(newDevice);
        SkRefCnt_SafeAssign(fDevice, newDevice);
    } else if (kDiscard_ContentChangeMode == mode) {
        this->SkSurface_Gpu::onDiscard();
    }
}

void SkSurface_Gpu::onDiscard() {
    fDevice->accessRenderTarget()->discard();
}

///////////////////////////////////////////////////////////////////////////////

SkSurface* SkSurface::NewRenderTargetDirect(GrRenderTarget* target, const SkSurfaceProps* props) {
    if (NULL == target) {
        return NULL;
    }
    return SkNEW_ARGS(SkSurface_Gpu, (target, false, props, false));
}

SkSurface* SkSurface::NewRenderTarget(GrContext* ctx, const SkImageInfo& info, int sampleCount,
                                      const SkSurfaceProps* props) {
    if (NULL == ctx) {
        return NULL;
    }

    GrTextureDesc desc;
    desc.fFlags = kRenderTarget_GrTextureFlagBit | kCheckAllocation_GrTextureFlagBit;
    desc.fWidth = info.width();
    desc.fHeight = info.height();
    desc.fConfig = SkImageInfo2GrPixelConfig(info);
    desc.fSampleCnt = sampleCount;

    SkAutoTUnref<GrTexture> tex(ctx->createUncachedTexture(desc, NULL, 0));
    if (NULL == tex) {
        return NULL;
    }

    return SkNEW_ARGS(SkSurface_Gpu, (tex->asRenderTarget(), false, props, true));
}

SkSurface* SkSurface::NewScratchRenderTarget(GrContext* ctx, const SkImageInfo& info,
                                             int sampleCount, const SkSurfaceProps* props) {
    if (NULL == ctx) {
        return NULL;
    }

    GrTextureDesc desc;
    desc.fFlags = kRenderTarget_GrTextureFlagBit | kCheckAllocation_GrTextureFlagBit;
    desc.fWidth = info.width();
    desc.fHeight = info.height();
    desc.fConfig = SkImageInfo2GrPixelConfig(info);
    desc.fSampleCnt = sampleCount;

    SkAutoTUnref<GrTexture> tex(ctx->lockAndRefScratchTexture(desc, GrContext::kExact_ScratchTexMatch));

    if (NULL == tex) {
        return NULL;
    }

    return SkNEW_ARGS(SkSurface_Gpu, (tex->asRenderTarget(), true, props, true));
}
