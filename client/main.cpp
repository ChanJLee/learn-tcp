#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main(int argc, char const *argv[])
{
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "open socket failed" << std::endl;
        return -1;
    }


    in_port_t port = 9527;
    const char * ip = "139.180.205.93";
    std::cout << "connect " << ip << " port: " << port << std::endl;

    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        std::cerr << "covert ip address failed" << std::endl;
        return -2;
    }

    if (connect(fd, (sockaddr*) &addr, sizeof(sockaddr_in)) < 0) {
        std::cerr << "connect failed" << std::endl;
        return -3;
    }

    std::cout << "connected" << std::endl;

    int len = 0;
    char buffer[256] = "what fuck\n";
    std::cout << "write len: " << write(fd, buffer, strlen(buffer)) << std::endl;
    while((len = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[len] = '\0';
        std::cout << buffer;
	if (buffer[len - 1] == '\n') {
		break;
	}
    }

    std::cout << std::endl;

    close(fd);
    return 0;
}
