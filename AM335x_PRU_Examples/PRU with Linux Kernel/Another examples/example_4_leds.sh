echo $1

arg=$1
init="init"
stop="stop"

led1="q"
led2="w"
led3="e"
led4="r"


if [ "$arg" == "$init" ]
then
	echo 'Script inicialization led blink.....'
	#configure pin output pru
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P9_28_pinmux/state
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P9_29_pinmux/state
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P9_30_pinmux/state
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P9_31_pinmux/state
	#load binary to de pru0
	echo 'lab6_led.out' > /sys/class/remoteproc/remoteproc1/firmware
	#load binary to de pru1
	echo 'PRU_halt.out' > /sys/class/remoteproc/remoteproc2/firmware

	#start to PRU0 and pru1 
	echo 'start' > /sys/class/remoteproc/remoteproc1/state
	echo 'start' > /sys/class/remoteproc/remoteproc2/state
	echo 'PRU 0 init and pru1 init ok ......'
fi

if [ "$arg" == "$stop" ]
then
	echo 'Scrip to stop led blink.....'
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
	echo 'stop' > /sys/class/remoteproc/remoteproc2/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_29_pinmux/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_28_pinmux/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_30_pinmux/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_31_pinmux/state
fi

if [ "$arg" == "$led1" ]
then
	echo "q" > /dev/rpmsg_pru30
fi

if [ "$arg" == "$led2" ]
then
	echo "w" > /dev/rpmsg_pru30
fi

if [ "$arg" == "$led3" ]
then
	echo "e" > /dev/rpmsg_pru30
fi

if [ "$arg" == "$led4" ]
then
	echo "r" > /dev/rpmsg_pru30
fi