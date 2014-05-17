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

#include "AddPatternWindow.h"

#include <QtGui/QLineEdit>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

static const QString encodings[] = {
	"UTF-8",
	"CP1251",
	"KOI8-R",
};

AddPatternWindow::AddPatternWindow(): QWidget() {
	auto inputLabel = new QLabel(tr("Search term:"), this);
	this->m_input = new QLineEdit(this);
	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	
	auto encodingLabel = new QLabel(tr("Encoding:"), this);
	this->m_encoding = new QComboBox(this);
	this->m_encoding->setEditable(true);
	for (const auto &encoding : encodings) {
		this->m_encoding->addItem(encoding);
	}
	
	auto mainLayout = new QVBoxLayout(this);
	auto buttonsLayout = new QHBoxLayout(this);
	
	buttonsLayout->addWidget(buttons, 0, Qt::AlignRight);
	mainLayout->addWidget(inputLabel);
	mainLayout->addWidget(this->m_input);
	mainLayout->addWidget(encodingLabel);
	mainLayout->addWidget(this->m_encoding);
	mainLayout->addItem(buttonsLayout);
	
	connect(buttons, SIGNAL(accepted()), SLOT(complete()));
	connect(buttons, SIGNAL(rejected()), SLOT(rejected()));
	connect(this->m_input, SIGNAL(returnPressed()), SLOT(complete()));
	
	this->setLayout(mainLayout);
}

void AddPatternWindow::resize(const QSize &size) {
	this->QWidget::resize(size);
}

void AddPatternWindow::show() {
	this->m_input->clear();
	this->QWidget::show();
}

void AddPatternWindow::complete() {
	auto text = this->m_input->displayText();
	if (not text.isEmpty()) {
		emit finished({text});
	}
	this->rejected();
}

void AddPatternWindow::rejected() {
	this->hide();
}

#include "AddPatternWindow.moc"
