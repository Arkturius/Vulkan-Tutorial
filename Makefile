# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/24 14:17:35 by rgramati          #+#    #+#              #
#    Updated: 2024/05/23 19:55:19 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLACK		=	\033[30;49;3m
RED			=	\033[31;49;3m
GREEN		=	\033[32;49;3m
YELLOW		=	\033[33;49;3m
BLUE		=	\033[34;49;3m
MAGENTA		=	\033[35;49;3m
CYAN		=	\033[36;49;3m
WHITE		=	\033[37;49;3m

BBLACK		=	\033[30;49;3;1m
BRED		=	\033[31;49;3;1m
BGREEN		=	\033[32;49;3;1m
BYELLOW		=	\033[33;49;3;1m
BBLUE		=	\033[34;49;3;1m
BMAGENTA	=	\033[35;49;3;1m
BCYAN		=	\033[36;49;3;1m
BWHITE		=	\033[37;49;3;1m

RESET		=	\033[0m

LINE_CLR	=	\33[2K\r

FILE	=	$(shell ls -lR | grep -F .c | wc -l)
CMP		=	1

NAME        :=	app

SRCS		:=	src/app.c				\
				src/err.c				\
				src/vulkan.c			\
				src/extensions.c		\
				src/validation_layers.c	\
				src/queue_families.c	\
				src/devices.c			\
				src/window_surface.c	\
				src/swapchain.c			\
				src/images.c			\
				src/pipeline.c			\
				src/commands.c

OBJS_DIR	:=	.objs

OBJS		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))

SHADERS		:=	src/shaders/

CC          :=	cc

CFLAGS      :=	-g3 -Wall -Wextra -Werror

ifeq ($(DEBUG), 1)
	CFLAGS 	+=	-fsanitize=address -static-libsan
endif

IFLAGS	    :=	-I ./include

LDFLAGS		:=	-lglfw -lvulkan -ldl -pthread -lX11 -lXxf86vm -lXrandr -lXi

RM          :=	rm -rf

MAKEFLAGS   +=	--no-print-directory

DIR_DUP     :=	mkdir -p

# RULES ********************************************************************** #

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	glslc $(SHADERS)shader.vert -o $(SHADERS)vert.spv
	glslc $(SHADERS)shader.frag -o $(SHADERS)frag.spv
	@printf "$(LINE_CLR)  ⭐$(BWHITE) $(NAME):\t PROJECT READY !$(RESET)\n\n"

$(OBJS_DIR)/%.o: %.c
	@$(DIR_DUP) $(@D)
	if [ $(CMP) -eq '1' ]; then \
		printf "\n"; \
	fi;
	printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $(CMP)/$(FILE)\t$(GREEN)compiling...$(RESET)\t$(BWHITE)$<$(RESET)"
	@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $^
	@$(eval CMP=$(shell echo $$(($(CMP)+1))))
	if [ $(CMP) -gt $(FILE) ]; then \
		printf "$(LINE_CLR)$(WHITE)  🔄 $(NAME): $$(($(CMP)-1))/$(FILE)\n$(LINE_CLR)$(BGREEN)  ✅ Compilation done !$(RESET)\n"; \
	fi \

clean:
	@printf "\n$(BWHITE)  🚫 $(NAME):\t$(BRED) .o files deleted.$(RESET)\n"
	@$(RM) $(OBJS)

dclean: clean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) objs dir deleted.$(RESET)\n"
	@$(RM) $(OBJS_DIR)

fclean: dclean
	@printf "$(BWHITE)  🚫 $(NAME):\t$(BRED) binary deleted.$(RESET)\n"
	@$(RM) $(NAME)

remake:
	@$(MAKE) clean
	@$(MAKE) all

re:
	@$(MAKE) fclean
	@$(MAKE) all

# **************************************************************************** #

.PHONY: all clean fclean dclean re
.SILENT:
