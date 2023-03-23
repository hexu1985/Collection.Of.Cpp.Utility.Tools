#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace util
{
    class ArgBase
    {
    public:
        ArgBase() {}
        virtual ~ArgBase() {}
        virtual void Format(std::ostringstream &ss, const std::string& fmt) = 0;
    };

    template <class T>
        class Arg : public ArgBase
    {
    public:
        Arg(T arg) : m_arg(arg) {}
        virtual ~Arg(){}
        virtual void Format(std::ostringstream &ss, const std::string& fmt)
        {
            (void) fmt;
            ss << m_arg;
        }
    private:
        T m_arg;
    };

    class ArgArray : public std::vector < ArgBase* >
    {
    public:
        ArgArray() {}
        ~ArgArray()
        {
            std::for_each(begin(), end(), [](ArgBase* p){ delete p; });
        }
    };

    static void FormatItem(std::ostringstream& ss, const std::string& item, const ArgArray& args, int& automic_index)
    {
        int len = item.size();

        // skip space char
        int start = 0;
        while (start < len && isspace(item[start])) {
            start++;
        }

        int end = start;
        while (end < len && isdigit(item[end])) {
            end++;
        }

        int index = 0;
        if (start == end) {     // empty arg-id
            index = ++automic_index;
        } else {
            if (automic_index != -1) {
                throw std::runtime_error("not support to mix automatic field numbering with manual field specification");
            }
            index = std::stoi(item.substr(start, end-start));
        }

        if (index < 0 || index >= (int) args.size())
        {
            throw std::out_of_range("args index out of range");
        }

        std::string fmt;
        if (end != len && item[end] == ':') {
            fmt = item.substr(end);
        }

        args[index]->Format(ss, fmt);

        return;
    }

    template <class T>
    static void Transfer(ArgArray& argArray, T t)
    {
        argArray.push_back(new Arg<T>(t));
    }

    template <class T, typename... Args>
    static void Transfer(ArgArray& argArray, T t, Args&&... args)
    {
        Transfer(argArray, t);
        Transfer(argArray, args...);
    }

    template <typename... Args>
    std::string Format(const std::string& format, Args&&... args)
    {
        static_assert(sizeof...(args) > 0);

        ArgArray argArray;
        Transfer(argArray, args...);
        int bracket_index = -1;
        std::ostringstream ss;

        int len = format.size();
        int i = 0;
        while (i < len) {
            if (format[i] == '{') {
                if (format[i+1] == '{') {
                    ss << '{';
                    i += 2;
                    continue;
                }
                auto start = i+1;
                auto pos = format.find('}', start);
                if (pos != std::string::npos) {
                    FormatItem(ss, format.substr(start, pos - start), argArray, bracket_index);
                    i = pos+1;
                } else {
                    ss << '{';
                    i += 1;
                }
            } else if (format[i] == '}') {
                if (format[i+1] == '}') {
                    i += 2;
                } else {
                    i += 1;
                }
                ss << '}';
            } else {
                ss << format[i];
                i += 1;
            }
        }

        return ss.str();
    }

    inline const std::string& Format(const std::string& format)
    {
        return format;
    }
}

