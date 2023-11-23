pragma Singleton
import QtQuick 2.15

QtObject {

	property string emojiFont: "Noto Color Emoji"
	property color orangeColor: "#FE5E00"
	property color buttonBackground: "#FE5E00"
	property color buttonPressedBackground: "#c74b02"
	property color buttonInversedBackground: "white"
	property color buttonPressedInversedBackground: "#fff1e8"
	property color buttonTextColor: "white"
	property color radioButtonCheckedColor: "#4AA8FF"
	property color radioButtonUncheckedColor: "#FE5E00"
	property color radioButtonTitleColor: "#070707"
	property int radioButtonTextSize: 8
	property int radioButtonTitleSize: 9
	property color buttonInversedTextColor: "#FE5E00"
	property color checkboxBorderColor: "#FE5E00"
	property double checkboxBorderWidth: 2
	property int buttonTextSize: 10
	property color carouselLightGrayColor: "#DFECF2"
	property color formItemLabelColor: "#4E6074"
	property int formItemLabelSize: 8
	property color formItemDisableColor: "#949494"
	property color formItemDisableBackgroundColor: "#EDEDED"
	property color formItemBackgroundColor: "#F9F9F9"
	property color formItemBorderColor: "#EDEDED"
	property color formItemFocusBorderColor: "#FE5E00"

	property int tabButtonTextSize: 11
	property color verticalTabBarColor: "#FE5E00"
	property color verticalTabBarTextColor: "white"
	property int verticalTabButtonTextSize: 6

	property color comboBoxHighlightColor: "#C0D1D9"
	property color comboBoxHoverColor: "#6C7A87"

	property color aboutButtonTextColor: "#6C7A87"

	property color questionTextColor: "#364860"
	property color errorMessageColor: "#DD5F5F"

	property color tooltipBackgroundColor: "#DFECF2"

	property color digitInputFocusedColor: "#FE5E00"
	property color digitInputColor: "#6C7A87"

	property color darkBlueColor: "#22334D"
	property color darkGrayColor: "#4E6074"
	property color grayColor: "#6C7A87"
	property color lightGrayColor: "#EDEDED"

	property color defaultTextColor: "#4E6074"
	property color disableTextColor: "#9AABB5"
	property int defaultTextSize: 7
	property string defaultFont: "Noto Sans"
	property int defaultFontPointSize: 10
	property int title1FontPointSize: 50
	property int title2FontPointSize: 20
	property color titleColor: "#22334D"

	property color mainPageRightPanelBackgroundColor: "#F9F9F9"
	property color mainPageTitleColor: "#364860"
	property int mainPageTitleSize: 15

	property color searchBarFocusBorderColor: "#6C7A87"

	property color numericPadBackgroundColor: "#F9F9F9"
	property color numericPadShadowColor: Qt.rgba(0.0, 0.0, 0.0, 0.1)
	property color numericPadButtonColor: "#FFFFFF"
	property int numericPadButtonTextSize: 15
	property int numericPadButtonSubtextSize: 6
	property color numericPadPressedButtonColor: "#EEF7F8"
	property color numericPadLaunchCallButtonColor: "#4FAE80"

	property color groupCallButtonColor: "#EEF7F8"

	property color splitViewHandleColor: "#F9F9F9"
	property color splitViewHoveredHandleColor: "#EDEDED"
}