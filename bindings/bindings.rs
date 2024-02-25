/* automatically generated by rust-bindgen 0.69.4 */

extern "C" {
    pub fn sign_ipa(
        c_ipaPath: *const ::std::os::raw::c_char,
        c_p12Path: *const ::std::os::raw::c_char,
        c_p12Password: *const ::std::os::raw::c_char,
        c_mpPath: *const ::std::os::raw::c_char,
        c_dylibFilePath: *const ::std::os::raw::c_char,
        c_dylibPrefixPath: *const ::std::os::raw::c_char,
        c_appName: *const ::std::os::raw::c_char,
        c_appVersion: *const ::std::os::raw::c_char,
        c_appBundleId: *const ::std::os::raw::c_char,
        c_appIconPath: *const ::std::os::raw::c_char,
        c_outputPath: *const ::std::os::raw::c_char,
        deletePlugIns: ::std::os::raw::c_int,
        deleteWatchPlugIns: ::std::os::raw::c_int,
        deleteDeviceSupport: ::std::os::raw::c_int,
        deleteSchemeURL: ::std::os::raw::c_int,
        enableFileAccess: ::std::os::raw::c_int,
        sign: ::std::os::raw::c_int,
        zipLevel: ::std::os::raw::c_int,
        zipIpa: ::std::os::raw::c_int,
        showLog: ::std::os::raw::c_int,
        error: *mut ::std::os::raw::c_char,
    );
}
