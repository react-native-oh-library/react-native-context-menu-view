/*
 * MIT License
 *
 * Copyright (C) 2023 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
class RTNContextMenuJSIBinder : public ViewComponentJSIBinder {

protected:
    facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
        LOG(INFO) << "20240722---------createDirectEventTypes----createNativeProps";
        auto object = ViewComponentJSIBinder::createNativeProps(rt);
        object.setProperty(rt, "title", "string");
        object.setProperty(rt, "subtitle", "string");
        object.setProperty(rt, "icon", "string");
        object.setProperty(rt, "systemIcon", "string");
        object.setProperty(rt, "dropdownMenuMode", "bool");
        object.setProperty(rt, "disabled", "bool");
        object.setProperty(rt, "destructive", "bool");
        object.setProperty(rt, "inlineChildren", "bool");
        object.setProperty(rt, "selected", "bool");
        object.setProperty(rt, "previewBackgroundColor", "string");
        object.setProperty(rt, "actions", "Object");
        return object;
    }

    facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
        LOG(INFO) << "20240722---------createDirectEventTypes----createDirectEventTypes";
        facebook::jsi::Object events = ViewComponentJSIBinder::createDirectEventTypes(rt);
        events.setProperty(rt, "topPress", createDirectEvent(rt, "onPress"));
        events.setProperty(rt, "topPreviewPress", createDirectEvent(rt, "onPreviewPress"));
        events.setProperty(rt, "topCancel", createDirectEvent(rt, "onCancel"));
        return events;
    }
};
} // namespace rnoh