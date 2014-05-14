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

#include <QtGui/QListWidget>

SearchResultItem::SearchResultItem(QListWidget *view, const QString &searchPattern, const byte_array_t &raw, size_t offset): 
	QListWidgetItem(searchPattern + " at 0x" + QString::number(offset, 16), view), array(raw), offset(offset)
{}

#include "SearchResultItem.moc"
