CC = gcc
SRC_DIRE = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

main.o = main.h
doctor.o = doctor.h
memory.o = memory.h
patient.o = patient.h
process.o = process.h
receptionist.o = receptionist.h

out: $(OBJECTOS)
$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/out
%.o: src/%.c $($@)
$(CC) -I include -o $(OBJ_dir)/$@ -c $<

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: clean