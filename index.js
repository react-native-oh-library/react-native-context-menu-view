import React from "react";
import { requireNativeComponent, View, Platform, StyleSheet, processColor } from "react-native";
import NativeContextMenuHarmony from './src/RTNContextMenuNativeComponent'

const NativeContextMenu = requireNativeComponent("ContextMenu", null);

const ContextMenu = (props) => {
  const iconColor = props?.iconColor
    ? Platform.OS === 'ios'
      ? processColor(props.iconColor)
      : props.iconColor
    : undefined;

  if (Platform.OS === 'harmony') {
    return (
      <NativeContextMenuHarmony {...props} iconColor={iconColor}>
        {props.children}
        {props.preview != null && Platform.OS === 'ios' ? (
          <View style={styles.preview} nativeID="ContextMenuPreview">{props.preview}</View>
        ) : null}
      </NativeContextMenuHarmony>
    );
  } else {
    return (
      <NativeContextMenu {...props} iconColor={iconColor}>
        {props.children}
        {props.preview != null && Platform.OS === 'ios' ? (
          <View style={styles.preview} nativeID="ContextMenuPreview">{props.preview}</View>
        ) : null}
      </NativeContextMenu>
    );
  }

};

const styles = StyleSheet.create({
  preview: {
    position: 'absolute',
    overflow: 'visible',
    backgroundColor: 'transparent'
  }
});

export default ContextMenu;