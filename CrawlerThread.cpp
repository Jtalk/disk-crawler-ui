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

#include "CrawlerThread.h"

#include "base/FSWalker.h"
#include "base/PlainWalker.h"

#include <functional>

using std::bind;
using namespace std::placeholders;

CrawlerThread::CrawlerThread(QObject *parent): QThread(parent), initialized(false)
{}

CrawlerThread::~CrawlerThread() 
{}

void CrawlerThread::addDevice(const DeviceInfo &info) {
	this->device = info;
	this->initialized = true;
}

void CrawlerThread::run() {
	if (not this->initialized) {
		emit error(tr("Running uninitialized thread, try again"));
		return;
	}
	
	this->find();	
}

void CrawlerThread::progressCallback(int percent) {
	emit progress(percent);
}

void CrawlerThread::find() {
	auto plain = new PlainWalker(this->device.name, this->device.size, bind(&CrawlerThread::progressCallback, this, _1));
	if (not *plain) {
		delete plain;
		emit error(tr("Access Denied"));
		return;
	}
	auto results = plain->find((uint8_t*)"abc");
	this->progressCallback(100);
}

#include "CrawlerThread.moc"
