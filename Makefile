CC=g++

volimage: VolImage.o
	$(CC) VolImage.o -o volimage

VolImage.o: VolImage.cpp VolImage.h
	$(CC) -c VolImage.cpp

run: volimage
	./volimage "MRI"
	
diffmap: volimage
	./volimage "MRI" -d 1 2 "output"
	
extract: volimage
	./volimage "MRI" -x 1 "output"

clean:
	rm -f *.o volimage
	rm -f *.dat
	rm -f *.raw

tidy:
	rm -f *.dat
	rm -f *.raw
	
	

