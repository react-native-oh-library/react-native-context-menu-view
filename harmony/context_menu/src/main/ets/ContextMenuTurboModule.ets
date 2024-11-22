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

import { TurboModule, TurboModuleContext } from "@rnoh/react-native-openharmony/ts";
import { ComponentContent, window } from '@kit.ArkUI';
import { ContextMenuLongPress } from './ContextMenuLongPress';
import { RNOHContext } from '@rnoh/react-native-openharmony';
import { ContextMenuLongPressWithNoSelect } from './ContextMenuLongPressWithNoSelect';

@Builder
//offset的单位是vp，传递的x，y值也对应的是组件里面的xy值,单点的特效
function buildContextMenu(realViewPoint: ESObject) {
  ContextMenuLongPress({ realViewPoint: realViewPoint })
}

@Builder
//不带Select属性或者Select属性为false
function buildContextMenuWithNoSelect(realViewPoint: ESObject) {
  ContextMenuLongPressWithNoSelect({ realViewPoint: realViewPoint })
}


export interface ContextMenuProps {
  tag?: number;
  width?: number;
  height?: number;
  x?: number;
  y?: number;
  windowX?: number;
  windowY?: number;
  title?: string;
  subtitle?: string;
  action?: string;
  dropdownMenuMode?: boolean;
  disabled?: boolean;
  previewBackgroundColor?: string;
  actionsTitle?: string;
  touchViewActions?: ContextMenuProps;
  ctx?: RNOHContext;
  romWidth?: number;
  romHeight?: number;
  systemIcon?: string;
  selected?: boolean;
  destructive?: string
  d_childActions?: ContextMenuProps;
  inlineChildren?: boolean;
  iconColor?: string;
  childActions?: ContextMenuProps;
  childActionsTag?: number;
}

export class ContextMenuTurboModule extends TurboModule {
  //基本判定属性
  timestamp: number = 0;
  result: number = 0;
  longResult: number = 0;
  isCancel: boolean = false;
  realViewPoint: ContextMenuProps = {};
  touchViewActions: ContextMenuProps = {};
  downTimestamp: number = 0;
  upTimestamp: number = 0;
  header: string = '';
  longPressLock: boolean = false;
  longPressTimestamp: number = 0;
  //contextMenu所携带的属性
  title: string = '';
  actions: string = '';
  dropdownMenuMode: boolean = false;
  previewBackgroundColor: string = '';
  actionsTitle: string = '';
  //弹窗偏移量需要的值
  width: number = 0;
  height: number = 0;
  romWidth: number = 0;
  romHeight: number = 0;
  windowX: number = 0;
  windowY: number = 0;
  customDialogOpen: boolean = false;
  selectedFlag: number = 0;
  menuHeight: number = 0;

