#!/usr/bin/env bash
# auto generate mini_utils.hpp

cat <<EOF > mini_utils.hpp
#ifndef MINI_UTILS_INC
#define MINI_UTILS_INC
EOF

find ../include -name "*.hpp" | grep -v 'mini_utils.hpp' | \
while read line
do
    echo "#include \"${line##*/}\""
done >> mini_utils.hpp

cat <<EOF >> mini_utils.hpp
#endif
EOF

mv mini_utils.hpp ../include
