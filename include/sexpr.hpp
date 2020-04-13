// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:49:09 by archid-           #+#    #+#             //
//   Updated: 2020/04/13 18:16:35 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef SEXPR_H
# define SEXPR_H

# include "common.hpp"

typedef shared_ptr<struct sexpr> sexpr_t;
typedef weak_ptr<struct sexpr> gc_sexpr_t;

sexpr_t nil();
sexpr_t symb(const string& s);
sexpr_t str(const string& s);
sexpr_t num(double n);
sexpr_t cons(const sexpr_t& car, const sexpr_t& cdr);
sexpr_t list(const sexpr_t& car...);

class sexpr
{
    struct sexpr_nil {};
    struct sexpr_text {
        string s; bool sy;
        sexpr_text(const string& foo, bool issymb)
            : s(foo), sy(issymb) {}
        string text() {
            if (sy) return s;
            else return "\"" + s + "\"";
        }
    };
    struct sexpr_number {
        double n;
        explicit sexpr_number(double d) : n(d) {}
    };
    struct sexpr_conslist {
        sexpr_t car, cdr;

        sexpr_conslist(const sexpr_t& foo, const sexpr_t& bar)
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
                os << c->car << " . " << c->cdr;
                return os;
            }
            os << c->car;
            for (auto e = c->cdr; not e->isnil(); e = e->cdr())
                os << " " << e->car();
            return os;
        }
    };

    static unordered_set<sexpr_t> pinned, keep, del;
    static void clean(bool forced = true);
    static void mark_and_sweep();

    unique_ptr<any> blob;

public:

    sexpr() : blob(new any(sexpr_nil())) {}

    ~sexpr() {
        if (iscons()) delete any_cast<sexpr_conslist *>(*blob);
    }
    explicit sexpr(double n) : blob(new any(sexpr_number(n))) {}
    explicit sexpr(const string& s, bool symbol = true)
        : blob(new any(sexpr_text(s, symbol))) {}
    explicit sexpr(const sexpr_t& car, const sexpr_t& cdr)
        : blob(new any(new sexpr_conslist(car, cdr))) {}

    friend ostream& operator<<(ostream& os, sexpr_t s) {
        if (s == nullptr) os << "(null)";
        else if (s->isnil()) os << "nil";
        else if (s->isnum()) os << any_cast<sexpr_number>(*s->blob).n;
        else if (s->issymb() or s->isstr())
            os << any_cast<sexpr_text>(*s->blob).text();
        else os << "(" << any_cast<sexpr_conslist *>(*s->blob) << ")";
        return os;
    }

    bool isnil(), iscons(), islist(), ispair();
    bool isnum(), isstr(), issymb();
    bool setcar(const sexpr_t& e), setcdr(const sexpr_t& e);

    sexpr_t car(), cdr();

    sexpr_t eval() const;
};

#endif
