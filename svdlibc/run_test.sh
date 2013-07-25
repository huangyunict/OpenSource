#!/bin/bash

#   locale
export LC_ALL="C"
export LANG="C"
export LANGUAGe="C"

#   verbose
export SAFE_EXECUTE_VERBOSE=1

#   source
source base_func.sh

#   array
declare -a arr_case
arr_case=(
            "case_1"
         )

#   test each case
fix_cmd="./svd"
for ((i=0; i<${#arr_case[@]}; ++i))
do
    case_cmd="${arr_case[$i]}"
    case_prefix=$(echo "${case_cmd}" | sed "s/ .*$//g")
    #   test
    var_opt=$(echo "${case_cmd}" | sed "s/^[^ ]*//")
    fix_opt="testcase/${case_prefix}.input.txt -o testcase/${case_prefix}.output"
    cmd_line="${fix_cmd} ${fix_opt} ${var_opt}"
    echo "========= test for ${case_prefix} ========="
    safe_execute "${cmd_line}"
    for suffix in "S" "Ut" "Vt"
    do
        if diff testcase/${case_prefix}.output-${suffix} testcase/${case_prefix}.ref-${suffix} > testcase/${case_prefix}.diff.txt
        then
            echo "passed matrix ${suffix}"
        else
            echo "diff failed for matrix ${suffix}, please refer to testcase/${case_prefix}.diff.txt for detail"
            exit 1
        fi
    done
    rm testcase/${case_prefix}.diff.txt testcase/${case_prefix}.output-S testcase/${case_prefix}.output-Ut testcase/${case_prefix}.output-Vt
done

exit 0

