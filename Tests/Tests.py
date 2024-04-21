# Inspired by https://docs.python.org/3/library/sys.html, https://docs.python.org/3/library/os.html,
# https://docs.python.org/3/library/subprocess.html and https://docs.python.org/3/library/difflib.html

import sys
import os
import subprocess
import difflib

# Variables to control the execution of the tests and the display of the errors and the summary
execute_syntax = True
execute_verif = True
execute_gencode = True
execute_OK = True
execute_KO = True
display_errors = True
display_summary = True

# Variables to count the numbers of OK and KO tests to display a summary at the end of the execution
OK_S_0, OK_S_1, KO_S_0, KO_S_1 = 0, 0, 0, 0
OK_V_0, OK_V_1, KO_V_0, KO_V_1 = 0, 0, 0, 0
OK_G_0, OK_G_1, KO_G_0, KO_G_1 = 0, 0, 0, 0


# This function only executes the files and checks for errors
def execute_files(path, flags=""):
    global OK_S_0, OK_S_1, KO_S_0, KO_S_1, OK_V_0, OK_V_1, KO_V_0, KO_V_1, execute_OK, execute_KO
    for root, _, files in os.walk(os.path.join(path)):
        files = sorted([f for f in files if f.startswith('test') and f.endswith('.c')], key=lambda x: int(x.split('test')[1].split('.c')[0]))
        
        if not execute_OK and os.path.basename(root) == "OK":
            continue
        elif not execute_KO and os.path.basename(root) == "KO":
            continue

        print(f"\n########################## {os.path.basename(root)} ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                command = f"./minicc {flags} {file_path}"
                result = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)

                expected_error_line = 0
                error_line = -1

                with open(file_path, 'r') as filec:
                    first_line = filec.readline().strip()
                    if first_line.startswith("//"):
                        try:
                            expected_error_line = int(first_line[3:].strip())
                        except ValueError:
                            expected_error_line = 0

                error_details = result.stderr.strip()
                if ':' in error_details:
                    parts = error_details.split(':')
                    line_number_part = parts[0].strip()
                    error_line = int(line_number_part.split()[-1]) if line_number_part.split()[-1].isdigit() else -1

                if display_errors and error_line == expected_error_line:
                    stderrbis = "SUCCESS - " + result.stderr[:-1] if result.stderr and result.stderr[-1] == '\n' else result.stderr
                elif error_line == expected_error_line:
                    stderrbis = "SUCCESS"
                elif display_errors:
                    stderrbis = "ERROR - " + result.stderr[:-1] if result.stderr and result.stderr[-1] == '\n' else result.stderr
                else:
                    stderrbis = "ERROR"

                if os.path.basename(root) == "OK":
                    if path == "Tests/Syntaxe":
                        OK_S_0 += (1 if not result.stderr else 0)
                        OK_S_1 += (1 if result.stderr else 0)
                    elif path == "Tests/Verif":
                        OK_V_0 += (1 if not result.stderr else 0)
                        OK_V_1 += (1 if result.stderr else 0)
                        
                elif os.path.basename(root) == "KO":
                    if path == "Tests/Syntaxe":
                        KO_S_0 += (1 if result.stderr and error_line == expected_error_line else 0)
                        KO_S_1 += (1 if not result.stderr or error_line != expected_error_line else 0)
                    elif path == "Tests/Verif":
                        KO_V_0 += (1 if result.stderr and error_line == expected_error_line else 0)
                        KO_V_1 += (1 if not result.stderr or error_line != expected_error_line else 0)

                res = "SUCCESS" if not result.stderr else stderrbis
                print(f"\t {root}/{file}    \t {res}")


