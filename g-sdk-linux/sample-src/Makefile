# OS, DISTRO, MACHINE
OS := $(shell uname -s)
ifeq ($(OS), Linux)
	ifneq (,$(wildcard /etc/os-release))
		DISTRO := $(shell cat /etc/os-release | awk -F= '/^ID\=/{print $$2}' | sed 's/\"//g')
	else
		DISTRO := $(shell head -1 /etc/issue | awk '{print $$1}')
	endif
	MACHINE := $(shell uname -m)
	SDKLIBPATH := /$(DISTRO)-$(MACHINE)
endif

# Set to use Gnu debugger
GDB_FLAG = -g -O3

CFG_INC = -I../include -I/usr/include -I/usr/local/include -I/usr/local/include/cjson
LIB_PATH = -L/usr/local/lib -L/usr/lib64 -L../lib$(SDKLIBPATH)

# Initialize other variables
OUTDIR  = .
OBJDIR  = $(OUTDIR)
OUTFILE = $(OUTDIR)/main

DEBUG_FLAG = -Wall -DLINUX # -DDEBUG

LIB = -lpthread -lcjson -lginside -lKwsRnet

OBJ =	$(OBJDIR)/main.o     \
		$(OBJDIR)/agent_callback.o

COMPILE = g++ -w -c $(GDB_FLAG) $(DEBUG_FLAG) -o "$(OUTDIR)/$(*F).o" $(CFG_INC) \
			"$<"
LINK = g++ $(GDB_FLAG) -o "$(OUTFILE)" $(OBJ) $(CFG_LIB) $(LIB_PATH) $(LIB)

# Pattern rules

$(OUTDIR)/%.o : %.cpp
	$(COMPILE)

$(OUTFILE): $(OUTDIR) $(OBJ) $(INC)
	$(LINK)

# Create the release dir if doesn't exist
$(OUTDIR):
	mkdir -p "$(OUTDIR)"

# Build rules
all:
	$(MAKE) $(OUTFILE)

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	rm -f $(OUTFILE)
	rm -f $(OBJ)

# Clean this project and all dependencies
cleanall: clean
