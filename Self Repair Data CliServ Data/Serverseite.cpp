#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Fehler beim Erstellen des Server-Sockets\n";
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Fehler beim Binden des Sockets\n";
        close(server_socket);
        return 1;
    }

    listen(server_socket, 5);
    std::cout << "Server gestartet und wartet auf Verbindungen...\n";

    client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
        std::cerr << "Fehler beim Akzeptieren der Verbindung\n";
        close(server_socket);
        return 1;
    }

    std::cout << "Verbindung hergestellt\n";

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Fehler beim Empfangen der Daten\n";
            break;
        }
        std::cout << "Empfangen: " << buffer << "\n";
        send(client_socket, buffer, bytes_received, 0);
    }

    close(client_socket);
    close(server_socket);
    return 0;
}