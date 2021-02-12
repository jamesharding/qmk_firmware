#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    RGB_TIMEOUT,
    RGB_TIMEOUT_INDICATOR
};

enum LAYERS {
    L0 = 0,
    L1,
    L2
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L0] = 	LAYOUT_65_ansi_blocker(
		KC_ESC , KC_1    , KC_2   , KC_3   , KC_4   , KC_5   , KC_6   , KC_7   , KC_8    , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC, KC_DEL ,
		KC_TAB , KC_Q    , KC_W   , KC_E   , KC_R   , KC_T   , KC_Y   , KC_U   , KC_I    , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
		KC_CAPS, KC_A    , KC_S   , KC_D   , KC_F   , KC_G   , KC_H   , KC_J   , KC_K    , KC_L   , KC_SCLN, KC_QUOT, KC_ENT , KC_PGUP,
		KC_LSPO, KC_Z    , KC_X   , KC_C   , KC_V   , KC_B   , KC_N   , KC_M   , KC_COMM , KC_DOT , KC_SLSH, KC_RSPC, KC_UP  , KC_PGDN,
		KC_LCTL, KC_LGUI , KC_LALT, KC_SPC , MO(2)  , MO(1)  , KC_LEFT, KC_DOWN, KC_RGHT),
	[L1] = LAYOUT_65_ansi_blocker(
		KC_GRV , KC_F1   , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8   , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_TRNS, KC_MUTE,
		KC_TRNS, RGB_SPD , RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, KC_TRNS, KC_TRNS, KC_TRNS , KC_TRNS, KC_PSCR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, RGB_RMOD, RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, KC_TRNS, KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU,
		KC_TRNS, RGB_TOG , KC_TRNS, KC_TRNS, KC_TRNS, MD_BOOT, NK_TOGG, RGB_TIMEOUT, RGB_TIMEOUT_INDICATOR , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD,
		KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT),
	[L2] = LAYOUT_65_ansi_blocker(
		KC_TRNS, KC_P1   , KC_P2  , KC_P3  , KC_P4  , KC_P5  , KC_P6  , KC_P7  , KC_P8   , KC_P9  , KC_P0  , KC_PMNS, KC_PPLS, KC_TRNS, KC_NLCK,
		KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, LAG_NRM , LAG_SWP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______
    ),
    */
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))
// #define IDLE_TIMER_DURATION 600 //how many seconds before RGB turns off
#define MILLISECOND 1000

static uint32_t idle_timer_seconds = 600; //options: 0 (disabled), 600 (10 mins), 3600 (1 hour)
static uint32_t idle_timer; //custom timer to check if keyboard is idled.
static bool rgbkeyIdle = false; //flag for keyboard idling, nil keys for set
static bool layer_reset_required = false;
static bool rgb_timeout_indicator_enabled = true;
static uint32_t elapsed_seconds = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    static uint32_t key_timer;
    idle_timer = timer_read();


    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT:
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG:
            if (record->event.pressed) {
              switch (rgb_matrix_get_flags()) {
                case LED_FLAG_ALL: {
                    rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR): {
                    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                    rgb_matrix_set_color_all(0, 0, 0);
                  }
                  break;
                case LED_FLAG_UNDERGLOW: {
                    rgb_matrix_set_flags(LED_FLAG_NONE);
                    rgb_matrix_disable_noeeprom();
                  }
                  break;
                default: {
                    rgb_matrix_set_flags(LED_FLAG_ALL);
                    rgb_matrix_enable_noeeprom();
                  }
                  break;
              }
            }
            return false;
        case RGB_TIMEOUT:
            if (record->event.pressed) {
                switch (idle_timer_seconds) {
                    case 0: {
                        idle_timer_seconds = 600;
                    }
                    break;
                    case 600: {
                        idle_timer_seconds = 3600;
                    }
                    break;
                    case 3600: {
                        idle_timer_seconds = 0;
                    }
                    break;
                }
            }
            return false;
        case RGB_TIMEOUT_INDICATOR:
            if (record->event.pressed) {
                rgb_timeout_indicator_enabled = !rgb_timeout_indicator_enabled;
            }
            return false;
        default:
            elapsed_seconds = 0;
            if (rgbkeyIdle) {       //check if the keyboards already idle and if it is, turn it back on as key is pressed.
                rgbkeyIdle = false;
                rgb_matrix_set_suspend_state(false);
                rgb_matrix_enable_noeeprom();
            }
            return true; //Process all other keycodes normally
    }
}

