// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   repl.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/12 02:28:48 by archid-           #+#    #+#             //
//   Updated: 2020/04/12 03:34:54 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "repl.hpp"

struct token {
    string s;
    enum tok_type {
        tok_l_paren = 0, tok_r_paren,
        tok_str, tok_num_or_symb
    } type;
};

bool next_token(string& e, token& tok, bool& err) {
    for (auto i = 0u; i < e.length(); i++) {
        if (isspace(e[i])) continue;
        else if (e[i] == '(' or e[i] == ')') {
            tok.s = e[i];
            tok.type = e[i] == '(' ? token::tok_l_paren
                : token::tok_r_paren;
            e.erase(i, 1);
            return true;
        }
        else if (e[i] == '\"') {
            int j = i + 1;
            for (; e[j] != '\"'; j++)
                if (e[j]) tok.s += e[j];
                else {err = true; return false;}
            tok.type = token::tok_str;
            e.erase(i,j + 1);
            return true;
        } else {
            auto j = i;
            for (; e[j] and e[j] != ' '; j++)
                tok.s += e[j];
            tok.type = token::tok_num_or_symb;
            e.erase(i, j + 1);
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
    while (next_token(t, tok, err))
        tokens.push_back(tok), tok = {};

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
