# Compiler and Flags
CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -I../inc/
LDFLAGS = -pthread

# Directories
SRCDIR = ../src
OBJDIR = obj

# Source Files
CLIENT_SOURCES = wd_client_test.c wd_client.c $(SRCDIR)/scheduler.c $(SRCDIR)/pqueue.c $(SRCDIR)/task.c $(SRCDIR)/uid.c $(SRCDIR)/dlist.c $(SRCDIR)/srtlist.c
PROC_SOURCES = wd.c wd_client.c $(SRCDIR)/scheduler.c $(SRCDIR)/pqueue.c $(SRCDIR)/task.c $(SRCDIR)/uid.c $(SRCDIR)/dlist.c $(SRCDIR)/srtlist.c

# Object Files
CLIENT_OBJECTS = $(addprefix $(OBJDIR)/,$(notdir $(CLIENT_SOURCES:.c=.o)))
PROC_OBJECTS = $(addprefix $(OBJDIR)/,$(notdir $(PROC_SOURCES:.c=.o)))

# Executables
EXECUTABLES = wd_client_test wd

# Compilation Only
all: $(EXECUTABLES)

# Linking Executables
wd_client_test: $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

wd: $(PROC_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compiling Object Files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile wd_client_test.c separately if it is not in SRCDIR
$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run Client After Compilation
run: wd_client_test
	./wd_client_test ./wd_client_test

.PHONY: clean

# Clean Up
clean:
	rm -rf $(OBJDIR) $(EXECUTABLES)
