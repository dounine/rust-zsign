#!/bin/bash
docker rm zsign
docker build . -t zsign
docker run -d --rm --name zsign zsign tail -f /dev/null
docker cp zsign:/zsign/build/linux/arm64-v8a/release/libzsign.so ./tmp
docker stop zsign