/*
    Disk Crawler library.
    Copyright (C) 2013  Jtalk <me@jtalk.me>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SearchResultItem.h"

#include "base/ByteReader.h"
#include "base/PlainFileStream.h"

#include <QtGui/QListWidget>

static QString readerInfo(const ByteReader *reader) {
	if (dynamic_cast<const PlainFileStream*>(reader) != nullptr) {
		return "";
	} else {
		return QLatin1String(" in container at 0x") + QString::number(reader->start_offset());
	}
}

SearchResultItem::SearchResultItem(QListWidget *view, const QString &searchPattern, Buffer &&raw, size_t offset, const ByteReader *reader): 
	QListWidgetItem(searchPattern + " at 0x" + QString::number(offset, 16) + readerInfo(reader), view), array(std::move(raw)), offset(offset)
{}

#include "SearchResultItem.moc"
