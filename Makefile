CC=g++
CFLAGS=-I.
DEPS = bmp.h bsave.h conv.h
OBJ = bmp.o bsave.o conv.o bmp2yjk.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bmp2yjk: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean:
	rm -f *.o *~ bmp2yjk

