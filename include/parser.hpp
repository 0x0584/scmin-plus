// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   parser.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/04/10 08:09:28 by archid-           #+#    #+#             //
//   Updated: 2020/04/23 07:55:56 by archid-          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARSER_H
# define PARSER_H

# include <csignal>
# include "token.hpp"

sexpr_t parse(string s);
sexpr_t parse_tokens(queue<token>& q, bool quasi = false);

#endif
