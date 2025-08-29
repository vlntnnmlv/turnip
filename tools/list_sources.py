from tools_core import *
from pathlib import Path
import sys

if __name__ == "__main__":
    srcs = [os.path.basename(f) for f in list(Path(SRCS_ROOT).rglob("*.*"))]
    headers = [os.path.basename(f) for f in list(Path(HEADERS_ROOT).rglob("*.*"))]

    print(len(srcs), "source files;")
    print(len(headers), "header files;")

    if len(sys.argv) > 1 and sys.argv[1] == "-l":

        if len(srcs) > 0:
            for s in srcs[:-1]:
                print(s, end = ", ")
            print(srcs[-1])

        if len(headers) > 0:
            for h in headers[:-1]:
                print(h, end = ", ")
            print(headers[-1])