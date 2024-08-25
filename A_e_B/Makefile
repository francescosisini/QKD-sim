# Definizione del compilatore
CC = gcc

# Opzioni del compilatore
CFLAGS = -Wall -g

# File di output
TARGET = qkd_simulation

# File sorgenti
OBJS = bob.o terminale_lib.o

# Regola per la compilazione finale
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilazione dei file sorgenti
bob.o: bob.c terminale_lib.h
	$(CC) $(CFLAGS) -c bob.c

terminale_lib.o: terminale_lib.c terminale_lib.h
	$(CC) $(CFLAGS) -c terminale_lib.c

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJS) $(TARGET)
