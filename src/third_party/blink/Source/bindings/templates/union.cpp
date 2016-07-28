// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by {{code_generator}}. DO NOT MODIFY!

#include "config.h"
#include "{{header_filename}}"

{% for filename in cpp_includes %}
#include "{{filename}}"
{% endfor %}

namespace blink {

{% for container in containers %}
{{container.cpp_class}}::{{container.cpp_class}}()
    : m_type(SpecificTypeNone)
{
}

{% for member in container.members %}
{{member.rvalue_cpp_type}} {{container.cpp_class}}::getAs{{member.type_name}}()
{
    ASSERT(is{{member.type_name}}());
    return m_{{member.cpp_name}};
}

void {{container.cpp_class}}::set{{member.type_name}}({{member.rvalue_cpp_type}} value)
{
    ASSERT(isNull());
    m_{{member.cpp_name}} = value;
    m_type = {{member.specific_type_enum}};
}

{% endfor %}
{% if container.needs_trace %}
void {{container.cpp_class}}::trace(Visitor* visitor)
{
    {% for member in container.members if member.is_traceable %}
    visitor->trace(m_{{member.cpp_name}});
    {% endfor %}
}

{% endif %}
void V8{{container.cpp_class}}::toImpl(v8::Isolate* isolate, v8::Handle<v8::Value> v8Value, {{container.cpp_class}}& impl, ExceptionState& exceptionState)
{
    {# FIXME: We don't follow the spec on handling null and undefined at this
       moment. Should be fixed once we implement all necessary conversion steps
       below. #}
    if (v8Value.IsEmpty())
        return;

    {# The numbers in the following comments refer to the steps described in
       http://heycam.github.io/webidl/#es-union
       FIXME: Implement all necessary steps #}
    {# 3. Platform objects (interfaces) #}
    {% for interface in container.interface_types %}
    if (V8{{interface.type_name}}::hasInstance(v8Value, isolate)) {
        {{interface.cpp_local_type}} cppValue = V8{{interface.type_name}}::toImpl(v8::Handle<v8::Object>::Cast(v8Value));
        impl.set{{interface.type_name}}(cppValue);
        return;
    }

    {% endfor %}
    {% if container.dictionary_type %}
    {# 12. Dictionaries #}
    {# FIXME: This should also check "object but not Date or RegExp". Add checks
       when we implement conversions for Date and RegExp. #}
    if (isUndefinedOrNull(v8Value) || v8Value->IsObject()) {
        {{container.dictionary_type.cpp_local_type}} cppValue = V8{{container.dictionary_type.type_name}}::toImpl(isolate, v8Value, exceptionState);
        if (!exceptionState.hadException())
            impl.set{{container.dictionary_type.type_name}}(cppValue);
        return;
    }

    {% endif %}
    {# FIXME: In some cases, we can omit boolean and numeric type checks because
       we have fallback conversions. (step 17 and 18) #}
    {% if container.boolean_type %}
    {# 14. Boolean #}
    if (v8Value->IsBoolean()) {
        impl.setBoolean(v8Value->ToBoolean()->Value());
        return;
    }

    {% endif %}
    {% if container.numeric_type %}
    {# 15. Number #}
    if (v8Value->IsNumber()) {
        {{container.numeric_type.v8_value_to_local_cpp_value}};
        impl.set{{container.numeric_type.type_name}}(cppValue);
        return;
    }

    {% endif %}
    {% if container.string_type %}
    {# 16. String #}
    {
        {{container.string_type.v8_value_to_local_cpp_value}};
        impl.set{{container.string_type.type_name}}(cppValue);
        return;
    }

    {# 17. Number (fallback) #}
    {% elif container.numeric_type %}
    {
        {{container.numeric_type.v8_value_to_local_cpp_value}};
        impl.set{{container.numeric_type.type_name}}(cppValue);
        return;
    }

    {# 18. Boolean (fallback) #}
    {% elif container.boolean_type %}
    {
        impl.setBoolean(v8Value->ToBoolean()->Value());
        return;
    }

    {% endif %}
    {# 19. TypeError #}
    exceptionState.throwTypeError("Not a valid union member.");
}

v8::Handle<v8::Value> toV8({{container.cpp_class}}& impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    {# FIXME: We might want to return undefined in some cases #}
    if (impl.isNull())
        return v8::Null(isolate);

    {% for member in container.members %}
    if (impl.is{{member.type_name}}())
        return {{member.cpp_value_to_v8_value}};

    {% endfor %}
    ASSERT_NOT_REACHED();
    return v8::Handle<v8::Value>();
}

{% endfor %}
} // namespace blink
