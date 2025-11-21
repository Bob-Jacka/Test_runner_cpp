# shellcheck disable=SC2102

cd ..

cd cmake-build-debug && ./Check_runner [Positive] #Run only positive tests

#cd cmake-build-debug && ./Check_runner [Negative] #Run only negative tests