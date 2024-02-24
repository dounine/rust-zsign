/*!
# zsign
[![Latest Version](https://img.shields.io/crates/v/zsign.svg)](https://crates.io/crates/zsign)

zsign ipa签名

[![QQ群](https://img.shields.io/badge/QQ%E7%BE%A4-799168925-blue)](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=dLoye8pBcO60zGzqLjGO0l-GgMIaf6wQ&authKey=LfxBdZ5A%2F9eWJbKpzTcuWPjmQu5UdIJ3TVTpqRAQYkCID50WLkYoIXcGxGKzupG3&noverify=0&group_code=799168925)

# 使用指南
```
use zsign::ZsignBuilder;
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
* ```
*/
pub mod zsign;
pub mod error;

pub use zsign::*;