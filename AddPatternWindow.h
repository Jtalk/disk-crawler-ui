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

#include <QWidget>

class QLineEdit;
class QComboBox;

class AddPatternWindow : public QWidget {
	Q_OBJECT

public:
	struct Result {
		QString pattern;
	};
	
	AddPatternWindow();
	
	void show();
	
	void resize(const QSize &size);
	
private:
	QLineEdit *m_input;
	QComboBox *m_encoding;
	
private slots:
	void complete();
	void rejected();
	
signals:
	void finished(const AddPatternWindow::Result&);
};