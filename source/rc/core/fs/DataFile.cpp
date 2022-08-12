/**
 * @file DataFile.cpp
 * @author Silmaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "DataFile.h"
#include <fstream>

namespace rc::core::fs {

/// Base path to the data
static DataFile::path DataPath;

/**
 * @brief Update the static file path
 */
static void searchDataPath() {
    DataPath = std::filesystem::current_path();                        // starts from current directory
    if (std::filesystem::exists((DataPath / "data" / "rcdata.conf"))) {// if folder data exists in child dir
        DataPath = DataPath / "data";                                  // ---UNCOVER---
        return;                                                        // ---UNCOVER---
    }
    while (DataPath.parent_path() != DataPath) {// if is possible to go to parent
        DataPath = DataPath.parent_path();
        if (std::filesystem::exists((DataPath / "data" / "rcdata.conf"))) {// if folder data exists in parent dir
            DataPath = DataPath / "data";
            return;
        }
    }
    // Defaults to current pat if nothing found
    DataPath = std::filesystem::current_path();// ---UNCOVER---
}

std::filesystem::path DataFile::getDataPath() {
    if (DataPath.empty())
        searchDataPath();
    return DataPath;
}

void DataFile::remove() const {
    if (!exists())
        return;
    std::filesystem::remove(getFullPath());
}


void DataFile::setPath(const DataFile::path& file) {
    getDataPath();
    if (file.is_absolute()) {
        filePath = relative(file, DataPath);
    } else {
        filePath = file;
    }
}

DataFile::path DataFile::getFullPath() const {
    getDataPath();
    return DataPath / filePath;
}

nlohmann::json DataFile::readJson() const {
    std::ifstream jFile(getFullPath());
    if (! jFile.is_open()){
        return nlohmann::json{};
    }
    auto data = nlohmann::json::parse(jFile);
    jFile.close();
    return data;
}
void DataFile::writeJson(const nlohmann::json& data) const {
    std::ofstream jFile(getFullPath());
    jFile << data.dump(4, ' ');
    jFile.close();
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
