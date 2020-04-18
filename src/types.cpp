// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   types.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/18 22:09:16 by archid-           #+#    #+#             //
//   Updated: 2020/04/18 22:39:47 by archid-          ###   ########.fr       //
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


bool sexpr::sexpr_lambda::bindargs(const sexpr_t& args, const env_t& parent) {
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

sexpr_t sexpr::sexpr_lambda::eval(const sexpr_t& args, const env_t& parent) {
    if (native) return native(args);
    if (not bindargs(args, parent))
        return nullptr;
    return ::eval(body, local);
}
