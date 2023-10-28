#include "impl.h"

#include <stdio.h>

static cr_impl *create_impl(int screen_width, int screen_height, int scale);

//----------------------------------------------------------------------------
// core functions

int cr_initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        fprintf(
            stderr,
            "failed to initialize SDL. error: %s\n",
            SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr,
                "failed to initialize SDL_ttf, error: %s\n",
                TTF_GetError());
        SDL_Quit();
        return 0;
    }

    if (!IMG_Init(IMG_INIT_PNG))
    {
        fprintf(stderr,
                "failed to initialize SDL_image, error: %s\n",
                IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Open the SDL_Mixer audio device.
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr,
                "failed to open audio device, error: %s\n",
                Mix_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Initialize SDL_Mixer to allow playback of OGG files.
    if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG)
    {
        fprintf(stderr,
                "failed to initialize SDL_mixer, error: %s\n",
                Mix_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    cr_impl_init_keyboard();

    return 1;
}

void cr_terminate()
{
    Mix_Quit();       // closes handles to dependencies
    Mix_CloseAudio(); // terminates the actual SDL_Mixer library
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

// default update function
static void default_update(cr_app *app)
{
}

// default draw function
static void default_draw(cr_app *app)
{
}

cr_app *cr_create_app()
{
    int default_scale = 3;
    cr_app *app = NULL;

    // Create the app struct.
    app = (cr_app *)malloc(sizeof(cr_app));
    if (app == NULL)
    {
        return NULL;
    }

    // interface to the underlying implementation
    app->impl = create_impl(
        CR_DEFAULT_SCREEN_WIDTH,
        CR_DEFAULT_SCREEN_HEIGHT,
        default_scale);
    if (app->impl == NULL)
    {
        printf("failed to create impl\n");
        free(app);
        return NULL;
    }

    // debug configuration
    app->debug.overlay = 0;
    app->debug.hitboxes = 0;
    app->debug.camera = 0;
    app->debug.collisions = 0;
    app->debug.frame_len = 1;
    app->debug.frame_by_frame = 0;
    app->debug.fps = 0;

    // input actuation
    for (int i = 0; i < CR_MAX_KEYCODE; i++)
    {
        app->key_captures[i] = 0;
        app->actuation_counters[i] = 0;
    }

    app->origin_x = 0;
    app->origin_y = 0;
    app->scale = default_scale;
    app->time = TIMING_DELTA;
    app->done = 0;
    app->pause = 0;
    app->ticks = 0;
    app->scene = 0;
    app->frame_check = 0;
    app->entity_cap = 0;

    // screen dimensions
    app->screen_width = CR_DEFAULT_SCREEN_WIDTH;
    app->screen_height = CR_DEFAULT_SCREEN_HEIGHT;

    // camera
    app->cam.x = 0;
    app->cam.y = 0;
    app->cam.cl = 50;
    app->cam.cr = 180;
    app->cam.ct = 20;
    app->cam.cb = 140;

    app->update = default_update;
    app->draw = default_draw;

    //------------------------------------------------------------------------
    // BEGIN arrays
    // entities
    // input
    // menus
    // dialogs
    // overlays
    // textures
    // fonts
    // sounds
    app->entities = NULL;
    app->entity_count = 0;

    app->input = NULL;
    app->input_count = 0;

    app->menus = NULL;
    app->menu_count = 0;

    app->dialogs = NULL;
    app->dialog_count = 0;

    app->overlays = NULL;
    app->overlay_count = 0;

    app->textures = NULL;
    app->texture_count = 0;

    app->fonts = NULL;
    app->font_count = 0;

    app->sounds = NULL;
    app->sound_count = 0;
    // END arrays
    //------------------------------------------------------------------------

    return app;
}

void cr_destroy_app(cr_app *app)
{
    // Destroy the fonts.
    for (int i = 0; i < app->font_count; i++)
    {
        cr_impl_destroy_font(app->fonts[i]);
    }

    // Destroy the textures.
    for (int i = 0; i < app->texture_count; i++)
    {
        cr_impl_destroy_texture(app->textures[i]);
    }

    // Destroy the sounds.
    for (int i = 0; i < app->sound_count; i++)
    {
        cr_impl_destroy_sound(app->sounds[i]);
    }

    // Destroy the implementation.
    if (app->impl != NULL)
    {
        SDL_DestroyRenderer(app->impl->renderer);
        SDL_DestroyWindow(app->impl->window);
        free(app->impl);
    }

    // Free the memory allocated for the application struct.
    free(app);
}

void cr_set_title(cr_app *app, const char *title)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    cr_impl *impl = app->impl;

    SDL_SetWindowTitle(impl->window, title);
}

void cr_begin_frame(cr_app *app)
{
    // cr_impl_process_events(app);
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    cr_impl *impl = app->impl;

    //------------------------------------------------------------------------
    // process events

    // First, we get the current tick count for regulating framerate.
    impl->ticks = SDL_GetTicks64();

    // delta time
    Uint64 count = SDL_GetPerformanceCounter();
    impl->timing.delta = (float)(count - impl->timing.count) / impl->timing.frequency;
    impl->timing.count = count;

    int dsx = 0, dsy = 0;

    // According to the wiki, it is common practice to process all events in
    // the event queue at the beginning of each iteration of the main loop.
    // The SDL_PollEvent function also calls SDL_PumpEvents, which updates the
    // keyboard state array.
    while (SDL_PollEvent(&(impl->event)))
    {
        // If the window was closed, exit the application.
        if (impl->event.type == SDL_QUIT)
        {
            app->done = 1;
        }

        // Reposition the screen when the window is resized.
        if (impl->event.type == SDL_WINDOWEVENT &&
            impl->event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            Sint32 d1 = impl->event.window.data1;
            Sint32 d2 = impl->event.window.data2;

            dsx = d1 / app->screen_width ? d1 / app->screen_width : -(app->screen_width / d1);
            dsy = d2 / app->screen_height ? d2 / app->screen_height : -(app->screen_height / d2);

            // Determine if we should change the scale.
            if (dsx && dsy)
            {
                int grow_factor = dsx < dsy ? dsx : dsy;
                grow_factor = grow_factor >= 0 ? grow_factor : 1;
                SDL_RenderSetScale(impl->renderer, (float)(grow_factor), (float)(grow_factor));
                app->scale = grow_factor;

                // Attempt to center the screen.
                // If the whole screen cannot fit within the window, set the
                // origin to (0, 0).
                app->origin_x = d1 < app->screen_width ? 0 : ((d1 - (app->screen_width * app->scale)) / app->scale) / 2;
                app->origin_y = d2 < app->screen_height ? 0 : ((d2 - (app->screen_height * app->scale)) / app->scale) / 2;

                // Correct the scale for high DPI monitors.
                int ww, wh;
                int rw, rh;
                SDL_GetWindowSize(app->impl->window, &ww, &wh);
                SDL_GetRendererOutputSize(app->impl->renderer, &rw, &rh);
                if (rw > ww)
                {
                    int scale_correction_x = rw / ww;
                    int scale_correction_y = rh / wh;
                    SDL_RenderSetScale(app->impl->renderer, (float)scale_correction_x * app->scale, (float)scale_correction_y * app->scale);
                }
                else if (app->scale > 0)
                {
                    SDL_RenderSetScale(app->impl->renderer, (float)app->scale, (float)app->scale);
                }
            }
        }

        // If we have any textures that are render targets, they will
        // need to be refreshed.
        if (impl->event.type == SDL_RENDER_TARGETS_RESET)
        {
            app->impl->render_reset = 1;
        }

        // If a key was released, convert the SDL_Scancode into an cr_keycode
        // and set the corresponding key capture flag to 0.
        if (impl->event.type == SDL_KEYUP)
        {
            SDL_Scancode sc = impl->event.key.keysym.scancode;
            cr_keycode k = cr_impl_get_cr_keycode(sc);
            app->key_captures[k] = 0;
        }
    }

    //------------------------------------------------------------------------
    // clear screen

    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check < 1)
        {
            return;
        }
    }

    SDL_SetRenderDrawColor(impl->renderer, 0, 0, 0, 255);
    SDL_RenderClear(impl->renderer);

    if (app->impl->render_reset)
    {
        if (!cr_impl_rebuild_font_atlases(app))
        {
            fprintf(stderr, "failed to rebuild all font atlases\n");
            app->done = 1;
            return;
        }
        app->impl->render_reset = 0;
    }
}

