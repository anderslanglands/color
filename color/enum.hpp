#pragma once

#define ENUM_DEFINE(EnumName, ...) \
    enum class EnumName: int; \
    \
    class EnumName##_enum { \
    EnumName##_enum() = delete(); \
public:\
static const std::vector<std::string>& get_strings() { \
static std::vector<std::string> _values;
