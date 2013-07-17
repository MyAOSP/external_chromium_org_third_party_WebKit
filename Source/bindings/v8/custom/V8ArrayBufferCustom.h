/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef V8ArrayBufferCustom_h
#define V8ArrayBufferCustom_h

#include "bindings/v8/V8Binding.h"
#include "bindings/v8/V8DOMWrapper.h"
#include "bindings/v8/WrapperTypeInfo.h"

#include "wtf/ArrayBuffer.h"

#include <v8.h>

namespace WebCore {

class V8ArrayBufferDeallocationObserver: public WTF::ArrayBufferDeallocationObserver {
public:
    virtual void ArrayBufferDeallocated(unsigned sizeInBytes)
    {
        v8::V8::AdjustAmountOfExternalAllocatedMemory(-static_cast<int>(sizeInBytes));
    }
    static V8ArrayBufferDeallocationObserver* instance();
};

class V8ArrayBuffer {
public:
    static bool HasInstance(v8::Handle<v8::Value>, v8::Isolate*, WrapperWorldType);
    static bool HasInstanceInAnyWorld(v8::Handle<v8::Value>, v8::Isolate*);
    static ArrayBuffer* toNative(v8::Handle<v8::Object>);
    static void derefObject(void*);
    static WrapperTypeInfo info;
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount;
    static void installPerContextProperties(v8::Handle<v8::Object>, ArrayBuffer*, v8::Isolate*) { }
    static void installPerContextPrototypeProperties(v8::Handle<v8::Object>, v8::Isolate*) { }

    static inline void* toInternalPointer(ArrayBuffer* impl)
    {
        return impl;
    }

    static inline ArrayBuffer* fromInternalPointer(void* impl)
    {
        return static_cast<ArrayBuffer*>(impl);
    }

private:
    friend v8::Handle<v8::Object> wrap(ArrayBuffer*, v8::Handle<v8::Object> creationContext, v8::Isolate*);
    static v8::Handle<v8::Object> createWrapper(PassRefPtr<ArrayBuffer>, v8::Handle<v8::Object> creationContext, v8::Isolate*);
};

template<>
class WrapperTypeTraits<ArrayBuffer > {
public:
    static WrapperTypeInfo* info() { return &V8ArrayBuffer::info; }
};


inline v8::Handle<v8::Object> wrap(ArrayBuffer* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(DOMDataStore::getWrapper<V8ArrayBuffer>(impl, isolate).IsEmpty());
    return V8ArrayBuffer::createWrapper(impl, creationContext, isolate);
}

inline v8::Handle<v8::Value> toV8(ArrayBuffer* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (UNLIKELY(!impl))
        return v8NullWithCheck(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapper<V8ArrayBuffer>(impl, isolate);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, creationContext, isolate);
}

inline v8::Handle<v8::Value> toV8ForMainWorld(ArrayBuffer* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(worldType(isolate) == MainWorld);
    if (UNLIKELY(!impl))
        return v8NullWithCheck(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapperForMainWorld<V8ArrayBuffer>(impl);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, creationContext, isolate);
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8Fast(ArrayBuffer* impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    if (UNLIKELY(!impl))
        return v8::Null(callbackInfo.GetIsolate());
    v8::Handle<v8::Object> wrapper = DOMDataStore::getWrapperFast<V8ArrayBuffer>(impl, callbackInfo, wrappable);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8FastForMainWorld(ArrayBuffer* impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    ASSERT(worldType(callbackInfo.GetIsolate()) == MainWorld);
    if (UNLIKELY(!impl))
        return v8Null(callbackInfo.GetIsolate());
    v8::Handle<v8::Object> wrapper = DOMDataStore::getWrapperForMainWorld<V8ArrayBuffer>(impl);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8FastForMainWorld(PassRefPtr< ArrayBuffer > impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    return toV8FastForMainWorld(impl.get(), callbackInfo, wrappable);
}


template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8Fast(PassRefPtr< ArrayBuffer > impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    return toV8Fast(impl.get(), callbackInfo, wrappable);
}

inline v8::Handle<v8::Value> toV8(PassRefPtr< ArrayBuffer > impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl.get(), creationContext, isolate);
}

} // namespace WebCore

#endif // V8ArrayBufferCustom_h
