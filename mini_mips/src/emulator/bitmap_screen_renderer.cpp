
#include "bitmap_screen_renderer.h"

/*
 * BitmapScreenRenderer class definition
 */

/* PUBLIC DEFINITION */
BitmapScreenRenderer::BitmapScreenRenderer(std::int32_t pixel_ratio) : pixel_ratio(pixel_ratio) {
    // Initializing SDL as Video
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(RAM_MEMORY_BITMAP_SCREEN_WIDTH * pixel_ratio, RAM_MEMORY_BITMAP_SCREEN_HEIGHT * pixel_ratio, 0, &window, &renderer);
    // setting draw color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // Clear the newly created window
    SDL_RenderClear(renderer);
    // Reflects the changes done in the window.
    SDL_RenderPresent(renderer);
}

BitmapScreenRenderer::~BitmapScreenRenderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void BitmapScreenRenderer::draw_pixel(std::int32_t p_x, std::int32_t p_y, std::int32_t color) {
    if (p_x >= (std::int32_t) RAM_MEMORY_BITMAP_SCREEN_WIDTH) {
        LOG(LOG_ERROR) << "Error : x value " << p_x << " is bigger than the screen width size (" << RAM_MEMORY_BITMAP_SCREEN_WIDTH << ").";
        throw std::invalid_argument("Too big screen x value!");
    }
    if (p_y >= (std::int32_t) RAM_MEMORY_BITMAP_SCREEN_HEIGHT) {
        LOG(LOG_ERROR) << "Error : y value " << p_y << " is bigger than the screen height size (" << RAM_MEMORY_BITMAP_SCREEN_HEIGHT << ").";
        throw std::invalid_argument("Too big screen y value!");
    }

    // Convert the color to the hexadecimal string
    std::stringstream stream = {};
    stream << std::setfill('0') << std::setw(sizeof(std::int32_t) * 2) << std::hex << color;
    std::string hex_str = stream.str();
    // Split the hexadecimal string to each rgba component
    std::uint16_t r = std::stoul(hex_str.substr(2, 2), nullptr, 16);
    std::uint16_t g = std::stoul(hex_str.substr(4, 2), nullptr, 16);
    std::uint16_t b = std::stoul(hex_str.substr(6, 2), nullptr, 16);
    // std::uint16_t a = std::stoul(hex_str.substr(6, 2), nullptr, 16);
    LOG(LOG_DEBUG) << "R:" << r << ", G: " << g << ", B: " << b;
    LOG(LOG_DEBUG) << "px:" << p_x << ", py: " << p_y;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    // Drawing a square of pixel_ratio*pixel_ratio
    for (std::int32_t x = p_x * pixel_ratio; x <= p_x * pixel_ratio + pixel_ratio; x++) {
        for (std::int32_t y = p_y * pixel_ratio; y <= p_y * pixel_ratio + pixel_ratio; y++) {
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void BitmapScreenRenderer::render_screen() {
    // Show the change on the screen
    SDL_RenderPresent(renderer);
    SDL_Delay(RENDER_SLEEP_TIME_MS);
}

