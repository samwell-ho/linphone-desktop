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

#include "ParticipantProxy.hpp"
#include "ParticipantList.hpp"

// #include "core/conference/ConferenceCore.hpp"
#include "model/core/CoreModel.hpp"
#include "tool/Utils.hpp"

#include "ParticipantList.hpp"
#include "core/participant/ParticipantCore.hpp"

#include <QDebug>

// =============================================================================

DEFINE_ABSTRACT_OBJECT(ParticipantProxy)

ParticipantProxy::ParticipantProxy(QObject *parent) : SortFilterProxy(parent) {
	mParticipants = ParticipantList::create();
	connect(this, &ParticipantProxy::chatRoomModelChanged, this, &ParticipantProxy::countChanged);
	connect(this, &ParticipantProxy::conferenceModelChanged, this, &ParticipantProxy::countChanged);
	setSourceModel(mParticipants.get());
}

ParticipantProxy::~ParticipantProxy() {
	setSourceModel(nullptr);
}

CallGui *ParticipantProxy::getCurrentCall() const {
	return mCurrentCall;
}

void ParticipantProxy::setCurrentCall(CallGui *call) {
	qDebug() << "[ParticipantProxy] set current call " << this << " => " << call;
	if (mCurrentCall != call) {
		CallCore *callCore = nullptr;
		if (mCurrentCall) {
			callCore = mCurrentCall->getCore();
			if (callCore) callCore->disconnect(mParticipants.get());
			callCore = nullptr;
		}
		mCurrentCall = call;
		if (mCurrentCall) callCore = mCurrentCall->getCore();
		if (callCore) {
			connect(callCore, &CallCore::conferenceChanged, mParticipants.get(), [this]() {
				auto conference = mCurrentCall->getCore()->getConferenceCore();
				qDebug() << "[ParticipantDeviceProxy] set conference " << this << " => " << conference;
				mParticipants->setConferenceModel(conference ? conference->getModel() : nullptr);
				// mParticipants->lSetConferenceModel(conference ? conference->getModel() : nullptr);
			});
			auto conference = callCore->getConferenceCore();
			qDebug() << "[ParticipantDeviceProxy] set conference " << this << " => " << conference;
			mParticipants->setConferenceModel(conference ? conference->getModel() : nullptr);
			// mParticipants->lSetConferenceModel(conference ? conference->getModel() : nullptr);
		}
		emit currentCallChanged();
	}
}

// ChatRoomModel *ParticipantProxy::getChatRoomModel() const {
// return mChatRoomModel;
// }

// ConferenceModel *ParticipantProxy::getConferenceModel() const {
// return mConferenceModel;
// }

bool ParticipantProxy::getShowMe() const {
	return mShowMe;
}

// -----------------------------------------------------------------------------

// void ParticipantProxy::setChatRoomModel(ChatRoomModel *chatRoomModel) {
// if (!mChatRoomModel || mChatRoomModel != chatRoomModel) {
// 	mChatRoomModel = chatRoomModel;
// 	if (mChatRoomModel) {
// 		auto participants = mChatRoomModel->getParticipantList();
// 		connect(participants, &ParticipantList::countChanged, this, &ParticipantProxy::countChanged);
// 		setSourceModel(participants);
// 		emit participantListChanged();
// 		for (int i = 0; i < participants->getCount(); ++i) {
// 			auto participant = participants->getAt<ParticipantCore>(i);
// 			connect(participant.get(), &ParticipantCore::invitationTimeout, this, &ParticipantProxy::removeModel);
// 			emit addressAdded(participant->getSipAddress());
// 		}
// 	} else if (!sourceModel()) {
// 		auto model = new ParticipantList((ChatRoomModel *)nullptr, this);
// 		connect(model, &ParticipantList::countChanged, this, &ParticipantProxy::countChanged);
// 		setSourceModel(model);
// 		emit participantListChanged();
// 	}
// 	sort(0);
// 	emit chatRoomModelChanged();
// }
// }

