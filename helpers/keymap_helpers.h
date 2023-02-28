#pragma once

#define STRINGIFY(x) #x

// -------- BEHAVIORS ---------
#define ZMK_BEHAVIOR(name, cell) \
    name: name {\
        label = STRINGIFY(name);\
        #binding-cells = <cell>;

#define SHIFT_MORPH(name, bind1, bind2) \
    ZMK_BEHAVIOR(name, 0)\
        compatible = "zmk,behavior-mod-morph";\
        bindings = <&kp bind1>, <&kp bind2>;\
        mods = <(MOD_LSFT|MOD_RSFT)>;\
    }

#define DOUBLE_TAP(name, tapterm, bind1, bind2) \
    ZMK_BEHAVIOR(name, 0)\
        compatible = "zmk,behavior-tap-dance";\
        bindings = <bind1>, <bind2>;\
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
    	wait-ms = <1>;\
	    bindings = <keys>;\
    }

#define HOLDTAP_(name, bind1, bind2, tapterm, quickterm) \
    ZMK_BEHAVIOR(name, 2)\
        compatible = "zmk,behavior-hold-tap";\
        bindings = <bind1>, <bind2>;\
        tapping-term-ms = <tapterm>;\
        quick-tap-ms = <quickterm>;\

#define HOLDTAP_TAP_PREFFERED(name, bind, tapterm, quickterm) \
    HOLDTAP_(name, bind, &kp, tapterm, quickterm) \
        flavor = "tap-preferred" ;\
        global-quick-tap;\
    }

#define MODTAP_TAP_BALANCED_POSITIONAL(name, tapterm, quickterm, holdkeys) \
    HOLDTAP_(name, &kp, &kp, tapterm, quickterm) \
        flavor = "balanced";\
        hold-trigger-key-positions = <holdkeys>;\
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

