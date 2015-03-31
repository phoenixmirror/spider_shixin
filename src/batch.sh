
echo "begin 0-1000"
export IPCKEY=111
export GT_CONDITION=0
export LT_CONDITION=1000
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30

echo "begin 1001-2000"
export IPCKEY=222
export LT_CONDITION=2000
export GT_CONDITION=1001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30


echo "begin 2001-3000"
export IPCKEY=333
export LT_CONDITION=3000
export GT_CONDITION=2001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30

echo "begin 3001-4000"
export IPCKEY=444
export LT_CONDITION=4000
export GT_CONDITION=3001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30

echo "begin 4001-5000"
export IPCKEY=555
export LT_CONDITION=5000
export GT_CONDITION=4001
spider clear
spider shixin.court.gov.cn -1 10 


echo "begin 5001-6000"
export IPCKEY=666
export LT_CONDITION=6000
export GT_CONDITION=5001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30


echo "begin 6001-7000"
export IPCKEY=777
export LT_CONDITION=7000
export GT_CONDITION=6001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30



echo "begin 7001-8000"
export IPCKEY=888
export LT_CONDITION=8000
export GT_CONDITION=7001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30


echo "begin 8001-9000"
export IPCKEY=999
export LT_CONDITION=9000
export GT_CONDITION=8001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30


echo "begin 9001-end"
export IPCKEY=1999
export LT_CONDITION=0
export GT_CONDITION=9001
spider clear
spider shixin.court.gov.cn -1 10 

sleep 30




