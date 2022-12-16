/*
 * Copyright (c) 2010-2022 Belledonne Communications SARL.
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

#include "VfsUtils.hpp"

#include <bctoolbox/crypto.hh>
#include <linphone/api/c-factory.h>
#include <linphone++/factory.hh>

#include <utils/Utils.hpp>
#include <utils/Constants.hpp>

#include <QCoreApplication>
#include <QDebug>

// =============================================================================

VfsUtils::VfsUtils (QObject *parent) : QObject(parent)
	, mReadCredentialJob(QLatin1String(APPLICATION_ID))
    , mWriteCredentialJob(QLatin1String(APPLICATION_ID))
	, mDeleteCredentialJob(QLatin1String(APPLICATION_ID))
{
    mReadCredentialJob.setAutoDelete(false);
    mWriteCredentialJob.setAutoDelete(false);
    mDeleteCredentialJob.setAutoDelete(false);
}

void VfsUtils::deleteKey(const QString &key){
    mDeleteCredentialJob.setKey(key);

    QObject::connect(&mDeleteCredentialJob, &QKeychain::DeletePasswordJob::finished, [=](){
        if (mDeleteCredentialJob.error()) {
            emit error(tr("Delete key failed: %1").arg(qPrintable(mDeleteCredentialJob.errorString())));
            return;
        }
        emit keyDeleted(key);
    });

    mDeleteCredentialJob.start();
}

void VfsUtils::readKey(const QString &key) {
    mReadCredentialJob.setKey(key);
    QObject::connect(&mReadCredentialJob, &QKeychain::ReadPasswordJob::finished, [=](){
        if (mReadCredentialJob.error()) {
            emit error(tr("Read key failed: %1").arg(qPrintable(mReadCredentialJob.errorString())));
            return;
        }
        emit keyRead(key, mReadCredentialJob.textData());
    });

    mReadCredentialJob.start();
}

void VfsUtils::writeKey(const QString &key, const QString &value) {
    mWriteCredentialJob.setKey(key);

    QObject::connect(&mWriteCredentialJob, &QKeychain::WritePasswordJob::finished, [=](){
        if (mWriteCredentialJob.error()) {
            emit error(tr("Write key failed: %1").arg(qPrintable(mWriteCredentialJob.errorString())));
            return;
        }
	if(key == getApplicationVfsEncryptionKey())
		updateSDKWithKey(value);
        emit keyWritten(key);
    });

    mWriteCredentialJob.setTextData(value);
    mWriteCredentialJob.start();
}

bool VfsUtils::needToDeleteUserData() const{
	return mNeedToDeleteUserData;
}

void VfsUtils::needToDeleteUserData(const bool& need){
	mNeedToDeleteUserData = need;
}
	
//-----------------------------------------------------------------------------------------------

void VfsUtils::newEncryptionKey(){
	QString value;
	bctoolbox::RNG rng;
	auto key = rng.randomize(32);
	size_t keySize = key.size();
	uint8_t * shaKey = new uint8_t[keySize];
	bctbx_sha256(&key[0], key.size(), keySize, shaKey);
	for(int i = 0 ; i < keySize ; ++i)
		value += QString::number(shaKey[i], 16);
	writeKey(getApplicationVfsEncryptionKey(), value);
}

bool VfsUtils::updateSDKWithKey() {
	int argc = 1;
	const char * argv = "dummy";
	QCoreApplication vfsSetter(argc,(char**)&argv);
	VfsUtils vfs;
	QObject::connect(&vfs, &VfsUtils::keyRead, &vfsSetter, [&vfsSetter, &vfs] (const QString& key, const QString& value){
		VfsUtils::updateSDKWithKey(value);
		vfs.mVfsEncrypted = true;
		vfsSetter.quit();
	}, Qt::QueuedConnection);
	QObject::connect(&vfs, &VfsUtils::error, &vfsSetter, [&vfsSetter](){
		vfsSetter.quit();
	}, Qt::QueuedConnection);
	vfs.readKey(vfs.getApplicationVfsEncryptionKey());
	vfsSetter.exec();
	return vfs.mVfsEncrypted;
}

void VfsUtils::updateSDKWithKey(const QString& key){
	std::string value = Utils::appStringToCoreString(key);
	linphone::Factory::get()->setVfsEncryption(LINPHONE_VFS_ENCRYPTION_AES256GCM128_SHA256, (const uint8_t*)value.c_str(), std::min(32, (int)value.length()));
}
	
QString VfsUtils::getApplicationVfsEncryptionKey() const{
	return QString(APPLICATION_ID)+"VfsEncryption";
}