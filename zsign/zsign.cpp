//
// Created by lake on 2024/2/2.
//
#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"
#include <getopt.h>
#include "zsign.h"
#include <iostream>
#include <string>

using namespace std;

void sign_ipa(
        const char *c_ipaPath,
        const char *c_keyPath,
        const char *c_mpPath,
        const char *c_tmpFolderPath,
        const char *c_dylibFilePath,
        const char *c_appName,
        const char *c_appVersion,
        const char *c_appBundleId,
        const char *c_appIconPath,
        int tmpFolderDelete,
        int showLog,
        char *error
) {
//    bool tmpFolderDelete=true;
//        bool showLog = true;
    string ipaPath = c_ipaPath;
    string keyPath = c_keyPath;
    string mpPath = c_mpPath;
    string tmpFolderPath = c_tmpFolderPath;
    string dylibFilePath = c_dylibFilePath;
    string iconPath = c_appIconPath;
    string appName = c_appName;
    string appVersion = c_appVersion;
    string appBundleId = c_appBundleId;
    ZSignAsset zSignAsset;
    if (!zSignAsset.Init("", keyPath, mpPath, "", "1")) {
        snprintf(error, 1024, "init sign asset failed");
        return;
    }

    ZTimer timer;
    if (tmpFolderPath.empty()) {
        const char* tempDir = getenv("TMPDIR");
        if (tempDir == nullptr) {
            tempDir = getenv("TEMP");
        }
        if (tempDir == nullptr) {
            tempDir = getenv("TMP");
        }
        if (tempDir == nullptr) {
            tempDir = "/tmp";
        }
        StringFormat(tmpFolderPath, "zsign_folder_%llu_%s", timer.Reset(), GenerateUUID().c_str());
        tmpFolderPath = tempDir + tmpFolderPath;
    }

    CreateFolder(tmpFolderPath);

    if (showLog) {
        ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());
    }


    if (IsZipFile(ipaPath)) {
        SystemExec("unzip -qq -n -d '%s' '%s'", tmpFolderPath.c_str(), ipaPath.c_str());
//        unzip(ipaPath, tmpFolderPath);
    }

    ZAppBundle bundle;

    if (!showLog) {
        bundle.DisableLog();
    }

    bool bRet;
    try {
        bool force = false;
        bool weakInject = false;
        bool enableCache = false;

        bRet = bundle.SignFolder(
                &zSignAsset,
                tmpFolderPath,
                appBundleId,
                appVersion,
                appName,
                iconPath,
                dylibFilePath,
                force,
                weakInject,
                enableCache
        );
    } catch (string e) {
        snprintf(error, 1024, "%s", e.c_str());
        bRet = false;
    }
    if (tmpFolderDelete) {
        RemoveFolder(tmpFolderPath.c_str());
    }
    if (showLog) {
        timer.PrintResult(bRet, "Signed %s!", bRet ? "OK" : "Failed");
    }
}

int main() {
    string keyPath = "/Users/lake/dounine/github/rust/rust-zsign/ipa/key.pem";
    string mpPath = "/Users/lake/dounine/github/rust/rust-zsign/ipa/lake_13_pm.mobileprovision";
    string ipaPath = "/Users/lake/dounine/github/rust/rust-zsign/ipa/video.ipa";
    string dylibFilePath = "/Users/lake/dounine/github/rust/rust-zsign/ipa/libs";
    string iconPath = "/Users/lake/dounine/github/rust/rust-zsign/ipa/icon.png";

    string tmpFolderPath = "./tmp";

    sign_ipa(
            ipaPath.c_str(),
            keyPath.c_str(),
            mpPath.c_str(),
            tmpFolderPath.c_str(),
            dylibFilePath.c_str(),
            "你好",
            "1.0",
            "com.lake.video",
            iconPath.c_str(),
            true,
            true,
            nullptr);

    if (true) {
        return 0;
    }

    ZSignAsset zSignAsset;
    if (!zSignAsset.Init("", keyPath, mpPath, "", "1")) {
        cerr << "init sign asset failed" << endl;
        return -1;
    }

    ZTimer timer;
    if (tmpFolderPath.empty()) {
        StringFormat(tmpFolderPath, "/tmp/zsign_folder_%llu_%s", timer.Reset(), GenerateUUID().c_str());
    }

    CreateFolder(tmpFolderPath);

//    ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());


    if (IsZipFile(ipaPath)) {
        SystemExec("unzip -qq -n -d '%s' '%s'", tmpFolderPath.c_str(), ipaPath.c_str());
//        unzip(ipaPath, tmpFolderPath);
    }

    string appBundleId;
    string appVersion;
    string appName = "你好";

    bool force = false;
    bool weakInject = false;
    bool enableCache = false;

    ZAppBundle bundle;

//    bundle.DisableLog();

//    char *error = nullptr;

    bool bRet;
    try {
        bRet = bundle.SignFolder(
                &zSignAsset,
                tmpFolderPath,
                appBundleId,
                appVersion,
                appName,
                iconPath,
                dylibFilePath,
                force,
                weakInject,
                enableCache
        );
    } catch (const string e) {
        cout << "sign failed: " << e << endl;
    }
    timer.PrintResult(bRet, "签名%s!", bRet ? "成功" : "失败");

    return 0;
}