void cr_end_frame(cr_app *app)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    cr_impl *impl = app->impl;

    //------------------------------------------------------------------------
    // render screen

    if ((app->time == TIMING_DELTA && app->frame_check >= 1) ||
        app->time == TIMING_WAIT)
    {
        // Fill in the padding around the screen.
        cr_impl_pad_window(app);

        SDL_RenderPresent(impl->renderer);
    }

    //------------------------------------------------------------------------
    // regulate framerate

    if (app->time == TIMING_DELTA)
    {
        if (app->frame_check >= 1)
        {
            app->frame_check = 0;
        }

        app->frame_check += app->impl->timing.delta * 60;
        return;
    }

    // Get the approximate number of milliseconds since the beginning of the
    // current iteration of the main loop.
    Uint64 elapsed = SDL_GetTicks64() - impl->ticks;

    // If the frame length is greater than the elapsed milliseconds since the
    // beginning of the frame, wait for the duration of the difference.
    if (impl->frame_len > elapsed)
    {
        // The SDL_Delay function expects a Uint32 as its argument.
        // Converting from Uint64 to Uint32 will truncate the value, but the
        // difference between the frame length and the elapsed milliseconds
        // should never be greater than UINT32_MAX.
        SDL_Delay((Uint32)(impl->frame_len * app->debug.frame_len - elapsed));
    }
}

