echo $1

arg=$1
init="init"
stop="stop"

if [ "$arg" == "$init" ]
then
	echo 'Script inicialization comunication PRU.....'
	#configure pin INPUT pru 0 
    echo 'pruin' > /sys/devices/platform/ocp/ocp\:P9_27_pinmux/state
    #configure pin INPUT pru 1
	echo 'pruout' > /sys/devices/platform/ocp/ocp\:P8_44_pinmux/state
	#load binary to de pru0
	echo 'pru_input_PRU0.out' > /sys/class/remoteproc/remoteproc1/firmware
    echo 'pru_input_PRU1.out' > /sys/class/remoteproc/remoteproc2/firmware
	#start to PRU0
	echo 'start' > /sys/class/remoteproc/remoteproc1/state
    echo 'start' > /sys/class/remoteproc/remoteproc2/state
	echo 'PRU 0 init ......'
fi


if [ "$arg" == "$stop" ]
then
	echo 'Scrip to stop led blink.....'
	echo 'stop' > /sys/class/remoteproc/remoteproc1/state
    echo 'stop' > /sys/class/remoteproc/remoteproc2/state
	echo 'default' > /sys/devices/platform/ocp/ocp\:P9_27_pinmux/state
    echo 'default' > /sys/devices/platform/ocp/ocp\:P8_44_pinmux/state
fi