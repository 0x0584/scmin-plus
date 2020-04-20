// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:53:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/20 03:40:02 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

sexpr_t nil() {
    return make_shared<sexpr>();
}

sexpr_t symb(const string& s) {
    return make_shared<sexpr>(s);
}

sexpr_t str(const string& s) {
    return make_shared<sexpr>(s, false);
}

sexpr_t num(double n) {
    return make_shared<sexpr>(n);
}

sexpr_t lambda(const sexpr_t& args, const sexpr_t& body) {
    return make_shared<sexpr>(cons(args, body));
}

sexpr_t native(builtin_lambda fn) {
    return make_shared<sexpr>(fn);
}

sexpr_t cons(const sexpr_t& car, const sexpr_t& cdr) {
    return make_shared<sexpr>(car, cdr);
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

bool sexpr::isnil() {
    return blob->type() == typeid(sexpr_nil);
}

bool sexpr::iscons() {
    return blob->type() == typeid(sexpr_conslist *);
}

bool sexpr::islist() {
    return blob->type() == typeid(sexpr_conslist *)
        and any_cast<sexpr_conslist *>(*blob)->islist();
}

bool sexpr::ispair() {
    return blob->type() == typeid(sexpr_conslist *)
        and any_cast<sexpr_conslist *>(*blob)->ispair();
}

bool sexpr::isatom() {
    return isnum() or isstr() or issymb();
}

bool sexpr::isnum() {
    return blob->type() == typeid(sexpr_number);
}

bool sexpr::islambda() {
    return blob->type() == typeid(sexpr_lambda);
}

bool sexpr::isstr() {
    return blob->type() == typeid(sexpr_text)
        and not any_cast<sexpr_text>(*blob).sy;
}

bool sexpr::issymb() {
    return blob->type() == typeid(sexpr_text)
        and any_cast<sexpr_text>(*blob).sy;
}

bool sexpr::setcar(const sexpr_t& e) {
    if (not (e->ispair() or e->islist()))
        return false;
    any_cast<sexpr_conslist *>(*blob)->car = e;
    return true;
}

bool sexpr::setcdr(const sexpr_t& e) {
    if (not (e->ispair() or e->islist()))
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

size_t sexpr::length() {
    size_t sz = 1;
    sexpr_t walk;

    if (isatom() or ispair())
        return 0;
    walk = cdr();
    while (not walk->isnil())
        sz++, walk = walk->cdr();
    return sz;
}

sexpr_t eval_args(const sexpr_t& args, env_t& parent) {
    sexpr_t walk;
    sexpr_t tail;
    sexpr_t evaled;

    if (args->isnil())
        return args;
    walk = args;
    while (not walk->isnil()) {
        auto expr = cons(eval(walk->car(), parent), nil());
        if (not evaled)
            tail = evaled = expr;
        else {
            tail->setcdr(expr);
            tail = tail->cdr();
        }
        walk = walk->cdr();
    }
    return evaled;
}

sexpr_t sexpr::eval(const sexpr_t& args, env_t& bindings) {
    auto lamb = any_cast<sexpr_lambda>(*blob);
    if (not lamb.require_evaled_args())
        return lamb.eval(args, bindings);
    return lamb.eval(eval_args(args, bindings), bindings);
}

sexpr_t eval(const sexpr_t& expr, env_t& parent) {
    sexpr_t op;
    sexpr_t res;

    if (not expr->islist())
        return sexpr::resolve(expr, parent);
    op = eval(expr->car(), parent);
    if (not op or not op->islambda()) {
        cerr << "Err: operator is not a lambda" << endl;
        return nullptr;
    }
    res = op->eval(expr->cdr(), parent);
    return sexpr::resolve(res, parent);
}
