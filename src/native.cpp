// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   native.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/19 20:39:51 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

// FIXME: unify the return values on failure
// FIXME: remove some code redundancy

sexpr_t sexpr::native_add(const sexpr_t& args, env_t& bindings) {
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

sexpr_t sexpr::native_sub(const sexpr_t& args, env_t& bindings) {
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

sexpr_t sexpr::native_mul(const sexpr_t& args, env_t& bindings) {
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

sexpr_t sexpr::native_div(const sexpr_t& args, env_t& bindings) {
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

sexpr_t sexpr::native_cons(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 2) {
        cerr << "cons operator requires exactly two arguments" << endl;
        return nullptr;
    }
    return cons(args->car(), args->cdr()->car());
}

sexpr_t sexpr::native_car(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
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

sexpr_t sexpr::native_cdr(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
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

sexpr_t sexpr::native_setcar(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (not args->car()->issymb() or args->length() != 2) {
        cerr << "Err: cannot set constant!" << endl;
        return nullptr;
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbole is not defined" << endl;
        return nullptr;
    } else if (not itr->second->iscons()) {
        cerr << "Err: Symbol is not bound to a conslist" << endl;
        return nullptr;
    }
    itr->second->setcar(::eval(args->cdr()->car(), bindings));
    return symb("t");
}

sexpr_t sexpr::native_setcdr(const sexpr_t& args, env_t& bindings) {
    if (not args->car()->issymb() or args->length() != 2) {
        cerr << "Err: cannot set constant!" << endl;
        return nullptr;
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbole is not defined" << endl;
        return nullptr;
    } else if (not itr->second->iscons()) {
        cerr << "Err: Symbol is not bound to a conslist" << endl;
        return nullptr;
    }
    itr->second->setcdr(::eval(args->cdr()->car(), bindings));
    return symb("t");
}

sexpr_t sexpr::native_quote(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1 ) {
        cerr << "quote expects only one argument" << endl;
        return nullptr;
    }
    return args->car();
}

sexpr_t sexpr::native_print(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    return args;
}

sexpr_t sexpr::native_ispair(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->ispair() ? symb("t") : nil();
}

sexpr_t sexpr::native_islist(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->islist() ? symb("t") : nil();
}

sexpr_t sexpr::native_isnum(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isnum() ? symb("t") : nil();
}

sexpr_t sexpr::native_isnil(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isnil() ? symb("t") : nil();
}

sexpr_t sexpr::native_isstr(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isstr() ? symb("t") : nil();
}

sexpr_t sexpr::native_issymb(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->issymb() ? symb("t") : nil();
}

sexpr_t sexpr::native_islambda(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->islambda() ? symb("t") : nil();
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

sexpr_t sexpr::native_n_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, !=) ? symb("t") : nil();
}

sexpr_t sexpr::native_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, ==) ? symb("t") : nil();
}

sexpr_t sexpr::native_gt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >) ? symb("t") : nil();
}

sexpr_t sexpr::native_gt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >=) ? symb("t") : nil();
}

sexpr_t sexpr::native_lt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <) ? symb("t") : nil();
}

sexpr_t sexpr::native_lt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <=) ? symb("t") : nil();
}


sexpr_t sexpr::native_define(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 2) {
        cerr << "Err: define expects two arguments" << endl;
        return nullptr;
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot set constant!" << endl;
        return nullptr;
    }
    auto sy = any_cast<sexpr_text>(*args->car()->blob).text();
    if (bindings.find(sy) != end(bindings)) {
        cerr << "Err: symbol is already defined, use set! instead" << endl;
        return nil();
    }
    bindings[sy] = ::eval(args->cdr()->car(), bindings);
    return symb("t");
}

sexpr_t sexpr::native_set(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 2) {
        cerr << "Err: expecting two arguments!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot set constant values!" << endl;
        return nil();
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbol is not defined, use define instead" << endl;
        return nil();
    }
    itr->second = ::eval(args->cdr()->car(), bindings);
    return symb("t");
}

sexpr_t sexpr::native_unset(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "Err: expecting one argument!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot unset constant values!" << endl;
        return nil();
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbol is not defined" << endl;
        return nil();
    }
    bindings.erase(itr);
    return symb("t");
}

sexpr_t sexpr::native_eval(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "Err: eval expects one argument" << endl;
        return nullptr;
    }
    return ::eval(args->car(), bindings);
}
