/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include "RTNContextMenuComponentInstance.h"
#include "Props.h"
#import "RNOH/arkui/StackNode.h"
#include <iostream>
#include <vector>

namespace rnoh {

RTNContextMenuComponentInstance::RTNContextMenuComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    // 单点事件
    // m_contextMenuNode.setCustomNodeDelegate(this);
    // 查看node生效的范围
    // m_contextMenuNode.setBackgroundColor(facebook::react::SharedColor(0xff7700ff));
    m_contextMenuNode.setStackNodeDelegate(this);
    ArkUINodeRegistry::getInstance().registerTouchHandler(&m_contextMenuNode, this);
    NativeNodeApi::getInstance()->registerNodeEvent(m_contextMenuNode.getArkUINodeHandle(), NODE_TOUCH_EVENT,
                                                    NODE_TOUCH_EVENT, 0);
    auto rnInstance = m_deps->rnInstance.lock();
    if (rnInstance) {
        // postMessageToArkTS,创建组件发送消息
        rnInstance->postMessageToArkTS("RNGH::ROOT_CREATED", m_tag);
    }
};

RTNContextMenuComponentInstance::~RTNContextMenuComponentInstance() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_contextMenuNode.getArkUINodeHandle(), NODE_TOUCH_EVENT);
    ArkUINodeRegistry::getInstance().unregisterTouchHandler(&m_contextMenuNode);
}

std::optional<std::string> RTNContextMenuComponentInstance::getTintColorFromDynamic(folly::dynamic value) {
    auto rawPropsColor =
        (value.count("backgroundColor") > 0) ? std::optional(value["backgroundColor"].asString()) : std::nullopt;
    return rawPropsColor;
}


void RTNContextMenuComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                      std::size_t index) {
    // 添加子节点，rn框架自带
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    m_contextMenuNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index + 1);
}

void RTNContextMenuComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    // 删子节点，rn框架自带
    CppComponentInstance::onChildRemoved(childComponentInstance);
    m_contextMenuNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

void RTNContextMenuComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    if (auto p = std::dynamic_pointer_cast<const facebook::react::RTNContextMenuProps>(props)) {
        this->title = props->title;
        this->dropdownMenuMode = props->dropdownMenuMode;
        this->disabled = props->disabled;
        this->previewBackgroundColor = props->previewBackgroundColor;
        this->icon = props->icon;
        this->subtitle = props->subtitle;
        this->systemIcon = props->systemIcon;
        this->iconColor = props->iconColor;
        // ios only
        this->destructive = props->destructive;
        this->selected = props->selected;
    }
}


StackNode &RTNContextMenuComponentInstance::getLocalRootArkUINode() {
    return m_contextMenuNode;
};

// 点击时候的传递的参数,从arkTS层发送
void RTNContextMenuComponentInstance::onClick(int index, std::vector<int> indexPath, std::string name) {
    m_eventEmitter->onPress({index : index, indexPath : indexPath, name : name});
}
void RTNContextMenuComponentInstance::onCancel() {
    m_eventEmitter->onCancel({});
}

folly::dynamic
dynamicFromTouchableViews(const std::vector<RTNContextMenuComponentInstance::TouchableView> &touchableViews) {
    folly::dynamic d_touchableViews = folly::dynamic::array();
    for (auto touchableView : touchableViews) {
        folly::dynamic d_touchableView = folly::dynamic::object;
        d_touchableView["tag"] = touchableView.tag;
        d_touchableView["x"] = touchableView.x;
        d_touchableView["y"] = touchableView.y;
        d_touchableView["width"] = touchableView.width;
        d_touchableView["height"] = touchableView.height;
        d_touchableViews.push_back(d_touchableView);
    }
    return d_touchableViews;
}

