OBJS = list.o Lex.o Parse.o
FLAGS = -std='c99'

default : $(OBJS)
	gcc $(FLAGS) $(OBJS) -o YAIL

%.o : %.c
	gcc $(FLAGS) -c $<

