# Utilisé à des fins de debug pour comparer les fichiers de sortie générés par minicc et minicc_ref

import os # Source : https://docs.python.org/3/library/os.html
import subprocess # Source : https://docs.python.org/3/library/subprocess.html
import difflib # Source : https://docs.python.org/3/library/difflib.html

def execute_files(path, flags=""):
    for root, dirs, files in os.walk(os.path.join(path)):
        i : int = 0
        print(f"########################## {os.path.basename(root)} ##########################")
        print(f"########################## Compilation ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                command = f"./minicc -o out{i}.s {flags} {file_path}"
                result1 = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)
                command = f"./minicc_ref -o out_ref{i}.s {flags} {file_path}"
                result2 = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)
                i += 1
                ok = "OK\n"
                print(f"\t{root}/{file}: {result1.stderr or result2.stderr or ok}")

    compare_output_files(root, path, i)

def compare_output_files(root, path, num_files):

    print(f"################################# Comparing ################################\n")

    for j in range(num_files):
        with open(f"out{j}.s", 'r') as file1, open(f"out_ref{j}.s", 'r') as file2:
            file1_lines = file1.readlines()
            file2_lines = file2.readlines()

            diff = list(difflib.unified_diff(file1_lines, file2_lines, fromfile="out{}.s".format(j), tofile="out_ref{}.s".format(j)))
            if diff:
                print(f"\tDifferences for file index {j}:")
                for line in diff:
                    print("\t\t", line, end='')
            else:
                print(f"\tFile index {j}: OK\n")

if __name__ == "__main__":
    execute_files("Tests/Gencode", "-r 4")
