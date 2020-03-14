CC = g++ -Wall -Werror -pedantic -ggdb

app.exe: main.o scene.o solid_object.o triangle_object.o sphere.o optics.o algebra.o color.o vector.o image.o intersection.o lodepng.o
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) -c $^

clean: 
	rm *.o *.exe