void matrix_init_user(void) {
    rgblight_mode(RGB_MATRIX_SOLID_REACTIVE);				// Initially in the fade mode.
	rgb_matrix_sethsv(HSV_CYAN);
    rgb_matrix_decrease_speed();
	rgb_matrix_decrease_speed();
	rgb_matrix_decrease_speed();
}

void matrix_scan_user(void) {
//custom idle rbg switch off function
    if(!rgbkeyIdle && idle_timer_seconds != 0){
        if (timer_elapsed(idle_timer) > MILLISECOND) {
            elapsed_seconds++;
            if(elapsed_seconds > idle_timer_seconds){
                timer_clear();
                rgbkeyIdle = true;
                rgb_matrix_set_suspend_state(true);
                rgb_matrix_disable_noeeprom();
            }
            idle_timer = timer_read(); // Tick the timer up one second
        }
    }
}

void suspend_power_down_user(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_user(void) {
    rgb_matrix_set_suspend_state(false);
}

void rgb_matrix_set_colour_range(uint8_t start, uint8_t finish, uint8_t r, uint8_t g, uint8_t b){
	uint8_t index = start;
	while(index <= finish){
			rgb_matrix_set_color(index, r, g, b);
		index++;
	}
}

#define RGB_BLACK 0x00, 0x00, 0x00

void rgb_matrix_indicators_user(void)
{
    switch (biton32(layer_state)) {
		case L1:
			rgb_matrix_set_colour_range(67, 81, RGB_RED); // Front bar
			rgb_matrix_set_colour_range(0, 12, RGB_RED);  // Number keys
            rgb_matrix_set_colour_range(45, 45, RGB_RED); // Underglow Toggle
            rgb_matrix_set_colour_range(16, 20, RGB_RED); // First Row
            rgb_matrix_set_colour_range(31, 35, RGB_RED); // First Row
			rgb_matrix_set_colour_range(49, 49, RGB_RED); // Reset
			rgb_matrix_set_colour_range(50, 50, RGB_RED); // NKRO

            if(idle_timer_seconds == 0){
                rgb_matrix_set_colour_range(51, 51, RGB_RED); // RGB Toggle
            }
            else if(idle_timer_seconds == 600){
                rgb_matrix_set_colour_range(51, 51, RGB_GREEN); // RGB Toggle
            }
            else if(idle_timer_seconds == 3600){
                rgb_matrix_set_colour_range(51, 51, RGB_ORANGE); // RGB Toggle
            }

            if(rgb_timeout_indicator_enabled){
                rgb_matrix_set_colour_range(52, 52, RGB_GREEN); // RGB Timeout Indicator Toggle
            }else{
                rgb_matrix_set_colour_range(52, 52, RGB_RED); // RGB Timeout Indicator Toggle
            }

			rgb_matrix_set_colour_range(64, 66, RGB_RED); // Media
			rgb_matrix_set_colour_range(57, 57, RGB_RED); // Media
			rgb_matrix_set_colour_range(43, 43, RGB_RED); // Media
			rgb_matrix_set_colour_range(14, 14, RGB_RED); // Media

            layer_reset_required = true;
			break;
		case L2:
			// Turn off all lights
			// rgb_matrix_set_colour_range(0, 104, RGB_BLACK);

			rgb_matrix_set_colour_range(67, 81, RGB_GREEN); // Front bar
			rgb_matrix_set_colour_range(1, 12, RGB_GREEN);  // Number keys
			rgb_matrix_set_colour_range(14, 14, RGB_GREEN); // DEL key
			rgb_matrix_set_colour_range(59, 60, RGB_GREEN); // Meta/Alt key

            layer_reset_required = true;
			break;
		case L0:
            if(layer_reset_required){
                layer_reset_required = false;
                rgb_matrix_set_colour_range(0, 104, RGB_BLACK);
            }
            if(rgb_timeout_indicator_enabled){
                float percent_elapsed = (float)(elapsed_seconds + 1) / idle_timer_seconds;
                int steps = 7;
                int to_extinguish = steps * percent_elapsed;

                if(to_extinguish > 0){
                    rgb_matrix_set_colour_range(67, 67 + (to_extinguish - 1), RGB_BLACK); // Front bar
                    rgb_matrix_set_colour_range(81 - (to_extinguish - 1), 81, RGB_BLACK); // Front bar
                }
            }
            break;
		}
}
