if [ -d target ]; then
	echo 'Removing target directory...'
	rm -r target
	echo 'Finished!'
else
	echo 'Nothing to be done.'
fi