void ParticipantProxy::setConferenceModel(ConferenceModel *conferenceModel) {
	// if (!mConferenceModel || mConferenceModel != conferenceModel) {
	// 	mConferenceModel = conferenceModel;
	// 	if (mConferenceModel) {
	// 		auto participants = mConferenceModel->getParticipantList();
	// 		connect(participants, &ParticipantList::countChanged, this, &ParticipantProxy::countChanged);
	// 		setSourceModel(participants);
	// 		emit participantListChanged();
	// 		for (int i = 0; i < participants->getCount(); ++i) {
	// 			auto participant = participants->getAt<ParticipantCore>(i);
	// 			connect(participant.get(), &ParticipantCore::invitationTimeout, this, &ParticipantProxy::removeModel);
	// 			emit addressAdded(participant->getSipAddress());
	// 		}
	// 	} else if (!sourceModel()) {
	// 		auto model = new ParticipantList((ConferenceModel *)nullptr, this);
	// 		connect(model, &ParticipantList::countChanged, this, &ParticipantProxy::countChanged);
	// 		setSourceModel(model);
	// 		emit participantListChanged();
	// 	}
	// 	sort(0);
	// 	emit conferenceModelChanged();
	// }
}

void ParticipantProxy::setAddresses(ConferenceInfoModel *conferenceInfoModel) {
	// if (conferenceInfoModel && conferenceInfoModel->getConferenceInfo())
	// for (auto address : conferenceInfoModel->getConferenceInfo()->getParticipants())
	// addAddress(QString::fromStdString(address->asString()));
}

void ParticipantProxy::setShowMe(const bool &show) {
	if (mShowMe != show) {
		mShowMe = show;
		emit showMeChanged();
		invalidate();
	}
}

// void ParticipantProxy::addAddress(const QString &address) {
// 	if (!participantsModel->contains(address)) {
// 		QSharedPointer<ParticipantCore> participant = QSharedPointer<ParticipantCore>::create(nullptr);
// 		connect(participant.get(), &ParticipantCore::invitationTimeout, this, &ParticipantProxy::removeModel);
// 		participant->setSipAddress(address);
// 		participantsModel->add(participant);
// 		if (mChatRoomModel && mChatRoomModel->getChatRoom()) { // Invite and wait for its creation
// 			participant->startInvitation();
// 			mChatRoomModel->getChatRoom()->addParticipant(Utils::interpretUrl(address));
// 		}
// 		if (mConferenceModel && mConferenceModel->getConference()) {
// 			auto addressToInvite = Utils::interpretUrl(address);
// 			std::list<std::shared_ptr<linphone::Call>> runningCallsToAdd;
// 			auto currentCalls = CoreManager::getInstance()->getCore()->getCalls();
// 			auto haveCall = std::find_if(currentCalls.begin(), currentCalls.end(),
// 			                             [addressToInvite](const std::shared_ptr<linphone::Call> &call) {
// 				                             return call->getRemoteAddress()->weakEqual(addressToInvite);
// 			                             });
// 			participant->startInvitation();
// 			if (haveCall == currentCalls.end()) mConferenceModel->getConference()->addParticipant(addressToInvite);
// 			else {
// 				runningCallsToAdd.push_back(*haveCall);
// 				mConferenceModel->getConference()->addParticipants(runningCallsToAdd);
// 			}
// 			/*
// 			    std::list<std::shared_ptr<linphone::Address>> addressesToInvite;
// 			    addressesToInvite.push_back(addressToInvite);
// 			    auto callParameters =
// 			   CoreManager::getInstance()->getCore()->createCallParams(mConferenceModel->getConference()->getCall());
// 			    mConferenceModel->getConference()->inviteParticipants(addressesToInvite, callParameters);*/
// 		}
// 		emit countChanged();
// 		emit addressAdded(address);
// 	}
// }

void ParticipantProxy::removeParticipant(ParticipantCore *participant) {
	if (participant) {
		mParticipants->remove(participant);
	}
}

void ParticipantProxy::setParticipantAdminStatus(ParticipantCore *participant, bool status) {
	emit mParticipants->lSetParticipantAdminStatus(participant, status);
}

// -----------------------------------------------------------------------------

bool ParticipantProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
	if (mShowMe) return true;
	else {
		const ParticipantCore *a =
		    sourceModel()->data(sourceModel()->index(sourceRow, 0, sourceParent)).value<ParticipantCore *>();
		return !a->isMe();
	}
}

bool ParticipantProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const {
	const ParticipantCore *a = sourceModel()->data(left).value<ParticipantCore *>();
	const ParticipantCore *b = sourceModel()->data(right).value<ParticipantCore *>();

	return a->getCreationTime() > b->getCreationTime() || b->isMe();
}
