/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#pragma once

#include <react/renderer/components/view/ConcreteViewShadowNode.h>
#include <jsi/jsi.h>
#include "EventEmitters.h"
#include "Props.h"
#include "States.h"

namespace facebook {
namespace react {

JSI_EXPORT extern const char RTNContextMenuComponentName[];

/*
 * `ShadowNode` for <RTNContextMenu> component.
 */
using RTNContextMenuShadowNode = ConcreteViewShadowNode<
    RTNContextMenuComponentName,
    RTNContextMenuProps,
    RTNContextMenuEventEmitter,
    RTNContextMenuState>;

} // namespace react
} // namespace facebook
