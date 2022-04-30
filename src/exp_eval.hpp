#ifndef exp_eval_hpp
#define exp_eval_hpp

#include <iostream>
#include <vector>
#include <stack>
#include <math.h>

#include "tree.hpp"

namespace eval
{
    // mathamatical constants
    const double PI = 3.14159265359;
    const double  E = 2.71828182845;

    // enum to store different types of input
    enum types
    {
        num,
        var,
        func,
        op,
        constant
    };

    // stores an individual term of an expression
    struct term
    {
        std::string m_data;
        types m_type;

        std::vector<term> m_functionArgs;

        friend std::ostream& operator<<(std::ostream& t_out, const term &t_term);
        friend std::string& operator+=(std::string &t_s, const term &t_term);
    };

    // check if the given charicter is an operator
    bool is_operator(char t_in);

    // check if the given string is a funciton
    bool is_function(std::string t_in);

    // check if the given string is a constant
    bool is_constant(std::string t_in);

    // types enum to string
    std::string types_to_string(types t_type);

    // parce input and convert to a list of terms
    std::vector<term> parce_input(std::string t_in);

    // simplify a list of terms
    // * group together like terms
    void simplify(std::vector<term> &t_in);

    // create a binary expression tree from a list of terms
    adt::tree<term>* create_binary_expression_tree(std::vector<term> t_terms);

    // determine presidance between operators
    int prec(char c);

    // convert infix input to postfix
    void infix_to_postfix(std::vector<term> &t_terms);

    // evalulate binary expression tree
    // * TODO: variables
    float evaluate(adt::tree<term>* t_tree);

    // convert the constant string value to double
    double const_to_double(std::string t_in);

    // convert and evalute the func
    double func_to_double(term t_func);

    // returns the factorail of the input
    double fact(double);
}

#endif