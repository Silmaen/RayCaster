/**
 * @file MapFile.h
 * @author argawaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "DataFile.h"

namespace rc::core::fs {

/**
 * @brief Class MapFile
 */
class MapFile : public DataFile {
public:
    /**
     * @brief Default constructor.
     */
    MapFile() = default;
    /**
     * @brief Default copy constructor
     */
    MapFile(const MapFile&) = default;
    /**
     * @brief Default move constructor
     */
    MapFile(MapFile&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    MapFile& operator=(const MapFile&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    MapFile& operator=(MapFile&&) = default;
    /**
     * @brief Destructor.
     */
    virtual ~MapFile() = default;

private:
};

}// namespace rc::core::fs
