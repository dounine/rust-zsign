#pragma once

#include "common/common.h"
#include "common/json.h"
#include "openssl.h"

class ZAppBundle {
public:
    ZAppBundle();

public:
    void DisableLog();

    bool SignFolder(ZSignAsset *pSignAsset,
                    const string &strFolder,
                    const string &strBundleID,
                    const string &strBundleVersion,
                    const string &strDisplayName,
                    const string &strIconPath,
                    const string &strDyLibFile,
                    const string &strDylibPrefix,
                    const string &removeDylibPath,
                    bool deletePlugIns,
                    bool deleteWatchPlugins,
                    bool deleteDeviceSupport,
                    bool deleteSchemeUrl,
                    bool enableFileAccess,
                    bool sign,
                    bool bForce,
                    bool bWeakInject,
                    bool bEnableCache);

private:
    bool SignNode(JValue &jvNode, bool sign);

    void GetNodeChangedFiles(JValue &jvNode);

    void GetChangedFiles(JValue &jvNode, vector<string> &arrChangedFiles);

    void GetPlugIns(const string &strFolder, vector<string> &arrPlugIns);

private:
    bool FindAppFolder(const string &strFolder, string &strAppFolder);

    bool GetObjectsToSign(const string &strFolder, JValue &jvInfo);

    bool GetSignFolderInfo(const string &strFolder, JValue &jvNode, bool bGetName = false);

    void GetFolderFiles(const string &strFolder, const string &strBaseFolder, set<string> &setFiles);

private:
    bool GenerateCodeResources(const string &strFolder, JValue &jvCodeRes);

private:
    bool m_bForceSign;
    bool m_bWeakInject;
    bool m_show_log = true;
    string m_strDyLibPath;
    set<string> dylibPaths;
    set<string> removeDylibPaths;
    ZSignAsset *m_pSignAsset;

public:
    string m_strAppFolder;
};
