CC = gcc
SRC = src
INC = include
CFLAGS = -O3 -fbounds-check -Wall -Wextra -I$(INC) -lm -g
EXEC = szabo

all:  
	$(CC) $(SRC)/main.c $(SRC)/initialize.c $(SRC)/distance.c $(SRC)/save_position.c $(SRC)/update.c $(SRC)/calc_box.c $(SRC)/box_list.c -o $(EXEC) $(CFLAGS)

clean:
	rm src/*~	
	rm $(INC)/*~

