﻿/*
 * Copyright (c) 2022 Belledonne Communications SARL.
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

#ifndef CONFERENCE_INFO_MODEL_H_
#define CONFERENCE_INFO_MODEL_H_

#include <linphone++/linphone.hh>
#include <QDateTime>
#include <QObject>
#include <QSharedPointer>
#include <QTimeZone>
#include <QTimer>

#include "utils/LinphoneEnums.hpp"

class ParticipantListModel;
class ConferenceScheduler;
class TimeZoneModel;

class ConferenceInfoModel : public QObject {
	Q_OBJECT
	
public:
	Q_PROPERTY(TimeZoneModel * timeZoneModel READ getTimeZoneModel CONSTANT)
	Q_PROPERTY(QDateTime dateTime READ getDateTimeSystem WRITE setDateTime NOTIFY dateTimeChanged)
	Q_PROPERTY(QDateTime dateTimeUtc READ getDateTimeUtc NOTIFY dateTimeChanged)
	Q_PROPERTY(int duration READ getDuration WRITE setDuration NOTIFY durationChanged)
	Q_PROPERTY(QDateTime endDateTime READ getEndDateTime NOTIFY dateTimeChanged)
	Q_PROPERTY(QString organizer READ getOrganizer WRITE setOrganizer NOTIFY organizerChanged)
	Q_PROPERTY(QString subject READ getSubject WRITE setSubject NOTIFY subjectChanged)
	Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
	Q_PROPERTY(QString displayNamesToString READ displayNamesToString NOTIFY participantsChanged)
	Q_PROPERTY(QString uri READ getUri NOTIFY uriChanged)
	Q_PROPERTY(bool isScheduled READ isScheduled WRITE setIsScheduled NOTIFY isScheduledChanged)
	Q_PROPERTY(bool isEnded READ isEnded WRITE setIsEnded NOTIFY isEndedChanged)
	Q_PROPERTY(int inviteMode READ getInviteMode WRITE setInviteMode NOTIFY inviteModeChanged)
	Q_PROPERTY(int participantCount READ getParticipantCount NOTIFY participantsChanged)
	Q_PROPERTY(int allParticipantCount READ getAllParticipantCount NOTIFY participantsChanged)
	Q_PROPERTY(LinphoneEnums::ConferenceInfoState state READ getConferenceInfoState NOTIFY conferenceInfoStateChanged)
	Q_PROPERTY(LinphoneEnums::ConferenceSchedulerState conferenceSchedulerState READ getConferenceSchedulerState NOTIFY conferenceSchedulerStateChanged)
		
	static QSharedPointer<ConferenceInfoModel> create(std::shared_ptr<linphone::ConferenceInfo> conferenceInfo);
	ConferenceInfoModel (QObject * parent = nullptr);
	ConferenceInfoModel (std::shared_ptr<linphone::ConferenceInfo> conferenceInfo, QObject * parent = nullptr);
	~ConferenceInfoModel ();
	std::shared_ptr<linphone::ConferenceInfo> getConferenceInfo();
	static std::shared_ptr<linphone::ConferenceInfo> findConferenceInfo(const std::shared_ptr<const linphone::ConferenceInfo> & conferenceInfo);
	
//-------------------------------
	Q_INVOKABLE void initDateTime();
	
	QDateTime getDateTimeUtc() const;
	QDateTime getDateTimeSystem() const;
	int getDuration() const;
	QDateTime getEndDateTime() const;
	QDateTime getEndDateTimeUtc() const;
	QString getOrganizer() const;	
	QString getSubject() const;
	QString getDescription() const;
	Q_INVOKABLE QString displayNamesToString()const;
	QString getUri() const;
	bool isScheduled() const;
	bool isEnded() const;
	bool getIsEnded() const;
	int getInviteMode() const;
	Q_INVOKABLE QVariantList getParticipants() const;
	Q_INVOKABLE QVariantList getAllParticipants() const;
	Q_INVOKABLE int getParticipantCount()const;
	Q_INVOKABLE int getAllParticipantCount()const;
	Q_INVOKABLE TimeZoneModel* getTimeZoneModel() const;
	Q_INVOKABLE QString getIcalendarString() const;
	LinphoneEnums::ConferenceInfoState getConferenceInfoState() const;
	LinphoneEnums::ConferenceSchedulerState getConferenceSchedulerState() const;
	
	void setDateTime(const QDateTime& dateTime);
	void setDuration(const int& duration);
	void setSubject(const QString& subject);
	void setOrganizer(const QString& organizerAddress);
	void setDescription(const QString& description);
	void setIsScheduled(const bool& on);
	void setIsEnded(const bool& end);
	void setInviteMode(const int& modes);
	
	Q_INVOKABLE void setDateTime(const QDate& date, const QTime& time, TimeZoneModel * model);
	Q_INVOKABLE void setParticipants(ParticipantListModel * participants);
	void setConferenceInfo(std::shared_ptr<linphone::ConferenceInfo> conferenceInfo);
	
// Tools
	Q_INVOKABLE void resetConferenceInfo();// Recreate a new conference info from factory
	Q_INVOKABLE void createConference(const int& securityLevel);
	Q_INVOKABLE void cancelConference();
	Q_INVOKABLE void deleteConferenceInfo();// Remove completly this conference info from DB

// SCHEDULER
	
	virtual void onConferenceSchedulerStateChanged(linphone::ConferenceScheduler::State state);
	virtual void onInvitationsSent(const std::list<std::shared_ptr<linphone::Address>> & failedInvitations);
	
signals:
	void dateTimeChanged();
	void durationChanged();
	void organizerChanged();
	void subjectChanged();
	void descriptionChanged();
	void participantsChanged();
	void uriChanged();
	void isScheduledChanged();
	void isEndedChanged();
	void inviteModeChanged();
	void conferenceInfoStateChanged();
	void conferenceSchedulerStateChanged();
	
	void conferenceCreated();
	void conferenceCreationFailed();
	void conferenceInfoChanged();
	void invitationsSent();
	void removed(bool byUser);
	
private:
	std::shared_ptr<linphone::ConferenceInfo> mConferenceInfo;
	QSharedPointer<ConferenceScheduler> mConferenceScheduler= nullptr;
	
	bool mIsScheduled = true;
	bool mIsEnded = false;
	QTimer mCheckEndTimer;
	int mInviteMode = 0;
	bool mRemoveRequested = false;// true if user has request its deletion from DB
	linphone::ConferenceScheduler::State  mLastConferenceSchedulerState = linphone::ConferenceScheduler::State::Idle;// Workaround for missing getter in scheduler.
};

Q_DECLARE_METATYPE(QSharedPointer<ConferenceInfoModel>)
Q_DECLARE_METATYPE(ConferenceInfoModel*)

#endif
