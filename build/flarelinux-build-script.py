# Builds flarelinux userspace using buildlists.json and components.json
# Licensed under GNU GPLv3, see COPYING for more infomation

import sys
import os
import json
import io
import subprocess

# default values for options
target_compiler_c = "gcc"
target_compiler_cpp = "g++"
output_folder = "./flarelinux-build"
output_log_file = "./logs/build.log"


BUILD_SUCSESS = 0
BUILD_FAILED = 1

def build(log: io.FileIO, file: dict) -> int:
    language = file.get("language", None)
    comp = None
    if language == None:
        log.write(f"[ ERROR ] at {file.get("path")}: Language not specified, exiting build task.\n")
        return BUILD_FAILED
    elif language == "c":
        comp = target_compiler_c
    elif language == "c++":
        comp = target_compiler_cpp
    if comp == None:
        log.write(f"[ ERROR ] at {file.get("path")}: Unable to get compiler from language (lang specified '{language}'), exiting build task.\n")
        return BUILD_FAILED

    log.write(f"{comp.upper()} {file.get("file")} >> {file.get("output")}\n")
    if file.get("comp-out", None) == "executable":
        result: subprocess.CompletedProcess = subprocess.run(
            [
                comp,
                file.get("file"),
                "-o",
                f"{output_folder}/{file.get("output")}",
                "-static" if file.get("buildstatic") == True else ""
            ],
            capture_output=True,
            text=True
        )
        if not result.returncode == 0:
            log.write(f"[ COMPILER ERROR ]: Compiler exited with code of 1...\nDumping output below \n\n{result.stderr}\n")
            return BUILD_FAILED
    elif file.get("comp-out", None) == "shared-lib":
        objectin = file.get("file").split(".")
        objectin[-1] = "o"
        objectin = ".".join(objectin)
        result: subprocess.CompletedProcess = subprocess.run(
            [
                target_compiler_c,
                "-c",
                "-Wall", 
                "-Werror",
                "-fpic",
                file.get("file"),
                "-o",
                objectin
            ],
            capture_output=True,
            text=True
        )
        if not result.returncode == 0:
            log.write(f"[ COMPILER ERROR ]: Compiler exited with code of 1...\nDumping output below \n\n{result.stderr}\n")
            return BUILD_FAILED

        result: subprocess.CompletedProcess = subprocess.run(
            [
                target_compiler_c,
                "-shared",
                objectin,
                "-o",
                f"{output_folder}/{file.get("output")}"
            ],
            capture_output=True,
            text=True
        )
        if not result.returncode == 0:
            log.write(f"[ COMPILER ERROR ]: Compiler exited with code of 1...\nDumping output below \n\n{result.stderr}\n")
            return BUILD_FAILED
    return BUILD_SUCSESS



def main() -> int:
    errors = 0
    with open("./build/buildlists.json") as file:
        buildlists: dict = json.load(file)
    logfile = open(output_log_file, "a")

    logfile.write(f"Started compilation of {buildlists.get("build")} version {buildlists.get("version")}\n\n")

    modules = buildlists.get("buildfiles")
    for i in modules:
        logfile.write(f"MODULE: {i.get("module")} | VERSION: {i.get("version")}\n")

        files = i.get("sources")
        for file in files:
            result = build(logfile, file)
            if result == BUILD_FAILED:
                logfile.write("\nBuild exited with status of 1\n")
                errors += 1
        logfile.write(f"\n\nSucsessfuly compiled with {errors} errors")

if __name__ == "__main__":
    main()