set ns [new Simulator]

$ns color 0 blue 
$ns color 1 red 
$ns color 2 white

set O [$ns node] 
set D [$ns node]

set f [open outTP1.tr w] 
$ns trace-all $f 
set nf [open outTP1.nam w] 
$ns namtrace-all $nf 
$ns duplex-link $O $D 2Mb 20ms DropTail

$ns duplex-link-op $O $D orient right-down

$ns queue-limit $O $D 100

$ns duplex-link-op $O $D queuePos 0.5

set udp0 [new Agent/UDP] 
$udp0 set fid_ 1 
$ns attach-agent $O $udp0 
set null0 [new Agent/Null] 
$ns attach-agent $D $null0 
$udp0 set class_ 1 
$ns connect $udp0 $null0 
set Expo0 [new Application/Traffic/Exponential] 
$Expo0 attach-agent $udp0 
$Expo0 set rate_ 1.5Mb 
$Expo0 set burst_time_ 10ms 
$Expo0 set idle_time_ 5ms

set TCP0 [new Agent/TCP] 
$TCP0 set fid_ 2 
$ns attach-agent $O $TCP0 
set sink1 [new Agent/TCPSink] 
$ns attach-agent $D $sink1 
$TCP0 set class_ 2 
$TCP0 set interval_ 50ms 
$ns connect $TCP0 $sink1 
set cbr0 [new Application/Traffic/CBR] 
$cbr0 attach-agent $TCP0 
$cbr0 set rate_ 1Mb

set TCP1 [new Agent/TCP] 
$TCP1 set fid_ 3 
$ns attach-agent $O $TCP1 
set sink2 [new Agent/TCPSink] 
$ns attach-agent $D $sink2 
$TCP0 set class_ 2 
$TCP1 set interval_ 100ms 
$ns connect $TCP1 $sink2 
set cbr1 [new Application/Traffic/CBR] 
$cbr1 attach-agent $TCP1 
$cbr1 set rate_ 1Mb

set TCP2 [new Agent/TCP] 
$TCP2 set fid_ 4 
$ns attach-agent $O $TCP2 
set sink3 [new Agent/TCPSink] 
$ns attach-agent $D $sink3 
$TCP0 set class_ 2 
$TCP0 set interval_ 150ms 
$ns connect $TCP2 $sink3 
set cbr2 [new Application/Traffic/CBR] 
$cbr2 attach-agent $TCP2 
$cbr2 set rate_ 1Mb

$ns at 0.1 "$Expo0 start" 
$ns at 0.1 "$cbr0 start" 
$ns at 0.1 "$cbr1 start" 
$ns at 0.1 "$cbr2 start" 
$ns at 3.0 "finish"

proc finish {} { 
	global ns f nf 
	$ns flush-trace 
	close $f 
	close $nf

	exit 0 
}

$ns run
