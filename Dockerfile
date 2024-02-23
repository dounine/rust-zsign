FROM alpine:3.14
WORKDIR /zsign
RUN apk add --no-cache curl g++ clang clang-static zip unzip openssl-dev openssl-libs-static
RUN curl --proto '=https' --tlsv1.2 https://sh.rustup.rs -sSf > rustup.sh && sh rustup.sh -y && rm rustup.sh
COPY . .
RUN ~/.cargo/bin/cargo build --release