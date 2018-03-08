#include "client.h"

char    *my_memset(char *s, int n)
{
    int   i;
    
    for (i = 0; i < n; ++i)
        s[i] = '\0';
    return (s);
}

// Enter IP and port
void game_loop (SDL_Event event, t_game game){

    pthread_t th;
    
    game.sock = -1;
    
    load_images(&game);
    

    
    //SDL_Event event;
    SDL_bool done;
    
    done = SDL_TRUE;
    Update_menu_ip_render(game.window_ip, game.menu_ip);
    
    while (done) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    /* Quit */
                    done = SDL_FALSE;
                    break;
                case SDL_KEYDOWN :
                    printf("%u", SDL_GetScancodeFromKey(event.key.keysym.sym));
                    switch (SDL_GetScancodeFromKey(event.key.keysym.sym)) {
                        case 41:
                            /* Quit */
                            done = SDL_FALSE;
                            SDL_Quit();
                            break;
                        case 230 :
                            done = SDL_FALSE;
                            free(NETWORK);
                            NETWORK = malloc(500 * sizeof(char));
                            break;
                        case (55):
                            if (strlen(NETWORK) == 0)
                                strcpy(NETWORK, convert_scancode(event.key.keysym.scancode));
                            else
                                strcat(NETWORK, convert_scancode(event.key.keysym.scancode));
                            break;
                        case (44):
                            if (game.sock < 0) {
                                if (strlen(NETWORK) == 0)
                                    strcpy(NETWORK, convert_scancode(event.key.keysym.scancode));
                                else
                                    strcat(NETWORK, convert_scancode(event.key.keysym.scancode));
                                Update_menu_ip_render(game.window_ip, game.menu_ip);
                            } else {
                                game.msg = "DROP\n";
                                talkServer(&game);
                            }
                            break;
                        case (42):
                            NETWORK[strlen(NETWORK) - 1] = '\0';
                            Update_menu_ip_render(game.window_ip, game.menu_ip);
                            break;
                        case (26):
                            printf(" game.sock = %d \n", game.sock);
                            if (game.sock > 0) {
                                game.msg = "UP\n";
                                talkServer(&game);
                            }
                            break;
                        case (7):
                            printf(" game.sock = %d \n", game.sock);
                            if (game.sock != 0) {
                                game.msg = "RIGHT\n";
                                talkServer(&game);
                            }
                            break;
                        case (4):
                            if (game.sock != 0) {
                                game.msg = "LEFT\n";
                                talkServer(&game);
                            }
                            break;
                        case (22):
                            if (game.sock != 0) {
                                game.msg = "DOWN\n";
                                talkServer(&game);
                            }
                            break;

                        case (40):

                            parse_ip_port(NETWORK, &game);
                            
                            if (game.port && game.ip) {
                                printf("ip %s - port %s\n", game.ip, game.port);
                                if (connectServer(&game) > 0) {
                                    show_map(game.menu_ip, &game);
                                    pthread_create(&th, NULL, listenServer, &game);
                                    
                                }
                                else {
                                    free(NETWORK);
                                    NETWORK = malloc(500 * sizeof(char));
                                }
                            }
                            break;
                        default:
                            if ((event.key.keysym.scancode >= 30 ) && (event.key.keysym.scancode <= 39)){
                                if (strlen(NETWORK) == 0){
                                    strcpy(NETWORK,convert_scancode(event.key.keysym.scancode));
                                }
                                else{
                                    strcat(NETWORK, convert_scancode(event.key.keysym.scancode));
                                }
                                Update_menu_ip_render(game.window_ip, game.menu_ip);
                            }
                            break;
                    }
                    break;
            }
        }
    }
}

void parse_ip_port(char *text, t_game *game){
    char *item;
    char port[200];
    int n;
    char ip[200];
    game->ip= malloc(sizeof(char)* 120);
    game->port= malloc(sizeof(char)* 120);
    /* get the first item */
    
    item = strtok(text, " ");
    printf("%s\n", game->ip);
    printf("%s\n", game->port);
    my_memset(ip, 200);
    my_memset(port, 200);
    
    for( n = 0; item != NULL; n++)
    {
        if (n < 4)
        {
            strcat(ip, item);
            strcat(ip, ".");
        } else {
            strcat(port, item);
        }
        item = strtok(NULL, " ");
    }

    ip[strlen(ip) - 1] = '\0';

    strcpy(game->ip, ip);
    strcpy(game->port, port);
    
    printf("IP : %s \n", game->ip);
    printf("Port : %s \n", game->port);
};

char* convert_scancode(int scancode){
    switch (scancode) {
        case 30:
            return "1";
            break;
        case 31:
            return "2";
            break;
        case 32:
            return "3";
            break;
        case 33:
            return "4";
            break;
        case 34:
            return "5";
            break;
        case 35:
            return "6";
            break;
        case 36:
            return "7";
            break;
        case 37:
            return "8";
            break;
        case 38:
            return "9";
            break;
        case 39:
            return "0";
            break;
        case 44:
            return " ";
            break;
        default:
            return "";
            break;
    }
};
