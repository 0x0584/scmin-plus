// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:20:52 by archid-           #+#    #+#             //
//   Updated: 2020/04/11 18:55:39 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

sexpr_t test() {
    return sexpr::symb("foo-bar");
}

int main()
{
    sexpr_t s = test();
    sexpr_t foo = sexpr::cons(sexpr::symb("pp"), sexpr::cons(s, sexpr::nil()));

    cout << boolalpha << endl;

    // typechecking
    cout << "this is a symbol: " << s << endl;
    cout << "this is a list: " << foo << endl;

    cout << "exit!" << endl;
    return 0;
}
