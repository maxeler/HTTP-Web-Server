NAME		= ExampleProject
EXEC		= $(NAME)

CC		= gcc
CFLAGS		= -std=gnu99 -Wall
LDFLAGS		= -lm

OBJ		= ExampleProjectControl.o ExampleProjectData.o

all:		run

$(EXEC):	$(OBJ)
		$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

run:		$(EXEC)
.INTERMEDIATE: 	$(OBJ)

clean:
		rm $(EXEC)	
