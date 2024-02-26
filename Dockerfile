FROM rust:1.76.0-alpine3.19 as builder
WORKDIR /zsign
RUN apk add --no-cache curl g++ clang-dev clang clang-static zip unzip openssl-dev openssl-libs-static
COPY . .
RUN cd example && RUSTFLAGS="-C target-feature=-crt-static" cargo build --release && \
    cd /zsign/target/release && \
    ldd example | awk '{print $3}' | grep -v '^$' > dependencies.txt

FROM alpine:3.19
WORKDIR /app
RUN apk add --no-cache zip unzip dumb-init
COPY ./ssl/openssl.cnf /etc/ssl/openssl.cnf
RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
RUN echo 'Asia/Shanghai' > /etc/timezone
# dependencies.txt为依赖的动态链接库，如果缺少请添加上
COPY --from=builder /zsign/target/release/dependencies.txt /app/dependencies.txt
COPY --from=builder /zsign/target/release/example /app/example

# 依赖的动态链接库，在dependencies.txt中列出
COPY --from=builder /lib/libcrypto.so.3 /lib/libcrypto.so.3
COPY --from=builder /usr/lib/libstdc++.so.6 /usr/lib/libstdc++.so.6
COPY --from=builder /usr/lib/libgcc_s.so.1 /usr/lib/libgcc_s.so.1
COPY --from=builder /lib/ld-musl-aarch64.so.1 /lib/ld-musl-aarch64.so.1

COPY ./ipa /app/ipa
EXPOSE 3000
ENTRYPOINT ["/usr/bin/dumb-init", "--"]
CMD ["/app/example"]