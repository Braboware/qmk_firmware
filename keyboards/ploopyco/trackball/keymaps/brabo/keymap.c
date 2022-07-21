/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum custom_keycodes {
    TASKVIEW = PLOOPY_SAFE_RANGE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN1, KC_BTN3, LT(0,KC_NO), KC_BTN2, KC_F13 ),
    [1] = LAYOUT( DPI_CONFIG, _______, _______, KC_BTN4, KC_BTN5 ),
    [2] = LAYOUT( _______, _______, _______, _______, _______ ),
    [3] = LAYOUT( _______, _______, _______, _______, _______ ),
    [4] = LAYOUT( _______, _______, _______, _______, _______ ),
    [5] = LAYOUT( _______, _______, _______, _______, _______ ),
    [6] = LAYOUT( _______, _______, _______, _______, _______ ),
    [7] = LAYOUT( _______, _______, _______, _______, _______ )
};


// https://github.com/bmijanovich/qmk_firmware/blob/ploopy_classic_mac/keyboards/ploopyco/trackball/keymaps/bmijanovich/keymap.c
// ----------------------------------------------------------------------------------------
/* Functions for sending custom keycodes
QMK functions can't register custom keycodes, but we can setup a keyrecord_t and call process_record_kb() directly.
Unknowns:
    * Do we need to set the column and row for each keycode?
    * Could reusing the same keyrecord_t struct cause problems with keycodes clobbering each other?
*/
// Dummy keyrecord_t for hooking process_record_kb() with custom keycodes
static keyrecord_t dummy_record = {
    .event = {
        .key = {
            .col = 0,
            .row = 0,
            },
        .pressed = false,
        .time = 0,
    },
    .tap = {0},
};

// Setup dummy_record for process_record_kb()
void setup_dummy_record(uint8_t col, uint8_t row, bool pressed) {
    dummy_record.event.key.col = col;
    dummy_record.event.key.row = row;
    dummy_record.event.pressed = pressed;
    dummy_record.event.time = timer_read();
}

// Register a custom keycode with process_record_kb()
void register_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
    setup_dummy_record(col, row, true);
    process_record_kb(keycode, &dummy_record);
}

// Unregister a custom keycode with process_record_kb()
void unregister_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
    setup_dummy_record(col, row, false);
    process_record_kb(keycode, &dummy_record);
}

// Tap a custom keycode with process_record_kb()
// void tap_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
//     register_custom_keycode(keycode, col, row);
//     wait_ms(10);
//     unregister_custom_keycode(keycode, col, row);
// }
/* End functions for sending custom keycodes */
// ----------------------------------------------------------------------------------------


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(0,KC_NO):
            if (record->tap.count && record->event.pressed) {
                tap_code16(LGUI(KC_TAB)); // Intercept tap function to send LGUI + tab
                unregister_custom_keycode(DRAG_SCROLL, 3, 0); // If this isn't here drag scroll toggles on tap and the 'else unregister' doesn't catch it for some reason
            } else if (!record->tap.count && record->event.pressed) {
                register_custom_keycode(DRAG_SCROLL, 3, 0);
                layer_on(1); // layer 1 on for forward and backward buttons while holding drag scroll
            } else {
                unregister_custom_keycode(DRAG_SCROLL, 3, 0); // Release drag scroll
                layer_off(1);
            }
            return false;
    }
    return true;
}
