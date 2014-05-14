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
#include "base/SignatureWalker.h"

#include <QtCore/QThread>

class CrawlerThread : public QThread {
	Q_OBJECT

public:
	CrawlerThread(QObject *parent);
	virtual ~CrawlerThread();
	
	void addDevice(const DeviceInfo &info);
	SignatureWalker::results_t &found();
	
signals:
	void progress(int percent);
	void endsearch();
	void error(QString text);
	
private:
	SignatureWalker::results_t m_results;
	
	DeviceInfo device;
	bool initialized;
	
	void run() override;
	
	void progressCallback(int percent);
	
	void find();
};