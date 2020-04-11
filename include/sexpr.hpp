// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   sexpr.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/09 23:49:09 by archid-           #+#    #+#             //
//   Updated: 2020/04/11 18:55:01 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef SEXPR_H
# define SEXPR_H

# include "common.hpp"

typedef shared_ptr<struct sexpr> sexpr_t;

struct sexpr
{
    struct sexpr_nil {};
    struct sexpr_text {
        string s; bool sy;
        sexpr_text(string foo, bool issymb) : s(foo), sy(issymb) {}
    };
    struct sexpr_number {
        double n;
        explicit sexpr_number(double d) : n(d) {}
    };
    struct sexpr_conslist {
        sexpr_t car, cdr;

        sexpr_conslist(sexpr_t foo, sexpr_t bar) : car(foo), cdr(bar) {}

        bool islist() {
            // (a (b . c))
            // cout << "islist" << endl;
            auto walk = cdr;
            while (walk->blob->type() == typeid(sexpr_conslist)) {
                walk = any_cast<sexpr_conslist>(*walk->blob).cdr;
            }
            // cout << ">> islist" << endl;
            return cdr->blob->type() == typeid(sexpr_nil);
        }
        bool ispair() { return !islist(); }

        friend ostream& operator<<(ostream& os, sexpr_conslist c) {
            // os << "here" << endl;
            if (c.ispair()) os << c.car << " . " << c.cdr;
            else {
                auto walk = c;
                while (walk.cdr->blob->type() != typeid(sexpr_nil)) {
                    os << walk.car;
                    // os << " -- // - " << endl;
                    walk = any_cast<sexpr_conslist>(*walk.cdr->blob);
                }
            }
            return os;
        }
    };

    unique_ptr<any> blob;

    sexpr() : blob(new any(sexpr_nil())) {}

    explicit sexpr(double n) : blob(new any(sexpr_number(n))) {}
    explicit sexpr(string s, bool symbol = true)
        : blob(new any(sexpr_text(s, symbol))) {}
    explicit sexpr(sexpr_t car, sexpr_t cdr)
        : blob(new any(sexpr_conslist(car,cdr))) {}

    friend ostream& operator<<(ostream& os, sexpr_t s) {
        if (s->blob->type() == typeid(sexpr_nil))
            os << "nil";
        else if (s->blob->type() == typeid(sexpr_number))
            os << any_cast<sexpr_number>(*s->blob).n;
        else if (s->blob->type() == typeid(sexpr_text)) {
            if (any_cast<sexpr_text>(*s->blob).sy)
                os << any_cast<sexpr_text>(*s->blob).s;
            else
                os << "\"" + any_cast<sexpr_text>(*s->blob).s + "\"";
        } else
            os << "(" << any_cast<sexpr_conslist>(*s->blob) << ")";
        return os;
    }

    friend ostream& operator<<(ostream& os, sexpr s) {
        if (s.blob->type() == typeid(sexpr_nil))
            os << "nil";
        else if (s.blob->type() == typeid(sexpr_number))
            os << any_cast<sexpr_number>(*s.blob).n;
        else if (s.blob->type() == typeid(sexpr_text))
            os << any_cast<sexpr_text>(*s.blob).s;
        else
            os << "(" << any_cast<sexpr_conslist>(*s.blob) << ")";
        return os;
    }

    sexpr_t eval() const {
        return nullptr;
    }

    static sexpr_t nil() {
        cout << "making nil" << endl;
        return make_shared<sexpr>();
    }
    static sexpr_t symb(string s) {
        cout << "making symb " << s << endl;
        return make_shared<sexpr>(s);
    }
    static sexpr_t str(string s) {
        cout << "making str " << s << endl;
        return make_shared<sexpr>(s, false);
    }
    static sexpr_t num(double n) {
        cout << "making num " << n << endl;
        return make_shared<sexpr>(n);
    }
    static sexpr_t cons(sexpr_t car, sexpr_t cdr) {
        cout << "making cons " << car << " " << cdr << endl;
        auto p = make_shared<sexpr>(car, cdr);
        // cout << " -- PP -" << endl;
        return p;
    }

private:

    static unordered_set<sexpr_t> pinned, keep, sweep;
    static void clean(bool forced = true);
    static void mark_and_sweep();
};

#endif
