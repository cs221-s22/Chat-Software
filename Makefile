OBJS =  project05.o presence.o client.o
PROG = project05

%.o: %.c 
	gcc -c -g -o $@ $<  

$(PROG): project05.h $(OBJS)
	gcc -lm -g -o $@ $^

clean:
	rm -rf $(OBJS) $(PROG)
