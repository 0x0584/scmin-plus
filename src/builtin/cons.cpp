// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   cons.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/24 18:43:49 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 23:29:57 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"

sexpr_t builtin::_cons(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 2) {
        cerr << "cons operator requires exactly two arguments" << endl;
        return nullptr;
    }
    return cons(args->car(), args->cdr()->car());
}

sexpr_t builtin::_car(const sexpr_t& args, env_t& bindings) {
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

sexpr_t builtin::_cdr(const sexpr_t& args, env_t& bindings) {
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

// sexpr_t _setcdr(const sexpr_t& args, env_t& bindings) {
//     if (not args->car()->issymb() or args->length() != 2) {
//         cerr << "Err: cannot set constant!" << endl;
//         return nil();
//     }
//     auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
//     if (itr == end(bindings)) {
//         cerr << "Err: symbole is not defined" << endl;
//         return nil();
//     } else if (not itr->second->iscons()) {
//         cerr << "Err: Symbol is not bound to a conslist" << endl;
//         return nil();
//     }
//     itr->second->setcdr(::eval(args->cdr()->car(), bindings));
//     return symb("t");
// }

static sexpr_t _cons_flavor(bool cdr, const sexpr_t& args, env_t& bindings) {
    sexpr_t e, set;

    if (args->length() != 2) {
        cerr << "Err: operator expect two arguments" << endl;
        return nullptr;
    }
    if (e = ::eval(args->car(), bindings); not (e and e->iscons())) {
        cerr << "Err: cannot set constant!" << endl;
        return nil();
    }
    set = ::eval(args->cdr()->car(), bindings);
    if (cdr) e->setcdr(set);
    else e->setcar(set);
    sexpr_t walk = args->car();
    while (not walk->issymb())
        walk = walk->cdr()->car();
    return ::eval(walk, bindings);
}

sexpr_t builtin::_cons_car_flavor(const sexpr_t& args, env_t& bindings) {
    return _cons_flavor(false, args, bindings);
}

sexpr_t builtin::_cons_cdr_flavor(const sexpr_t& args, env_t& bindings) {
    return _cons_flavor(true, args, bindings);
}
