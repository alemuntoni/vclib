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

#ifndef VCL_MISC_TOKENIZER_H
#define VCL_MISC_TOKENIZER_H

#include <string>
#include <vector>

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Tokenizer class
 */
class Tokenizer
{
    char mSeparator = '\0';

    std::vector<std::string> mSplitted;

public:
    using iterator = std::vector<std::string>::const_iterator;

    Tokenizer() = default;

    Tokenizer(const char* string, char separator, bool jumpEmptyTokens = true) :
            mSeparator(separator)
    {
        split(string, jumpEmptyTokens);
    }

    Tokenizer(
        const std::string& string,
        char               separator,
        bool               jumpEmptyTokens = true) :
            mSeparator(separator)
    {
        split(string.c_str(), jumpEmptyTokens);
    }

    iterator begin() const { return mSplitted.begin(); }

    iterator end() const { return mSplitted.end(); }

    unsigned long int size() const { return (unsigned long) mSplitted.size(); }

    const std::string& operator[](uint i) const { return mSplitted[i]; }

private:
    void split(const char* str, bool jumpEmptyTokens = true)
    {
        // https://stackoverflow.com/questions/53849/
        mSplitted.clear();
        if (*str != '\0') {
            do {
                const char* begin = str;
                while (*str != mSeparator && *str)
                    str++;
                if (begin != str)
                    mSplitted.push_back(std::string(begin, str));
                else if (!jumpEmptyTokens) {
                    mSplitted.push_back(std::string());
                }
            } while ('\0' != *str++);
        }
    }
};

} // namespace vcl

#endif // VCL_MISC_TOKENIZER_H
