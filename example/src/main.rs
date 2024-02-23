fn main() {
    let ipa_path = std::ffi::CString::new("./ipa/video.ipa").unwrap();
    let key_path = std::ffi::CString::new("./ipa/key.pem").unwrap();
    let mp_path = std::ffi::CString::new("./ipa/lake_13_pm.mobileprovision").unwrap();
    let dylib_file_path = std::ffi::CString::new("./ipa/d.dylib").unwrap();
    let icon_path = std::ffi::CString::new("./ipa/icon.png").unwrap();
    let tmp_folder_path = std::ffi::CString::new("./ipa/tmp").unwrap();

    let mut error_mut: [std::os::raw::c_char; 1024] = [0; 1024];
    // let c_error = get_hello(error_mut.as_mut_ptr());
    unsafe {
        zsign::sign_ipa(ipa_path.as_ptr(), key_path.as_ptr(), mp_path.as_ptr(), dylib_file_path.as_ptr(), icon_path.as_ptr(), tmp_folder_path.as_ptr(), error_mut.as_mut_ptr());
    }
}
