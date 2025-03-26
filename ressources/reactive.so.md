# ft_irc

ft_irc est un projet que vous pouvez réaliser pendant votre cursus, et si vous lisez ceci, j'imagine que c'est le cas. C'est un projet sympa qui consiste à développer un serveur IRC (Internet Relay Chat). Bien que le projet soit gérable, il vous mettra au défi et élargira votre compréhension de la communication bidirectionnelle via des descripteurs de fichiers (alias sockets).

Si vous ne savez pas ce qu'est IRC, voici un TL;DR : c’est un protocole simple et robuste créé pour héberger des communautés. Imaginez-le comme un prédécesseur des plateformes modernes telles que Discord ou Slack, avec une différence clé : IRC est ouvert, ce qui signifie que n’importe qui peut créer un serveur ou développer un client, offrant ainsi aux utilisateurs un contrôle total sur leur environnement de chat, sans dépendre de services tiers.

## Comprendre l'étendue du projet

J'aime diviser ce projet en quatre piliers fondamentaux : le serveur, le parseur de commandes, les salons (channels) et le client (ce n’est pas le client IRC que vous utiliserez pour vous connecter au serveur, mais une classe qui représente une « connexion », que j'appelle client). Chacun de ces composants joue un rôle crucial dans la création d’un serveur IRC fonctionnel qui permet aux clients de se connecter, communiquer et interagir en temps réel. Examinons chacun de ces composants pour comprendre leur rôle et la façon dont ils interagissent pour créer un espace de discussion génial !

Voici un petit schéma de la manière dont j'ai personnellement développé cela, en me concentrant principalement sur la structure orientée objet du projet :

https://reactive.so/static/assets/irc-global-overview.png

## Le serveur

Le serveur est essentiel pour gérer les connexions socket, qui constituent l'épine dorsale de la communication entre le serveur et le client, en plus de protéger les variables de haut niveau telles que le mot de passe du serveur. Le serveur utilise des sockets pour gérer les données entrantes et sortantes, permettant des interactions en temps réel entre les clients et le réseau IRC.

La gestion des sockets débute par la création et l'association d'un socket d'écoute. Ce socket écoute les demandes de connexion entrantes des clients. Une fois la connexion établie, le serveur crée un nouveau socket pour communiquer avec le client. La gestion de ces sockets implique plusieurs étapes : créer le socket, le lier à un port, l’écouter pour les connexions entrantes et accepter ces connexions. Chaque connexion acceptée est alors traitée par un nouveau socket, permettant à plusieurs clients d'interagir avec le serveur de manière simultanée.

### Exemple de gestion des connexions en C++

```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    int port = 6667; // Port IRC par défaut

    // Créer le socket du serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Configuration de la structure d'adresse du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Utiliser IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Lier à toutes les interfaces disponibles
    server_addr.sin_port = htons(port); // Convertir le port en ordre réseau

    // Lier le socket à l'adresse et au port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // Écouter les connexions entrantes
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Serveur à l'écoute sur le port " << port << std::endl;

    // Accepter une connexion
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Client connecté" << std::endl;

    // Lire les données du client
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("read");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    buffer[bytes_read] = '\0'; // Terminer la chaîne avec un caractère nul
    std::cout << "Message reçu : " << buffer << std::endl;

    // Fermer les sockets client et serveur
    close(client_fd);
    close(server_fd);

    return 0;
}
```

Ce programme simple vous permet de copier et d'exécuter le code localement (après l'avoir lu et vous être assuré qu'il ne contient pas de code malveillant bien entendu !). Vous pouvez le tester en l'exécutant en arrière-plan et en lançant, dans un autre terminal, la commande :  
`nc 127.0.0.1 6697`  
Saisissez ce que vous voulez dans l'entrée standard et vous verrez s'afficher dans les logs du serveur votre saisie. Voici une explication rapide du code :

- **Création du socket** : La fonction socket(2) est utilisée pour créer un socket TCP pour le serveur. Elle établit un point de communication pour que le serveur puisse écouter les connexions des clients.
- **Liaison** : La fonction bind(2) associe le socket créé à un port et à une adresse IP spécifiques, telles que définies dans la structure sockaddr_in. Cela lie le socket à une adresse réseau.
- **Écoute** : La fonction listen(2) prépare le socket à accepter des connexions entrantes en spécifiant le nombre maximal de connexions en file d'attente.
- **Acceptation des connexions** : La fonction accept(2) attend la connexion d'un client. Lorsqu'une connexion est établie, elle crée un nouveau socket (appelé client_fd) pour gérer la communication avec le client connecté.
- **Lecture des données** : La fonction read(2) permet de lire les données envoyées par le client à travers son socket. Ces données peuvent ensuite être traitées par le serveur.

Cette configuration de base montre comment gérer les connexions clients et lire les données de manière directe. Vous devrez adapter ce code légèrement pour tenir compte de la complexité de votre serveur — ce serait trop facile si je vous le donnais exactement tel quel, n'est-ce pas ?

## Parser l'entrée

Après avoir créé un socket et géré les connexions actives, vous devez avoir un moyen de parser les commandes entrantes et de les rediriger vers le service approprié pour traitement. En général, une simple série de conditions (if-else) peut suffire.

