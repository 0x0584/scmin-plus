// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   env.cpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/18 22:04:59 by archid-           #+#    #+#             //
//   Updated: 2020/04/23 09:08:05 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"

sexpr_t sexpr::resolve(const sexpr_t& expr, env_t& local) {
    if (not (expr and expr->issymb()))
        return expr;
    auto sy = any_cast<sexpr_text>(*expr->blob).text();
    auto it = local.find(sy);
    if (it != end(local))
        return it->second;
    it = global.find(sy);
    if (it != end(global))
        return it->second;
    else return expr;
}

void sexpr::init_global() {
    sexpr::global["add"]        = native(builtin::_add);
    sexpr::global["+"]          = native(builtin::_add);
    sexpr::global["sub"]        = native(builtin::_sub);
    sexpr::global["-"]          = native(builtin::_sub);
    sexpr::global["mul"]        = native(builtin::_mul);
    sexpr::global["*"]          = native(builtin::_mul);
    sexpr::global["div"]        = native(builtin::_div);
    sexpr::global["/"]          = native(builtin::_div);

    sexpr::global["gt?"]        = native(builtin::_gt);
    sexpr::global[">"]          = native(builtin::_gt);
    sexpr::global["ge?"]        = native(builtin::_gt_eq);
    sexpr::global["> ="]         = native(builtin::_gt_eq);
    sexpr::global["lt?"]        = native(builtin::_lt);
    sexpr::global["<"]          = native(builtin::_lt);
    sexpr::global["le?"]        = native(builtin::_lt_eq);
    sexpr::global["< ="]         = native(builtin::_lt_eq);
    sexpr::global["eq?"]        = native(builtin::_eq);
    sexpr::global[" ="]          = native(builtin::_eq);
    sexpr::global["ne?"]        = native(builtin::_n_eq);
    sexpr::global["! ="]         = native(builtin::_n_eq);

    sexpr::global["cons"]       = native(builtin::_cons);
    sexpr::global["cdr"]        = native(builtin::_cdr);
    sexpr::global["car"]        = native(builtin::_car);
    sexpr::global["set-cdr!"]   = native(builtin::_setcdr);
    sexpr::global["set-car!"]   = native(builtin::_setcar);

    sexpr::global["quote"]      = native(builtin::_quote);
    sexpr::global["quasiquote"] = native(builtin::_quasiquote);
    sexpr::global["print"]      = native(builtin::_print);

    sexpr::global["pair?"]      = native(builtin::_ispair);
    sexpr::global["list?"]      = native(builtin::_islist);
    sexpr::global["symbol?"]    = native(builtin::_issymb);
    sexpr::global["string?"]    = native(builtin::_isstr);
    sexpr::global["number?"]    = native(builtin::_isnum);
    sexpr::global["lambda?"]    = native(builtin::_islambda);
    sexpr::global["nil?"]       = native(builtin::_isnil);

    sexpr::global["define"]     = native(builtin::_define);
    sexpr::global["set!"]       = native(builtin::_set);
    sexpr::global["unset!"]     = native(builtin::_unset);
    sexpr::global["eval"]       = native(builtin::_eval);
    sexpr::global["if"]         = native(builtin::_if);
    sexpr::global["and"]        = native(builtin::_and);
    sexpr::global["or"]         = native(builtin::_or);
    sexpr::global["not"]        = native(builtin::_not);
    sexpr::global["cond"]       = native(builtin::_cond);
    sexpr::global["let"]        = native(builtin::_let);
    sexpr::global["let*"]       = native(builtin::_let_astrk);
    sexpr::global["letrec"]     = native(builtin::_let_rec);
    sexpr::global["list"]       = native(builtin::_list);
    sexpr::global["begin"]      = native(builtin::_begin);

}
