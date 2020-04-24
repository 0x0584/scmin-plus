// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   cons.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/24 18:55:42 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 18:56:14 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

bool cons_append(const sexpr_t& e, sexpr_t& expr, sexpr_t& tail) {
    if (e == nullptr)
        return false;
    if (not expr)
        tail = expr = cons(e, nil());
    else {
        tail->setcdr(cons(e, nil()));
        tail = tail->cdr();
    }
    return true;
}

// clang is really strict!!
// sexpr_t list(const sexpr_t& car...) {
//     stack<sexpr_t> s;
//     va_list args;

//     s.push(car);
//     va_start(args, car);
//     do s.push(va_arg(args, sexpr_t));
//     while (not s.top()->isnil());
//     va_end(args);
//     while (s.size() > 1) {
//         auto u = s.top(); s.pop();
//         auto v = s.top(); s.pop();
//         s.push(cons(v, u));
//     }
//     return s.top();
// }

bool sexpr::setcar(const sexpr_t& e) {
    if (not (ispair() or islist()))
        return false;
    any_cast<sexpr_conslist *>(*blob)->car = e;
    return true;
}

bool sexpr::setcdr(const sexpr_t& e) {
    if (not (ispair() or islist()))
        return false;
    any_cast<sexpr_conslist *>(*blob)->cdr = e;
    return true;
}

sexpr_t sexpr::car() {
    return not iscons() ? nullptr
        : any_cast<sexpr_conslist *>(*blob)->car;
}

sexpr_t sexpr::cdr() {
    return not iscons() ?  nullptr
        : any_cast<sexpr_conslist *>(*blob)->cdr;
}
