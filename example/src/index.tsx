/*
 * Copyright (c) 2025 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

import ContextMenu from "react-native-context-menu-view";
import { View, StyleSheet } from "react-native";

const Example = () => {
  return (
    <View style={styles.container}>
      <ContextMenu
        actions={[{ title: "Title 1" }, { title: "Title 2" }]}
        onPress={(e) => {
          console.warn(`Pressed ${e.nativeEvent.name} at index ${e.nativeEvent.index}`);
        }}
      >
        <View style={styles.yourOwnStyles}>
        </View>
      </ContextMenu>
    </View>
  );
};

export default Example;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  yourOwnStyles: {
    width: 200,
    height: 100,
    backgroundColor: 'grey',
    justifyContent: 'center',
    alignItems: 'center',
  },
});