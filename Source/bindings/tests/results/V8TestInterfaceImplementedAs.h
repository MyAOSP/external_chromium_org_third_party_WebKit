/*
    This file is part of the Blink open source project.
    This file has been auto-generated by CodeGeneratorV8.pm. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef V8TestInterfaceImplementedAs_h
#define V8TestInterfaceImplementedAs_h

#include "bindings/bindings/tests/idls/RealClass.h"
#include "bindings/v8/V8Binding.h"
#include "bindings/v8/V8DOMWrapper.h"
#include "bindings/v8/WrapperTypeInfo.h"

namespace WebCore {

class V8TestInterfaceImplementedAs {
public:
    static bool HasInstance(v8::Handle<v8::Value>, v8::Isolate*, WrapperWorldType);
    static bool HasInstanceInAnyWorld(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::FunctionTemplate> GetTemplate(v8::Isolate*, WrapperWorldType);
    static RealClass* toNative(v8::Handle<v8::Object> object)
    {
        return fromInternalPointer(object->GetAlignedPointerFromInternalField(v8DOMWrapperObjectIndex));
    }
    static void derefObject(void*);
    static WrapperTypeInfo info;
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 0;
    static inline void* toInternalPointer(RealClass* impl)
    {
        return impl;
    }

    static inline RealClass* fromInternalPointer(void* object)
    {
        return static_cast<RealClass*>(object);
    }
    static void installPerContextProperties(v8::Handle<v8::Object>, RealClass*, v8::Isolate*) { }
    static void installPerContextPrototypeProperties(v8::Handle<v8::Object>, v8::Isolate*) { }
private:
    friend v8::Handle<v8::Object> wrap(RealClass*, v8::Handle<v8::Object> creationContext, v8::Isolate*);
    static v8::Handle<v8::Object> createWrapper(PassRefPtr<RealClass>, v8::Handle<v8::Object> creationContext, v8::Isolate*);
};

template<>
class WrapperTypeTraits<RealClass > {
public:
    static WrapperTypeInfo* info() { return &V8TestInterfaceImplementedAs::info; }
};


inline v8::Handle<v8::Object> wrap(RealClass* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(DOMDataStore::getWrapper<V8TestInterfaceImplementedAs>(impl, isolate).IsEmpty());
    if (ScriptWrappable::wrapperCanBeStoredInObject(impl)) {
        const WrapperTypeInfo* actualInfo = ScriptWrappable::getTypeInfoFromObject(impl);
        // Might be a XXXConstructor::info instead of an XXX::info. These will both have
        // the same object de-ref functions, though, so use that as the basis of the check.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(actualInfo->derefObjectFunction == V8TestInterfaceImplementedAs::info.derefObjectFunction);
    }
    return V8TestInterfaceImplementedAs::createWrapper(impl, creationContext, isolate);
}

inline v8::Handle<v8::Value> toV8(RealClass* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (UNLIKELY(!impl))
        return v8NullWithCheck(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapper<V8TestInterfaceImplementedAs>(impl, isolate);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, creationContext, isolate);
}

inline v8::Handle<v8::Value> toV8ForMainWorld(RealClass* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(worldType(isolate) == MainWorld);
    if (UNLIKELY(!impl))
        return v8NullWithCheck(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapperForMainWorld<V8TestInterfaceImplementedAs>(impl);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, creationContext, isolate);
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8Fast(RealClass* impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    if (UNLIKELY(!impl))
        return v8::Null(callbackInfo.GetIsolate());
    v8::Handle<v8::Object> wrapper = DOMDataStore::getWrapperFast<V8TestInterfaceImplementedAs>(impl, callbackInfo, wrappable);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8FastForMainWorld(RealClass* impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    ASSERT(worldType(callbackInfo.GetIsolate()) == MainWorld);
    if (UNLIKELY(!impl))
        return v8::Null(callbackInfo.GetIsolate());
    v8::Handle<v8::Object> wrapper = DOMDataStore::getWrapperForMainWorld<V8TestInterfaceImplementedAs>(impl);
    if (!wrapper.IsEmpty())
        return wrapper;
    return wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
}

template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8FastForMainWorld(PassRefPtr< RealClass > impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    return toV8FastForMainWorld(impl.get(), callbackInfo, wrappable);
}


template<class CallbackInfo, class Wrappable>
inline v8::Handle<v8::Value> toV8Fast(PassRefPtr< RealClass > impl, const CallbackInfo& callbackInfo, Wrappable* wrappable)
{
    return toV8Fast(impl.get(), callbackInfo, wrappable);
}

inline v8::Handle<v8::Value> toV8(PassRefPtr< RealClass > impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl.get(), creationContext, isolate);
}

}

#endif // V8TestInterfaceImplementedAs_h
