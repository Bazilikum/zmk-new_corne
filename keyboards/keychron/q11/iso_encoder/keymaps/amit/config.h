#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 6

#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 150
#define QUICK_TAP_TERM_PER_KEY

// Same-hand-key suppression: an HRM only triggers its hold when chorded with
// an opposite-hand key. Layout markings live in chordal_hold_layout in keymap.c.
#define CHORDAL_HOLD

#define PERMISSIVE_HOLD

#define COMBO_TERM 30

// Mouse keys: snappy ramp, no startup delay.
// Acceleration is on by default; MS_ACL0 / MS_ACL2 override to a fixed
// slow/fast speed while held (bound to Z and C on the NAV layer).
#define MOUSEKEY_DELAY 0
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_MAX_SPEED 7
#define MOUSEKEY_TIME_TO_MAX 60
#define MOUSEKEY_WHEEL_DELAY 0
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

// RGB: switch to SPLASH while typing, restore the user-selected pattern after
// this many milliseconds of inactivity. 2500 = ~2.5s after the last keypress.
#define RGB_IDLE_RESTORE_MS 2500

// Auto-off after 5 minutes of total inactivity. Wakes on any key press.
// 300000 ms = 5 min. Lower for sooner sleep.
#define RGB_MATRIX_TIMEOUT 300000
