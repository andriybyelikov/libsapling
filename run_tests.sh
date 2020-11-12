for x in ./test/*.out
do
    echo $x
    ./$x
done
