/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */
 
#pragma once

#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>
#include <react/renderer/graphics/Color.h>
#include <vector>

namespace facebook {
namespace react {

struct RTNContextMenuActionsStruct {
    std::string title;
    std::string subtitle = "";
    std::string systemIcon;
    std::string icon;
    std::string iconColor;
    bool destructive = false;
    bool selected = false;
    bool disabled = false;
    bool inlineChildren = false;
    std::vector<RTNContextMenuActionsStruct> actions;
};

static inline void fromRawValue(const PropsParserContext &context, const RawValue &value,
                                RTNContextMenuActionsStruct &result) {
    auto map = (butter::map<std::string, RawValue>)value;

    auto tmp_title = map.find("title");
    if (tmp_title != map.end()) {
        fromRawValue(context, tmp_title->second, result.title);
    }
    auto tmp_subtitle = map.find("subtitle");
    if (tmp_subtitle != map.end()) {
        fromRawValue(context, tmp_subtitle->second, result.subtitle);
    }
    auto tmp_systemIcon = map.find("systemIcon");
    if (tmp_systemIcon != map.end()) {
        fromRawValue(context, tmp_systemIcon->second, result.systemIcon);
    }
    auto tmp_icon = map.find("icon");
    if (tmp_icon != map.end()) {
        fromRawValue(context, tmp_icon->second, result.icon);
    }
    auto tmp_iconColor = map.find("iconColor");
    if (tmp_iconColor != map.end()) {
        fromRawValue(context, tmp_iconColor->second, result.iconColor);
    }
    auto tmp_destructive = map.find("destructive");
    if (tmp_destructive != map.end()) {
        fromRawValue(context, tmp_destructive->second, result.destructive);
    }
    auto tmp_selected = map.find("selected");
    if (tmp_selected != map.end()) {
        fromRawValue(context, tmp_selected->second, result.selected);
    }
    auto tmp_disabled = map.find("disabled");
    if (tmp_disabled != map.end()) {
        fromRawValue(context, tmp_disabled->second, result.disabled);
    }
    auto tmp_inlineChildren = map.find("inlineChildren");
    if (tmp_inlineChildren != map.end()) {
        fromRawValue(context, tmp_inlineChildren->second, result.inlineChildren);
    }
    auto tmp_actions = map.find("actions");
    if (tmp_actions != map.end()) {
        fromRawValue(context, tmp_actions->second, result.actions);
    }
}

static inline std::string toString(const RTNContextMenuActionsStruct &value) {
    return "[Object RTNContextMenuActionsStruct]";
}

static inline void fromRawValue(const PropsParserContext &context, const RawValue &value,
                                std::vector<RTNContextMenuActionsStruct> &result) {
    auto items = (std::vector<RawValue>)value;
    for (const auto &item : items) {
        RTNContextMenuActionsStruct newItem;
        fromRawValue(context, item, newItem);
        result.emplace_back(newItem);
    }
}

// Props的定义域
class JSI_EXPORT RTNContextMenuProps final : public ViewProps {
public:
    RTNContextMenuProps() = default;
    RTNContextMenuProps(const PropsParserContext &context, const RTNContextMenuProps &sourceProps,
                        const RawProps &rawProps);

#pragma mark - Props

    std::string title{};
    std::vector<RTNContextMenuActionsStruct> actions{};
    std::string previewBackgroundColor{};
    bool dropdownMenuMode{};
    bool disabled{};
    std::string subtitle{};
    std::string systemIcon{};
    std::string icon{};
    std::string iconColor{};
    bool destructive{};
    bool selected{};
    bool inlineChildren{};
};

} // namespace react
} // namespace facebook
