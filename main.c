#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "main1.h"
#define WIDTH 1024
#define HEIGHT 512
#define SIZEh 42
#define SIZEw 42
#define DASH_SPEED 1300
#define DASH_DURATION 100
#define DASH_COOLDOWN 500
#define SPEED 400
#define GRAVITY 40
#define FPS 60
#define JUMP -750
#define MAX_VELOCITY 700
void showTitleScreen(SDL_Renderer* rend,TTF_Font* f,SDL_Texture* tx);
int handlecollisions(float *x_pos,float *y_pos,float *x_vel,float *y_vel,bool *can_jump,bool jump_pressed,bool *can_dash,SDL_Rect obj,int id);
int showendScreen(SDL_Renderer* rend, TTF_Font* f,SDL_Texture* tx);
int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 0;
  }
  SDL_Window* wind = SDL_CreateWindow("Bessbes",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WIDTH, HEIGHT, 0);
  if (!wind)
  {
    printf("Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);
  if (!rend)
  {
    printf("Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
  }
  if (TTF_Init()== -1){
    printf("Couldnt initialize ttf");
  }
    TTF_Font* font = TTF_OpenFont("Born2bSportyFS.otf",32);
  	SDL_Surface* bmp = SDL_LoadBMP("gfx.bmp");
  	SDL_Surface* bmp1 = SDL_LoadBMP("player.bmp");
  	SDL_Surface* bmp2 = SDL_LoadBMP("titlescreen.bmp");
  	SDL_Surface* bmp3 = SDL_LoadBMP("endscreen.bmp");
	if (!bmp || !bmp1 || !font) {
		return 0;
	}
  	SDL_Texture* tile_text = SDL_CreateTextureFromSurface(rend,bmp);
  	SDL_Texture* tile_text1 = SDL_CreateTextureFromSurface(rend,bmp1);
  	SDL_Texture* tile_text2 = SDL_CreateTextureFromSurface(rend,bmp2);
  	SDL_Texture* tile_text3 = SDL_CreateTextureFromSurface(rend,bmp3);
    SDL_FreeSurface(bmp);
    SDL_FreeSurface(bmp1);
    SDL_FreeSurface(bmp2);
    SDL_FreeSurface(bmp3);

    SDL_Rect tile[32][16];
    for (int x=0;x<32;x++)
    {
        for (int y=0;y<16;y++)
        {
            tile[x][y].x=x*32;
            tile[x][y].y=y*32;
            tile[x][y].w= 32;
            tile[x][y].h=32;
        }
    }
   showTitleScreen(rend,font,tile_text2);
  bool running = true, jump_pressed = false, can_jump = true,
                  left_pressed = false, right_pressed = false,dash_pressed = false, can_dash = true,
                  dashing=false , up_pressed=false,down_pressed=false;
  int room=0,room1=0,y=0,x=0;
  float x_pos = spawn[room][0], y_pos = spawn[room][1], x_vel = 0, y_vel = 0;
  SDL_Rect rect = {(int) x_pos, (int) y_pos, SIZEw, SIZEh};
  SDL_Event event;
  int v=2;
  Uint32 dash_start_time = 0;
  while (running)
  {
if (v == 1 || v == 0) {
    if (v == 1) {room++;}
    if (room>=9)
    {
        room=showendScreen(rend,font,tile_text3);
    }
    x_pos = spawn[room][0];
    y_pos = spawn[room][1];
    x_vel = 0;
    y_vel = 0;
    can_jump = false;
    can_dash = false;
    rect.x = (int) x_pos;
    rect.y = (int) y_pos;
    v = 2;
    SDL_FlushEvent(SDL_KEYDOWN);
}
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_UP:
                        case SDL_SCANCODE_W:
                            up_pressed = true;
                            break;
                        case SDL_SCANCODE_DOWN:
                        case SDL_SCANCODE_S:
                            down_pressed = true;
                            break;
                        case SDL_SCANCODE_LEFT:
                        case SDL_SCANCODE_A:
                            left_pressed = true;
                            break;
                        case SDL_SCANCODE_RIGHT:
                        case SDL_SCANCODE_D:
                            right_pressed = true;
                            break;
                        case SDL_SCANCODE_R:
                            v = 0;
                            break;
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = true;
                            break;
                        case SDL_SCANCODE_LSHIFT:
                            if (can_dash) {
                                dash_pressed = true;
                                can_dash = false;
                                dashing = true;
                                dash_start_time = SDL_GetTicks();
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_UP:
                        case SDL_SCANCODE_W:
                            up_pressed = false;
                            break;
                        case SDL_SCANCODE_DOWN:
                        case SDL_SCANCODE_S:
                            down_pressed = false;
                            break;
                        case SDL_SCANCODE_LEFT:
                        case SDL_SCANCODE_A:
                            left_pressed = false;
                            break;
                        case SDL_SCANCODE_RIGHT:
                        case SDL_SCANCODE_D:
                            right_pressed = false;
                            break;
                        case SDL_SCANCODE_SPACE:
                            jump_pressed = false;
                            break;
                        case SDL_SCANCODE_LSHIFT:
                            dash_pressed = false;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (dashing) {
            Uint32 current_time = SDL_GetTicks();
            if (current_time - dash_start_time > DASH_DURATION) {
                dashing = false;
                y_vel = -250*up_pressed+250*down_pressed;
                x_vel = -250*right_pressed+250*left_pressed;
            }
        }
        if (dashing) {
            float dash_x = (right_pressed - left_pressed);
            float dash_y = (down_pressed - up_pressed);
            float magnitude = sqrt(dash_x * dash_x + dash_y * dash_y);
            if (magnitude > 0) {
                dash_x /= magnitude;
                dash_y /= magnitude;
            }

            x_vel = dash_x * DASH_SPEED;
            y_vel = dash_y * DASH_SPEED;

        } else {
            x_vel = (right_pressed - left_pressed) * SPEED;
            if (y_vel < MAX_VELOCITY) {
                y_vel += GRAVITY;
            }
        }
    if (jump_pressed && can_jump)
    {
      if (v==-1)
      {
      x_vel-=400;}
      if(v==-2)
      {
      x_vel+=400;
      }
      y_vel = JUMP;
      can_jump = false;
    }
    for(int i = 0 ; i<3;i++)
    {
        x_pos += x_vel / 200;
        y_pos += y_vel / 200;
    for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 16; y++) {
            if (SDL_HasIntersection(&rect,&tile[x][y])&&tilemap[x+room*32][y]!=14)
            {
                v=handlecollisions(&x_pos,&y_pos,&x_vel,&y_vel,&can_jump,jump_pressed,&can_dash,tile[x][y],tilemap[x+room*32][y]);
            }
                switch (tilemap[x+room*32][y]) {
            case 1: SDL_RenderCopy(rend,tile_text, &select_tile1,&tile[x][y]); break;
            case 2: SDL_RenderCopy(rend,tile_text, &select_tile2,&tile[x][y]); break;
            case 3: SDL_RenderCopy(rend,tile_text, &select_tile3,&tile[x][y]); break;
            case 4: SDL_RenderCopy(rend,tile_text, &select_tile4,&tile[x][y]); break;
            case 5: SDL_RenderCopy(rend,tile_text, &select_tile5,&tile[x][y]); break;
            case 6: SDL_RenderCopy(rend,tile_text, &select_tile6,&tile[x][y]); break;
            case 7: SDL_RenderCopy(rend,tile_text, &select_tile7,&tile[x][y]); break;
            case 8: SDL_RenderCopy(rend,tile_text, &select_tile8,&tile[x][y]); break;
            case 9: SDL_RenderCopy(rend,tile_text, &select_tile9,&tile[x][y]); break;
            case 10: SDL_RenderCopy(rend,tile_text, &select_tile10,&tile[x][y]); break;
            case 11: SDL_RenderCopy(rend,tile_text, &select_tile11,&tile[x][y]); break;
            case 12: SDL_RenderCopy(rend,tile_text, &select_tile12,&tile[x][y]); break;
            case 13: SDL_RenderCopy(rend,tile_text, &select_tile13,&tile[x][y]); break;
            case 14: SDL_RenderCopy(rend,tile_text, &select_tile14,&tile[x][y]); break;
            case 17: SDL_RenderCopy(rend,tile_text, &select_tile17,&tile[x][y]); break;
            default: SDL_RenderCopy(rend,tile_text, &select_tile14,&tile[x][y]); break;
        }
        if (v==3)
        {
            SDL_RenderCopy(rend,tile_text, &select_tile14,&tile[x][y]);
        }
        rect.x = (int) x_pos;
        rect.y = (int) y_pos;
    }
    }
}

if (x_vel>0)
    {
        y=0;
    }
    else
    {
        if (x_vel<0)
        {y=32;}
    }
    if (y_vel>=0)
    {
        x=0;
    }
    else
        {
            x=32;
        }
    if (!can_dash)
   {x=x+64;}
    playertile.x=x;
    playertile.y=y;
    playertile.h=32;
    playertile.w=32;
    SDL_RenderCopy(rend,tile_text1, &playertile,&rect);
    SDL_RenderPresent(rend);
    SDL_Delay(1000/FPS);
        if(v==0 || room1!=room){
                room1=room;
    for (int i = 0 ; i<40 ; i++)
    {
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_Rect textRect2 = {WIDTH / 2 - 129, HEIGHT / 2 - 45,250, 100};
    SDL_RenderFillRect(rend, &textRect2);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_Rect textRect = {WIDTH / 2 - 100, HEIGHT / 2 - 37, 200, 75};
    SDL_RenderFillRect(rend, &textRect);
    SDL_Color color ={255,255,255};
    char string[10];
    strcpy(string, "Room ");
    char number[10];
    sprintf(number, "%d", room);
    strcat(string, number);
    SDL_Surface* surf = TTF_RenderText_Solid(font,string, color);
    SDL_Texture* ft = SDL_CreateTextureFromSurface(rend, surf);
    SDL_FreeSurface(surf);
    SDL_RenderCopy(rend,ft,NULL,&textRect);
    SDL_RenderPresent(rend);
    }}
  }
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);
  SDL_Quit();
  return 0;
}

