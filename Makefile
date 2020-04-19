#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/09 23:37:54 by archid-           #+#    #+#              #
#    Updated: 2020/04/19 13:15:47 by archid-          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME        = scmin++

SRCDIR      = src
HEADIR      = include
OBJDIR      = .obj

SRCS        = $(shell find $(SRCDIR) -name '*.cpp' -type f)
HDRS        = $(shell find $(HEADIR) -name '*.hpp' -type f)
OBJS        = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

CC          = clang++
CPPFLAGS    = -Wall -Wextra -I$(HEADIR) -std=c++17 -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

re: clean all

check: all
	@echo
	@ls -lR
	@echo
	cat -n scheme/basic.scm
	@echo
	./$(NAME) < scheme/basic.scm

distcheck:
	@uname -a
	@echo
	@$(CC) --version
	@git --version

valg: all
#	@echo
#	@cppcheck -v --std=c++17 .
	@echo
	@valgrind --leak-check=full --show-leak-kinds=all --show-reachable=yes ./$(NAME) < scheme/basic.scm


.PHONY: all re clean
