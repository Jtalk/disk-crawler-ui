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

#include "crawler-qt.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>

crawler_qt::crawler_qt() {
	this->makeActions();
	this->makeMenu();
	this->makeMain();
}

crawler_qt::~crawler_qt()
{}

void crawler_qt::makeActions() {
	this->m_actions.fill(nullptr);
	
	this->m_actions[ACTION_QUIT] = new QAction(tr("Quit"), this);
	connect(this->m_actions[ACTION_QUIT], SIGNAL(triggered()), SLOT(close()));
}

void crawler_qt::makeMenu() {
	auto menubar = this->menuBar();
	auto file = new QMenu(tr("&File"), menubar);
	file->addAction(this->m_actions[ACTION_QUIT]);
	menubar->addMenu(file);
}

void crawler_qt::makeMain() {
	QLabel *l = new QLabel(this);
	l->setText("Hello World!");
	setCentralWidget(l);
}



#include "crawler-qt.moc"

