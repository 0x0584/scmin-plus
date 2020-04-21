// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   builtin.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/21 14:56:28 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"

// FIXME: unify the return values on failure ///////////////////////////////////
//        this could actually be done by detecting them before actually
//        evaluating them, things like like args count and typing issues
//        could be raised earlier on in the parsing phase
////////////////////////////////////////////////////////////////////////////////

// FIXME: remove some code redundancy //////////////////////////////////////////

sexpr_t builtin::_add(const sexpr_t& args, env_t& bindings) {
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

sexpr_t builtin::_sub(const sexpr_t& args, env_t& bindings) {
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

sexpr_t builtin::_mul(const sexpr_t& args, env_t& bindings) {
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

sexpr_t builtin::_div(const sexpr_t& args, env_t& bindings) {
    (void)bindings;

    if (not args->car()->isnum()) {
        cerr << "fatal: operands are not numbers!" << endl;
        return nullptr;
    }
    double d = any_cast<sexpr_number>(*args->car()->blob).n;
    sexpr_t walk = args->cdr();
    while (not walk->isnil()) {
        if (not walk->car()->isnum()) {
            cerr << "fatal: operands are not numbers!" << endl;
            return nullptr;
        }
        d /= any_cast<sexpr_number>(*walk->car()->blob).n;
        walk = walk->cdr();
    }
    return num(d);
}

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

sexpr_t builtin::_setcar(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (not args->car()->issymb() or args->length() != 2) {
        cerr << "Err: cannot set constant!" << endl;
        return nullptr;
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbole is not defined" << endl;
        return nullptr;
    } else if (not itr->second->iscons()) {
        cerr << "Err: Symbol is not bound to a conslist" << endl;
        return nullptr;
    }
    itr->second->setcar(::eval(args->cdr()->car(), bindings));
    return symb("t");
}

sexpr_t builtin::_setcdr(const sexpr_t& args, env_t& bindings) {
    if (not args->car()->issymb() or args->length() != 2) {
        cerr << "Err: cannot set constant!" << endl;
        return nullptr;
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbole is not defined" << endl;
        return nullptr;
    } else if (not itr->second->iscons()) {
        cerr << "Err: Symbol is not bound to a conslist" << endl;
        return nullptr;
    }
    itr->second->setcdr(::eval(args->cdr()->car(), bindings));
    return symb("t");
}

sexpr_t builtin::_quote(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1 ) {
        cerr << "quote expects only one argument" << endl;
        return nullptr;
    }
    return args->car();
}

sexpr_t builtin::_print(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t walk = args;

    while (not walk->isnil()) {
        cout << walk->car() << (walk->cdr()->isnil() ? "\n" : " ");
        walk = walk->cdr();
    }
    return nil();
}

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

#define cmp_helper(u, v, cmp) (any_cast<sexpr_number>(*u->blob).n \
                               cmp any_cast<sexpr_number>(*v->blob).n)

bool valid_numeric_args(sexpr_t args) {
    if (args->length() != 2) {
        cerr << "Err: expected two arguments!" << endl;
        return false;
    }
    if (not (args->car()->isnum() and args->cdr()->car()->isnum())) {
        cerr << "Err: arguments should both be numbers!" << endl;
        return false;
    }
    return true;
}

sexpr_t builtin::_n_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, !=) ? symb("t") : nil();
}

sexpr_t builtin::_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, ==) ? symb("t") : nil();
}

sexpr_t builtin::_gt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >) ? symb("t") : nil();
}

sexpr_t builtin::_gt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, >=) ? symb("t") : nil();
}

sexpr_t builtin::_lt(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <) ? symb("t") : nil();
}

sexpr_t builtin::_lt_eq(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t u, v;
    if (not valid_numeric_args(args))
        return nullptr;
    u = args->car(), v = args->cdr()->car();
    return cmp_helper(u, v, <=) ? symb("t") : nil();
}

sexpr_t builtin::_and(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t walk = args;

    while (not walk->isnil()) {
        if (walk->car()->isnil())
            return nil();
        walk = walk->cdr();
    }
    return symb("t");
}

