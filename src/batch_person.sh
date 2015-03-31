
total=63772
begin_index=0
end_index=0

keylist[0]=111
keylist[1]=222
keylist[2]=333
keylist[3]=444
keylist[4]=555
keylist[5]=666
keylist[6]=777
keylist[7]=888
keylist[8]=999
keylist[9]=1999

while [ $total -gt 0 ]
do
	echo " `date` "
	exsit=`ps -fea | grep spider | grep shixin | grep -v grep | wc -l`
	if [ $exsit -eq 0 ]
	then
		i=0
		len=${#keylist[*]}
		while [ $i -lt $len ]
		do
			if [ $total -gt 0 ]	
			then
				begin_index=`expr $end_index + 1`
				end_index=`expr $end_index + 1000`
				echo "begin $begin_index - $end_index  ipckey ${keylist[$i]}"
				export IPCKEY=${keylist[$i]}
				export GT_CONDITION=$begin_index
				export LT_CONDITION=$end_index
				spider clear
				spider shixin.court.gov.cn -1 10 person
				total=`expr $total - 1000`
				i=`expr $i + 1`
				echo "left page $total"
				sleep 30 
			else
				break
			fi
		done
	fi

	sleep 60 
done



