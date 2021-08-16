#!/usr/bin/env bash
# auto generate mini_util.hpp

cp mainpage.md mini_util.hpp

cat <<EOF >> mini_util.hpp
#ifndef MINI_UTILS_INC
#define MINI_UTILS_INC
EOF

find ../include -name "*.hpp" | grep -v 'mini_util.hpp' | \
while read line
do
    echo "#include \"${line##*/}\""
done >> mini_util.hpp

cat <<EOF >> mini_util.hpp
#endif
EOF

mv mini_util.hpp ../include
