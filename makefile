-----------------------------------------------------
							  
-----------------------------------------------------


# Compiler variables
CC = gcc
CFLAGS = -Wall -I $(INCLUDE_DIR)

# Directory variables
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_DIR = include

EXEC_NAME = HospitalSO

# Objects variables
OBJS = main.o memory.o process.o receptionist.o patient.o doctor.o

main.o = main.h
memory.o = memory.h
process.o = process.h
receptionist.o = receptionist.h
patient.o = patient.h
doctor.o = doctor.h

# Object files path
vpath %.o $(OBJ_DIR)
vpath %.c $(SRC_DIR)

# Make targets
$(BIN_DIR)/$(EXEC_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_DIR)/,$(OBJS)) -o $@

%.o: %.c $($@)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/$@ -c $<

# Create folders (PHONY)
folders:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Clean object files and executable (PHONY)
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/$(EXEC_NAME)
