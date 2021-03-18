#include "validator.h"

namespace Validation {


Rule::Rule() {}

Rule::Rule(PolymorphicTypes::VariantPointer base_pointer,
           RuleOperator rule,
           PolymorphicTypes::VariantPointer compare_pointer)
{
    m_base_pointer = base_pointer;
    m_operator = rule;
    m_compare_pointer = compare_pointer;
    m_compare_by_value = false;
}

Rule::Rule(PolymorphicTypes::VariantPointer base_pointer,
           RuleOperator rule,
           PolymorphicTypes::Variant compare_value)
{
    m_base_pointer = base_pointer;
    m_operator = rule;
    m_compare_value = compare_value;
    m_compare_by_value = true;
}

bool Rule::run() const
{
    PolymorphicTypes::Variant base_value = m_base_pointer.toVariant();
    PolymorphicTypes::Variant compare_value = m_compare_value;
    if (!m_compare_by_value) compare_value = m_compare_pointer.toVariant();

    auto result = base_value.compare(compare_value);
    switch (m_operator) {
        case RuleOperator::Equal:
            return result == 0;
        case RuleOperator::NotEqual:
            return result != 0;
        case RuleOperator::Less:
            return result < 0;
        case RuleOperator::Greater:
            return result > 0;
        default:
            return false;
    }
}

/// VALIDATOR

void Validator::addRule(Rule rule)
{
    m_rules.push_back(rule);
}

void Validator::clearRules()
{
    m_rules.clear();
}

bool Validator::validate() const
{
    bool result = true;
    for (const auto& rule : m_rules)
    {
        result &= rule.run();
    }
    return result;
}

}
