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

#include "config.h"
#include "V8TestOverloadedConstructors.h"

#include "RuntimeEnabledFeatures.h"
#include "V8Blob.h"
#include "bindings/v8/ExceptionState.h"
#include "bindings/v8/ScriptController.h"
#include "bindings/v8/V8Binding.h"
#include "bindings/v8/V8DOMConfiguration.h"
#include "bindings/v8/V8DOMWrapper.h"
#include "bindings/v8/V8ObjectConstructor.h"
#include "bindings/v8/custom/V8ArrayBufferCustom.h"
#include "bindings/v8/custom/V8ArrayBufferViewCustom.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "core/page/Frame.h"
#include "core/platform/chromium/TraceEvent.h"
#include "wtf/UnusedParam.h"

namespace WebCore {

static void initializeScriptWrappableForInterface(TestOverloadedConstructors* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::setTypeInfoInObject(object, &V8TestOverloadedConstructors::info);
    else
        ASSERT_NOT_REACHED();
}

} // namespace WebCore

// In ScriptWrappable::init, the use of a local function declaration has an issue on Windows:
// the local declaration does not pick up the surrounding namespace. Therefore, we provide this function
// in the global namespace.
// (More info on the MSVC bug here: http://connect.microsoft.com/VisualStudio/feedback/details/664619/the-namespace-of-local-function-declarations-in-c)
void webCoreInitializeScriptWrappableForInterface(WebCore::TestOverloadedConstructors* object)
{
    WebCore::initializeScriptWrappableForInterface(object);
}

namespace WebCore {
WrapperTypeInfo V8TestOverloadedConstructors::info = { V8TestOverloadedConstructors::GetTemplate, V8TestOverloadedConstructors::derefObject, 0, 0, 0, V8TestOverloadedConstructors::installPerContextPrototypeProperties, 0, WrapperTypeObjectPrototype };

namespace TestOverloadedConstructorsV8Internal {

template <typename T> void V8_USE(T) { }

static void constructor1(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    V8TRYCATCH_VOID(ArrayBuffer*, arrayBuffer, args[0]->IsArrayBuffer() ? V8ArrayBuffer::toNative(v8::Handle<v8::ArrayBuffer>::Cast(args[0])) : 0);

    RefPtr<TestOverloadedConstructors> impl = TestOverloadedConstructors::create(arrayBuffer);
    v8::Handle<v8::Object> wrapper = args.Holder();

    V8DOMWrapper::associateObjectWithWrapper<V8TestOverloadedConstructors>(impl.release(), &V8TestOverloadedConstructors::info, wrapper, args.GetIsolate(), WrapperConfiguration::Dependent);
    args.GetReturnValue().Set(wrapper);
}

static void constructor2(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    V8TRYCATCH_VOID(ArrayBufferView*, arrayBufferView, args[0]->IsArrayBufferView() ? V8ArrayBufferView::toNative(v8::Handle<v8::ArrayBufferView>::Cast(args[0])) : 0);

    RefPtr<TestOverloadedConstructors> impl = TestOverloadedConstructors::create(arrayBufferView);
    v8::Handle<v8::Object> wrapper = args.Holder();

    V8DOMWrapper::associateObjectWithWrapper<V8TestOverloadedConstructors>(impl.release(), &V8TestOverloadedConstructors::info, wrapper, args.GetIsolate(), WrapperConfiguration::Dependent);
    args.GetReturnValue().Set(wrapper);
}

static void constructor3(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    V8TRYCATCH_VOID(Blob*, blob, V8Blob::HasInstance(args[0], args.GetIsolate(), worldType(args.GetIsolate())) ? V8Blob::toNative(v8::Handle<v8::Object>::Cast(args[0])) : 0);

    RefPtr<TestOverloadedConstructors> impl = TestOverloadedConstructors::create(blob);
    v8::Handle<v8::Object> wrapper = args.Holder();

    V8DOMWrapper::associateObjectWithWrapper<V8TestOverloadedConstructors>(impl.release(), &V8TestOverloadedConstructors::info, wrapper, args.GetIsolate(), WrapperConfiguration::Dependent);
    args.GetReturnValue().Set(wrapper);
}

static void constructor4(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    V8TRYCATCH_FOR_V8STRINGRESOURCE_VOID(V8StringResource<>, string, args[0]);

    RefPtr<TestOverloadedConstructors> impl = TestOverloadedConstructors::create(string);
    v8::Handle<v8::Object> wrapper = args.Holder();

    V8DOMWrapper::associateObjectWithWrapper<V8TestOverloadedConstructors>(impl.release(), &V8TestOverloadedConstructors::info, wrapper, args.GetIsolate(), WrapperConfiguration::Dependent);
    args.GetReturnValue().Set(wrapper);
}

static void constructor(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    if ((args.Length() == 1 && (V8ArrayBuffer::HasInstance(args[0], args.GetIsolate(), worldType(args.GetIsolate()))))) {
        TestOverloadedConstructorsV8Internal::constructor1(args);
        return;
    }
    if ((args.Length() == 1 && (V8ArrayBufferView::HasInstance(args[0], args.GetIsolate(), worldType(args.GetIsolate()))))) {
        TestOverloadedConstructorsV8Internal::constructor2(args);
        return;
    }
    if ((args.Length() == 1 && (V8Blob::HasInstance(args[0], args.GetIsolate(), worldType(args.GetIsolate()))))) {
        TestOverloadedConstructorsV8Internal::constructor3(args);
        return;
    }
    if (args.Length() == 1) {
        TestOverloadedConstructorsV8Internal::constructor4(args);
        return;
    }
    if (args.Length() < 1) {
        throwNotEnoughArgumentsError(args.GetIsolate());
        return;
    }
    throwTypeError(args.GetIsolate());
    return;
}

} // namespace TestOverloadedConstructorsV8Internal

void V8TestOverloadedConstructors::constructorCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    TRACE_EVENT_SCOPED_SAMPLING_STATE("Blink", "DOMConstructor");
    if (!args.IsConstructCall()) {
        throwTypeError("DOM object constructor cannot be called as a function.", args.GetIsolate());
        return;
    }

