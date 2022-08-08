/**
 * @file DataFile.cpp
 * @author Silmaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DataFile.h"
#include <fstream>
#include <iostream>

namespace rc::core::fs {

/// Base path to the data
static DataFile::path DataPath;

void DataFile::remove() {
    if (!exists())
        return;
    std::filesystem::remove(getFullPath());
}

void DataFile::searchDataPath() {
    DataPath = std::filesystem::current_path();                        // starts from current directory
    if (std::filesystem::exists((DataPath / "data" / "rcdata.conf"))) {// if folder data exists in child dir
        DataPath = DataPath / "data";                                  // --- UNCOVER ---
        return;                                                        // --- UNCOVER ---
    }
    while (DataPath.parent_path() != DataPath) {// if is possible to go to parent
        DataPath = DataPath.parent_path();
        if (std::filesystem::exists((DataPath / "data" / "rcdata.conf"))) {// if folder data exists in parent dir
            DataPath = DataPath / "data";
            return;
        }
    }
    // Defaults to current pat if nothing found
    DataPath = std::filesystem::current_path();// --- UNCOVER ---
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

nlohmann::json DataFile::readJson() const {
    std::ifstream j(getFullPath());
    auto data = nlohmann::json::parse(j);
    j.close();
    return data;
}
void DataFile::writeJson(const nlohmann::json& data) const {
    std::ofstream j(getFullPath());
    j << data.dump(4, ' ');
    j.close();
}

void DataFile::touch() const {
    if (std::filesystem::exists(getFullPath())){
        std::filesystem::last_write_time(getFullPath(), std::chrono::file_clock::now());
    }else{
        std::ofstream file(getFullPath());
        file.close();
    }
}

}// namespace rc::core::fs
