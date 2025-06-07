#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;
const char *SERVER_IP = "192.168.1.1";

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Fehler beim Erstellen des Client-Sockets\n";
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Ungültige Server-Adresse\n";
        close(client_socket);
        return 1;
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Verbindung zum Server fehlgeschlagen\n";
        close(client_socket);
        return 1;
    }

    std::cout << "Verbunden mit Server\n";

    while (true) {
        std::cout << "Nachricht an Server: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        if (std::cin.fail() || strlen(buffer) == 0) {
            break;
        }
        send(client_socket, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            std::cerr << "Fehler beim Empfangen der Daten\n";
            break;
        }
        std::cout << "Antwort vom Server: " << buffer << "\n";
    }

    close(client_socket);
    return 0;
}