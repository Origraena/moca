############################################################
# tp1.tcl
# TP de simulation de reseaux avec ns
# 2012/04/20
# Maziane Abdel-Mourhit
# Rocher Swan
# UM2 - M1 MOCA
############################################################
set ns [new Simulator]

# creation des noeuds
set O [$ns node]
set D [$ns node]

# ouverture des fichiers
set f [open res/tp1.tr w]
set queueFile001 [open res/tp1_queue001.tr w]
set queueFile005 [open res/tp1_queue005.tr w]
set windowSizeFile1 [open res/tp1_winsize1.tr w]
set windowSizeFile2 [open res/tp1_winsize2.tr w]
set windowSizeFile3 [open res/tp1_winsize3.tr w]
$ns trace-all $f

# creation et configuration du lien "physique"
$ns duplex-link $O $D 2Mb 20ms DropTail
$ns queue-limit $O $D 100

# creation d'un moniteur sur la file (afin de mesurer sa taille)
set queueMonitor [$ns monitor-queue $O $D $queueFile001]

# lien 1 : UDP 
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
$Expo0 set packet_size_ 100

# lien 2 : TCP
set tcp0 [new Agent/TCP]
$tcp0 set fid_ 2
$ns attach-agent $O $tcp0
set sink0 [new Agent/TCPSink]
$ns attach-agent $D $sink0
$ns connect $tcp0 $sink0
set cbr0 [new Application/Traffic/CBR]
$cbr0 set packet_size_ 6250
$cbr0 set interval_ 50ms
$cbr0 attach-agent $tcp0

# lien 3 : TCP
set tcp1 [new Agent/TCP]
$tcp1 set fid_ 3
$ns attach-agent $O $tcp1
set sink1 [new Agent/TCPSink]
$ns attach-agent $D $sink1
$ns connect $tcp1 $sink1
set cbr1 [new Application/Traffic/CBR]
$cbr0 set packet_size_ 12500
$cbr1 set interval_ 100ms
$cbr1 attach-agent $tcp1
 
# lien 4 : TCP
set tcp2 [new Agent/TCP]
$tcp2 set fid_ 4
$ns attach-agent $O $tcp2
set sink2 [new Agent/TCPSink]
$ns attach-agent $D $sink2
$ns connect $tcp2 $sink2
set cbr2 [new Application/Traffic/CBR]
$cbr0 set packet_size_ 18750
$cbr2 set interval_ 150ms
$cbr2 attach-agent $tcp2

# instructions de demarrages
$ns at 0.0 "recordQueue 0.01 $queueFile001"
$ns at 0.0 "recordQueue 0.05 $queueFile005"
$ns at 0.0 "recordWinSize 0.01 $windowSizeFile1 $tcp0"
$ns at 0.0 "recordWinSize 0.01 $windowSizeFile2 $tcp1"
$ns at 0.0 "recordWinSize 0.01 $windowSizeFile3 $tcp2"
$ns at 0.1 "$Expo0 start"
$ns at 0.1 "$cbr0 start"
$ns at 0.1 "$cbr1 start"
$ns at 0.1 "$cbr2 start"
$ns at 10 "finish"


# Procedures 

# effectue les logs de la taille de file
proc recordQueue {delai qfile} {
	global ns queueMonitor
	set now [$ns now]
	$queueMonitor instvar size_ pkts_ barrivals_ bdepartures_ parrivals_ pdepartures_ bdrops_ pdrops_ 
	puts $qfile "$now [$queueMonitor set pkts_]" 
	$ns after $delai "recordQueue $delai $qfile"
}

# effectue les logs des tailles de fenetre
proc recordWinSize {delai wfile tcpAgent} {
	global ns
	set now [$ns now]
	puts $wfile "$now [$tcpAgent set cwnd_]"
	$ns after $delai "recordWinSize $delai $wfile $tcpAgent"
}

# ferme les differents fichiers
proc finish {} {
	global ns f queueFile001 queueFile005 windowSizeFile1 windowSizeFile2 windowSizeFile3
	$ns flush-trace
	close $f
	close $queueFile001
	close $queueFile005
	close $windowSizeFile1
	close $windowSizeFile2
	close $windowSizeFile3
	exit 0
}

$ns run

