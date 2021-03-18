#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <list>
#include "variant.h"

namespace Validation {


enum class RuleOperator : int
{
    Undefined = 0,
    Equal,
    NotEqual,
    Less,
    Greater
};


class Rule
{
    RuleOperator m_operator = RuleOperator::Undefined;

    PolymorphicTypes::VariantPointer m_base_pointer;

    PolymorphicTypes::VariantPointer m_compare_pointer;
    PolymorphicTypes::Variant        m_compare_value;

    bool m_compare_by_value = false;

public:
    Rule();
    explicit Rule(PolymorphicTypes::VariantPointer base_pointer, RuleOperator rule, PolymorphicTypes::VariantPointer compare_pointer);
    explicit Rule(PolymorphicTypes::VariantPointer base_pointer, RuleOperator rule, PolymorphicTypes::Variant compare_value);

    template<class T, class U>
    explicit Rule(T* base_pointer, RuleOperator rule, U value)
    {
        m_base_pointer = PolymorphicTypes::VariantPointer(base_pointer);
        m_compare_value = PolymorphicTypes::Variant(value);
        m_operator = rule;
        m_compare_by_value = true;
    }

    template<class T, class U>
    explicit Rule(T* base_pointer, RuleOperator rule, U* value)
    {
        m_base_pointer = PolymorphicTypes::VariantPointer(base_pointer);
        m_compare_pointer = PolymorphicTypes::VariantPointer(value);
        m_operator = rule;
        m_compare_by_value = false;
    }

    bool run() const;
};


class Validator
{
    std::list<Rule> m_rules;

public:

    void addRule(Rule rule);
    void clearRules();

    bool validate() const;
};


}

#endif // VALIDATOR_H