    if (ConstructorMode::current() == ConstructorMode::WrapExistingObject) {
        args.GetReturnValue().Set(args.Holder());
        return;
    }

    TestOverloadedConstructorsV8Internal::constructor(args);
}

static v8::Handle<v8::FunctionTemplate> ConfigureV8TestOverloadedConstructorsTemplate(v8::Handle<v8::FunctionTemplate> desc, v8::Isolate* isolate, WrapperWorldType currentWorldType)
{
    desc->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(desc, "TestOverloadedConstructors", v8::Local<v8::FunctionTemplate>(), V8TestOverloadedConstructors::internalFieldCount,
        0, 0,
        0, 0, isolate, currentWorldType);
    UNUSED_PARAM(defaultSignature);
    desc->SetCallHandler(V8TestOverloadedConstructors::constructorCallback);
    desc->SetLength(1);

    // Custom toString template
    desc->Set(v8::String::NewSymbol("toString"), V8PerIsolateData::current()->toStringTemplate());
    return desc;
}

v8::Handle<v8::FunctionTemplate> V8TestOverloadedConstructors::GetTemplate(v8::Isolate* isolate, WrapperWorldType currentWorldType)
{
    V8PerIsolateData* data = V8PerIsolateData::from(isolate);
    V8PerIsolateData::TemplateMap::iterator result = data->templateMap(currentWorldType).find(&info);
    if (result != data->templateMap(currentWorldType).end())
        return result->value.newLocal(isolate);

    TRACE_EVENT_SCOPED_SAMPLING_STATE("Blink", "BuildDOMTemplate");
    v8::HandleScope handleScope(isolate);
    v8::Handle<v8::FunctionTemplate> templ =
        ConfigureV8TestOverloadedConstructorsTemplate(data->rawTemplate(&info, currentWorldType), isolate, currentWorldType);
    data->templateMap(currentWorldType).add(&info, UnsafePersistent<v8::FunctionTemplate>(isolate, templ));
    return handleScope.Close(templ);
}

bool V8TestOverloadedConstructors::HasInstance(v8::Handle<v8::Value> value, v8::Isolate* isolate, WrapperWorldType currentWorldType)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&info, value, currentWorldType);
}

bool V8TestOverloadedConstructors::HasInstanceInAnyWorld(v8::Handle<v8::Value> value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&info, value, MainWorld)
        || V8PerIsolateData::from(isolate)->hasInstance(&info, value, IsolatedWorld)
        || V8PerIsolateData::from(isolate)->hasInstance(&info, value, WorkerWorld);
}

v8::Handle<v8::Object> V8TestOverloadedConstructors::createWrapper(PassRefPtr<TestOverloadedConstructors> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl.get());
    ASSERT(!DOMDataStore::containsWrapper<V8TestOverloadedConstructors>(impl.get(), isolate));
    if (ScriptWrappable::wrapperCanBeStoredInObject(impl.get())) {
        const WrapperTypeInfo* actualInfo = ScriptWrappable::getTypeInfoFromObject(impl.get());
        // Might be a XXXConstructor::info instead of an XXX::info. These will both have
        // the same object de-ref functions, though, so use that as the basis of the check.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(actualInfo->derefObjectFunction == info.derefObjectFunction);
    }

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::createWrapper(creationContext, &info, toInternalPointer(impl.get()), isolate);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installPerContextProperties(wrapper, impl.get(), isolate);
    V8DOMWrapper::associateObjectWithWrapper<V8TestOverloadedConstructors>(impl, &info, wrapper, isolate, WrapperConfiguration::Independent);
    return wrapper;
}

void V8TestOverloadedConstructors::derefObject(void* object)
{
    fromInternalPointer(object)->deref();
}

} // namespace WebCore
