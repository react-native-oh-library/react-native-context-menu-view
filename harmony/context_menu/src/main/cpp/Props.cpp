
/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GeneratePropsCpp.js
 */

#include "Props.h"
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>

namespace facebook {
namespace react {

RTNContextMenuProps::RTNContextMenuProps(const PropsParserContext &context, const RTNContextMenuProps &sourceProps,
                                         const RawProps &rawProps)
    : ViewProps(context, sourceProps, rawProps),

      title(convertRawProp(context, rawProps, "title", sourceProps.title, {})),
      actions(convertRawProp(context, rawProps, "actions", sourceProps.actions, {})),
      previewBackgroundColor(
          convertRawProp(context, rawProps, "previewBackgroundColor", sourceProps.previewBackgroundColor, {})),
      dropdownMenuMode(convertRawProp(context, rawProps, "dropdownMenuMode", sourceProps.dropdownMenuMode, {false})),
      disabled(convertRawProp(context, rawProps, "disabled", sourceProps.disabled, {false})),
      subtitle(convertRawProp(context, rawProps, "subtitle", sourceProps.subtitle, {})),
      systemIcon(convertRawProp(context, rawProps, "systemIcon", sourceProps.systemIcon, {})),
      icon(convertRawProp(context, rawProps, "icon", sourceProps.icon, {})),
      iconColor(convertRawProp(context, rawProps, "iconColor", sourceProps.iconColor, {})),
      destructive(convertRawProp(context, rawProps, "destructive", sourceProps.destructive, {false})),
      selected(convertRawProp(context, rawProps, "selected", sourceProps.selected, {false})),
      inlineChildren(convertRawProp(context, rawProps, "inlineChildren", sourceProps.inlineChildren, {false})) {}

} // namespace react
} // namespace facebook
