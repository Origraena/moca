
set ns [new Simulator]

$ns color 0 blue
$ns color 1 red
$ns color 2 white

puts "Mise en place des noeuds"
set O [$ns node]
set D [$ns node]

set windowSize [open ./windowSize.txt w]
set queue1 [open ./queueSize1.txt w]
set queue5 [open ./queueSize5.txt w]
set f [open ./tp1.tr w]
$ns trace-all $f
set nf [open ./tp1.nam w]
$ns namtrace-all $nf

puts "Mise en place des liens"
$ns duplex-link $O $D 2Mb 20ms DropTail
$ns queue-limit $O $D 100
set spy [$ns monitor-queue $O $D $queue1]

#Mise de la connection UDP
set udp [new Agent/UDP]
$udp set fid_ 1
set trappe [new Agent/LossMonitor]
$ns attach-agent $O $udp
$ns attach-agent $D $trappe
$ns connect $udp $trappe
set appUDP [new Application/Traffic/Exponential]
$appUDP attach-agent $udp
#$udp set packetSize_ 100
$appUDP set burst_time_ 10ms
$appUDP set idle_time_ 5ms
$appUDP set rate_ 1.5Mb
$ns at 0.0 "$appUDP start"


puts "Mise de la premiere connection TCP"
set tcp1 [$ns create-connection TCP $O TCPSink $D 2]
set appTCP1 [new Application/Traffic/CBR]
$appTCP1 attach-agent $tcp1
$appTCP1 set interval_ 50ms
$tcp1 set packetSize_ 6250
$ns at 0.0 "$appTCP1 start"

puts "Mise de la seconde connection TCP"
set sinkTCP2 [new Agent/TCPSink]
set tcp2 [new Agent/TCP]
$tcp2 set fid_ 3
$ns attach-agent $O $tcp2
$ns attach-agent $D $sinkTCP2
$ns connect $tcp2 $sinkTCP2
set appTCP2 [new Application/Traffic/CBR]
$appTCP2 attach-agent $tcp2
$appTCP2 set interval_ 100ms
$tcp2 set packetSize_ 12500
$ns at 0.0 "$appTCP2 start"

puts "Mise de la troisieme connection TCP"
set sinkTCP3 [new Agent/TCPSink]
set tcp3 [new Agent/TCP]
$tcp3 set fid_ 4
$ns attach-agent $O $tcp3
$ns attach-agent $D $sinkTCP3
$ns connect $tcp3 $sinkTCP3
set appTCP3 [new Application/Traffic/CBR]
$appTCP3 attach-agent $tcp3
$appTCP3 set interval_ 150ms
$tcp3 set packetSize_ 18750
$ns at 0.0 "$appTCP3 start"

set timeFinish 20.0
 
$ns at 0.0 "printTCPwin"
$ns at 0.0 "printQueueSize1"
$ns at 0.0 "printQueueSize5"
$ns at $timeFinish "finish"

proc printTCPwin {} {
	global ns tcp1 tcp2 tcp3 timeFinish windowSize
	
	set now [$ns now]
	set delai 0.05
	
	puts $windowSize "$now [$tcp1 set cwnd_] [$tcp2 set cwnd_] [$tcp3 set cwnd_]"
	
	$ns after $delai printTCPwin
}

proc printQueueSize1 {} {
	global ns spy queue1
	
	set now [$ns now]
	set delai 0.01
	
	if {[$spy set pkts_] > 0} {
		puts $queue1 "$now [$spy set pkts_]"
		}
	
	$ns after $delai printQueueSize1
}

proc printQueueSize5 {} {
	global ns spy queue5
	
	set now [$ns now]
	set delai 0.05
	
	if {[$spy set pkts_] > 0} {
		puts $queue5 "$now [$spy set pkts_]"
	}
	
	$ns after $delai printQueueSize5
}

proc finish {} {
	global ns f nf filename
	$ns flush-trace
	close $f
	close $nf

	#puts "running nam..."
	#exec nam "tp1.tr" &
	
	exit 0
}

$ns run


