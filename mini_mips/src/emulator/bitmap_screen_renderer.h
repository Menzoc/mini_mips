
#ifndef BITMAP_SCREEN_RENDERER_H
#define BITMAP_SCREEN_RENDERER_H

#include "../helpers/log.h"
#include "emulator_globals.h"
#include "SDL.h"
#include <sstream>
#include <iomanip>


class BitmapScreenRenderer {
public:
    explicit BitmapScreenRenderer(std::int32_t pixel_ratio);

    ~BitmapScreenRenderer();

    void draw_pixel(std::int32_t p_x, std::int32_t p_y, std::int32_t color);

    void render_screen();

private:
    const std::uint32_t RENDER_SLEEP_TIME_MS = 1000;

    std::int32_t pixel_ratio;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
};


#endif //BITMAP_SCREEN_RENDERER_H
