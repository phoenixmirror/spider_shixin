
ipcs -m | grep idd | while read f1 f2 f3 f4 f5 f6
do
  echo "$f4"
  if [ "$f4" == "666" ]
  then
  	echo "ipcrm -m $f2"
	ipcrm -m $f2
  fi
done

ipcs -s | grep idd | while read f1 f2 f3
do
  echo "ipcrm -s $f2"
  ipcrm -s $f2
done

ipcs -q | grep idd | while read f1 f2 f3
do
  echo "ipcrm -q $f2"
  ipcrm -q $f2
done

