// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   handy.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/24 19:38:10 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 19:38:28 by archid-          ###   ########.fr       //
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
