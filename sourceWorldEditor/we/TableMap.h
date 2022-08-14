/**
 * @file TableMap.h
 * @author Silmaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "game/Map.h"
#include <QTableWidget>


namespace we {

/**
 * @brief Class TableMap
 */
class TableMap : public QTableWidget {
    Q_OBJECT
public:
    /**
     * @brief Default constructor.
     * @param parent Parent widget
     */
    explicit TableMap(QWidget* parent = nullptr);
    /**
     * @brief Destructor.
     */
    ~TableMap() override;

    /**
     * @brief Link to the map
     * @param map map to render
     */
    void defineMap(const std::shared_ptr<rc::graphics::Map>& map, const QString& name){
        mapLink = map;
        mapName = name;
    }

    /**
     * @brief Fill the table with the map info
     */
    void fillingUp();

    bool checkModification(){
        bool ret =modified;
        modified =false;
        return ret;
    }
public slots:

    void updatePlayer();
protected:


private:
    /// Name of the map
    QString mapName = "";
    /// Link to the map
    std::shared_ptr<rc::graphics::Map> mapLink;
    /// if some modification occurs
    bool modified;
};

}// namespace we
