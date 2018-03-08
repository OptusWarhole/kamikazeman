#ifndef __SERVER_H__
#define __SERVER_H__

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// Client
typedef struct	s_clients
{
  int		fd;
  int		id;
  int   nb_bomb;
  int		fbomb;
  int		stock_bomb;
  int		power_bomb;
  int		done;
}		t_clients;

// Map
typedef struct  s_map
{
  int   x;
  int   y;
  int   min;
  int   max;
  int   new_move;
}               t_map;

// Bomb
typedef struct  s_bombs
{
  int		id;
  int   x;
  int   y;
  int		to_explode;
}               t_bombs;

// function init socket
int     load_server(int fd_socket, t_clients *clients);
int     init_serv(int fd_socket, int port);
int     client_process(int fd_socket, t_clients **clients);
int     load_client(int fd_socket);
void    check_socket(int fd_socket, fd_set *readfds);
void    init_fd_set(int fd_socket, fd_set *readfds, t_clients *clients);

// function management
int    start(int fd_socket, t_clients *clients, int map[10][10], t_bombs *bombs);
int    commands(char *buff, t_clients *clients, int map[10][10], t_bombs *bombs);
int    my_select(fd_set *readfds, int n);
int    manage(fd_set *readfds, t_clients *clients, int map[10][10], t_bombs *bombs, int fd_socket);
int    my_close_sock(int client);

// function from lib
void   my_putstr(char *str, int fd);
void   my_putint(int nb, int fd);
char   *my_memset(char *s, int n);
char	 *my_strcat(char *str1, char *str2);
int    my_strcmp(char *str1, char *str2);

// function map
int    init_map(int fd_socket, t_clients *clients);
int    move_to(t_map coord, t_clients *clients, char *buff, int map[10][10], int trig);
int	   send_info(t_clients *clients, char *buff, int flag);
void   display_map(int map[10][10]);
void   trigger(int *pos, int *box, t_map coord, int map[10][10], int trig);
void   init_coord(int client, char *buff, t_map *coord, int map[10][10]);
void   *find_player(t_map *coord, int player, int map[10][10]);

//function bomb
int    drop_bomb(t_clients *clients, char *buff, t_map *coord, t_bombs **bombs);
int    verif_fbomb(t_clients *clients);
int    explode_bomb(t_clients *clients, t_bombs *bombs, int map[10][10]);
void   manage_bomb(t_clients *clients);
void   init_bomb(t_clients *clients);
void   put_bomb_map(t_clients *clients, int map[10][10], t_bombs **bombs);

//function bonus/malus
int	   my_random(t_clients *clients, int x, int y);
int	   bonus_malus(t_clients *clients, int bomb, int id);
void   map_bm(t_clients *clients, t_bombs *bombs, int cible, int map[10][10]);
void	 map_pw(t_bombs *bombs, int map[10][10], int cible, int pw);

// function player
int    clients_signal(int client, int id, int len, char *buff);
int    check_players(t_clients *clients, int map[10][10]);
int    send_bomb(t_clients *clients, int id, int bomb, int pm);
int    player_dies(t_clients *clients, int player_id, int map[10][10]);
int	   new_pos(t_map coord, t_clients *clients, char *buff, int map[10][10], int trig);
int	   up_flag(int map[10][10], int id);
int	   verif_neg(t_clients *clients, int id, int p);
int	   player_state(t_clients *clients, int player_id);

#endif
