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

#include "EncodingsWindow.h"

#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>

static const uint8_t BUTTON_SIZE = 180;

static const QString DEFAULT_ENCODINGS[] = {
	"UTF-8",
	"CP1251",
	"KOI8-R",
};

EncodingsWindow::EncodingsWindow(): QWidget() {
	this->setWindowTitle(tr("Disk Crawler: Encodings"));
	
	auto layout = new QVBoxLayout(this);
	this->setLayout(layout);
		
	auto upperLayout = new QHBoxLayout(this);
	auto inputLayout = new QVBoxLayout(this);
	this->m_addField = new QComboBox(this);
	for (auto &enc : DEFAULT_ENCODINGS) {
		this->m_addField->addItem(enc);
	}
	this->m_addField->setEditable(true);
	connect(this->m_addField->lineEdit(), SIGNAL(returnPressed()), SLOT(add()));
	inputLayout->addWidget(this->m_addField);
	
	this->m_items = new QListWidget(this);
	this->m_items->setSelectionMode(QAbstractItemView::ExtendedSelection);
	inputLayout->addWidget(this->m_items);
	
	auto buttonsLayout = new QVBoxLayout(this);
	
	auto addEncButton = new QPushButton(tr("Add"), this);
	addEncButton->setMinimumWidth(BUTTON_SIZE);
	connect(addEncButton, SIGNAL(clicked(bool)), SLOT(add()));
	buttonsLayout->addWidget(addEncButton, 0, Qt::AlignTop);
	
	auto remvoveEncButton = new QPushButton(tr("Remove"), this);
	connect(remvoveEncButton, SIGNAL(clicked(bool)), SLOT(remove()));
	remvoveEncButton->setMinimumWidth(BUTTON_SIZE);
	buttonsLayout->addWidget(remvoveEncButton, 0, Qt::AlignTop);
	
	buttonsLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	
	upperLayout->addItem(inputLayout);
	upperLayout->addItem(buttonsLayout);
	
	
	auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	buttons->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	buttons->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
	connect(buttons, SIGNAL(accepted()), SLOT(complete()));
	connect(buttons, SIGNAL(rejected()), SLOT(reject()));
	
	layout->addItem(upperLayout);
	layout->addWidget(buttons, 0, Qt::AlignRight);
}

void EncodingsWindow::show() {
	this->backup();
	this->activateWindow();
	this->QWidget::show();
}

void EncodingsWindow::hide() {
	this->QWidget::hide();
	this->modified = false;
}

void EncodingsWindow::clear() {
	this->m_items->clear();
}

void EncodingsWindow::complete() {
	this->hide();
	this->m_oldItems.clear();
	emit added();
}

void EncodingsWindow::reject() {
	this->hide();
	this->restore();
}

void EncodingsWindow::backup() {
	for (auto i = 0; i < this->m_items->count(); i++) {
		this->m_oldItems.push_back(this->m_items->item(i)->text());
	}
}

void EncodingsWindow::restore() {
	this->m_items->clear();
	for (const auto &item : this->m_oldItems) {
		this->m_items->addItem(item);
	}
}

void EncodingsWindow::add() {
	auto text = this->m_addField->currentText();
	if (not text.isEmpty()) {
		this->m_items->addItem(text);
		this->modified = true;
	}
}

void EncodingsWindow::remove() {
	auto to_remove = this->m_items->selectedItems();
	if (not to_remove.isEmpty()) {
		qDeleteAll(to_remove);
		this->modified = true;
	}
}

encodings_t EncodingsWindow::get() const {
	encodings_t encodings;
	for (int i = 0; i < this->m_items->count(); i++) {
		auto encoding = this->m_items->item(i)->text();
		if (not encoding.isEmpty()) {
			encodings.push_back(encoding.toStdString());
		}
	}
	return encodings;
}

void EncodingsWindow::closeEvent(QCloseEvent *event) {
	if (this->modified) {
		// TODO: Save something
		this->reject();
	}
	QWidget::closeEvent(event);
}

#include "EncodingsWindow.moc"
