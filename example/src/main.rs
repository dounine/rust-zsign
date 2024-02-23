fn main() {
    let ipa_path = "./ipa/video.ipa";
    let key_path = "./ipa/key.pem";
    let mp_path = "./ipa/lake_13_pm.mobileprovision";
    let dylib_file_path = "./ipa/d.dylib";
    let icon_path = "./ipa/icon.png";
    let tmp_folder_path = "./ipa/tmp";

    let mut error_mut: [std::os::raw::c_char; 1024] = [0; 1024];
    let result = zsign::sign(
        ipa_path,
        key_path,
        mp_path,
        tmp_folder_path,
        None,
        None,
        None,
        None,
        None,
        true,
        true,
    );
    result.unwrap();
}