  constructor(ctx: TurboModuleContext) {
    super(ctx)
    if (this.ctx.rnInstance.getArchitecture() === "C_API") {
      //e为C层拦截的ArkUI_UIInputEvent
      this.ctx.rnInstance.cppEventEmitter.subscribe("RNGH::TOUCH_EVENT", (e: ESObject) => {
        //获取view坐标
        console.log('20240722---我在ArkTs侧获取到了点击事件！TOUCH_EVENT，touchableViews：' +
        JSON.stringify(e.touchableViews) + '第三个节点为View:' + JSON.stringify(e.touchableViews[2]));
        //e.action进行获取点击事件的次数，进行判定
        switch (e.action) {
          case 1:
            //down手势判断,这里通过,手指按下的时间戳进行判断
            this.downTimestamp = e.timestamp / Math.pow(10, 6);
            //获取设备的rom
            this.realViewPoint.romWidth = e.touchableViews[0].width;
            this.romWidth = e.touchableViews[0].width;
            this.realViewPoint.romHeight = e.touchableViews[0].height;
            this.romHeight = e.touchableViews[0].height;
            break;
          case 2:
            //isCancel = true; move x1-x > 150 移动判断  y1-y >200
            //在按压的时候，此时会一直接口case2的消息，如果此在点击区域，且e.dropdownMenuMode为false按压事件超过500ms，则需要触发长按事件
            this.longPressTimestamp = e.timestamp / Math.pow(10, 6);
            this.longResult = this.longPressTimestamp - this.downTimestamp
            if (e.touchableViews[2] && !(e.dropdownMenuMode) && this.longResult >= 500) {
              this.realViewPoint.tag = e.touchableViews[1].tag;
              this.realViewPoint.x = e.touchableViews[2].x;
              this.realViewPoint.y = e.touchableViews[2].y;
              this.realViewPoint.width = e.touchableViews[2].width;
              this.width = e.touchableViews[2].width;
              this.realViewPoint.height = e.touchableViews[2].height;
              this.height = e.touchableViews[2].height;
              this.realViewPoint.title = e.title;
              //获取点击的时候的触摸事件的坐标
              this.realViewPoint.windowX = e.touchPoints[0].windowX;
              this.windowX = e.touchPoints[0].windowX;
              this.realViewPoint.windowY = e.touchPoints[0].windowY;
              this.windowY = e.touchPoints[0].windowY;
              this.realViewPoint.touchViewActions = e.touchViewActions;
              let selectedActions: ESObject = e.touchViewActions;
              if (!this.longPressLock) {
                this.realViewPoint.ctx = this.ctx;
                for (let i = 0; i < selectedActions.length; i++) {
                  if (selectedActions[i].selected) {
                    //复选框，默认为false
                    this.selectedFlag++;
                  }
                  this.menuHeight++;
                }
                if (this.menuHeight >= 5) {
                  this.menuHeight = this.menuHeight - 5;
                } else {
                  this.menuHeight = 0;
                }
                if (this.selectedFlag > 0) {
                  //传入对应的ctx
                  this.realViewPoint.ctx = this.ctx;
                  this.onTouch(this.realViewPoint);
                  this.selectedFlag = 0;
                } else {
                  //传入对应的ctx
                  this.realViewPoint.ctx = this.ctx;
                  //当获取到了view视图,并且dropdownMenuMode为true的时候,进入单点效果.
                  this.onTouchWithNoSelect(this.realViewPoint);
                  this.selectedFlag = 0;
                }
                this.longPressLock = true;
              }
            }
            break;
          case 3:
            //up 手势判断，走入up才算一次点击;up手势判断,这里通过,手指按下的时间戳进行判断
            this.upTimestamp = e.timestamp / Math.pow(10, 6);
            //计算result,如果>=500则为长按,如果<500为单击
            this.result = this.upTimestamp - this.downTimestamp
            //e.touchableViews：当点击view区域时,到获取的view固定有三个,主屏幕,contextmenu容器,view.需要获取view的坐标,view固定为数组中第三个
            //当点击外部的stack区域时候,获取的view只有两个，主屏幕和contextmenu容器,逻辑上是点击view才会触发menu
            // dropdownMenuMode-->true---->onTouch(),如果此时点击的事件大于500ms则不触发点击事件，如果小于500ms则触发点击效果
            if (e.touchableViews[2] && e.dropdownMenuMode && this.result < 500) {
              this.realViewPoint.tag = e.touchableViews[1].tag;
              this.realViewPoint.x = e.touchableViews[2].x;
              this.realViewPoint.y = e.touchableViews[2].y;
              this.realViewPoint.width = e.touchableViews[2].width;
              this.width = e.touchableViews[2].width;
              this.realViewPoint.height = e.touchableViews[2].height;
              this.height = e.touchableViews[2].height;
              this.realViewPoint.title = e.title;
              //点击事件，触摸坐标
              this.realViewPoint.windowX = e.touchPoints[0].windowX;
              this.windowX = e.touchPoints[0].windowX;
              this.realViewPoint.windowY = e.touchPoints[0].windowY;
              this.windowY = e.touchPoints[0].windowY;
              //此处获取actions值
              this.realViewPoint.touchViewActions = e.touchViewActions;
              //当获取到了view视图,并且dropdownMenuMode为true的时候,进入单点效果.
              let selectedActions: ESObject = e.touchViewActions;
              for (let i = 0; i < selectedActions.length; i++) {
                if (selectedActions[i].selected) {
                  //复选框，默认为false
                  this.selectedFlag++;
                }
                this.menuHeight++;
              }
              if (this.menuHeight >= 5) {
                this.menuHeight = this.menuHeight - 5;
              } else {
                this.menuHeight = 0;
              }
              if (this.selectedFlag > 0) {
                //传入对应的ctx
                this.realViewPoint.ctx = this.ctx;
                this.onTouch(this.realViewPoint);
                this.selectedFlag = 0;
              } else {
                //传入对应的ctx
                this.realViewPoint.ctx = this.ctx;
                //当获取到了view视图,并且dropdownMenuMode为true的时候,进入单点效果.
                this.onTouchWithNoSelect(this.realViewPoint);
                this.selectedFlag = 0;
              }
              //e.dropdownMenuMode(默认)--->false,那么默认为长按，如果此时单次点击时间大于500ms则可以触发长按事件，小于500ms则不触发点击事件
            } else if (e.touchableViews[2] && !(e.dropdownMenuMode) && this.result >= 500) {
              // 因为在case中，已经判定是否为长按，case2中的逻辑，当点击在view视图，且dropdownMenuMode为false，并且，满足按压时间大于500ms，就会走长按
              //并且将长按锁置于true，在手指抬起的时候，会触发case3事件，那么在该处进行判断,不用再触发longPress事件，只用将长按锁置于初始化即可
              this.longPressLock = false;
            }
            break;
          default:
            break;
        }
      })
    }
  }

