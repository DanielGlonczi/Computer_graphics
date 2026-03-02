#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_LINES 1000
#define PALETTE_COLORS 6

/* a szín RGBA komponensekkel */
typedef struct Color {
    Uint8 r, g, b, a;
} Color;

/* szakasz két végponttal és színnel */
typedef struct Line {
    int x1, y1;
    int x2, y2;
    Color color;
} Line;

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Szakaszrajzoló",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH,
                                       WINDOW_HEIGHT,
                                       0);
    if (!win) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    /* színpaletta néhány alap színnel */
    Color palette[PALETTE_COLORS] = {
        {255, 0, 0, 255},   /* piros */
        {0, 255, 0, 255},   /* zöld */
        {0, 0, 255, 255},   /* kék */
        {255, 255, 0, 255}, /* sárga */
        {255, 0, 255, 255}, /* magenta */
        {255, 255, 255, 255}/* fehér */
    };
    Color current_color = palette[0];

    Line lines[MAX_LINES];
    int line_count = 0;
    bool have_start = false;
    int sx = 0, sy = 0;

    bool quit = false;
    while (!quit) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_MOUSEBUTTONDOWN: {
                int mx = ev.button.x;
                int my = ev.button.y;
                printf("Egérkattintás: %d, %d\n", mx, my);

                /* paletta ellenőrzése */
                int cellw = 40, cellh = 40;
                int px = 10, py = 10;
                for (int i = 0; i < PALETTE_COLORS; ++i) {
                    SDL_Rect rc = {px + i * (cellw + 5), py, cellw, cellh};
                    if (mx >= rc.x && mx < rc.x + rc.w &&
                        my >= rc.y && my < rc.y + rc.h) {
                        current_color = palette[i];
                        break;
                    }
                }

                /* ha nincs kezdőpont, akkor beállítjuk, ellenkező esetben
                   létrehozzuk a szakaszt a két pont között */
                if (!have_start) {
                    sx = mx;
                    sy = my;
                    have_start = true;
                } else {
                    if (line_count < MAX_LINES) {
#ifndef RECTS
                        lines[line_count++] = (Line){sx, sy, mx, my, current_color};
#else
                        /* Téglalapként értelmezettek a két pont és a szín */
                        lines[line_count++] = (Line){sx, sy, mx, my, current_color};
#endif
                    }
                    have_start = false;
                }
                break;
            }
            }
        }

        /* kirajzolás */
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        /* paletta megjelenítése */
        for (int i = 0; i < PALETTE_COLORS; ++i) {
            SDL_SetRenderDrawColor(ren,
                                   palette[i].r,
                                   palette[i].g,
                                   palette[i].b,
                                   palette[i].a);
            SDL_Rect rc = {10 + i * 45, 10, 40, 40};
            SDL_RenderFillRect(ren, &rc);
        }

        /* szakaszok/téglalapok */
        for (int i = 0; i < line_count; ++i) {
            SDL_SetRenderDrawColor(ren,
                                   lines[i].color.r,
                                   lines[i].color.g,
                                   lines[i].color.b,
                                   lines[i].color.a);
#ifndef RECTS
            SDL_RenderDrawLine(ren,
                               lines[i].x1,
                               lines[i].y1,
                               lines[i].x2,
                               lines[i].y2);
#else
            SDL_Rect r;
            r.x = lines[i].x1;
            r.y = lines[i].y1;
            r.w = lines[i].x2 - lines[i].x1;
            r.h = lines[i].y2 - lines[i].y1;
            if (r.w < 0) { r.x += r.w; r.w = -r.w; }
            if (r.h < 0) { r.y += r.h; r.h = -r.h; }
            SDL_RenderFillRect(ren, &r);
#endif
        }

        SDL_RenderPresent(ren);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
