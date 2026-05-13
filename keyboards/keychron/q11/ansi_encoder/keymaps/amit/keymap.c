/* Q11 ANSI encoder — Amit's keymap, ported from Corne ZMK setup.
 *
 * Layers:
 *   0 MAC_BASE  QWERTY + HRMs on ASDF/JKL; + LT spaces
 *   1 MAC_FN    stock Mac Fn layer
 *   2 WIN_BASE  same as MAC_BASE with Win modifiers
 *   3 WIN_FN    stock Win Fn layer
 *   4 NAV       hold left space  — arrows on HJKL, mouse on YUIO, scroll, mouse buttons
 *   5 NUM       hold right space — numpad on right hand
 *
 * Thumb chord:
 *   hold left space  + tap right space (which is on NAV layer) → TAB
 *   hold right space + tap left space  (which is on NUM layer) → TAB
 *
 * Per-key tapping terms (see get_tapping_term below) mirror the ZMK Corne:
 *   pinky 220ms · ring 210 · middle 200 · index 180
 */
#include QMK_KEYBOARD_H

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
    NAV,
    NUM,
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

// Home-row mod aliases (left / right hand)
#define HM_A LCTL_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LGUI_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J RSFT_T(KC_J)
#define HM_K RGUI_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_SCLN RCTL_T(KC_SCLN)

// Layer-tap thumbs
#define LT_SPC LT(NAV, KC_SPC)
#define LT_ENT LT(NUM, KC_ENT)

// Homerow.app triggers — match these to the shortcuts set in Homerow prefs.
// Hyper combos (Ctrl+Alt+Cmd+<key>) are unlikely to clash with anything.
#define HR_CLK LCAG(KC_J)  // Click   -- label every clickable element
#define HR_SCR LCAG(KC_K)  // Scroll  -- keyboard-driven scrolling
#define HR_SRC LCAG(KC_L)  // Search  -- click by visible text

// Tap-dance on left space:
//   single tap  -> SPACE
//   hold        -> NAV layer
//   double tap  -> Homerow Click (HR_CLK)
// The single-tap fires immediately if interrupted by the next key press,
// so mid-sentence typing is not delayed; only an isolated space at the
// end of a burst waits ~tapping_term.
enum custom_tap_dances {
    TD_SPC_HR,
};

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
} td_state_t;

static td_state_t td_spc_state = TD_NONE;

static td_state_t spc_cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        return (state->interrupted || !state->pressed) ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
    }
    if (state->count >= 2) return TD_DOUBLE_TAP;
    return TD_NONE;
}

static void td_spc_finished(tap_dance_state_t *state, void *user_data) {
    td_spc_state = spc_cur_dance(state);
    switch (td_spc_state) {
        case TD_SINGLE_TAP:  tap_code(KC_SPC); break;
        case TD_SINGLE_HOLD: layer_on(NAV); break;
        case TD_DOUBLE_TAP:
            register_code(KC_LCTL); register_code(KC_LALT); register_code(KC_LGUI);
            tap_code(KC_J);
            unregister_code(KC_LGUI); unregister_code(KC_LALT); unregister_code(KC_LCTL);
            break;
        default: break;
    }
}

static void td_spc_reset(tap_dance_state_t *state, void *user_data) {
    if (td_spc_state == TD_SINGLE_HOLD) layer_off(NAV);
    td_spc_state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_SPC_HR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_spc_finished, td_spc_reset),
};

#define TD_SPC TD(TD_SPC_HR)