sexpr_t builtin::_or(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    sexpr_t walk = args;

    while (not walk->isnil()) {
        if (not walk->car()->isnil())
            return symb("t");
        walk = walk->cdr();
    }
    return nil();
}

sexpr_t builtin::_not(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1) {
        cerr << "Err: not takes only one argument" << endl;
        return nullptr;
    }
    return args->car()->isnil() ? symb("t") : nil();
}

sexpr_t builtin::_define(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 2) {
        cerr << "Err: define expects two arguments" << endl;
        return nullptr;
    }
    bool is_lambda = args->car()->islist();
    sexpr_t def = is_lambda ? args->car() : args;
    if (not def->car()->issymb()) {
        cerr << "Err: define is mal-formatted" << endl;
        return nullptr;
    }
    auto sy = any_cast<sexpr_text>(*def->car()->blob).text();
    if (bindings.find(sy) != end(bindings)) {
        cerr << "Err: symbol is already defined, use set! instead" << endl;
        return nil();
    }
    bindings[sy] = ::eval(is_lambda ? lambda(args->car()->cdr(),
                                             args->cdr()->car()) :
                          args->cdr()->car(), bindings);
    return symb("t");
}

sexpr_t builtin::_set(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 2) {
        cerr << "Err: expecting two arguments!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot set constant values!" << endl;
        return nil();
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbol is not defined, use define instead" << endl;
        return nil();
    }
    itr->second = ::eval(args->cdr()->car(), bindings);
    return symb("t");
}

sexpr_t builtin::_unset(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "Err: expecting one argument!" << endl;
        return nil();
    }
    if (not args->car()->issymb()) {
        cerr << "Err: cannot unset constant values!" << endl;
        return nil();
    }
    auto itr = bindings.find(any_cast<sexpr_text>(*args->car()->blob).text());
    if (itr == end(bindings)) {
        cerr << "Err: symbol is not defined" << endl;
        return nil();
    }
    bindings.erase(itr);
    return symb("t");
}

sexpr_t builtin::_eval(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "Err: eval expects one expression" << endl;
        return nullptr;
    }
    return ::eval(args->car(), bindings);
}

// FIXME: list should be part of stdlib instead of builtin ////////////////////
sexpr_t builtin::_list(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    return args;
}

sexpr_t builtin::_if(const sexpr_t& args, env_t& bindings) {
    if (args->length() < 2 or args->length() > 3) {
        cerr << "Err: if requires a condition and at most two expression" << endl;
        return nullptr;
    }
    sexpr_t s = ::eval(args->car(), bindings);
    if (not s) return nullptr;
    else if (not s->isnil())
        return ::eval(args->cdr()->car(), bindings);
    else if (not args->cdr()->cdr()->isnil())
        return ::eval(args->cdr()->cdr()->car(), bindings);
    return nil();
}

sexpr_t builtin::_begin(const sexpr_t& args, env_t& bindings) {
    sexpr_t walk = args;
    sexpr_t res = nil();

    while (not walk->isnil()) {
        res = ::eval(walk->car(), bindings);
        walk = walk->cdr();
    }
    return res;
}

sexpr_t builtin::_cond(const sexpr_t& args, env_t& bindings) {
    sexpr_t walk = args;
    sexpr_t curr;

    while (not walk->isnil()) {
        if (not walk->car()->islist()) {
            cerr << "Err: cond clauses need to be a list" << endl;
            return nullptr;
        }
        curr = walk->car();
        if ((curr->car()->issymb() and
             any_cast<sexpr_text>(*curr->car()->blob).text() == "else")
            or not ::eval(curr->car(), bindings)->isnil()) {
            return builtin::_begin(curr->cdr(), bindings);
        }
        walk = walk->cdr();
    }
    return nil();
}

