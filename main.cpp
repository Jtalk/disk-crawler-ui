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

#include "base/Config.h"
#include "base/Log.h"

#include <clocale>

#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtGui/QApplication>

QTranslator translator;
	
void translate(QApplication &app) {
	translator.load("crawler-qt-" + QLocale::system().name());
	app.installTranslator(&translator);
}

void encode() {
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
}

int main(int argc, char **argv) {
	QApplication app(argc, argv); 
	encode();
	translate(app);
	
	Log logger;
	Config config;
	
	crawler_qt foo;
	
	foo.show();
	
	return app.exec();
}
