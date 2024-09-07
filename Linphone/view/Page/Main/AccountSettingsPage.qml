import QtQuick
import QtQuick.Effects
import QtQuick.Layouts
import QtQuick.Controls as Control
import Linphone
import UtilsCpp 1.0
import SettingsCpp 1.0
import LinphoneAccountsCpp

AbstractMasterDetailPage {
	layoutsPath: "qrc:/Linphone/view/App/Layout/Account"
	titleText: qsTr("Mon compte")
	property AccountProxy  accounts: AccountProxy {id: accountProxy}
	property AccountGui account: accountProxy.defaultAccount
	signal accountRemoved()
	families: [
		{title: qsTr("Général"), layout: "AccountSettingsGeneralLayout", model: account},
		{title: qsTr("Paramètres de compte"), layout: "AccountSettingsParametersLayout", model: account}
	]
	Connections {
		target: account.core
		onRemoved: accountRemoved()
	}
}