// CHORDAL_HOLD: which physical hand each key belongs to.
// HRMs only trigger their hold when chorded with an opposite-hand key.
// Thumbs and Fn keys are '*' so they chord with both hands — otherwise
// rolling thumb -> same-hand finger fast would suppress layer activation.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT_91_ansi(
    'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',      'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',      'R',
    'L', 'L', 'L', 'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R',                'R',           'R',
    'L', 'L',      'L', 'L', 'L', 'L',   'R', 'R', 'R', 'R', 'R', 'R',                'R',  'R',
    'L', 'L', 'L', 'L', '*',      '*',         '*',                  'R', '*', 'R', 'R', 'R', 'R'
);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_91_ansi(
        KC_MUTE,  KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_INS,   KC_DEL,   KC_MUTE,
        _______,  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        _______,  KC_ESC,   HM_A,     HM_S,     HM_D,     HM_F,     KC_G,      KC_H,     HM_J,     HM_K,     HM_L,     HM_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        _______,  KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        _______,  KC_LCTL,  KC_LOPT,  KC_LCMD,  MO(MAC_FN),         TD_SPC,                        LT_ENT,             KC_RCMD,  MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_91_ansi(
        RM_TOGG,  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,  _______,  RM_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,   _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            _______,                       _______,            _______,  _______,    _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_91_ansi(
        KC_MUTE,  KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,     KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_INS,   KC_DEL,   KC_MUTE,
        _______,  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,      KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,
        _______,  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,      KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,
        _______,  KC_ESC,   HM_A,     HM_S,     HM_D,     HM_F,     KC_G,      KC_H,     HM_J,     HM_K,     HM_L,     HM_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,
        _______,  KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,
        _______,  KC_LCTL,  KC_LWIN,  KC_LALT,  MO(WIN_FN),         TD_SPC,                        LT_ENT,             KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

    [WIN_FN] = LAYOUT_91_ansi(
        RM_TOGG,  _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,   RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  _______,  _______,  RM_TOGG,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,   _______,  _______,  _______,  _______,  _______,  _______,              _______,            _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            _______,                       _______,            _______,  _______,    _______,  _______,  _______,  _______),

    // hold LEFT space → nav + mouse + Homerow
    // Number row 1-5: macOS space/window navigation
    // Q/W/E: Homerow click/scroll/search
    // YUIOP: mouse buttons + scroll wheel
    // HJKL: arrow keys
    // Z/C: MS_ACL0 (precision) / MS_ACL2 (jump fast)
    // NM,.: mouse movement
    [NAV] = LAYOUT_91_ansi(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        _______,  _______,  LCTL(KC_LEFT), LCTL(KC_RIGHT), LCTL(KC_UP), LCTL(KC_DOWN), LGUI(KC_GRV),   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  HR_CLK,   HR_SCR,   HR_SRC,   _______,  _______,   MS_BTN1,  MS_BTN3,  MS_BTN2,  MS_WHLU,  MS_WHLD,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_TAB,   _______,              _______,            _______,
        _______,  _______,            MS_ACL0,  _______,  MS_ACL2,  _______,   _______,  MS_LEFT,  MS_DOWN,  MS_UP,    MS_RGHT,  _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            _______,                       KC_TAB,             _______,  _______,    _______,  _______,  _______,  _______),

    // hold RIGHT space → numpad on right hand
    [NUM] = LAYOUT_91_ansi(
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  KC_7,     KC_8,     KC_9,     KC_MINS,  _______,    _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  KC_4,     KC_5,     KC_6,     KC_0,     KC_EQL,               _______,            _______,
        _______,  _______,            _______,  _______,  _______,  _______,   _______,  KC_1,     KC_2,     KC_3,     KC_DOT,   _______,              _______,  _______,
        _______,  _______,  _______,  _______,  _______,            KC_TAB,                        _______,            _______,  _______,    _______,  _______,  _______,  _______),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HM_A:
        case HM_SCLN:
            return 220;  // pinky — slowest
        case HM_S:
        case HM_L:
            return 210;  // ring
        case HM_D:
        case HM_K:
            return 200;  // middle
        case HM_F:
        case HM_J:
            return 180;  // index — fastest (most rolls)
        default:
            return TAPPING_TERM;
    }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HM_A: case HM_S: case HM_D: case HM_F:
        case HM_J: case HM_K: case HM_L: case HM_SCLN:
            return 150;
        default:
            return QUICK_TAP_TERM;
    }
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN]   = { ENCODER_CCW_CW(RM_VALD, RM_VALU), ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [NAV]      = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(MS_WHLD, MS_WHLU) },
    [NUM]      = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
};
#endif
