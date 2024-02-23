extern crate cc;

fn env<N: AsRef<str>>(name: N) -> String {
    option_env(name).expect("missing env var")
}
fn option_env<N: AsRef<str>>(name: N) -> Option<String> {
    let name = name.as_ref();
    eprintln!("cargo:rerun-if-env-changed={}", name);
    std::env::var(name).ok()
}
fn main() {
    let mut builder: cc::Build = cc::Build::new();

    let zsign_files = [
        "zsign/common/base64.cpp",
        "zsign/common/json.cpp",
        "zsign/common/common.cpp",
        "zsign/archo.cpp",
        "zsign/bundle.cpp",
        "zsign/macho.cpp",
        "zsign/openssl.cpp",
        "zsign/signing.cpp",
        "zsign/zsign.cpp",
    ];

    //依赖openssl
    builder
        .cpp(true)
        .warnings(false)
        .std("c++11")
        .flag("-c")
        .shared_flag(false)
        .files(zsign_files.iter())
        .include(env("DEP_OPENSSL_INCLUDE")) //openssl 头文件
        .compile("zsign");

    println!("cargo:rustc-link-lib=ssl");
    println!("cargo:rustc-link-lib=crypto");
    println!("cargo:rerun-if-changed=zsign/common/common.cpp");
    println!("cargo:rerun-if-changed=build.rs");

    let bindings = bindgen::Builder::default()
        .header("wrapper.h")
        .generate()
        .expect("Unable to generate bindings");

    bindings
        .write_to_file("bindings/bindings.rs")
        .expect("Couldn't write bindings!");
}
