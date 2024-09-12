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

#pragma once
#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/StackNode.h"

namespace rnoh {
class ContextMenuNodeDelegate {
public:
    virtual ~ContextMenuNodeDelegate() = default;
    virtual void onClick(){};
    virtual void onHoverIn(){};
    virtual void onHoverOut(){};
};

class ContextMenuStackNode: public ArkUINode{
protected:
    ContextMenuNodeDelegate *m_contextMenuNodeDelegate;

public:
    ContextMenuStackNode();
    ~ContextMenuStackNode() override;

    void insertChild(ArkUINode &child, std::size_t index);
    void removeChild(ArkUINode &child);

    ContextMenuStackNode &setTitle(std::string const &);

    void onClick();
    void onMeasure(ArkUI_NodeCustomEventType eventType);
    void onLayout();

    // 点击事件
    void onNodeEvent(ArkUI_NodeEventType eventType, EventArgs &eventArgs) override;
    void receiveEvent(ArkUI_NodeEvent* event);
    void setCustomNodeDelegate(ContextMenuNodeDelegate *contextMenuNodeDelegate);
    std::unordered_map<ArkUI_NodeHandle, TouchEventHandler*>
      m_touchHandlerByNodeHandle;
    std::unordered_map<ArkUI_NodeHandle, ArkUINode*> m_nodeByHandle;
    ArkTSBridge::Shared m_arkTSBridge;

private:
    void (*eventReceiver)(ArkUI_NodeCustomEvent *event);
};

} // namespace rnoh