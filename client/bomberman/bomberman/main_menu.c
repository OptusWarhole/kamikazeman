#include "client.h"

SDL_Renderer* Create_menu_render(SDL_Window * window){
    
    TTF_Init();
    SDL_Renderer *render;
    int font_size = 24;
    char * text = "Press ENTER to start";
    SDL_Color normalColor = {255, 255, 255};
    SDL_Rect Rect_dest;
    SDL_Surface* surface_background;
    SDL_Surface* surface_text;
    SDL_Texture* texture;
    SDL_Texture* texture_text;
    
    Rect_dest.x = 0;
    Rect_dest.y = 0;
    Rect_dest.w = 500;
    Rect_dest.h = 500;
    
    TTF_Font* police = TTF_OpenFont(FONT_MENU_PATH, font_size);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_RenderClear(render);
    //Ecriture des surfaces surfaces
    surface_background = SDL_LoadBMP(BACKGROUND_MENU_PATH);
    if ( !surface_background )
        fprintf(stdout,"Échec de chargement du background (%s)\n",SDL_GetError());
    
    surface_text = TTF_RenderText_Blended(police, text, normalColor);
    if ( !surface_text )
        fprintf(stdout,"Échec de chargement du texte (%s)\n",SDL_GetError());
    
    //Libère la mémoire utilisé par la police
    TTF_CloseFont(police);
    
    texture = SDL_CreateTextureFromSurface(render, surface_background);
    if ( !texture )
        fprintf(stdout,"Échec de création de la texture1 - %s\n",SDL_GetError());
    
    SDL_FreeSurface(surface_background);
    
    
    // on copy sur le render le background
    SDL_RenderCopy(render, texture, NULL, &Rect_dest);
    
    Rect_dest.x = 100;
    Rect_dest.y = 30;
    Rect_dest.w = 300;
    Rect_dest.h = 50;
    
    // on copy sur le render le background
    texture_text = SDL_CreateTextureFromSurface(render, surface_text);
    if ( !texture_text )
        fprintf(stdout,"Échec de création de la texture2 - %s\n",SDL_GetError());
    
    SDL_RenderCopy(render, texture_text, NULL, &Rect_dest);
    SDL_FreeSurface(surface_text);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_text);
    TTF_Quit();
    return render;
}

void Main_menu(SDL_Window *window, SDL_Window *window_ip, SDL_Renderer *main_menu, SDL_Renderer *menu_ip){

    SDL_Event event;
    SDL_bool done = SDL_TRUE;
    //SDL_RenderClear(menu_ip);
    SDL_RenderPresent(main_menu);
    SDL_HideWindow(window_ip);
    int menu_on;
    t_game game;
    
    //game
    /*pthread_t t_listenServer;
    pthread_t t_askServer;
    pthread_t th;*/
    
    NETWORK = malloc(500 * sizeof(char));
    
    game.window_ip = window_ip;
    game.main_window = window;
    game.menu_ip = menu_ip;
    game.sock = -1;
    game.main_menu = main_menu;
    game.render = menu_ip;
    
    //defini le menu afficher. 0:main;1:menu_ip
    menu_on = 0;
    
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
                    
                            /* ENTER preessed, start le menu IP */
                    case 40:
                            //on crée le menu_ip
                    if (menu_on != 1){
                        printf("lancement du menuIP\n");

                        SDL_HideWindow(window);
                        SDL_ShowWindow(window_ip);
                                //lancement du create enter ip
                        menu_on = 1;
                                //check_enterText(event, window, window_ip, NETWORK, menu_ip);
                        game_loop(event, game);
                    }
                    break;
                    case 230:
                            //press Alt
                    if(menu_on != 0){
                        printf("retour au menu principal\n");
                        SDL_HideWindow(window_ip);
                        SDL_ShowWindow(window);
                        menu_on = 0;
                    }
                    break;
                    case 41:
                            /* Quit */
                    done = SDL_FALSE;
                    SDL_Quit();
                    break;
                }
                break;
            }
        }
    }
    free(NETWORK);
}

#include "client.h"

