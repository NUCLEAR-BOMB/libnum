import sys
import subprocess
import glob
import re

def parse_functions_disassembly(string):
    functions = {}
    while True:
        name_start = string.find('<')
        if name_start == -1: break
        name_end = string.find('>', name_start + 1)
        if name_end == -1:
            print('Internal error: closing > is not found')
            sys.exit(1)
        name = string[name_start+1:name_end]

        string = string[name_end + 3:] # skip >:\n
        insts = []
        while len(string) != 0 and string[0] != '\n':
            stripped_str = string.lstrip()
            inst_end = stripped_str.find('\n')
            inst_str = stripped_str[:inst_end]
            insts.append((inst_str[:inst_str.find('#')] if '#' in inst_str else inst_str).rstrip().replace('\t', ' '))

            string = stripped_str[inst_end+1:]
        string = string[1:] # skip \n\n
        
        functions[name] = insts

    return functions

def parse_functions_disassembly_check(string):
    def strip_comment(raw_str):
        if ';' in raw_str:
            return raw_str[:raw_str.find(';')].strip()
        else:
            return raw_str.strip()

    functions = {}

    while True:
        name_start = string.find('{')
        if name_start == -1: break
        name_end = string.find('}', name_start + 1)
        if name_end == -1:
            print('Error: closing } is not found')
            sys.exit(1)
        fn_name, *tags = string[name_start + 1:name_end].split(':')

        string = string[name_end + 1:]
        expected_insts = []
        while len(string) != 0:
            inst_string = string.lstrip()
            inst_end = inst_string.find('\n')
            inst = inst_string[:inst_end]

            if '{' in inst and '}' in inst:
                break

            expected_insts.append(strip_comment(inst))

            string = inst_string[inst_end + 1:]

        functions[fn_name] = (tags, expected_insts)

    return functions

def find_nth(haystack, needle, n):
    start = haystack.find(needle)
    while start >= 0 and n > 1:
        start = haystack.find(needle, start + len(needle))
        n -= 1
    return start

def rreplace(string, old, new, count):
    return string[::-1].replace(old[::-1], new[::-1], count)[::-1]

def strip_disassembly_info(string):
    info_end = find_nth(string, '\n', 5)
    if info_end == -1:
        print('Error: disassembly header info is too short')
        sys.exit(1)
    return string[info_end:]

def disassembly(file_path):
    LLVM_OBJDUMP = 'llvm-objdump'
    extra_args = ['-M', 'intel', '--no-addresses', '--no-show-raw-insn', '--no-print-imm-hex']
    try:
        result = subprocess.run([LLVM_OBJDUMP, '-d', *extra_args, file_path], capture_output=True, text=True)
    except FileNotFoundError:
        print(f'Error: tool "{LLVM_OBJDUMP}" is not found. unable to disassemble file')
        sys.exit(1)

    if (code := result.returncode) != 0:
        print(f'llvm-objdump failed with code: {code}.\nstderr: {result.stderr}')
        sys.exit(1)

    return strip_disassembly_info(result.stdout)

def find_check_files():
    return list(glob.glob('*.check'))

def verify_check_file(actual_disasm, expected_disasm, tags):
    def apply_pattern_matching(string):
        patched = re.escape(string).replace('\\*', '.*')
        while True:
            before, itself, after = patched.partition('\\|')
            if itself == '': break
            before = rreplace(before, '\\ ', ' (', 1) if '\\ ' in before else '(' + before
            after = after.replace('\\ ', ') ', 1) if '\\ ' in after else after + ')'

            patched = before + '|' + after
        return patched

    for fn_name, (fn_tags, fn_insts) in expected_disasm.items():
        actual_insts = actual_disasm.pop(fn_name, None)
        if actual_insts is None:
            print(f'Error: unknown function "{fn_name}"')
            sys.exit(1)

        if not any(fn_tag in tags for fn_tag in fn_tags) and len(fn_tags) != 0:
            continue
        
        if len(actual_insts) > len(fn_insts):
            print(f'Error: expected disassembly function is too short. {len(actual_insts)} (actual) > {len(fn_insts)} (expected). function: "{fn_name}"')
            print(f'\nFull disassembly of function "{fn_name}":\n\t{'\n\t'.join(actual_insts)}')
            sys.exit(1)
        elif len(actual_insts) < len(fn_insts):
            print(f'Error: expected disassembly function is too long. {len(actual_insts)} (actual) < {len(fn_insts)} (expected). function: "{fn_name}"')
            print(f'\nFull disassembly of function "{fn_name}":\n\t{'\n\t'.join(actual_insts)}')
            sys.exit(1)

        for actual_inst, expected_inst in zip(actual_insts, fn_insts):
            if not re.match(apply_pattern_matching(expected_inst), actual_inst):
                print(f'Error: mismatch at function: "{fn_name}"')
                print(f'Expected:\n\t{expected_inst}\nGot:\n\t{actual_inst}')
                print(f'\nFull disassembly of function "{fn_name}":\n\t{'\n\t'.join(actual_insts)}')
                sys.exit(1)

def main():
    if len(sys.argv) <= 1:
        print('Error: expected path to library file as first argument')
        sys.exit(1)

    tags = [] if len(sys.argv) <= 2 else sys.argv[2].split(':')

    target_path = sys.argv[1]
    disasm_db = parse_functions_disassembly(disassembly(target_path))

    check_files = find_check_files()
    for check_file in check_files:
        with open(check_file, 'r') as f:
            verify_check_file(disasm_db, parse_functions_disassembly_check(f.read()), tags)

    if len(disasm_db) != 0:
        remaining = [f'"{name}"' for name in disasm_db]
        print(f'Error: not full coverage of check files. remaining functions: {', '.join(remaining)}')
        sys.exit(1)

if __name__ == '__main__':
    main()
