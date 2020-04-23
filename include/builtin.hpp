// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   builtin.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/20 03:37:22 by archid-           #+#    #+#             //
//   Updated: 2020/04/23 08:25:56 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef BUILTIN_H
# define BUILTIN_H

# include "sexpr.hpp"

struct builtin : public sexpr
{
    static sexpr_t _add(const sexpr_t& args, env_t& bindings);
    static sexpr_t _sub(const sexpr_t& args, env_t& bindings);
    static sexpr_t _mul(const sexpr_t& args, env_t& bindings);
    static sexpr_t _div(const sexpr_t& args, env_t& bindings);

    static sexpr_t _print(const sexpr_t& args, env_t& bindings);
    static sexpr_t _set(const sexpr_t& args, env_t& bindings);
    static sexpr_t _unset(const sexpr_t& args, env_t& bindings);
    static sexpr_t _quote(const sexpr_t& args, env_t& bindings);
    static sexpr_t _unquote_sp(const sexpr_t& args, env_t& bindings);
    static sexpr_t _unquote(const sexpr_t& args, env_t& bindings);
    static sexpr_t _quasiquote(const sexpr_t& args, env_t& bindings);
    static sexpr_t _define(const sexpr_t& args, env_t& bindings);
    static sexpr_t _eval(const sexpr_t& args, env_t& bindings);

    static sexpr_t _car(const sexpr_t& args, env_t& bindings);
    static sexpr_t _cdr(const sexpr_t& args, env_t& bindings);
    static sexpr_t _setcar(const sexpr_t& args, env_t& bindings);
    static sexpr_t _setcdr(const sexpr_t& args, env_t& bindings);
    static sexpr_t _cons(const sexpr_t& args, env_t& bindings);

    static sexpr_t _ispair(const sexpr_t& args, env_t& bindings);
    static sexpr_t _islist(const sexpr_t& args, env_t& bindings);
    static sexpr_t _isnum(const sexpr_t& args, env_t& bindings);
    static sexpr_t _isnil(const sexpr_t& args, env_t& bindings);
    static sexpr_t _isstr(const sexpr_t& args, env_t& bindings);
    static sexpr_t _issymb(const sexpr_t& args, env_t& bindings);
    static sexpr_t _islambda(const sexpr_t& args, env_t& bindings);

    static sexpr_t _n_eq(const sexpr_t& args, env_t& bindings);
    static sexpr_t _eq(const sexpr_t& args, env_t& bindings);
    static sexpr_t _gt(const sexpr_t& args, env_t& bindings);
    static sexpr_t _gt_eq(const sexpr_t& args, env_t& bindings);
    static sexpr_t _lt(const sexpr_t& args, env_t& bindings);
    static sexpr_t _lt_eq(const sexpr_t& args, env_t& bindings);

    static sexpr_t _and(const sexpr_t& args, env_t& bindings);
    static sexpr_t _or(const sexpr_t& args, env_t& bindings);
    static sexpr_t _not(const sexpr_t& args, env_t& bindings);

    static sexpr_t _do(const sexpr_t& args, env_t& bindings);
    static sexpr_t _let(const sexpr_t& args, env_t& bindings);
    static sexpr_t _let_astrk(const sexpr_t& args, env_t& bindings);
    static sexpr_t _let_rec(const sexpr_t& args, env_t& bindings);
    static sexpr_t _if(const sexpr_t& args, env_t& bindings);
    static sexpr_t _cond(const sexpr_t& args, env_t& bindings);
    static sexpr_t _begin(const sexpr_t& args, env_t& bindings);
    static sexpr_t _list(const sexpr_t& args, env_t& bindings);
};
#endif
