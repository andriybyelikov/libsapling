f0()
{
    grep \#include $1 > $2
    gcc -E -I. -Iinclude $1 | sed '/#.*/d' \
    | sed '1,/static int libsapling_end_of_includes;/d' \
    | indent -kr -bad -bap -bbb -bbo -blf -bls | sed 's/((void \*) 0)/NULL/g' \
    >> $2
}

f0 $1 $2
