#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"
#include <getopt.h>

const struct option options[] = {
        {"debug",          no_argument,       nullptr, 'd'},
        {"force",          no_argument,       nullptr, 'f'},
        {"unzip",          no_argument,       nullptr, 'u'},
        {"verbose",        no_argument,       nullptr, 'v'},
        {"cert",           required_argument, nullptr, 'c'},
        {"pkey",           required_argument, nullptr, 'k'},
        {"prov",           required_argument, nullptr, 'm'},
        {"password",       required_argument, nullptr, 'p'},
        {"bundle_id",      required_argument, nullptr, 'b'},
        {"bundle_name",    required_argument, nullptr, 'n'},
        {"bundle_version", required_argument, nullptr, 'r'},
        {"tmp_fold",       required_argument, nullptr, 't'},
        {"entitlements",   required_argument, nullptr, 'e'},
        {"output",         required_argument, nullptr, 'o'},
        {"zip_level",      required_argument, nullptr, 'z'},
        {"dylib",          required_argument, nullptr, 'l'},
        {"weak",           no_argument,       nullptr, 'w'},
        {"install",        no_argument,       nullptr, 'i'},
        {"quiet",          no_argument,       nullptr, 'q'},
        {"help",           no_argument,       nullptr, 'h'},
        {}};

int usage() {
    ZLog::Print(
            "from sign.ipadump.com Usage: sign [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder\n");
    ZLog::Print("options:\n");
    ZLog::Print("-k, --pkey\t\tPath to private key or p12 file. (PEM or DER format)\n");
    ZLog::Print("-m, --prov\t\tPath to mobile provisioning profile.\n");
    ZLog::Print("-c, --cert\t\tPath to certificate file. (PEM or DER format)\n");
    ZLog::Print("-d, --debug\t\tGenerate debug output files. (.zsign_debug folder)\n");
    ZLog::Print("-f, --force\t\tForce sign without cache when signing folder.\n");
    ZLog::Print("-u, --unzip\t\tSign unzip folder\n");
    ZLog::Print("-o, --output\t\tPath to output ipa file.\n");
    ZLog::Print("-p, --password\t\tPassword for private key or p12 file.\n");
    ZLog::Print("-b, --bundle_id\t\tNew bundle id to change.\n");
    ZLog::Print("-n, --bundle_name\tNew bundle name to change.\n");
    ZLog::Print("-r, --bundle_version\tNew bundle version to change.\n");
    ZLog::Print("-t, --tmp_fold\tunzip ipa tmp file will be to save here.\n");
    ZLog::Print("-e, --entitlements\tNew entitlements to change.\n");
    ZLog::Print("-z, --zip_level\t\tCompressed level when output the ipa file. (0-9)\n");
    ZLog::Print("-l, --dylib\t\tPath to inject dylib file.\n");
    ZLog::Print("-w, --weak\t\tInject dylib as LC_LOAD_WEAK_DYLIB.\n");
    ZLog::Print("-q, --quiet\t\tQuiet operation.\n");
    ZLog::Print("-v, --version\t\tShows version.\n");
    ZLog::Print("-h, --help\t\tShows help (this message).\n");

    return -1;
}