SDL_Renderer* Create_menu_ip_render(SDL_Window *window){
    TTF_Init();
    SDL_Renderer* menu_ip;
    int font_size = 24;
    char *pressAlt;
    SDL_Color normalColor = {255, 255, 255};
    SDL_Rect Rect_dest;
    SDL_Surface* surface_background;
    SDL_Surface* surface_pressAlt;
    SDL_Texture* main_texture;
    SDL_Texture* texture_pressAlt;
    
    Rect_dest.x = 0;
    Rect_dest.y = 0;
    Rect_dest.w = 500;
    Rect_dest.h = 500;
    
    pressAlt = malloc(500 * sizeof(char));
    
    pressAlt = "Press Alt for Main Menu";
    TTF_Font* police = TTF_OpenFont(FONT_MENU_PATH, font_size);
    menu_ip = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    //Ecriture des surfaces
    surface_background = SDL_LoadBMP(BACKGROUND_MENU_IP_PATH);
    if ( !surface_background )
        fprintf(stdout,"Échec de chargement du background (%s)\n",SDL_GetError());
    
    surface_pressAlt = TTF_RenderText_Blended(police, pressAlt, normalColor);
    if ( !surface_pressAlt )
        fprintf(stdout,"Échec de chargement du texte (%s)\n",SDL_GetError());
    
    TTF_CloseFont(police);
    
    main_texture = SDL_CreateTextureFromSurface(menu_ip, surface_background);
    SDL_FreeSurface(surface_background);
    SDL_RenderCopy(menu_ip, main_texture, NULL, &Rect_dest);
    
    Rect_dest.x = 100;
    Rect_dest.y = 30;
    Rect_dest.w = 300;
    Rect_dest.h = 50;
    texture_pressAlt = SDL_CreateTextureFromSurface(menu_ip, surface_pressAlt);
    SDL_FreeSurface(surface_pressAlt);
    SDL_RenderCopy(menu_ip, texture_pressAlt, NULL, &Rect_dest);
    SDL_RenderPresent(menu_ip);
    TTF_Quit();
    return menu_ip;
    
}

SDL_Renderer* Update_menu_ip_render(SDL_Window *window, SDL_Renderer* menu_ip){
    SDL_RenderClear(menu_ip);
    TTF_Init();
    //SDL_Renderer* menu_ip;
    int font_size = 24;
    char *pressAlt;
    SDL_Color normalColor = {255, 255, 255};
    SDL_Rect Rect_dest;
    SDL_Surface* surface_background;
    SDL_Surface* surface_acceuil;
    SDL_Surface* surface_pressAlt;
    SDL_Texture* main_texture;
    SDL_Texture* texture_acceuil;
    SDL_Texture* texture_pressAlt;
    
    Rect_dest.x = 0;
    Rect_dest.y = 0;
    Rect_dest.w = 500;
    Rect_dest.h = 500;
    
    pressAlt = malloc(500 * sizeof(char));
    
    pressAlt = "Press Alt for Main Menu";
    TTF_Font* police = TTF_OpenFont(FONT_MENU_PATH, font_size);
    //menu_ip = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    //Ecriture des surfaces
    surface_background = SDL_LoadBMP(BACKGROUND_MENU_IP_PATH);
    if ( !surface_background )
        fprintf(stdout,"Échec de chargement du background (%s)\n",SDL_GetError());
    
    surface_acceuil = TTF_RenderText_Blended(police, NETWORK, normalColor);
    if ( !surface_acceuil )
        fprintf(stdout,"Échec de chargement du texte (%s)\n",SDL_GetError());
    
    surface_pressAlt = TTF_RenderText_Blended(police, pressAlt, normalColor);
    if ( !surface_pressAlt )
        fprintf(stdout,"Échec de chargement du texte (%s)\n",SDL_GetError());
    
    TTF_CloseFont(police);
    
    main_texture = SDL_CreateTextureFromSurface(menu_ip, surface_background);
    SDL_FreeSurface(surface_background);
    SDL_RenderCopy(menu_ip, main_texture, NULL, &Rect_dest);
    
    //free(pressAlt);
    //free(acceuil);
    
    Rect_dest.x = 50;
    Rect_dest.y = 150;
    Rect_dest.w = 400;
    Rect_dest.h = 50;
    texture_acceuil = SDL_CreateTextureFromSurface(menu_ip, surface_acceuil);
    SDL_FreeSurface(surface_acceuil);
    SDL_RenderCopy(menu_ip, texture_acceuil, NULL, &Rect_dest);
    
    Rect_dest.x = 100;
    Rect_dest.y = 30;
    Rect_dest.w = 300;
    Rect_dest.h = 50;
    texture_pressAlt = SDL_CreateTextureFromSurface(menu_ip, surface_pressAlt);
    SDL_FreeSurface(surface_pressAlt);
    SDL_RenderCopy(menu_ip, texture_pressAlt, NULL, &Rect_dest);
    SDL_RenderPresent(menu_ip);
    TTF_Quit();
    return menu_ip;
    
    
    
}


