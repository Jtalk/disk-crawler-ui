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

CrawlerThread::CrawlerThread(QObject *parent): QThread(parent), m_initialized(false)
{}

CrawlerThread::~CrawlerThread() 
{}

void CrawlerThread::addDevice(const DeviceInfo &info) {
	this->m_device = info;
	this->m_initialized = true;
}

SignatureWalker::results_t &CrawlerThread::found() {
	return this->m_results;
}

const DeviceInfo &CrawlerThread::device() const {
	return this->m_device;
}

void CrawlerThread::run() {
	if (not this->m_initialized) {
		emit error(tr("Running uninitialized thread, try again"));
		return;
	}
	
	for (auto &result : this->m_results) {
		delete result.first;
	}
	this->m_results.clear();
	
	this->find();	
}

void CrawlerThread::progressCallback(int percent) {
	emit progress(percent);
}

void CrawlerThread::find() {
	auto plain = new PlainWalker(this->m_device.name, this->m_device.size, bind(&CrawlerThread::progressCallback, this, _1));
	if (not *plain) {
		delete plain;
		emit error(tr("Access Denied"));
		return;
	}
	this->m_results.splice(this->m_results.end(), plain->find((uint8_t*)"main"));
	
// 	auto checker = utility::walker(this->device.file_system);
// 	if (checker == nullptr or not *checker) {
// 		delete checker;
// 		
// 	}
	
	emit endsearch();
}

#include "CrawlerThread.moc"