// 处理单个 RTNContextMenuActionsStruct 对象的函数
folly::dynamic convertToDynamic(const facebook::react::RTNContextMenuActionsStruct &action, int level) {
    folly::dynamic touchViewChildActions = folly::dynamic::object;
    touchViewChildActions["childActionsTag"] = level;
    touchViewChildActions["title"] = action.title;
    touchViewChildActions["subtitle"] = action.subtitle;
    touchViewChildActions["systemIcon"] = action.systemIcon;
    touchViewChildActions["icon"] = action.icon;
    touchViewChildActions["iconColor"] = action.iconColor;
    touchViewChildActions["destructive"] = action.destructive;
    touchViewChildActions["selected"] = action.selected;
    touchViewChildActions["disabled"] = action.disabled;
    touchViewChildActions["inlineChildren"] = action.inlineChildren;

    if (!action.actions.empty()) {
        folly::dynamic childActionsArray = folly::dynamic::array;
        for (const auto &childAction : action.actions) {
            childActionsArray.push_back(convertToDynamic(childAction, level + 1));
        }
        touchViewChildActions["childActions"] = childActionsArray;
    }
    return touchViewChildActions;
}


folly::dynamic convertNodeTouchPointToDynamic(ArkUI_UIInputEvent *e, int32_t index = 0) {
    folly::dynamic result = folly::dynamic::object;
    result["pointerId"] = OH_ArkUI_PointerEvent_GetPointerId(e, index);
    result["windowX"] = OH_ArkUI_PointerEvent_GetWindowXByIndex(e, index);
    result["windowY"] = OH_ArkUI_PointerEvent_GetWindowYByIndex(e, index);

    return result;
}

// 1.首先触发touch事件
void RTNContextMenuComponentInstance::onTouchEvent(ArkUI_UIInputEvent *e) {
    auto ancestor = this->getParent().lock();
    while (ancestor != nullptr) {
        auto ancestorRNGHRootView = std::dynamic_pointer_cast<RTNContextMenuComponentInstance>(ancestor);
        if (ancestorRNGHRootView != nullptr) {
            // If there are multiple nested GestureHandlerRootViews, the one nearest to the actual root will handle
            // the touch.
            return;
        }
        ancestor = ancestor->getParent().lock();
    }

    auto ancestorTouchTarget = this->getTouchTargetParent();
    auto rnInstance = m_deps->rnInstance.lock();
    while (ancestorTouchTarget != nullptr) {
        if (ancestorTouchTarget->isHandlingTouches()) {
            // 这里同理发送给ArtTs的消息，CANCEL_TOUCHES
            rnInstance->postMessageToArkTS("RNGH::CANCEL_TOUCHES", m_tag);
            return;
        }
        ancestorTouchTarget = ancestorTouchTarget->getTouchTargetParent();
    }

    folly::dynamic payload = folly::dynamic::object;
    payload["action"] = OH_ArkUI_UIInputEvent_GetAction(e);
    folly::dynamic touchPoints = folly::dynamic::array();
    auto activeWindowX = OH_ArkUI_PointerEvent_GetWindowX(e);
    auto activeWindowY = OH_ArkUI_PointerEvent_GetWindowY(e);

    // point relative to top left corner of this component（相对位置）
    auto componentX = OH_ArkUI_PointerEvent_GetX(e);
    auto componentY = OH_ArkUI_PointerEvent_GetY(e);
    // 2.通过点击的xy，获取触发的点击视图
    auto touchableViews = this->findTouchableViews(componentX, componentY);

    std::stringstream touchableViewTags;
    for (auto touchableView : touchableViews) {
        touchableViewTags << touchableView.tag << ";";
    }

    // 3.动态获取touchableViews，然后赋值给touchableViews
    payload["touchableViews"] = dynamicFromTouchableViews(touchableViews);

    int32_t pointerCount = OH_ArkUI_PointerEvent_GetPointerCount(e);
    int activePointerIdx = 0;

    for (int i = 0; i < pointerCount; i++) {
        // 4.将触摸节点变成动态
        auto touchPoint = convertNodeTouchPointToDynamic(e, i);
        touchPoints.push_back(touchPoint);
        if (activeWindowX == touchPoint["windowX"].asDouble() && activeWindowY == touchPoint["windowY"].asDouble()) {
            activePointerIdx = i;
        }
    }
    // 根据点击的视图传递view参数
    payload["title"] = this->title;
    payload["dropdownMenuMode"] = this->dropdownMenuMode;
    payload["disabled"] = this->disabled;
    payload["previewBackgroundColor"] = this->previewBackgroundColor;
    
    folly::dynamic touchViewActions = folly::dynamic::array();
    for (auto actions : m_props->actions) {
        folly::dynamic touchViewAction = folly::dynamic::object;
        touchViewAction["title"] = actions.title;
        touchViewAction["subtitle"] = actions.subtitle;
        touchViewAction["systemIcon"] = actions.systemIcon;
        touchViewAction["icon"] = actions.icon;
        touchViewAction["iconColor"] = actions.iconColor;
        touchViewAction["destructive"] = actions.destructive;
        touchViewAction["selected"] = actions.selected;
        touchViewAction["disabled"] = actions.disabled;
        touchViewAction["inlineChildren"] = actions.inlineChildren;
        if (actions.actions.size() > 0) {
            folly::dynamic d_childActions = folly::dynamic::array;
            for (const auto &childAction : actions.actions) {
                d_childActions.push_back(convertToDynamic(childAction, 0));
                touchViewAction["d_childActions"] = d_childActions;
            }
        }
        touchViewActions.push_back(touchViewAction);
    }
    payload["touchViewActions"] = touchViewActions;

    payload["actionTouch"] = touchPoints[activePointerIdx];
    payload["touchPoints"] = touchPoints;
    payload["sourceType"] = OH_ArkUI_UIInputEvent_GetSourceType(e);
    payload["timestamp"] = OH_ArkUI_UIInputEvent_GetEventTime(e);
    payload["rootTag"] = m_tag;

    if (rnInstance) {
        // 点击事件可以在此处获取
        rnInstance->getTurboModule("ContextMenuTurboModule");
        rnInstance->postMessageToArkTS("RNGH::TOUCH_EVENT", payload);
    } else {
    }
}

