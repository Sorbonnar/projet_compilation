# Inspired by https://docs.python.org/3/library/os.html and https://docs.python.org/3/library/subprocess.html

import os
import subprocess

def execute_files(path, flags=""):
    for root, dirs, files in os.walk(os.path.join(path)):
        print(f"########################## {os.path.basename(root)} ##########################")
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                command = f"./minicc {flags} {file_path}"
                result = subprocess.run(command, shell=True, stderr=subprocess.PIPE, text=True)
                ok = "OK\n"
                print(f"\t {root}/{file}: {result.stderr or ok}")

if __name__ == "__main__":
    execute_files("Tests/Syntaxe", "-s")
    execute_files("Tests/Verif", "-v")
    execute_files("Tests/Gencode")
