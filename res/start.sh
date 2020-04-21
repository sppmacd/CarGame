#!/bin/bash
CG_ARCH_PREFIX="x64"
"./${CG_ARCH_PREFIX}/cg" "$*"
read -n 1 -s -r -p "Press any key to continue . . ."
