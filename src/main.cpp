// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:20:52 by archid-           #+#    #+#             //
//   Updated: 2020/04/15 00:52:03 by archid-          ###   ########.fr       //
 //                                                                            //
// ************************************************************************** //

#include "repl.hpp"

int main()
{
    vector<string> good = {
        // numbers
        "10", "   20", "   20     ", "20      ",
        "+10", "   -20", "   +20     ", "+20      ",
        "78.34", "78.34      ", "   +78.34",
        "   78.34     ",

        // symbols
        "foo", "   bar", "foobar  ", "foo-bar", "   foo-bar", "foo-bar   ",
        "-+10", "   -+20",
        "fo#$o", "   +-bar", "-foo+bar  ", "foo-bar",

        // strings
        "\"this is a string\"",  "\"this + 11 is a strddd sing\"",

        // conslists
        "(op . po)", "   (op  op)", "(op param1         param2)",
        "(op \"string arg\" 2)", "(op (\"string arg\") 2)",
        "(op\"this\"2)", "(1(2(3(4)5)6)7)",
        "('a 'b \" sdsd \" (quote bb))", "'(a . b)",

        // nil
        "'()", "nil",

        // lambdas
        "(lambda () ())", "(lambda () ((cons '() '())))", "(lambda (z) (e))",
        "(lambda (x y) (foo baz baz))", "(lambda () (4))",
        "((lambda (x y) (+ x y)) 4 5)", "(define foo (lambda (x y) (+ x y)))",

    }, bad = {
        "(gg 1 3", "one two)", ")()", "\"sssdsd"
    };

    int i = 0;
    int err = 0;

    // string str = "(lambda (z) (e))";
    for (auto str : good) {
        sexpr_t e;
        cout << "#" << i++ << " [" << str << "] >> "
             << (e = parse(str)) << endl;
        if (e == nullptr) err++;
    }

    // cout << parse(str) << endl;
    cout << "exit! " << err << "/" << i << endl;
    // if (err) return -1;
    return 0;
}
