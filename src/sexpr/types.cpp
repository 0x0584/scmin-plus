// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   types.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/18 22:09:16 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 19:37:26 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"
#include "builtin.hpp"

string sexpr::sexpr_text::text() {
    if (sy) return s;
    else return "\"" + s + "\"";
}

bool sexpr::sexpr_conslist::islist() {
    auto walk = cdr;
    while (walk->iscons())
        walk = walk->cdr();
    return walk->isnil();
}

bool sexpr::sexpr_conslist::ispair() {
    return not islist();
}

bool sexpr::sexpr_lambda::bindargs(const sexpr_t& args, env_t& parent) {
    auto u = this->args, v = args;

    if (u->length() != v->length()) {
        cerr << "argument count mismatched, expected "
             << u->length() << "got " << v->length()
             << endl;
        return false;
    }

    local = parent;
    while (not u->isnil()) {
        if (not u->car()->issymb()) {
            cerr << "Err: argument is not a symbol" << endl;
            return false;
        }
        local[any_cast<sexpr_text>(*u->car()->blob).text()] = v->car();
        u = u->cdr(); v = v->cdr();
    }
    return true;
}

bool sexpr::sexpr_lambda::require_evaled_args() {
    return not (native == builtin::_quote
                or native == builtin::_quasiquote
                or native == builtin::_define
                or native == builtin::_set
                or native == builtin::_unset
                or native == builtin::_cons_car_flavor
                or native == builtin::_cons_cdr_flavor
                or native == builtin::_if
                or native == builtin::_cond
                or native == builtin::_let
                or native == builtin::_let_astrk
                or native == builtin::_let_rec
                or native == builtin::_do);
}

sexpr_t sexpr::sexpr_lambda::eval(const sexpr_t& args, env_t& parent) {
    if (native) return native(args, parent);
    else if (not bindargs(args, parent)) return nullptr;
    else return ::eval(body, local);
}
