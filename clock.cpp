#include <SDL2/SDL.h>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
const int CLOCK_RADIUS = 150;
const int HOUR_HAND_LENGTH = 80;
const int MINUTE_HAND_LENGTH = 120;
const int SECOND_HAND_LENGTH = 140;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void drawClock() {
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw the clock outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int angle = 0; angle < 360; ++angle) {
        float radian = angle * M_PI / 180.0f;
        int x = centerX + CLOCK_RADIUS * std::cos(radian);
        int y = centerY + CLOCK_RADIUS * std::sin(radian);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    // Get the current time
    time_t currentTime = time(nullptr);
    struct tm* localTime = localtime(&currentTime);

    // Calculate the angles for the hour, minute, and second hands
    int hour = localTime->tm_hour % 12;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;
    float hourAngle = (hour * 30 + minute * 0.5) * M_PI / 180.0;
    float minuteAngle = (minute * 6 + second * 0.1) * M_PI / 180.0;
    float secondAngle = (second * 6) * M_PI / 180.0;

    // Draw the hour hand
    int hourX = centerX + HOUR_HAND_LENGTH * sin(hourAngle);
    int hourY = centerY - HOUR_HAND_LENGTH * cos(hourAngle);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, centerX, centerY, hourX, hourY);

    // Draw the minute hand
    int minuteX = centerX + MINUTE_HAND_LENGTH * sin(minuteAngle);
    int minuteY = centerY - MINUTE_HAND_LENGTH * cos(minuteAngle);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, centerX, centerY, minuteX, minuteY);

    // Draw the second hand
    int secondX = centerX + SECOND_HAND_LENGTH * sin(secondAngle);
    int secondY = centerY - SECOND_HAND_LENGTH * cos(secondAngle);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, centerX, centerY, secondX, secondY);

    // Render the clock
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Analog Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while (true) {
        auto start = std::chrono::steady_clock::now();

        SDL_Event event;
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }

        drawClock();

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Wait for approximately 500 milliseconds before updating the clock again
        // Framerate limiter to reduce CPU load
        SDL_Delay(500 - elapsed.count());
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}