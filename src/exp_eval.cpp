#include "exp_eval.hpp"

namespace eval
{
    std::vector<term> parce_input(std::string t_in)
    {
        // remove whitespace
        std::string::iterator end_pos = std::remove(t_in.begin(), t_in.end(), ' ');
        t_in.erase(end_pos, t_in.end());

        std::vector<term> terms;
        int i = 0;
        while (t_in != "")
        {
            if (is_operator(t_in[i])) 
            {
                std::string op = std::string(1, t_in[i]);
                t_in = t_in.substr(i + 1);

                if (!terms.empty())
                {
                    if ((op == "(") && terms.back().m_type != types::op)
                    {
                        term newt;
                        newt.m_data = "*";
                        newt.m_type = types::op;
                        terms.push_back(newt);
                    }
                }

                term newt;
                newt.m_data = op;
                newt.m_type = types::op;
                terms.push_back(newt);

                i = 0;
            } else if (std::isdigit(t_in[i]) && !(std::isdigit(t_in[i + 1]) || t_in[i+1] == '.') )
            {
                std::string num = t_in.substr(0, i + 1);
                t_in = t_in.substr(i + 1);
                i = 0;

                term newt;
                newt.m_data = num;
                newt.m_type = types::num;
                terms.push_back(newt);
            } else if (std::isalpha(t_in[i]) && !std::isalpha(t_in[i+1]))
            {
                std::string word = t_in.substr(0, i + 1);
                t_in = t_in.substr(i + 1);
                i = 0;

                types word_type = types::var;
                if (is_function(word))
                {
                    word_type = types::func;
                } else if (is_constant(word))
                {
                    word_type = types::constant;
                }

                if (word_type == types::var)
                {
                    for (int x = 0; x < word.size(); x++)
                    {
                        if (!terms.empty())
                        {
                            if (terms.back().m_type != types::op)
                            {
                                term newt;
                                newt.m_data = "*";
                                newt.m_type = types::op;
                                terms.push_back(newt);
                            }
                        }

                        term newt;
                        newt.m_data = std::string(1, word[x]);
                        newt.m_type = word_type;
                        terms.push_back(newt);
                    }
                    
                } else if (word_type == types::func)
                {
                    if (!terms.empty())
                    {
                        if (terms.back().m_type != types::op)
                        {
                            term newt;
                            newt.m_data = "*";
                            newt.m_type = types::op;
                            terms.push_back(newt);
                        }
                    }

                    if (t_in[i] != '(')
                    {
                        throw std::invalid_argument("expected function arguments in parenthasies");
                    }

                    t_in = t_in.substr(i + 1);
                    int pcount = 0;
                    int z;
                    for (z = 0; z < t_in.size(); z++)
                    {
                        if (t_in[z] == '(')
                        {
                            pcount++;
                        } else if (t_in[z] == ')')
                        {
                            pcount--;
                        }

                        if (pcount < 0)
                        {
                            break;
                        }
                    }

                    std::string args = t_in.substr(0, z);
                    t_in = t_in.substr(z + 1);

                    term newt;
                    newt.m_data = word;
                    newt.m_type = word_type;
                    newt.m_functionArgs = parce_input(args);
                    infix_to_postfix(newt.m_functionArgs);
                    terms.push_back(newt);
                    
                } else {
                    if (!terms.empty())
                    {
                        if (terms.back().m_type != types::op)
                        {
                            term newt;
                            newt.m_data = "*";
                            newt.m_type = types::op;
                            terms.push_back(newt);
                        }
                    }

                    term newt;
                    newt.m_data = word;
                    newt.m_type = word_type;
                    terms.push_back(newt);
                }
            } else {
                i++;
            }
        }

        infix_to_postfix(terms);

        return terms;
    }

    bool is_operator(char t_in)
    {
        bool op = false;
        if (t_in == '+' || t_in == '-' || t_in == '*' || t_in == '/')
        {
            op = true;
        } else if (t_in == '^' || t_in == '%')
        {
            op = true;
        } else if (t_in == '(' || t_in == ')') {
            op = true;
        }

        return op;
    }

    bool is_function(std::string t_in)
    {
        if (t_in == "sin" || t_in == "cos" || t_in == "tan")
        {
            return true;
        }

        if (t_in == "fact" || t_in == "sqrt")
        {
            return true;
        }

        if (t_in == "ln" || t_in == "log" || t_in == "exp")
        {
            return true;
        }

        if (t_in == "asin" || t_in == "acos" || t_in == "atan")
        {
            return true;
        }

        return false;
    }

    bool is_constant(std::string t_in)
    {
        if (t_in == "e" || t_in == "pi")
        {
            return true;
        }

        return false;
    }

    std::string types_to_string(types t_type)
    {
        switch (t_type)
        {
        case types::constant:
            return "constant";
        case types::func:
            return "function";
        case types::num:
            return "number";
        case types::op:
            return "operator";
        case types::var:
            return "variable";
        }
    }

    int prec(char c)
    {
        if (c == '^')
        {
            return 3;
        } else if (c == '/' || c == '*') {
            return 2;
        } else if (c == '+' || c == '-') {
            return 1;
        } else {
            return -1;
        }
    }

