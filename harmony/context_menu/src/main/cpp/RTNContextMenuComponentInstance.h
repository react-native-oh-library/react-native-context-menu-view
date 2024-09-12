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
#include "RNOH/CppComponentInstance.h"
// #include "ContextMenuStackNode.h"
#include "ShadowNodes.h"
#include "Props.h"
#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/CustomNode.h"
#include "RNOH/arkui/TextNode.h"
#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/ArkUINodeRegistry.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "RNOH/RNInstanceCAPI.h"

#include "RNOH/arkui/StackNode.h"


namespace rnoh {
class RTNContextMenuComponentInstance : public CppComponentInstance<facebook::react::RTNContextMenuShadowNode>,
                                        //    public ContextMenuNodeDelegate
                                        public StackNodeDelegate,
                                        public TouchEventHandler {

    using Point = facebook::react::Point;

    /**
     * This function is borrowed from TouchEventDispatcher
     */
    static TouchTarget::Shared findTargetForTouchPoint(Point const &point, TouchTarget::Shared const &target) {
        bool canHandleTouch =
            target->canHandleTouch() && target->containsPoint(point) && (target->getTouchEventEmitter() != nullptr);
        bool canChildrenHandleTouch = target->canChildrenHandleTouch() && target->containsPointInBoundingBox(point);

        if (canChildrenHandleTouch) {
            auto children = target->getTouchTargetChildren();
            // we want to check the children in reverse order, since the last child is the topmost one
            std::reverse(children.begin(), children.end());
            for (auto const &child : children) {
                auto childPoint = target->computeChildPoint(point, child);
                auto result = findTargetForTouchPoint(childPoint, child);
                if (result != nullptr) {
                    return result;
                }
            }
        }
        if (canHandleTouch) {
            return target;
        }
        return nullptr;
    }

public:
    RTNContextMenuComponentInstance(Context context);
    ~RTNContextMenuComponentInstance();

    // override属于覆写接口，需要实例化
    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;

    // arkui为页面ui
    // ContextMenuStackNode &getLocalRootArkUINode() override;
    StackNode &getLocalRootArkUINode() override;

    void onPropsChanged(SharedConcreteProps const &props) override;

    void setIsHandlingTouches(bool isHandlingTouches) { m_isHandlingTouches = isHandlingTouches; }

    bool isHandlingTouches() const override { return m_isHandlingTouches; }

    // StackNodeDelegate
    void onClick(int index, std::vector<int> indexPath, std::string name);
    void onCancel ();
    void onTouchEvent(ArkUI_UIInputEvent *e) override;

    struct TouchableView {
        Tag tag;
        facebook::react::Float width;
        facebook::react::Float height;
        facebook::react::Float x;
        facebook::react::Float y;
    };

    struct RTNContextMenuActionsStruct {
        std::string title;
        std::string subtitle;
        std::string systemIcon;
        std::string icon;
        std::string iconColor;
        bool destructive;
        bool selected;
        bool disabled;
        bool inlineChildren;
        std::vector<RTNContextMenuActionsStruct> actions;
    };

private:
    //  ContextMenuStackNode m_contextMenuNode{};
    //     StackNode m_contextMenuNode{};

    std::string title{};
    //     std::vector<facebook::react::RTNContextMenuProps> actions{};
    //     std::vector<RTNContextMenuActionsStruct> actions{};
    //     RTNContextMenuActionsStruct actions{};
    RTNContextMenuActionsStruct actions{};
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

    bool m_isHandlingTouches = false;
    StackNode m_contextMenuNode;
    std::vector<TouchableView> findTouchableViews(float componentX, float componentY);
    std::optional<std::string> getTintColorFromDynamic(folly::dynamic value);
    
    
    Surface::Weak m_surface;
    Surface::Weak getSurface() {
        if (m_surface.lock() != nullptr) {
            return m_surface;
        }
        auto rnInstance = m_deps->rnInstance.lock();
        if (rnInstance == nullptr) {
            m_surface.reset();
            return m_surface;
        }
        ComponentInstance::Shared currentRoot = shared_from_this();
        while (true) {
            auto maybeNewCurrentRoot = currentRoot->getParent().lock();
            if (maybeNewCurrentRoot == nullptr) {
                break;
            }
            currentRoot = maybeNewCurrentRoot;
        }
        auto maybeSurface = rnInstance->getSurfaceByRootTag(currentRoot->getTag());
        if (!maybeSurface.has_value()) {
            m_surface.reset();
            return m_surface;
        }
        m_surface = maybeSurface.value();
        return m_surface;
    }
};

} // namespace rnoh
