OBJS = list.o Lex.o Parse.o

default : $(OBJS)
	gcc $(OBJS) -o YAIL

%.o : %.c
	gcc -c $<
