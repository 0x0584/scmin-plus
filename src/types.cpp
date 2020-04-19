// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   types.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/18 22:09:16 by archid-           #+#    #+#             //
//   Updated: 2020/04/19 20:14:03 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"

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
             << u->length() << "got " << v->length();
        return false;
    }

    local = parent;
    while (not u->isnil()) {
        // FIXME: only symbols are allowed
        local[any_cast<sexpr_text>(*u->car()->blob).text()] = v->car();
        u = u->cdr(); v = v->cdr();
    }
    return true;
}

bool sexpr::sexpr_lambda::require_evaled_args() {
    return not (native == native_quote
                or native == native_eval
                or native == native_define
                or native == native_set
                or native == native_unset
                or native == native_setcar
                or native == native_setcdr);
}

sexpr_t sexpr::sexpr_lambda::eval(const sexpr_t& args, env_t& parent) {
    if (native) return native(args, parent);
    else if (not bindargs(args, parent)) return nullptr;
    else return ::eval(body, local);
}
