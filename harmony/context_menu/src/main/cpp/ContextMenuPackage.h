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

#include "RNOH/Package.h"
#include "RTNContextMenuComponentInstance.h"
#include "ComponentDescriptors.h"
#include "ContextMenuJSIBinder.h"
#include "ContextMenuTurboModule.h"

using namespace rnoh;
using namespace facebook;

namespace rnoh {

class ContextMenuPackageComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {

public:
    using ComponentInstanceFactoryDelegate::ComponentInstanceFactoryDelegate;

    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
        if (ctx.componentName == "RTNContextMenu") {
            // RTNContextMenuComponentInstance 这里的方法对应是ComponentInstance.h命名的方法
            return std::make_shared<RTNContextMenuComponentInstance>(std::move(ctx));
        }
        return nullptr;
    }
};

//注册对应的TurboModuleFactoryDelegate，715版本新增。让组件在C化的时候同时进行ArkTs的初始化
class ContextMenuTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
public:
    SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
        if (name == "ContextMenuTurboModule") {
            return std::make_shared<ContextMenuTurboModule>(ctx, name);
        }
        return nullptr;
    };
};

class ContextMenuPackage : public Package {
public:
    ContextMenuPackage(Package::Context ctx) : Package(ctx) {}

    //创建TurboModuleFactoryDelegate,715版本新增
    std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override {
        return std::make_unique<ContextMenuTurboModuleFactoryDelegate>();
    }

    // 创建组件实体类
    ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate() override {
        return std::make_shared<ContextMenuPackageComponentInstanceFactoryDelegate>();
    }

    std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
        return {
            facebook::react::concreteComponentDescriptorProvider<facebook::react::RTNContextMenuComponentDescriptor>()};
    }

    ComponentJSIBinderByString createComponentJSIBinderByName() override {
        return {{"RTNContextMenu", std::make_shared<RTNContextMenuJSIBinder>()}};
    }
    
    // arkTs2CPP
    std::vector<ArkTSMessageHandler::Shared> createArkTSMessageHandlers() override {
        return {std::make_shared<ScrollLockerArkTSMessageHandler>()};
    }

    // 使用handleArkTSMessage接收消息,这里用来接收来自arkTs的消息
    class ScrollLockerArkTSMessageHandler : public ArkTSMessageHandler {
    public:
        void handleArkTSMessage(const Context &ctx) override {
            if (ctx.messageName == "contextMenu::onCancel") {
                LOG(INFO) << "ContextMenuPackage:handleArkTSMessage--onCancel";
                auto itemTag = ctx.messagePayload["itemTag"].asInt();
                auto rnInstance = ctx.rnInstance.lock();
                auto rnInstanceCAPI = std::dynamic_pointer_cast<RNInstanceCAPI>(rnInstance);
                auto contextInstance = rnInstanceCAPI->findComponentInstanceByTag(itemTag);
                if (contextInstance) {
                    auto rnContextInstance =
                        std::dynamic_pointer_cast<RTNContextMenuComponentInstance>(contextInstance);
                    if (rnContextInstance) {
                        rnContextInstance->onCancel();
                        rnInstance->postMessageToArkTS("RNGH::CLOSE_MENU", NULL);
                    }
                }
            }

            if (ctx.messageName == "contextMenu::SET_NATIVE_RESPONDERS_BLOCK") {
                LOG(INFO) << "contextMenu:handleArkTSMessage--SET_NATIVE_RESPONDERS_BLOCK";
                auto itemTag = ctx.messagePayload["itemTag"].asInt();
                auto itemTitle = ctx.messagePayload["itemTitle"].asString();
                auto itemIndex = ctx.messagePayload["itemIndex"].asInt();
                auto itemIndexPath = ctx.messagePayload["itemIndexPath"];
                auto itemInlineChildren = ctx.messagePayload["itemInlineChildren"].asBool();
                // 数据转换,将dynamic类型转成std::vector<int>
                std::vector<int> onPressIndexPath;
                for (size_t i = 0; i < itemIndexPath.size() - 1; i++) {
                    onPressIndexPath.push_back(i);
                }
                auto rnInstance = ctx.rnInstance.lock();
                auto rnInstanceCAPI = std::dynamic_pointer_cast<RNInstanceCAPI>(rnInstance);
                auto contextInstance = rnInstanceCAPI->findComponentInstanceByTag(itemTag);
                LOG(INFO) << "ContextMenuPackage:ContextMenuPackage的方法---------ArkTSMessageHandler---contextInstance:"
                          << contextInstance;
                if (contextInstance) {
                    auto rnContextInstance =
                        std::dynamic_pointer_cast<RTNContextMenuComponentInstance>(contextInstance);
                    if (rnContextInstance) {
                        rnContextInstance->onClick(itemIndex, onPressIndexPath, itemTitle);
                        // 发送消息到arkTS侧，提醒arkTS关闭当前弹框
//                         rnInstance->postMessageToArkTS("RNGH::CLOSE_MENU", NULL);
                    }
                }
            }
        };
    };
};

} // namespace rnoh
