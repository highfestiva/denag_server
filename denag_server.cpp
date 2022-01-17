#ifdef WIN32
#include <WinSock2.h>
#define close closesocket
#pragma comment(lib, "Ws2_32.lib")
#else
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#endif
#include <string>
#include <time.h>
#include <unordered_map>


#define PORT 3384
using namespace std;
typedef unordered_map<string, string> smap;


smap map;


int main(int argv, char* argc[]) {

	char value[256];
	sprintf(value, "%d-%d", 120 * 60, (int)time(0));
	//map["lokis_gametime"] = value;
	map["mirjis_gametime"] = value;

	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

#ifdef WIN32
	WSADATA socket_data;
	WSAStartup(MAKEWORD(2, 2), &socket_data);
#endif

	int s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1) {
		perror("Unable to create socket");
		return 1;
	}
	sockaddr_in addr;
	bzero((char*)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);
	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("Unable to bind socket");
		return 1;
	}
	listen(s, 2);

	printf("Server started.\n");
	for (;;) {
		sockaddr client_addr;
		socklen_t __size = sizeof(client_addr);
		int cs = accept(s, &client_addr, &__size);
		char data[256];
		int len = recv(cs, data, (int)sizeof(data), 0);
		size_t klen = strnlen(data, 256 - 1 - 1);
		const string key(data, klen);
		size_t vlen = strnlen(data + klen + 1, 256 - klen - 1);
		string value(data+klen+1, vlen);
		if (value.empty()) {
			smap::iterator i = map.find(key);
			if (i != map.end()) {
				value = i->second;
			}
			printf("Client getting '%s'='%s'.\n", key.c_str(), value.c_str());
		} else {
			printf("Client setting '%s'='%s'.\n", key.c_str(), value.c_str());
			map[key] = value;
		}
		send(cs, value.c_str(), value.size()+1, 0);
		close(cs);
	}
}
