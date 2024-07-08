/*
 * Copyright (c) 2010-2024 Belledonne Communications SARL.
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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "model/setting/SettingsModel.hpp"
#include "tool/thread/SafeConnection.hpp"

#include <QCommandLineParser>
#include <QObject>
#include <QSettings>
#include <QVariantMap>

class Settings : public QObject, public AbstractObject {
	Q_OBJECT

	// Security
	Q_PROPERTY(bool vfsEnabled READ getVfsEnabled WRITE setVfsEnabled NOTIFY vfsEnabledChanged)

	// Call
	Q_PROPERTY(bool videoEnabled READ getVideoEnabled WRITE setVideoEnabled NOTIFY videoEnabledChanged)
	Q_PROPERTY(bool echoCancellationEnabled READ getEchoCancellationEnabled WRITE setEchoCancellationEnabled NOTIFY
	               echoCancellationEnabledChanged)
	Q_PROPERTY(
	    int echoCancellationCalibration READ getEchoCancellationCalibration NOTIFY echoCancellationCalibrationChanged)
	Q_PROPERTY(bool automaticallyRecordCallsEnabled READ getAutomaticallyRecordCallsEnabled WRITE
	               setAutomaticallyRecordCallsEnabled NOTIFY automaticallyRecordCallsEnabledChanged)

	Q_PROPERTY(bool captureGraphRunning READ getCaptureGraphRunning NOTIFY captureGraphRunningChanged)

	Q_PROPERTY(QStringList captureDevices READ getCaptureDevices NOTIFY captureDevicesChanged)
	Q_PROPERTY(QStringList playbackDevices READ getPlaybackDevices NOTIFY playbackDevicesChanged)

	Q_PROPERTY(float playbackGain READ getPlaybackGain WRITE lSetPlaybackGain NOTIFY playbackGainChanged)
	Q_PROPERTY(float captureGain READ getCaptureGain WRITE lSetCaptureGain NOTIFY captureGainChanged)

	Q_PROPERTY(QString captureDevice READ getCaptureDevice WRITE lSetCaptureDevice NOTIFY captureDeviceChanged)
	Q_PROPERTY(QString playbackDevice READ getPlaybackDevice WRITE lSetPlaybackDevice NOTIFY playbackDeviceChanged)

	Q_PROPERTY(QStringList videoDevices READ getVideoDevices NOTIFY videoDevicesChanged)
	Q_PROPERTY(QString videoDevice READ getVideoDevice WRITE lSetVideoDevice NOTIFY videoDeviceChanged)
	Q_PROPERTY(int videoDeviceIndex READ getVideoDeviceIndex NOTIFY videoDeviceChanged)

	Q_PROPERTY(float micVolume MEMBER _dummy_int NOTIFY micVolumeChanged)

	Q_PROPERTY(bool logsEnabled READ getLogsEnabled WRITE setLogsEnabled NOTIFY logsEnabledChanged)
	Q_PROPERTY(bool fullLogsEnabled READ getFullLogsEnabled WRITE setFullLogsEnabled NOTIFY fullLogsEnabledChanged)
	Q_PROPERTY(QString logsEmail READ getLogsEmail)
	Q_PROPERTY(QString logsFolder READ getLogsFolder)

public:
	static QSharedPointer<Settings> create();
	Settings(QObject *parent = Q_NULLPTR);
	virtual ~Settings();

	void setSelf(QSharedPointer<Settings> me);

	QString getConfigPath(const QCommandLineParser &parser = QCommandLineParser());

	Q_INVOKABLE void setFirstLaunch(bool first);
	Q_INVOKABLE bool getFirstLaunch() const;

	Q_INVOKABLE void setDisplayDeviceCheckConfirmation(bool display);
	Q_INVOKABLE bool getDisplayDeviceCheckConfirmation() const;

	// Security. --------------------------------------------------------------------
	bool getVfsEnabled() {
		return mVfsEnabled;
	}

	// Call. --------------------------------------------------------------------

	bool getVideoEnabled() {
		return mVideoEnabled;
	}
	bool getEchoCancellationEnabled() {
		return mEchoCancellationEnabled;
	}
	bool getAutomaticallyRecordCallsEnabled() {
		return mAutomaticallyRecordCallsEnabled;
	}

	float getPlaybackGain() const;

	float getCaptureGain() const;

	QStringList getCaptureDevices() const;
	QStringList getPlaybackDevices() const;

	QString getCaptureDevice() const;

	QString getPlaybackDevice() const;

	QString getVideoDevice() const {
		return mVideoDevice;
	}
	int getVideoDeviceIndex() const;
	QStringList getVideoDevices() const;

	bool getCaptureGraphRunning();

	Q_INVOKABLE void startEchoCancellerCalibration();
	int getEchoCancellationCalibration() const;

	Q_INVOKABLE void accessCallSettings();
	Q_INVOKABLE void closeCallSettings();
	Q_INVOKABLE void updateMicVolume() const;

	bool getLogsEnabled() const;
	bool getFullLogsEnabled() const;

	Q_INVOKABLE void cleanLogs() const;
	Q_INVOKABLE void sendLogs() const;
	QString getLogsEmail() const;
	QString getLogsFolder() const;

	DECLARE_CORE_GETSET(bool, disableChatFeature, DisableChatFeature)
	DECLARE_CORE_GETSET(bool, disableMeetingsFeature, DisableMeetingsFeature)
	DECLARE_CORE_GETSET(bool, disableBroadcastFeature, DisableBroadcastFeature)
	DECLARE_CORE_GETSET(bool, hideSettings, HideSettings)
	DECLARE_CORE_GETSET(bool, hideAccountSettings, HideAccountSettings)
	DECLARE_CORE_GETSET(bool, disableCallRecordings, DisableCallRecordings)
	DECLARE_CORE_GETSET(bool, assistantHideCreateAccount, AssistantHideCreateAccount)
	DECLARE_CORE_GETSET(bool, assistantDisableQrCode, AssistantDisableQrCode)
	DECLARE_CORE_GETSET(bool, assistantHideThirdPartyAccount, AssistantHideThirdPartyAccount)
	DECLARE_CORE_GETSET(bool, onlyDisplaySipUriUsername, OnlyDisplaySipUriUsername)
	DECLARE_CORE_GETSET(bool, darkModeAllowed, DarkModeAllowed)
	DECLARE_CORE_GETSET(int, maxAccount, MaxAccount)

signals:

	// Security
	void setVfsEnabled(const bool enabled);
	void vfsEnabledChanged();

	// Call
	void setVideoEnabled(const bool enabled);
	void videoEnabledChanged();

	void setEchoCancellationEnabled(const bool enabled);
	void echoCancellationEnabledChanged();

	void setAutomaticallyRecordCallsEnabled(const bool enabled);
	void automaticallyRecordCallsEnabledChanged();

	void captureGraphRunningChanged(bool running);

	void playbackGainChanged(float gain);
	void captureGainChanged(float gain);

	void captureDevicesChanged(const QStringList &devices);
	void playbackDevicesChanged(const QStringList &devices);

	void lSetCaptureDevice(const QString &device);
	void captureDeviceChanged(const QString &device);

	void lSetPlaybackDevice(const QString &device);
	void playbackDeviceChanged(const QString &device);

	void lSetVideoDevice(const QString &device);
	void videoDeviceChanged();
	void videoDevicesChanged();

	void lSetCaptureGain(float gain);
	void lSetPlaybackGain(float gain);

	void echoCancellationCalibrationChanged();
	void micVolumeChanged(float volume);

	void logsEnabledChanged();
	void fullLogsEnabledChanged();

	void setLogsEnabled(bool status);
	void setFullLogsEnabled(bool status);

	void logsUploadTerminated(bool status, QString url);
	void logsEmailChanged(const QString &email);
	void logsFolderChanged(const QString &folder);

	void firstLaunchChanged(bool firstLaunch);
	void showVerifyDeviceConfirmationChanged(bool showVerifyDeviceConfirmation);

private:
	std::shared_ptr<SettingsModel> mSettingsModel;

	// Dummy properties (for properties that use values from core received through signals)
	int _dummy_int = 0;

	// Security
	bool mVfsEnabled;

	// Call
	bool mVideoEnabled;
	bool mEchoCancellationEnabled;
	bool mAutomaticallyRecordCallsEnabled;

	// Audio
	QStringList mCaptureDevices;
	QStringList mPlaybackDevices;
	QString mCaptureDevice;
	QString mPlaybackDevice;

	// Video
	QStringList mVideoDevices;
	QString mVideoDevice;

	bool mCaptureGraphRunning;
	float mCaptureGain;
	float mPlaybackGain;
	int mEchoCancellationCalibration;

	// Debug logs
	bool mLogsEnabled;
	bool mFullLogsEnabled;
	QString mLogsFolder;
	QString mLogsEmail;

	QSettings mAppSettings;
	QSharedPointer<SafeConnection<Settings, SettingsModel>> mSettingsModelConnection;

	DECLARE_ABSTRACT_OBJECT
};
#endif