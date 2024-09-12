import { ViewProps, ProcessedColorValue, HostComponent } from "react-native";
import {DirectEventHandler,DefaultTypes, Int32, WithDefault} from 'react-native/Libraries/Types/CodegenTypes';
import codegenNativeComponent from 'react-native/Libraries/Utilities/codegenNativeComponent';
import codegenNativeCommands from 'react-native/Libraries/Utilities/codegenNativeCommands';

export interface ContextMenuAction {
    title: string;
    subtitle?: string;
    systemIcon?: string;
    icon?: string;
    iconColor?: string;
    destructive?: boolean;
    selected?: boolean;
    disabled?: boolean;
    inlineChildren?: boolean;
    actions?: ContextMenuAction;
  }
  
  export interface ContextMenuOnPressNativeEvent {
    index: Int32;
    indexPath: Int32[];
    name: string;
  }
  
  export interface ContextMenuProps extends ViewProps {
    title?: string;
    actions?: ContextMenuAction;
    onPress?: DirectEventHandler<ContextMenuOnPressNativeEvent>
    onPreviewPress?: DirectEventHandler<Readonly<{}>>
    onCancel?: DirectEventHandler<Readonly<{}>>
    previewBackgroundColor?: string;
    // preview?: React.ReactNode;
    dropdownMenuMode?: boolean;
    disabled?: boolean;
    // children?: React.ReactNode;
    subtitle?: string;
    systemIcon?: string;
    icon?: string;
    iconColor?: string;
    destructive?: boolean;
    selected?: boolean;
    inlineChildren?: boolean;
    preview?: React.ReactNode;
  }

export type ContextMenuViewComponentType = HostComponent<ContextMenuProps>

console.log('2024-7-27,components')

export default codegenNativeComponent<ContextMenuProps>('RTNContextMenu') as ContextMenuViewComponentType;
