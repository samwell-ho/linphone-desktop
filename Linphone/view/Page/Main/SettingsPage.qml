import QtQuick
import QtQuick.Effects
import QtQuick.Layouts
import QtQuick.Controls as Control
import SettingsCpp

AbstractMasterDetailPage {
	layoutsPath: "qrc:/Linphone/view/App/Layout/Settings"
	titleText: qsTr("Paramètres")
	families: [
		{title: qsTr("Appels"), layout: "CallSettingsLayout"},
		//{title: qsTr("Sécurité"), layout: "SecuritySettingsLayout"},
		{title: qsTr("Conversations"), layout: "ChatSettingsLayout", visible: !SettingsCpp.disableChatFeature},
		{title: qsTr("Contacts"), layout: "ContactsSettingsLayout"},
		{title: qsTr("Réunions"), layout: "MeetingsSettingsLayout", visible: !SettingsCpp.disableMeetingsFeature},
		{title: qsTr("Affichage"), layout: "DisplaySettingsLayout"},
		{title: qsTr("Réseau"), layout: "NetworkSettingsLayout"},
		{title: qsTr("Paramètres avancés"), layout: "AdvancedSettingsLayout"}
	]
}
