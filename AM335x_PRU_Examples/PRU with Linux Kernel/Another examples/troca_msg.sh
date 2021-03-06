echo $1 $2

arg=$1
init="init"
stop="stop"
msg="msg"

message=$2


if [ "$arg" == "$init" ]
then
	echo 'Script inicialization message.....'
	#load binary to de pru0
	echo 'PRU_halt.out' > /sys/class/remoteproc/remoteproc1/firmware
	#load binary to de pru0
	echo 'LAB5_echo_interrup.out' > /sys/class/remoteproc/remoteproc2/firmware

	#start to PRU0 and pru1 
	echo 'start' > /sys/class/remoteproc/remoteproc1/state
	echo 'start' > /sys/class/remoteproc/remoteproc2/state
     
	#confirm 
	ls /dev/ | grep pru

	echo 'PRU 0 init and pru1 init ok ......'
fi

if [ "$arg" == "$stop" ]
then
	echo 'Scrip to stop led blink.....'
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
	echo 'stop' > /sys/class/remoteproc/remoteproc2/state
fi

if [ "$arg" == "$msg" ]
then
	echo "$message" > /dev/rpmsg_pru31
	echo "Echo pru message: "
	cat /dev/rpmsg_pru31
fi
