import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as Control
import QtQuick.Dialogs
import Linphone
import SettingsCpp 1.0
import UtilsCpp

AbstractDetailsLayout {
	id: mainItem
	contentComponent: content
	topbarOptionalComponent: topBar
	property alias ldapGui: mainItem.model
	property bool isNew: false
	Component {
		id: topBar
		RowLayout {
			spacing: 20 * DefaultStyle.dp
			Button {
				background: Item{}
				icon.source: AppIcons.trashCan
				icon.width: 32 * DefaultStyle.dp
				icon.height: 32 * DefaultStyle.dp
				contentImageColor: DefaultStyle.main2_600
				visible: !isNew
				onClicked: {
					var mainWin = UtilsCpp.getMainWindow()
					mainWin.showConfirmationLambdaPopup(
						qsTr("Supprimer l'annuaire LDAP ?"),
						"",
						function (confirmed) {
							if (confirmed) {
								ldapGui.core.remove()
								mainItem.container.pop()
							}
						}
					)
				}
			}
			Button {
				text: qsTr("Enregistrer")
				onClicked: {
					if (ldapGui.core.isValid()) {
						ldapGui.core.save()
						UtilsCpp.showInformationPopup(qsTr("Succès"), qsTr("L'annuaire LDAP a été sauvegardé"), true, mainWindow)
					} else {
						UtilsCpp.showInformationPopup(qsTr("Erreur"), qsTr("Une erreur s'est produite, la configuration LDAP n'a pas été sauvegardée !"), false, mainWindow)
					}
				}
			}
		}
	}
	
	Component {
		id: content
		ColumnLayout {
			width: parent.width
			spacing: 5 * DefaultStyle.dp
			RowLayout {
				Layout.topMargin: 16 * DefaultStyle.dp
				spacing: 5 * DefaultStyle.dp
				ColumnLayout {
					Layout.fillWidth: true
					spacing: 5 * DefaultStyle.dp
					ColumnLayout {
						Layout.preferredWidth: 341 * DefaultStyle.dp
						Layout.maximumWidth: 341 * DefaultStyle.dp
						Layout.minimumWidth: 341 * DefaultStyle.dp
						spacing: 5 * DefaultStyle.dp
						Text {
							Layout.fillWidth: true
							text: qsTr("Annuaires LDAP")
							font: Typography.h4
							wrapMode: Text.WordWrap
							color: DefaultStyle.main2_600
						}
						Text {
							text: qsTr("Ajouter vos annuaires LDAP pour pouvoir effectuer des recherches dans la magic search bar.")
							font: Typography.p1s
							wrapMode: Text.WordWrap
							color: DefaultStyle.main2_600
							Layout.fillWidth: true
						}
					}
					Item {
						Layout.fillHeight: true
					}
				}
				ColumnLayout {
					Layout.fillWidth: true
					spacing: 20 * DefaultStyle.dp
					Layout.rightMargin: 44 * DefaultStyle.dp
					Layout.topMargin: 20 * DefaultStyle.dp
					Layout.leftMargin: 64 * DefaultStyle.dp
					ValidatedTextField {
						id: server
						propertyName: "server"
						propertyOwner: ldapGui.core
						title: qsTr("URL du serveur (ne peut être vide)")
					}
					ValidatedTextField {
						propertyName: "bindDn"
						propertyOwner: ldapGui.core
						title: qsTr("Bind DN")
					}
					ValidatedTextField {
						propertyName: "password"
						hidden: true
						propertyOwner: ldapGui.core
						title: qsTr("Mot de passe")
					}
					SwitchSetting {
						titleText: qsTr("Utiliser TLS")
						propertyName: "tls"
						propertyOwner: ldapGui.core
					}
					ValidatedTextField {
						propertyName: "baseObject"
						propertyOwner: ldapGui.core
						title: qsTr("Base de recherche (ne peut être vide)")
					}
					ValidatedTextField {
						propertyName: "filter"
						propertyOwner: ldapGui.core
						title: qsTr("Filtre")
					}
					ValidatedTextField {
						propertyName: "maxResults"
						propertyOwner: ldapGui.core
						validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
						title: qsTr("Nombre maximum de résultats")
					}
					ValidatedTextField {
						propertyName: "delay"
						propertyOwner: ldapGui.core
						validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
						title: qsTr("Délai entre 2 requêtes (en millisecondes)")
					}
					ValidatedTextField {
						propertyName: "timeout"
						propertyOwner: ldapGui.core
						title: qsTr("Durée maximun (en secondes)")
						validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
					}
					ValidatedTextField {
						propertyName: "minChars"
						propertyOwner: ldapGui.core
						title: qsTr("Nombre minimum de caractères pour la requête")
						validator: RegularExpressionValidator { regularExpression: /[0-9]+/ }
					}
					ValidatedTextField {
						propertyName: "nameAttribute"
						propertyOwner: ldapGui.core
						title: qsTr("Attributs de nom")
					}
					ValidatedTextField {
						propertyName: "sipAttribute"
						propertyOwner: ldapGui.core
						title: qsTr("Attributs SIP")
					}
					ValidatedTextField {
						propertyName: "sipDomain"
						propertyOwner: ldapGui.core
						title: qsTr("Domaine SIP")
					}
				}
			}
		}
	}
}