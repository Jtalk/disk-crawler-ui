/*
 *  Disk Crawler library.
 *  Copyright (C) 2013  Jtalk <me@jtalk.me>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QtGui/QWidget>

class QLabel;
class QToolButton;

class NotificationWidget : public QWidget {
	Q_OBJECT
	
public:
	NotificationWidget(QWidget *parent);
	
	void notify(const QString &text);
	
private slots:
	void hide();
	void show();
	
private:
	QToolButton *m_hideButton;
	QLabel *m_label;
	QPalette m_palette;
	QPalette m_defaultPalette;
	
	void paintColour();
};