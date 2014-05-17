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

#include "AddPatternWindow.h"

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

class AddPatternWindow;
class CrawlerThread;
class NotificationWidget;
class ResultsWindow;
class EncodingsWindow;

class crawler_qt : public QMainWindow {
	Q_OBJECT
	
private:
	enum ActionType {
		QUIT = 0,
		ANALYZE,
		ADD_PATTERN,
		REMOVE_PATTERN,
		SHOW_ENCODINGS,
		
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
	AddPatternWindow *m_addWindow;
	EncodingsWindow *m_encodingsWindow;
	QSize m_addWindowSize;
	
	CrawlerThread *m_thread;
	
	void makeActions();
	void makeMenu();
	void makeMain();
	void makeResultsWindow();
	void makeAddWindow();
	void makeEncodingsWindow();
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
	void showAddPattern();
	void showEncodings();
	void removePattern();
	void addPattern(const AddPatternWindow::Result &result);
	void addEncodings();
	
public:
	crawler_qt();
	virtual ~crawler_qt();
};
