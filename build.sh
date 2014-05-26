#! /usr/bin/env bash

cc -I./include -o ./hexcoder src/hexcoder.c -L./lib -lwordspace -lgreyio