Cependant, cette partie est cruciale car elle peut nécessiter la mise en place d'un mur d'authentification global, la plupart des commandes étant inaccessibles aux utilisateurs non enregistrés. Une configuration de base requise pour le sujet comporte un état d'authentification en 3 niveaux, allant de NON AUTHENTIFIÉ à AUTHENTIFIÉ, puis à ENREGISTRÉ.

https://reactive.so/static/assets/irc-command-flow.png

## Les salons (Channels)

Les salons sont le cœur de la communication de groupe sur votre serveur IRC — ce sont les espaces où les clients se retrouvent pour discuter et interagir en temps réel. Pensez aux salons comme des espaces virtuels, chacun avec ses propres règles, membres et ambiance.

Chaque salon est identifié par un nom (précédé d’un #, comme #general) et peut posséder divers attributs tels qu’un sujet, une liste d’opérateurs, une limite d’utilisateurs et même un mot de passe pour un accès restreint. Ainsi, lorsque un utilisateur rejoint un salon, il doit respecter les règles et permissions spécifiques à ce salon.

Le salon est responsable du maintien d’une liste de membres, du suivi des utilisateurs présents à tout moment et de l’application des règles. Cela inclut la gestion des privilèges d’opérateur, qui permettent à certains utilisateurs de retirer ou bannir d'autres membres, de définir le sujet, et de modifier les paramètres du salon.

Les salons contrôlent également le flux des messages. Lorsqu’un utilisateur envoie un message à un salon, celui-ci se doit de le diffuser à tous les membres présents. Cette synchronisation garantit que tout le monde est sur la même longueur d’onde, maintenant ainsi le rythme naturel de la conversation.

Un autre aspect clé est le contrôle d'accès aux salons. Ceux-ci peuvent être ouverts à tous, sur invitation seulement, ou protégés par un mot de passe. Ces réglages s'appliquent de différentes manières : certains lors de l'initialisation, d'autres via la commande MODE. À vous de jouer pour y voir plus clair !

## Le client

Dans le contexte du serveur, un « Client » n'est pas seulement une personne utilisant un client IRC comme HexChat ou irssi — il s’agit en réalité d’une représentation d’une connexion au serveur, encapsulée dans une classe qui gère l’état de chaque utilisateur interagissant sur le réseau IRC.

La classe client gère toutes les données essentielles liées à chaque utilisateur : leur pseudonyme, nom d'utilisateur, nom d'hôte, nom réel, descripteur de fichier et, surtout, leur état d'authentification courant. Au fur et à mesure que les utilisateurs passent par le cycle de vie de la connexion — de la connexion initiale, à la configuration de leur pseudonyme et nom d'utilisateur, puis à leur enregistrement sur le serveur —, la classe client permet de suivre et d'appliquer ces états pour assurer le bon fonctionnement du serveur.

Cette classe est également en charge de gérer les salons dans lesquels l'utilisateur se trouve, de traiter les messages entrants et sortants, et de maintenir un suivi précis de l'activité de l'utilisateur sur le serveur. Par exemple, lorsqu'un utilisateur envoie un message, la classe client doit s'assurer que le message est correctement formaté avant d'être transmis au(x) destinataire(s), qu'il s'agisse d'un autre utilisateur ou d'un salon entier.

Le client ne se contente pas de stocker des données — c’est un acteur actif dans la fonctionnalité du serveur, veillant à ce que toute communication se déroule comme prévu et que l'expérience utilisateur soit fluide et agréable.

## Répondre aux messages

À présent que vous comprenez la structure de base du projet, voici quelques conseils pour répondre à certains messages, car ce n’est pas toujours évident…

Je vous recommande de vous renseigner sur la spécification IRCv3, plutôt que sur les spécifications RFC1459, RFC2812 ou RFC7194, qui sont relativement obsolètes. Ce n’est pas que les anciennes RFC ne fonctionnent pas, mais simplement parce que vous pourriez perdre du temps à décoder des messages de clients qui suivent le nouveau protocole.

Ensuite, je vous suggère de vous connecter à Libera Chat — le serveur IRC public le plus populaire — en utilisant netcat. En faisant cela, vous pouvez reproduire leur comportement sans avoir à vous reposer constamment sur les RFC pour expliquer les commandes (car, encore une fois, ce n’est pas toujours clair).

## Quel client utiliser ?

Il existe de nombreux clients parmi lesquels choisir, mais de loin celui qui nous a le plus convaincu est Halloy. Il est multiplateforme, fiable, simple et facile à configurer. Le seul bémol est que vous avez besoin de Cargo pour l'exécuter à 42, ce qui peut ne pas toujours être disponible. Une autre option est irssi, mais là encore, il nécessite quelques dépendances pour fonctionner correctement.

## Erreurs courantes

- Assurez-vous de bien diffuser chaque événement que les autres clients doivent voir (lorsqu’un utilisateur rejoint/quitte un salon, est expulsé, ou lorsque le mode change).
  
- Exemple d'utilisation de JOIN avec multiples salons et mots de passe :
  ```
  JOIN #chan1,#chan2,#chan3 pass1,pass2
  ```
  
- Que se passe-t-il quand le nom d'utilisateur, le pseudonyme et le nom réel sont différents ?

- Testez correctement le mode +k !

- Pourquoi ai-je un @ en préfixe de mon pseudonyme ?


Sources - https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/