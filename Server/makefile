server:
	g++ Server/Server.cpp -lpthread -lmysqlcppconn 
	./a.out
	rm ./a.out

test:
	g++ ./testing/Test.cpp -lpthread -lmysqlcppconn -lGL -lGLU -lglut
	./a.out
	rm ./a.out
	
client:
	g++ Client/client.cpp
	./a.out
	rm ./a.out