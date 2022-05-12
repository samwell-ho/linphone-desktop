/*
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

#include "components/core/CoreManager.hpp"

#include "TimeZoneModel.hpp"
#include "TimeZoneListModel.hpp"
#include "TimeZoneProxyModel.hpp"

// -----------------------------------------------------------------------------

TimeZoneProxyModel::TimeZoneProxyModel (QObject *parent) : QSortFilterProxyModel(parent) {
	setSourceModel(new TimeZoneListModel(parent));
	sort(0);
}

// -----------------------------------------------------------------------------

bool TimeZoneProxyModel::filterAcceptsRow (int sourceRow, const QModelIndex &sourceParent) const {
  const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
  return true;
}

bool TimeZoneProxyModel::lessThan (const QModelIndex &left, const QModelIndex &right) const {
	auto test = sourceModel()->data(left);
	const TimeZoneModel* a = sourceModel()->data(left).value<TimeZoneModel*>();
	const TimeZoneModel* b = sourceModel()->data(right).value<TimeZoneModel*>();
    auto timeA = a->getStandardTimeOffset();
    auto timeB = b->getStandardTimeOffset();

/*
	const QVariantMap mapA = sourceModel()->data(left).value<QVariantMap>();
	const QVariantMap mapB = sourceModel()->data(right).value<QVariantMap>();
    const TimeZoneModel* a = mapA["timeZoneModel"].value<TimeZoneModel*>();
    const TimeZoneModel* b = mapB["timeZoneModel"].value<TimeZoneModel*>();
    auto timeA = a->getStandardTimeOffset();
    auto timeB = b->getStandardTimeOffset();
    */
  
	return timeA < timeB || (timeA == timeB && a->getCountryName() < b->getCountryName());
}

int TimeZoneProxyModel::getDefaultIndex() const{
	return mapFromSource(sourceModel()->index(qobject_cast<TimeZoneListModel*>(sourceModel())->getDefaultIndex(), 0)).row();
}
