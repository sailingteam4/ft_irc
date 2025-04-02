#include "Server.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    
    std::string port(argv[1]);
    std::string password(argv[2]);

    if (password.empty())
    {
        std::cerr << "Error: Password cannot be empty" << std::endl;
        return 1;
    }

    Server server(port, password);


    // Crée le socket (IPv4, TCP)
    // AF_INET est la famille d'adresses pour IPv4
    // SOCK_STREAM est le type de socket pour TCP
    // 0 est le protocole, généralement défini à 0 pour choisir le protocole par défaut pour le type de socket
    // La fonction socket retourne un descripteur de fichier pour le nouveau socket, ou -1 en cas d'erreur
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Error: Could not create socket" << std::endl;
        return 1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET; // IPv4
    hint.sin_port = htons(54000); // Convertit le numéro de port en ordre d'octets réseau car l'ordre des octets diffère selon les machines
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // Convertit l'adresse IP de la forme chaîne à la forme binaire car l'API socket utilise la forme binaire pour les adresses IP

    // Lie le socket à l'adresse IP et au port
    // La fonction bind associe le socket à l'adresse et au numéro de port spécifiés
    // Le premier argument est le descripteur de fichier du socket, le deuxième argument est un pointeur vers une structure sockaddr contenant l'adresse et le numéro de port, et le troisième argument est la taille de la structure sockaddr
    // Si la fonction bind retourne -1, cela signifie que l'opération de liaison a échoué
    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        std::cerr << "Error: Could not bind socket" << std::endl;
        return 1;
    }


    // Écoute les connexions entrantes
    // La fonction listen marque le socket comme un socket passif qui sera utilisé pour accepter les demandes de connexion entrantes
    // Le premier argument est le descripteur de fichier du socket, et le deuxième argument est le nombre maximum de connexions en attente qui peuvent être mises en file d'attente avant que le noyau commence à rejeter de nouvelles connexions
    // Si la fonction listen retourne -1, cela signifie que l'opération d'écoute a échoué
    // SOMAXCONN est une constante qui spécifie le nombre maximum de connexions en attente qui peuvent être mises en file d'attente
    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Error: Could not listen on socket" << std::endl;
        return 1;
    }


    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST]; // Nom distant du client*
    char service[NI_MAXSERV]; // Service (port) sur lequel le client se connecte


    // Accepte une connexion client
    // La fonction accept bloque le programme jusqu'à ce qu'un client se connecte au serveur
    // Le premier argument est le descripteur de fichier du socket, le deuxième argument est un pointeur vers une structure sockaddr qui sera remplie avec l'adresse et le numéro de port du client, et le troisième argument est un pointeur vers une variable socklen_t qui spécifie la taille de la structure sockaddr
    // Si la fonction accept retourne -1, cela signifie que l'opération d'acceptation a échoué
    // La fonction accept retourne un nouveau descripteur de fichier de socket qui est utilisé pour communiquer avec le client
    std::cout << "Waiting for a connection..." << std::endl;
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == -1)
    {
        std::cerr << "Error: Could not accept connection" << std::endl;
        return 1;
    }
    
    close(listening); // Ferme le socket d'écoute car nous n'en avons plus besoin

    memset(host, 0, NI_MAXHOST); // Efface la variable host
    memset(service, 0, NI_MAXSERV); // Efface la variable service


    // Obtient l'adresse et le numéro de port du client
    // La fonction getnameinfo convertit l'adresse et le numéro de port du client de la forme binaire à une chaîne lisible par l'humain
    // Le premier argument est un pointeur vers une structure sockaddr contenant l'adresse et le numéro de port du client, le deuxième argument est la taille de la structure sockaddr, le troisième argument est un pointeur vers un tampon qui sera rempli avec le nom distant du client, le quatrième argument est la taille du tampon, le cinquième argument est un pointeur vers un tampon qui sera rempli avec le nom du service (port), et le sixième argument est la taille du tampon de service
    // Le septième argument est un ensemble de drapeaux qui contrôlent le comportement de la fonction
    // Si la fonction getnameinfo retourne 0, cela signifie que l'opération a réussi
    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if (result)
    {
        std::cout << host << " connected on " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); // Convertit l'adresse IP de la forme binaire à la forme chaîne
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl; // Convertit le numéro de port du réseau à l'hôte byte par byte
    }

    char buf[4096]; // Tampon pour stocker les données reçues
    while (true)
    {
        memset(buf, 0, 4096); //clear buffer
        int bytesReceived = recv(clientSocket, buf, 4096, 0); // Reçoit des données du client
        // La fonction recv lit les données du socket dans le tampon
        // Le premier argument est le descripteur de fichier du socket, le deuxième argument est un pointeur vers un tampon qui sera rempli avec les données reçues, le troisième argument est la taille du tampon, et le quatrième argument est un ensemble de drapeaux qui contrôlent le comportement de la fonction
        // La fonction recv retourne le nombre d'octets reçus, ou -1 en cas d'erreur
        // Si la fonction recv retourne 0, cela signifie que le client a fermé la connexion
        // Si la fonction recv retourne -1, cela signifie qu'une erreur s'est produite
        // Si la fonction recv retourne un nombre positif, cela signifie que l'opération a réussi et le nombre d'octets reçus est retourné
        if (bytesReceived == -1)
        {
            std::cerr << "Error: Could not receive data" << std::endl;
            break;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        std::cout << "Received: " << std::string(buf, 0, bytesReceived) << std::endl; // Affiche les données reçues
        

        send(clientSocket, buf, bytesReceived + 1, 0); // Envoie les données reçues au client
    }

    close(clientSocket); // Ferme le socket client
    std::cout << "Server shutting down..." << std::endl;

    return 0;
}