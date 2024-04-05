#ifndef _ZSIGN_H

#ifdef __cplusplus
extern "C" {
#endif
void sign_ipa(
        const char *c_ipaPath,//签名的ipa路径
        const char *c_p12Path,//签名的p12路径
        const char *c_p12Password,//p12密码
        const char *c_mpPath,//mobileprovision路径
        const char *c_dylibFilePath,//dylib路径,目录也可以
        const char *c_dylibPrefixPath,//dylib注入位置
        const char *c_removeDylibPath,//删除的dylib路径,多个用逗号分隔
        const char *c_appName,//app名称
        const char *c_appVersion,//app版本
        const char *c_appBundleId,//app包名
        const char *c_appIconPath,//app图标路径
        const char *c_outputPath,//输出路径
        int deletePlugIns,//删除插件
        int deleteWatchPlugIns,//删除手表插件
        int deleteDeviceSupport,//删除设备机型限制
        int deleteSchemeURL,//删除schemeURL应用跳转
        int enableFileAccess,//是否启用文件访问
        int sign,//是否签名
        int zipLevel,//压缩等级1~9
        int zipIpa,//是否压缩Payload
        int showLog,//是否显示日志
        char *error//错误信息
);
#ifdef __cplusplus
}
#endif

#endif