#!/bin/bash

#   locale
export LC_ALL="C"
export LANG="C"
export LANGUAGE="C"

#   verbose
export SAFE_EXECUTE_VERBOSE=0

#   source
source base_func.sh

#   function
function test_one
{
    if [ $# -lt 2 ]
    then
        echo "${FUNCNAME[0]}: src tgt" 1>&2
        echo "src: source encoding, e.g. 8" 1>&2
        echo "tgt: target encoding, e.g. 16" 1>&2
        exit 1
    fi
    local src="${1}"
    local tgt="${2}"
    #   convert src to tgt
    safe_execute "./test_utfcpp --test-utf${src}to${tgt} < testcase/${case_prefix}_utf${src}.txt > testcase/${case_prefix}_output.txt"
    if diff --text testcase/${case_prefix}_output.txt testcase/${case_prefix}_utf${tgt}.txt > testcase/${case_prefix}_diff.txt
    then
        echo "passed test UTF-${src} to UTF-${tgt}" 1>&2
    else
        echo "diff [UTF-${src} to UTF-${tgt}] failed, please refer to testcase/${case_prefix}_diff.txt for detail"
        exit 1
    fi
    #   clean
    safe_rm "testcase/${case_prefix}_output.txt"
    safe_rm "testcase/${case_prefix}_diff.txt"
    return 0
}

#   test
#safe_execute "./test_utfcpp --test-common"
safe_execute "./test_utfcpp --test-split"

#   test UTF-8 to UTF-32
case_prefix="case_1"
safe_execute "test_one 8 16"
safe_execute "test_one 8 32"
safe_execute "test_one 16 8"
safe_execute "test_one 32 8"

exit 0

