TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes

CC=${CROSS_COMPILE}gcc

#
# Programs
#
all:	digital.arm

digital.arm:  digital.o gpio-utils.o
	$(CC) $(LDFLAGS) -o $@ $^

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm digital.o digital.arm gpio-utils.o 
