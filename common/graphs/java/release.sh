if [ ! -d target/class ]; then
	echo 'No target/class directory'
	echo 'Run build.sh first!'
else
	echo 'Creating jar file...'
	jar cfe target/graphs.jar Main -C target/class/ ./
	echo 'Finished!'
fi
