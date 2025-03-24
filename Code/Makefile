CC = gcc
CFLAGS = -Wall -Wno-deprecated-declarations -Iinclude
OBJDIR = obj
OBJ = $(OBJDIR)/main.o $(OBJDIR)/products.o $(OBJDIR)/db_handler.o $(OBJDIR)/menu.o \
      $(OBJDIR)/quotations.o $(OBJDIR)/invoices.o $(OBJDIR)/models.o

all: program

program: $(OBJ)
	$(CC) $(OBJ) -o program -lmysqlclient -lcrypto

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) program
