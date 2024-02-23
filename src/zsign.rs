use crate::error::ZsignError;

#[link(name = "zsign")]
extern "C" {
    //int sign_ipa(
    //             const char *ipaPath,
    //             const char *keyPath,
    //             const char *mpPath,
    //             const char *tmpFolderPath,
    //             const char *dylibFilePath,
    //             const char *appIconPath,
    //             const char *appName,
    //             const char *appVersion,
    //             const char *appBundleId,
    //             const bool tmpFolderDelete,
    //             const bool log,
    //             char *error
    //     );
    pub fn sign_ipa(
        ipa_path: *const std::os::raw::c_char,
        key_path: *const std::os::raw::c_char,
        mp_path: *const std::os::raw::c_char,
        tmp_folder_path: *const std::os::raw::c_char,
        dylib_file_path: *const std::os::raw::c_char,
        app_name: *const std::os::raw::c_char,
        app_version: *const std::os::raw::c_char,
        app_bundle_id: *const std::os::raw::c_char,
        app_icon_path: *const std::os::raw::c_char,
        tmp_folder_delete: *const std::os::raw::c_int,
        show_log: *const std::os::raw::c_int,
        error: *mut std::os::raw::c_char,
    ) -> *const std::os::raw::c_int;
}

pub fn sign<T: AsRef<str>>(
    ipa_path: T,
    key_path: T,
    mp_path: T,
    tmp_folder_path: T,
    dylib_file_path: Option<T>,
    app_icon_path: Option<T>,
    app_name: Option<T>,
    app_version: Option<T>,
    app_bundle_id: Option<T>,
    tmp_folder_delete: bool,
    show_log: bool,
) -> Result<(), ZsignError> {
    let dylib_file_path_default = dylib_file_path
        .map(|x| x.as_ref().to_string())
        .unwrap_or_default();
    let app_icon_path_default = app_icon_path
        .map(|x| x.as_ref().to_string())
        .unwrap_or_default();
    let ipa_path =
        std::ffi::CString::new(ipa_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
    let key_path =
        std::ffi::CString::new(key_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
    let mp_path =
        std::ffi::CString::new(mp_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
    let tmp_folder_path = std::ffi::CString::new(tmp_folder_path.as_ref())
        .map_err(|e| ZsignError::Msg(e.to_string()))?;
    let dylib_file_path = std::ffi::CString::new(dylib_file_path_default)
        .map_err(|e| ZsignError::Msg(e.to_string()))?;
    let app_icon_path = std::ffi::CString::new(app_icon_path_default)
        .map_err(|e| ZsignError::Msg(e.to_string()))?;

    let app_name_default = app_name.map(|x| x.as_ref().to_string()).unwrap_or_default();
    let app_name =
        std::ffi::CString::new(app_name_default).map_err(|e| ZsignError::Msg(e.to_string()))?;
    let app_version_default = app_version
        .map(|x| x.as_ref().to_string())
        .unwrap_or_default();
    let app_version =
        std::ffi::CString::new(app_version_default).map_err(|e| ZsignError::Msg(e.to_string()))?;
    let app_bundle_id_default = app_bundle_id
        .map(|x| x.as_ref().to_string())
        .unwrap_or_default();
    let app_bundle_id = std::ffi::CString::new(app_bundle_id_default)
        .map_err(|e| ZsignError::Msg(e.to_string()))?;
    let tmp_folder_delete = if tmp_folder_delete { 1 } else { 0 } as *const std::os::raw::c_int;
    let show_log = if show_log { 1 } else { 0 } as *const std::os::raw::c_int;
    //int sign_ipa(
    //             const char *ipaPath,
    //             const char *keyPath,
    //             const char *mpPath,
    //             const char *tmpFolderPath,
    //             const char *dylibFilePath,
    //             const char *appIconPath,
    //             const char *appName,
    //             const char *appVersion,
    //             const char *appBundleId,
    //             const bool tmpFolderDelete,
    //             const bool log,
    //             char *error
    //     );

    let mut error_mut: [std::os::raw::c_char; 1024] = [0; 1024];
    unsafe {
        sign_ipa(
            ipa_path.as_ptr(),
            key_path.as_ptr(),
            mp_path.as_ptr(),
            tmp_folder_path.as_ptr(),
            dylib_file_path.as_ptr(),
            app_name.as_ptr(),
            app_version.as_ptr(),
            app_bundle_id.as_ptr(),
            app_icon_path.as_ptr(),
            tmp_folder_delete,
            show_log,
            error_mut.as_mut_ptr(),
        );
        let error_msg = std::ffi::CStr::from_ptr(error_mut.as_ptr())
            .to_string_lossy()
            .to_string();
        if error_msg.is_empty() {
            return Ok(());
        }
        Err(ZsignError::Msg(error_msg))
    }
}
