/**
 * @file DataFile.h
 * @author Silmaen
 * @date 07/08/22
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <filesystem>
#include <fstream>

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
     * @brief Destructor.
     */
    virtual ~DataFile() = default;

    /**
     * @brief Search for data path and actualize the path.
     */
    static void searchDataPath();

    /**
     * @brief Define the path to the file
     * @param file Direct use if absolute or relative to data directory
     */
    virtual void setPath(const path& file);

    /**
     * @brief Get the full path of the file
     * @return Full path of the file
     */
    [[nodiscard]] virtual path getFullPath() const;

    /**
     * @brief Access to the relative path
     * @return Relative path
     */
    [[nodiscard]] const path& getPath() const { return filePath; }

    /**
     * @brief Open data file for reading
     * @return Input file stream
     */
    [[nodiscard]] std::ifstream openRead() const;

private:
    /// Base path to the data
    static std::filesystem::path DataPath;

    /// Path to the file relative to DataPath
    std::filesystem::path filePath;
};
}// namespace rc::core::fs
