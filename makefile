# Directories
INCDIR = inc
OBJDIR = obj
LIBDIR = lib
SRCDIR = src
# Compiler def
CC=gcc
cflags=-I$(INCDIR) -L$(LIBDIR) $(LIBS)


_RUN_DEPS = main.h errors.h dataStructs.h
_TEST_DEPS = tests.h test_runner.h test_sts_queue.h  test_TCPmain.h test_dataValues.h test_connValues.h test_setsValues.h
_UDP_DEPS = UDPcomm.h UDPmain.h
_TCP_DEPS = TCPcomm.h TCPmain.h
_FPGA_DEPS = FPGAhandler.h hps.h hps_linux.h hps_soc_system.h socal.h
_THREAD_DEPS = dataValues.h setsValues.h connValues.h sts_queue.h

#LIST OF DEPS
RUN_DEPS = $(patsubst %, $(INCDIR)/%,$(_RUN_DEPS))
TEST_DEPS = $(patsubst %, $(INCDIR)/%,$(_TEST_DEPS))
UDP_DEPS = $(patsubst %, $(INCDIR)/%,$(_UDP_DEPS))
TCP_DEPS = $(patsubst %, $(INCDIR)/%,$(_TCP_DEPS))
FPGA_DEPS = $(patsubst %, $(INCDIR)/%,$(_FPGA_DEPS))
THREAD_DEPS = $(patsubst %, $(INCDIR)/%,$(_THREAD_DEPS))


# LIST OF OBJECTS
_RUN_OBJS = main.o
RUN_OBJS = $(patsubst %,$(OBJDIR)/%,$(_RUN_OBJS))

_TEST_OBJS = tests.o test_sts_queue.o test_runner.o test_FPGAhandler.o test_TCPmain.o test_dataValues.o test_connValues.o test_setsValues.o
TEST_OBJS = $(patsubst %,$(OBJDIR)/%,$(_TEST_OBJS))

_TCP_OBJS = TCPmain.o TCPcomm.o
TCP_OBJS = $(patsubst %,$(OBJDIR)/%,$(_TCP_OBJS))

_UDP_OBJS = UDPmain.o UDPcomm.o
UDP_OBJS = $(patsubst %,$(OBJDIR)/%,$(_UDP_OBJS))

_FPGA_OBJS = FPGAhandler.o hps_linux.o
FPGA_OBJS = $(patsubst %,$(OBJDIR)/%,$(_FPGA_OBJS))

_THREAD_OBJS =  dataValues.o setsValues.o connValues.o sts_queue.o
THREAD_OBJS = $(patsubst %,$(OBJDIR)/%,$(_THREAD_OBJS))



COMPILEFLAGS = -pthread

#RULES
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(RUN_DEPS) $(TEST_DEPS) $(UDP_DEPS) $(TCP_DEPS) $(FPGA_DEPS) $(THREAD_DEPS) 
	gcc $(COMPILEFLAGS) -c -o $@ $< $(CFLAGS)

#TARGETS

all: main

main: $(OBJDIR)/main.o $(RUN_OBJS) $(TEST_OBJS) $(TCP_OBJS) $(UDP_OBJS) $(FPGA_OBJS) $(THREAD_OBJS)
	gcc $(COMPILEFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean clean-targets

clean:
	rm -f $(OBJDIR)/*.o $(SRCDIR)/*~ core $(INCDIR)/*~ *~

clean-targets:
	rm -f main