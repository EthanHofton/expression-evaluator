#include <iostream>

#include "exp_eval.hpp"
#include "tree.hpp"

int main(int argc, char **argv)
{
    auto expression = eval::parce_input("atan(1)");

    auto exptree = eval::create_binary_expression_tree(expression);

    std::cout << eval::evaluate(exptree) << std::endl;

    // std::cout << eval::simplify(exptree) << std::endl;

    delete exptree;

    return 0;
}