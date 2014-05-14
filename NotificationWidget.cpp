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

#include "NotificationWidget.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QStyle>

static const uint8_t LABEL_TEXT_INDENT = 10;
static const QColor WARNING_BACKGROUND_COLOUR = QColor(0xff, 0x5e, 0x5e);

NotificationWidget::NotificationWidget(QWidget *parent):
	QWidget(parent) {	
	this->m_label = new QLabel(this);
	this->m_label->setIndent(LABEL_TEXT_INDENT);
	this->m_hideButton = new QToolButton(this);
	auto buttonIcon = this->m_hideButton->style()->standardIcon(QStyle::SP_TitleBarCloseButton);
	this->m_hideButton->setIcon(buttonIcon);
	this->hide();
	connect(this->m_hideButton, SIGNAL(clicked(bool)), SLOT(hide()));
	
	auto layout = new QHBoxLayout(this);
	layout->addWidget(this->m_label, 100);
	layout->addWidget(this->m_hideButton, 0, Qt::AlignRight);
	this->setLayout(layout);
	
	this->paintColour();
}

void NotificationWidget::notify(const QString &text) {
	this->m_label->setText(text);
	this->show();
}

void NotificationWidget::hide() {
	this->m_label->setVisible(false);
	this->m_hideButton->setVisible(false);
	this->setAutoFillBackground(false);
}

void NotificationWidget::show() {
	this->setAutoFillBackground(true);
	this->m_label->setVisible(true);
	this->m_hideButton->setVisible(true);
}

void NotificationWidget::paintColour() {
	this->m_defaultPalette = this->palette();
	this->m_palette = this->m_defaultPalette;
	this->m_palette.setColor(QPalette::Background, WARNING_BACKGROUND_COLOUR);
	
	this->m_label->setAutoFillBackground(true);
	this->m_hideButton->setAutoFillBackground(true);
	
	this->m_label->setPalette(this->m_palette);
	this->m_hideButton->setPalette(this->m_palette);
	this->setPalette(this->m_palette);
}

#include "NotificationWidget.moc"
