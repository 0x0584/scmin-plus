// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   types.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/24 18:49:08 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 18:49:21 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"


sexpr_t builtin::_ispair(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->ispair() ? symb("t") : nil();
}

sexpr_t builtin::_islist(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->islist() ? symb("t") : nil();
}

sexpr_t builtin::_isnum(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isnum() ? symb("t") : nil();
}

sexpr_t builtin::_isnil(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isnil() ? symb("t") : nil();
}

sexpr_t builtin::_isstr(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->isstr() ? symb("t") : nil();
}

sexpr_t builtin::_issymb(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->issymb() ? symb("t") : nil();
}

sexpr_t builtin::_islambda(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: expect one argument" << endl;
        return nullptr;
    }
    return args->car()->islambda() ? symb("t") : nil();
}
