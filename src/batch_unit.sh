

wait()
{
	id=`ps -fea | grep idd | grep -w shixin_unit_downloadlist_and_detail | grep -v grep | awk '{print $2}' | tail -1`
	while [ "$id"X != ""X ]
	do
		date
		sleep 10
		id=`ps -fea | grep idd | grep -w shixin_unit_downloadlist_and_detail | grep -v grep | awk '{print $2}' | tail -1`
	done
}

export TOTALCNT=165000

echo "clear sharemem"
export IPCKEY=111
export GT_CONDITION=1
export LT_CONDITION=1000
shixin_unit_downloadlist_and_detail clear

export IPCKEY=222
export LT_CONDITION=2000
export GT_CONDITION=1001

export IPCKEY=333
export LT_CONDITION=3000
export GT_CONDITION=2001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=444
export LT_CONDITION=4000
export GT_CONDITION=3001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=555
export LT_CONDITION=5000
export GT_CONDITION=4001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=666
export LT_CONDITION=6000
export GT_CONDITION=5001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=777
export LT_CONDITION=7000
export GT_CONDITION=6001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=888
export LT_CONDITION=8000
export GT_CONDITION=7001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=999
export LT_CONDITION=9000
export GT_CONDITION=8001
shixin_unit_downloadlist_and_detail clear

export IPCKEY=1999
export LT_CONDITION=0
export GT_CONDITION=9001
shixin_unit_downloadlist_and_detail clear

echo "begin 0-1000"
export IPCKEY=111
export GT_CONDITION=1
export LT_CONDITION=1000
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

echo "begin 1001-2000"
export IPCKEY=222
export LT_CONDITION=2000
export GT_CONDITION=1001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

wait

echo "begin 2001-3000"
export IPCKEY=333
export LT_CONDITION=3000
export GT_CONDITION=2001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

echo "begin 3001-4000"
export IPCKEY=444
export LT_CONDITION=4000
export GT_CONDITION=3001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

wait

echo "begin 4001-5000"
export IPCKEY=555
export LT_CONDITION=5000
export GT_CONDITION=4001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 


echo "begin 5001-6000"
export IPCKEY=666
export LT_CONDITION=6000
export GT_CONDITION=5001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

wait

echo "begin 6001-7000"
export IPCKEY=777
export LT_CONDITION=7000
export GT_CONDITION=6001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10



echo "begin 7001-8000"
export IPCKEY=888
export LT_CONDITION=8000
export GT_CONDITION=7001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

wait

echo "begin 8001-9000"
export IPCKEY=999
export LT_CONDITION=9000
export GT_CONDITION=8001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

echo "begin 9001-end"
export IPCKEY=1999
export LT_CONDITION=0
export GT_CONDITION=9001
shixin_unit_downloadlist_and_detail clear
shixin_unit_downloadlist_and_detail shixin.court.gov.cn -1 8 

sleep 10

wait


