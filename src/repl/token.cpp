// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   token.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/13 03:01:20 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 18:11:45 by archid-          ###   ########.fr       //
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

static bool has_suffix(const string& s, const string& t) {
    return equal(rbegin(s), rbegin(s) + t.length(),
                 rbegin(t), rend(t));
}

static bool is_cons_flavor(const string& s, token *tok) {
    const string t = "r!";
    size_t front, rear;

    if ((s.rfind("set-c", 0) == 0 and has_suffix(s, t)))
        front = 5, rear = s.length() - 2, tok->isset = true;
    else if (s.front() == 'c' and s.back() == 'r')
        front = 1, rear = s.length() - 1, tok->isset = false;
    else
        return false;
    for (auto i = front; i < rear; i++)
        if (s[i] == 'a' or s[i] == 'd') tok->cons_s.push(s[i]);
        else return false;
    tok->type = token::tok_cons_flavor;
    return true;
}

bool token::next_symbol(string& e) {
    const string delim = "() \"";
    int j = 0;

    for (; e[j] and delim.find(e[j]) == string::npos; j++)
        s += e[j];
    if (not is_cons_flavor(s, this))
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
        or tok.next_special(e) or tok.next_symbol(e);
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
