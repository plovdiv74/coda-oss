/* =========================================================================
 * This file is part of re-c++
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * re-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include "re/PCRE.h" // this has to come before the #ifdef checks below

// we use this file if we're not using actual PCRE itself
#if defined(__CODA_CPP11)

re::PCRE::PCRE(const std::string& pattern, Flag flags) :
    mPattern(pattern)
{
    if (!mPattern.empty())
    {
        compile(mPattern, flags);
    }
}

void re::PCRE::destroy()
{
}

re::PCRE::~PCRE()
{
}

re::PCRE::PCRE(const re::PCRE& rhs)
{
    mPattern = rhs.mPattern;
    compile(mPattern);
}

re::PCRE& re::PCRE::operator=(const re::PCRE& rhs)
{
    if (this != &rhs)
    {
        mPattern = rhs.mPattern;

        compile(mPattern);
    }

    return *this;
}

re::PCRE& re::PCRE::compile(const std::string& pattern,
                            int flags)
{
    mPattern = (flags==PCRE_DOTALL) ? replace_dot(pattern) : pattern;
    try 
    {
        mRegex = std::regex(mPattern);
    }
    catch (const std::regex_error& e)
    {
        throw PCREException(Ctxt("PCRE std::regex constructor error: "
                                 + e.what()));
    }

    return *this;
}


const std::string& re::PCRE::getPattern() const
{
    return mPattern;
}

bool re::PCRE::matches(const std::string& str, int) const
{
    return std::regex_match(str, mRegex);
}

bool re::PCRE::match(const std::string& str,
                     PCREMatch & matchObject,
                     int )
{
    std::smatch matches;
    bool result = std::regex_search(str, matches, mRegex);

    // copy resulting substrings into matchObject
    matchObject.resize(matches.size());

    // This causes a crash for some reason
    //std::copy(matches.begin(), matches.end(), matchObject.begin());

    for(size_t ii = 0; ii < matches.size(); ++ii)
    {
        matchObject[i] = matches[i].str();
    }

    return result;
}

std::string re::PCRE::search(const std::string& matchString,
                             int startIndex,
                             int)
{
    std::smatch matches;

    // search the string starting at index "startIndex"
    bool result = std::regex_search(matchString.begin()+startIndex, 
                                    matchString.end(), matches, mRegex);
    
    // if successful, return the substring matching the regex,
    // otherwise return empty string
    if (result && !matches.empty())
    {
        return matches[0].str();
    }
    else
    {
        return "";
    }
}

void re::PCRE::searchAll(const std::string& matchString,
                         PCREMatch& v)
{
    // this iterates mRegex over the input string, returning an
    // iterator to the match objects
    auto wordsBegin = 
        std::sregex_iterator(matchString.begin(), matchString.end(), mRegex);
    auto wordsEnd = std::sregex_iterator();
 
    // copy the matches into v
    for (std::sregex_iterator matchIter = wordsBegin; 
         matchIter != wordsEnd; ++matchIter)
    {
        std::string matchStr = matchIter->str(); 
        v.push_back(matchStr);
    }
}

void re::PCRE::split(const std::string& str,
                     std::vector<std::string> & v)
{
    size_t idx = 0;
    auto flags = std::regex_constants::match_default;
    std::smatch match;
    while (std::regex_search(str.begin()+idx, str.end(), match, mRegex, flags))
    {
        v.push_back( str.substr(idx, match.position()) );
        idx += (match.position() + match.length());

        // not sure this will ever be needed for a split() operation,
        // but we'll be safe (matches after first match will not match
        // beginning-of-line))
        if (flags == std::regex_constants::match_default)
        {
            flags = std::regex_constants::match_not_bol;
        }
    }

    // Push on last bit if there is some
    if (!str.substr(idx).empty())
    {
        v.push_back(str.substr(idx));
    }
}

std::string re::PCRE::sub(const std::string& str,
                          const std::string& repl)
{
    std::string toReplace = str;

    size_t idx = 0;
    auto flags = std::regex_constants::match_default;
    std::smatch match;
    while (std::regex_search(toReplace.cbegin()+idx, toReplace.cend(), 
                             match, mRegex, flags))
    {
        toReplace.replace(idx + match.position(), match.length(), repl);
        idx += (match.position() + match.length());

        // matches after first match will not match beginning-of-line
        if (flags == std::regex_constants::match_default)
        {
            flags = std::regex_constants::match_not_bol;
        }
    }

    return toReplace;
}

std::string re::PCRE::escape(const std::string& str) const
{
    std::string r;
    for (size_t ii = 0; ii < str.length(); ii++)
    {
        if (!isalpha(str[i]) && !isspace(str[i]))
        {
            r += '\\';
        }
        r += str[i];
    }
    return r;
}

std::string re::PCRE::replaceDot(const std::string& str) const
{
    std::regex reg("([^\\\\])\\.");
    std::string newstr = std::regex_replace(str, reg, "$1[\\s\\S]");
    return newstr;
}

#endif
