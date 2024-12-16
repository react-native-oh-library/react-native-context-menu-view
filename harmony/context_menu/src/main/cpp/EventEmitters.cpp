/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#include "EventEmitters.h"

namespace facebook {
namespace react {

void RTNContextMenuEventEmitter::onPress(OnPress event) const {
    dispatchEvent("press", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        payload.setProperty(runtime, "index", event.index);

        auto indexPath = jsi::Array(runtime, event.indexPath.size());
        size_t indexPathIndex = 0;
        for (auto indexPathValue : event.indexPath) {
            indexPath.setValueAtIndex(runtime, indexPathIndex++, indexPathValue);
        }
        payload.setProperty(runtime, "indexPath", indexPath);
        payload.setProperty(runtime, "name", event.name);
        return payload;
    });
}
void RTNContextMenuEventEmitter::onPreviewPress(OnPreviewPress event) const {
    dispatchEvent("previewPress", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);

        return payload;
    });
}
void RTNContextMenuEventEmitter::onCancel(OnCancel event) const {
    dispatchEvent("cancel", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RTNContextMenuEventEmitter::onContextMenuClick() const {
    dispatchEvent("onContextMenuClick", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

} // namespace react
} // namespace facebook
