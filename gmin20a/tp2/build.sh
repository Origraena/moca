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
javac -d target/class \
-cp lib/graphs.jar \
src/Main.java \
src/tictactoe/*.java
echo 'Finished!'
