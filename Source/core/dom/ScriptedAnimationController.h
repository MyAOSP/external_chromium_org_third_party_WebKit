/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ScriptedAnimationController_h
#define ScriptedAnimationController_h

#include "heap/Handle.h"
#include "wtf/ListHashSet.h"
#include "wtf/RefCounted.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/StringImpl.h"

namespace WebCore {

class Document;
class Event;
class EventTarget;
class RequestAnimationFrameCallback;

class ScriptedAnimationController : public RefCounted<ScriptedAnimationController> {
public:
    static PassRefPtr<ScriptedAnimationController> create(Document* document)
    {
        return adoptRef(new ScriptedAnimationController(document));
    }
    ~ScriptedAnimationController();
    void clearDocumentPointer() { m_document = 0; }

    typedef int CallbackId;

    int registerCallback(PassOwnPtr<RequestAnimationFrameCallback>);
    void cancelCallback(CallbackId);
    void serviceScriptedAnimations(double monotonicTimeNow);

    void enqueueEvent(PassRefPtrWillBeRawPtr<Event>);
    void enqueuePerFrameEvent(PassRefPtrWillBeRawPtr<Event>);

    void suspend();
    void resume();

private:
    explicit ScriptedAnimationController(Document*);

    void scheduleAnimationIfNeeded();

    void dispatchEvents();
    void executeCallbacks(double monotonicTimeNow);

    typedef Vector<OwnPtr<RequestAnimationFrameCallback> > CallbackList;
    CallbackList m_callbacks;
    CallbackList m_callbacksToInvoke; // only non-empty while inside executeCallbacks

    Document* m_document;
    CallbackId m_nextCallbackId;
    int m_suspendCount;
    WillBePersistentHeapVector<RefPtrWillBeMember<Event> > m_eventQueue;
    ListHashSet<std::pair<const EventTarget*, const StringImpl*> > m_perFrameEvents;
};

}

#endif // ScriptedAnimationController_h
