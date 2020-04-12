// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:53:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/12 02:12:40 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

sexpr_t nil() {
    cout << "making nil" << endl;
    return make_shared<sexpr>();
}

sexpr_t symb(const string& s) {
    cout << "making symb " << s << endl;
    return make_shared<sexpr>(s);
}
sexpr_t str(const string& s) {
    cout << "making str " << s << endl;
    return make_shared<sexpr>(s, false);
}

sexpr_t num(double n) {
    cout << "making num " << n << endl;
    return make_shared<sexpr>(n);
}

sexpr_t cons(const sexpr_t& car, const sexpr_t& cdr) {
    cout << "making cons " << car << " " << cdr << endl;
    return make_shared<sexpr>(car, cdr);
}

sexpr_t list(const sexpr_t& car...) {
    stack<sexpr_t> s;
    va_list args;

    s.push(car);
    va_start(args, car);
    do s.push(va_arg(args, sexpr_t));
    while (not s.top()->isnil());
    va_end(args);
    while (s.size() > 1) {
        auto u = s.top(); s.pop();
        auto v = s.top(); s.pop();
        s.push(cons(v, u));
    }
    return s.top();
}

bool sexpr::isnil() {
    return blob->type() == typeid(sexpr_nil);
}

bool sexpr::iscons() {
    return blob->type() == typeid(sexpr_conslist);
}

bool sexpr::islist() {
    return blob->type() == typeid(sexpr_conslist)
        and any_cast<sexpr_conslist>(*blob).islist();
}

bool sexpr::ispair() {
    return blob->type() == typeid(sexpr_conslist)
        and any_cast<sexpr_conslist>(*blob).ispair();
}

bool sexpr::isnum() {
    return blob->type() == typeid(sexpr_number);
}

bool sexpr::isstr() {
    return blob->type() == typeid(sexpr_text)
        and not any_cast<sexpr_text>(*blob).sy;
}

bool sexpr::issymb() {
    return blob->type() == typeid(sexpr_text)
        and any_cast<sexpr_text>(*blob).sy;
}

sexpr_t sexpr::car() {
    return not iscons() ? nullptr
        : any_cast<sexpr_conslist>(*blob).car;
}

sexpr_t sexpr::cdr() {
    return not iscons() ?  nullptr
        : any_cast<sexpr_conslist>(*blob).cdr;
}

sexpr_t sexpr::eval() const {
    return nullptr;
}
