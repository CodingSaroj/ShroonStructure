#pragma once

#include <functional>
#include <iostream>
#include <string>

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
         * @defgroup ErrorReporting
         * This group contains the error reporting API for this library.
         * @{
         */

        /**
         * @brief A callback the data structures will use when an error occurs. First parameter is
         * the info about the error and second parameter is true when the error is fatal.
         */
        extern std::function<void(std::string, bool)> ErrorReporter;

        /**
         * @}
         */
    }
}

/**
 * @defgroup ErrorReporting
 * This group contains the error reporting API for this library.
 * @{
 */

#ifndef SHRN_STRUCTURE_DEFAULT_ERROR_REPORTER
    /**
     * @brief A basic error reporter function is defined if this is 1. This macro only needs to be
     * defined as 1 in one translation unit (.cpp file) in global namespace. Don't define this as 1
     * if you are defining your own error reporter function. If no macro with the name is defined,
     * then it is defined as 0.
     */
    #define SHRN_STRUCTURE_DEFAULT_ERROR_REPORTER 0
#endif

/**
 * @}
 */

#if SHRN_STRUCTURE_DEFAULT_ERROR_REPORTER == 1
    std::function<void(std::string, bool)> Shroon::Structure::ErrorReporter = [](std::string log, bool fatal)
    {
        std::cerr<<"Shroon::Structure::"<<(fatal ? "FatalError: " : "Error: ")<<log<<"\n";

        if (fatal)
        {
            exit(1);
        }
    };
#endif
