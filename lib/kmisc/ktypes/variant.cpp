#include "variant.h"

namespace PolymorphicTypes {

bool isNumeric(BasicType type)
{
    return (type == BasicType::Integer ||
            type == BasicType::Float ||
            type == BasicType::Double);
}

bool isFloat(BasicType type)
{
    return (type == BasicType::Float ||
            type == BasicType::Double);
}

Variant::Variant() {}

//template<>
//Variant::Variant(std::string value)
//{
//    _rawValue = std::move(value);
//    _type = BasicType::String;
//}

BasicType Variant::type() const
{
    return _type;
}

int Variant::toInt() const
{
    return std::stoi(_rawValue);
}

double Variant::toDouble() const
{
    return std::stod(_rawValue);
}

float Variant::toFloat() const
{
    return std::stof(_rawValue);
}

std::string Variant::toString() const
{
    return _rawValue;
}

int Variant::compare_as_strings(const Variant& value) const
{
    return _rawValue.compare(value._rawValue);
}

int Variant::compare(const Variant& value) const
{
    if (isNumeric(this->type()))
    {
        if (!isNumeric(value.type()))   // TODO: compare as strings
        {
            debug("Can't compare numbers to strings. Comparing as strings");
            return compare_as_strings(value);
        }

        double a = toDouble();
        double b = value.toDouble();
        return static_cast<int>(a - b);
    } else {
        SOFT_ASSERT(isNumeric(value.type())) debug("Can't compare numbers to strings. Comparing as strings");
        return compare_as_strings(value);
    }
}

/// Variant pointer!

VariantPointer::VariantPointer() {}

BasicType VariantPointer::type() const
{
    return _type;
}

size_t VariantPointer::size() const
{
    return _size;
}

void* VariantPointer::rawPtr() const
{
    return _rawValue;
}

int* VariantPointer::toIntPtr() const
{
    return toCustomPtr<int>();
}

double* VariantPointer::toDoublePtr() const
{
    return toCustomPtr<double>();
}

float* VariantPointer::toFloatPtr() const
{
    return toCustomPtr<float>();
}

std::string* VariantPointer::toStrPtr() const
{
    return toCustomPtr<std::string>();
}

}

