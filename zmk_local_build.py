#!/usr/bin/env python3

import os
import sys
import yaml
from datetime import datetime

# config
DIR = {
    "zmk": '~/code/zmk'
    ,"zmk_config":  '~/code/zmk-config'
    ,"output":  '~/code/zmk-config/firmware'
}

def get_build_cmd(board, shield, config_dir, outdir):
    if shield is not None:
        shield_flag = " -DSHIELD=" + shield
        shield_nm = f'_{shield}'
    else:
        shield_flag = ''
        shield_nm = ''
    now = datetime.now().strftime('%Y%m%d_%H%M')
    output = f'{outdir}/{board}{shield_nm}_{now}.uf2'
    cmd = f'west build -p -b {board} --{shield_flag} -DZMK_CONFIG="{config_dir}/config"'
    cmd2 = f' && cp build/zephyr/zmk.uf2 ' + output

    print()
    print("-- " * 40)
    print("Starting build script:" + board + shield_nm + "\tOutput target: " + output)
    print("-- " * 40)
    print()
    return cmd + cmd2


try:
    for k, v in DIR.items():
        DIR[k] = os.path.expanduser(v)

    zmk_app = DIR['zmk'] + '/app'
    yaml_path = DIR['zmk_config'] + '/build.yaml'
    west_output = zmk_app + '/build/zephyr/zmk.uf2'

    if not os.path.exists(DIR['output']):
        os.mkdir(DIR['output'])

    with open(yaml_path) as f:
        y = yaml.safe_load(f)

    if 'include' not in y.keys():
        print('expecting build.yaml with an "include:"')
        pass

    os.chdir(zmk_app)
    for kb in y['include']:
        board = kb.get('board')
        shield = kb.get('shield')
        cmd = get_build_cmd(board, shield, DIR['zmk_config'], DIR['output'])

        if os.system(cmd) == 0:
            print("-- " * 40)
            print(f"Build sucess. Firmware copied to {DIR['output']}")
            print("-- " * 40)

except Exception as e:
    print(repr(e))
