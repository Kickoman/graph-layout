#ifndef FUNCTION_COMPILER_H
#define FUNCTION_COMPILER_H
#include <string>
#include <vector>
#include <math.h>
#include <stack>

enum UnitType{NUMBER, VARIABLE, FUNCTION, OPERATOR, UNDEFINED};

namespace {

template<typename T>
T sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

}

template<class T>
class Function
{
public:
    Function();
    Function(const std::string& source);
    virtual ~Function();

    int set_function(const std::string& source);
    T run(T xvalue) const;

    Function& operator=(const std::string& source);
    T operator()(T xvalue) const;

protected:

    struct Unit;
    typedef std::vector<Unit> RPN;


    std::string expression;
    RPN m_rpn_expr;

    static bool is_operator(char c);
    int priority(char c) const;

    RPN convert(const std::string& source);
    std::string simplify(const std::string& source);
private:
};

template<class T>
struct Function<T>::Unit
{
    typedef double (*_Function)(double);

    UnitType type;
    T value;
    _Function function;
    char operation;

    Unit() = default;
    explicit Unit(const T& x) : type(NUMBER), value(x) {}
    explicit Unit(const std::string& s) {
        if (s.compare("x") == 0) {
            type = VARIABLE;
        } else {
            type = FUNCTION;
            if (s.compare("sin") == 0)
                function = sin;
            else if (s.compare("cos") == 0)
                function = cos;
            else if (s.compare("asin") == 0)
                function = asin;
            else if (s.compare("acos") == 0)
                function = acos;
            else if (s.compare("sgn") == 0)
                function = sgn;
            else if (s.compare("floor") == 0)
                function = floor;
            else if (s.compare("exp") == 0)
                function = exp;
            else if (s.compare("log10") == 0)
                function = log10;
            else
                function = tan;
        }
    }
    explicit Unit(char c) {
        type = OPERATOR;
        if (Function<T>::is_operator(c))
            operation = c;
        else
            operation = '*';
    }
};






template<class T>
Function<T>::Function()
{
    //ctor
}

template<class T>
Function<T>::Function(const std::string &source)
{
    set_function(source);
}

template<class T>
Function<T>::~Function()
{
    //dtor
}

template<class T>
int Function<T>::set_function(const std::string& source)
{
    expression = source;
    m_rpn_expr = convert(simplify(expression));
    return 0;
}

template<class T>
std::string Function<T>::simplify(const std::string& source)
{
    std::string result = "";

    for (size_t i = 0; i < source.size(); ++i)
    {
        if (isspace(source[i])) continue;

        if (is_operator(source[i]) && source[i] != '(' && source[i] != ')')
            if (i == 0 || source[i - 1] == '(')
            {
                result += '0';
            }

        result += source[i];
        if (!isalpha(source[i]) || source[i] == 'x') { continue; }

        size_t j;
        for (j = i + 1; j < source.size() && isalpha(source[j]); ++j)
            result += source[j];
        result += '#';
        i = j - 1;
    }

    return result;
}

template<class T>
bool Function<T>::is_operator(char c)
{
    return (
        c == '*' || c == '/' || c == '%' ||
        c == '+' || c == '-' ||
        c == '^' || c == '#' ||
        c == '(' || c == ')'
        );
}

template<class T>
int Function<T>::priority(char c) const
{
    if (c == '#')
        return 5;
    if (c == '^')
        return 4;
    if (c == '*' || c == '/' || c == '%')
        return 3;
    if (c == '+' || c == '-')
        return 2;
    if (c == '(')
        return 1;
    return 0;
}

template<class T>
T Function<T>::run(T xvalue) const
{
    std::stack<Unit> st;
    T result = 0;

    for (size_t i = 0; i < m_rpn_expr.size(); ++i)
    {
        Unit cur = m_rpn_expr[i];

        if (cur.type == OPERATOR)
        {
            Unit val2 = st.top(); st.pop();
            Unit val1 = st.top(); st.pop();

            switch (cur.operation)
            {
            case '+':
                result = val1.value + val2.value;
                break;
            case '-':
                result = val1.value - val2.value;
                break;
            case '*':
                result = val1.value * val2.value;
                break;
            case '/':
                result = val1.value / val2.value;
                break;
            case '%':
                result = static_cast<long long>(val1.value) %
                         static_cast<long long>(val2.value);
                break;
            case '^':
                result = pow(val1.value, val2.value);
                break;
            case '#':
                result = val1.function(val2.value);
                break;
            default:
                result = 0;
            }

            st.push(Unit(result));
        } else {
            if (cur.type == VARIABLE)
                st.push(Unit(xvalue));
            else
                st.push(cur);
        }
    }
    return result;
}

template<class T>
Function<T>& Function<T>::operator=(const std::string &source)
{
    set_function(source);
    return *this;
}

template<class T>
T Function<T>::operator()(T xvalue) const
{
    return run(xvalue);
}


template<class T>
typename Function<T>::RPN Function<T>::convert(const std::string& s)
{
    std::stack<Unit> st;
    Function<T>::RPN result;

    std::string buffer = "";
    for (size_t i = 0; i < s.size(); ++i)
    {

        if (s[i] == '(') {
            st.push(Unit((char('('))));
            continue;
        }
        if (s[i] == ')') {
            if (buffer != "") {
                if (isdigit(buffer[0])) {
                    // that's number
                    char *kek;
                    Unit tmp(T(strtod(buffer.c_str(), &kek)));
                    result.push_back(tmp);
                } else if (buffer == "x") {
                    Unit tmp("x");
                    result.push_back(tmp);
                } else {
                    Unit tmp(buffer);
                    result.push_back(tmp);
                }
                buffer = "";
            }

            while (!st.empty() && st.top().operation != '(')
            {
                result.push_back(st.top());
                st.pop();
            }
            if (!st.empty() && st.top().operation == '(') st.pop();
            continue;
        }
        if (is_operator(s[i])) {
            if (buffer != "") {

                if (isdigit(buffer[0])) {
                    // that's number
                    char *kek;
                    Unit tmp(T(strtod(buffer.c_str(), &kek)));
                    result.push_back(tmp);
                } else if (buffer == "x") {
                    Unit tmp("x");
                    result.push_back(tmp);
                } else {
                    Unit tmp(buffer);
                    result.push_back(tmp);
                }
            }
            while (!st.empty() && priority(st.top().operation) >= priority(s[i]))
            {
                result.push_back(st.top());
                st.pop();
            }
            st.push(Unit(s[i]));

            buffer = "";
        } else {
            buffer += s[i];
        }
    }
    if (buffer != "") {
        if (isdigit(buffer[0])) {
            // that's number
            char *kek;
            Unit tmp(T(strtod(buffer.c_str(), &kek)));
            result.push_back(tmp);
        } else if (buffer == "x") {
            Unit tmp("x");
            result.push_back(tmp);
        } else {
            Unit tmp(buffer);
            result.push_back(tmp);
        }
    }
    while (!st.empty())
    {
        result.push_back(st.top());
        st.pop();
    }
    return result;
}


#endif // FUNCTION_COMPILER_H
