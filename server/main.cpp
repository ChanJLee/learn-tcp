#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>

int handle_request(int fd) {
	std::cout << "handle request" << std::endl;	
	char buffer[256];
	int len = 0;
	while((len = read(fd, buffer, sizeof(buffer))) > 0) {
		std::cout << "read len: " << len << std::endl;
		write(fd, buffer, len);
		if (buffer[len - 1] == '\n') {
			std::cout << "end" << std::endl;
			break;
		} 
	}
	return 0;
}

int main(int argc, char* argv[])
{
	std::cout << "starting server..." << std::endl;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		std::cerr << "open socket fd failed" << std::endl;
		return -1;
	}

	sockaddr_in server_sockaddr, client_sockaddr;
	memset(&server_sockaddr, 0, sizeof(sockaddr_in));
	memset(&client_sockaddr, 0, sizeof(sockaddr_in));
	
	int port = 9527;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_sockaddr.sin_port = htons(port);
	if (bind(server_fd, (const sockaddr*) (&server_sockaddr), sizeof(sockaddr_in)) < 0) {
		std::cerr << "bind failed" << std::endl;
		return -2;
	}
	
	if (listen(server_fd, 1024) < 0) {
		std::cerr << "listen failed" << std::endl;
		return -3;
	}

	std::cout << "start success, port: " << port << std::endl;
	while(true) {
		std::cout << "try to accept" << std::endl;
		socklen_t client_sockaddr_len;
		int client_fd = accept(server_fd, (sockaddr*) (&client_sockaddr), &client_sockaddr_len);
		std::cout << "accepted: " << client_fd << std::endl;
		if (fork() == 0) {
			close(server_fd);
			exit(handle_request(client_fd));
		}
		close(client_fd);
	}

	return 0;
}
