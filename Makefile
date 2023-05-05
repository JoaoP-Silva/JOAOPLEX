CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LDFLAGS += -L./gmp-6.2.1/.libs
LDLIBS += -lgmp

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = .

SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCFILES))

$(BINDIR)/programa: $(OBJFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/JOAOPLEX

run:
	./JOAOPLEX input.txt output.txt
