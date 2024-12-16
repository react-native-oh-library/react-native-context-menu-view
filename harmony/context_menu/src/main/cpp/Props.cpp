/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#include "Props.h"
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>

namespace facebook {
namespace react {

RTNContextMenuProps::RTNContextMenuProps(const PropsParserContext &context, const RTNContextMenuProps &sourceProps,
                                         const RawProps &rawProps)
    : ViewProps(context, sourceProps, rawProps),

      title(convertRawProp(context, rawProps, "title", sourceProps.title, {})),
      actions(convertRawProp(context, rawProps, "actions", sourceProps.actions, {})),
      previewBackgroundColor(
          convertRawProp(context, rawProps, "previewBackgroundColor", sourceProps.previewBackgroundColor, {})),
      dropdownMenuMode(convertRawProp(context, rawProps, "dropdownMenuMode", sourceProps.dropdownMenuMode, {false})),
      disabled(convertRawProp(context, rawProps, "disabled", sourceProps.disabled, {false})),
      subtitle(convertRawProp(context, rawProps, "subtitle", sourceProps.subtitle, {})),
      systemIcon(convertRawProp(context, rawProps, "systemIcon", sourceProps.systemIcon, {})),
      icon(convertRawProp(context, rawProps, "icon", sourceProps.icon, {})),
      iconColor(convertRawProp(context, rawProps, "iconColor", sourceProps.iconColor, {})),
      destructive(convertRawProp(context, rawProps, "destructive", sourceProps.destructive, {false})),
      selected(convertRawProp(context, rawProps, "selected", sourceProps.selected, {false})),
      inlineChildren(convertRawProp(context, rawProps, "inlineChildren", sourceProps.inlineChildren, {false})) {}

} // namespace react
} // namespace facebook
