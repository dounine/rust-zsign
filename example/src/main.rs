use zsign::ZsignBuilder;

fn main() {
    let ipa_path = "./ipa/video.ipa";
    let key_path = "./ipa/key.pem";
    let mp_path = "./ipa/lake_13_pm.mobileprovision";
    let dylib_file_path = "./ipa/d.dylib";
    let icon_path = "./ipa/icon.png";
    let tmp_folder_path = "./ipa/tmp";

    ZsignBuilder::new()
        .app_icon_path(icon_path)
        .app_name("hello")
        .app_version("1.0.0")
        .app_bundle_id("com.lake.hello")
        .tmp_folder_path(tmp_folder_path)
        .tmp_folder_no_delete()
        .dylib_file_path(dylib_file_path)
        .sign(ipa_path, key_path, mp_path)
        .unwrap();


    // let result = zsign::sign(
    //     ipa_path,
    //     key_path,
    //     mp_path,
    //     tmp_folder_path,
    //     None,
    //     None,
    //     None,
    //     None,
    //     None,
    //     true,
    //     true,
    // );
    // result.unwrap();
}
