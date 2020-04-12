// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   repl.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/12 02:28:48 by archid-           #+#    #+#             //
//   Updated: 2020/04/13 00:16:43 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "repl.hpp"

struct token {
    inline static stack<char> paren;
    string s;
    enum tok_type {
        tok_l_paren = 0, tok_r_paren,
        tok_str, tok_num_or_symb
    } type;
    static void reset() {paren = {};}
    static bool delim(char c) {
        return c == ' ' or c == '('
            or c == ')' or  c == '\"';
    }
    static bool balanced() {
        return paren.size() == 0;
    }
    static bool balanced(char c) {
        if (c == ')' and paren.top() == '(') {
            paren.pop();
            return true;
        } else if (c == '(') {
            paren.push(c);
            return true;
        }
        return false;
    }
    friend ostream& operator<<(ostream& os, enum tok_type e) {
        if (e == tok_l_paren) os << "tok_l_paren";
        else if (e == tok_r_paren) os << "tok_r_paren";
        else if (e == tok_str) os << "tok_str";
        else os << "tok_num_or_symb";
        return os;
    }
};

bool next_token(string& e, token& tok, bool& err) {
    for (auto i = 0u; i < e.length(); i++) {
        if (isspace(e[i])) {
            e.erase(i--, 1);
            continue;
        }
        else if (e[i] == '(' or e[i] == ')') {
            tok.s = e[i];
            tok.type = e[i] == '(' ? token::tok_l_paren
                : token::tok_r_paren;
            if (not token::balanced(e[i])) {
                err = true;
                return false;
            }
            e.erase(i, 1);
            return true;
        } else if (e[i] == '\"') {
            auto j = i + 1;
            for (; e[j] != '\"'; j++)
                if (e[j]) tok.s += e[j];
                else {
                    err = true;
                    return false;
                }
            tok.type = token::tok_str;
            e.erase(i,j + 1);
            return true;
        } else {
            auto j = i;
            for (; e[j] and not token::delim(e[j]); j++)
                tok.s += e[j];
            tok.type = token::tok_num_or_symb;
            e.erase(i, j);
            return true;
        }
    }
    return false;
}

sexpr_t parse(string s) {
    vector<token> tokens;
    token tok;
    bool err = false;
    string t = s;
    sexpr_t e;

    // read tokens
    token::reset();
    while (next_token(t, tok, err))
        tokens.push_back(tok), tok = {};

    // TODO: implement more sophisticated error management
    if (err or not token::balanced()) {
        cout << "there was error" << endl;
        return nullptr;
    }

    cout << "list tokens" << endl;
    for (auto u : tokens) {
        cout << u.type << " {" << u.s << "}" <<  endl;
    }
    cout << " ------ " << endl;

    // parse tokens into sexpr

    e = nil();
    return e;
}

void repl() {
    string s;
    while (true) {
        if (s.length() == 0) break;
        parse(s)->eval();
    }
}
