rm -rf vis
mkdir -p vis
i=0
while IFS= read -r line
do
    echo "$line" | dot -Tpng -o "vis/$i.png"
    ((i=i+1))
done
