/**
 * @file DataFile.cpp
 * @author Silmaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DataFile.h"

namespace rc::core::fs {

void DataFile::searchDataPath() {
    DataPath = std::filesystem::current_path();                        // starts from current directory
    if (std::filesystem::exists((DataPath / "data" / "rcdata.conf"))) {// if folder data exists in child dir
        DataPath = DataPath / "data";
        return;
    }
    while (DataPath.parent_path() != DataPath) {// if is possible to go to parent
        DataPath = DataPath.parent_path();
        if (std::filesystem::exists((DataPath.parent_path() / "data" / "rcdata.conf"))) {// if folder data exists in parent dir
            DataPath = DataPath.parent_path() / "data";
            return;
        }
    }
    DataPath = std::filesystem::current_path();// Defaults to current pat if nothing found
}

void DataFile::setPath(const DataFile::path& file) {
    if (DataPath.empty())
        searchDataPath();
    if (file.is_absolute()) {
        filePath = relative(file, DataPath);
    } else {
        filePath = file;
    }
}

DataFile::path DataFile::getFullPath() const {
    if (DataPath.empty())
        searchDataPath();
    return DataPath / filePath;
}

std::ifstream DataFile::openRead() const {
    return std::ifstream(getFullPath());
}

}// namespace rc::core::fs
