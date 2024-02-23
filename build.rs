extern crate cc;

fn main() {
    let mut builder: cc::Build = cc::Build::new();
    //依赖openssl
    builder
        .cpp(true)
        .warnings(false)
        .std("c++11")
        .flag("-c")
        .shared_flag(false)
        .flag("-I/opt/homebrew/Cellar/openssl@3/3.2.0_1/include")
        .flag("-L/opt/homebrew/Cellar/openssl@3/3.2.0_1/lib")
        .file("zsign/common/base64.cpp")
        .file("zsign/common/json.cpp")
        .file("zsign/common/common.cpp")
        .file("zsign/archo.cpp")
        .file("zsign/bundle.cpp")
        .file("zsign/macho.cpp")
        .file("zsign/openssl.cpp")
        .file("zsign/signing.cpp")
        .file("zsign/zsign.cpp")
        .file("zsign/zsign.cpp")
        .compile("zsign");

    println!("cargo:rustc-link-lib=ssl");
    println!("cargo:rustc-link-lib=crypto");
    println!("cargo:rustc-link-search=/opt/homebrew/Cellar/openssl@3/3.2.0_1/lib");
    println!("cargo:rerun-if-changed=zsign/common/common.cpp");
    println!("cargo:rerun-if-changed=build.rs");
}