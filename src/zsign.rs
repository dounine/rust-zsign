#[link(name = "zsign")]
extern "C" {
    pub fn sign_ipa(ipa_path: *const std::os::raw::c_char, key_path: *const std::os::raw::c_char, mp_path: *const std::os::raw::c_char, dylib_file_path: *const std::os::raw::c_char, icon_path: *const std::os::raw::c_char, tmp_folder_path: *const std::os::raw::c_char, error: *mut std::os::raw::c_char) -> *const std::os::raw::c_int;
}