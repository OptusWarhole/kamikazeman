#ifndef Bomberman_client_h
#define Bomberman_client_h

#include "const.h"
//Librairies
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

extern char *NETWORK;

// Const
#define	BUFFER 255

#define TILE_WIDTH 900  // hauteur et largeur du tileset.
#define TILE_HEIGHT 900
#define TILESET_SIZE 100
#define SCREEN_WIDTH 10  // nombre a afficher en x et y
#define SCREEN_HEIGHT 10

/* Struct bombe */
typedef struct  s_bombs
{
    int		id;
    int     x;
    int     y;
    int		timer;
}               t_bombs;

/* Struc client */
typedef struct	s_clients
{
    int		fd;
    int		id;
    int     drop;
    int     wait;
    int		stock_bomb; // 1
    int		power_bomb; // porté de la bombe, 1
    int     xB;
    int     yB;
    int     bp;
    int		x;
    int		y;
}               t_clients;

/* Game's Structure */
typedef struct	s_game
{
    char        *port;
    char        *ip;
    int         sock;
    int         id;
    char        *msg;
    t_clients   clients[4];
    t_bombs     bombs[50];
    int         nbBomb;
    int         t_bomb_id;
    int         map[10][10];
    SDL_Renderer*   render;
    SDL_Renderer*   main_menu;
    SDL_Texture*    textures[10][10];
    //liste des images déja chargé dans une surface
    SDL_Surface*    img_texture[20];
    Mix_Music       *song;
    SDL_Renderer    *menu_ip;
    SDL_Window      *window_ip;
    SDL_Window      *main_window;
    struct sockaddr_in	server;
    
}               t_game;

/* Map's Structure */
typedef struct  s_map
{
    int     x;
    int		y;
    int		min;
    int		max;
    int		new_move;
}               t_map;

typedef struct  s_tiles
{
    int		id;
    int     x;
    int     y;
    int		timer;
}   s_map;

// Client <-> Serveur
int connectServer(t_game *ag);
int	listenServer(t_game *games);
void change_map(char *msg, int sock);
int talkServer(t_game *games);
void cleanMsg(char msg[BUFFER]);

// Client
void init_client(t_clients *client, int x, int y);
void move_client(t_game *game, char* msg, int id);
void drop_client(t_game *game, int id, int x, int y);
void display_client(t_game *game, char* msg, int id, int x, int y);
void die_client(t_game *game, int id);

// Map
void show_map(SDL_Renderer* render, t_game *game);
void init_map(t_game *game);
SDL_Texture* get_texture(int val, t_game *game);
void explode_bomber(t_game *game);
char* get_filepath(int val);
SDL_Rect get_src_dest (int val);

// Menus
SDL_Renderer* Create_menu_render(SDL_Window *window);
void Main_menu(SDL_Window *window, SDL_Window *window_ip, SDL_Renderer *main_menu, SDL_Renderer *menu_ip);
SDL_Renderer* Create_menu_ip_render(SDL_Window *window);
void Menu_ip(SDL_Window *window, SDL_Renderer *menu_ip);
SDL_Renderer* Update_menu_ip_render(SDL_Window *window, SDL_Renderer* menu_ip);

// Get IP input
char* event_enterText();
void menu_ip(SDL_Event event, char* text);
char* convert_scancode(int scancode);
char* check_enterText(SDL_Event event, SDL_Window* window, SDL_Window* window_ip, char *text, SDL_Renderer* renderer_ip);
void game_loop (SDL_Event event, t_game game);
void parse_ip_port(char *text, t_game *game);
char    *my_memset(char *s, int n);

// Setup
int setup_SDL();
TTF_Font* Create_font( char* name, int size);
SDL_Window* CreateMainWindow(char* text, int x, int y);
SDL_Window* CreateWindow(char* text, int x, int y);
SDL_Renderer* CreateRenderer(SDL_Window* window);
SDL_Texture* SurfaceToTexture( SDL_Surface* surf, SDL_Renderer* renderer);
SDL_Rect CreateSolid_Rect(int x, int y, int w, int h);
SDL_Texture* CreateSolidTexture(TTF_Font* font, SDL_Color textColor, char* text, SDL_Renderer* renderer, int * space);
void load_images(t_game *game);


// Update windows
void Update_IP_Windows(SDL_Renderer* renderer, char* text, SDL_Window* window);
void Update_Windows(SDL_Renderer* renderer, SDL_Window* window);

// Lib
int	my_getnbr(char *str);

#endif
