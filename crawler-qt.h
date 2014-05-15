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

#include "devpick.h"

#include <QtGui/QMainWindow>

#include <array>

class QListWidget;
class QListWidgetItem;
class QComboBox;
class QAction;
class QProgressBar;
class QCheckBox;
class QToolButton;

class CrawlerThread;
class NotificationWidget;
class ResultsWindow;

class crawler_qt : public QMainWindow {
	Q_OBJECT
	
private:
	enum ActionType {
		QUIT = 0,
		ANALYZE,
		
		MAX_ACTION
	};
	
	std::array<QAction*, MAX_ACTION> m_actions;
	
	info_list_t m_devicesListInfo;
	QComboBox *m_devicesListView;
	
	QListWidget *m_searchList;
	
	QProgressBar *m_progressbar;
	NotificationWidget *m_notificationWidget;
	QToolButton *m_analyzeButton;
	QCheckBox *m_verboseBox;
	
	ResultsWindow *m_resultsWindow;
	
	CrawlerThread *m_thread;
	
	void makeActions();
	void makeMenu();
	void makeMain();
	void makeResultsWindow();
	void place();
	
	void showResult();
	
	void lock();
	void unlock();
	
	void setSearchTerms();
	
	void inform(const QString &message);
	
public slots:
	void analyze();
	void onEndSearch();
	void onThreadError(QString error);
	void verbosity(bool verbose);
	
public:
	crawler_qt();
	virtual ~crawler_qt();
};
