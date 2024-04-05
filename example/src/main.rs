use zsign::ZsignBuilder;

fn main() {
    let ipa_path = "./ipa/video.ipa";
    let p12_path = "./ipa/lake.p12";
    let p12_password = "1";
    let mp_path = "./ipa/lake_13_pm.mobileprovision";
    let dylib_file_path = "./ipa/d.dylib";
    let icon_path = "./ipa/icon.png";
    let output_path = "./ipa/output.ipa";

    //delete output_path
    std::fs::remove_file(output_path).unwrap_or_default();
    //openssl3 需要兼容低版本p12，否则无法解析p12文件,链接：https://www.practicalnetworking.net/practical-tls/openssl-3-and-legacy-providers/
    //或者自己把p12文件先转key.pem再给p12_path也可以
    ZsignBuilder::new()
        .app_icon_path(icon_path)
        .app_name("hello")
        .app_version("1.0.0")
        .app_bundle_id("com.lake.hello")
        .dylib_file_path(dylib_file_path)
        .dylib_remove_path("@executable_path/Frameworks/test.dylib")
        // .disable_sign()
        // .disable_zip_ipa()
        .build(ipa_path, p12_path, p12_password, mp_path, output_path)
        .unwrap();
}
