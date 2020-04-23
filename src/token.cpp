// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   token.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/13 03:01:20 by archid-           #+#    #+#             //
//   Updated: 2020/04/23 07:08:55 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "token.hpp"

bool token::next_paren(string& e) {
    char c = e[0];
    if (c != '(' and c != ')')
        return false;
    e.erase(0, 1);
    s = c;
    type = c == '(' ? tok_l_paren : tok_r_paren;
    if (c == ')') {
        if (paren.size() == 0 or paren.top() != '(')
            return err = true;
        paren.pop();
        return true;
    } else if (c == '(') {
        paren.push(c);
        return true;
    }
    return true;
}

bool token::next_string(string& e) {
    int j = 1;
    if (e[0] != '\"')
        return false;
    type = tok_str;
    for (; e[j] != '\"'; j++)
        if (e[j]) s += e[j];
        else return err = true;
    e.erase(0, j + 1);
    return true;

}

bool token::next_special(string& e) {
    char c = e[0];

    if (c == '\'') {
        s = c, type = tok_quote;
        e.erase(0, 1);
        return true;
    } else if (c == '.') {
        s = c, type = tok_pair;
        e.erase(0, 1);
        return true;
    } else if (c == '`') {
        s = c, type = tok_quasi;
        e.erase(0, 1);
        return true;
    } else if (c == ',') {
        s = c, type = (e[1] == '@' ? tok_unquote_sp : tok_unquote);
        e.erase(0, 1 + (e[1] == '@'));
        return true;
    }
    return false;
}

bool token::next_text(string& e) {
    string delim = "() \"";
    int j = 0;

    for (; e[j] and delim.find(e[j]) == string::npos; j++)
        s += e[j];
    type = (s == "lambda" ? tok_lambda : tok_num_or_symb);
    e.erase(0, j);
    return true;
}

bool token::next_token(string& e, token& tok) {
    for (int i = 0; e[i] and isspace(e[i]); i++)
        e.erase(i--, 1);
    if (e.empty() or e[0] == ';')
        return false;
    return tok.next_paren(e) or tok.next_string(e)
        or tok.next_special(e) or tok.next_text(e);
}

queue<token> token::tokenize(string& s) {
    queue<token> tokens;
    token tok;

    paren = {};
    while (next_token(s, tok) and not tok.err)
        tokens.push(tok), tok = {};
    // TODO: implement more sophisticated error management ////////////////////
    if (tok.err or paren.size()) {
        cerr << "there was error while lexing" << endl;
        return {};
    }
    return tokens;
}

// string sp = "!$%&*/:<=>?^_~";
// if (not (::isalpha(e[0]) or sp.find(e[0]) == string::npos)) {
//     cerr << "Invalid indetifier initial" << endl;
//     tok.err = true;
//     return true;
// }
