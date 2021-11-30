#define FMT_HEADER_ONLY
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <memory>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include "ServerPackages/ClientConnection.hpp"
using namespace std;

std::chrono::high_resolution_clock::duration ClientConnection::maxSuspension;


typedef std::unordered_map<id_T, std::shared_ptr<ClientConnection>> conn_set;

class Server
{
private:
	int listening_socket;
	int listening_port;
	sockaddr_in server_hint;
	conn_set connections;
	
public:

	Server(const int& port_num)
		: listening_port(port_num)
	{
		createListeningSocket();
		bindlListeningSocket();
		runServer();
    }

	void runServer()
	{
		listen(listening_socket, SOMAXCONN);
		std::cout << "listening ..." << endl;
		while (true)
			this->acceptNewConnection();
	}
	
	inline void acceptNewConnection()
	{
		sockaddr_in client_hint;
		socklen_t client_size = sizeof(client_hint);
		int client_socket = accept(listening_socket, (sockaddr*)&client_hint, &client_size);
		std::cout << "accepted!" << std::endl;
		this->printClientInfo(client_hint);
		
		std::thread new_thread(&Server::handleNewConnection, this, client_socket, client_hint);
		new_thread.detach();
	}

private:
	inline bool createListeningSocket()
	{
		listening_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listening_socket == -1)
		{
			cerr << "Can't create a socket! Quitting" << endl;
			return false;
		}
		return true;
	}

	inline bool bindlListeningSocket()
	{
		server_hint.sin_family = AF_INET;
		server_hint.sin_port = htons(this->listening_port);
		inet_pton(AF_INET, "127.0.0.1", &server_hint.sin_addr);
		while (bind(listening_socket, (sockaddr*)&server_hint, sizeof(server_hint)))
		{
			cout << "Couldn't bind to ip and port trying again ... " << endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
        return true;
	}


	void handleNewConnection(const int& client_socket, const sockaddr_in& client_hint)
	{
		auto new_connection = std::make_shared<ClientConnection>(client_socket, connections);
        auto user_id = new_connection->handleLoginOrRegister();
		if (user_id != INVALID_ID)
		{
			this->connections[user_id] = new_connection;
			new_connection->talk();
			connections.erase(user_id);
		}
	}

    inline void printClientInfo(const sockaddr_in& client)
    {
        char host[NI_MAXHOST];      
        char service[NI_MAXSERV];  
    
        memset(host, 0, NI_MAXHOST); 
        memset(service, 0, NI_MAXSERV);
    
        if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            cout << host << " connected on port " << service << endl;
        }
        else
        {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            cout << host << " connected on port " << ntohs(client.sin_port) << endl;
        }
    }
 
	

public:
    ~Server()
    {
        cout << "Destroyed server" << endl;
    }


};


int main(){
	Server server(54000);
    std::cout << "Hello world!" << std::endl;
	return 0;
}