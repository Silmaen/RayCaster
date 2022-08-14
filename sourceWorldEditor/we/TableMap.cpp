/**
 * @file TableMap.cpp
 * @author Silmaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "TableMap.h"
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

namespace we {


TableMap::TableMap(QWidget* parent) :
    QTableWidget(parent) {
}

TableMap::~TableMap() {
}

void TableMap::fillingUp() {

    clear();
    setColumnCount(2);
    setRowCount(0);
    setHorizontalHeaderLabels({"Property", "value"});
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    if (mapLink == nullptr)
        return;
    // index
    int row = 0;
    // First line: map name
    {
        insertRow(row);
        auto* label = new QLabel("Map name:", this);
        auto* edit  = new QLabel(mapName, this);
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        row++;
    }
    // Second line: map width
    {
        insertRow(row);
        auto* label = new QLabel("Map Size X", this);
        auto* edit  = new QLabel(QString::number(mapLink->width()));
        /*auto* edit = new QSpinBox(this);
        edit->setMinimum(8);
        edit->setMaximum(128);
        edit->setSingleStep(2);
        edit->setValue(mapLink->width());*/
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        row++;
    }
    // Third line: map width
    {
        insertRow(row);
        auto* label = new QLabel("Map Size Y", this);
        auto* edit  = new QLabel(QString::number(mapLink->height()));
        /*auto* edit = new QSpinBox(this);
        edit->setMinimum(8);
        edit->setMaximum(128);
        edit->setSingleStep(2);
        edit->setValue(mapLink->height());*/
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        row++;
    }
    // Fourth line: map width
    {
        insertRow(row);
        auto* label = new QLabel("Map Cell Size", this);
        auto* edit  = new QLabel(QString::number(mapLink->getCellSize()));
        /*auto* edit = new QSpinBox(this);
        edit->setMinimum(8);
        edit->setMaximum(96);
        edit->setSingleStep(2);
        edit->setValue(mapLink->getCellSize());*/
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        row++;
    }
    auto [pPos, pDir] = mapLink->getPlayerStart();
    // Fifth line: Player start X
    {
        insertRow(row);
        auto* label = new QLabel("Player Start X", this);
        auto* edit  = new QSpinBox(this);
        edit->setMinimum(0);
        edit->setMaximum(mapLink->fullWidth());
        edit->setSingleStep(mapLink->getCellSize());
        edit->setValue(pPos[0]);
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        connect(edit, SIGNAL(valueChanged(int)), this, SLOT(updatePlayer()));
        row++;
    }
    // Sixth line: Player start Y
    {
        insertRow(row);
        auto* label = new QLabel("Player Start Y", this);
        auto* edit  = new QSpinBox(this);
        edit->setMinimum(0);
        edit->setMaximum(mapLink->fullHeight());
        edit->setSingleStep(mapLink->getCellSize());
        edit->setValue(pPos[1]);
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        connect(edit, SIGNAL(valueChanged(int)), this, SLOT(updatePlayer()));
        row++;
    }
    // Sixth line: Player start angle
    {
        insertRow(row);
        auto* label = new QLabel("Player Start angle", this);
        auto* edit  = new QSpinBox(this);
        edit->setMinimum(0);
        edit->setMaximum(360);
        edit->setSingleStep(15);
        edit->setValue(pDir.getAngle().getUnit(rc::math::Angle::Unit::Degree));
        setCellWidget(row, 0, label);
        setCellWidget(row, 1, edit);
        connect(edit, SIGNAL(valueChanged(int)), this, SLOT(updatePlayer()));
        row++;
    }
    verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void TableMap::updatePlayer() {
    if (rowCount() < 7)
        return;
    rc::math::Vecti pPos{
            (dynamic_cast<QSpinBox*>(cellWidget(4, 1)))->value(),
            (dynamic_cast<QSpinBox*>(cellWidget(5, 1)))->value(),
    };
    rc::math::Vectf pDir{1, 0};
    pDir.rotate({static_cast<double>((dynamic_cast<QSpinBox*>(cellWidget(6, 1)))->value()), rc::math::Angle::Unit::Degree});
    mapLink->setPlayerStart(pPos, pDir);
    modified = true;
}

}// namespace we
