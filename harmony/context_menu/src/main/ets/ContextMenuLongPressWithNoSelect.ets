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
import { ContextMenuTwoMenu } from './ContextMenuTwoMenu';
import { ContextMenuTwoMenuWithNoSelect } from './ContextMenuTwoMenuWithNoSelect';

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
export struct ContextMenuLongPressWithNoSelect {
  public static readonly NAME = "ContextMenuLongPressWithNoSelect"
  menuCtx!: RNOHContext;
  menuTag: number = 0;
  @State realViewPoint: Object = [];
  @State listDate: ContextMenuProps[] = [];
  @State menuTitle: string = '';
  @State menuX: number = 0;
  @State menuY: number = 0;
  @State menuWidth: number = 0;
  @State menuHeight: number = 0;
  @State menuDisable: boolean = true;
  @State menuRomWidth: number = 0;
  @State menuRomHeight: number = 0;
  @State menuWindowX: number = 0;
  @State menuWindowY: number = 0;
  @State menuIndexPath: number[] = [];
  @State menuDestructive: string = '';
  @State childActionsListDate: ESObject[] = [];
  @State isTouch: boolean | undefined = false;
  @State isTouchWithNoSelect: boolean | undefined = false;
  @State isTouchViewNumber: number = 0;
  @State isChildActionsTouchViewNumber: number = 0;
  @State menuChildActions: ContextMenuProps[] = [];
  @State menuChild: ESObject[] = [];
  @State deepChildActions: ESObject[] = [];
  @State isShow: boolean = false;
  @State twoMenu: ESObject[] = [];
  @State selectedFlag: ESObject[] = [ ];

  aboutToAppear() {
    let longPress: ESObject = this.realViewPoint;
    //menu的大标题
    this.menuTitle = longPress.title;
    //获取menu的xy轴，以及view视图的长宽
    this.menuX = longPress.x;
    this.menuY = longPress.y;
    this.menuWidth = longPress.width;
    this.menuHeight = longPress.height;
    this.menuCtx = longPress.ctx;
    this.menuTag = longPress.tag;
    //获取设备的长宽
    this.menuRomWidth = longPress.romWidth;
    this.menuRomHeight = longPress.romHeight;
    //获取点击时候的点击事件的xy,基于屏幕左上角
    this.menuWindowX = longPress.windowX;
    this.menuWindowY = longPress.windowY;
    this.listDate = longPress.touchViewActions;
    //判断destructive(可破坏)，默认false，表示不可破坏。
    for (let i = 0; i < longPress.touchViewActions.length; i++) {
      if (longPress.touchViewActions[i].destructive) {
        //可破坏为红色
        this.listDate[i].destructive = 'red';
      } else {
        //不可破坏为black
        this.listDate[i].destructive = 'black';
      }
    }
    //  contextMenu入参为disabled,是否禁用选项，默认为false，表示不用禁用.
    //  但是在harmony Next调用enable的入参,harmony Next:true =  RN：false
    for (let i = 0; i < longPress.touchViewActions.length; i++) {
      if (longPress.touchViewActions[i].disabled) {
        //为true，表示禁用，对应harmony next 的false
        this.listDate[i].disabled = false;
      } else {
        //contextMenu的false 对标 harmony 的 true
        this.listDate[i].disabled = true;
      }
    }
    for (let i = 0; i < longPress.touchViewActions.length; i++) {
      if (longPress.touchViewActions[i].selected) {
        //复选框，默认为false
        this.listDate[i].selected = true;
      } else {
        this.listDate[i].selected = false;
      }
    }

    //遍历传递回前端的indexPath,indexPath只会计算子节点index
    for (let i = 0; i < longPress.touchViewActions.length; i++) {
      this.menuIndexPath[i] = i;
    }

    //二级目录,均和一级目录一致
    // 遍历查询是否有二级目录
    for (let i = 0; i < longPress.touchViewActions.length; i++) {
      if (longPress.touchViewActions[i].d_childActions !== '' ||
        longPress.touchViewActions[i].d_childActions !== undefined ||
        longPress.touchViewActions[i].d_childActions !== null) {
        //有二级菜单
        this.childActionsListDate[i] = longPress.touchViewActions[i].d_childActions;
      }
    }
    ;

    for (let i = 0; i < this.childActionsListDate.length; i++) {
      if (this.childActionsListDate[i] !== null && this.childActionsListDate[i] !== undefined &&
        this.childActionsListDate[i] !== '') {
        let j = i;
        //默认为false，这里判断disabled
        for (let k = 0; k < this.childActionsListDate[j].length; k++) {
          this.menuIndexPath[k] = k;
          if ((this.childActionsListDate[j])[k].disabled) {
            (this.childActionsListDate[j])[k].disabled = false;
          } else {
            //contextMenu的false 对标 harmony 的 true
            (this.childActionsListDate[j])[k].disabled = true;
          }
          if ((this.childActionsListDate[j])[k].selected) {
            this.selectedFlag[k] = 1;
          } else {
            this.selectedFlag[k] = 0;
          }
        }
      }
    }
    console.log('20240919----------------------'+JSON.stringify(this.childActionsListDate.length));
  }

