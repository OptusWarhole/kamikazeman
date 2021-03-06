#include "client.h"

SDL_Rect get_src_dest (int val){
    SDL_Rect src;
    int x;
    int y;

    switch (val)
    {
        case (0) :
        x = 3;
        y = 0;
        break;
        case (1) :
        x = 1;
        y = 0;
        break;
        case (2) :
        x = 1;
        y = 1;
        break;
        case (3) :
        x = 1;
        y = 2;
        break;
        case (4) :
        x = 1;
        y = 3;
        break;
        case (5) :
        x = 2;
        y = 0;
        break;
        case (6) :
        x = 6;
        y = 5;
        break;
        case (7) :
        x = 1;
        y = 0;
        break;
        case (8):
        x = 2;
        y = 0;
        break;
        default:
        x = 2;
        y = 0;
        break;
    };
    src.x = x * TILESET_SIZE;
    src.y = y * TILESET_SIZE;
    src.w = TILESET_SIZE;
    src.h = TILESET_SIZE;
    
    return src;
}


SDL_Texture* get_texture(int val, t_game *game)
{
    SDL_Texture* Shape = NULL;
    SDL_Surface *tileset;
    printf("val : %i\n", val);
    if (val >= 1 && val <= 4){
        tileset = game->img_texture[11];
    }else{
        tileset = game->img_texture[10];
    }
    if (!tileset)
        fprintf(stdout, "Échec de chargement du sprite (%s)\n", SDL_GetError());
    else
    {
        Shape = SDL_CreateTextureFromSurface(game->render, tileset);
        if ( !Shape )
        {
            fprintf(stdout, "Échec de création de la texture (%s)\n", SDL_GetError());
            return (NULL);
        }
    }
    return (Shape);
}

void init_map(t_game *game)
{
    int i;
    int j;
    int x;
    int y;
    
    int		map[10][10] = {
        {1, 0, 5, 5, 7, 7, 5, 5, 0, 4},
        {0, 7, 5, 5, 5, 5, 5, 5, 7, 0},
        {5, 5, 8, 7, 7, 5, 7, 8, 5, 5},
        {8, 5, 7, 7, 8, 5, 7, 7, 5, 8},
        {7, 5, 5, 5, 5, 5, 7, 5, 5, 7},
        {7, 5, 5, 7, 5, 5, 5, 5, 5, 7},
        {8, 5, 7, 7, 5, 8, 7, 7, 5, 8},
        {5, 5, 8, 7, 5, 7, 7, 8, 5, 5},
        {0, 7, 5, 5, 5, 5, 5, 5, 7, 0},
        {2, 0, 5, 5, 7, 7, 5, 5, 0, 3}};
        
        for(i=0; i < SCREEN_WIDTH; i++)
        {
            y = i * 50;
            for(j=0; j < SCREEN_HEIGHT; j++)
            {
                game->map[i][j] = map[i][j];
                x = j * 50;
                
                switch (game->map[i][j]) {
                    case (0):
                    game->textures[i][j] = get_texture(0, game);
                    break;
                    case (1):
                    init_client(&game->clients[0], x, y);
                    game->textures[i][j] = get_texture(1, game);
                    break;
                    case (2):
                    init_client(&game->clients[1], x, y);
                    game->textures[i][j] = get_texture(2, game);
                    break;
                    case (3):
                    init_client(&game->clients[2], x, y);
                    game->textures[i][j] = get_texture(3, game);
                    break;
                    case (4):
                    init_client(&game->clients[3], x, y);
                    game->textures[i][j] = get_texture(4, game);
                    break;
                    case (8):
                    game->textures[i][j] = get_texture(8, game);
                    break;
                    default:
                    if (game->map[i][j] == 5 || game->map[i][j] == 6 || game->map[i][j] == 7)
                        game->textures[i][j] = get_texture(5, game);
                }
            }
        }
    }


    void show_map(SDL_Renderer* render, t_game *game)
    {
        int y,x;
        SDL_Rect Rect_dest;
        
        SDL_RenderClear(game->render);
        for(y=0; y < SCREEN_WIDTH; y++)
        {
            for(x=0; x < SCREEN_HEIGHT; x++)
            {
                SDL_Rect Src_dest;
                Src_dest = get_src_dest(game->map[y][x]);
                Rect_dest.x = x * 50;
                Rect_dest.y = y * 50;
                Rect_dest.w = 50;
                Rect_dest.h = 50;
                if (game->render)
                    SDL_RenderCopy(game->render, game->textures[y][x], &Src_dest, &Rect_dest);
                else
                    fprintf(stdout, "ERROR (%s)\n", SDL_GetError());
            }
        }
        SDL_RenderPresent(game->render);
    }

    void bomb_side(int pos_bomb, int id, t_game *game)
    {
      int h;
      int y;
      int x;
      int stock;

      y = game->clients[id].yB;
      x = game->clients[id].xB;
      stock = 0;
      if (pos_bomb == 6)
      {
          while (stock < 4)
          {
             for (h = stock; h < 4; h++)
               if (game->clients[h].yB == y) {
                 stock = h + 1;
                 break;
             }
             if (game->clients[h].xB == x) {
               game->t_bomb_id = h;
               stock = 4;
               explode_bomber(game);
           }
       }
   }
}

void after_bomb(int y, int x, int id, t_game *game)
{
  bomb_side(game->map[y][x], id, game);
  game->map[y][x] = 0;
  game->textures[y][x] = get_texture(0, game);
}

void explode_bomber(t_game *game)
{
  int id;
  int i;
  int y;
  int x;
  int bp;

  id = game->t_bomb_id;
  sleep(2);
  printf("---_> %i - %i\n", id, game->id);
  if (id == game->id) {
    game->msg = "EXPLODE\n";
    talkServer(game);
}

bp = game->clients[id].bp;
y = game->clients[id].yB;
x = game->clients[id].xB;
for (i = 1; i <= bp; i++)
{
  if (game->map[y][x] != 7)
  {
      game->map[y][x] = 0;
      game->textures[y][x] = get_texture(0, game);
  }
  if (y - i >= 0)
    if (game->map[y - i][x] != 7)
        after_bomb(y - i, x, id, game);
    if (y + i <= 9)
        if (game->map[y + i][x] != 7)
            after_bomb(y + i, x, id, game);
        if (x - i >= 0)
            if (game->map[y][x - i] != 7)
                after_bomb(y, x - i, id, game);
            if (x + i <= 9)
                if (game->map[y][x + i] != 7)
                    after_bomb(y, x + i, id, game);
                show_map(game->render, game);
            }
        }

