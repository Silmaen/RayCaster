/**
 * @file RenderArea.h
 * @author Silmaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "game/Map.h"
#include <QWidget>


namespace we {

/**
 * @brief Class RenderArea
 */
class RenderArea : public QWidget {
public:
    /**
     * @brief Constructor.
     */
    explicit RenderArea(QWidget* parent);

    /**
     * @brief Destructor.
     */
    ~RenderArea() override;

    /**
     * @brief Link to the map
     * @param map map to render
     */
    void defineMap(const std::shared_ptr<rc::game::Map>& map) {
        mapLink = map;
    }

    std::tuple<const QPoint&, const QPoint&> getMouseInfo() const { return {MousePos, MouseCell}; }

protected:
    void paintEvent(QPaintEvent*) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPoint mapCoordToRectCoord(const QPointF& mapPoint);

    QPointF rectCoordToMapCoord(const QPoint& drawPoint);

    [[nodiscard]] QRect getMapRect() const;
    double getMapRatio() const;
    int32_t getMapIncrement() const;
    /// Link to the map
    std::shared_ptr<rc::game::Map> mapLink = nullptr;

    QPoint MousePos{-1, -1};

    QPoint MouseCell{-1, -1};
};

}// namespace we
