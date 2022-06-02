#!/bin/sh
set -e
LOG=$(mktemp)
$1 $2 | tee $LOG
test ! -s $LOG

