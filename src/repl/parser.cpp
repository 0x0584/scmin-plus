// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parser.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/12 02:28:48 by archid-           #+#    #+#             //
//   Updated: 2020/04/24 21:29:32 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "parser.hpp"

#define tok q.front()

inline static bool is_quote(token::tok_type type) {
    return type == token::tok_quote or type == token::tok_unquote
        or type == token::tok_quasi or type == token::tok_unquote_sp;
}

inline static bool is_unquote(token::tok_type type) {
    return type == token::tok_unquote or type == token::tok_unquote_sp;
}

sexpr_t parse_quote(queue<token>& q, bool quasi) {
    string sy;

    switch (tok.type) {
    case token::tok_unquote:    sy = "unquote"; break;
    case token::tok_unquote_sp: sy = "unquote-splicing"; break;
    case token::tok_quote:      sy = "quote"; break;
    case token::tok_quasi:      sy = "quasiquote"; break;
    default: break;
    }
    if (q.pop(); q.size() == 0 || tok.type == token::tok_r_paren) {
        cerr << "parsing error: quotes are mal formed!" << endl;
        return nullptr;
    }
    sexpr_t e = parse_tokens(q, quasi);
    if (not e or e->isnil())
        return e;
    return cons(symb(sy), cons(e, nil()));
}

sexpr_t parse_pair(queue<token>& q, const sexpr_t& e, sexpr_t& tail) {
    if (q.pop(); not e or tok.type == token::tok_r_paren) {
        cerr << "parsing error! pair is not valid" << endl;
        return nullptr;
    }
    sexpr_t tmp = tok;
    if (q.pop(); not tmp->isatom() or tok.type != token::tok_r_paren) {
        cerr << "parsing error! pair is not valid" << endl;
        return nullptr;
    }
    tail->setcdr(tmp);
    return e;
}

sexpr_t parse_lambda(queue<token>& q, sexpr_t& e) {
    if (q.pop(); e or tok.type != token::tok_l_paren) {
        cerr << "lambda args is mal-formatted" << endl;
        return nullptr;
    }
    auto args = parse_tokens(q);
    if (tok.type != token::tok_l_paren) {
        cerr << "lambda body is mal-formatted" << endl;
        return nullptr;
    }
    auto body = parse_tokens(q);
    if (tok.type != token::tok_r_paren) {
        cerr << "lambda is mal-formatted" << endl;
        return nullptr;
    }
    return (e = lambda(args, body));
}

sexpr_t parse_cons_flavor(queue<token>& q, sexpr_t& expr) {
    token tmp = tok;

    if (q.pop(); q.size() == 0 or tok.type == token::tok_r_paren) {
        cerr << "Err: malformed car/cdr" << endl;
        return nullptr;
    }
    sexpr_t e = parse_tokens(q);
    sexpr_t tail = nil();

    if (tmp.isset) {
        if ((q.size() == 0 or tok.type == token::tok_r_paren)) {
            cerr << "Err: malformed car/cdr" << endl;
            return nullptr;
        }
        tail = cons(parse_tokens(q), tail);
    }
    if (not e or e->isnil())
        return e;
    for (; tmp.cons_s.size() > 1; tmp.cons_s.pop())
        e = cons(symb(tmp.cons_s.top() == 'a' ? "car" : "cdr"),
                 cons(e, nil()));
    expr = cons(symb(tmp.cons_s.top() == 'a' ?
                     (tmp.isset ? "set-car!" : "car")
                     : (tmp.isset ? "set-cdr!" : "cdr")),
                cons(e, tail));
    return expr;
}

sexpr_t parse_list(queue<token>& q, bool quasi) {
    sexpr_t e, tail;
    bool op = true;

    if (q.pop(); tok.type == token::tok_r_paren) {
        q.pop();
        return nil();
    }
    while (tok.type != token::tok_r_paren) {
        sexpr_t tmp;

        if (tok.type == token::tok_l_paren)
            tmp = parse_tokens(q, quasi);
        else if (tok.type == token::tok_cons_flavor) {
            if (not (op and parse_cons_flavor(q, e))) {
                cerr << "Err: car/cdr must be called as operators!" << endl;
                return nullptr;
            }
            break;
        } else if (is_quote(tok.type)) {
            if (not quasi and is_unquote(tok.type)) {
                cerr << "Err: unquote appeared without quasiquote" << endl;
                return nullptr;
            }
            tmp = parse_quote(q, quasi or tok.type == token::tok_quasi);
        } else if (tok.type == token::tok_pair) {
            if (not parse_pair(q, e, tail))
                return nullptr;
            break;
        } else if (tok.type == token::tok_lambda) {
            if (not (op and parse_lambda(q, e))) {
                cerr << "Err: Lambda is mal formatted" << endl;
                return nullptr;
            }
            break;
        } else {
            tmp = tok; q.pop();
        }
        if (not cons_append(tmp, e, tail))
            return nullptr;
        op = false;
    }
    q.pop();
    return e;
}

sexpr_t parse_tokens(queue<token>& q, bool quasi) {
    sexpr_t e;

    if (tok.type == token::tok_l_paren) {
        e = parse_list(q, quasi);
    } else if (is_quote(tok.type)) {
        if (not quasi and is_unquote(tok.type)) {
            cerr << "Err: unquote appeared without quasiquote" << endl;
            return nullptr;
        }
        e = parse_quote(q, quasi or tok.type == token::tok_quasi);
    } else if (tok.type == token::tok_pair) {
        cerr << "parsing error! pair is not valid" << endl;
        return nullptr;
    } else if (tok.type == token::tok_cons_flavor) {
        cerr << "Err: car/cdr must be called as operators!" << endl;
        return nullptr;
    } else {
        e = tok, q.pop();
    }
    return e;
}

sexpr_t parse(string s) {
    queue<token> tokens;
    sexpr_t e;

    if ((tokens = token::tokenize(s)).size() == 0)
        return nullptr;
    e = parse_tokens(tokens);
    cout << "expr: " << e << endl;
    if (not e)
        cerr << "Err: expression is mal-formed" << endl;
    return e;
}
