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
