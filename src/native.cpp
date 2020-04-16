// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   native.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/17 00:22:19 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

sexpr_t sexpr::native_add(const sexpr_t& args, const bonds_t& bonds) {
    sexpr_t walk = args;
    double d = 0;

    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!";
            return nullptr;
        }
        d += any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

sexpr_t sexpr::native_cons(const sexpr_t& args, const bonds_t& bonds) {
    if (args->length() != 2) {
        cerr << "cons operator requires exactly two arguments" << endl;
        return nullptr;
    }
    return cons(args->car(), args->cdr()->car());
}

sexpr_t sexpr::native_car(const sexpr_t& args, const bonds_t& bonds) {
    if (not args->islist() or not args->ispair()) {
        // cerr << args << endl;
        cerr << "car is applicable only on conslists" << endl;
        return nullptr;
    }
    return args->car();
}

sexpr_t sexpr::native_cdr(const sexpr_t& args, const bonds_t& bonds) {
    if (not args->islist() or not args->ispair()) {
        cerr << "cdr is applicable only on conslists" << endl;
        return nullptr;
    }
    return args->cdr()->car();
}

sexpr_t sexpr::native_quote(const sexpr_t& args, const bonds_t& bonds) {
    return args->car();
}

sexpr_t sexpr::native_print(const sexpr_t& args, const bonds_t& bonds) {
    sexpr_t walk = args;

    while (not walk->isnil()) {
        cout << walk->car();
        walk = walk->cdr();
        cout << (walk->isnil() ? "\n" : " ");
    }
    return nil();
}

sexpr_t native_mul(sexpr_t args) {
    return nullptr;
}

sexpr_t native_div(sexpr_t args) {
    return nullptr;
}
