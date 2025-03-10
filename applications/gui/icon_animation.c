#include "icon_animation_i.h"
#include "icon_i.h"

#include <furi.h>

IconAnimation* icon_animation_alloc(const Icon* icon) {
    furi_assert(icon);
    IconAnimation* instance = furi_alloc(sizeof(IconAnimation));
    instance->icon = icon;
    instance->timer = osTimerNew(icon_animation_timer_callback, osTimerPeriodic, instance, NULL);
    return instance;
}

void icon_animation_free(IconAnimation* instance) {
    furi_assert(instance);
    furi_check(osTimerDelete(instance->timer) == osOK);
    free(instance);
}

void icon_animation_set_update_callback(
    IconAnimation* instance,
    IconAnimationCallback callback,
    void* context) {
    furi_assert(instance);
    instance->callback = callback;
    instance->callback_context = context;
}

const uint8_t* icon_animation_get_data(IconAnimation* instance) {
    return instance->icon->frames[instance->frame];
}

void icon_animation_next_frame(IconAnimation* instance) {
    furi_assert(instance);
    instance->frame = (instance->frame + 1) % instance->icon->frame_count;
}

void icon_animation_timer_callback(void* context) {
    furi_assert(context);

    IconAnimation* instance = context;

    if(!instance->animating) return;

    icon_animation_next_frame(instance);
    if(instance->callback) {
        instance->callback(instance, instance->callback_context);
    }
}

uint8_t icon_animation_get_width(IconAnimation* instance) {
    furi_assert(instance);
    return instance->icon->width;
}

uint8_t icon_animation_get_height(IconAnimation* instance) {
    furi_assert(instance);
    return instance->icon->height;
}

void icon_animation_start(IconAnimation* instance) {
    furi_assert(instance);
    if(!instance->animating) {
        instance->animating = true;
        furi_assert(instance->icon->frame_rate);
        furi_check(
            xTimerChangePeriod(
                instance->timer, (osKernelGetTickFreq() / instance->icon->frame_rate), 0) ==
            pdPASS);
    }
}

void icon_animation_stop(IconAnimation* instance) {
    furi_assert(instance);
    if(instance->animating) {
        instance->animating = false;
        furi_check(xTimerStop(instance->timer, 0) == pdPASS);
        instance->frame = 0;
    }
}

bool icon_animation_is_last_frame(IconAnimation* instance) {
    furi_assert(instance);
    return instance->icon->frame_count - instance->frame <= 1;
}
