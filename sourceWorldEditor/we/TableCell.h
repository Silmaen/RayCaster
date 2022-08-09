/**
 * @file TableCell.h
 * @author argawaen
 * @date 10/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <QTableWidget>

namespace we {

/**
 * @brief Class TableCell
 */
class TableCell : public QTableWidget {
    Q_OBJECT
public:
    /**
     * @brief Default constructor.
     * @param parent Parent widget
     */
    explicit TableCell(QWidget* parent = nullptr);
    /**
     * @brief Destructor.
     */
    ~TableCell() override;

    void setCell(const QPoint& cell){currentCell = cell; fillingUp();}

    void fillingUp();

private:
    QPoint currentCell{-1,-1};
};

}// namespace we
