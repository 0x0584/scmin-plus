// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   native.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/18 22:01:15 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

sexpr_t sexpr::native_add(const sexpr_t& args) {
    sexpr_t walk = args;
    double d = 0;

    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!" << endl;
            return nullptr;
        }
        d += any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

sexpr_t sexpr::native_sub(const sexpr_t& args) {
    if (not args->car()->isnum()) {
        cerr << "fatal: operand is not a number!" << endl;
        return nullptr;
    }

    double d = any_cast<sexpr_number>(*args->car()->blob).n;

    if (args->length() == 1)
        return num(-d);

    sexpr_t walk = args->cdr();

    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!" << endl;
            return nullptr;
        }
        d -= any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

sexpr_t sexpr::native_mul(const sexpr_t& args) {
    sexpr_t walk = args;
    double d = 1;

    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!" << endl;
            return nullptr;
        }
        d *= any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

sexpr_t sexpr::native_div(const sexpr_t& args) {
    sexpr_t walk = args;
    double d = 0;

    // while (not walk->isnil()) {
    //     if (not walk->car()->isnum()) {
    //         cerr << "fatal: operands are not numbers!" << endl;
    //         return nullptr;
    //     }
    //     d /= any_cast<sexpr_number>(*walk->car()->blob).n;
    //     walk = walk->cdr();
    // }
    return num(d);
}

sexpr_t sexpr::native_cons(const sexpr_t& args) {
    if (args->length() != 2) {
        cerr << "cons operator requires exactly two arguments" << endl;
        return nullptr;
    }
    return cons(args->car(), args->cdr()->car());
}

sexpr_t sexpr::native_car(const sexpr_t& args) {
    if (args->length() != 1) {
        cerr << "car expects only one argument" << endl;
        return nullptr;
    }
    if (not (args->car()->islist() or args->car()->ispair())) {
        // cerr << args << endl;
        cerr << "car is applicable only on conslists" << endl;
        return nullptr;
    }
    return args->car()->car();
}

sexpr_t sexpr::native_cdr(const sexpr_t& args) {
    if (args->length() != 1) {
        cerr << "cdr expects only one argument" << endl;
        return nullptr;
    }
    if (not (args->car()->islist() or args->car()->ispair())) {
        // cerr << args << endl;
        cerr << "car is applicable only on conslists" << endl;
        return nullptr;
    }
    return args->car()->cdr();
}

sexpr_t sexpr::native_quote(const sexpr_t& args) {
    if (args->length() != 1 ) {
        cerr << "quote expects only one argument" << endl;
        return nullptr;
    }
    return args->car();
}

sexpr_t sexpr::native_print(const sexpr_t& args) {
    return args;
}
