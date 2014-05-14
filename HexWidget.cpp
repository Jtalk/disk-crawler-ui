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

#include "HexWidget.h"

#include <cmath>

QString format(const byte_array_t &text, size_t startOffset) {
	static constexpr uint8_t ITEMS_PER_ROW = 16;
	static const char *PLACEHOLDER_HEX = "  ";
	static const char *PLACEHOLDER_VIEW = " ";
	const uint8_t ADDRESS_SIZE = (text.length() + startOffset) / 0x10 + 2;
	
	QStringList addresses;
	QStringList hexes;
	QStringList visuals;
	
	size_t startAddress = ITEMS_PER_ROW * (startOffset / ITEMS_PER_ROW);
	size_t lastAddress = ITEMS_PER_ROW * ceil(float(startOffset + text.length()) / ITEMS_PER_ROW);
	
	for (size_t addr = startAddress; addr <= lastAddress; addr++) {
		QString straddr = "0x" + QString::number(addr, 16).rightJustified(ADDRESS_SIZE, '0');
		addresses << straddr;
		
		QString hex;
		QString visual;
		for (size_t charidx = addr; charidx < addr + ITEMS_PER_ROW; charidx++) {
			QString item_hex;
			QString item_view;
			if (charidx < startOffset or charidx >= startOffset + text.length()) {
				item_hex = PLACEHOLDER_HEX;
				item_view = PLACEHOLDER_VIEW;
			} else {
				size_t offset = charidx - startOffset;
				item_hex = QString::number(text[offset], 16);
				QChar value(text[offset]);
				if (not value.isDigit() and not value.isLetter() and value != ' ') {
					value = '.';
				}
				item_view = value;
			}
			if (not hex.isEmpty()) {
				hex += QLatin1String(" ");
			}
			hex += item_hex;
			visual += item_view;
		}
		hexes << hex;
		visuals << visual;		
	}
	
	QString result;
	auto addri = addresses.constBegin();
	auto hexi = hexes.constBegin();
	auto visuali = visuals.constBegin();
	for (; addri != addresses.constEnd() and hexi != hexes.constEnd() and visuali != visuals.constEnd(); addri++, hexi++, visuali++) {
		if (not result.isEmpty()) {
			result += "\n";
		}
		result += (*addri + " | " + *hexi + " | " + *visuali);
	}
	
	return result;
}

HexWidget::HexWidget(QWidget *parent): QTextEdit(parent) 
{
	this->setReadOnly(true);
	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	this->setFont(font);
}

void HexWidget::setText(const byte_array_t &text, size_t startOffset) {
	QString formatted = format(text, startOffset);
	this->QTextEdit::setText(formatted);
}

#include "HexWidget.moc"
