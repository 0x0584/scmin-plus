// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:20:52 by archid-           #+#    #+#             //
//   Updated: 2020/04/21 20:41:40 by archid-          ###   ########.fr       //
 //                                                                            //
// ************************************************************************** //

#include "parser.hpp"

size_t loop = 1;

void flush(int sig) {
    (void)sig;
    cout << endl << "(" << loop << ")> ";
    cout.flush();
}

int main()
{
    cout << "This software comes with ABSOLUTLY NO WARRANTY." << endl;
    cout << "scmin++ is a basic scheme interpreter, GPLv2\n" << endl;

    signal(SIGINT, &flush);
    sexpr::init_global();
    while (true) {
        sexpr_t e, res;
        string s;

        cout << "(" << loop << ")> ";
        if (not getline(cin, s)) {
            cout << "Bye!" << endl;
            break;
        } else if (not (e = parse(s)))
            continue;
        if ((res = eval(e, sexpr::global))) {
            cout << "=> "<<  res << endl;
            loop++;
        }
    }
    return 0;
}
