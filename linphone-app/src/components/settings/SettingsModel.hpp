/*
 * Copyright (c) 2010-2020 Belledonne Communications SARL.
 *
 * This file is part of linphone-desktop
 * (see https://www.linphone.org).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGS_MODEL_H_
#define SETTINGS_MODEL_H_

#include <linphone++/linphone.hh>
#include <utils/MediastreamerUtils.hpp>
#include <QObject>
#include <QVariantMap>
#include <QFont>

#include "components/core/CoreHandlers.hpp"
#include "components/contacts/ContactsImporterModel.hpp"
#include "utils/LinphoneEnums.hpp"
#include "utils/Utils.hpp"

#ifdef ENABLE_QT_KEYCHAIN
#include "components/vfs/VfsUtils.hpp"
#endif

// =============================================================================
class TunnelModel;

class SettingsModel : public QObject {
	Q_OBJECT
	
	// ===========================================================================
	// PROPERTIES.
	// ===========================================================================
	
	// Assistant. ----------------------------------------------------------------

	Q_PROPERTY(bool createAppSipAccountEnabled READ getCreateAppSipAccountEnabled WRITE setCreateAppSipAccountEnabled NOTIFY createAppSipAccountEnabledChanged)
	Q_PROPERTY(bool fetchRemoteConfigurationEnabled READ getFetchRemoteConfigurationEnabled WRITE setFetchRemoteConfigurationEnabled NOTIFY fetchRemoteConfigurationEnabledChanged)
	Q_PROPERTY(bool useAppSipAccountEnabled READ getUseAppSipAccountEnabled WRITE setUseAppSipAccountEnabled NOTIFY useAppSipAccountEnabledChanged)
	Q_PROPERTY(bool useOtherSipAccountEnabled READ getUseOtherSipAccountEnabled WRITE setUseOtherSipAccountEnabled NOTIFY useOtherSipAccountEnabledChanged)
	Q_PROPERTY(bool autoApplyProvisioningConfigUriHandlerEnabled READ getAutoApplyProvisioningConfigUriHandlerEnabled WRITE setAutoApplyProvisioningConfigUriHandlerEnabled NOTIFY autoApplyProvisioningConfigUriHandlerEnabledChanged)
	
	Q_PROPERTY(bool assistantSupportsPhoneNumbers READ getAssistantSupportsPhoneNumbers WRITE setAssistantSupportsPhoneNumbers NOTIFY assistantSupportsPhoneNumbersChanged)
// Webviews config
	Q_PROPERTY(QString assistantRegistrationUrl READ getAssistantRegistrationUrl WRITE setAssistantRegistrationUrl NOTIFY assistantRegistrationUrlChanged)
	Q_PROPERTY(QString assistantLoginUrl READ getAssistantLoginUrl WRITE setAssistantLoginUrl NOTIFY assistantLoginUrlChanged)
	Q_PROPERTY(QString assistantLogoutUrl READ getAssistantLogoutUrl WRITE setAssistantLogoutUrl NOTIFY assistantLogoutUrlChanged)
//----
	Q_PROPERTY(bool cguAccepted READ isCguAccepted WRITE acceptCgu NOTIFY cguAcceptedChanged)
	
	// SIP Accounts. -------------------------------------------------------------
	
	Q_PROPERTY(QString deviceName READ getDeviceName WRITE setDeviceName NOTIFY deviceNameChanged)

	// Audio. --------------------------------------------------------------------
	
	Q_PROPERTY(bool captureGraphRunning READ getCaptureGraphRunning NOTIFY captureGraphRunningChanged)
	
	Q_PROPERTY(QStringList captureDevices READ getCaptureDevices NOTIFY captureDevicesChanged)
	Q_PROPERTY(QStringList playbackDevices READ getPlaybackDevices NOTIFY playbackDevicesChanged)
	
	Q_PROPERTY(float playbackGain READ getPlaybackGain WRITE setPlaybackGain NOTIFY playbackGainChanged)
	Q_PROPERTY(float captureGain READ getCaptureGain WRITE setCaptureGain NOTIFY captureGainChanged)
	
	Q_PROPERTY(QString captureDevice READ getCaptureDevice WRITE setCaptureDevice NOTIFY captureDeviceChanged)
	Q_PROPERTY(QString playbackDevice READ getPlaybackDevice WRITE setPlaybackDevice NOTIFY playbackDeviceChanged)
	Q_PROPERTY(QString ringerDevice READ getRingerDevice WRITE setRingerDevice NOTIFY ringerDeviceChanged)
	
	Q_PROPERTY(QString ringPath READ getRingPath WRITE setRingPath NOTIFY ringPathChanged)
	
	Q_PROPERTY(bool echoCancellationEnabled READ getEchoCancellationEnabled WRITE setEchoCancellationEnabled NOTIFY echoCancellationEnabledChanged)
	
	Q_PROPERTY(bool showAudioCodecs READ getShowAudioCodecs WRITE setShowAudioCodecs NOTIFY showAudioCodecsChanged)
	
	// Video. --------------------------------------------------------------------
	
	Q_PROPERTY(QStringList videoDevices READ getVideoDevices NOTIFY videoDevicesChanged)
	
	Q_PROPERTY(QString videoDevice READ getVideoDevice WRITE setVideoDevice NOTIFY videoDeviceChanged)
	
	Q_PROPERTY(QString videoPreset READ getVideoPreset WRITE setVideoPreset NOTIFY videoPresetChanged)
	Q_PROPERTY(int videoFramerate READ getVideoFramerate WRITE setVideoFramerate NOTIFY videoFramerateChanged)
	
	Q_PROPERTY(QVariantList supportedVideoDefinitions READ getSupportedVideoDefinitions CONSTANT)
	
	Q_PROPERTY(QVariantMap videoDefinition READ getVideoDefinition WRITE setVideoDefinition NOTIFY videoDefinitionChanged)
	
	Q_PROPERTY(bool videoEnabled READ getVideoEnabled WRITE setVideoEnabled NOTIFY videoEnabledChanged)
	
	Q_PROPERTY(bool showVideoCodecs READ getShowVideoCodecs WRITE setShowVideoCodecs NOTIFY showVideoCodecsChanged)
	
	Q_PROPERTY(CameraMode gridCameraMode READ getGridCameraMode WRITE setGridCameraMode NOTIFY gridCameraModeChanged)
	Q_PROPERTY(CameraMode activeSpeakerCameraMode READ getActiveSpeakerCameraMode WRITE setActiveSpeakerCameraMode NOTIFY activeSpeakerCameraModeChanged)
	Q_PROPERTY(CameraMode callCameraMode READ getCallCameraMode WRITE setCallCameraMode NOTIFY callCameraModeChanged)
	Q_PROPERTY(LinphoneEnums::ConferenceLayout videoConferenceLayout READ getVideoConferenceLayout WRITE setVideoConferenceLayout NOTIFY videoConferenceLayoutChanged)
	
	
	// Chat & calls. -------------------------------------------------------------
	
	Q_PROPERTY(bool autoAnswerStatus READ getAutoAnswerStatus WRITE setAutoAnswerStatus NOTIFY autoAnswerStatusChanged)
	Q_PROPERTY(bool autoAnswerVideoStatus READ getAutoAnswerVideoStatus WRITE setAutoAnswerVideoStatus NOTIFY autoAnswerVideoStatusChanged)
	Q_PROPERTY(int autoAnswerDelay READ getAutoAnswerDelay WRITE setAutoAnswerDelay NOTIFY autoAnswerDelayChanged)
	
	Q_PROPERTY(bool showTelKeypadAutomatically READ getShowTelKeypadAutomatically WRITE setShowTelKeypadAutomatically NOTIFY showTelKeypadAutomaticallyChanged)
	
	Q_PROPERTY(bool keepCallsWindowInBackground READ getKeepCallsWindowInBackground WRITE setKeepCallsWindowInBackground NOTIFY keepCallsWindowInBackgroundChanged)
	
	Q_PROPERTY(bool outgoingCallsEnabled READ getOutgoingCallsEnabled WRITE setOutgoingCallsEnabled NOTIFY outgoingCallsEnabledChanged)
	
	Q_PROPERTY(bool callRecorderEnabled READ getCallRecorderEnabled WRITE setCallRecorderEnabled NOTIFY callRecorderEnabledChanged)
	Q_PROPERTY(bool automaticallyRecordCalls READ getAutomaticallyRecordCalls WRITE setAutomaticallyRecordCalls NOTIFY automaticallyRecordCallsChanged)
	Q_PROPERTY(int autoDownloadMaxSize READ getAutoDownloadMaxSize WRITE setAutoDownloadMaxSize NOTIFY autoDownloadMaxSizeChanged)
	
	Q_PROPERTY(bool callPauseEnabled READ getCallPauseEnabled WRITE setCallPauseEnabled NOTIFY callPauseEnabledChanged)
	Q_PROPERTY(bool muteMicrophoneEnabled READ getMuteMicrophoneEnabled WRITE setMuteMicrophoneEnabled NOTIFY muteMicrophoneEnabledChanged)
	
	Q_PROPERTY(bool standardChatEnabled READ getStandardChatEnabled WRITE setStandardChatEnabled NOTIFY standardChatEnabledChanged)
	Q_PROPERTY(bool secureChatEnabled READ getSecureChatEnabled WRITE setSecureChatEnabled NOTIFY secureChatEnabledChanged)
	Q_PROPERTY(bool groupChatEnabled READ getGroupChatEnabled NOTIFY groupChatEnabledChanged)
	Q_PROPERTY(bool hideEmptyChatRooms READ getHideEmptyChatRooms WRITE setHideEmptyChatRooms NOTIFY hideEmptyChatRoomsChanged)
	
	
	Q_PROPERTY(bool waitRegistrationForCall READ getWaitRegistrationForCall WRITE setWaitRegistrationForCall NOTIFY waitRegistrationForCallChanged)// Allow call only if the current proxy has been registered
	Q_PROPERTY(bool incallScreenshotEnabled READ getIncallScreenshotEnabled WRITE setIncallScreenshotEnabled NOTIFY incallScreenshotEnabledChanged)
	
	Q_PROPERTY(bool conferenceEnabled READ getConferenceEnabled WRITE setConferenceEnabled NOTIFY conferenceEnabledChanged)
	Q_PROPERTY(bool videoConferenceEnabled READ getVideoConferenceEnabled NOTIFY videoConferenceEnabledChanged)
	
	Q_PROPERTY(bool chatNotificationsEnabled READ getChatNotificationsEnabled WRITE setChatNotificationsEnabled NOTIFY chatNotificationsEnabledChanged)
	Q_PROPERTY(bool chatNotificationSoundEnabled READ getChatNotificationSoundEnabled WRITE setChatNotificationSoundEnabled NOTIFY chatNotificationSoundEnabledChanged)
	Q_PROPERTY(QString chatNotificationSoundPath READ getChatNotificationSoundPath WRITE setChatNotificationSoundPath NOTIFY chatNotificationSoundPathChanged)
	
	Q_PROPERTY(QString fileTransferUrl READ getFileTransferUrl WRITE setFileTransferUrl NOTIFY fileTransferUrlChanged)
	
	Q_PROPERTY(bool limeIsSupported READ getLimeIsSupported CONSTANT)
	Q_PROPERTY(QVariantList supportedMediaEncryptions READ getSupportedMediaEncryptions CONSTANT)
	
	Q_PROPERTY(MediaEncryption mediaEncryption READ getMediaEncryption WRITE setMediaEncryption NOTIFY mediaEncryptionChanged)
	Q_PROPERTY(bool mediaEncryptionMandatory READ mandatoryMediaEncryptionEnabled WRITE enableMandatoryMediaEncryption NOTIFY mediaEncryptionChanged)
	Q_PROPERTY(bool isPostQuantumAvailable READ getPostQuantumAvailable CONSTANT)
	
	Q_PROPERTY(bool limeState READ getLimeState WRITE setLimeState NOTIFY limeStateChanged)
	
	Q_PROPERTY(bool contactsEnabled READ getContactsEnabled WRITE setContactsEnabled NOTIFY contactsEnabledChanged)
	
	// Network. ------------------------------------------------------------------
	
	Q_PROPERTY(bool showNetworkSettings READ getShowNetworkSettings WRITE setShowNetworkSettings NOTIFY showNetworkSettingsChanged)
	
	Q_PROPERTY(bool useSipInfoForDtmfs READ getUseSipInfoForDtmfs WRITE setUseSipInfoForDtmfs NOTIFY dtmfsProtocolChanged)
	Q_PROPERTY(bool useRfc2833ForDtmfs READ getUseRfc2833ForDtmfs WRITE setUseRfc2833ForDtmfs NOTIFY dtmfsProtocolChanged)
	
	Q_PROPERTY(bool ipv6Enabled READ getIpv6Enabled WRITE setIpv6Enabled NOTIFY ipv6EnabledChanged)
	
	Q_PROPERTY(int downloadBandwidth READ getDownloadBandwidth WRITE setDownloadBandwidth NOTIFY downloadBandWidthChanged)
	Q_PROPERTY(int uploadBandwidth READ getUploadBandwidth WRITE setUploadBandwidth NOTIFY uploadBandWidthChanged)
	
	Q_PROPERTY(
			bool adaptiveRateControlEnabled
			READ getAdaptiveRateControlEnabled
			WRITE setAdaptiveRateControlEnabled
			NOTIFY adaptiveRateControlEnabledChanged
			)
	
	Q_PROPERTY(int tcpPort READ getTcpPort WRITE setTcpPort NOTIFY tcpPortChanged)
	Q_PROPERTY(int udpPort READ getUdpPort WRITE setUdpPort NOTIFY udpPortChanged)
	
	Q_PROPERTY(QList<int> audioPortRange READ getAudioPortRange WRITE setAudioPortRange NOTIFY audioPortRangeChanged)
	Q_PROPERTY(QList<int> videoPortRange READ getVideoPortRange WRITE setVideoPortRange NOTIFY videoPortRangeChanged)
	
	Q_PROPERTY(bool iceEnabled READ getIceEnabled WRITE setIceEnabled NOTIFY iceEnabledChanged)
	Q_PROPERTY(bool turnEnabled READ getTurnEnabled WRITE setTurnEnabled NOTIFY turnEnabledChanged)
	
	Q_PROPERTY(QString stunServer READ getStunServer WRITE setStunServer NOTIFY stunServerChanged)
	
	Q_PROPERTY(QString turnUser READ getTurnUser WRITE setTurnUser NOTIFY turnUserChanged)
	Q_PROPERTY(QString turnPassword READ getTurnPassword WRITE setTurnPassword NOTIFY turnPasswordChanged)
	
	Q_PROPERTY(int dscpSip READ getDscpSip WRITE setDscpSip NOTIFY dscpSipChanged)
	Q_PROPERTY(int dscpAudio READ getDscpAudio WRITE setDscpAudio NOTIFY dscpAudioChanged)
	Q_PROPERTY(int dscpVideo READ getDscpVideo WRITE setDscpVideo NOTIFY dscpVideoChanged)
	
	Q_PROPERTY(bool rlsUriEnabled READ getRlsUriEnabled WRITE setRlsUriEnabled NOTIFY rlsUriEnabledChanged)
	Q_PROPERTY(QString rlsUri READ getRlsUri WRITE setRlsUri NOTIFY rlsUriChanged)
	
	// UI. -----------------------------------------------------------------------
	
	Q_PROPERTY(QFont textMessageFont READ getTextMessageFont WRITE setTextMessageFont NOTIFY textMessageFontChanged)
	Q_PROPERTY(int textMessageFontSize READ getTextMessageFontSize WRITE setTextMessageFontSize NOTIFY textMessageFontSizeChanged)
	Q_PROPERTY(QFont emojiFont READ getEmojiFont WRITE setEmojiFont NOTIFY emojiFontChanged)
	Q_PROPERTY(int emojiFontSize READ getEmojiFontSize WRITE setEmojiFontSize NOTIFY emojiFontSizeChanged)
	
	Q_PROPERTY(QString remoteProvisioning READ getRemoteProvisioning WRITE setRemoteProvisioning NOTIFY remoteProvisioningChanged)
	Q_PROPERTY(QString flexiAPIUrl READ getFlexiAPIUrl WRITE setFlexiAPIUrl NOTIFY flexiAPIUrlChanged)
	
	Q_PROPERTY(QString savedScreenshotsFolder READ getSavedScreenshotsFolder WRITE setSavedScreenshotsFolder NOTIFY savedScreenshotsFolderChanged)
	Q_PROPERTY(QString savedCallsFolder READ getSavedCallsFolder WRITE setSavedCallsFolder NOTIFY savedCallsFolderChanged)
	Q_PROPERTY(QString downloadFolder READ getDownloadFolder WRITE setDownloadFolder NOTIFY downloadFolderChanged)
	
	Q_PROPERTY(bool exitOnClose READ getExitOnClose WRITE setExitOnClose NOTIFY exitOnCloseChanged)
	Q_PROPERTY(bool checkForUpdateEnabled READ isCheckForUpdateEnabled WRITE setCheckForUpdateEnabled NOTIFY checkForUpdateEnabledChanged)
	Q_PROPERTY(QString versionCheckUrl READ getVersionCheckUrl WRITE setVersionCheckUrl NOTIFY versionCheckUrlChanged)
	Q_PROPERTY(VersionCheckType versionCheckType READ getVersionCheckType WRITE setVersionCheckType NOTIFY versionCheckTypeChanged)
	
	Q_PROPERTY(bool showLocalSipAccount READ getShowLocalSipAccount CONSTANT)
	Q_PROPERTY(bool showStartChatButton READ getShowStartChatButton CONSTANT)
	Q_PROPERTY(bool showStartVideoCallButton READ getShowStartVideoCallButton CONSTANT)
	
	Q_PROPERTY(bool mipmapEnabled READ isMipmapEnabled WRITE setMipmapEnabled NOTIFY mipmapEnabledChanged)
	Q_PROPERTY(bool useMinimalTimelineFilter READ useMinimalTimelineFilter WRITE setUseMinimalTimelineFilter NOTIFY useMinimalTimelineFilterChanged)
	Q_PROPERTY(Utils::SipDisplayMode sipDisplayMode READ getSipDisplayMode WRITE setSipDisplayMode NOTIFY sipDisplayModeChanged)
	Q_PROPERTY(int magicSearchMaxResults READ getMagicSearchMaxResults WRITE setMagicSearchMaxResults NOTIFY magicSearchMaxResultsChanged)
	
	Q_PROPERTY(bool dontAskAgainInfoEncryption READ getDontAskAgainInfoEncryption WRITE setDontAskAgainInfoEncryption NOTIFY dontAskAgainInfoEncryptionChanged)
	Q_PROPERTY(bool haveDontAskAgainChoices READ getHaveDontAskAgainChoices NOTIFY haveDontAskAgainChoicesChanged)
	
	
	// Advanced. -----------------------------------------------------------------
	
	Q_PROPERTY(QString logsFolder READ getLogsFolder WRITE setLogsFolder NOTIFY logsFolderChanged)
	Q_PROPERTY(QString logsUploadUrl READ getLogsUploadUrl WRITE setLogsUploadUrl NOTIFY logsUploadUrlChanged)
	Q_PROPERTY(bool logsEnabled READ getLogsEnabled WRITE setLogsEnabled NOTIFY logsEnabledChanged)
	Q_PROPERTY(bool fullLogsEnabled READ getFullLogsEnabled WRITE setFullLogsEnabled NOTIFY fullLogsEnabledChanged)
	Q_PROPERTY(QString logsEmail READ getLogsEmail WRITE setLogsEmail NOTIFY logsEmailChanged)
	
	Q_PROPERTY(bool isVfsEncrypted READ getVfsEncrypted NOTIFY vfsEncryptedChanged)
	
	Q_PROPERTY(bool developerSettingsEnabled READ getDeveloperSettingsEnabled WRITE setDeveloperSettingsEnabled NOTIFY developerSettingsEnabledChanged)
	
	Q_PROPERTY(bool isInCall READ getIsInCall NOTIFY isInCallChanged)
	
public:
	enum MediaEncryption {
		MediaEncryptionNone = int(linphone::MediaEncryption::None),
		MediaEncryptionDtls = int(linphone::MediaEncryption::DTLS),
		MediaEncryptionSrtp = int(linphone::MediaEncryption::SRTP),
		MediaEncryptionZrtp = int(linphone::MediaEncryption::ZRTP)
	};
	Q_ENUM(MediaEncryption)
	
	enum VersionCheckType {
		VersionCheckType_Release,
		VersionCheckType_Nightly,
		VersionCheckType_Custom
	};
	Q_ENUM(VersionCheckType);
	
	enum CameraMode{
		CameraMode_Hybrid = 0,
		CameraMode_OccupyAllSpace = 1, 
		CameraMode_BlackBars = 2
	};
	Q_ENUM(CameraMode);
	static SettingsModel::CameraMode cameraModefromString(const std::string& mode);
	static std::string toString(const CameraMode& mode);
	
	
	SettingsModel (QObject *parent = Q_NULLPTR);
	virtual ~SettingsModel ();
	
	// ===========================================================================
	// METHODS.
	// ===========================================================================
	
	Q_INVOKABLE void onSettingsTabChanged(int idx);
	Q_INVOKABLE void settingsWindowClosing(void);
	Q_INVOKABLE void reloadDevices();
	
	// Assistant. ----------------------------------------------------------------
	
	bool getCreateAppSipAccountEnabled () const;
	void setCreateAppSipAccountEnabled (bool status);
	
	bool getFetchRemoteConfigurationEnabled () const;
	void setFetchRemoteConfigurationEnabled (bool status);
	
	bool getAutoApplyProvisioningConfigUriHandlerEnabled () const;
	void setAutoApplyProvisioningConfigUriHandlerEnabled (bool status);
	
	bool getUseAppSipAccountEnabled () const;
	void setUseAppSipAccountEnabled (bool status);
	
	bool getUseOtherSipAccountEnabled () const;
	void setUseOtherSipAccountEnabled (bool status);
	
	bool getAssistantSupportsPhoneNumbers () const;
	void setAssistantSupportsPhoneNumbers (bool status);
	
	Q_INVOKABLE bool useWebview() const;

	QString getAssistantRegistrationUrl () const;
	void setAssistantRegistrationUrl (QString url);

	QString getAssistantLoginUrl () const;
	void setAssistantLoginUrl (QString url);
	
	QString getAssistantLogoutUrl () const;
	void setAssistantLogoutUrl (QString url);
	
	bool isCguAccepted () const;
	void acceptCgu(const bool accept);
	
	// SIP Accounts. -------------------------------------------------------------
	
	static QString getDeviceName(const std::shared_ptr<linphone::Config>& config);
	QString getDeviceName() const;
	void setDeviceName(const QString& deviceName);

	// Audio. --------------------------------------------------------------------

	Q_INVOKABLE void startCaptureGraph();
	Q_INVOKABLE void stopCaptureGraph();
	Q_INVOKABLE void resetCaptureGraph();
	void createCaptureGraph();
	void deleteCaptureGraph();
	bool getCaptureGraphRunning();
	void accessAudioSettings();
	void closeAudioSettings();
	
	Q_INVOKABLE float getMicVolume();
	
	float getPlaybackGain() const;
	void setPlaybackGain(float gain);
	
	float getCaptureGain() const;
	void setCaptureGain(float gain);
	
	QStringList getCaptureDevices () const;
	QStringList getPlaybackDevices () const;
	
	QString getCaptureDevice () const;
	void setCaptureDevice (const QString &device);
	
	QString getPlaybackDevice () const;
	void setPlaybackDevice (const QString &device);
	
	QString getRingerDevice () const;
	void setRingerDevice (const QString &device);
	
	QString getRingPath () const;
	void setRingPath (const QString &path);
	
	bool getEchoCancellationEnabled () const;
	void setEchoCancellationEnabled (bool status);
	
	Q_INVOKABLE void startEchoCancellerCalibration();
	
	bool getShowAudioCodecs () const;
	void setShowAudioCodecs (bool status);
	
	// Video. --------------------------------------------------------------------
	
	//Called from qml when accessing audio settings panel
	Q_INVOKABLE void accessVideoSettings();
	
	QStringList getVideoDevices () const;
	
	QString getVideoDevice () const;
	void setVideoDevice (const QString &device);
	
	QString getVideoPreset () const;
	void setVideoPreset (const QString &preset);
	
	int getVideoFramerate () const;
	void setVideoFramerate (int framerate);
	
	QVariantList getSupportedVideoDefinitions () const;
	
	Q_INVOKABLE void setHighMosaicQuality();
	Q_INVOKABLE void setLimitedMosaicQuality();
	
	QVariantMap getVideoDefinition () const;
	Q_INVOKABLE QVariantMap getCurrentPreviewVideoDefinition () const;
	void setVideoDefinition (const QVariantMap &definition);
	
	bool getVideoEnabled() const;
	void setVideoEnabled(const bool& enable);
	
	bool getShowVideoCodecs () const;
	void setShowVideoCodecs (bool status);
	
	void updateCameraMode();
	CameraMode getCameraMode() const;
	Q_INVOKABLE void setCameraMode(CameraMode mode);
	// Custom modes to set default for setCameraMode
	CameraMode getGridCameraMode() const;
	void setGridCameraMode(CameraMode mode);
	CameraMode getActiveSpeakerCameraMode() const;
	void setActiveSpeakerCameraMode(CameraMode mode);
	CameraMode getCallCameraMode() const;
	void setCallCameraMode(CameraMode mode);
	
	LinphoneEnums::ConferenceLayout getVideoConferenceLayout() const;
	void setVideoConferenceLayout(LinphoneEnums::ConferenceLayout layout);
	
	
	// Chat & calls. -------------------------------------------------------------
	
	bool getAutoAnswerStatus () const;
	void setAutoAnswerStatus (bool status);
	
	bool getAutoAnswerVideoStatus () const;
	void setAutoAnswerVideoStatus (bool status);
	
	int getAutoAnswerDelay () const;
	void setAutoAnswerDelay (int delay);
	
	bool getShowTelKeypadAutomatically () const;
	void setShowTelKeypadAutomatically (bool status);
	
	bool getKeepCallsWindowInBackground () const;
	void setKeepCallsWindowInBackground (bool status);
	
	bool getOutgoingCallsEnabled () const;
	void setOutgoingCallsEnabled (bool status);
	
	bool getCallRecorderEnabled () const;
	void setCallRecorderEnabled (bool status);
	
	bool getAutomaticallyRecordCalls () const;
	void setAutomaticallyRecordCalls (bool status);
	
	int getAutoDownloadMaxSize() const;
	void setAutoDownloadMaxSize(int maxSize);
	
	bool getCallPauseEnabled () const;
	void setCallPauseEnabled (bool status);
	
	bool getMuteMicrophoneEnabled () const;
	void setMuteMicrophoneEnabled (bool status);
	
	bool getStandardChatEnabled () const;
	void setStandardChatEnabled (bool status);
	
	bool getSecureChatEnabled () const;
	void setSecureChatEnabled (bool status);
	
	bool getHideEmptyChatRooms() const;
	void setHideEmptyChatRooms(const bool& data);
	
	bool getWaitRegistrationForCall() const;
	void setWaitRegistrationForCall(const bool& status);
	
	bool getIncallScreenshotEnabled() const;
	void setIncallScreenshotEnabled(const bool& status);
	
	bool getGroupChatEnabled()const;
	
	bool getConferenceEnabled () const;
	void setConferenceEnabled (bool status);
	bool getVideoConferenceEnabled()const;
	
	bool getChatNotificationsEnabled () const;
	void setChatNotificationsEnabled (bool status);
	
	bool getChatNotificationSoundEnabled () const;
	void setChatNotificationSoundEnabled (bool status);
	
	QString getChatNotificationSoundPath () const;
	void setChatNotificationSoundPath (const QString &path);
	
	QString getFileTransferUrl () const;
	void setFileTransferUrl (const QString &url);
	
	bool getLimeIsSupported () const;
	QVariantList getSupportedMediaEncryptions () const;
	
	MediaEncryption getMediaEncryption () const;
	void setMediaEncryption (MediaEncryption encryption);
	
	bool mandatoryMediaEncryptionEnabled () const;
	void enableMandatoryMediaEncryption(bool mandatory);
	
	bool getPostQuantumAvailable() const;
	
	bool getDontAskAgainInfoEncryption() const;
	void setDontAskAgainInfoEncryption(bool show);
	
	bool getLimeState () const;
	void setLimeState (const bool& state);
	
	bool getContactsEnabled () const;
	void setContactsEnabled (bool status);
	
	int getIncomingCallTimeout() const;
	
	// Network. ------------------------------------------------------------------
	
	bool getShowNetworkSettings () const;
	void setShowNetworkSettings (bool status);
	
	bool getUseSipInfoForDtmfs () const;
	void setUseSipInfoForDtmfs (bool status);
	
	bool getUseRfc2833ForDtmfs () const;
	void setUseRfc2833ForDtmfs (bool status);
	
	bool getIpv6Enabled () const;
	void setIpv6Enabled (bool status);
	
	int getDownloadBandwidth () const;
	void setDownloadBandwidth (int bandwidth);
	
	int getUploadBandwidth () const;
	void setUploadBandwidth (int bandwidth);
	
	bool getAdaptiveRateControlEnabled () const;
	void setAdaptiveRateControlEnabled (bool status);
	
	int getTcpPort () const;
	void setTcpPort (int port);
	
	int getUdpPort () const;
	void setUdpPort (int port);
	
	QList<int> getAudioPortRange () const;
	void setAudioPortRange (const QList<int> &range);
	
	QList<int> getVideoPortRange () const;
	void setVideoPortRange (const QList<int> &range);
	
	bool getIceEnabled () const;
	void setIceEnabled (bool status);
	
	bool getTurnEnabled () const;
	void setTurnEnabled (bool status);
	
	QString getStunServer () const;
	void setStunServer (const QString &stunServer);
	
	QString getTurnUser () const;
	void setTurnUser (const QString &user);
	
	QString getTurnPassword () const;
	void setTurnPassword (const QString &password);
	
	int getDscpSip () const;
	void setDscpSip (int dscp);
	
	int getDscpAudio () const;
	void setDscpAudio (int dscp);
	
	int getDscpVideo () const;
	void setDscpVideo (int dscp);
	
	bool getRlsUriEnabled () const;
	void setRlsUriEnabled (bool status);
	
	QString getRlsUri() const;
	void setRlsUri (const QString& rlsUri);
	void updateRlsUri();
	
	Q_INVOKABLE bool tunnelAvailable() const;
	Q_INVOKABLE TunnelModel * getTunnel() const;
	
	// UI. -----------------------------------------------------------------------
	
	QFont getTextMessageFont() const;
	void setTextMessageFont(const QFont& font);
	
	int getTextMessageFontSize() const;
	void setTextMessageFontSize(const int& size);
	
	QFont getEmojiFont() const;
	void setEmojiFont(const QFont& font);
	
	int getEmojiFontSize() const;
	void setEmojiFontSize(const int& size);
	
	QString getSavedScreenshotsFolder () const;
	void setSavedScreenshotsFolder (const QString &folder);
	
	QString getSavedCallsFolder () const;
	void setSavedCallsFolder (const QString &folder);
	
	QString getDownloadFolder () const;
	void setDownloadFolder (const QString &folder);
	
	QString getRemoteProvisioningRootUrl() const;
	QString getRemoteProvisioning () const;
	void setRemoteProvisioning (const QString &remoteProvisioning);
	
	Q_INVOKABLE bool isQRCodeAvailable() const;
	QString getFlexiAPIUrl() const;
	void setFlexiAPIUrl (const QString &url);
	
	bool getExitOnClose () const;
	void setExitOnClose (bool value);
	
	Q_INVOKABLE static bool isCheckForUpdateAvailable();
	bool isCheckForUpdateEnabled() const;
	void setCheckForUpdateEnabled(bool enable);
	
	QString getVersionCheckUrl();
	void setVersionCheckUrl(const QString& url);
	
	VersionCheckType getVersionCheckType() const;
	void setVersionCheckType(const VersionCheckType& type);
	Q_INVOKABLE bool haveVersionNightlyUrl()const;
	
	
	Q_INVOKABLE bool getShowLocalSipAccount () const;
	Q_INVOKABLE bool getShowStartChatButton () const;
	Q_INVOKABLE bool getShowStartVideoCallButton () const;
	
	bool isMipmapEnabled() const;
	void setMipmapEnabled(const bool& enabled);
	
	bool useMinimalTimelineFilter() const;
	void setUseMinimalTimelineFilter(const bool& useMinimal);
	
	Utils::SipDisplayMode getSipDisplayMode() const;
	void setSipDisplayMode(Utils::SipDisplayMode mode);
	
	int getMagicSearchMaxResults() const;
	void setMagicSearchMaxResults(int maxResults);
	
// Show all "don't ask again" checkboxes and popups.
	bool getHaveDontAskAgainChoices() const;
	Q_INVOKABLE void resetDontAskAgainChoices();
	
	// Advanced. ---------------------------------------------------------------------------
	
	
	void accessAdvancedSettings();
	
	Q_INVOKABLE QString getLogText()const;
	
	QString getLogsFolder () const;
	void setLogsFolder (const QString &folder);
	
	QString getLogsUploadUrl () const;
	void setLogsUploadUrl (const QString &url);
	
	bool getLogsEnabled () const;
	void setLogsEnabled (bool status);
	
	bool getFullLogsEnabled () const;
	void setFullLogsEnabled (bool status);
	
	QString getLogsEmail () const;
	void setLogsEmail (const QString &email);
	
	bool getVfsEncrypted ();
	Q_INVOKABLE void setVfsEncrypted (bool encrypted, const bool deleteUserData);
	
	Q_INVOKABLE bool isLdapAvailable();
	
// OAuth 2
	Q_INVOKABLE bool isOAuth2Available();
	QString getOAuth2AuthorizationUrl()const;
	QString getOAuth2AccessTokenUrl()const;
	QString getOAuth2RedirectUri()const;
	QString getOAuth2Identifier()const;
	QString getOAuth2Password()const;
	QString getOAuth2Scope()const;
	QString getOAuth2RemoteProvisioningBasicAuth()const;
	QString getOAuth2RemoteProvisioningHeader()const;
	
	// ---------------------------------------------------------------------------
	
	static QString getLogsFolder (const std::shared_ptr<linphone::Config> &config);
	static bool getLogsEnabled (const std::shared_ptr<linphone::Config> &config);
	static bool getFullLogsEnabled (const std::shared_ptr<linphone::Config> &config);
	
	// ---------------------------------------------------------------------------
	Q_INVOKABLE bool isDeveloperSettingsAvailable() const;
	bool getDeveloperSettingsEnabled () const;
	void setDeveloperSettingsEnabled (bool status);
	
	void handleCallCreated(const std::shared_ptr<linphone::Call> &call);
	void handleCallStateChanged(const std::shared_ptr<linphone::Call> &call, linphone::Call::State state);
	void handleEcCalibrationResult(linphone::EcCalibratorStatus status, int delayMs);
	
	bool getIsInCall() const;
	
	bool isReadOnly(const std::string& section, const std::string& name) const;
	std::string getEntryFullName(const std::string& section, const std::string& name) const;	// Return the full name of the entry : 'name/readonly' or 'name'
	
	static const std::string UiSection;
	static const std::string ContactsSection;
	
	// ===========================================================================
	// SIGNALS.
	// ===========================================================================
	
signals:
	// Assistant. ----------------------------------------------------------------
	
	void createAppSipAccountEnabledChanged (bool status);
	void fetchRemoteConfigurationEnabledChanged (bool status);
	void useAppSipAccountEnabledChanged (bool status);
	void useOtherSipAccountEnabledChanged (bool status);
	void autoApplyProvisioningConfigUriHandlerEnabledChanged();
	
	void assistantSupportsPhoneNumbersChanged (bool status);

	void assistantRegistrationUrlChanged (QString url);
	void assistantLoginUrlChanged (QString url);
	void assistantLogoutUrlChanged (QString url);
	
	void cguAcceptedChanged(bool accepted);
	
	// SIP Accounts. -------------------------------------------------------------
	
	void deviceNameChanged();

	// Audio. --------------------------------------------------------------------
	
	void captureGraphRunningChanged(bool running);
	
	void playbackGainChanged(float gain);
	void captureGainChanged(float gain);
	
	void captureDevicesChanged (const QStringList &devices);
	void playbackDevicesChanged (const QStringList &devices);
	
	void captureDeviceChanged (const QString &device);
	void playbackDeviceChanged (const QString &device);
	void ringerDeviceChanged (const QString &device);
	
	void ringPathChanged (const QString &path);
	
	void echoCancellationEnabledChanged (bool status);
	void echoCancellationStatus(int status, int msDelay);
	
	void showAudioCodecsChanged (bool status);
	
	// Video. --------------------------------------------------------------------
	void videoEnabledChanged();
	void videoDevicesChanged (const QStringList &devices);
	void videoDeviceChanged (const QString &device);
	
	void videoPresetChanged (const QString &preset);
	void videoFramerateChanged (int framerate);
	
	void videoDefinitionChanged (const QVariantMap &definition);
	
	void showVideoCodecsChanged (bool status);
	
	void cameraModeChanged();
	void gridCameraModeChanged();
	void activeSpeakerCameraModeChanged();
	void callCameraModeChanged();
	void videoConferenceLayoutChanged();
	
	// Chat & calls. -------------------------------------------------------------
	
	void autoAnswerStatusChanged (bool status);
	void autoAnswerVideoStatusChanged (bool status);
	void autoAnswerDelayChanged (int delay);
	
	void showTelKeypadAutomaticallyChanged (bool status);
	
	void keepCallsWindowInBackgroundChanged (bool status);
	
	void outgoingCallsEnabledChanged (bool status);
	
	void callRecorderEnabledChanged (bool status);
	void automaticallyRecordCallsChanged (bool status);
	void autoDownloadMaxSizeChanged (int maxSize);
	
	void callPauseEnabledChanged (bool status);
	void muteMicrophoneEnabledChanged (bool status);
	
	void standardChatEnabledChanged (bool status);
	void secureChatEnabledChanged ();
	void groupChatEnabledChanged();
	void hideEmptyChatRoomsChanged (bool status);
	void waitRegistrationForCallChanged (bool status);
	void incallScreenshotEnabledChanged(bool status);
	
	void conferenceEnabledChanged (bool status);
	void videoConferenceEnabledChanged ();
	
	void chatNotificationsEnabledChanged (bool status);
	void chatNotificationSoundEnabledChanged (bool status);
	void chatNotificationSoundPathChanged (const QString &path);
	
	void fileTransferUrlChanged (const QString &url);
	
	void mediaEncryptionChanged (MediaEncryption encryption);
	void limeStateChanged (bool state);
	
	void contactsEnabledChanged (bool status);
	
	// Network. ------------------------------------------------------------------
	
	void showNetworkSettingsChanged (bool status);
	
	void dtmfsProtocolChanged ();
	
	void ipv6EnabledChanged (bool status);
	
	void downloadBandWidthChanged (int bandwidth);
	void uploadBandWidthChanged (int bandwidth);
	
	bool adaptiveRateControlEnabledChanged (bool status);
	
	void tcpPortChanged (int port);
	void udpPortChanged (int port);
	
	void audioPortRangeChanged (int a, int b);
	void videoPortRangeChanged (int a, int b);
	
	void iceEnabledChanged (bool status);
	void turnEnabledChanged (bool status);
	
	void stunServerChanged (const QString &server);
	
	void turnUserChanged (const QString &user);
	void turnPasswordChanged (const QString &password);
	
	void dscpSipChanged (int dscp);
	void dscpAudioChanged (int dscp);
	void dscpVideoChanged (int dscp);
	
	void rlsUriEnabledChanged (bool status);
	void rlsUriChanged ();
		
	// UI. -----------------------------------------------------------------------
	
	void textMessageFontChanged(const QFont& font);
	void textMessageFontSizeChanged(const int& size);
	
	void emojiFontChanged(const QFont& font);
	void emojiFontSizeChanged(const int& size);
	
	void savedScreenshotsFolderChanged (const QString &folder);
	void savedCallsFolderChanged (const QString &folder);
	void downloadFolderChanged (const QString &folder);
	
	void remoteProvisioningChanged (const QString &remoteProvisioning);
	void remoteProvisioningNotChanged (const QString &remoteProvisioning);
	void flexiAPIUrlChanged (const QString &url);
	
	void exitOnCloseChanged (bool value);
	void mipmapEnabledChanged();
	void useMinimalTimelineFilterChanged();
	
	void sipDisplayModeChanged();
	
	void checkForUpdateEnabledChanged();
	void versionCheckUrlChanged();
	void versionCheckTypeChanged();
	
	void magicSearchMaxResultsChanged();
	
	void dontAskAgainInfoEncryptionChanged();
	void haveDontAskAgainChoicesChanged();
	
	// Advanced. -----------------------------------------------------------------
	
	void logsFolderChanged (const QString &folder);
	void logsUploadUrlChanged (const QString &url);
	void logsEnabledChanged (bool status);
	void fullLogsEnabledChanged ();
	void logsEmailChanged (const QString &email);
	void vfsEncryptedChanged();
	
	void contactImporterChanged();
	
	bool developerSettingsEnabledChanged (bool status);
	
	bool isInCallChanged(bool);
	
private:
	int mCurrentSettingsTab = 0;
	MediastreamerUtils::SimpleCaptureGraph *mSimpleCaptureGraph = nullptr;
	int mCaptureGraphListenerCount = 0;
#ifdef ENABLE_QT_KEYCHAIN
	VfsUtils mVfsUtils;
#endif
	
	std::shared_ptr<linphone::Config> mConfig;
};

Q_DECLARE_METATYPE(std::shared_ptr<const linphone::VideoDefinition>);

#endif // SETTINGS_MODEL_H_
