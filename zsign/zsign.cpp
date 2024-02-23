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

int sign_ipa(
        const char *ipa_path,
        const char *key_path,
        const char *mp_path,
        const char *dylib_file_path,
        const char *icon_path,
        const char *tmp_folder_path,
        char *error
) {
    string tmpFolderPath = tmp_folder_path;
    string keyPath = key_path;
    string mpPath = mp_path;
    string ipaPath = ipa_path;
    string dylibFilePath = dylib_file_path;
    string iconPath = icon_path;

    ZSignAsset zSignAsset;
    if (!zSignAsset.Init("", keyPath, mpPath, "", "1")) {
//        cerr << "init sign asset failed" << endl;
        snprintf(error, 1024, "init sign asset failed");
        return -1;
    }

    ZTimer timer;
    if (tmpFolderPath.empty()) {
        StringFormat(tmpFolderPath, "/tmp/zsign_folder_%llu_%s", timer.Reset(), GenerateUUID().c_str());
    }

    CreateFolder(tmpFolderPath);

    ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());


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

    bool bRet = bundle.SignFolder(
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
    timer.PrintResult(bRet, "from sign.ipadump.com>>> Signed %s!", bRet ? "OK" : "Failed");
    return 1;
}

int main() {
    string keyPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/key.pem";
    string mpPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/lake_13_pm.mobileprovision";
    string ipaPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/video.ipa";
    string dylibFilePath = "/Users/lake/dounine/github/ipadump/zsign/ipa/d.dylib";
    string iconPath = "/Users/lake/dounine/github/ipadump/zsign/ipa/jpg_2_png.png";

    string tmpFolderPath = "/Users/lake/dounine/github/ipadump/zsign/tmp";


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

    ZLog::PrintV("signing ipa: %s \n", tmpFolderPath.c_str());


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

    bool bRet = bundle.SignFolder(
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
    timer.PrintResult(bRet, "from sign.ipadump.com>>> Signed %s!", bRet ? "OK" : "Failed");

    return 0;
}