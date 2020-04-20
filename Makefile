#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: archid- <archid-@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/04/09 23:37:54 by archid-           #+#    #+#              #
#    Updated: 2020/04/20 04:24:12 by archid-          ###   ########.fr        #
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
CPPFLAGS    = -Wall -Wextra -pedantic -I$(HEADIR) -std=c++17 -g

all: init $(NAME)

init:
	@echo "CC: " $(CC) $(CPPFLAGS)
	@echo

$(NAME): $(OBJS)
	@echo "CC " $@
	@$(CC) $(CPPFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS)
	@mkdir -p $(@D)
	@echo "CC " $(notdir $<)
	@$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

re: clean all

check: all
	@echo
	@ls -lR
	@echo
	cat -n scheme/basic.scm
	@echo
	./$(NAME) < scheme/basic.scm
	@echo
	cat -n scheme/cons.scm
	@echo
	./$(NAME) < scheme/cons.scm
	@echo
	cat -n scheme/cond.scm
	@echo
	./$(NAME) < scheme/cond.scm

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
