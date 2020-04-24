// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   arithmetic.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/24 18:44:47 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 18:46:06 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"

sexpr_t builtin::_add(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
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

sexpr_t builtin::_sub(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
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

sexpr_t builtin::_mul(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
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

sexpr_t builtin::_div(const sexpr_t& args, env_t& bindings) {
    (void)bindings;

    if (not args->car()->isnum()) {
        cerr << "fatal: operands are not numbers!" << endl;
        return nullptr;
    }
    double d = any_cast<sexpr_number>(*args->car()->blob).n;
    sexpr_t walk = args->cdr();
    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!" << endl;
            return nullptr;
        }
        d /= any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

#define cmp_helper(u, v, cmp) (any_cast<sexpr_number>(*u->blob).n \
                               cmp any_cast<sexpr_number>(*v->blob).n)

bool valid_numeric_args(sexpr_t args) {
    if (args->length() != 2) {
        cerr << "Err: expected two arguments!" << endl;
        return false;
    }
    if (not (args->car()->isnum() and args->cdr()->car()->isnum())) {
        cerr << "Err: arguments should both be numbers!" << endl;
        return false;
    }
    return true;
}

sexpr_t builtin::_n_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, !=) ? symb("t") : nil();
}

sexpr_t builtin::_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, ==) ? symb("t") : nil();
}

sexpr_t builtin::_gt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >) ? symb("t") : nil();
}

sexpr_t builtin::_gt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >=) ? symb("t") : nil();
}

sexpr_t builtin::_lt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <) ? symb("t") : nil();
}

sexpr_t builtin::_lt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <=) ? symb("t") : nil();
}
