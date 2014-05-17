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

#pragma once

#include "base/types.h"

#include <QtGui/QWidget>

class QListWidget;
class QComboBox;

class EncodingsWindow : public QWidget {
	Q_OBJECT

public:
	EncodingsWindow();
	
	void show();
	void hide();
	
	void clear();
	
	encodings_t get() const;
	
private slots:
	void complete();
	void reject();
	
	void add();
	void remove();
	
signals:
	void added();

private:
	QComboBox *m_addField;
	QListWidget *m_items;
	QStringList m_oldItems;
	
	bool modified;
	
	void backup();
	void restore();
	
	void closeEvent(QCloseEvent *event) override;
};