// findTouchableViews方法
std::vector<RTNContextMenuComponentInstance::TouchableView>
RTNContextMenuComponentInstance::findTouchableViews(float componentX, float componentY) {
    auto touchTarget = findTargetForTouchPoint({.x = componentX, .y = componentY}, this->shared_from_this());
    std::vector<TouchTarget::Shared> touchTargets{};
    auto tmp = touchTarget;
    while (tmp != nullptr) {
        touchTargets.push_back(tmp);
        tmp = tmp->getTouchTargetParent();
    }
    std::reverse(touchTargets.begin(), touchTargets.end()); // leaf / ... / root -> root / ... / leaf
    std::vector<TouchableView> touchableViews{};

    float offsetX = 0;
    float offsetY = 0;
    auto surface = this->getSurface().lock();
    if (surface != nullptr) {
        offsetX = surface->getLayoutContext().viewportOffset.x;
        offsetY = surface->getLayoutContext().viewportOffset.y;
    } else {
        LOG(WARNING) << "Surface is nullptr";
    }
    for (auto &touchTarget : touchTargets) {
        touchableViews.push_back({
            .tag = touchTarget->getTouchTargetTag(),
            .width = touchTarget->getLayoutMetrics().frame.size.width,
            .height = touchTarget->getLayoutMetrics().frame.size.height,
            .x = touchTarget->getLayoutMetrics().frame.origin.x + offsetX,
            .y = touchTarget->getLayoutMetrics().frame.origin.y + offsetY,
        });
        offsetX += touchTarget->getLayoutMetrics().frame.origin.x;
        offsetY += touchTarget->getLayoutMetrics().frame.origin.y;
        offsetX -= touchTarget->getCurrentOffset().x;
        offsetY -= touchTarget->getCurrentOffset().y;
    }

    return touchableViews;
};

} // namespace rnoh