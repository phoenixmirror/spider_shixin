
CC	=	gcc -g 
#CC	=	gcc -g -O2 -fPIC

OBJS	=	$(C_SRCS:.c=.o)

all	:	clean shixin_unit_downloadlist_and_detail $(OBJS) db.c db_schedule.c

C_SRCS  =       common.c spider.c dlist.c main.c debuglog.c db.c db_schedule.c


CFLAGS  =       -I.				\
		-I$(HOME)/include               \
		-I /usr/include/libxml2		\
		-I /home/idd/src/shixin_court_gov_cn 
		
DFLAGS	=	-DMULTI_PROC
#DFLAGS	=	-DCOMMON
#DFLAGS	=	-DTHREAD

LFLAGS  =       -L.                             \
		-L$(HOME)/lib                   \
		-lhzb_log			\
		-lxml2                          \
		-lcurl				\
		-lpthread			\
		-lecpg
		

shixin_unit_downloadlist_and_detail:	$(OBJS)
	@echo 'Building target: $@'
	$(CC) $(DFALGS) -o $@ $(LFLAGS) $(OBJS) 
	cp $@ $(HOME)/bin
	cp batch_unit.sh $(HOME)/shbin

.c.o	:
	@echo 'Building target: $@'
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

db.c	: db.ec
	ecpg db.ec	


db_schedule.c : db_schedule.ec
	ecpg db_schedule.ec

clean	:
	rm -f *.o
	rm -f spider 
	rm -f db.c
	rm -f db_schedule.c

