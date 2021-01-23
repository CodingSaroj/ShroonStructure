#pragma once

#include <functional>
#include <iostream>
#include <string>

/**
 * @brief Defines a basic error reporter function. This macro only needs to be invoked in one
 * translation unit (.cpp file) in global namespace. Don't invoke this if you are defining
 * your own error reporter function.
 */
#define SHRN_STRUCTURE_DEFAULT_ERROR_REPORTER \
    std::function<void(std::string, bool)> Shroon::Structure::ErrorReporter = [](std::string log, bool fatal)\
    {\
        std::cerr<<"Shroon::Structure::"<<(fatal ? "FatalError: " : "Error: ")<<log<<"\n";\
        \
        if (fatal)\
        {\
            exit(1);\
        }\
    };

/**
 * @brief The namespace in which all the projects under Shroon exist.
 */
namespace Shroon
{
    /**
     * @brief The namespace in which all the ShroonStructure API exists.
     */
    namespace Structure
    {
        /**
         * @brief A callback the data structures will use when an error occurs. First parameter is
         * the info about the error and second parameter is true when the error is fatal.
         */
        extern std::function<void(std::string, bool)> ErrorReporter;
    }
}