  //进行touch单击事件的调用
  //长按和单击均属于点击事件,无非就是时间长短,最后均触发该事件
  onTouch(realViewPoint: Object) {
    window.getLastWindow(this.ctx.uiAbilityContext)
      .then((value) => {
        const uiContext = value.getUIContext()
        const promptAction = uiContext?.getPromptAction();
        const contentNode = new ComponentContent(uiContext!, wrapBuilder(buildContextMenu), realViewPoint);
        let offDx = -this.romWidth / 2 + this.windowX + this.width / 2;
        let OffDy = -this.romHeight / 2 + this.windowY + this.height * 0.4 + 48 * this.menuHeight;
        try {
          promptAction?.openCustomDialog(contentNode, {
            offset: {
              dx: offDx,
              dy: OffDy,
            }
          })
          this.menuHeight = 0;
          this.ctx.rnInstance.cppEventEmitter.subscribe("RNGH::CLOSE_MENU", () => {
            promptAction?.closeCustomDialog(contentNode);
            offDx = 0;
            OffDy = 0;
          })
        } catch (error) {
          console.error(`eroor`);
        }
      })
  }

  onTouchWithNoSelect(realViewPoint: Object) {
    window.getLastWindow(this.ctx.uiAbilityContext)
      .then((value) => {
        const uiContext = value.getUIContext()
        const promptAction = uiContext?.getPromptAction();
        const contentNode = new ComponentContent(uiContext!, wrapBuilder(buildContextMenuWithNoSelect), realViewPoint);
        let offDx = -this.romWidth / 2 + this.windowX + this.width / 2;
        let OffDy = -this.romHeight / 2 + this.windowY + this.height * 0.4 + 48 * this.menuHeight;
        try {
          promptAction?.openCustomDialog(contentNode, {
            offset: {
              dx: offDx,
              dy: OffDy,
            }
          })
          this.menuHeight = 0;
          this.ctx.rnInstance.cppEventEmitter.subscribe("RNGH::CLOSE_MENU", () => {
            promptAction?.closeCustomDialog(contentNode);
            offDx = 0;
            OffDy = 0;
          })
        } catch (error) {
          console.error(`eroor`);
        }
      })
  }
}