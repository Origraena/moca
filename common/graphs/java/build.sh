clear
if [ ! -d target ]; then
	echo 'Creating target directory...'
	mkdir target
	echo 'Created target directory!'
fi
if [ ! -d target/class ]; then
	echo 'Creating target/class directory...'
	mkdir target/class
	echo 'Created target/class directory!'
fi
echo 'Invoking javac...'
javac -d target/class -Xlint:unchecked src/Main.java src/moca/graphs/*.java src/moca/operators/*.java src/moca/lists/*.java src/moca/graphs/edges/*.java src/moca/graphs/vertices/*.java
echo 'Finished!'
