use crate::error::ZsignError;

#[derive(Debug)]
pub struct ZsignBuilder {
    dylib_file_path: Option<String>,
    dylib_prefix_path: Option<String>,
    app_icon_path: Option<String>,
    app_name: Option<String>,
    app_version: Option<String>,
    app_bundle_id: Option<String>,
    delete_plugins: bool,
    delete_watch_plugins: bool,
    delete_device_support: bool,
    delete_scheme_url: bool,
    enable_file_access: bool,
    sign: bool,
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
            dylib_file_path: None,//动态库路径,如果是目录则会遍历目录下所有dylib文件
            dylib_prefix_path: Some("@executable_path/".to_string()),//动态库注入路径,默认为@executable_path/["@executable_path/","@rpath/","@executable_path/Framework/","@rpath/Framework/"]
            app_icon_path: None,//图标
            app_name: None,//名称
            app_version: None,//版本
            app_bundle_id: None,//包id
            delete_plugins: true,//删除插件
            delete_watch_plugins: true,//删除watch插件
            delete_device_support: true,//删除device_support设备限制
            delete_scheme_url: false,//删除scheme跳转
            enable_file_access: false,//是否开启文件访问
            sign: true,//是否签名，还是修改配置
            show_log: true,//显示zsign日志
            zip_ipa: true,//是否zip压缩Payload
            zip_level: 3,//压缩级别
        }
    }
    pub fn enable_file_access(mut self) -> Self {
        self.enable_file_access = true;
        self
    }
    pub fn disable_file_access(mut self) -> Self {
        self.enable_file_access = false;
        self
    }
    pub fn dylib_prefix_path<T: AsRef<str>>(mut self, dylib_prefix_path: T) -> Self {
        self.dylib_prefix_path = Some(dylib_prefix_path.as_ref().to_string());
        self
    }
    pub fn disable_sign(mut self) -> Self {
        self.sign = false;
        self
    }
    pub fn zip_level(mut self, zip_level: u8) -> Self {
        self.zip_level = zip_level;
        self
    }
    pub fn delete_device_support(mut self) -> Self {
        self.delete_device_support = true;
        self
    }
    pub fn disable_delete_device_support(mut self) -> Self {
        self.delete_device_support = false;
        self
    }
    pub fn delete_scheme_url(mut self) -> Self {
        self.delete_scheme_url = true;
        self
    }
    pub fn disable_delete_scheme_url(mut self) -> Self {
        self.delete_scheme_url = false;
        self
    }
    pub fn delete_plugins(mut self) -> Self {
        self.delete_plugins = true;
        self
    }
    pub fn disable_delete_plugins(mut self) -> Self {
        self.delete_plugins = false;
        self
    }
    pub fn delete_watch_plugins(mut self) -> Self {
        self.delete_watch_plugins = true;
        self
    }
    pub fn disable_delete_watch_plugins(mut self) -> Self {
        self.delete_watch_plugins = false;
        self
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
    pub fn disable_zip_ipa(mut self) -> Self {
        self.zip_ipa = false;
        self
    }
    pub fn show_log(mut self) -> Self {
        self.show_log = true;
        self
    }
    pub fn disable_show_log(mut self) -> Self {
        self.show_log = false;
        self
    }
    pub fn build<T>(self, ipa_path: T, p12_path: T, p12_password: T, mp_path: T, output_path: T) -> Result<(), ZsignError>
        where T: AsRef<str>
    {
        let dylib_file_path_default = self.dylib_file_path
            .unwrap_or_default();
        let dylib_prefix_path = std::ffi::CString::new(self.dylib_prefix_path
            .unwrap_or("@executable_path/".to_string()))
            .map_err(|e| ZsignError::Msg(e.to_string()))?;
        let app_icon_path_default = self.app_icon_path
            .unwrap_or_default();
        let ipa_path =
            std::ffi::CString::new(ipa_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let p12_path =
            std::ffi::CString::new(p12_path.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
        let p12_password =
            std::ffi::CString::new(p12_password.as_ref()).map_err(|e| ZsignError::Msg(e.to_string()))?;
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
        let zip_level = std::ffi::c_int::from(self.zip_level as i8);
        let delete_plugins = if self.delete_plugins {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let enable_file_access = if self.enable_file_access {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let sign = if self.sign {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let delete_watch_plugins = if self.delete_watch_plugins {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let delete_device_support = if self.delete_device_support {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
        let delete_scheme_url = if self.delete_scheme_url {
            std::ffi::c_int::from(1)
        } else {
            std::ffi::c_int::from(0)
        };
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
                p12_path.as_ptr(),
                p12_password.as_ptr(),
                mp_path.as_ptr(),
                dylib_file_path.as_ptr(),
                dylib_prefix_path.as_ptr(),
                app_name.as_ptr(),
                app_version.as_ptr(),
                app_bundle_id.as_ptr(),
                app_icon_path.as_ptr(),
                output_path.as_ptr(),
                delete_plugins,
                delete_watch_plugins,
                delete_device_support,
                delete_scheme_url,
                enable_file_access,
                sign,
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

pub fn p12_parse() {}


