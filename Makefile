CC = gcc
CFLAGS = -g -Wall

INCLUDES = -I/usr/local/include
LFLAGS=
LIBS = -lzmq

SRCS = play0mq.c

OBJS = $(SRCS:.c=.o)

MAIN = play0mq

.PHONY: depend clean

all:    $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE

play0mq.o: /usr/local/include/zmq.h /usr/include/errno.h
play0mq.o: /usr/include/sys/errno.h /usr/include/sys/cdefs.h
play0mq.o: /usr/include/sys/_symbol_aliasing.h
play0mq.o: /usr/include/sys/_posix_availability.h
play0mq.o: /usr/include/sys/_types/_errno_t.h /usr/include/stddef.h
play0mq.o: /usr/include/_types.h /usr/include/sys/_types.h
play0mq.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
play0mq.o: /usr/include/sys/_types/_ptrdiff_t.h
play0mq.o: /usr/include/sys/_types/_size_t.h
play0mq.o: /usr/include/sys/_types/_rsize_t.h
play0mq.o: /usr/include/sys/_types/_wchar_t.h
play0mq.o: /usr/include/sys/_types/_wint_t.h /usr/include/sys/_types/_null.h
play0mq.o: /usr/include/stdio.h /usr/include/Availability.h
play0mq.o: /usr/include/AvailabilityInternal.h
play0mq.o: /usr/include/sys/_types/_va_list.h
play0mq.o: /usr/include/sys/_types/_off_t.h
play0mq.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
play0mq.o: /usr/include/secure/_common.h /usr/include/stdint.h
play0mq.o: /usr/include/sys/_types/_int8_t.h
play0mq.o: /usr/include/sys/_types/_int16_t.h
play0mq.o: /usr/include/sys/_types/_int32_t.h
play0mq.o: /usr/include/sys/_types/_int64_t.h /usr/include/_types/_uint8_t.h
play0mq.o: /usr/include/_types/_uint16_t.h /usr/include/_types/_uint32_t.h
play0mq.o: /usr/include/_types/_uint64_t.h
play0mq.o: /usr/include/sys/_types/_intptr_t.h
play0mq.o: /usr/include/sys/_types/_uintptr_t.h
play0mq.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
play0mq.o: /usr/include/unistd.h /usr/include/sys/unistd.h
play0mq.o: /usr/include/sys/_types/_posix_vdisable.h
play0mq.o: /usr/include/sys/_types/_seek_set.h
play0mq.o: /usr/include/sys/_types/_gid_t.h /usr/include/sys/_types/_pid_t.h
play0mq.o: /usr/include/sys/_types/_uid_t.h
play0mq.o: /usr/include/sys/_types/_useconds_t.h /usr/include/sys/select.h
play0mq.o: /usr/include/sys/appleapiopts.h /usr/include/sys/_types/_fd_def.h
play0mq.o: /usr/include/sys/_types/_timespec.h
play0mq.o: /usr/include/sys/_types/_timeval.h
play0mq.o: /usr/include/sys/_types/_time_t.h
play0mq.o: /usr/include/sys/_types/_suseconds_t.h
play0mq.o: /usr/include/sys/_types/_sigset_t.h
play0mq.o: /usr/include/sys/_types/_fd_setsize.h
play0mq.o: /usr/include/sys/_types/_fd_set.h
play0mq.o: /usr/include/sys/_types/_fd_clr.h
play0mq.o: /usr/include/sys/_types/_fd_isset.h
play0mq.o: /usr/include/sys/_types/_fd_zero.h
play0mq.o: /usr/include/sys/_types/_fd_copy.h /usr/include/sys/_select.h
play0mq.o: /usr/include/sys/_types/_dev_t.h /usr/include/sys/_types/_mode_t.h
play0mq.o: /usr/include/sys/_types/_uuid_t.h /usr/include/gethostuuid.h
play0mq.o: /usr/include/string.h /usr/include/strings.h
play0mq.o: /usr/include/secure/_string.h
