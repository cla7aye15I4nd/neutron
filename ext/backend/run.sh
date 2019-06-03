#!/bin/sh
#name:run.sh

socat tcp-l:9999,fork exec:./main,reuseaddr
