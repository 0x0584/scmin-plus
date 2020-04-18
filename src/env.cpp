// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   env.cpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/18 22:04:59 by archid-           #+#    #+#             //
//   Updated: 2020/04/18 22:05:20 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "sexpr.hpp"


sexpr_t sexpr::context(const sexpr_t& expr, const env_t& local) {
    if (not expr) {
        cerr << "scope err: no context for null" << endl;
        return nullptr;
    }
    if (not expr->issymb()) return expr;
    string sy = any_cast<sexpr_text>(*expr->blob).text();
    auto it = local.find(sy);
    if (it != end(local))
        return it->second;
    it = global.find(sy);
    if (it != end(global))
        return it->second;
    else return expr;
}

void sexpr::unbind(const sexpr_t& expr, env_t& local) {
    if (not expr) {
        cerr << "scope err: unbinding null" << endl;
        return;
    }
    string sy = any_cast<sexpr_text>(*expr->blob).text();
    if (local.find(sy) != end(local)) local.erase(sy);
    else if (global.find(sy) != end(global)) global.erase(sy);
}

bool sexpr::bind(const sexpr_t& rexpr, const sexpr_t& lexpr,
                 env_t& local) {
    if (not rexpr or not lexpr) {
        cerr << "scope err: cannot bind null!" << endl;
        return false;
    }
    if (not rexpr->issymb()) {
        cerr << "scope err: only symbols are bind-able" << endl;
        return false;
    }
    local[any_cast<sexpr_text>(*rexpr->blob).text()] = lexpr;
    return true;
}

void sexpr::init_global() {
    sexpr::global["add"] = native(sexpr::native_add);
    sexpr::global["+"] = native(sexpr::native_add);
    sexpr::global["sub"] = native(sexpr::native_sub);
    sexpr::global["-"] = native(sexpr::native_sub);
    sexpr::global["mul"] = native(sexpr::native_mul);
    sexpr::global["*"] = native(sexpr::native_mul);

    sexpr::global["cons"] = native(sexpr::native_cons);
    sexpr::global["cdr"] = native(sexpr::native_cdr);
    sexpr::global["car"] = native(sexpr::native_car);
    sexpr::global["quote"] = native(sexpr::native_quote);
    sexpr::global["print"] = native(sexpr::native_print);
}
