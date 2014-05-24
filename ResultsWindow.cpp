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
#include "SearchResultItem.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

#include <utility>

static const uint8_t VIEW_SIZE = 26;

ResultsWindow::ResultsWindow(): QWidget()
{
	this->makeMain();
}

void ResultsWindow::view(QListWidgetItem *item) {
	SearchResultItem *current;
	if (item == nullptr) {
		auto selected = this->m_list->selectedItems();
		if (selected.empty()) {
			return;
		} 
		current = (SearchResultItem*)selected.front();
	} else {
		current = (SearchResultItem*)item;
	}
	
	this->m_hex->setText(current->array, current->offset);
}

void ResultsWindow::setResults(search_terms_t &&patterns, SignatureWalker::results_t &&new_results) {
	this->clear();
	this->m_patterns = std::move(patterns);
	this->m_results = std::move(new_results);
}

void ResultsWindow::show() {
	this->formList();
	this->activateWindow();
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
	this->m_hex->clear();
}

void ResultsWindow::formList() {
	this->m_list->clear();
	for (const auto &result : this->m_results) {
		ByteReader *reader = result.first;
		reader->reset();
		for (const auto &offset : result.second) {
			size_t to_read = VIEW_SIZE * 3;
			Buffer buffer(to_read + 4);
			size_t startp = 0;
			if (offset.offset > to_read / 2) {
				startp = offset.offset - to_read / 2;
			}
			reader->seekg(startp);
			auto read = reader->read(buffer, to_read);
			buffer.shrink(read);
			buffer.begin()[read] = 0;

			bool encoded = utility::sanitize(buffer, this->m_patterns[offset.pattern_n].encoding);
			if (not encoded) {
				logger()->debug("Encodings for pattern %u mismatches: %s for current and %s for environment", 
					offset.pattern_n, this->m_patterns[offset.pattern_n].encoding.c_str(), this->m_patterns.crbegin()->encoding.c_str());
				buffer.clear();
				std::generate(buffer.begin(), buffer.end(), [] () {return (uint8_t)'.';});
			}
			
			Buffer pattern(this->m_patterns[offset.pattern_n].pattern.size() + 1);
			pattern.capture(this->m_patterns[offset.pattern_n].pattern.c_str(), this->m_patterns[offset.pattern_n].pattern.size() + 1);
			encoded = utility::sanitize(pattern, this->m_patterns[offset.pattern_n].encoding, '.');
			if (not encoded) {
				logger()->error("Encodings for pattern %u mismatches: %s for current and %s for environment", 
					offset.pattern_n, this->m_patterns[offset.pattern_n].encoding.c_str(), this->m_patterns.crbegin()->encoding.c_str());
			}
			
			auto item = new SearchResultItem(this->m_list, QString((const char*)pattern.cbegin()), std::move(buffer), offset.offset, reader);
			this->m_list->addItem(item);
		}
	}
}

#include "ResultsWindow.moc"
