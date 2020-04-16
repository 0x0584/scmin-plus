// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:49:09 by archid-           #+#    #+#             //
//   Updated: 2020/04/16 23:51:51 by archid-          ###   ########.fr       //
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
typedef weak_ptr<struct sexpr> gc_sexpr_t;
typedef unordered_map<string, sexpr_t> bonds_t;
typedef sexpr_t (*native_lambda)(const sexpr_t&, const bonds_t&);

sexpr_t nil();
sexpr_t symb(const string& s);
sexpr_t str(const string& s);
sexpr_t num(double n);
sexpr_t cons(const sexpr_t& car, const sexpr_t& cdr);
sexpr_t lambda(const sexpr_t& args, const sexpr_t& body);
sexpr_t native(native_lambda fn);
sexpr_t eval(const sexpr_t& expr, const bonds_t& parent);
sexpr_t list(const sexpr_t& car...);

class sexpr
{
    struct sexpr_nil {};
    struct sexpr_text
    {
        string s;
        bool sy;
        explicit sexpr_text(const string& foo, bool issymb)
            : s(foo), sy(issymb) {}
        string text() {
            if (sy) return s;
            else return "\"" + s + "\"";
        }
    };
    struct sexpr_number
    {
        double n;
        explicit sexpr_number(double d) : n(d) {}
    };
    struct sexpr_conslist
    {
        sexpr_t car, cdr;

        explicit sexpr_conslist(const sexpr_t& foo, const sexpr_t& bar)
            : car(foo), cdr(bar) {}

        bool ispair() {return not islist();}
        bool islist() {
            auto walk = cdr;
            while (walk->iscons())
                walk = walk->cdr();
            return walk->isnil();
        }

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
        bonds_t local;
        native_lambda native;

        explicit sexpr_lambda(native_lambda fn) : native(fn) {}
        explicit sexpr_lambda(const sexpr_t& foo)
            : args(foo->car()), body(foo->cdr()), native(nullptr) {}

        bool bindargs(const sexpr_t& args, const bonds_t& parent) {
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

        sexpr_t eval(const sexpr_t& args, const bonds_t& parent) {
            if (native) return native(args, parent);
            if (not bindargs(args, parent))
                return nullptr;
            return ::eval(body, local);
        }

        friend ostream& operator<<(ostream& os, sexpr_lambda b) {
            os << "[args: " << b.args << ", body: " << b.body << "]" << endl;
            return os;
        }
    };

    static unordered_set<sexpr_t> pinned, keep, del;
    static void clean(bool forced = true);
    static void mark_and_sweep();

    unique_ptr<any> blob;

public:

    inline static bonds_t global;

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
    explicit sexpr(native_lambda fn)
        : blob(new any(sexpr_lambda(fn))) {}

    friend ostream& operator<<(ostream& os, sexpr_t expr) {
        if (not expr) os << "(null)";
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

    sexpr_t eval(const sexpr_t& args, const bonds_t& scope) {
        if (not islambda()) {
            cerr << "operator is not lambda" << endl;
            return nullptr;
        }
        return any_cast<sexpr_lambda>(*blob).eval(args, scope);
    }

    static void init_global_scope();
    static sexpr_t context(const sexpr_t& expr, const bonds_t& local);
    static void unbind(const sexpr_t& expr, bonds_t& local = global);
    static bool bind(const sexpr_t& rexpr, const sexpr_t& lexpr,
                     bonds_t local = global);

    static sexpr_t native_add(const sexpr_t& args, const bonds_t& bonds);
};

#endif
