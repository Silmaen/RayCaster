/**
 * @file MainWindow.h
 * @author Silmaen
 * @date 09/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Map.h"
#include <QMainWindow>

/**
 * @brief Namespace for QT ui
 */
namespace Ui {
/**
 * @brief Ui encapsulation of main window
 */
class MainWindow;

}// namespace Ui

/**
 * @brief Namespace for the world editor
 */
namespace we {

/**
 * @brief Class MainWindow
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Default constructor.
     */
    explicit MainWindow(QWidget* parent = nullptr);
    /**
     * @brief Destructor.
     */
    ~MainWindow() override;

private slots:

    // menu file
    /**
     * @brief Action new map
     */
    void actNew();
    /**
     * @brief Action open map
     */
    void actOpen();
    /**
     * @brief Action save Map
     */
    void actSave();
    /**
     * @brief Action Save as map
     */
    void actSaveAs();
    /**
     * @brief Action quit
     */
    void actQuit();

    void actChangeMode();

    void actChangePattern();
protected:

    void paintEvent(QPaintEvent*)override;

    void mouseReleaseEvent(QMouseEvent* event) override;
private:
    /// UI page link.
    Ui::MainWindow* ui;

    /// Name of the map
    QString mapName;

    /// Name of the map
    QString filename = "";

    /// if the map has been modified
    bool mapModified = false;

    /// map objet to manipulate
    std::shared_ptr<rc::core::Map> theMap;

    QTimer* timer;

    enum struct Mode {
        Select,
        Edit,
    };
    Mode currentMode = Mode::Select;

    enum struct Pattern {
        Passable,
        Wall1,
        Wall1b,
        Wall2,
        Wall2b,
        Wall3,
        Wall3b,
    };
    Pattern currentPattern = Pattern::Passable;

    void timedOut();
    /**
     * @brief Define the map pointer everywhere it is needed
     * @param map the map to set
     */
    void setMap(const std::shared_ptr<rc::core::Map>& map);
};

}// namespace we
