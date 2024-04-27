import subprocess
import sys
import shutil

def has_wsl():
    return shutil.which('wsl.exe') is not None

def to_wsl_path(win_path):
    return subprocess.run(['wsl.exe', 'wslpath', '-a', win_path.replace('\\', '/')], capture_output=True, text=True).stdout.strip()

def run_llvm_objdump(path):
    return subprocess.run(
        ['wsl.exe', 'llvm-objdump', '-d', '--no-addresses', '--no-show-raw-insn', '-M', 'intel', '--no-print-imm-hex', path],
        capture_output=True
    ).stdout.strip()

def main():
    if len(sys.argv) < 1:
        print('expected path to .lib object')
        sys.exit(1)
    if len(sys.argv) < 2:
        print('expected list of filecheck files')
        sys.exit(1)

    if not has_wsl():
        print('wsl not found')
        sys.exit(1)

    lib_path = to_wsl_path(sys.argv[1])

    objdump_result = run_llvm_objdump(lib_path)

    files = [to_wsl_path(path) for path in sys.argv[2].split(';')]
    subprocess.run(['wsl.exe', 'FileCheck', *files], input=objdump_result)

if __name__ == '__main__':
    main()
