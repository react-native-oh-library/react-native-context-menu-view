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

import { RNOHContext, } from '@rnoh/react-native-openharmony'
import Logger from './Logger';
import { SymbolGlyphModifier } from '@kit.ArkUI';
import { ContextMenuThreeMenu } from './ContextMenuThreeMenu';

//这里的属性类型要和ContextMenuTurboModule.ets下的ContextMenuProps一致
interface ContextMenuProps {
  tag?: number;
  width?: number;
  height?: number;
  x?: number;
  y?: number;
  windowX?: number;
  windowY?: number;
  title?: string;
  action?: string;
  dropdownMenuMode?: boolean;
  disabled?: boolean;
  subtitle?: string;

  // disabledArray?: Array<boolean>;
  selected?: boolean;
  previewBackgroundColor?: string;
  actionsTitle?: string;
  touchViewActions?: ContextMenuProps
  ctx: RNOHContext
  romWidth?: number;
  romHeight?: number;
  systemIcon?: string;
  icon?: string;
  destructive?: string
  iconColor?: string;

  //二级目录
  inlineChildren?: boolean;
  d_childActions?: ContextMenuProps;
  childActionsTag?: number;

  //2+目录
  childActions?: ContextMenuProps;
}

@Component
export struct ContextMenuThreeMenuWithNoSelect {
  public static readonly NAME = "ContextMenuThreeMenuWithNoSelect"
  menuCtx!: RNOHContext;
  menuTag: number = 0;
  longPress: ESObject = []
  @State realViewPoint: Object = [];
  @State listDate: ContextMenuProps[] = [];
  @State menuIndexPath: number[] = [];
  @State menuDestructive: string = '';
  @State childActionsListDate: ESObject[] = [];
  @State isTouch: boolean | undefined = false;
  @State isTouchViewNumber: number = 0;
  @State isChildActionsTouchViewNumber: number = 0;
  @State menuChildActions: ContextMenuProps[] = [];
  @State menuChild: ESObject[] = [];
  @State deepChildActions: ESObject[] = [];
  @State isShow: boolean = false;
  @State nextMenu: ESObject[] = [];
  @State selectedFlag: number = 0;

  aboutToAppear() {
    //从前一个菜单传递过来的
    this.longPress = this.realViewPoint;
    this.menuCtx = this.menuCtx;
    this.menuTag = this.menuTag;
    Logger.info('2024-3menu:' + JSON.stringify(this.longPress))
    //menu的大标题
    this.listDate = this.longPress;
    Logger.info('2024-3menu,this.listDate:' + JSON.stringify(this.listDate))
    for (let i = 0; i < this.listDate.length; i++) {
      if (this.listDate[i] !== null && this.listDate[i] !== undefined) {
        //为true，表示禁用，对应harmony next 的false
        if (this.listDate[i].disabled) {
          //为true，表示禁用，对应harmony next 的false
          this.listDate[i].disabled = false;
        } else {
          //contextMenu的false 对标 harmony 的 true
          this.listDate[i].disabled = true;
        }
      }
    }
    //遍历传递回前端的indexPath,indexPath只会计算子节点index
    //第一个节点表示对应的深层目录，从3级起算
    for (let i = 0; i < this.listDate.length; i++) {
      this.menuIndexPath[i+1] = i;
    }
    for (let i = 0; i < this.listDate.length; i++) {
      if (this.longPress[i].selected) {
        this.selectedFlag++;
      }
    }

  }

  //退出组件
  aboutToDisappear() {
  }

  @Builder
  NextMenuWithNoSelect() {
    ContextMenuThreeMenuWithNoSelect(
      {
        realViewPoint: this.nextMenu,
        menuCtx: this.menuCtx,
        menuTag: this.menuTag
      });
  }

  @Builder
  NextMenu() {
    ContextMenuThreeMenu(
      {
        realViewPoint: this.nextMenu,
        menuCtx: this.menuCtx,
        menuTag: this.menuTag
      });
  }

  build() {
    Menu() {
      ForEach(this.listDate, (childItem: ContextMenuProps, index1) => {
        if (childItem.systemIcon) {
          MenuItem({
            content: childItem.title,
            symbolEndIcon: new SymbolGlyphModifier($r(childItem.systemIcon)).fontSize('24vp')
              .fontColor([childItem.iconColor]),
            labelInfo: childItem.subtitle,
            builder: (): void => {
              if (childItem.childActions !== undefined) {
                this.nextMenu = this.longPress[index1].childActions;
                if (this.selectedFlag !== 0) {
                  this.NextMenu();
                } else {
                  this.NextMenuWithNoSelect();
                }
              }
            }
          })//selected复选标记,根据传入的selected来判断selected为默认为false,不显示
            .alignSelf(ItemAlign.Start)
            .enabled(childItem.disabled)
            .contentFontColor(childItem.destructive)
            .onClick(() => {
              this.nextMenu = this.longPress[index1].childActions;
              this.isTouch = true && (!childItem.inlineChildren) && (childItem.childActions !== undefined);
              this.isTouchViewNumber = index1;
              if (childItem.inlineChildren || (childItem.childActions == undefined)) {
                this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                  itemTitle: childItem.title,
                  itemTag: this.menuTag,
                  itemIndex: index1,
                  itemIndexPath: this.menuIndexPath,
                  itemInlineChildren: childItem.inlineChildren
                });
              }
            })
        } else {
          MenuItem({
            content: childItem.title,
            endIcon: $r(childItem.icon),
            labelInfo: childItem.subtitle,
            builder: (): void => {
              if (childItem.childActions !== undefined) {
                this.nextMenu = this.longPress[index1].childActions;
                if (this.selectedFlag !== 0) {
                  this.NextMenu();
                } else {
                  this.NextMenuWithNoSelect();
                }
              }
            }
          })//selected复选标记,根据传入的selected来判断selected为默认为false,不显示
            .alignSelf(ItemAlign.Start)
            .enabled(childItem.disabled)
            .contentFontColor(childItem.destructive)
            .onClick(() => {
              this.nextMenu = this.longPress[index1].childActions;
              this.isTouch = true && (!childItem.inlineChildren) && (childItem.childActions !== undefined);
              this.isTouchViewNumber = index1;
              if (childItem.inlineChildren || (childItem.childActions == undefined)) {
                this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                  itemTitle: childItem.title,
                  itemTag: this.menuTag,
                  itemIndex: index1,
                  itemIndexPath: this.menuIndexPath,
                  itemInlineChildren: childItem.inlineChildren
                });
              }
            })
        }

      })
    }
  }
}