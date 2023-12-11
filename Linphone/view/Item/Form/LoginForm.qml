import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls as Control
import Linphone


ColumnLayout {
	id: mainItem
	spacing: 15 * DefaultStyle.dp
	signal connectionSucceed()

	TextInput {
		id: username
		label: "Username"
		mandatory: true
		textInputWidth: 250 * DefaultStyle.dp
	}
	TextInput {
		id: password
		label: "Password"
		mandatory: true
		hidden: true
		textInputWidth: 250 * DefaultStyle.dp
	}

	Text {
		id: errorText
		text: "Connection has failed. Please verify your credentials"
		color: DefaultStyle.danger_500main
		opacity: 0
		states: [
			State{
				name: "Visible"
				PropertyChanges{target: errorText; opacity: 1.0}
			},
			State{
				name:"Invisible"
				PropertyChanges{target: errorText; opacity: 0.0}
			}
		]
		transitions: [
			Transition {
				from: "Visible"
				to: "Invisible"
				NumberAnimation {
					property: "opacity"
					duration: 1000
				}
			}
		]
		Timer {
			id: autoHideErrorMessage
			interval: 2500
			onTriggered: errorText.state = "Invisible"
		}
		Connections {
			target: LoginPageCpp
			onRegistrationStateChanged: {
				if (LoginPageCpp.registrationState === LinphoneEnums.RegistrationState.Failed) {
					errorText.state = "Visible"
					autoHideErrorMessage.restart()
				} else if (LoginPageCpp.registrationState === LinphoneEnums.RegistrationState.Ok) {
					mainItem.connectionSucceed()
				}
			}
		}
	}

	RowLayout {
		id: lastFormLineLayout
		Button {
			text: "Log in"
			Layout.rightMargin: 20 * DefaultStyle.dp
			onClicked: {
				username.errorMessage = ""
				password.errorMessage = ""

				if (username.text.length == 0 || password.text.length == 0) {
					if (username.text.length == 0)
						username.errorMessage = qsTr("You must enter a username")
					if (password.text.length == 0)
						password.errorMessage = qsTr("You must enter a password")
					return
				}
				LoginPageCpp.login(username.text, password.text)
			}
		}
		Button {
			background: Item {
				visible: false
			}
			contentItem: Text {
				color: DefaultStyle.main2_500main
				text: "Forgotten password?"
				font{
					underline: true
					pixelSize: 13 * DefaultStyle.dp
					weight : 600 * DefaultStyle.dp
				}
			}
			onClicked: console.debug("[LoginForm]User: forgotten password button clicked")
		}
	
	}
}
