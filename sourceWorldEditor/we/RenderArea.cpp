/**
 * @file RenderArea.cpp
 * @author Silmaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "RenderArea.h"
#include <QMouseEvent>
#include <QPainter>
#include <iostream>

namespace we {

RenderArea::RenderArea(QWidget* parent) :
    QWidget(parent) {
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

RenderArea::~RenderArea() {
}


QPoint RenderArea::mapCoordToRectCoord(const QPointF& mapPoint) const {
    if (mapLink == nullptr)
        return {-1, -1};
    if (!mapLink->isValid())
        return {-1, -1};
    if (!mapLink->isIn(rc::math::geometry::Vectf {mapPoint.x(), mapPoint.y()}))
        return {-1, -1};
    const QRect drawArea = getMapRect();
    return QPoint{
        static_cast<int>(drawArea.x() + (mapPoint.x() * drawArea.width() / mapLink->fullWidth() )),
        static_cast<int>(drawArea.y() + (mapPoint.y() * drawArea.height() / mapLink->fullHeight()))
    };
}

QPointF RenderArea::rectCoordToMapCoord(const QPoint& drawPoint) const {
    if (mapLink == nullptr)
        return {-1, -1};
    if (!mapLink->isValid())
        return {-1, -1};
    const QRect drawArea = getMapRect();
    if (!drawArea.contains(drawPoint))
        return {-1, -1};
    return QPointF{
            static_cast<qreal>((drawPoint.x()-drawArea.x()) * static_cast<int32_t>(mapLink->fullWidth())) / drawArea.width(),
            static_cast<qreal>((drawPoint.y()-drawArea.y()) * static_cast<int32_t>(mapLink->fullHeight())) / drawArea.height()
    };
}


void RenderArea::paintEvent(QPaintEvent*) {
    const QRect drawArea = this->frameGeometry();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Background
    painter.setBrush(QBrush(QColor(64, 64, 64)));
    painter.drawRect(drawArea);
    // MAP
    const int increment = getMapIncrement();
    if (increment < 0)
        return;
    const QPoint Start{static_cast<int>((width() - increment * static_cast<int32_t>(mapLink->width())) / 2), static_cast<int>((height() - increment * static_cast<int32_t>(mapLink->height())) / 2)};
    QRect CellRect{Start.x(), Start.y(), increment, increment};
    for (const auto& line : mapLink->getMapData()) {
        for (const auto& cell : line) {
            if (cell.passable) {
                painter.setBrush(QBrush(QColor(10, 10, 10)));
            } else {
                auto& col = cell.getMapColor();
                painter.setBrush(QBrush(QColor(col.red(), col.green(), col.blue())));
            }
            painter.drawRect(CellRect);
            CellRect.moveTo(CellRect.right() + 1, CellRect.top());
        }
        CellRect.moveTo(Start.x(), CellRect.bottom() + 1);
    }
    // Player start
    const auto [pPos,pDir] = mapLink->getPlayerStart();
    const auto playerStart = mapCoordToRectCoord({pPos[0], pPos[1]});
    const int halfLength = std::min(CellRect.width(),CellRect.height()) / 2;
    const QPoint Direction{static_cast<int>(pDir[0]*halfLength),static_cast<int>(pDir[1]*halfLength)};
    const QPoint Left{static_cast<int>(-pDir[1]*halfLength),static_cast<int>(pDir[0]*halfLength)};
    painter.setPen(QColor(0,255,0));
    painter.drawLine(playerStart-Direction,playerStart+Direction);
    painter.drawLine(playerStart-Left,playerStart+Direction);
    painter.drawLine(playerStart+Left,playerStart+Direction);
}

void RenderArea::mouseMoveEvent(QMouseEvent* event) {
    const auto rect    = getMapRect();
    const QPoint mouse = event->pos();
    if (!rect.contains(mouse)) {
        MouseCell = QPoint{-1, -1};
        MousePos  = QPoint{-1, -1};
        return;
    }
    MousePos = mouse - rect.topLeft();
    MouseCell.setX(MousePos.x() * static_cast<int32_t>(mapLink->width()) / rect.width());
    MouseCell.setY(MousePos.y() * static_cast<int32_t>(mapLink->height()) / rect.height());
    MousePos.setX(MousePos.x() * static_cast<int32_t>(mapLink->fullWidth()) / rect.width());
    MousePos.setY(MousePos.y() * static_cast<int32_t>(mapLink->fullHeight()) / rect.height());
}

double RenderArea::getMapRatio() const {
    if (mapLink == nullptr)
        return -1;
    if (!mapLink->isValid())
        return -1;
    return std::min(width() / static_cast<double>(mapLink->fullWidth()), height() / static_cast<double>(mapLink->fullHeight()));
}

int32_t RenderArea::getMapIncrement() const {
    const double ratio = getMapRatio();
    if (ratio < 0) return -1;
    return static_cast<int>(ratio * mapLink->getCellSize());
}

QRect RenderArea::getMapRect() const {
    const int increment = getMapIncrement();
    if (increment < 0) return QRect();
    const QPoint Start{static_cast<int>((width() - increment * static_cast<int32_t>(mapLink->width())) / 2), static_cast<int>((height() - increment * static_cast<int32_t>(mapLink->height())) / 2)};
    return QRect{Start, Start + QPoint{static_cast<int>(mapLink->width()), static_cast<int>(mapLink->height())} * increment};
}

}// namespace we
