/* automatically generated by rust-bindgen 0.69.4 */

extern "C" {
    pub fn sign_ipa(
        c_ipaPath: *const ::std::os::raw::c_char,
        c_keyPath: *const ::std::os::raw::c_char,
        c_mpPath: *const ::std::os::raw::c_char,
        c_tmpFolderPath: *const ::std::os::raw::c_char,
        c_dylibFilePath: *const ::std::os::raw::c_char,
        c_appName: *const ::std::os::raw::c_char,
        c_appVersion: *const ::std::os::raw::c_char,
        c_appBundleId: *const ::std::os::raw::c_char,
        c_appIconPath: *const ::std::os::raw::c_char,
        tmpFolderDelete: ::std::os::raw::c_int,
        showLog: ::std::os::raw::c_int,
        error: *mut ::std::os::raw::c_char,
    );
}
