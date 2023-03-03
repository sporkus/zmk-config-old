#!/usr/bin/env python3
import os
import yaml
from datetime import datetime

# config. Comment out 'flash' to skip copying to USB
DIR = {
    "zmk": '~/code/zmk'
    ,"zmk_config":  '~/code/zmk-config'
    ,"output":  '~/code/zmk-config/firmware'
    ,"flash":  '/Volumes/XIAO-SENSE'
}

def main():
    try:
        for k, v in DIR.items():
            DIR[k] = os.path.expanduser(v)
        zmk_app = DIR['zmk'] + '/app'
        yaml_path = DIR['zmk_config'] + '/build.yaml'
        if not os.path.exists(DIR['output']):
            os.mkdir(DIR['output'])

        os.chdir(zmk_app)
        keyboards = parse_yaml(yaml_path)
        for kb in keyboards:
            board = kb.get('board')
            shield = kb.get('shield')
            cmd, target = get_build_cmd(board, shield, DIR['zmk_config'], DIR['output'])
            if os.system(cmd) == 0:
                msg(f"Build sucess. Firmware copied to {target}")
                if DIR.get('flash'):
                    copy_to_flash(target)
    except Exception as e:
        print(repr(e))

def get_build_cmd(board, shield, config_dir, outdir):
    if shield is not None:
        shield_flag = " -DSHIELD=" + shield
        shield_nm = f'{shield}-'
    else:
        shield_flag = ''
        shield_nm = ''
    now = datetime.now().strftime('%Y%m%d%H%M')
    output = f'{outdir}/{shield_nm}{board}-{now}.uf2'
    cmd = f'west build -p -b {board} --{shield_flag} -DZMK_CONFIG="{config_dir}/config"'
    cmd2 = f' && cp build/zephyr/zmk.uf2 ' + output

    msg("Starting build script:" + board + shield_nm + "\tOutput target: " + output)
    return cmd + cmd2, output

def copy_to_flash(src):
    target = DIR['flash']
    if target[-1] != '/':
        target += '/'
    cmd = '?'
    while cmd not in ['', 'n']:
        cmd = input(f"\nCopy {src} to {target}?\nEnter to continue, n to skip: ")

    if cmd == 'n':
        print('USB copying skipped...')
        return

    if os.system(f'cp {src} {target}') == 0:
        print(f'copied {src} to {target}')
    else:
        msg('file copy failed')

def parse_yaml(yml_path):
    with open(yml_path) as f:
        y = yaml.safe_load(f)

    if 'include' not in y.keys():
        print('expecting build.yaml with an "include:"')
        return
    return y['include']

def msg(x):
    print('\n'*2 + "-" * 100)
    print(x)
    print("-" * 100 + '\n'*2)

main()
