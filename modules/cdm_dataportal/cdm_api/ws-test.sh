#!/usr/bin/bash

wget    --header='Accept: application/json' \
        --header='Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7'  \
        --header='Accept-Language: de'\
        -O ws-whatis.json \
        http://127.0.0.1:8080/cdmserver/whatis/6838182f-823d-4568-9a5e-9eeb8028f290

wget    --header='Accept: application/json' \
        --header='Accept-Charset: UTF-8'  \
        --header='Accept-Language: de'\
        -O ws-taxon.json \
        http://127.0.0.1:8080/cdmserver/taxon/e5d81723-61f1-47c6-956b-0819eeed1069



#wget    --header='Accept: application/json' \
#	      --header='Accept-Charset: UTF-8'  \
#        --header='Accept-Language: de'\
#        -O ws-taxonomy-root.json \
#	      http://127.0.0.1:8080/cdmserver/taxonomy/root/

	