int _main(int argc, char *argv[]) {
    ZTimer gtimer;

    bool bForce = false;
    bool bWeakInject = false;
    uint32_t uZipLevel = -1;

    string strCertFile;
    string strPKeyFile;
    string strProvFile;
    string strPassword;
    string strBundleId;
    string strBundleVersion;
    string strDyLibFile;
    string strOutputFile;
    string strTmpFolder;
    string strDisplayName;
    string strEntitlementsFile;

    ZAppBundle bundle;

//    string ss = "/Users/lake/dounine/github/ipadump/sign-docker/lib/zsign/files/aa.ipa";
//    DIR *dir = opendir(ss.c_str());
//    if (NULL != dir) {
//        ZLog::PrintV("");
//        dirent *ptr = readdir(dir);
//
//        closedir(dir);
//    } else {
//
//    }
//    if (true) {
//        return 1;
//    }

    //打印文件夹下的所有文件路径
//    string path = "/Users/lake/dounine/github/ipadump/sign-docker/lib/zsign/common";
//    string basePath = "";
//    set<string> files;
//    bundle.GetFolderFiles(path, basePath, files);
//    //获取文件夹下的所有文件路径
//    DIR *dir = opendir(path.c_str());
//    if (NULL != dir) {
//        dirent *ptr = readdir(dir);
//        while (NULL != ptr) {
//            if (0 != strcmp(ptr->d_name, ".") && 0 != strcmp(ptr->d_name, "..")) {
//                string strNode = path;
//                strNode += "/";
//                strNode += ptr->d_name;
////                if (DT_DIR == ptr->d_type) {
////                    GetFolderFiles(strNode, strBaseFolder, setFiles);
//                if (DT_REG == ptr->d_type) {
//                    files.insert(strNode);
//                }
//            }
//            ptr = readdir(dir);
//        }
//        closedir(dir);
//    }
    //for files;
//    for (auto &file: files) {
//        ZLog::Print(file.c_str());
//        ZLog::Print("\n");
//    }


//    string hi = "";
//    string ss = StringFormat(hi,"%s/tmp/%s/hello","/files/uuid","hi");
//
//    //打印ss
//    ZLog::Print(ss.c_str());
//    if (true) {
//        return 1;
//    }

    int opt = 0;
    int argslot = -1;
    while (-1 != (opt = getopt_long(argc, argv, "dfvhc:k:t:r:m:o:p:e:b:n:z:ql:w", options, &argslot))) {
        switch (opt) {
            case 'd':
                ZLog::SetLogLever(ZLog::E_DEBUG);
                break;
            case 'f':
                bForce = true;
                break;
            case 'c':
                strCertFile = optarg;
                break;
            case 'k':
                strPKeyFile = optarg;
                break;
            case 'm':
                strProvFile = optarg;
                break;
            case 'p':
                strPassword = optarg;
                break;
            case 'b':
                strBundleId = optarg;
                break;
            case 'r':
                strBundleVersion = optarg;
                break;
            case 'n':
                strDisplayName = optarg;
                break;
            case 'e':
                strEntitlementsFile = optarg;
                break;
            case 'l':
                strDyLibFile = optarg;
                break;
            case 'o':
                strOutputFile = GetCanonicalizePath(optarg);
                break;
            case 't':
                strTmpFolder = GetCanonicalizePath(optarg);
                break;
            case 'z':
                uZipLevel = atoi(optarg);
                break;
            case 'w':
                bWeakInject = true;
                break;
            case 'q':
                ZLog::SetLogLever(ZLog::E_NONE);
                break;
            case 'v': {
                printf("version: 0.5\n");
                return 0;
            }
                break;
            case 'h':
            case '?':
                return usage();
                break;
        }

        ZLog::DebugV("Option:\t-%c, %s\n", opt, optarg);
    }

    if (optind >= argc) {
        return usage();
    }

    if (ZLog::IsDebug()) {
        CreateFolder("./.zsign_debug");
        for (int i = optind; i < argc; i++) {
            ZLog::DebugV("Argument:\t%s\n", argv[i]);
        }
    }

    string strPath = GetCanonicalizePath(argv[optind]);
    if (!IsFileExists(strPath.c_str())) {
        ZLog::ErrorV("Invalid Path! %s\n", strPath.c_str());
        return -1;
    }

    bool bZipFile = false;
    //只给macho文件使用、例子：./build/macosx/arm64/release/zsign -f -p 1 -l ./ipa/d.dylib -k ./ipa/key.pem -m ./ipa/lake_13_pm.mobileprovision -o ./ipa/hello.ipa ./ipa/Payload/FKCamera\ Full.app/FKCamera\ Full
    if (!IsFolder(strPath.c_str())) {
        ZLog::PrintV("------ 1 %s\n", strPath.c_str());
        bZipFile = IsZipFile(strPath.c_str());
        if (!bZipFile) { //macho file
            ZLog::PrintV("------ 2\n");
            ZMachO macho;
            if (macho.Init(strPath.c_str())) {
                if (!strDyLibFile.empty()) { //inject dylib
                    bool bCreate = false;
                    ZLog::PrintV("------ 3\n");
                    macho.InjectDyLib(bWeakInject, strDyLibFile.c_str(), bCreate);
                } else {
                    macho.PrintInfo();
                }
                macho.Free();
            }
            return 0;
        }
    }

    ZTimer timer;
    ZSignAsset zSignAsset;
    if (!zSignAsset.Init(strCertFile, strPKeyFile, strProvFile, strEntitlementsFile, strPassword)) {
        return -1;
    }

    ZLog::PrintV("strTmpFolder:%s \n", strTmpFolder.c_str());
    if (strTmpFolder.empty()) {
        StringFormat(strTmpFolder, "");
    }

    bool bEnableCache = true;
    string strFolder = strPath;
    if (bZipFile) { //ipa file
        bForce = true;
        bEnableCache = false;
        if (!strTmpFolder.empty()) {
            strFolder = strTmpFolder;
        } else {
            StringFormat(strFolder, "/tmp/zsign_folder_%llu", timer.Reset());
        }

        //strFolder目录不存在创建多级目录
        SystemExec("mkdir -p '%s'", strFolder.c_str());

        if (IsFolder(strPath)) {
            timer.PrintResult(true, "sing unzip folder，not unzip");
        } else {
            ZLog::PrintV("Unzip:\t%s (%s) -> %s ... \n", strPath.c_str(),
                         GetFileSizeString(strPath.c_str()).c_str(),
                         strFolder.c_str());
            RemoveFolder(strFolder.c_str());
            if (!SystemExec("unzip -qq -n -d '%s' '%s'", strFolder.c_str(), strPath.c_str())) {
                RemoveFolder(strFolder.c_str());
                ZLog::ErrorV("Unzip Failed!\n");
                return -1;
            }
            timer.PrintResult(true, "Unzip OK!");
        }
    }

    timer.Reset();

    //开始签名
    bool bRet = bundle.SignFolder(&zSignAsset, strFolder, strBundleId, strBundleVersion, strDisplayName,"", strDyLibFile,
                                  bForce, bWeakInject, bEnableCache);

    timer.PrintResult(bRet, "Signed %s!", bRet ? "OK" : "Failed");


    //如果不指定保存路径，保存到临时文件中
    if (strOutputFile.empty()) {
        StringFormat(strOutputFile, "/tmp/zsign_temp_%llu.ipa", GetMicroSecond());
    }


    //文件夹打包压缩
    if (!strOutputFile.empty() && uZipLevel != -1) {
        timer.Reset();
        size_t pos = bundle.m_strAppFolder.rfind("/Payload");
        if (string::npos == pos) {
            ZLog::Error("Can't Find Payload Directory!\n");
            return -1;
        }

        ZLog::PrintV("Archiving: \t%s ... \n", strOutputFile.c_str());
        string strBaseFolder = bundle.m_strAppFolder.substr(0, pos);
        char szOldFolder[PATH_MAX] = {0};
        if (nullptr != getcwd(szOldFolder, PATH_MAX)) {
            if (0 == chdir(strBaseFolder.c_str())) {
                uZipLevel = uZipLevel > 9 ? 9 : uZipLevel;
                RemoveFile(strOutputFile.c_str());
                SystemExec("zip -q -%u -r '%s' Payload", uZipLevel, strOutputFile.c_str());
                chdir(szOldFolder);
                if (!IsFileExists(strOutputFile.c_str())) {
                    ZLog::Error("Archive Failed!\n");
                    return -1;
                }
            }
        }
        timer.PrintResult(true, "Archive OK! (%s)",
                          GetFileSizeString(strOutputFile.c_str()).c_str());
    } else {
        timer.PrintResult(true, "Not Archive!");
    }

//    if (bRet && bInstall) {
//        SystemExec("ideviceinstaller -i '%s'", strOutputFile.c_str());
//    }

//    if (0 == strOutputFile.find("/tmp/zsign_tmp_")) {
//        RemoveFile(strOutputFile.c_str());
//    }

    //if (0 == strFolder.find( "/tmp/zsign_folder_")) {
//    RemoveFolder(strFolder.c_str());
    //}

    gtimer.Print("Done.");
    return bRet ? 0 : -1;
}
