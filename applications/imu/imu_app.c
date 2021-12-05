#include <furi.h>
#include <furi-hal.h>

#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification-messages.h>

#include "imu.h"

typedef struct {
    InputEvent input;
} IMUEvent;

void imu_draw_callback(Canvas* canvas, void* ctx) {
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "IMU application");

    char acc_str[40];
    struct Vector acc = imu_get_acc();
    sprintf(acc_str, "Acc: %.1f %.1f %.1f ", acc.XAxis, acc.YAxis, acc.ZAxis);

    char gyro_str[40];
    struct Vector gyro = imu_get_gyro();
    sprintf(gyro_str, "Gyro: %.1f %.1f %.1f ", gyro.XAxis, gyro.YAxis, gyro.ZAxis);

    char att_str[40];
    struct Vector att = imu_get_attitude();
    sprintf(att_str, "Roll: %.1f° Pitch: %.1f°", att.XAxis, att.YAxis);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 22, acc_str);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 34, gyro_str);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 46, att_str);
}

void imu_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    osMessageQueueId_t event_queue = ctx;

    IMUEvent event = {.input = *input_event};
    osMessageQueuePut(event_queue, &event, 0, 0);
}

int32_t imu_app(void* p) {
    imu_init();

    osMessageQueueId_t event_queue = osMessageQueueNew(8, sizeof(IMUEvent), NULL);

    // Configure view port
    ViewPort* view_port = view_port_alloc();
    furi_check(view_port);
    view_port_draw_callback_set(view_port, imu_draw_callback, NULL);
    view_port_input_callback_set(view_port, imu_input_callback, event_queue);

    // Register view port in GUI
    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    // NotificationApp* notification = furi_record_open("notification");

    IMUEvent event;

    while(1) {
        furi_check(osMessageQueueGet(event_queue, &event, NULL, osWaitForever) == osOK);
        if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
            // notification_message(notification, &sequence_reset_vibro);
            // notification_message(notification, &sequence_reset_green);
            furi_record_close("notification");
            view_port_enabled_set(view_port, false);
            gui_remove_view_port(gui, view_port);
            view_port_free(view_port);
            osMessageQueueDelete(event_queue);

            imu_release();
            return 0;
        }
        // if(event.input.key == InputKeyOk) {
        //     if(event.input.type == InputTypePress) {
        //         notification_message(notification, &sequence_set_vibro_on);
        //         notification_message(notification, &sequence_set_green_255);
        //     } else if(event.input.type == InputTypeRelease) {
        //         notification_message(notification, &sequence_reset_vibro);
        //         notification_message(notification, &sequence_reset_green);
        //     }
        // }
    }

    return 0;
}
