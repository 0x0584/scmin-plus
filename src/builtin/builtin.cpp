// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   builtin.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/16 22:19:53 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 21:10:43 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "builtin.hpp"

// FIXME: a lot of stuff should be in stdlib instead of builtins ///////////////
//
// this should be possible using syntax-rules, like let and let* and list...
////////////////////////////////////////////////////////////////////////////////

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
    return def->car();
}

sexpr_t builtin::_quote(const sexpr_t& args, env_t& bindings) {
    (void)bindings;
    if (args->length() != 1 ) {
        cerr << "quote expects only one argument" << endl;
        return nullptr;
    }
    return args->car();
}

sexpr_t builtin::_unquote(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1 ) {
        cerr << "unquote expects only one argument" << endl;
        return nullptr;
    }
    cout << "aegrs: " << args << endl;
    return ::eval(args->car(), bindings);
}

sexpr_t builtin::_quasiquote(const sexpr_t& args, env_t& bindings) {
    if (args->length() != 1) {
        cerr << "quasiquote expects only one argument" << endl;
        return nullptr;
    }
    sexpr_t e, tail, walk;

    if (not args->car()->islist())
        return args->car();
    walk = args->car();
    while (not walk->isnil()) {
        sexpr_t tmp = cons(walk->car(), nil());

        if (walk->car()->islist() and walk->car()->car()->issymb()) {
            auto sy = any_cast<sexpr_text>(*walk->car()->car()->blob).text();
            if (sy == "unquote" or sy == "unquote-splicing") {
                tmp = builtin::_unquote(walk->car()->cdr(), bindings);
                if (sy == "unquote")
                    tmp = cons(tmp, nil());
            }
        }
        if (not e)
            tail = e = tmp;
        else {
            tail->setcdr(tmp);
            tail = tail->cdr();
        }
        walk = walk->cdr();
    }
    return e;
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
    return itr->second;
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
        if (not res) return nullptr;
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

    if (not (walk->islist() or walk->isnil())) {
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
    (void)args;
    (void)bindings;
    return nil();
}

// (do ((vec (make-vector 5)) (i 0 (+ i 1))) ((= i 5) vec) (vector-set! vec i i))
