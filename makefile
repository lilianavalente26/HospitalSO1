OBJ_dir = obj
OBJECTOS = 
main.o = main.h
doctor.o = doctor.h
memory.o = memory.h
patient.o = patient.h
process.o = process.h
receptionist.o = receptionist.h
CC = gcc

out: $(OBJECTOS)
$(CC) $(addprefix $(OBJ_dir)/,$(OBJECTOS)) -o bin/out
%.o: src/%.c $($@)
$(CC) -I include -o $(OBJ_dir)/$@ -c $<

clean:
	$(info A REMOVER OS FICHEIROS OBJETO E O FICHEIRO EXECUTAVEL)
	rm -f *.o out
	rm -f out