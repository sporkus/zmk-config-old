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

#define NO_GLOBAL_QT
#define GLOBAL_QT global-quick-tap;
#define TAP_PREFERRED flavor = "tap-preferred";
#define HOLD_PREFERRED flavor = "hold-preferred";
#define BALANCED flavor = "balanced";
#define HOLDTAP_CORE(name, b1, b2, tapterm, quickterm, global_quick_tap) \
    ZMK_BEHAVIOR(name, 2)\
        compatible = "zmk,behavior-hold-tap";\
        bindings = <b1>, <b2>;\
        tapping-term-ms = <tapterm>;\
        quick-tap-ms = <quickterm>;\
        global_quick_tap

#define HOLDTAP_TAP(name, b1, b2, tapterm, quickterm, global_quick_tap) \
    HOLDTAP_CORE(name, b1, b2, tapterm, quickterm, global_quick_tap) \
    TAP_PREFERRED\
    }

#define HOLDTAP_BAL(name, b1, b2, tapterm, quickterm, global_quick_tap) \
    HOLDTAP_CORE(name, b1, b2, tapterm, quickterm, global_quick_tap) \
    BALANCED\
    }

#define MODTAP_POSITIONAL_BAL(name, b1, b2, tapterm, quickterm, global_quick_tap, holdkeys) \
    HOLDTAP_CORE(name, b1, b2, tapterm, quickterm, global_quick_tap) \
        hold-trigger-key-positions = <holdkeys>;\
        BALANCED\
    }


// -------- COMBOS ---------
#define COMBO_ALL 0xff 
#define COMBO(name, keypress, keypos, term, lay) \
combo_##name {\
    timeout-ms = <term>;\
    bindings = <keypress>;\
    key-positions = <keypos>;\
    layers = <lay>;\
}

// -------- CONDITIONAL_LAYERS ---------
#define TRI_LAYER(name, ifs, thens) \
    tri_layer_##name {\
        if-layers = <ifs>;\
        then-layer = <thens>;\
    }

