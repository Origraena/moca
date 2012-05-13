TARGET_NAME=logging.jar
if [ ! -d target/class ]; then
	echo 'No target/class directory'
	echo 'Run build.sh first!'
else
	if [ -f target/class/$TARGET_NAME ]; then
		echo 'Removing old jar file...'
		rm target/class/$TARGET_NAME
	fi
	echo 'Creating jar file...'
	jar cfe target/$TARGET_NAME ori.logging.Logger -C target/class/ ./
	echo 'Finished!'
fi
