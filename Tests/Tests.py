# Inspired by https://docs.python.org/3/library/sys.html, https://docs.python.org/3/library/os.html,
# https://docs.python.org/3/library/subprocess.html and https://docs.python.org/3/library/difflib.html

import sys
import os
import subprocess
import difflib

execute_syntax = True
execute_verif = True
execute_gencode = True
display_errors = True

# This function only executes the files and checks for errors
def execute_files(path, flags=""):
    for root, _, files in os.walk(os.path.join(path)):
        print(f"\n########################## {os.path.basename(root)} ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                command = f"./minicc {flags} {file_path}"
                result = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)
                if display_errors:
                    stderrbis = "ERROR - " + result.stderr[:-1] if result.stderr and result.stderr[-1] == '\n' else result.stderr
                else:
                    stderrbis = "ERROR"
                res = "OK" if not result.stderr else stderrbis
                print(f"\t {root}/{file}    \t {res}")


# This function executes the files using minicc and compares the output of the
# mips files with the expected output
# The expected output is stored in a comment block at the beginning of the test file
def execute_out_files(path, flags=""):
    for root, _, files in os.walk(path):
        print(f"\n########################## {os.path.basename(root)} ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                assembly = file_path.replace('.c', '.s')

                compile_command = f"./minicc {flags} {file_path} -o {assembly}"
                compile_result = subprocess.run(compile_command, shell=True, stderr=subprocess.PIPE, text=True)

                compile_result.stderr = compile_result.stderr[:-1] if compile_result.stderr and compile_result.stderr[-1] == '\n' else compile_result.stderr
                if compile_result.stderr and display_errors:
                    print(f"\t{root}/{file}    \t ERROR - {compile_result.stderr}")
                    continue
                elif compile_result.stderr:
                    print(f"\t{root}/{file}    \t ERROR")
                    continue

                execute_command = f"java -jar Tests/mars_4_2.jar nc np ae1 {assembly}"
                execute_result = subprocess.run(execute_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

                execute_result.stderr = execute_result.stderr[:-1] if execute_result.stderr and execute_result.stderr[-1] == '\n' else execute_result.stderr
                if execute_result.stderr and display_errors:
                    print(f"\t{root}/{file}    \t ERROR - {execute_result.stderr}")
                    continue
                elif execute_result.stderr:
                    print(f"\t{root}/{file}    \t ERROR")
                    continue

                expected_output = get_output(file_path)

                if execute_result.stdout.strip() == expected_output.strip():
                    status = "OK"
                else:
                    diff = difflib.unified_diff(execute_result.stdout.splitlines(keepends=True), expected_output.splitlines(keepends=True), fromfile='expected', tofile='got', lineterm='\n')
                    diff_output = ''.join(diff)
                    indented_diff = "ERROR\n" + '\n'.join(['\t\t' + line for line in diff_output.split('\n')])
                    status = indented_diff if display_errors else "ERROR"

                print(f"\t{root}/{file}    \t {status}")

# This function reads the expected output from the test file
# The expected output is stored in a comment block at the beginning of each test file
def get_output(file_path):
    expected_output = []
    with open(file_path, 'r') as file:
        capture = False
        for line in file:
            if line.strip() == "// Expected output:":
                capture = True
            elif capture:
                if line.startswith("//"):
                    expected_output.append(line.strip("//").strip())
                else:
                    break
    return "\n".join(expected_output)


# Main function
# The flags --v, --s and --g are used to disable the execution of the verification, syntax and code generation tests
# Every other argument is passed to minicc
if __name__ == "__main__":

    minicc_flags = []

    for arg in sys.argv[1:]:
        if arg == "--s":
            execute_syntax = False
        elif arg == "--v":
            execute_verif = False
        elif arg == "--g":
            execute_gencode = False
        elif arg == "--e":
            display_errors = False
        else:
            minicc_flags.append(arg)

    flags_str = ' '.join(minicc_flags)

    if execute_syntax:
        execute_files("Tests/Syntaxe", flags_str + " -s ")
    if execute_verif:
        execute_files("Tests/Verif", flags_str + " -v ")
    if execute_gencode:
        execute_out_files("Tests/Gencode", flags_str)
    
    print("\n")
