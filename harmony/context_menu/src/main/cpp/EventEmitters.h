/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */
 
#pragma once

#include <react/renderer/components/view/ViewEventEmitter.h>
#include <jsi/jsi.h>

namespace facebook {
namespace react {

class JSI_EXPORT RTNContextMenuEventEmitter : public ViewEventEmitter {
public:
    using ViewEventEmitter::ViewEventEmitter;

    // 属性
    struct OnPress {
        int index;
        std::vector<int> indexPath;
        std::string name;
    };

    struct OnPreviewPress {};

    struct OnCancel {};

    // 方法
    void onContextMenuClick() const;

    void onPress(OnPress value) const;

    void onPreviewPress(OnPreviewPress value) const;

    void onCancel(OnCancel value) const;
};

} // namespace react
} // namespace facebook
