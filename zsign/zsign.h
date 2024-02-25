#ifndef _ZSIGN_H

#ifdef __cplusplus
extern "C" {
#endif
    void sign_ipa(
            const char *c_ipaPath,
            const char *c_keyPath,
            const char *c_mpPath,
            const char *c_dylibFilePath,
            const char *c_appName,
            const char *c_appVersion,
            const char *c_appBundleId,
            const char *c_appIconPath,
            const char *c_outputPath,
            int zipLevel,
            int zipIpa,
            int showLog,
            char *error
    );
#ifdef __cplusplus
}
#endif

#endif