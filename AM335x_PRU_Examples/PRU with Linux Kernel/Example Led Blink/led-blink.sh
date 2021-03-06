echo $1

arg=$1
init="init"
stop="stop"

if [ "$arg" == "$init" ]
then
	echo 'Script inicialization led blink.....'
	#configure pin output pru
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P9_29_pinmux/state
	#load binary to de pru0
	echo 'initcializacao.out' > /sys/class/remoteproc/remoteproc1/firmware
	#start to PRU0
	echo 'start' > /sys/class/remoteproc/remoteproc1/state
	echo 'PRU 0 init ......'
fi

if [ "$arg" == "$stop" ]
then
	echo 'Scrip to stop led blink.....'
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_29_pinmux/state
fi
