#!/bin/sh

NMSG_MSGMOD_DIR="/home/reed/work/farsight/src/public-github-farsightsec/nmsg/nmsg/base/.libs"
export NMSG_MSGMOD_DIR

NMSGTOOL="/home/reed/work/farsight/src/public-github-farsightsec/nmsg/src/nmsgtool"
TJSON="../../tests/empty-string-tests/empty-string.json"
TNMSG="../../tests/empty-string-tests/empty-string.nmsg"

status=0

if $NMSGTOOL -r $TNMSG | fgrep "(null)"; then
    echo FAIL: presentation contains '"(null)"'
    status=1
else
    echo PASS: presentation format empty
fi

if $NMSGTOOL -j $TJSON -w - | cmp - $TNMSG; then
    echo PASS: json load succeeded
else
    echo FAIL: json load failed
    status=1
fi

# test writing to JSON
if $NMSGTOOL -r $TNMSG --writejson - | cmp - $TJSON; then
    echo PASS: json output succeeded
else
    echo FAIL: json output failed
    status=1
fi

exit $status
