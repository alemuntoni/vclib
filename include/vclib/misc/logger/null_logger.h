/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MISC_LOGGER_NULL_LOGGER_H
#define VCL_MISC_LOGGER_NULL_LOGGER_H

#ifndef VCLIB_WITH_MODULES
#include <string>

#include <vclib/concepts/logger.h>
#endif

namespace vcl {

/**
 * @brief The NullLogger class is used as default type in all the library
 * functions that take as input a logger type.
 *
 * This class satisfies the LoggerConcept, and just does nothing in all its
 * member functions.
 *
 * @ingroup miscellaneous
 */
class NullLogger
{
public:
    enum LogLevel { ERROR = 0, WARNING, MESSAGE, PROGRESS, DEBUG };

    NullLogger() = default;

    void enableIndentation() {}

    void disableIndentation() {}

    void enablePrintPercentage() {}

    void disablePrintPercentage() {}

    void enablePrintMessageDuringProgress() {}

    void disablePrintMessageDuringProgress() {}

    void enablePrintTimer() {}

    void disablePrintTimer() {}

    void reset() {}

    void setMaxLineWidth(uint) {}

    void startTimer() {}

    void stopTimer() {}

    double getTime() { return 0; }

    void startNewTask(double, double, std::string) {}

    void endTask(std::string) {}

    double percentage() const { return 0; }

    void setPercentage(uint) {}

    void log(std::string) {}

    void log(LogLevel, std::string) {}

    void log(uint, std::string) {}

    void log(uint, LogLevel, std::string) {}

    void startProgress(std::string, uint, uint = 0, uint = 0, uint = 0) {}

    void endProgress() {}

    void progress(uint) {}
};

/**
 * @brief The nullLogger object is an object of type NullLogger that is
 * used as default argument in the functions that can take as input a logger.
 *
 * A typical function that could take a logger is defined as follows:
 *
 * @code{.cpp}
 * template<typename T, LoggerConcept LoggerType = NullLogger>
 * void foo(T obj, LoggerType log = nullLogger)
 * {
 *    // code of the function...
 *
 *    // empty function will be called if the user does not give a logger
 *    log.log("log!");
 *
 * }
 * @endcode
 *
 * In this way, when the user does not give a logger argument, the default will
 * be an object of NullLogger.
 *
 * @ingroup miscellaneous
 */
inline NullLogger nullLogger;

/**
 * @brief The isLoggerValid() function returns true at compile time if the type
 * T is not the NullLogger type.
 *
 * It is useful when you need to perform some operation explicitely for logging
 * that should not be done if the user does not give a logger. In this case the
 * code will not be compiled.
 *
 * @tparam T: the type to test if is a valid Logger. It must satisfy the
 * LoggerConcept.
 *
 * @return true it T is a valid Logger type.
 */
template<LoggerConcept T>
constexpr bool isLoggerValid()
{
    return !std::is_same_v<T, NullLogger>;
}

} // namespace vcl

#endif // VCL_MISC_LOGGER_NULL_LOGGER_H
