// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   repl.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/12 02:28:48 by archid-           #+#    #+#             //
//   Updated: 2020/04/15 00:50:35 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "repl.hpp"

sexpr_t parse_quote(queue<token>& q) {
    q.pop();
    if (q.size() == 0 || q.front().type == token::tok_r_paren) {
        cerr << "parsing error: empty quote";
        return nullptr;
    }
    sexpr_t e = parse_tokens(q);
    if (e == nullptr)
        return nullptr;
    else if (e->isnil())
        return e;
    return cons(symb("quote"), e);
}

sexpr_t parse_tokens(queue<token>& q) {
    sexpr_t e = nullptr;
    token tok;

    if (q.front().type == token::tok_l_paren) {
        q.pop();
        if (q.front().type == token::tok_r_paren) {
            q.pop();
            return nil();
        }
        sexpr_t tail;

        while (q.front().type != token::tok_r_paren) {
            sexpr_t tmp = nullptr;

            if (q.front().type == token::tok_l_paren)
                tmp = parse_tokens(q);
            else if (q.front().type == token::tok_quote)
                tmp = parse_quote(q);
            else if (q.front().type == token::tok_pair) {
                q.pop();
                if (e == nullptr or q.front().type == token::tok_r_paren) {
                    cerr << "parsing error! pair is not valid" << endl;
                    return nullptr;
                }
                tmp = q.front(); q.pop();
                if (not tmp->isatom() or q.front().type != token::tok_r_paren) {
                    cerr << "parsing error! pair is not valid" << endl;
                    return nullptr;
                }
                e = cons(e->car(), tmp);
                break;
            } else if (q.front().type == token::tok_lambda) {
                q.pop();
                if (e != nullptr || q.front().type != token::tok_l_paren) {
                    cerr << "lambda args is mal-formatted" << endl;
                    return nullptr;
                }
                auto args = parse_tokens(q);
                // cout << "args: " << args << endl;
                if (q.front().type != token::tok_l_paren) {
                    cerr << "lambda body is mal-formatted" << endl;
                    return nullptr;
                }
                auto body = parse_tokens(q);
                // cout << "body: " << body << endl;
                if (q.front().type != token::tok_r_paren) {
                    cerr << "lambda is mal-formatted" << endl;
                    return nullptr;
                }
                // cout << " ,,, " << args << " ,,, "<< body << endl;
                e = lambda(args, body);
                break;
            } else {
                tmp = static_cast<sexpr_t>(q.front());
                // cout << "here!!!" << endl;
                // cout << tmp << endl;
                q.pop();
            }

            if (tmp == nullptr)
                return nullptr;

            if (e == nullptr)
                tail = e = cons(tmp, nil());
            else {
                tail->setcdr(cons(tmp, nil()));
                tail = tail->cdr();
            }
            // cout << ".... " << e << endl;
        }
        q.pop();
    } else if (q.front().type == token::tok_quote) {
        e = parse_quote(q);
    } else if (q.front().type == token::tok_pair) {
        cerr << "parsing error! pair is not valid" << endl;
        return nullptr;
    } else {
        e = q.front(), q.pop();
    }
    return e;
}

sexpr_t parse(string s) {
    queue<token> tokens;
    if ((tokens = token::tokenize(s)).size() == 0)
        return nullptr;
    return parse_tokens(tokens);
}

void repl() {
    string s;
    while (true) {
        if (s.length() == 0) break;
        else parse(s)->eval();
    }
}
