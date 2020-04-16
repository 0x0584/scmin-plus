// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   native.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/16 22:40:39 by archid-          ###   ########.fr       //
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

sexpr_t native_mul(sexpr_t args) {
    return nullptr;
}

sexpr_t native_div(sexpr_t args) {
    return nullptr;
}
