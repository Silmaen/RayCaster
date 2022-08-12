/**
 * @file NewMap.h
 * @author Silmaen
 * @date 09/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <QDialog>

namespace Ui {
/**
 * @brief Ui encapsulation of main window
 */
class NewMap;

}// namespace Ui

namespace we {

/**
 * @brief Class NewMap
 */
class NewMap: public QDialog{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
   explicit NewMap(QWidget* parent);
    /**
     * @brief Destructor.
     */
    ~NewMap()override;

    /**
     * @brief Struct gathering window info
     */
    struct Infos {
        QString name; ///< Map name
        uint8_t width; ///< Map width
        uint8_t height; ///< Map height
        uint8_t cellSize; ///< Map Cell size
    };

    /**
     * @brief Get the window info
     * @return Window info
     */
    const Infos getInfos()const;

    /**
     * @brief Define the window info
     * @param infos Windo info
     */
    void setInfos(const Infos& infos);
private:
    /// link to UI
    Ui::NewMap* ui;
};

}// namespace we
