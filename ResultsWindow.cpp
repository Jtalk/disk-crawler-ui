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

#include "ResultsWindow.h"

#include "HexWidget.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

#include <utility>

ResultsWindow::ResultsWindow(): QWidget()
{
	this->makeMain();
}

void ResultsWindow::view(QListWidgetItem *item) {
	QListWidgetItem *current;
	if (item == nullptr) {
		auto selected = this->m_list->selectedItems();
		if (selected.empty()) {
			return;
		} 
		current = selected.front();
	} else {
		current = item;
	}
	
	// this->m_hex->setText();
}

void ResultsWindow::setResults(SignatureWalker::results_t && new_results) {
	this->clear();
	this->m_results = std::move(new_results);
}

void ResultsWindow::show() {
	this->formList();
	this->QWidget::show();
}

void ResultsWindow::hide() {
	this->QWidget::hide();
	this->m_list->clear();
}

void ResultsWindow::makeMain() {
	auto layout = new QVBoxLayout(this);
	this->setLayout(layout);
	
	this->m_hex = new HexWidget(this);
	
	auto upperLayout = new QHBoxLayout(this);
	
	this->m_list = new QListWidget(this);
	connect(this->m_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(view(QListWidgetItem*)));
	upperLayout->addWidget(this->m_list);
	
	auto buttonsLayout = new QVBoxLayout(this);
	auto viewButton = new QPushButton(tr("Show"), this);
	connect(viewButton, SIGNAL(clicked(bool)), SLOT(view()));
	buttonsLayout->addWidget(viewButton, 0, Qt::AlignTop);
	upperLayout->addItem(buttonsLayout);
	//upperLayout->setStretchFactor(6);
	
	layout->addItem(upperLayout);
	layout->addWidget(this->m_hex, 4);
}

void ResultsWindow::clear() {
	for (auto &result : this->m_results) {
		delete result.first;
	}
	this->m_results.clear();
}

void ResultsWindow::formList() {

}

#include "ResultsWindow.moc"
