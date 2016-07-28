// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8TestInterfaceNotScriptWrappable.h"

#include "bindings/core/v8/ExceptionState.h"
#include "bindings/core/v8/V8DOMConfiguration.h"
#include "bindings/core/v8/V8HiddenValue.h"
#include "bindings/core/v8/V8ObjectConstructor.h"
#include "bindings/core/v8/V8TestInterfaceNotScriptWrappable.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/TraceEvent.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

const WrapperTypeInfo V8TestInterfaceNotScriptWrappable::wrapperTypeInfo = { gin::kEmbedderBlink, V8TestInterfaceNotScriptWrappable::domTemplate, V8TestInterfaceNotScriptWrappable::refObject, V8TestInterfaceNotScriptWrappable::derefObject, V8TestInterfaceNotScriptWrappable::trace, 0, 0, 0, V8TestInterfaceNotScriptWrappable::installConditionallyEnabledMethods, V8TestInterfaceNotScriptWrappable::installConditionallyEnabledProperties, 0, WrapperTypeInfo::WrapperTypeObjectPrototype, WrapperTypeInfo::ObjectClassId, WrapperTypeInfo::Independent, WrapperTypeInfo::RefCountedObject };

namespace TestInterfaceNotScriptWrappableV8Internal {

static void attr1AttributeGetter(const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Handle<v8::Object> holder = info.Holder();
    TestInterfaceNotScriptWrappable* impl = V8TestInterfaceNotScriptWrappable::toImpl(holder);
    v8SetReturnValueFast(info, WTF::getPtr(impl->attr1()), impl);
}

static void attr1AttributeGetterCallback(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("blink", "DOMGetter");
    TestInterfaceNotScriptWrappableV8Internal::attr1AttributeGetter(info);
    TRACE_EVENT_SET_SAMPLING_STATE("v8", "V8Execution");
}

static void attr1AttributeSetter(v8::Local<v8::Value> v8Value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Handle<v8::Object> holder = info.Holder();
    TestInterfaceNotScriptWrappable* impl = V8TestInterfaceNotScriptWrappable::toImpl(holder);
    TestInterfaceNotScriptWrappable* cppValue = V8TestInterfaceNotScriptWrappable::toImplWithTypeCheck(info.GetIsolate(), v8Value);
    impl->setAttr1(WTF::getPtr(cppValue));
}

static void attr1AttributeSetterCallback(v8::Local<v8::String>, v8::Local<v8::Value> v8Value, const v8::PropertyCallbackInfo<void>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("blink", "DOMSetter");
    TestInterfaceNotScriptWrappableV8Internal::attr1AttributeSetter(v8Value, info);
    TRACE_EVENT_SET_SAMPLING_STATE("v8", "V8Execution");
}

static void funcMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (UNLIKELY(info.Length() < 1)) {
        V8ThrowException::throwException(createMinimumArityTypeErrorForMethod(info.GetIsolate(), "func", "TestInterfaceNotScriptWrappable", 1, info.Length()), info.GetIsolate());
        return;
    }
    TestInterfaceNotScriptWrappable* impl = V8TestInterfaceNotScriptWrappable::toImpl(info.Holder());
    TestInterfaceNotScriptWrappable* arg;
    {
        arg = V8TestInterfaceNotScriptWrappable::toImplWithTypeCheck(info.GetIsolate(), info[0]);
    }
    impl->func(arg);
}

static void funcMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("blink", "DOMMethod");
    TestInterfaceNotScriptWrappableV8Internal::funcMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("v8", "V8Execution");
}

} // namespace TestInterfaceNotScriptWrappableV8Internal

static const V8DOMConfiguration::AttributeConfiguration V8TestInterfaceNotScriptWrappableAttributes[] = {
    {"attr1", TestInterfaceNotScriptWrappableV8Internal::attr1AttributeGetterCallback, TestInterfaceNotScriptWrappableV8Internal::attr1AttributeSetterCallback, 0, 0, 0, static_cast<v8::AccessControl>(v8::DEFAULT), static_cast<v8::PropertyAttribute>(v8::None), V8DOMConfiguration::ExposedToAllScripts, V8DOMConfiguration::OnInstance},
};

static const V8DOMConfiguration::MethodConfiguration V8TestInterfaceNotScriptWrappableMethods[] = {
    {"func", TestInterfaceNotScriptWrappableV8Internal::funcMethodCallback, 0, 1, V8DOMConfiguration::ExposedToAllScripts},
};

static void installV8TestInterfaceNotScriptWrappableTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "TestInterfaceNotScriptWrappable", v8::Local<v8::FunctionTemplate>(), V8TestInterfaceNotScriptWrappable::internalFieldCount,
        V8TestInterfaceNotScriptWrappableAttributes, WTF_ARRAY_LENGTH(V8TestInterfaceNotScriptWrappableAttributes),
        0, 0,
        V8TestInterfaceNotScriptWrappableMethods, WTF_ARRAY_LENGTH(V8TestInterfaceNotScriptWrappableMethods),
        isolate);
    v8::Local<v8::ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
    ALLOW_UNUSED_LOCAL(instanceTemplate);
    v8::Local<v8::ObjectTemplate> prototypeTemplate = functionTemplate->PrototypeTemplate();
    ALLOW_UNUSED_LOCAL(prototypeTemplate);

    // Custom toString template
    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::from(isolate)->toStringTemplate());
}

v8::Handle<v8::FunctionTemplate> V8TestInterfaceNotScriptWrappable::domTemplate(v8::Isolate* isolate)
{
    return V8DOMConfiguration::domClassTemplate(isolate, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), installV8TestInterfaceNotScriptWrappableTemplate);
}

bool V8TestInterfaceNotScriptWrappable::hasInstance(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Handle<v8::Object> V8TestInterfaceNotScriptWrappable::findInstanceInPrototypeChain(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

TestInterfaceNotScriptWrappable* V8TestInterfaceNotScriptWrappable::toImplWithTypeCheck(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    return hasInstance(value, isolate) ? blink::toScriptWrappableBase(v8::Handle<v8::Object>::Cast(value))->toImpl<TestInterfaceNotScriptWrappable>() : 0;
}

v8::Handle<v8::Object> wrap(TestInterfaceNotScriptWrappable* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8TestInterfaceNotScriptWrappable>(impl, isolate));
    return V8TestInterfaceNotScriptWrappable::createWrapper(impl, creationContext, isolate);
}

v8::Handle<v8::Object> V8TestInterfaceNotScriptWrappable::createWrapper(PassRefPtr<TestInterfaceNotScriptWrappable> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8TestInterfaceNotScriptWrappable>(impl.get(), isolate));

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::createWrapper(creationContext, &wrapperTypeInfo, impl->toScriptWrappableBase(), isolate);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installConditionallyEnabledProperties(wrapper, isolate);
    V8DOMWrapper::associateObjectWithWrapper<V8TestInterfaceNotScriptWrappable>(impl, &wrapperTypeInfo, wrapper, isolate);
    return wrapper;
}

void V8TestInterfaceNotScriptWrappable::refObject(ScriptWrappableBase* scriptWrappableBase)
{
    scriptWrappableBase->toImpl<TestInterfaceNotScriptWrappable>()->ref();
}

void V8TestInterfaceNotScriptWrappable::derefObject(ScriptWrappableBase* scriptWrappableBase)
{
    scriptWrappableBase->toImpl<TestInterfaceNotScriptWrappable>()->deref();
}

template<>
v8::Handle<v8::Value> toV8NoInline(TestInterfaceNotScriptWrappable* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace blink
