/**
 * @file MainWindow.cpp
 * @author Silmaen
 * @date 09/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "MainWindow.h"
#include "NewMap.h"
#include "core/fs/DataFile.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QTimer>

// QT stuff
#include <QLabel>
#include <QPainter>
#include <QSpinBox>
#include <moc_MainWindow.cpp>
#include <ui/ui_MainWindow.h>

namespace we {

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)) {
    // initialise l’ui depuis le fichier ui.
    ui->setupUi(this);

    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::timedOut));
    timer->setInterval(50);
    timer->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::actNew() {
    NewMap map(this);
    map.setModal(true);
    bool running = true;
    while (running) {
        auto returnCode = map.exec();
        if (returnCode == 0) break;// user has canceled the action
        auto info = map.getInfos();
        // check information
        if (info.name == "") {
            QMessageBox::critical(this,
                                  "Error creating the new map",
                                  "You have no choice: DEFINE A FUCKING MAP NAME!");
            continue;
        }
        mapName = info.name;
        setMap(std::make_shared<rc::core::Map>(info.width, info.height, info.cellSize));
        running = false;
    }
}

void MainWindow::actOpen() {
    rc::core::fs::DataFile fs("maps");
    QString file = QFileDialog::getOpenFileName(this, "Open Map files", QString::fromStdString(fs.getFullPath().string()), "Map files (*.map)", nullptr);
    if (file == "") return;
    auto map = std::make_shared<rc::core::Map>();
    map->loadFromFile(file.toStdString());
    if (!map->isValid())
        return;
    mapName = file.split("/").back().split(".").front();
    filename = file;
    setMap(map);
}

void MainWindow::actSave() {
    theMap->saveToFile(filename.toStdString());
    mapModified = false;
}

void MainWindow::actSaveAs() {
    rc::core::fs::DataFile fs("maps");
    QString file = QFileDialog::getSaveFileName(this, "Save Map file as", QString::fromStdString(fs.getFullPath().string()),"Map files (*.map)");
    if (file == "") return;
    mapName = file.split("/").back().split(".").front();
    filename = file;
    actSave();
}

void MainWindow::actQuit() {
    close();
}

void MainWindow::actChangeMode(){
    // update the checked
    if (currentMode == Mode::Select)
        ui->actionSelect->setChecked(false);
    if (currentMode == Mode::Edit)
        ui->actionQuickEdit->setChecked(false);
    // update the current mode
    if (ui->actionSelect->isChecked())
        currentMode = Mode::Select;
    if (ui->actionQuickEdit->isChecked())
        currentMode = Mode::Edit;
    // update the checked
    if (currentMode == Mode::Select) {
        ui->actionSelect->setChecked(true);
        ui->DrawArea->setCursor(Qt::CursorShape::CrossCursor);
    }
    if (currentMode == Mode::Edit) {
        ui->actionQuickEdit->setChecked(true);
        ui->DrawArea->setCursor(Qt::CursorShape::PointingHandCursor);
    }
}

void MainWindow::actChangePattern(){
    // update the checked
    if (currentPattern == Pattern::Passable)
        ui->actionPassable->setChecked(false);
    if (currentPattern == Pattern::Wall1)
        ui->actionWallType_1->setChecked(false);
    if (currentPattern == Pattern::Wall1b)
        ui->actionWallType_1b->setChecked(false);
    if (currentPattern == Pattern::Wall2)
        ui->actionWallType_2->setChecked(false);
    if (currentPattern == Pattern::Wall2b)
        ui->actionWallType_2b->setChecked(false);
    if (currentPattern == Pattern::Wall3)
        ui->actionWallType_3->setChecked(false);
    if (currentPattern == Pattern::Wall3b)
        ui->actionWallType_3b->setChecked(false);
    // update pattern
    if (ui->actionPassable->isChecked())
        currentPattern = Pattern::Passable;
    if (ui->actionWallType_1->isChecked())
        currentPattern = Pattern::Wall1;
    if (ui->actionWallType_1b->isChecked())
        currentPattern = Pattern::Wall1b;
    if (ui->actionWallType_2->isChecked())
        currentPattern = Pattern::Wall2;
    if (ui->actionWallType_2b->isChecked())
        currentPattern = Pattern::Wall2b;
    if (ui->actionWallType_3->isChecked())
        currentPattern = Pattern::Wall3;
    if (ui->actionWallType_3b->isChecked())
        currentPattern = Pattern::Wall3b;
    // update the checked
    if (currentPattern == Pattern::Passable)
        ui->actionPassable->setChecked(true);
    if (currentPattern == Pattern::Wall1)
        ui->actionWallType_1->setChecked(true);
    if (currentPattern == Pattern::Wall1b)
        ui->actionWallType_1b->setChecked(true);
    if (currentPattern == Pattern::Wall2)
        ui->actionWallType_2->setChecked(true);
    if (currentPattern == Pattern::Wall2b)
        ui->actionWallType_2b->setChecked(true);
    if (currentPattern == Pattern::Wall3)
        ui->actionWallType_3->setChecked(true);
    if (currentPattern == Pattern::Wall3b)
        ui->actionWallType_3b->setChecked(true);
}

void MainWindow::setMap(const std::shared_ptr<rc::core::Map>& map) {
    ui->DrawArea->defineMap(map);
    ui->tableMap->defineMap(map, mapName);
    theMap = map;
}

void MainWindow::paintEvent(QPaintEvent*) {
    ui->tableMap->fillingUp();
    ui->tableCell->fillingUp();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* /*event*/) {
    auto [mousePos,mouseCell] = ui->DrawArea->getMouseInfo();
    if (mousePos.x()<0)return; // nothing to do
    if (currentMode == Mode::Select){
        ui->tableCell->setCell(mouseCell);
    }else if (currentMode == Mode::Edit){
        ui->tableCell->setCell(mouseCell);
        auto& cell = theMap->at({static_cast<unsigned char>(mouseCell.x()),static_cast<unsigned char>(mouseCell.y())});
        switch (currentPattern) {
        case Pattern::Passable:
            cell.passable = true;
            cell.visibility = true;
            cell.textureId = 0;
            break;
        case Pattern::Wall1:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b10;
            break;
        case Pattern::Wall1b:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b11;
            break;
        case Pattern::Wall2:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b100;
            break;
        case Pattern::Wall2b:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b101;
            break;
        case Pattern::Wall3:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b110;
            break;
        case Pattern::Wall3b:
            cell.passable = false;
            cell.visibility = false;
            cell.textureId = 0b111;
            break;
        }
        mapModified = true;
        ui->DrawArea->repaint();
    }
}

void MainWindow::timedOut() {
    ui->actionSave->setEnabled(filename != "");

    QString title= "RayCaster Map editor";
    if (!mapName.isEmpty())
        title += " - " + mapName;
    if (mapModified)
        title += "*";
    this->setWindowTitle(title);

    auto [mousePos,mouseCell] = ui->DrawArea->getMouseInfo();
    if (mousePos.x() < 0){
        ui->statusBar->showMessage("Mouse outside Map");
    }else{
        QString message = "Mouse position in map: " + QString::number(mousePos.x()) + " x " + QString::number(mousePos.y()) +
                          " // Cell: " + QString::number(mouseCell.x()) + " x " + QString::number(mouseCell.y());
        ui->statusBar->showMessage(message);
    }
    if (ui->tableMap->checkModification()){
        mapModified = true;
        ui->DrawArea->repaint();
    }
}


}// namespace we
