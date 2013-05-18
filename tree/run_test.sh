#!/bin/bash

export LC_ALL="C"
export LANG="C"
export LANGUAGE="C"

source base_func.sh

safe_execute "./test > out.txt"

if diff out.txt ref.txt > diff.txt
then
    echo "passed" 1>&2
else
    echo "failed, check diff.txt" 1>&2
    exit 1
fi

safe_rm "out.txt"
safe_rm "diff.txt"

exit 0

