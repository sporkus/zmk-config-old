#pragma once

#define STRINGIFY(x) #x

// -------- BEHAVIORS ---------
#define ZMK_BEHAVIOR(name, cell) \
    name: name {\
        label = STRINGIFY(name);\
        #binding-cells = <cell>;

#define SHIFT_MORPH(name, b1, b2) \
    ZMK_BEHAVIOR(name, 0)\
        compatible = "zmk,behavior-mod-morph";\
        bindings = <b1>, <b2>;\
        mods = <(MOD_LSFT|MOD_RSFT)>;\
    }

#define DOUBLE_TAP(name, tapterm, b1, b2) \
    ZMK_BEHAVIOR(name, 0)\
        compatible = "zmk,behavior-tap-dance";\
        bindings = <b1>, <b2>;\
        tapping-term-ms = <tapterm>;\
    }

#define TRIPLE_TAP(name, tapterm, b1, b2, b3) \
    ZMK_BEHAVIOR(name, 0)\
        compatible = "zmk,behavior-tap-dance";\
        bindings = <b1>, <b2>, <b3>;\
        tapping-term-ms = <tapterm>;\
    }

#define STICKY(name, bind, timeout) \
    ZMK_BEHAVIOR(name, 1)\
        compatible = "zmk,behavior-sticky-key";\
        bindings = <bind>;\
        release-after-ms = <timeout>;\
        quick-release;\
    }

#define MACRO(name, keys) \
    ZMK_BEHAVIOR(name, 0)\
	    compatible = "zmk,behavior-macro";\
    	tap-ms = <1>;\
    	wait-ms = <30>;\
	    bindings = <keys>;\
    }

#define NO_GLOBAL_QT 0
#define TAP_PREFERRED flavor = "tap-preferred";
#define HOLD_PREFERRED flavor = "hold-preferred";
#define BALANCED flavor = "balanced";
#define HOLDTAP_CORE(name, b1, b2, tapterm, global_qt_ms) \
    ZMK_BEHAVIOR(name, 2)\
        compatible = "zmk,behavior-hold-tap";\
        bindings = <b1>, <b2>;\
        tapping-term-ms = <tapterm>;\
        quick-tap-ms = <250>;\
        global-quick-tap-ms = <global_qt_ms>;

#define HOLDTAP_TAP(name, b1, b2, tapterm, global_qt_ms) \
    HOLDTAP_CORE(name, b1, b2, tapterm, global_qt_ms) \
    TAP_PREFERRED\
    }

#define HOLDTAP_BAL(name, b1, b2, tapterm, global_qt_ms) \
    HOLDTAP_CORE(name, b1, b2, tapterm, global_qt_ms) \
    BALANCED\
    }

#define MODTAP_POSITIONAL_BAL(name, b1, b2, tapterm, global_qt_ms, holdkeys) \
    HOLDTAP_CORE(name, b1, b2, tapterm, global_qt_ms) \
        hold-trigger-key-positions = <holdkeys>;\
        hold-trigger-on-release;\
        BALANCED\
    }


// -------- COMBOS ---------
#define COMBO_ALL 0xff
#define COMBO(name, keypress, keypos, term, global_qt_ms, lay) \
combo_##name {\
    timeout-ms = <term>;\
    bindings = <keypress>;\
    key-positions = <keypos>;\
    global-quick-tap-ms = <global_qt_ms>;\
    layers = <lay>;\
}

// -------- CONDITIONAL_LAYERS ---------
#define TRI_LAYER(name, ifs, thens) \
    tri_layer_##name {\
        if-layers = <ifs>;\
        then-layer = <thens>;\
    }

