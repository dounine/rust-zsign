/*!
# zsign
[![Latest Version](https://img.shields.io/crates/v/zsign.svg)](https://crates.io/crates/zsign)

zsign ipa签名

[![QQ群](https://img.shields.io/badge/QQ%E7%BE%A4-799168925-blue)](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=dLoye8pBcO60zGzqLjGO0l-GgMIaf6wQ&authKey=LfxBdZ5A%2F9eWJbKpzTcuWPjmQu5UdIJ3TVTpqRAQYkCID50WLkYoIXcGxGKzupG3&noverify=0&group_code=799168925)

# 使用指南
```
let ipa_path = std::ffi::CString::new("./ipa/video.ipa").unwrap();
let key_path = std::ffi::CString::new("./ipa/key.pem").unwrap();
let mp_path = std::ffi::CString::new("./ipa/lake_13_pm.mobileprovision").unwrap();
let dylib_file_path = std::ffi::CString::new("./ipa/d.dylib").unwrap();
let icon_path = std::ffi::CString::new("./ipa/icon.png").unwrap();
let tmp_folder_path = std::ffi::CString::new("./ipa/tmp").unwrap();

let mut error_mut: [std::os::raw::c_char; 1024] = [0; 1024];
unsafe {
    zsign::sign_ipa(ipa_path.as_ptr(), key_path.as_ptr(), mp_path.as_ptr(), dylib_file_path.as_ptr(), icon_path.as_ptr(), tmp_folder_path.as_ptr(), error_mut.as_mut_ptr());
}
```
*/
pub mod zsign;

pub use zsign::*;