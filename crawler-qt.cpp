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

#include "AddPatternWindow.h"
#include "CrawlerThread.h"
#include "SearchListWidgetItem.h"
#include "NotificationWidget.h"
#include "ResultsWindow.h"

#include "base/Config.h"

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>
#include <QtGui/QBoxLayout>
#include <QtGui/QProgressBar>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>

#include <iterator>

static const uint8_t BUTTON_SIZE = 180;

static QString humanReadable(size_t size) {
	int i = 0;
	const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	while (size > 1024) {
		size /= 1024;
		i++;
	}
	return QString::number(size) + " " + units[i];
}

crawler_qt::crawler_qt(): m_thread(nullptr) {
	this->place();
	this->makeActions();
	this->makeMenu();
	this->makeMain();
	this->makeResultsWindow();
	this->makeAddWindow();
}

crawler_qt::~crawler_qt()
{
	this->m_thread->wait();
	delete this->m_thread;
	delete this->m_resultsWindow;
	delete this->m_addWindow;
}

void crawler_qt::place() {
	QDesktopWidget *d = QApplication::desktop();
	int ws = d->width();   // returns screen width
	int h = d->height();  // returns screen height
	QSize size(ws / 2, h / 2);
	int mw = size.width();
	int mh = size.height();
	int cw = (ws/2) - (mw/2);
	int ch = (h/2) - (mh/2);
	this->resize(size);
	this->move(cw,ch);
}

