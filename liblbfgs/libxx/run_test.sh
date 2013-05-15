#!/bin/bash

export LC_ALL="C"
export LANG="C"
export LANGUAGE="C"

export SAFE_EXECUTE_VERBOSE=0

source base_func.sh

root_dir="$(pwd)"
work_dir="${root_dir}/work"

safe_mkdir "${work_dir}"
safe_execute "../sample/sample > ${work_dir}/ref.txt"
safe_execute "./test_lbfgsxx   > ${work_dir}/out.txt"

#   diff
if diff ${work_dir}/out.txt ${work_dir}/ref.txt > ${work_dir}/diff.txt
then
    echo "passed"
else
    echo "diff failed, please refer to ${work_dir}/diff.txt for detail"
    exit 1
fi

#   clean
#safe_rm "${work_dir}"

exit 0

