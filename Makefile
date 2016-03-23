.PHONY:output
output:
	mkdir include
	g++ -o libvld.so vld.cpp -fPIC -shared
	mv libvld.so ./include
	cp ./vld.h ./include
.PHONY:clean
clean:
	rm -rf include 

#test1:test.o vld.o
# 	g++ -o $@ $^
#
#test.o:test.cpp
#	g++ -c $@ $<
#
#vld.o:vld.cpp
#	g++ -c $@ $<
#
#.PHONY:clean
#clean:
#	rm *.o test1

