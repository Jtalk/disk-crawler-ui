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

#include "DiskListWidgetItem.h"
#include "NotificationWidget.h"

#include "devpick.h"

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

static QString humanReadable(size_t size) {
	int i = 0;
	const char* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	while (size > 1024) {
		size /= 1024;
		i++;
	}
	return QString::number(size) + " " + units[i];
}

crawler_qt::crawler_qt() {
	this->place();
	this->makeActions();
	this->makeMenu();
	this->makeMain();
}

crawler_qt::~crawler_qt()
{}

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
	connect(this->m_actions[QUIT], SIGNAL(triggered()), SLOT(close()));
	
	this->m_actions[ANALYZE] = new QAction(tr("Analyze"), this);
	connect(this->m_actions[ANALYZE], SIGNAL(triggered()), SLOT(analyze()));
}

void crawler_qt::makeMenu() {
	auto menubar = this->menuBar();
	auto file = new QMenu(tr("&File"), menubar);
	file->addAction(this->m_actions[QUIT]);
	menubar->addMenu(file);
}

void crawler_qt::makeMain() {
	this->setWindowTitle(tr("Disk Crawler"));
	
	auto mainWidget = new QWidget(this);
	auto layout = new QVBoxLayout(mainWidget);
	mainWidget->setLayout(layout);
	this->setCentralWidget(mainWidget);
	
	this->m_notificationWidget = new NotificationWidget(mainWidget);
	layout->addWidget(this->m_notificationWidget);
	
	auto upperLayout = new QHBoxLayout(mainWidget);
	
	this->m_devicesList = new QListWidget(mainWidget);
	auto list = devpick();
	for (auto &device : list) {
		QLatin1String device_name = QLatin1String(device.name.c_str());
		QString content = device_name + ", size: " + humanReadable(device.size) + ", " + QLatin1String(device.file_system.c_str());
		auto current = new DiskListWidgetItem(content, this->m_devicesList, device_name);
		this->m_devicesList->addItem(current);
	}
	connect(this->m_devicesList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(analyze(QListWidgetItem*)));
	upperLayout->addItem(new QWidgetItem(this->m_devicesList));
	
	auto buttonsLayout = new QVBoxLayout(mainWidget);
	auto analyzeButton = new QToolButton(mainWidget);
	analyzeButton->setDefaultAction(this->m_actions[ANALYZE]);
	buttonsLayout->addWidget(analyzeButton, 0, Qt::AlignTop);
	upperLayout->addItem(buttonsLayout);
	
	layout->addItem(upperLayout);
}

void crawler_qt::inform(const QString &message) {
	this->m_notificationWidget->notify(message);
}

void crawler_qt::analyze(QListWidgetItem *chosen) {
	DiskListWidgetItem *diskSelected;
	if (chosen == nullptr) {
		auto selected = this->m_devicesList->selectedItems();
		if (selected.empty()) {
			this->inform(tr("No device selected"));
			return;
		} 
		diskSelected = (DiskListWidgetItem*)selected.front();
	} else {
		diskSelected = (DiskListWidgetItem*)chosen;
	}
	this->inform(QString("Device %1 is selected").arg(diskSelected->device_name));	
}

#include "crawler-qt.moc"