//----------------------------------------------------------------------------
// drawing functions

void cr_set_color(cr_app *app, cr_color color)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    b = (Uint8)(color & 0xFF);
    g = (Uint8)((color >> 8) & 0xFF);
    r = (Uint8)((color >> 16) & 0xFF);
    a = (Uint8)((color >> 24) & 0xFF);

    cr_impl *impl = app->impl;
    SDL_SetRenderDrawColor(impl->renderer, r, g, b, a);
}

void cr_draw_line(cr_app *app, cr_point *a, cr_point *b)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    cr_impl *impl = app->impl;
    SDL_RenderDrawLine(
        impl->renderer,
        app->origin_x + a->x,
        app->origin_y + a->y,
        app->origin_x + b->x,
        app->origin_y + b->y);
}

void cr_draw_rect(cr_app *app, cr_rect *r, int filled)
{
    if (app == NULL || app->impl == NULL)
    {
        return;
    }

    cr_impl *impl = app->impl;

    SDL_Rect sr = {
        .x = app->origin_x + r->x,
        .y = app->origin_y + r->y,
        .w = r->w,
        .h = r->h};

    if (filled)
    {
        SDL_RenderFillRect(impl->renderer, &sr);
        return;
    }

    SDL_RenderDrawRect(impl->renderer, &sr);
}

//----------------------------------------------------------------------------
// input handling functions

int cr_peek_input(cr_app *app, int code)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    cr_impl *impl = app->impl;

    SDL_Scancode sc = cr_impl_get_sdl_scancode(code);

    if (sc >= SDL_NUM_SCANCODES || sc <= SDL_SCANCODE_UNKNOWN)
    {
        return 0;
    }

    if (impl->keystates[sc])
    {
        return 1;
    }

    return 0;
}

int cr_consume_input(cr_app *app, int code)
{
    if (app == NULL || app->impl == NULL)
    {
        return 0;
    }

    cr_impl *impl = app->impl;

    SDL_Scancode sc = cr_impl_get_sdl_scancode(code);

    if (sc >= SDL_NUM_SCANCODES || sc <= SDL_SCANCODE_UNKNOWN)
    {
        return 0;
    }

    if (app->key_captures[code])
    {
        return 0;
    }

    if (impl->keystates[sc])
    {
        app->key_captures[code] = 1;
        return 1;
    }

    return 0;
}