  //退出组件
  aboutToDisappear() {
  }

  @Builder
  twoMenuView() {
    ContextMenuTwoMenu(
      {
        realViewPoint: this.twoMenu,
        menuCtx: this.menuCtx,
        menuTag: this.menuTag
      });
  }

  @Builder
  twoMenuViewWithNoSelect() {
    ContextMenuTwoMenuWithNoSelect(
      {
        realViewPoint: this.twoMenu,
        menuCtx: this.menuCtx,
        menuTag: this.menuTag
      });
  }

  build() {
    Menu() {
      MenuItemGroup({ header: this.menuTitle }) {
        //遍历数据，把每个menuTitle塞入到对应的menu中
        ForEach(this.listDate, (item: ContextMenuProps, index) => {
          if (item.systemIcon) {
            if (this.childActionsListDate.length >= 0) {
              MenuItem({
                content: item.title,
                symbolEndIcon: new SymbolGlyphModifier($r(item.systemIcon)).fontSize('24vp')
                  .fontColor([item.iconColor]),
                labelInfo: item.subtitle,
              })
                .alignSelf(ItemAlign.Start)
                .enabled(item.disabled)
                .contentFontColor(item.destructive)
                .onClick(() => {
                  //当item.inlineChildren默认为false,表示以二级目录展示
                  //true的时候,以同目录展示
                  this.isTouch = true && (!item.inlineChildren) && (item.d_childActions !== undefined);
                  this.isTouchWithNoSelect = this.isTouch ;
                  this.isTouchViewNumber = index;
                  this.twoMenu = this.childActionsListDate[this.isTouchViewNumber]
                  console.log('2024touch:' + this.isTouch + ',' + 'inlineChildren:' + item.inlineChildren + ',' +
                    'isTouchViewNumber:' + this.isTouchViewNumber);
                  //当item.inlineChildren默认为false，二级目录展示时候,点击一级目录，不关闭，在二级目录后关闭
                  //当item.inlineChildren为false的时候，就不需要关闭该窗口.
                  if (item.inlineChildren || (item.d_childActions == undefined)) {
                    this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                      itemTitle: item.title,
                      itemTag: this.menuTag,
                      itemIndex: index,
                      itemIndexPath: [0],
                      itemInlineChildren: item.inlineChildren
                    });
                  }     })//弹框,通过inlineChildren来判定是否有二级目录,ture有二级目录
                .bindContextMenu(this.isTouch, this.twoMenuView())

            } else {
              MenuItem({
                content: item.title,
                symbolEndIcon: new SymbolGlyphModifier($r(item.systemIcon)).fontSize('24vp')
                  .fontColor([item.iconColor]),
                labelInfo: item.subtitle,
              })
                .alignSelf(ItemAlign.Start)
                .enabled(item.disabled)
                .contentFontColor(item.destructive)
                .onClick(() => {
                  //当item.inlineChildren默认为false,表示以二级目录展示
                  //true的时候,以同目录展示
                  this.isTouch = true && (!item.inlineChildren) && (item.d_childActions !== undefined);
                  this.isTouchWithNoSelect = this.isTouch ;
                  this.isTouchViewNumber = index;
                  this.twoMenu = this.childActionsListDate[this.isTouchViewNumber]
                  console.log('2024touch:' + this.isTouch + ',' + 'inlineChildren:' + item.inlineChildren + ',' +
                    'isTouchViewNumber:' + this.isTouchViewNumber);
                  //当item.inlineChildren默认为false，二级目录展示时候,点击一级目录，不关闭，在二级目录后关闭
                  //当item.inlineChildren为false的时候，就不需要关闭该窗口.
                  if (item.inlineChildren || (item.d_childActions == undefined)) {
                    this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                      itemTitle: item.title,
                      itemTag: this.menuTag,
                      itemIndex: index,
                      itemIndexPath: [0],
                      itemInlineChildren: item.inlineChildren
                    });
                  }
                })//弹框,通过inlineChildren来判定是否有二级目录,ture有二级目录
                .bindContextMenu(this.isTouchWithNoSelect, this.twoMenuViewWithNoSelect())
            }
          } else {
            //如果没有systemIcon,则走自定义图标
            if (this.childActionsListDate.length >= 0) {
              MenuItem({
                content: item.title,
                endIcon: $r(item.icon),
                labelInfo: item.subtitle,
              })
                .alignSelf(ItemAlign.Start)
                .enabled(item.disabled)
                .contentFontColor(item.destructive)
                .onClick(() => {
                  //当item.inlineChildren默认为false,表示以二级目录展示
                  //true的时候,以同目录展示
                  this.isTouch = true && (!item.inlineChildren) && (item.d_childActions !== undefined);
                  this.isTouchViewNumber = index;
                  this.twoMenu = this.childActionsListDate[this.isTouchViewNumber]
                  console.log('2024touch:' + this.isTouch + ',' + 'inlineChildren:' + item.inlineChildren + ',' +
                    'isTouchViewNumber:' + this.isTouchViewNumber);
                  //当item.inlineChildren默认为false，二级目录展示时候,点击一级目录，不关闭，在二级目录后关闭
                  //当item.inlineChildren为false的时候，就不需要关闭该窗口.
                  if (item.inlineChildren || (item.d_childActions == undefined)) {
                    this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                      itemTitle: item.title,
                      itemTag: this.menuTag,
                      itemIndex: index,
                      itemIndexPath: [0],
                      itemInlineChildren: item.inlineChildren
                    });
                  }
                })//弹框,通过inlineChildren来判定是否有二级目录,ture有二级目录
                .bindContextMenu(this.isTouch, this.twoMenuView())
            } else {
              MenuItem({
                content: item.title,
                endIcon: $r(item.icon),
                labelInfo: item.subtitle,
              })
                .alignSelf(ItemAlign.Start)
                .enabled(item.disabled)
                .contentFontColor(item.destructive)
                .onClick(() => {
                  //当item.inlineChildren默认为false,表示以二级目录展示
                  //true的时候,以同目录展示
                  this.isTouch = true && (!item.inlineChildren) && (item.d_childActions !== undefined);
                  this.isTouchWithNoSelect = this.isTouch;
                  this.isTouchViewNumber = index;
                  this.twoMenu = this.childActionsListDate[this.isTouchViewNumber]
                  console.log('2024touch:' + this.isTouch + ',' + 'inlineChildren:' + item.inlineChildren + ',' +
                    'isTouchViewNumber:' + this.isTouchViewNumber);
                  //当item.inlineChildren默认为false，二级目录展示时候,点击一级目录，不关闭，在二级目录后关闭
                  //当item.inlineChildren为false的时候，就不需要关闭该窗口.
                  if (item.inlineChildren || (item.d_childActions == undefined)) {
                    this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                      itemTitle: item.title,
                      itemTag: this.menuTag,
                      itemIndex: index,
                      itemIndexPath: [0],
                      itemInlineChildren: item.inlineChildren
                    });
                  }
                })//弹框,通过inlineChildren来判定是否有二级目录,ture有二级目录
                .bindContextMenu(this.isTouchWithNoSelect, this.twoMenuViewWithNoSelect())
            }
          }
        })
      }.backgroundColor('white')
    }.onDisAppear(() => {
      this.menuCtx.rnInstance.postMessageToCpp('contextMenu::onCancel', {
        itemTag: this.menuTag,
      });
    })
  }
}
