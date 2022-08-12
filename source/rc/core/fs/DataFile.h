/**
 * @file DataFile.h
 * @author Silmaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>

/**
 * @brief Namespace for filesystem management
 */
namespace rc::core::fs {

/**
 * @brief Class DataFile
 *
 * It manages files located in data folder
 */
class DataFile {
public:
    /// Type for path
    using path = std::filesystem::path;

    /**
     * @brief Default constructor.
     */
    DataFile() = default;

    /**
     * @brief Default copy constructor
     */
    DataFile(const DataFile&) = default;

    /**
     * @brief Default move constructor
     */
    DataFile(DataFile&&) = default;

    /**
     * @brief Construct with a file
     * @param file The file to construct
     */
    DataFile(const path& file){setPath(file);}

    /**
     * @brief Default copy assignation
     * @return this
     */
    DataFile& operator=(const DataFile&) = default;

    /**
     * @brief Default move assignation
     * @return this
     */
    DataFile& operator=(DataFile&&) = default;

    /**
     * @brief Copy assignation from path
     * @param file Path to copy
     * @return this
     */
    DataFile& operator=(const path& file){ setPath(file);return *this;}

    /**
     * @brief Destructor.
     */
    ~DataFile() = default;

    /**
     * @brief Search for data path and actualize the path.
     * @return Path to the data folder
     */
    static path getDataPath();

    /**
     * @brief Define the path to the file
     * @param file Direct use if absolute or relative to data directory
     */
    void setPath(const path& file);

    /**
     * @brief Get the full path of the file
     * @return Full path of the file
     */
    [[nodiscard]] path getFullPath() const;

    /**
     * @brief Access to the relative path
     * @return Relative path
     */
    [[nodiscard]] const path& getPath() const { return filePath; }

    /**
     * @brief Check file existence
     * @return True file exists
     */
    [[nodiscard]] bool exists()const{return std::filesystem::exists(getFullPath());}

    /**
     * @brief Remove the file if exists
     */
    void remove() const;

    /**
     * @brief Touch the file
     */
    void touch() const;

    /**
     * @brief Open data file for reading
     * @return Json read in file
     */
    [[nodiscard]] nlohmann::json readJson() const;

    /**
     * @brief Open data file for writing
     * @param data Json to write in file
     */
    void writeJson(const nlohmann::json& data)const;
private:
    /// Path to the file relative to DataPath
    path filePath;
};
}// namespace rc::core::fs