void cr_push_input_handler(cr_app *app, cr_func handler)
{
    if (app == NULL || app->input == NULL || handler == NULL)
    {
        return;
    }

    app->input[app->input_count++] = handler;
}

void cr_pop_input_handler(cr_app *app)
{
    // If the stack is empty, then return without doing anything.
    if (app == NULL || app->input == NULL)
    {
        return;
    }

    if (app->input_count < 1)
    {
        printf("[WARN] attempting to pop input handler from empty stack\n");
        return;
    }

    app->input_count--;
}

//----------------------------------------------------------------------------
// entity functions

cr_entity *cr_create_entity(cr_app *app)
{
    cr_entity *entity = NULL;

    // Look for the available entity slot.
    for (int i = 0; i < app->entity_cap && entity == NULL; i++)
    {
        if (!app->entities[i].present)
        {
            entity = &(app->entities[i]);
        }
    }

    if (entity == NULL)
    {
        printf("[WARN] entity list is full\n");
        return NULL;
    }

    entity->present = 1;
    entity->type = 0;
    entity->x_pos = 0;
    entity->y_pos = 0;
    entity->x_vel = 0;
    entity->y_vel = 0;
    entity->x_acc = 0;
    entity->y_acc = 0;
    entity->x_t = 0;
    entity->y_t = 0;
    entity->flags = 0;
    entity->data = 0;
    entity->animation_ticks = 0;
    entity->ticks = 0;
    entity->iframes = 0;
    entity->carrier = NULL;
    entity->text = NULL;
    entity->text_len = 0;
    entity->tick_limit = 0;
    entity->result = 0;
    entity->cursor_x = 0;
    entity->cursor_y = 0;
    entity->scroll_y = 0;
    entity->scroll_y = 0;

    return entity;
}

void cr_remove_entity(cr_app *app, cr_entity *entity)
{
    entity->present = 0;
}

int cr_check_flag(cr_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return 0;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Check if the flag is set.
    if (e->flags & bit)
    {
        return 1;
    }

    return 0;
}

void cr_set_flag(cr_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Set the flag.
    e->flags |= bit;
}

void cr_clear_flag(cr_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // Clear the flag.
    e->flags &= ~bit;
}

void cr_toggle_flag(cr_entity *e, int f)
{
    // The flag index must be in the range [0, 15] since we're only dealing
    // with 16 bits.
    if (f < 0 || f >= 16)
    {
        return;
    }

    // Convert the flag index to a bit value.
    uint16_t bit = (uint16_t)(1 << f);

    // If the flag is already set, then clear it.
    if (e->flags & bit)
    {
        e->flags &= ~bit;
        return;
    }

    // Set the flag.
    e->flags |= bit;
}

//----------------------------------------------------------------------------
// font functions

cr_font *cr_load_font(cr_app *app, const char *path, int p)
{
    return cr_impl_load_font(app, path, p);
}

void cr_draw_text(cr_app *app, cr_font *font, const char *msg, int x, int y)
{
    cr_impl_draw_text(app, font, msg, app->origin_x + x, app->origin_y + y);
}

void cr_draw_text_bounded(cr_app *app, cr_font *font, const char *msg, cr_rect *bounds, int *result)
{
    cr_impl_draw_text_bounded(app, font, msg, bounds, result);
}

//----------------------------------------------------------------------------
// texture functions

cr_texture *cr_load_texture(cr_app *app, const char *path)
{
    return cr_impl_load_texture(app, path);
}

void cr_draw_texture(cr_app *app, cr_texture *texture, cr_rect *src, cr_rect *dest, int mirror)
{
    cr_impl_draw_texture(app, texture, src, dest, mirror);
}

//----------------------------------------------------------------------------
// audio functions