void crawler_qt::makeActions() {
	this->m_actions.fill(nullptr);
	
	this->m_actions[QUIT] = new QAction(tr("Quit"), this);
	this->m_actions[QUIT]->setShortcut(QString("Ctrl+Q"));
	connect(this->m_actions[QUIT], SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	
	this->m_actions[ANALYZE] = new QAction(tr("Anal&yze"), this);
	this->m_actions[ANALYZE]->setShortcut(QString("Ctrl+Y"));
	connect(this->m_actions[ANALYZE], SIGNAL(triggered()), SLOT(analyze()));
	
	this->m_actions[ADD_PATTERN] = new QAction(tr("&Add"), this);
	this->m_actions[ADD_PATTERN]->setShortcut(QString("Ctrl+A"));
	connect(this->m_actions[ADD_PATTERN], SIGNAL(triggered()), SLOT(showAddPattern()));
	
	this->m_actions[REMOVE_PATTERN] = new QAction(tr("&Remove"), this);
	this->m_actions[REMOVE_PATTERN]->setShortcut(QString("Ctrl+R"));
	connect(this->m_actions[REMOVE_PATTERN], SIGNAL(triggered()), SLOT(removePattern()));
}

void crawler_qt::makeMenu() {
	auto menubar = this->menuBar();
	auto file = new QMenu(tr("&File"), menubar);
	file->addAction(this->m_actions[QUIT]);
	menubar->addMenu(file);
	
	menubar->setVisible(false);
}

void crawler_qt::makeMain() {
	this->setWindowTitle(tr("Disk Crawler"));
	this->addAction(this->m_actions[QUIT]);
	
	auto mainWidget = new QWidget(this);
	auto layout = new QVBoxLayout(mainWidget);
	mainWidget->setLayout(layout);
	this->setCentralWidget(mainWidget);
	
	this->m_notificationWidget = new NotificationWidget(mainWidget);
	layout->addWidget(this->m_notificationWidget);
	
	auto upperLayout = new QHBoxLayout(mainWidget);
	
	this->m_devicesListView = new QComboBox(mainWidget);
	this->m_devicesListInfo = devpick();
	for (auto &device : this->m_devicesListInfo) {
		QString content = QLatin1String(device.name.c_str()) + ", size: " + humanReadable(device.size) + ", " + QLatin1String(device.file_system.c_str());
		this->m_devicesListView->addItem(content);
	}
	layout->addWidget(this->m_devicesListView);
	
	this->m_searchList = new QListWidget(mainWidget);
	this->m_searchList->setSelectionMode(QListWidget::ExtendedSelection);
	upperLayout->addWidget(this->m_searchList);
	
	auto buttonsLayout = new QVBoxLayout(mainWidget);
	
	auto addPatternButton = new QToolButton(mainWidget);
	addPatternButton->setDefaultAction(this->m_actions[ADD_PATTERN]);
	addPatternButton->setMinimumWidth(BUTTON_SIZE);
	buttonsLayout->addWidget(addPatternButton, 0, Qt::AlignTop);
	
	auto remvovePatternButton = new QToolButton(mainWidget);
	remvovePatternButton->setDefaultAction(this->m_actions[REMOVE_PATTERN]);
	remvovePatternButton->setMinimumWidth(BUTTON_SIZE);
	buttonsLayout->addWidget(remvovePatternButton, 0, Qt::AlignTop);
	
	buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	
	this->m_analyzeButton = new QToolButton(mainWidget);
	this->m_analyzeButton->setDefaultAction(this->m_actions[ANALYZE]);
	this->m_analyzeButton->setMinimumWidth(BUTTON_SIZE);
	this->m_analyzeButton->setMinimumHeight(this->m_analyzeButton->height() * 2);
	buttonsLayout->addWidget(this->m_analyzeButton, 0, Qt::AlignBottom);
	
	this->m_verboseBox = new QCheckBox(tr("Verbose logging"), mainWidget);
	connect(this->m_verboseBox, SIGNAL(clicked(bool)), SLOT(verbosity(bool)));
	buttonsLayout->addWidget(this->m_verboseBox);
	
	upperLayout->addItem(buttonsLayout);
	
	layout->addItem(upperLayout);
	
	this->m_progressbar = new QProgressBar(mainWidget);
	this->m_progressbar->setVisible(false);
	this->m_progressbar->setMaximum(100);
	this->m_progressbar->setMinimum(0);
	
	this->m_thread = new CrawlerThread(this);
	connect(this->m_thread, SIGNAL(progress(int)), this->m_progressbar, SLOT(setValue(int)));
	connect(this->m_thread, SIGNAL(error(QString)), SLOT(onThreadError(QString)));
	connect(this->m_thread, SIGNAL(endsearch()), SLOT(onEndSearch()));
	
	layout->addWidget(this->m_progressbar);
}

void crawler_qt::makeResultsWindow() {
	this->m_resultsWindow = new ResultsWindow();
	this->m_resultsWindow->hide();
	auto geom = this->geometry();
	this->m_resultsWindow->setGeometry(geom);
	this->m_resultsWindow->addAction(this->m_actions[QUIT]);
}

void crawler_qt::makeAddWindow() {
	this->m_addWindow = new AddPatternWindow();
	this->m_addWindow->addAction(this->m_actions[QUIT]);
	connect(this->m_addWindow, SIGNAL(finished(AddPatternWindow::Result)), SLOT(addPattern(AddPatternWindow::Result)));
	this->m_addWindow->hide();
	this->m_addWindowSize = this->m_addWindow->size();
	this->m_addWindowSize.setWidth(this->m_addWindowSize.width() * 1.5);
	this->m_addWindowSize.setHeight(1);
}

void crawler_qt::showResult() {
	this->m_resultsWindow->setResults(std::move(this->m_thread->patterns()), std::move(this->m_thread->found()));
	this->m_resultsWindow->setWindowTitle(this->m_thread->device().name.c_str());
	this->m_resultsWindow->show();
}

void crawler_qt::inform(const QString &message) {
	this->m_notificationWidget->notify(message);
}

void crawler_qt::lock() {
	this->m_analyzeButton->setEnabled(false);
	this->m_verboseBox->setEnabled(false);
}

void crawler_qt::unlock() {
	this->m_analyzeButton->setEnabled(true);
	this->m_verboseBox->setEnabled(true);
}

void crawler_qt::analyze() {
	int index = this->m_devicesListView->currentIndex();
	if (index < 0) {
		this->inform(tr("No device selected"));
		return;
	}
	auto &disk = *std::next(this->m_devicesListInfo.cbegin(), index);
	this->lock();
	logger()->debug("Device %s is selected", disk.name.c_str());
	
	this->setSearchTerms();
	
	this->m_progressbar->setVisible(true);
	this->m_progressbar->setValue(0);
	
	this->m_thread->addDevice(disk);
	this->m_thread->start();
}

void crawler_qt::setSearchTerms() {
	search_terms_t terms;
	for (int i = 0; i < this->m_searchList->count(); i++) {
		auto item = (SearchListWidgetItem*)this->m_searchList->item(i);
		terms.push_back({item->pattern, {}});
	}
	this->m_thread->addPatterns(std::move(terms));	
}

void crawler_qt::verbosity(bool verbose) {
	config()->VERBOSE = verbose;
}

void crawler_qt::onEndSearch() {
	logger()->debug("Showing results");
	this->m_thread->wait();
	this->unlock();
	this->showResult();
}

void crawler_qt::onThreadError(QString error) {
	this->inform(error);
	this->m_thread->wait();
	this->unlock();
	this->m_progressbar->setVisible(false);
}

void crawler_qt::showAddPattern() {
	QSize size = this->m_addWindowSize;
	
	int ws = this->width();
	int h = this->height();
	int mw = size.width();
	int mh = size.height();
	int cw = (ws/2) - (mw/2);
	int ch = (h/2) - (mh/2);
	this->m_addWindow->resize(size);
	this->m_addWindow->move(this->geometry().x() + cw, this->geometry().y() + ch);
	this->m_addWindow->show();
	this->m_addWindow->activateWindow();	
}

void crawler_qt::removePattern() {
	qDeleteAll(this->m_searchList->selectedItems());
}

void crawler_qt::addPattern(const AddPatternWindow::Result &result) {
	auto item = new SearchListWidgetItem(result.pattern, this->m_searchList, byte_array_t((uint8_t*)result.pattern.toStdString().c_str()));
	this->m_searchList->addItem(item);
}

#include "crawler-qt.moc"

