// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   token.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/13 03:00:33 by archid-           #+#    #+#             //
//   Updated: 2020/04/23 08:03:15 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TOKEN_H
# define TOKEN_H

# include "sexpr.hpp"

struct token
{
    inline static stack<char> paren = {};

    string s;
    bool err = false;

    enum tok_type {
        tok_l_paren, tok_r_paren, tok_str,
        tok_num_or_symb, tok_quote, tok_quasi,
        tok_unquote, tok_unquote_sp, tok_pair,
        tok_lambda
    } type;

    bool next_paren(string& e), next_string(string& e),
        next_special(string& e), next_text(string& e);

    static bool next_token(string& e, token& tok);
    static queue<token> tokenize(string& s);

    operator sexpr_t() const {
        if (type == tok_num_or_symb) {
            bool redix = false;
            bool flag = true;

            if (s == "+" || s == "-") return symb(s);
            for (auto i = 0u; i < s.length(); i++) {
                if (s[i] == '+' or s[i] == '-') {
                    if (i == 0) continue;
                    else {flag = false; break;}
                } else if (not isdigit(s[i])) {
                    flag = false;
                    break;
                } else if (s[i] == '.') {
                    if (not redix) redix = true;
                    else {flag = false; break;}
                }
            }

            if (flag) return num(stod(s));
            else return s == "nil" ? nil() : symb(s);
        } else if (type == tok_str) {
            return str(s);
        }
        return nil();
    }

    friend ostream& operator<<(ostream& os, enum tok_type e) {
        if (e == tok_l_paren) os << "tok_l_paren";
        else if (e == tok_r_paren) os << "tok_r_paren";
        else if (e == tok_str) os << "tok_str";
        else if (e == tok_quote) os << "tok_quote";
        else if (e == tok_quasi) os << "tok_quasiquote";
        else if (e == tok_unquote) os << "tok_unquote";
        else if (e == tok_unquote_sp) os << "tok_unquote_sp";
        else if (e == tok_pair) os << "tok_pair";
        else if (e == tok_lambda) os << "tok_lambda";
        else os << "tok_num_or_symb";
        return os;
    }

    friend ostream& operator<<(ostream& os, token tok) {
        os << tok.type << " "  << tok.s;
        return os;
    }
};

#endif
