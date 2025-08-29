from tools_core import *
from pathlib import Path
import sys

if __name__ == "__main__":
    srcPaths = list(Path(SRCS_ROOT).rglob("*.*"))
    headerPaths = list(Path(HEADERS_ROOT).rglob("*.*"))

    srcs = [os.path.basename(f) for f in list(Path(SRCS_ROOT).rglob("*.*"))]
    headers = [os.path.basename(f) for f in list(Path(HEADERS_ROOT).rglob("*.*"))]

    print(len(srcs), "source files;")
    print(len(headers), "header files;")

    if len(sys.argv) > 1 and sys.argv[1] == "-l":

        if len(srcs) > 0:
            for s in srcs[:-1]:
                print(s, end = ", ")
            print(srcs[-1])
                
            prefix = SRCS_ROOT + "/"
            lines = [f"#include \"{str(src).removeprefix(prefix)}\"\n" for src in srcPaths]
            with open(f"{SRCS_ROOT}/unity.cpp", "w") as unity:
                unity.writelines(lines)


        if len(headers) > 0:
            for h in headers[:-1]:
                print(h, end = ", ")
            print(headers[-1])