    void infix_to_postfix(std::vector<term> &t_terms)
    {
        std::stack<term> st;
        std::vector<term> result;

        for (int i = 0; i < t_terms.size(); i++)
        {
            term cterm = t_terms[i];
            if (cterm.m_type != types::op)
            {
                result.push_back(cterm);
            } else if (cterm.m_data == "(") {
                st.push(cterm);
            } else if (cterm.m_data == ")") {
                while (st.top().m_data != "(")
                {
                    result.push_back(st.top());
                    st.pop();
                }
                st.pop();
            } else {
                while (!st.empty() && prec(cterm.m_data[0]) <= prec(st.top().m_data[0]))
                {
                    if (cterm.m_data == "^" && st.top().m_data == "^")
                    {
                        break;
                    } else {
                        result.push_back(st.top());
                        st.pop();
                    }
                }
                st.push(cterm);
            }
        }

        while (!st.empty())
        {
            result.push_back(st.top());
            st.pop();
        }

        t_terms.clear();
        t_terms.reserve(result.size());
        for (int i = 0; i < result.size(); i++)
        {
            t_terms.push_back(result[i]);
        }
    }

    adt::tree<term>* create_binary_expression_tree(std::vector<term> t_terms)
    {
        std::stack<adt::tree<term>*> st;
        
        for (int i = 0; i < t_terms.size(); i++)
        {
            term cterm = t_terms[i];
            if (cterm.m_type != types::op)
            {
                adt::tree<term>* newtree = new adt::tree<term>(cterm);
                st.push(newtree);
            } else {
                adt::tree<term>* newtree = new adt::tree<term>(cterm);
                newtree->pushright(st.top());
                st.pop();
                newtree->pushleft(st.top());
                st.pop();
                st.push(newtree);
            }
        }

        return st.top();
    }

    std::ostream& operator<<(std::ostream& t_out, const term &t_term)
    {
        t_out << types_to_string(t_term.m_type) << ": " << t_term.m_data;

        return t_out;
    }

    std::string& operator+=(std::string &t_s, const term &t_term)
    {
        t_s += "[" + types_to_string(t_term.m_type) + ": " + t_term.m_data + "]";

        return t_s;
    }

    float evaluate(adt::tree<term>* t_tree)
    {
        // if tree data is not operator, 
        //  return value
        // else,
        //  A = evaluate(left)
        //  B = evaluate(right)
        //  return A op B

        if (t_tree->data().m_type != types::op)
        {
            // if the value is a number, return the number
            if (t_tree->data().m_type == types::num)
            {
                return std::stod(t_tree->data().m_data);
            }

            // if the value is a constant, return that constant
            if (t_tree->data().m_type == types::constant)
            {
                return const_to_double(t_tree->data().m_data);
            }

            // if the value is a variable, return that variable
            // TODO: this ^
            if (t_tree->data().m_type == types::var)
            {
                // return 1 for now
                return 1;
            }

            // if the value is a function, return that function
            if (t_tree->data().m_type == types::func)
            {
                return func_to_double(t_tree->data());
            }
        } else {
            double A = evaluate(t_tree->left());
            double B = evaluate(t_tree->right());

            if (t_tree->data().m_data == "+")
            {
                return A + B;
            } else if (t_tree->data().m_data == "-")
            {
                return A - B;
            } else if (t_tree->data().m_data == "*")
            {
                return A * B;
            } else if (t_tree->data().m_data == "/")
            {
                return A / B;
            } else if (t_tree->data().m_data == "/")
            {
                return A / B;
            } else if (t_tree->data().m_data == "^")
            {
                return pow(A, B);
            } else if (t_tree->data().m_data == "%")
            {
                // check if a and b are ints
                double intpartA;
                double intpartB;
                if (modf(A, &intpartA) != 0.0 || modf(B, &intpartB) != 0.0)
                {
                    throw std::invalid_argument("modulus funciton only works with integers");
                }

                return int(A) % int(B);
            }
        }

        return 0;
    }

    double const_to_double(std::string t_in)
    {
        if (t_in == "e")
        {
            return E;
        }

        if (t_in == "pi")
        {
            return PI;
        }

        return 0;
    }

    double func_to_double(term t_func)
    {
        // find out the function type and evalue the function args
        auto tree = create_binary_expression_tree(t_func.m_functionArgs);
        double val = 0;

        if (t_func.m_data == "sin")
        {
            val = sin(evaluate(tree));
        } else if (t_func.m_data == "cos") {
            val = cos(evaluate(tree));
        }  else if (t_func.m_data == "tan") {
            val = tan(evaluate(tree));
        } else if (t_func.m_data == "fact") {
            val = fact(evaluate(tree));
        }  else if (t_func.m_data == "sqrt") {
            val = sqrt(evaluate(tree));
        } else if (t_func.m_data == "ln") {
            val = log(evaluate(tree));
        } else if (t_func.m_data == "log") {
            val = log10(evaluate(tree));
        } else if (t_func.m_data == "exp") {
            val = exp(evaluate(tree));
        } else if (t_func.m_data == "asin") {
            val = asin(evaluate(tree));
        } else if (t_func.m_data == "acos") {
            val = acos(evaluate(tree));
        } else if (t_func.m_data == "atan") {
            val = atan(evaluate(tree));
        }

        delete tree;
        return val;
    }

    double fact(double a)
    {
        if (a <= 1)
        {
            return 1;
        }

        return a * fact(a - 1);
    }
}