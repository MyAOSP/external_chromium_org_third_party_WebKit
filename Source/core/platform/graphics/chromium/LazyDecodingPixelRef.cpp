/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "core/platform/graphics/chromium/LazyDecodingPixelRef.h"

#include "SkData.h"
#include "core/platform/chromium/TraceEvent.h"
#include "core/platform/graphics/chromium/ImageDecodingStore.h"
#include "core/platform/graphics/chromium/ImageFrameGenerator.h"
#include <wtf/MainThread.h>

namespace WebCore {

LazyDecodingPixelRef::LazyDecodingPixelRef(PassRefPtr<ImageFrameGenerator> frameGenerator, const SkISize& scaledSize, const SkIRect& scaledSubset)
    : m_frameGenerator(frameGenerator)
    , m_scaledSize(scaledSize)
    , m_scaledSubset(scaledSubset)
    , m_lockedCachedImage(0)
{
}

LazyDecodingPixelRef::~LazyDecodingPixelRef()
{
}

bool LazyDecodingPixelRef::isScaled(const SkISize& fullSize) const
{
    return fullSize != m_scaledSize;
}

bool LazyDecodingPixelRef::isClipped() const
{
    return m_scaledSize.width() != m_scaledSubset.width() || m_scaledSize.height() != m_scaledSubset.height();
}

SkData* LazyDecodingPixelRef::onRefEncodedData()
{
    // If the image has been clipped or scaled, do not return the original encoded data, since
    // on playback it will not be known how the clipping/scaling was done.
    if (isClipped() || isScaled(m_frameGenerator->getFullSize()))
        return 0;
    RefPtr<SharedBuffer> buffer = 0;
    bool allDataReceived = false;
    m_frameGenerator->copyData(&buffer, &allDataReceived);
    if (buffer && allDataReceived) {
        SkData* skdata = SkData::NewWithCopy((void*)buffer->data(), buffer->size());
        return skdata;
    }
    return 0;
}

void* LazyDecodingPixelRef::onLockPixels(SkColorTable**)
{
    TRACE_EVENT_ASYNC_BEGIN0("webkit", "LazyDecodingPixelRef::lockPixels", this);

    m_mutex.lock();
    ASSERT(!m_lockedCachedImage);

    if (!ImageDecodingStore::instance()->lockCache(m_frameGenerator.get(), m_scaledSize, 0, &m_lockedCachedImage))
        m_lockedCachedImage = 0;

    // Use ImageFrameGenerator to generate the image. It will lock the cache
    // entry for us.
    if (!m_lockedCachedImage)
        m_lockedCachedImage = m_frameGenerator->decodeAndScale(m_scaledSize);

    if (!m_lockedCachedImage)
        return 0;

    ASSERT(!m_lockedCachedImage->bitmap().isNull());
    ASSERT(m_lockedCachedImage->scaledSize() == m_scaledSize);
    return m_lockedCachedImage->bitmap().getAddr(m_scaledSubset.x(), m_scaledSubset.y());
}

void LazyDecodingPixelRef::onUnlockPixels()
{
    if (m_lockedCachedImage) {
        ImageDecodingStore::instance()->unlockCache(m_frameGenerator.get(), m_lockedCachedImage);
        m_lockedCachedImage = 0;
    }
    m_mutex.unlock();

    TRACE_EVENT_ASYNC_END0("webkit", "LazyDecodingPixelRef::lockPixels", this);
}

bool LazyDecodingPixelRef::onLockPixelsAreWritable() const
{
    return false;
}

bool LazyDecodingPixelRef::MaybeDecoded()
{
    return ImageDecodingStore::instance()->isCached(m_frameGenerator.get(), m_scaledSize, 0);
}

bool LazyDecodingPixelRef::PrepareToDecode(const LazyPixelRef::PrepareParams& params)
{
    // TODO: check if only a particular rect is available in image cache.
    UNUSED_PARAM(params);
    const ScaledImageFragment* cachedImage = 0;
    bool cached = ImageDecodingStore::instance()->lockCache(m_frameGenerator.get(), m_scaledSize, 0, &cachedImage);
    if (cached)
        ImageDecodingStore::instance()->unlockCache(m_frameGenerator.get(), cachedImage);
    return cached;
}

void LazyDecodingPixelRef::Decode()
{
    lockPixels();
    unlockPixels();
}


} // namespace WebKit
