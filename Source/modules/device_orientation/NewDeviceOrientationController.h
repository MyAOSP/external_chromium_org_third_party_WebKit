/*
 * Copyright 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2012 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NewDeviceOrientationController_h
#define NewDeviceOrientationController_h

#include "core/dom/Event.h"
#include "core/platform/Supplementable.h"
#include "modules/device_orientation/DeviceSensorEventController.h"

namespace WebCore {

class DeviceOrientationData;

// FIXME: rename this class to DeviceOrientationController once Device Orientation is completely implemented
// and the old implementation deleted from core/dom.
class NewDeviceOrientationController : public DeviceSensorEventController, public Supplement<ScriptExecutionContext> {

public:
    virtual ~NewDeviceOrientationController();

    static const char* supplementName();
    static NewDeviceOrientationController* from(Document*);

    void didChangeDeviceOrientation(WebCore::DeviceOrientationData*);

private:
    explicit NewDeviceOrientationController(Document*);
    virtual void registerWithDispatcher() OVERRIDE;
    virtual void unregisterWithDispatcher() OVERRIDE;

    virtual bool hasLastData() OVERRIDE;
    virtual PassRefPtr<Event> getLastEvent() OVERRIDE;
};

} // namespace WebCore

#endif // NewDeviceOrientationController_h