#ifndef VARIANT_H
#define VARIANT_H

#include <cstdio>
#include <string>
#include "CommonUtils.h"

namespace PolymorphicTypes
{
enum class BasicType : int
{
    Undefined   = 0,
    String,
    Integer,
    Double,
    Float
};

template<typename T>
BasicType GetTypeOf() { return BasicType::Undefined; }

template<> inline
BasicType GetTypeOf<int>() { return BasicType::Integer; }

template<> inline
BasicType GetTypeOf<STRING_TYPE>() { return BasicType::String; }

template<> inline
BasicType GetTypeOf<double>() { return BasicType::Double; }

template<> inline
BasicType GetTypeOf<float>() { return BasicType::Float; }

bool isNumeric(BasicType type);
bool isFloat(BasicType type);

class Variant
{
    BasicType _type = BasicType::Undefined;
    STRING_TYPE _rawValue;

    int compare_as_strings(const Variant& value) const;
public:
    Variant();

    template<typename T>
    explicit Variant(T value)
    {
        _type = GetTypeOf<T>();
        _rawValue = std::to_string(value);
    }

    BasicType type() const;

    int toInt() const;
    double toDouble() const;
    float toFloat() const;
    std::string toString() const;

    int compare(const Variant& value) const;
};

template<>
inline Variant::Variant(std::string value)
{
    _rawValue = std::move(value);
    _type = BasicType::String;
}

class VariantPointer
{
    BasicType _type = BasicType::Undefined;
    void* _rawValue;
    size_t _size;

public:
    VariantPointer();

    template<typename T>
    explicit VariantPointer(T* value)
    {
        _type = GetTypeOf<T>();
        _size = sizeof(T);
        _rawValue = value;
    }

    BasicType type() const;
    size_t size() const;

    template<typename T>
    T* toCustomPtr() const
    {
        SOFT_ASSERT(sizeof(T) != _size) debug("Warning! Dangerous cast!\n");
        return reinterpret_cast<T*>(_rawValue);
    }
    int* toIntPtr() const;
    double* toDoublePtr() const;
    float* toFloatPtr() const;
    std::string* toStrPtr() const;

    Variant toVariant() const
    {
        Variant result;
        switch (_type) {
            case BasicType::Integer:    result = Variant(*toIntPtr()); break;
            case BasicType::Double:     result = Variant(*toDoublePtr()); break;
            case BasicType::Float:      result = Variant(*toFloatPtr()); break;
            case BasicType::String:     result = Variant(*toStrPtr()); break;
            default:                    result = Variant();
        }
        return result;
    }

    void* rawPtr() const;
};


template<>
inline VariantPointer::VariantPointer(const char* value)
{
    _type = BasicType::String;
    _rawValue = new std::string(value);
    _size = sizeof(_rawValue);
}

}
#endif // VARIANT_H
