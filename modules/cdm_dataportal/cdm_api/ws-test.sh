#!/usr/bin/bash

wget    --header='Accept: application/json' \
	--header='Accept-Charset: UTF-8'  \
        --header='Accept-Language: de'\
	--save-headers\
        -O ws-test.json \
	http://127.0.0.1:8080/cdmserver/taxonomy/root/
