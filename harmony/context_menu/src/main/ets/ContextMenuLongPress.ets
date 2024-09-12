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
export struct ContextMenuLongPress {
  public static readonly NAME = "ContextMenuLongPress"
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
  @State isTouchViewNumber: number = 0;
  @State isChildActionsTouchViewNumber: number = 0;
  @State menuChildActions: ContextMenuProps[] = [];
  @State menuChild: ESObject[] = [];
  @State deepChildActions: ESObject[] = [];

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
        }
        //这里判断是否有三级以上的目录
        //     for (let m = 0; m < this.childActionsListDate[j].length; m++) {
        //       this.menuIndexPath[m] = m;
        //       if ((this.childActionsListDate[j])[m].childActions !== null &&
        //         (this.childActionsListDate[j])[m].childActions !== undefined &&
        //         (this.childActionsListDate[j])[m].childActions !== '') {
        //         let test: ESObject = this.ParsingChildActions((this.childActionsListDate[j])[m].childActions, j, m);
        //       }
        //     }
      }
    }
  }

  // ParsingChildActions(childActions: ESObject, index: number, childActionsIndex: number,
  //   level: number = 0) {
  //   let child: ESObject[] = [];
  //   Logger.info(`20240722--------------------->ContextMenu.ParsingChildActions. 这行数据是第` +
  //     index + '个数组的，第' + childActionsIndex + '个子元素');
  //   Logger.info(`20240722--------------------->ContextMenu.ParsingChildActions=` +
  //   JSON.stringify(childActions));
  //   Logger.info(`20240722--------------------->ContextMenu.ParsingChildActions.length=` +
  //   JSON.stringify(childActions.length));
  //   for (let i = 0; i < childActions.length; i++) {
  //     child[i] = childActions[i];
  //     Logger.info(`20240722--------------------->ContextMenu.ParsingChildActions longPress.ParsingChildActions=` + i +
  //       ',数据位:' +
  //     JSON.stringify(child[i]));
  //     Logger.info(`20240722--------------------->ContextMenu.ParsingChildActions longPress.ParsingChildActions.child[i].childActions=` +
  //     JSON.stringify(child[i].childActions));
  //     if (child[i].childActions !== undefined && child[i].childActions.length > 0) {
  //       Logger.info('20240722--------------------->ContextMenu.ParsingChildActions longPress.ParsingChildActions.111111111111111');
  //       for (this.menuChild of child[i].childActions) {
  //         Logger.info('20240722--------------------->ContextMenu.ParsingChildActions longPress.ParsingChildActions.111111111111111' +
  //         JSON.stringify(this.menuChild));
  //         this.ParsingChildActions(this.menuChild, index + 1, childActionsIndex + 1, level + 1);
  //       }
  //     }
  //   }
  // }

  //退出组件
  aboutToDisappear() {
  }

  @Builder
  SubMenu() {
    Menu() {
      ForEach(this.childActionsListDate[this.isTouchViewNumber], (childItem: ContextMenuProps, index1) => {
        MenuItem({
          content: childItem.title,
          symbolEndIcon: new SymbolGlyphModifier($r(childItem.systemIcon)).fontSize('24vp')
            .fontColor([childItem.iconColor]),
          labelInfo: childItem.subtitle,
        })//selected复选标记,根据传入的selected来判断selected为默认为false,不显示
          .selected(childItem.selected)
          .selectIcon(new SymbolGlyphModifier($r('sys.symbol.checkmark')).fontSize('24vp'))
          .alignSelf(ItemAlign.Start)
          .enabled(childItem.disabled)
          .contentFontColor(childItem.destructive)
          .onClick(() => {
            if (childItem.childActions !== undefined) {
              this.isChildActionsTouchViewNumber = index1;
              this.deepChildActions =
                this.childActionsListDate[this.isTouchViewNumber][this.isChildActionsTouchViewNumber].childActions;
            }
            if (childItem.inlineChildren || (childItem.childActions == undefined)) {
              this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
                itemTitle: childItem.title,
                itemTag: this.menuTag,
                itemIndex: index1,
                itemIndexPath: this.menuIndexPath,
                itemInlineChildren: childItem.inlineChildren
              });
            }
          }
          )
      })
    }
  }

  // @Builder
  // childMenu() {
  //   Menu() {
  //     ForEach(this.deepChildActions, (childItem: ContextMenuProps, index2) => {
  //       MenuItem({
  //         content: childItem.title,
  //         // symbolEndIcon: new SymbolGlyphModifier($r(childItem.systemIcon)).fontSize('24vp')
  //         //   .fontColor([childItem.iconColor]),
  //         // labelInfo: childItem.subtitle,
  //         builder: (): void => this.childMenu()
  //       })//selected复选标记,根据传入的selected来判断selected为默认为false,不显示
  //         // .selected(childItem.selected)
  //         // .selectIcon(new SymbolGlyphModifier($r('sys.symbol.checkmark')).fontSize('24vp'))
  //         // .alignSelf(ItemAlign.Start)
  //         // .enabled(childItem.disabled)
  //         // .contentFontColor(childItem.destructive)
  //         .onClick(() => {
  //           Logger.info(`20240722SubMenu--------------------->ContextMenu.aboutToAppear3级目录.` +
  //           JSON.stringify(this.deepChildActions));
  //           Logger.info(`20240722SubMenu--------------------->ContextMenu.aboutToAppear3级目录.index2:` +
  //             index2);
  //           // this.deepChildActions = this.deepChildActions[this.isTouchViewNumber]
  //           Logger.info(`20240722SubMenu--------------------->ContextMenu.aboutToAppear3级目录.` +
  //           JSON.stringify(this.deepChildActions[index2].childActions));
  //           this.deepChildActions = this.deepChildActions[index2].childActions;
  //           // this.childActionsListDate = this.childActionsListDate[this.isTouchViewNumber][this.isChildActionsTouchViewNumber].childActions;
  //           // Logger.info(`20240722SubMenu--------------------->ContextMenu.aboutToAppear3级目录.` +
  //           // JSON.stringify(this.deepChildActions));
  //           // Logger.info(`20240722SubMenu--------------------->ContextMenu.aboutToAppear.index2:` + index2);
  //           // this.isTouch = true && (!childItem.childActions);
  //           if (childItem.inlineChildren || (childItem.childActions == undefined)) {
  //             this.menuCtx.rnInstance.postMessageToCpp('contextMenu::SET_NATIVE_RESPONDERS_BLOCK', {
  //               itemTitle: childItem.title,
  //               itemTag: this.menuTag,
  //               itemIndex: index2,
  //               itemIndexPath: this.menuIndexPath,
  //               itemInlineChildren: childItem.inlineChildren
  //             });
  //           }
  //         })
  //     })
  //   }
  // }

  build() {
    Menu() {
      MenuItemGroup({ header: this.menuTitle }) {
        //遍历数据，把每个menuTitle塞入到对应的menu中
        ForEach(this.listDate, (item: ContextMenuProps, index) => {
          MenuItem({
            content: item.title,
            symbolEndIcon: new SymbolGlyphModifier($r(item.systemIcon)).fontSize('24vp').fontColor([item.iconColor]),
            labelInfo: item.subtitle,
          })//selected复选标记,根据传入的selected来判断selected为默认为false,不显示
            .selected(item.selected)
            .selectIcon(new SymbolGlyphModifier($r('sys.symbol.checkmark')).fontSize('24vp'))
            .alignSelf(ItemAlign.Start)
            .enabled(item.disabled)
            .contentFontColor(item.destructive)
            .onClick(() => {
              //当item.inlineChildren默认为false,表示以二级目录展示
              //true的时候,以同目录展示
              this.isTouch = true && (!item.inlineChildren) && (item.d_childActions !== undefined);
              this.isTouchViewNumber = index;
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
            .bindContextMenu(this.isTouch, this.SubMenu())
        })
      }.backgroundColor('white')
    }.onDisAppear(() => {
      this.menuCtx.rnInstance.postMessageToCpp('contextMenu::onCancel', {
        itemTag: this.menuTag,
      });
    })
  }
}
