/**
 * @file TableCell.cpp
 * @author argawaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "TableCell.h"
#include <QHeaderView>
#include <QLabel>
#include <QPaintEvent>

namespace we {


TableCell::TableCell(QWidget* parent) :
    QTableWidget(parent) {
}

TableCell::~TableCell() {
}

void TableCell::fillingUp() {
    clear();
    setColumnCount(2);
    setRowCount(0);
    setHorizontalHeaderLabels({"Property", "value"});
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
    if (currentCell.x()<0) return;
    int row = 0;
    // First line: cell index
    {
        insertRow(row);
        auto* label = new QLabel("Cell Location", this);
        auto* edit = new QLabel(QString::number(currentCell.x())+" "+QString::number(currentCell.y()), this);
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        row++;
    }
}


}// namespace we
