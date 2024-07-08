import QtQuick 2.7
import QtQuick.Controls 2.2 as Control
import QtQuick.Layouts as Layout
import QtQuick.Effects
import Linphone
  
Control.Popup {
	id: mainItem
	signal buttonPressed(string text)
	signal launchCall()
	signal wipe()
	property bool closeButtonVisible: true
	property bool roundedBottom: false
	closePolicy: Control.Popup.CloseOnEscape
	leftPadding: 72 * DefaultStyle.dp
	rightPadding: 72 * DefaultStyle.dp
	topPadding: 41 * DefaultStyle.dp
	bottomPadding: 18 * DefaultStyle.dp
	background: Item {
		anchors.fill: parent
		Rectangle {
			id: numPadBackground
			width: parent.width
			height: parent.height
			color: DefaultStyle.grey_100
			radius: 20 * DefaultStyle.dp
		}
		MultiEffect {
			id: effect
			anchors.fill: numPadBackground
			source: numPadBackground
			shadowEnabled: true
			shadowColor: DefaultStyle.grey_1000
			shadowOpacity: 0.1
			shadowBlur: 1
			z: -1
		}
		Rectangle {
			width: parent.width
			height: parent.height / 2
			anchors.bottom: parent.bottom
			color: DefaultStyle.grey_100
			visible: !mainItem.roundedBottom
		}
		Button {
			id: closeButton
			visible: mainItem.closeButtonVisible
			anchors.top: parent.top
			anchors.right: parent.right
			anchors.topMargin: 10 * DefaultStyle.dp
			anchors.rightMargin: 10 * DefaultStyle.dp
			background: Item {
				anchors.fill: parent
				visible: false
			}
			icon.source: AppIcons.closeX
			width: 24 * DefaultStyle.dp
			height: 24 * DefaultStyle.dp
			icon.width: 24 * DefaultStyle.dp
			icon.height: 24 * DefaultStyle.dp
			onClicked: mainItem.close()
		}
	}
	contentItem: Layout.GridLayout {
		columns: 3
		columnSpacing: 40 * DefaultStyle.dp
		rowSpacing: 10 * DefaultStyle.dp
		Repeater {
			model: 9
			Button {
				id: numPadButton
				Layout.Layout.alignment: Qt.AlignHCenter
				required property int index
				implicitWidth: 60 * DefaultStyle.dp
				implicitHeight: 60 * DefaultStyle.dp
				background: Rectangle {
					anchors.fill: parent
					color: numPadButton.down ? DefaultStyle.numericPadPressedButtonColor : DefaultStyle.grey_0
					radius: 71 * DefaultStyle.dp
				}
				contentItem: Text {
					id: innerText
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					anchors.centerIn: parent
					text: index + 1
					font {
						pixelSize: 32 * DefaultStyle.dp
						weight: 400 * DefaultStyle.dp
					}
				}
				onClicked: {
					mainItem.buttonPressed(innerText.text)
				}
			}
		}
		Repeater {
			model: [
				{pressText: "*"},
				{pressText: "0", longPressText: "+"},
				{pressText: "#"}
			]
			Button {
				id: digitButton
				Layout.Layout.alignment: Qt.AlignHCenter
				shadowEnabled: true
				implicitWidth: 60 * DefaultStyle.dp
				implicitHeight: 60 * DefaultStyle.dp

				background: Rectangle {
					anchors.fill: parent
					color: digitButton.down ? DefaultStyle.numericPadPressedButtonColor : DefaultStyle.grey_0
					radius: 71 * DefaultStyle.dp
				}
				contentItem: Item {
					anchors.fill: parent
					Text {
						id: pressText
						height: contentHeight
						anchors.left: parent.left
						anchors.right: parent.right
						horizontalAlignment: Text.AlignHCenter
						Component.onCompleted: {if (modelData.longPressText === undefined) anchors.centerIn= parent}
						text: modelData.pressText
						font.pixelSize: 32 * DefaultStyle.dp
					}
					Text {
						id: longPressText
						height: contentHeight
						anchors.left: parent.left
						anchors.right: parent.right
						y: digitButton.height/2
						horizontalAlignment: Text.AlignHCenter
						visible: modelData.longPressText ? modelData.longPressText.length > 0 : false
						text: modelData.longPressText ? modelData.longPressText : ""
						font.pixelSize: 22 * DefaultStyle.dp
					}
				}
				onClicked: mainItem.buttonPressed(pressText.text)
				onPressAndHold: mainItem.buttonPressed(longPressText.text)
			}
		}
		Item {
			// Invisible item to move the last two buttons to the right
		}
		Button {
			id: launchCallButton
			implicitWidth: 75 * DefaultStyle.dp
			implicitHeight: 55 * DefaultStyle.dp
			Layout.Layout.alignment: Qt.AlignHCenter
			background: Rectangle {
				anchors.fill: parent
				color: DefaultStyle.success_500main
				radius: 71 * DefaultStyle.dp
			}
			icon.source: AppIcons.phone
			icon.width: 32 * DefaultStyle.dp
			icon.height: 32 * DefaultStyle.dp
			contentImageColor: DefaultStyle.grey_0
			onClicked: mainItem.launchCall()
		}
		Button {
			leftPadding: 5 * DefaultStyle.dp
			rightPadding: 5 * DefaultStyle.dp
			topPadding: 5 * DefaultStyle.dp
			bottomPadding: 5 * DefaultStyle.dp
			Layout.Layout.alignment: Qt.AlignHCenter
			background: Item {
				visible: false
			}
			icon.source: AppIcons.backspaceFill
			icon.width: 38 * DefaultStyle.dp
			icon.height: 38 * DefaultStyle.dp
			onClicked: mainItem.wipe()
		}
	}
}