use crate::error::ZsignError;

#[derive(Debug)]
pub struct ZsignBuilder {
    dylib_file_path: Option<String>,
    app_icon_path: Option<String>,
    app_name: Option<String>,
    app_version: Option<String>,
    app_bundle_id: Option<String>,
    show_log: bool,
    zip_ipa: bool,
    zip_level: u8,
}

unsafe impl Send for ZsignBuilder {}

unsafe impl Sync for ZsignBuilder {}

include!("../bindings/bindings.rs");

impl ZsignBuilder {
    pub fn new() -> Self {
        Self {
            dylib_file_path: None,
            app_icon_path: None,
            app_name: None,
            app_version: None,
            app_bundle_id: None,
            show_log: true,
            zip_ipa: true,
            zip_level: 3,
        }
    }
    pub fn dylib_file_path<T: AsRef<str>>(mut self, dylib_file_path: T) -> Self {
        self.dylib_file_path = Some(dylib_file_path.as_ref().to_string());
        self
    }
    pub fn app_icon_path<T: AsRef<str>>(mut self, app_icon_path: T) -> Self {
        self.app_icon_path = Some(app_icon_path.as_ref().to_string());
        self
    }
    pub fn app_name<T: AsRef<str>>(mut self, app_name: T) -> Self {
        self.app_name = Some(app_name.as_ref().to_string());
        self
    }
    pub fn app_version<T: AsRef<str>>(mut self, app_version: T) -> Self {
        self.app_version = Some(app_version.as_ref().to_string());
        self
    }
    pub fn app_bundle_id<T: AsRef<str>>(mut self, app_bundle_id: T) -> Self {
        self.app_bundle_id = Some(app_bundle_id.as_ref().to_string());
        self
    }
    pub fn zip_ipa(mut self) -> Self {
        self.zip_ipa = true;
        self
    }
    pub fn no_zip_ipa(mut self) -> Self {
        self.zip_ipa = false;
        self
    }
    pub fn show_log(mut self) -> Self {
        self.show_log = true;
        self
    }
    pub fn no_show_log(mut self) -> Self {
        self.show_log = false;
        self
    }
    pub fn sign<T>(self, ipa_path: T, key_path: T, mp_path: T, output_path: T) -> Result<(), ZsignError>
        where T: AsRef<str>
    {
        let dylib_file_path_default = self.dylib_file_path
            .unwrap_or_default();
        let app_icon_path_default = self.app_icon_path
            .unwrap_or_default();
        let ipa_path =
            std::ffi::CString::new(ipa_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let key_path =
            std::ffi::CString::new(key_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let mp_path =
            std::ffi::CString::new(mp_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let dylib_file_path = std::ffi::CString::new(dylib_file_path_default)
            .map_err(|e| ZsignError::Msg(e.to_string()))?;
        let app_icon_path = std::ffi::CString::new(app_icon_path_default)
            .map_err(|e| ZsignError::Msg(e.to_string()))?;
        let output_path = std::ffi::CString::new(output_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let app_name_default = self.app_name.unwrap_or_default();
        let app_name =
            std::ffi::CString::new(app_name_default).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let app_version_default = self.app_version
            .unwrap_or_default();
        let app_version =
            std::ffi::CString::new(app_version_default).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let app_bundle_id_default = self.app_bundle_id
            .unwrap_or_default();
        let app_bundle_id = std::ffi::CString::new(app_bundle_id_default)
            .map_err(|e| ZsignError::Msg(e.to_string()))?;
        let zip_level = std::ffi::c_int::from(self.zip_level);
        let zip_ipa = if self.zip_ipa {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let show_log = if self.show_log {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let mut error_mut: [std::os::raw::c_char; 1024] = [0; 1024];
        unsafe {
            sign_ipa(
                ipa_path.as_ptr(),
                key_path.as_ptr(),
                mp_path.as_ptr(),
                dylib_file_path.as_ptr(),
                app_name.as_ptr(),
                app_version.as_ptr(),
                app_bundle_id.as_ptr(),
                app_icon_path.as_ptr(),
                output_path.as_ptr(),
                zip_level,
                zip_ipa,
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
}


