// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:49:09 by archid-           #+#    #+#             //
//   Updated: 2020/04/21 14:17:55 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef SEXPR_H
# define SEXPR_H

# include <memory>
# include <cstdarg>
# include <any>
# include <iostream>
# include <string>
# include <vector>
# include <queue>
# include <stack>
# include <algorithm>
# include <unordered_map>
# include <unordered_set>

using namespace std;

typedef shared_ptr<struct sexpr> sexpr_t;
typedef unordered_map<string, sexpr_t> env_t;
typedef sexpr_t (*builtin_lambda)(const sexpr_t&, env_t&);

sexpr_t nil();
sexpr_t symb(const string& s);
sexpr_t str(const string& s);
sexpr_t num(double n);
sexpr_t cons(const sexpr_t& car, const sexpr_t& cdr);
sexpr_t lambda(const sexpr_t& args, const sexpr_t& body);
sexpr_t native(builtin_lambda fn);
sexpr_t eval(const sexpr_t& expr, env_t& parent);
sexpr_t eval_args(const sexpr_t& args, env_t& parent);
// sexpr_t list(const sexpr_t& car...);

// FIXME: use polymorphism instead of those structs! //////////////////////////
struct sexpr
{
    struct sexpr_nil {};
    struct sexpr_number
    {
        double n;
        explicit sexpr_number(double d) : n(d) {}
    };
    struct sexpr_text
    {
        string s;
        bool sy;
        explicit sexpr_text(const string& foo, bool issymb)
            : s(foo), sy(issymb) {}
        string text();
    };
    struct sexpr_conslist
    {
        sexpr_t car, cdr;

        explicit sexpr_conslist(const sexpr_t& foo, const sexpr_t& bar)
            : car(foo), cdr(bar) {}

        bool ispair(), islist();

        friend ostream& operator<<(ostream& os, sexpr_conslist *c) {
            if (c->ispair()) {
                os << "(" << c->car << " . " << c->cdr << ")";
                return os;
            }
            os << "(" << c->car;
            for (auto e = c->cdr; not e->isnil(); e = e->cdr())
                os << " " << e->car();
            os << ")";
            return os;
        }
    };
    struct sexpr_lambda
    {
        sexpr_t args, body;
        env_t local;
        builtin_lambda native;

        explicit sexpr_lambda(builtin_lambda fn) : native(fn) {}
        explicit sexpr_lambda(const sexpr_t& foo)
            : args(foo->car()), body(foo->cdr()), native(nullptr) {}

        bool bindargs(const sexpr_t& args, env_t& parent);
        bool require_evaled_args(); // not the best name ever
        sexpr_t eval(const sexpr_t& args, env_t& parent);


        friend ostream& operator<<(ostream& os, sexpr_lambda b) {
            if (b.native)
                os << "[built-in]";
            else
                os << "[args: " << b.args << ", body: " << b.body << "]";
            return os;
        }
    };

    unique_ptr<any> blob;

    inline static env_t global;

    sexpr() : blob(new any(sexpr_nil())) {}
    ~sexpr() {
        if (iscons())
            delete any_cast<sexpr_conslist *>(*blob);
    }

    explicit sexpr(double n) : blob(new any(sexpr_number(n))) {}
    explicit sexpr(const string& s, bool symbol = true)
        : blob(new any(sexpr_text(s, symbol))) {}
    explicit sexpr(const sexpr_t& car, const sexpr_t& cdr)
        : blob(new any(new sexpr_conslist(car, cdr))) {}
    explicit sexpr(const sexpr_t& lambda)
        : blob(new any(sexpr_lambda(lambda))) {}
    explicit sexpr(builtin_lambda fn)
        : blob(new any(sexpr_lambda(fn))) {}

    friend ostream& operator<<(ostream& os, sexpr_t expr) {
        if (not expr) os << "(null)"; // this is for debug,
                                      // should be empty string
        else if (expr->isnil()) os << "nil";
        else if (expr->isnum()) os << any_cast<sexpr_number>(*expr->blob).n;
        else if (expr->iscons()) os << any_cast<sexpr_conslist *>(*expr->blob);
        else if (expr->islambda()) os << any_cast<sexpr_lambda>(*expr->blob);
        else os << any_cast<sexpr_text>(*expr->blob).text();
        return os;
    }

    size_t length();

    bool isnil(), isatom(), isnum(), isstr(), issymb(),
        iscons(), islist(), ispair(), islambda();

    sexpr_t car(), cdr();
    bool setcar(const sexpr_t& e), setcdr(const sexpr_t& e);

    sexpr_t eval(const sexpr_t& args, env_t& bindings);

    static void init_global();
    static sexpr_t resolve(const sexpr_t& expr, env_t& local);
};

#endif
