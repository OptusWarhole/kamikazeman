#include "client.h"

int connectServer(t_game *ag)
{
    struct sockaddr_in	server;
    
    if (ag->port == "" || ag->ip == "") {
        return (0);
    }
    ag->sock = socket(AF_INET , SOCK_STREAM , 0);
    
    if (ag->sock == -1){
        return (0);
    }
    
    server.sin_addr.s_addr = inet_addr(ag->ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(my_getnbr(ag->port));
    ag->server = server;
    
    if (connect(ag->sock,(struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("Connection fail");
        return (0);
    }
    init_map(ag);
    return (1);
}

int		listenServer(t_game *game)
{
    int			v;
    int			id;
    char		Server_msg[BUFFER];
    char        *idChar;
    char        *x;
    char        *y;
    char        *msg;
    pthread_t t_bomber = NULL;
    
    while ((v = read(game->sock, Server_msg, BUFFER)) > 0)
    {
        printf("Server MSG %s \n", Server_msg);
        idChar = strtok(Server_msg, " ");
        msg = strtok(NULL, " ");
        id = -1;
        if (strcmp(idChar, "OK\n") == 0)
        {
            msg = game->msg;
            id = game->id;
        }
        
        if (strcmp(idChar, "BP") == 0 || strcmp(idChar, "BB") == 0 || strcmp(idChar, "MP") == 0 || strcmp(idChar, "MB") == 0)
        {
            x = strtok(msg, ";");
            y = strtok(NULL, ";");
            if (y == NULL) {
                game->clients[game->id].bp = my_getnbr(idChar);
            } else {
                // changer les sprite des position X - Y pour y mettre l'image du bonus. le type du bonus c'est la variable idChar
            }
            
            printf("--> msg : %s - id : %s -- x %s -- y : %s", msg , idChar, x ,y);
            
            msg = NULL;
        }
        
        if (msg != NULL) {
            if (id == -1)
                id = my_getnbr(idChar) - 1;
            
            // gestion des commandes sans modification de map
            if ( strcmp(msg, "ID\n") == 0) {
                game->id = id;
                printf("ID game: %i - %i\n", game->id, id);
            }
            else if (strcmp(msg, "DROP\n") == 0)
                game->clients[id].drop = 1;
            else if (strcmp(msg, "DIE\n") == 0) {
                die_client(game, id);
            }
            else if (strcmp(msg, "BP\n") == 0) {
                game->clients[id].bp = my_getnbr(idChar);
            }
            else if (strcmp(msg, "MP\n") == 0) {
                game->clients[id].bp = my_getnbr(idChar);
            }
            else if (strcmp(msg, "EXPLODE\n") != 0)
            {
                move_client(game, msg, id);
                show_map(game->render, game);
                if (game->clients[id].wait == 1)
                {
                    game->clients[id].wait = 0;
                    game->t_bomb_id = id;
                    pthread_join(t_bomber, NULL);
                    pthread_create(&t_bomber, NULL, explode_bomber, game);
                }
            }
        }
        memset(Server_msg, '\0', BUFFER);
    }
    printf("%s\n", "-- listenServer fin.");
    return (1);
}

int	talkServer(t_game *game)
{
    printf("CLient msg : %s\n", game->msg);
    if( write(game->sock, game->msg, strlen(game->msg)) < 0)
    {
        perror("send()");
        exit(errno);
    }
    return (1);
}
