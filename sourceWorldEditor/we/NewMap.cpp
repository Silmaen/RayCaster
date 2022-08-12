/**
 * @file NewMap.cpp
 * @author Silmaen
 * @date 09/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "NewMap.h"

// QT stuff
#include <moc_NewMap.cpp>
#include <ui/ui_NewMap.h>

namespace we {

NewMap::NewMap(QWidget* parent):
    QDialog(parent),ui(new Ui::NewMap) {
    ui->setupUi(this);
}

NewMap::~NewMap() {
    delete ui;
}

const NewMap::Infos NewMap::getInfos() const {
    return {ui->EditName->text(), static_cast<uint8_t>(ui->EditWidth->value()),static_cast<uint8_t>(ui->EditHeight->value()), static_cast<uint8_t>(ui->EditCellSize->value())};
}

void NewMap::setInfos(const NewMap::Infos& infos) {
    ui->EditName->setText(infos.name);
    ui->EditWidth->setValue(infos.width);
    ui->EditHeight->setValue(infos.height);
    ui->EditCellSize->setValue(infos.cellSize);
}


}// namespace we
