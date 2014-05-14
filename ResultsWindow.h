/*
    Disk Crawler library.
    Copyright (C) 2013  Jtalk <me@jtalk.me>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "base/SignatureWalker.h"

#include <QtGui/QWidget>

class QListWidget;

class HexWidget;

class ResultsWindow : public QWidget {
	Q_OBJECT

public:
	ResultsWindow();

	void setResults(SignatureWalker::results_t &&new_results);
	void show();
	void hide();
	
private:
	QListWidget *m_list;
	HexWidget *m_hex;
	
	SignatureWalker::results_t m_results;
	
	void makeMain();
	
	void clear();
	void formList();
};