# This function executes the files using minicc and compares the output of the
# mips files with the expected output
# The expected output is stored in a comment block at the beginning of the test file
def execute_out_files(path, flags=""):
    global OK_G_0, OK_G_1, KO_G_0, KO_G_1, execute_OK, execute_KO
    for root, _, files in os.walk(path):
        files = sorted([f for f in files if f.startswith('test') and f.endswith('.c')], key=lambda x: int(x.split('test')[1].split('.c')[0]))

        if not execute_OK and os.path.basename(root) == "OK":
            continue
        elif not execute_KO and os.path.basename(root) == "KO":
            continue

        print(f"\n########################## {os.path.basename(root)} ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                assembly = file_path.replace('.c', '.s')

                compile_command = f"./minicc -o {assembly} {flags} {file_path}"
                compile_result = subprocess.run(compile_command, shell=True, stderr=subprocess.PIPE, text=True)

                compile_result.stderr = compile_result.stderr[:-1] if compile_result.stderr and compile_result.stderr[-1] == '\n' else compile_result.stderr
                if compile_result.stderr and display_errors:
                    print(f"\t{root}/{file}    \t SUCCESS - {compile_result.stderr}")
                    continue
                elif compile_result.stderr:
                    print(f"\t{root}/{file}    \t SUCCESS")
                    continue

                execute_command = f"java -jar Tests/mars_4_2.jar nc np {assembly}"
                execute_result = subprocess.run(execute_command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

                execute_result.stderr = execute_result.stderr[:-1] if execute_result.stderr and execute_result.stderr[-1] == '\n' else execute_result.stderr
                
                if execute_result.stderr and display_errors:
                    print(f"\t{root}/{file}    \t SUCCESS - {execute_result.stderr}")
                    if os.path.basename(root) == "OK":
                        OK_G_1 += 1
                    elif os.path.basename(root) == "KO":
                        KO_G_1 += 1
                    continue

                elif execute_result.stderr:
                    print(f"\t{root}/{file}    \t SUCCESS")
                    if os.path.basename(root) == "OK":
                        OK_G_1 += 1
                    elif os.path.basename(root) == "KO":
                        KO_G_1 += 1
                    continue

                expected_output = get_output(file_path)
                expected_output = expected_output

                diff = difflib.unified_diff(execute_result.stdout.splitlines(keepends=True), expected_output.splitlines(keepends=True), fromfile='expected', tofile='got', lineterm='\n')
                diff_output = ''.join(diff)

                if (execute_result.stdout.strip() == expected_output.strip() and os.path.basename(root) == "OK") or (execute_result.stdout.strip() != expected_output.strip() and os.path.basename(root) == "KO"):
                    indented_diff = "SUCCESS\n" + '\n'.join(['\t\t' + line for line in diff_output.split('\n')])
                    status = indented_diff if display_errors and os.path.basename(root) == "KO" else "SUCCESS"

                    if os.path.basename(root) == "OK":
                        OK_G_0 += 1
                    elif os.path.basename(root) == "KO":
                        KO_G_0 += 1
                else:
                    indented_diff = "ERROR\n" + '\n'.join(['\t\t' + line for line in diff_output.split('\n')])
                    status = indented_diff if display_errors else "ERROR"

                    if os.path.basename(root) == "OK":
                        OK_G_1 += 1
                    elif os.path.basename(root) == "KO":
                        KO_G_1 += 1

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

def get_help():
    print("Usage: python3 Tests/Tests.py [options] [minicc_flags]\n")
    print("Options:")
    print("  --s\t\tDisable syntax tests")
    print("  --v\t\tDisable verification tests")
    print("  --g\t\tDisable code generation tests")
    print("  --e\t\tDisable error details display")
    print("  --ko\t\tDisable KO tests")
    print("  --ok\t\tDisable OK tests")
    print("  --help\tDisplay help")
    print("  --h\t\tDisplay minicc help")
    print("  --b\t\tDisplay minicc banner\n")
    print("\nRules:")
    print("  Every other argument is passed to minicc, use --h to display minicc help")
    print("  Arguments --help, --h and --b will stop the execution of the script")
    print("  Arguments can be passed in any order, including minicc arguments")
    print("  If no argument is passed, all tests are enabled\n")

def get_summary():
    print("\n########################## SUMMARY ##########################")
    print("\tTotal tests executed\t\t", OK_S_0 + OK_S_1 + KO_S_0 + KO_S_1 + OK_V_0 + OK_V_1 + KO_V_0 + KO_V_1 + OK_G_0 + OK_G_1 + KO_G_0 + KO_G_1)
    print("\tTotal tests passed\t\t", OK_S_0 + OK_V_0 + OK_G_0 + KO_S_0 + KO_V_0 + KO_G_0)
    print("\tTotal tests failed\t\t", OK_S_1 + OK_V_1 + OK_G_1 + KO_S_1 + KO_V_1 + KO_G_1)
    print("\tTotal tests in OK\t\t", OK_S_1 + OK_V_1 + OK_G_1 + OK_S_0 + OK_V_0 + OK_G_0)
    print("\tTotal tests in KO\t\t", KO_S_1 + KO_V_1 + KO_G_1 + KO_S_0 + KO_V_0 + KO_G_0, "\n")

    if display_errors:
        print("\tErrors displayed\n")
    else:
        print("\tErrors not displayed\n")

    if execute_syntax:
        print(f"\tSyntax tests\t\t\t {KO_S_0} passed and {KO_S_1} failed in KO")
        print(f"\t\t\t\t\t {OK_S_0} passed and {OK_S_1} failed in OK\n")
    else:
        print("\tSyntax tests\t\t\t Disabled\n")
    if execute_verif:
        print(f"\tVerification tests\t\t {KO_V_0} passed and {KO_V_1} failed in KO")
        print(f"\t\t\t\t\t {OK_V_0} passed and {OK_V_1} failed in OK\n")
    else:
        print("\tVerification tests\t\t Disabled\n")
    if execute_gencode:
        print(f"\tCode generation tests\t\t {KO_G_0} passed and {KO_G_1} failed in KO")
        print(f"\t\t\t\t\t {OK_G_0} passed and {OK_G_1} failed in OK\n")
    else:
        print("\tCode generation tests\t\t Disabled\n")
    print("\tNote: SUCCESS means that the test passed and ERROR means that the test had an unintended behavior")
    print("\t      successful tests in OK should return no errors while successful tests in KO should return errors")
    print("\t      failed tests in KO should return the wrong line or no errors")
    print("##############################################################\n")


# Main function
# The flags --v, --s and --g are used to disable the execution of the verification, syntax and code generation tests
# The flags --e, --ko and --ok are used to disable the display of errors, KO tests and OK tests
# The flag --r is used to disable the display of the summary at the end of the execution
# The flag --help displays the help message
# The flag --h displays the minicc help message and the flag --b displays the minicc banner
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
        elif arg == "--ko":
            execute_KO = False
        elif arg == "--ok":
            execute_OK = False
        elif arg == "--help":
            get_help()
            exit(0)
        elif arg == "--h":
            os.system("./minicc -h")
            exit(0)
        elif arg == "--b":
            os.system("./minicc -b")
            exit(0)
        elif arg == "--r":
            display_summary = False
        else:
            minicc_flags.append(arg)

    flags_str = ' '.join(minicc_flags)

    if (not execute_syntax and not execute_verif and not execute_gencode) or (not execute_OK and not execute_KO):
        print("\nNo tests to execute\n")
        get_help()
        exit(0)

    if execute_syntax:
        execute_files("Tests/Syntaxe", flags_str + " -s ")
    if execute_verif:
        execute_files("Tests/Verif", flags_str + " -v ")
    if execute_gencode:
        execute_out_files("Tests/Gencode", flags_str)
    if display_summary:
        get_summary()
