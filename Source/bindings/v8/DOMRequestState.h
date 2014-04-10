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

#ifndef DOMRequestState_h
#define DOMRequestState_h

#include "bindings/v8/DOMWrapperWorld.h"
#include "bindings/v8/V8Binding.h"
#include "core/dom/ExecutionContext.h"
#include "v8.h"
#include "wtf/RefPtr.h"

namespace WebCore {

// FIXME: DOMRequestState is deprecated and going to be removed. Use NewScriptState instead.
class DOMRequestState {
public:
    explicit DOMRequestState(v8::Isolate* isolate)
        : m_scriptState(NewScriptState::current(isolate))
    {
    }

    class Scope {
    public:
        explicit Scope(DOMRequestState& state)
            : m_handleScope(state.isolate())
            , m_contextScope(state.context())
        {
        }
    private:
        v8::HandleScope m_handleScope;
        v8::Context::Scope m_contextScope;
    };

    void clear() { m_scriptState.clear(); }
    v8::Local<v8::Context> context() { return m_scriptState->context(); }
    v8::Isolate* isolate() const { return m_scriptState->isolate(); }
    bool isValid() const { return m_scriptState; }

private:
    RefPtr<NewScriptState> m_scriptState;
};

}
#endif
