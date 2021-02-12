#include QMK_KEYBOARD_H

enum alt_keycodes {
    MD_BOOT,               //Restart into bootloader after hold timeout
};

#define TG_NKRO MAGIC_TOGGLE_NKRO //Toggle 6KRO / NKRO mode

keymap_config_t keymap_config;

enum LAYERS {
    L0 = 0,
    L1,
    L2
};

enum RGB_STATUS {
    ALL_ON = 0,
    NO_UNDERGLOW,
    NO_KEYBOARD,
	ALL_OFF
};

int RGB_state = ALL_ON;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[L0] = 	LAYOUT_65_ansi_blocker(
		KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, 
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, 
		KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_PGUP,
		KC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC, KC_UP, KC_PGDN,
		KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, MO(2), MO(1), KC_LEFT, KC_DOWN, KC_RGHT),
	[L1] = LAYOUT_65_ansi_blocker(
		KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_MUTE,
		KC_TRNS, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, RGB_RMOD, RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU,
		KC_TRNS, RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, MD_BOOT, NK_TOGG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT),
	[L2] = LAYOUT_65_ansi_blocker(
		KC_TRNS, KC_P1, KC_P2, KC_P3, KC_P4, KC_P5, KC_P6, KC_P7, KC_P8, KC_P9, KC_P0, KC_PMNS, KC_PPLS, KC_TRNS, KC_NLCK, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, LAG_NRM, LAG_SWP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
    rgblight_mode(RGB_MATRIX_SOLID_REACTIVE);				// Initially in the fade mode.
	rgb_matrix_sethsv(HSV_CYAN);
	rgb_matrix_decrease_speed();
	rgb_matrix_decrease_speed();
	rgb_matrix_decrease_speed();
	// rgblight_mode(37); // 37 is the first custom matrix
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

};

void rgb_matrix_set_colour_range(uint8_t start, uint8_t finish, uint8_t r, uint8_t g, uint8_t b){
	uint8_t index = start;
	while(index <= finish){
			rgb_matrix_set_color(index, r, g, b);
		index++;
	}
}

#define RGB_BLACK 0x00, 0x00, 0x00



#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
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
				switch (RGB_state) {
                case ALL_ON: {
					RGB_state = NO_KEYBOARD;
                  }
                  break;
                case NO_KEYBOARD: {
                    RGB_state = NO_UNDERGLOW;
                  }
                  break;
                case NO_UNDERGLOW: {
                    RGB_state = ALL_OFF;
                  }
                  break;
                default: {
                    RGB_state = ALL_ON;
                  }
                  break;
              }
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}


void rgb_matrix_indicators_user(void)
{
    switch (biton32(layer_state)) {
		case L1:
			// Turn off all lights except the middle row to see RGB changes :)
			rgb_matrix_set_colour_range(0, 29, RGB_BLACK);
			rgb_matrix_set_colour_range(44, 104, RGB_BLACK);

			
			// rgb_matrix_set_colour_range(67, 81, RGB_ORANGE); // Front bar
			rgb_matrix_set_colour_range(0, 12, RGB_CYAN);  // Number keys
			rgb_matrix_set_colour_range(49, 49, RGB_RED); // Reset
			rgb_matrix_set_colour_range(50, 50, RGB_CORAL); // NKRO

			rgb_matrix_set_colour_range(64, 66, RGB_WHITE); // Media
			rgb_matrix_set_colour_range(57, 57, RGB_WHITE); // Media
			rgb_matrix_set_colour_range(43, 43, RGB_WHITE); // Media
			rgb_matrix_set_colour_range(14, 14, RGB_WHITE); // Media
			break;
		case L2:
			// Turn off all lights
			rgb_matrix_set_colour_range(0, 104, RGB_BLACK);

			// rgb_matrix_set_colour_range(67, 81, RGB_GREEN); // Front bar
			rgb_matrix_set_colour_range(1, 12, RGB_CYAN);  // Number keys
			rgb_matrix_set_colour_range(14, 14, RGB_CYAN); // DEL key
			rgb_matrix_set_colour_range(59, 60, RGB_RED); // Meta/Alt key
			break;
		case L0:
			switch (RGB_state) {
                case ALL_ON: {
                    
                  }
                  break;
                case NO_KEYBOARD: {
					// rgb_matrix_set_color_all(0, 0, 0);
                    rgb_matrix_set_colour_range(0, 29, RGB_BLACK);
					rgb_matrix_set_colour_range(31, 66, RGB_BLACK);
					if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
					    rgb_matrix_set_color(30, 0xAA, 0xAA, 0xAA);
					}else{
						rgb_matrix_set_color(30, 0x00, 0x00, 0x00);
					}
                  }
                  break;
                case NO_UNDERGLOW: {
					// rgb_matrix_set_color_all(0, 0, 0);
                    rgb_matrix_set_colour_range(67, 104, RGB_BLACK);
                  }
                  break;
                default: {
					rgb_matrix_set_colour_range(0, 29, RGB_BLACK);
					rgb_matrix_set_colour_range(31, 104, RGB_BLACK);
					if (IS_HOST_LED_ON(USB_LED_CAPS_LOCK)) {
					    rgb_matrix_set_color(30, 0xAA, 0xAA, 0xAA);
					}else{
						rgb_matrix_set_color(30, 0x00, 0x00, 0x00);
					}
                  }
                  break;
              }
		}
		
	
}











































// led_instruction_t led_instructions[] = {
// 	// Layer 0
// 	{ .flags =  LED_FLAG_USE_ROTATE_PATTERN | LED_FLAG_MATCH_LAYER, .layer = 0 },

// 	// Layer 1
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     , .layer = 1, .id0 = 8191,       							.r = 0,   .g = 166, .b = 180 }, 		// Top row F keys
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     , .layer = 1, .id0 = 2149515264, .id1 = 8207, 				.r = 0,   .g = 255, .b = 0   }, 		// RGB Controls
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     , .layer = 1, .id1 = 131072,     							.r = 255, .g = 0,   .b = 0   }, 		// Reset
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     , .layer = 1, .id0 = 33554432,   .id1 = 262144,     		.r = 255, .g = 100, .b = 100 }, 		// NKRO + prt scr
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     , .layer = 1, .id0 = 16384, .id1 = 33556480, .id2 = 7,  	.r = 255, .g = 165, .b = 0 }, 			// Media

// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_PATTERN , .layer = 1, .id2 = 262136, .pattern_id = 9 },	// Bottom LEDs


// 	// Layer 2


// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     	, .layer = 2, .id0 = 24575,     .r = 0,   .g = 166, .b = 180 }, 		// Numkeys
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_RGB     	, .layer = 2, .id1 = 402653184, .r = 255, .g = 0,   .b = 0 }, 			// Meta swap
// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_PATTERN     , .layer = 2, .id0 = 268402688, .pattern_id = 8 }, 						// Media

// 	{ .flags =  LED_FLAG_MATCH_ID | LED_FLAG_MATCH_LAYER | LED_FLAG_USE_PATTERN 	, .layer = 2, .id2 = 262136, 	.pattern_id = 9 },					// Bottom LEDs

//     { .end = 1 }
// };
