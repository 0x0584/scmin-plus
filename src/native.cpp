// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   native.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/18 23:07:20 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

// FIXME: unify the return value on failure

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

sexpr_t sexpr::native_set(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 2) {
        cerr << "Err: expecting two arguments!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot set constant values!" << endl;
        return nil();
    }
    bindings[any_cast<sexpr_text>(*args->car()->blob).text()] =
        args->cdr()->car();
    return symb("t");
}

sexpr_t sexpr::native_unset(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "Err: expecting one argument!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot set constant values!" << endl;
        return nil();
    }
    bindings.erase(any_cast<sexpr_text>(*args->car()->blob).text());
    return symb("t");
}
