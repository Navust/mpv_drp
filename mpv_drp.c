#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <libmpv/client.h>

void mpv_print(mpv_handle *handle, const char *msg) {
    const char *cmd[] = {"print-text", msg, NULL};
    mpv_command(handle, cmd);
}

void mpv_printf(mpv_handle *handle, const char *fmt, ...) {
    char buf[1024];
    va_list args;

    va_start(args, fmt);

    vsnprintf(buf, sizeof(buf), fmt, args);
    mpv_print(handle, buf);

    va_end(args);
}

MPV_EXPORT int mpv_open_cplugin(mpv_handle *handle) {

    mpv_observe_property(handle, 0, "pause", MPV_FORMAT_FLAG);

    while (1) {
        const mpv_event *event = mpv_wait_event(handle, -1);
        // mpv_printf(handle, "Got event: %d", event->event_id);

        if (event->event_id == MPV_EVENT_PROPERTY_CHANGE) {
            const mpv_event_property *property = event->data;

            if (strcmp(property->name, "pause") == 0) {
                const int pause = *(int *) property->data;
                mpv_printf(handle, "Property changed: %s %d", property->name, pause);
            }
        }

        if (event->event_id == MPV_EVENT_SEEK) {
            mpv_printf(handle, "Seeking");
        }

        if (event->event_id == MPV_EVENT_START_FILE) {
            char *title;
            mpv_get_property(handle, "media-title/no-ext", MPV_FORMAT_STRING, &title);
            mpv_printf(handle, "Starting episode: %s", title);
            mpv_free(title);
        }

        if (event->event_id == MPV_EVENT_END_FILE) {
            mpv_printf(handle, "Finished");
        }

        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
    }

    return 0;
}
