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

#include <QtGui/QMainWindow>

#include <array>

class crawler_qt : public QMainWindow {
	Q_OBJECT
	
private:
	enum ActionType {
		ACTION_QUIT = 0,
		
		MAX_ACTION
	};
	
	std::array<QAction*, MAX_ACTION> m_actions;
	
	void makeActions();
	void makeMenu();
	void makeMain();
	
public:
	crawler_qt();
	virtual ~crawler_qt();
};
