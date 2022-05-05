#include "app.h"

#include <stdio.h>

/**
 * Main function
 */
int main(int argc, char *argv[]) {
    App app;

    //1536x810
    init_app(&app, 1536, 810);
    //init_app(&app, 1024, 768);
    while (app.is_running) {
        //toggles cursor
        SDL_ShowCursor(SDL_DISABLE);
        handle_app_events(&app);
        update_app(&app);
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
