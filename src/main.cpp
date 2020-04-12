// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:20:52 by archid-           #+#    #+#             //
//   Updated: 2020/04/12 02:16:40 by archid-          ###   ########.fr       //
 //                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"


int main()
{

    sexpr_t bar;
    {
        auto t = cons(str("bla"), cons(str("cool"), nil()));
        auto s = cons(str("ss"), str("sd"));
        auto foo = list(str("bla"), num(4), str("cool"), nil());

        cout << t << endl;
        cout << s << endl;
        cout << foo << endl;

        bar = list(t, s, foo, nil());
    }
    cout << bar << endl;

    cout << "exit!" << endl;
    return 0;
}
