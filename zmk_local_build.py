#!/usr/bin/env python3
"""ZMK local build script

A simple script to build ZMK firmware from your local zmk-config and zmk folders.
After building, the firmware will be copied to an output folder with branch name and timestamp.
If specified, it will also copy the firmare to your usb device.
You can also run it as a git hook.
"""
import os
import yaml
import logging
from time import sleep
from datetime import datetime

logger = logging.getLogger(__name__)

# config. Comment out 'flash' to skip copying to USB
PATHS = {
    "zmk": '~/code/zmk'
    ,"zmk_config":  '~/code/zmk-config'
    ,"output":  '~/code/zmk-config/firmware'
    ,"flash":  '/Volumes/RPI-RP2'
}
ZEPHYR_TOOLCHAIN_VARIANT='zephyr'

def main():
    try:
        process_dirs(PATHS)
        os.environ['ZEPHYR_TOOLCHAIN_VARIANT'] = ZEPHYR_TOOLCHAIN_VARIANT
        os.chdir(PATHS['zmk_config'])
        git_branch = get_git_branch()

        os.chdir(PATHS['zmk_app'])
        keyboards = parse_yaml(PATHS['yaml_path'])
        for kb in keyboards:
            kb.update(zmk_build(kb, git_branch))

        msg('Build summary:')
        print(keyboards)
    except Exception as e:
        logger.exception(e)

def zmk_build(kb: dict, git_branch:str) -> dict:
    # kb should have at least a value for 'board', 'shield' is optional
    board = kb.get('board')
    shield = kb.get('shield')
    if board is None:
        logger.warning("No 'board' found. Check build.yaml")

    build_status = {}
    cmd, target = get_build_cmd(board, shield, PATHS['zmk_config'], PATHS['output'], git_branch)
    if os.system(cmd) == 0:
        msg(f"Build sucess. Firmware copied to {target}")
        build_status['build'] = 'success'
        if PATHS.get('flash'):
            build_status['flash_to_usb'] = copy_to_flash(target)
        return build_status
    else:
        return {'build':'failed'}

def process_dirs(config:dict) -> None:
    for k, v in PATHS.items():
        PATHS[k] = os.path.expanduser(v)

    PATHS.update({
        'zmk_app': PATHS['zmk'] + '/app'
        ,'yaml_path': PATHS['zmk_config'] + '/build.yaml'
    })
    if not os.path.exists(PATHS['output']):
        os.mkdir(PATHS['output'])

def get_git_branch() -> str:
    try:
        from git import Repo
        b = Repo().active_branch.name
        if b in ['master', 'main']:
            return ''
        else:
            return f"[{b}]"
    except:
        return ''

def get_build_cmd(board:str, shield:str, config_dir:str, outdir:str, git_branch:str) -> tuple:
    if shield is not None:
        shield_flag = " -DSHIELD=" + shield
        shield_nm = f'{shield}-'
    else:
        shield_flag = ''
        shield_nm = ''
    now = datetime.now().strftime('%Y%m%d%H%M')
    output = f'{outdir}/{shield_nm}{board}{git_branch}-{now}.uf2'
    cmd = f'west build -c -p -b {board} --{shield_flag} -DZMK_CONFIG="{config_dir}/config"'
    cmd += f' && cp build/zephyr/zmk.uf2 ' + output

    msg(f"Starting build script: shield={shield} board={board} \tOutput target: {output}")
    return cmd, output

def copy_to_flash(src:str) -> str:
    target = PATHS['flash']
    if target[-1] != '/':
        target += '/'
    cmd = '?'
    try:
        while cmd not in ['', 'n']:
            cmd = input(f"\nCopy {src} to {target}?\nEnter to continue, n to skip: ")
    except EOFError:
        # Skipping when stdin is not available when running from scripts
        cmd = 'n'

    if cmd == 'n':
        print('USB copying skipped...')
        return 'skipped'

    boot_ready = os.path.exists(PATHS['flash'])
    if not boot_ready:
        print('waiting for usb bootloader', end='')
        while not boot_ready:
            print('.', end='', flush=True)
            sleep(1)
            boot_ready = os.path.exists(PATHS['flash'])
        print(PATHS['flash'] + ' ready')
        sleep(1)  # can't write to usb when it first pops up

    if os.system(f'cp {src} {target}') == 0:
        print(f'copied {src} to {target}')
        return 'copied'
    else:
        msg('file copy failed')
        return 'failed'

def parse_yaml(yml_path:str) -> dict:
    with open(yml_path) as f:
        y = yaml.safe_load(f)

    if 'include' not in y.keys():
        print('expecting build.yaml with an "include:"')
        return
    return y['include']

def msg(x:str) -> None:
    print('\n'*2 + "-" * 100)
    print(x)
    print("-" * 100 + '\n'*2)

main()