sexpr_t builtin::_let(const sexpr_t& args, env_t& bindings) {
    if (args->length() < 2) {
        cerr << "Err: let requires an env and sexpr(s)" << endl;
        return nullptr;
    }

    unordered_set<string> seen;
    bool labeled = args->car()->issymb();
    env_t local = bindings;
    sexpr_t walk = labeled ? args->cdr()->car(): args->car(),
        curr;
    sexpr_t let_args, let_args_tail;

    if (not walk->islist()) {
        cerr << "Err: let is mal formatted" << endl;
        return nullptr;
    }
    while (not walk->isnil()) {
        curr = walk->car();
        if (not curr->islist() or curr->length() != 2) {
            cerr << "Err: let bound is mal formatted" << endl;
            return nullptr;
        }
        if (not curr->car()->issymb()) {
            cerr << "Err: cannot bind constant" << endl;
            return nullptr;
        }
        auto sy = any_cast<sexpr_text>(*curr->car()->blob).text();
        if (not seen.insert(sy).second) {
            cerr << "Err: variable appeared twice!" << endl;
            return nil();
        }
        if (labeled) {
            auto tmp = cons(curr->car(), nil());
            if (not let_args) {
                let_args_tail = let_args = tmp;
            } else {
                let_args_tail->setcdr(tmp);
                let_args_tail = let_args_tail->cdr();
            }
        }
        local[sy] = ::eval(curr->cdr()->car(), bindings);
        walk = walk->cdr();
    }
    if (labeled) {
        auto let_lambda = lambda(let_args, cons(symb("begin"),
                                                args->cdr()->cdr()));
        local[any_cast<sexpr_text>(*args->car()->blob).text()] = let_lambda;
    }
    return builtin::_begin(labeled ? args->cdr()->cdr() : args->cdr(), local) ;
}

sexpr_t builtin::_let_astrk(const sexpr_t& args, env_t& bindings) {
    if (args->length() < 2) {
        cerr << "Err: let requires an env and sexpr(s)" << endl;
        return nullptr;
    }

    env_t local = bindings;
    sexpr_t walk = args->car(), curr;
    unordered_set<string> seen;

    while (not walk->isnil()) {
        curr = walk->car();
        if (not curr->islist() or curr->length() != 2) {
            cerr << "Err: let bound is mal formatted" << endl;
            return nullptr;
        }
        if (not curr->car()->issymb()) {
            cerr << "Err: cannot bind constant" << endl;
            return nullptr;
        }
        auto sy = any_cast<sexpr_text>(*curr->car()->blob).text();
        if (not seen.insert(sy).second) {
            cerr << "Err: variable appeared twice!" << endl;
            return nil();
        }
        local[sy] = ::eval(curr->cdr()->car(), local);
        walk = walk->cdr();
    }
    return builtin::_begin(args->cdr(), local);
}

sexpr_t builtin::_let_rec(const sexpr_t& args, env_t& bindings) {
    if (args->length() < 2) {
        cerr << "Err: let requires an env and sexpr(s)" << endl;
        return nullptr;
    }

    env_t local = bindings;
    sexpr_t walk = args->car(), curr;
    unordered_set<string> seen;

    while (not walk->isnil()) {
        curr = walk->car();
        if (not curr or not curr->islist() or curr->length() != 2) {
            cerr << "Err: let bound is mal formatted" << endl;
            return nullptr;
        }
        if (not curr->car()->issymb()) {
            cerr << "Err: cannot bind constant" << endl;
            return nullptr;
        }
        auto sy = any_cast<sexpr_text>(*curr->car()->blob).text();
        if (not seen.insert(sy).second) {
            cerr << "Err: variable appeared twice!" << endl;
            return nil();
        }
        local[sy] = nil();
        walk = walk->cdr();
    }
    walk = args->car();
    while (not walk->isnil()) {
        curr = walk->car();
        auto sy = any_cast<sexpr_text>(*curr->car()->blob).text();
        local[sy] = ::eval(curr->cdr()->car(), local);
        walk = walk->cdr();
    }
    return builtin::_begin(args->cdr(), local);
}

sexpr_t builtin::_do(const sexpr_t& args, env_t& bindings) {

    return nil();
}

// (do ((vec (make-vector 5)) (i 0 (+ i 1))) ((= i 5) vec) (vector-set! vec i i))
