INC= -I ./include/

OBJ = rs485.o \
	  rs485_example.o \


TARGET = rs485_exam

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ) -lpthread -lm

%.o: %.c
	gcc $(INC) -Wall -c $^ -o $@
.PHONY: clean
clean:
	rm -f $(OBJDIR)/$(TARGET) $(OBJ)
