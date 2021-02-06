echo $1

arg=$1
init="init"
stop="stop"

if [ "$arg" == "$init" ]
then
	echo 'Script inicialization uart .....'
	#configure pin output pru
	echo 'pru_uart' > /sys/devices/platform/ocp/ocp\:P9_24_pinmux/state
	echo 'pru_uart' > /sys/devices/platform/ocp/ocp\:P9_26_pinmux/state

	#load binary to de pru0
	echo 'uart_pru_0.out' > /sys/class/remoteproc/remoteproc1/firmware
	#start to PRU0
	echo 'start' > /sys/class/remoteproc/remoteproc1/state
	echo 'PRU 0 init UART ......'
fi

if [ "$arg" == "$stop" ]
then
	echo 'Scrip to stop led blink.....'
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_24_pinmux/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_26_pinmux/state
fi
