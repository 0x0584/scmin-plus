// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   repl.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/12 02:28:48 by archid-           #+#    #+#             //
//   Updated: 2020/04/21 00:21:21 by archid-          ###   ########.fr       //
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
    if (not e or e->isnil()) return e;
    return cons(symb("quote"), cons(e, nil()));
}

sexpr_t parse_tokens(queue<token>& q) {
    sexpr_t e;
    token tok;

    if (q.front().type == token::tok_l_paren) {
        q.pop();
        if (q.front().type == token::tok_r_paren) {
            q.pop();
            return nil();
        }
        sexpr_t tail;

        while (q.front().type != token::tok_r_paren) {
            sexpr_t tmp;

            if (q.front().type == token::tok_l_paren)
                tmp = parse_tokens(q);
            else if (q.front().type == token::tok_quote)
                tmp = parse_quote(q);
            else if (q.front().type == token::tok_pair) {
                q.pop();
                if (not e or q.front().type == token::tok_r_paren) {
                    cerr << "parsing error! pair is not valid" << endl;
                    return nullptr;
                }
                tmp = q.front(); q.pop();
                if (not tmp->isatom() or q.front().type != token::tok_r_paren) {
                    cerr << "parsing error! pair is not valid" << endl;
                    return nullptr;
                }
                tail->setcdr(tmp);
                break;
            } else if (q.front().type == token::tok_lambda) {
                q.pop();
                if (e or q.front().type != token::tok_l_paren) {
                    cerr << "lambda args is mal-formatted" << endl;
                    return nullptr;
                }
                auto args = parse_tokens(q);
                if (q.front().type != token::tok_l_paren) {
                    cerr << "lambda body is mal-formatted" << endl;
                    return nullptr;
                }
                auto body = parse_tokens(q);
                if (q.front().type != token::tok_r_paren) {
                    cerr << "lambda is mal-formatted" << endl;
                    return nullptr;
                }
                e = lambda(args, body);
                break;
            } else {
                tmp = q.front(); q.pop();
            }

            if (not tmp)
                return nullptr;
            else if (not e)
                tail = e = cons(tmp, nil());
            else {
                tail->setcdr(cons(tmp, nil()));
                tail = tail->cdr();
            }
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


size_t loop = 1;

void flush(int sig) {
    (void)sig;
    cout << endl << "(" << loop << ")> ";
    cout.flush();
}

void repl() {
    string s;
    sexpr_t e;

    cout << "This software comes with ABSOLUTLY NO WARRANTY." << endl;
    cout << "scmin++ is a basic scheme interpreter, GPLv2\n" << endl;

    signal(SIGINT, &flush);
    sexpr::init_global();
    while (true) {
        cout << "(" << loop << ")> ";
        if (not getline(cin, s)) {
            cout << "Bye!" << endl;
            break;
        } else if (not (e = parse(s)))
            continue;
        loop++;
        e = eval(e, sexpr::global);
        cout << "=> "<<  e << endl;
    }
}
