// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:20:52 by archid-           #+#    #+#             //
//   Updated: 2020/04/13 00:18:17 by archid-          ###   ########.fr       //
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
        "(op param1         param2)", "(op \"string arg\" 2)",
        "(op (\"string arg\") 2)", "(op . po)", "   (op  op)",
        "(op\"this\"2)", "(1(2(3(4)5)6)7)"
        // nil
        "nil"
    }, bad = {
        "(gg 1 3", "one two)", ")()", "\"sssdsd"
    };

    int i = 0;
    int err = 0;
    // string str = "(op (\"string arg\") 2)";
    for (auto str : good) {
        sexpr_t e;
        cout << "#" << i++ << " [" << str << "] >> "
             << (e = parse(str)) << endl;
        if (e == nullptr) err++;
    }
    cout << "exit! " << err << "/" << i << endl;
    return 0;
}
