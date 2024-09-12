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

#include "ContextMenuStackNode.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include <ace/xcomponent/native_interface_xcomponent.h>

namespace rnoh {

ContextMenuStackNode::ContextMenuStackNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_CUSTOM)),
      m_contextMenuNodeDelegate(nullptr) {
    // 组件创建在此处，需要set对应的方法
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_CLICK, 0, this));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_HOVER, 0, this));

    NativeNodeApi::getInstance()->registerNodeEventReceiver([](ArkUI_NodeEvent *event) {
        auto eventType = OH_ArkUI_NodeEvent_GetEventType(event);
        auto node = OH_ArkUI_NodeEvent_GetNodeHandle(event);
        auto componentEvent = OH_ArkUI_NodeEvent_GetNodeComponentEvent(event);
        if (componentEvent != nullptr) {
            ContextMenuStackNode contextMenuStackNode;
            contextMenuStackNode.receiveEvent(event);
            // it->second->onNodeEvent(eventType, componentEvent->data);
            return;
        }
    });
}

// 插入子节点
void ContextMenuStackNode::insertChild(ArkUINode &child, std::size_t index) {
    maybeThrow(NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, child.getArkUINodeHandle(), index));
}

// 删除子节点
void ContextMenuStackNode::removeChild(ArkUINode &child) {
    maybeThrow(NativeNodeApi::getInstance()->removeChild(m_nodeHandle, child.getArkUINodeHandle()));
}

// 设置title
ContextMenuStackNode &ContextMenuStackNode::setTitle(std::string const &title) {
    if (title.empty())
        return *this;
    ArkUI_NumberValue indexValue[] = {{.i32 = 1}};
    ArkUI_AttributeItem item = {indexValue, sizeof(indexValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_SWIPER_VERTICAL, &item));
    return *this;
}

// 设置自定义节点
void ContextMenuStackNode::setCustomNodeDelegate(ContextMenuNodeDelegate *contextMenuNodeDelegate) {
    m_contextMenuNodeDelegate = contextMenuNodeDelegate;
}

ContextMenuStackNode::~ContextMenuStackNode() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_CLICK);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_HOVER);
}

// onMeasure回调
void ContextMenuStackNode::onMeasure(ArkUI_NodeCustomEventType eventType) {
    int32_t width = getSavedWidth();
    int32_t height = getSavedHeight();
    maybeThrow(NativeNodeApi::getInstance()->setMeasuredSize(m_nodeHandle, width, height));
}

// onLayout回调
void ContextMenuStackNode::onLayout() { LOG(INFO) << "20240722---------Node:onMeasure----onMeasure"; }

// 节点事件，用来触发点击
void ContextMenuStackNode::onNodeEvent(ArkUI_NodeEventType eventType, EventArgs &eventArgs) {
    if (eventType == ArkUI_NodeEventType::NODE_ON_CLICK && eventArgs[3].i32 == 2) {
        onClick();
    }
    if (eventType == ArkUI_NodeEventType::NODE_ON_HOVER) {
        if (m_contextMenuNodeDelegate != nullptr) {
            if (eventArgs[0].i32) {
                m_contextMenuNodeDelegate->onHoverIn();
            } else {
                m_contextMenuNodeDelegate->onHoverOut();
            }
        }
    }
}

// 单点击事件
void ContextMenuStackNode::onClick() {
    LOG(INFO) << "20240722---------Node:onClick----onClick begin,m_contextMenuNodeDelegate:"
              << m_contextMenuNodeDelegate;
    if (m_contextMenuNodeDelegate != nullptr) {
        LOG(INFO) << "20240722---------Node:onClick----onClick to do ";
        m_contextMenuNodeDelegate->onClick();
    }
}

void ContextMenuStackNode::receiveEvent(ArkUI_NodeEvent *event) {
#ifdef C_API_ARCH
    try {
        auto eventType = OH_ArkUI_NodeEvent_GetEventType(event);
        auto node = OH_ArkUI_NodeEvent_GetNodeHandle(event);
        LOG(INFO) << "20240722---------Node:receiveEvent----eventType: " << eventType << " 自定义node为: " << node;
        if (eventType == ArkUI_NodeEventType::NODE_TOUCH_EVENT) {
            LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent111111111";
            auto it = m_touchHandlerByNodeHandle.find(node);
            if (it == m_touchHandlerByNodeHandle.end()) {
                LOG(WARNING) << "Touch event for node with handle: " << node << " not found";
                return;
            }
            LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent22222222";

            auto inputEvent = OH_ArkUI_NodeEvent_GetInputEvent(event);
            if (inputEvent == nullptr ||
                OH_ArkUI_UIInputEvent_GetType(inputEvent) != ArkUI_UIInputEvent_Type::ARKUI_UIINPUTEVENT_TYPE_TOUCH) {
                LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent333333333";
                return;
            }
            LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent444444444";
            it->second->onTouchEvent(inputEvent);
            return;
        }
        LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent55555555";
        auto it = m_nodeByHandle.find(node);
        if (it == m_nodeByHandle.end()) {
            LOG(INFO) << "20240722---------Node:receiveEvent----m_nodeByHandle:" << node;
            LOG(WARNING) << "Node with handle: " << node << " not found";
            return;
        }
        LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent666666";
        auto componentEvent = OH_ArkUI_NodeEvent_GetNodeComponentEvent(event);
        if (componentEvent != nullptr) {
            LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent777777";
            it->second->onNodeEvent(eventType, componentEvent->data);
            return;
        }
        auto eventString = OH_ArkUI_NodeEvent_GetStringAsyncEvent(event);
        if (eventString != nullptr) {
            LOG(INFO) << "20240722---------Node:receiveEvent----onTouchEvent8888888";
            it->second->onNodeEvent(eventType, std::string_view(eventString->pStr));
            return;
        }
    } catch (std::exception &e) {
        m_arkTSBridge->handleError(std::current_exception());
    }
#endif
};

} // namespace rnoh