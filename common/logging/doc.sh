SRC_PATH='./src'
PACKAGES='moca.logging'
javadoc \
-d target/doc \
-sourcepath $SRC_PATH \
-subpackages $PACKAGES \
-private \
-use \
-windowtitle "Logging" \
-doctitle "<b>Logging</b>" \
-header "logging" \
-bottom "Master 1 MOCA, Universite Montpellier 2, 2012"
