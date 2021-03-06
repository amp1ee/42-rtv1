NAME	=RTv1
CC		=clang
CFLAGS	=-Wall -Wextra -Werror
ifdef DEBUG
	CFLAGS	+=-g
endif
CFLAGS	+=-Ofast

SRCDIR	=./src/
OBJDIR	=./obj/
INCLDIR	=./include/
LIBFTDIR=./libft/
SDLDIR	=./sdl2/

SRC		=$(addprefix $(SRCDIR),	\
			main.c				\
			render.c			\
			render_utils.c		\
			vec3_basic.c		\
			vec3_utils.c		\
			parser.c			\
			controls.c			\
			misc.c				\
			lights.c			\
			plane.c				\
			sphere.c			\
			cylinder.c			\
			cone.c				)
OBJ		=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
INCLUDE	=-I$(INCLDIR) -I$(LIBFTDIR)
HEADERS	=$(addprefix $(INCLDIR), rtv1.h)
LIBFT	=$(addprefix $(LIBFTDIR), libft.a)
LFLAGS	=-L$(LIBFTDIR) -lft -lm

OSNAME	=$(shell uname -s)
ifeq ($(OSNAME), Darwin)
	CG_LIBS	=-framework OpenGL -framework AppKit -framework OpenCL	\
				-framework SDL2 -F$(SDLDIR) -rpath $(SDLDIR)
	INCLUDE	+=-F$(SDLDIR)							\
				-I$(SDLDIR)SDL2.framework/Headers/
else
	CG_LIBS = `sdl2-config --libs`
endif

.PHONY:	all clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(info Linking objects)
	@$(CC) $(OBJ) -o $(NAME) $(LFLAGS) $(CG_LIBS)
	$(info Done.)

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADERS)
	$(info Compiling $<)
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(LIBFT):
	@make -sC $(LIBFTDIR)

clean:
	$(info Deleting objects)
	@rm -rf $(OBJDIR)
	@make -sC $(LIBFTDIR) clean

fclean: clean
	$(info Deleting $(NAME))
	@rm -rf $(NAME)
	@make -sC $(LIBFTDIR) fclean

re: fclean all