cr_sound *cr_load_sound(cr_app *app, const char *path, int type)
{
    return cr_impl_load_sound(app, path, type);
}

void cr_play_sound(cr_app *app, cr_sound *sound)
{
    cr_impl_play_sound(app, sound);
}

static cr_impl *create_impl(int screen_width, int screen_height, int scale)
{
    cr_impl *impl;
    SDL_Window *window;
    SDL_Renderer *renderer;
    const Uint8 *keystates;

    // Create the wrapper struct.
    impl = (cr_impl *)malloc(sizeof(struct cr_impl));
    if (impl == NULL)
    {
        return NULL;
    }

    // Create the window.
    // NOTE: we use SDL_WINDOW_ALLOW_HIGHDPI on macOS.
    window = SDL_CreateWindow(
        "Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screen_width * scale,
        screen_height * scale,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
    {
        free(impl);
        return NULL;
    }

    // Create the renderer.
    // We used SDL_RENDERER_PRESENTVSYNC to prevent screen tearing on things
    // like MacBook.
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    // NOTE: to enable transparency when setting the draw color,
    // we set the blend mode to SDL_BLENDMODE_BLEND.
    // This is mainly used for debugging.
    // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Correct the scale for high DPI monitors.
    int ww, wh;
    int rw, rh;
    SDL_GetWindowSize(window, &ww, &wh);
    SDL_GetRendererOutputSize(renderer, &rw, &rh);
    printf("[DEBUG] window: (%d x %d), renderer: (%d, %d)\n", ww, wh, rw, rh);
    if (rw > ww)
    {
        int scale_correction_x = rw / ww;
        int scale_correction_y = rh / wh;
        printf("[DEBUG] scale correction factor: (%d, %d)\n",
               scale_correction_x,
               scale_correction_y);
        SDL_RenderSetScale(renderer, (float)scale_correction_x * scale, (float)scale_correction_y * scale);
    }
    else if (scale > 0)
    {
        SDL_RenderSetScale(renderer, (float)scale, (float)scale);
    }

    // Get the keyboard state.
    keystates = SDL_GetKeyboardState(NULL);
    if (keystates == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        free(impl);
        return NULL;
    }

    // Populate the fields of the wrapper struct.
    impl->window = window;
    impl->renderer = renderer;
    impl->keystates = keystates;
    impl->frame_len = 16;
    impl->ticks = 0;
    impl->timing.frequency = SDL_GetPerformanceFrequency();
    impl->timing.count = SDL_GetPerformanceCounter();
    impl->timing.delta = 0;

    // Notes on frame_len field:
    // Since the desired framerate is 60 frames per second, we set the
    // approximate milliseconds per frame to 16. This is the truncated result
    // of 1000 / 60.

    // Output renderer info in case anyone is interested.
    SDL_RendererInfo ri;
    if (!SDL_GetRendererInfo(impl->renderer, &ri))
    {
        printf("[DEBUG] Renderer Info\n-------------\n");
        printf("[DEBUG] Name: %s\n", ri.name);
        printf("[DEBUG] Number of texture formats: %d\n", ri.num_texture_formats);
        printf("[DEBUG] Max Width: %d\n", ri.max_texture_width);
        printf("[DEBUG] Max Height: %d\n", ri.max_texture_height);
        printf("[DEBUG] SDL_RENDERER_SOFTWARE: %s\n", (ri.flags & SDL_RENDERER_SOFTWARE) ? "yes" : "no");
        printf("[DEBUG] SDL_RENDERER_ACCELERATED: %s\n", (ri.flags & SDL_RENDERER_ACCELERATED) ? "yes" : "no");
        printf("[DEBUG] SDL_RENDERER_PRESENTVSYNC: %s\n", (ri.flags & SDL_RENDERER_PRESENTVSYNC) ? "yes" : "no");
        printf("[DEBUG] SDL_RENDERER_TARGETTEXTURE: %s\n", (ri.flags & SDL_RENDERER_TARGETTEXTURE) ? "yes" : "no");
    }

    return impl;
}