int handlecollisions(float *x_pos,float *y_pos,float *x_vel,float *y_vel,bool *can_jump,bool jump_pressed,bool *can_dash,SDL_Rect obj,int id){
     if ((id==10)||id==11 ||id==12 ||id==13 || id==16)
     {
         {return(0);}
     }
     else{
            if (id==15)
     {
         return(1);
     }
    else
    {if (id == 17)
    {
        *can_dash=true;
    }
    else
    {

        if (id!=14){
    if (*y_pos>obj.y-obj.h-10 && *y_pos<obj.y-obj.h+15)
    {
        *y_pos=obj.y-obj.h-10;
        *y_vel=0;
        *can_dash=true;
        *can_jump=true;
    }
    else{
        if (*x_pos<obj.x+obj.w && *x_pos>obj.x+obj.w-15)
    {
        *x_pos=obj.x+obj.w;
        *y_vel=35;
        *can_jump=true;
        if (jump_pressed)
        {
            return(-2);
        }

    }
    else{if (*x_pos>obj.x-obj.w-10 && *x_pos<obj.x-obj.w+15)
    {
        *x_pos=obj.x-obj.w-10;
        *y_vel=35;
        *can_jump=true;
        if (jump_pressed)
        {
            return(-1);
        }
    }
    else{if (*y_pos<obj.y+obj.h && *y_pos>obj.y+obj.h-15)
        {
            *y_pos=obj.y+obj.h;
            *y_vel=0;
        }
        }}}
    return(2);
    }
    }}}
 }
 void showTitleScreen(SDL_Renderer* rend, TTF_Font* f,SDL_Texture* tx) {
        if (tx) {
            SDL_Rect titleRect = {0, 0, WIDTH, HEIGHT};
            SDL_RenderCopy(rend, tx, NULL, &titleRect);
        }
    SDL_RenderPresent(rend);
    SDL_Event event;
    bool waiting = true;
    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                exit(0);
            }
            if (event.type == SDL_KEYDOWN) {
                    for(int i = 0 ; i<3;i++){
                SDL_SetRenderDrawColor(rend, 221, 205, 169, 255);
                SDL_RenderClear(rend);
                SDL_RenderPresent(rend);
                SDL_Delay(FPS*0.5);
                SDL_SetRenderDrawColor(rend, 0, 51, 115, 255);
                SDL_RenderClear(rend);
                SDL_RenderPresent(rend);
                SDL_Delay(FPS*1.5);
                waiting = false;
            }}
        }
    }
}

int showendScreen(SDL_Renderer* rend, TTF_Font* f,SDL_Texture* tx) {
        if (tx) {
            SDL_Rect titleRect = {0, 0, WIDTH, HEIGHT};
            SDL_RenderCopy(rend, tx, NULL, &titleRect);
        }
    SDL_RenderPresent(rend);
    SDL_Event event;
    bool waiting = true;
    while (waiting) {
       while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    {SDL_Quit();
                    exit(0);}
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_LSHIFT:
                    for(int i = 0 ; i<3;i++){
                SDL_SetRenderDrawColor(rend, 221, 205, 169, 255);
                SDL_RenderClear(rend);
                SDL_RenderPresent(rend);
                SDL_Delay(FPS*0.5);
                SDL_SetRenderDrawColor(rend, 0, 51, 115, 255);
                SDL_RenderClear(rend);
                SDL_RenderPresent(rend);
                SDL_Delay(FPS*1.5);
                waiting = false;
                    }
                    return(0);
                break;
                        case SDL_SCANCODE_ESCAPE:
                            SDL_Quit();
                            exit(0);
                            break;
                        default:
                            break;

        }
    }
}}}
