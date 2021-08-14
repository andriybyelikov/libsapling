f0()
{
    echo "sed -i 's/\("$2"[ ]*=\).*/\1 "$3"/' "$1
}

f1()
{
    maxdepth=$(if [[ $# -eq 3 ]]; then echo "-maxdepth $3"; fi)
    echo "$(find $1 $maxdepth -name $2 | sed 's/\//\\\//g')"
}

f2()
{
    sed -i '/# meta test sources/q' $1
    for file in $(grep -oP "(?<=check_PROGRAMS = ).*" $1)
    do
        file_canonical=$(echo $file | sed 's/[\/.]/_/g')
        echo $file_canonical"_SOURCES =" $file.c
        echo $file_canonical"_CPPFLAGS = -I\$(srcdir)/include"
        echo $file_canonical"_LDADD = libsapling.a"
        echo
    done >> $1
}

f0 Makefile.am libsapling_a_SOURCES "$(f1 src *.c)" | sh
f0 Makefile.am pkginclude_HEADERS "$(f1 include/libsapling *.h 1)" | sh
f0 Makefile.am dm_HEADERS "$(f1 include/libsapling/dm *.h 1)" | sh
f0 Makefile.am dm_typed_HEADERS "$(f1 include/libsapling/dm/typed *.h 1)" | sh
f0 Makefile.am cc_HEADERS "$(f1 include/libsapling/cc *.h)" | sh
f0 Makefile.am noinst_HEADERS "$(f1 src *.h)" | sh

f2 Makefile.am
