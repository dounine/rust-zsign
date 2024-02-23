#include "common.h"
#include "base64.h"
#include <sys/stat.h>
#include <inttypes.h>
//#include "../zip/zip.h"
#include <iostream>
#include <fstream>
#include <openssl/sha.h>

#define PARSEVALIST(szFormatArgs, szArgs)                       \
    ZBuffer buffer;                                             \
    char szBuffer[PATH_MAX] = {0};                              \
    char *szArgs = szBuffer;                                    \
    va_list args;                                               \
    va_start(args, szFormatArgs);                               \
    int nRet = vsnprintf(szArgs, PATH_MAX, szFormatArgs, args); \
    va_end(args);                                               \
    if (nRet > PATH_MAX - 1)                                    \
    {                                                           \
        char *szNewBuffer = buffer.GetBuffer(nRet + 1);         \
        if (NULL != szNewBuffer)                                \
        {                                                       \
            szArgs = szNewBuffer;                               \
            va_start(args, szFormatArgs);                       \
            vsnprintf(szArgs, nRet + 1, szFormatArgs, args);    \
            va_end(args);                                       \
        }                                                       \
    }

bool IsRegularFile(const char *file) {
    struct stat info{};
    stat(file, &info);
    return S_ISREG(info.st_mode);
}

void *MapFile(const char *path, size_t offset, size_t size, size_t *psize, bool ro) {
    int fd = open(path, ro ? O_RDONLY : O_RDWR);
    if (fd <= 0) {
        return nullptr;
    }

    if (0 == size) {
        struct stat stat{};
        fstat(fd, &stat);
        size = stat.st_size;
    }

    if (nullptr != psize) {
        *psize = size;
    }

    void *base = mmap(nullptr, size, ro ? PROT_READ : PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    close(fd);

    if (MAP_FAILED == base) {
        base = nullptr;
    }

    return base;
}

bool WriteFile(const char *szFile, const char *szData, size_t sLen) {
    if (nullptr == szFile) {
        return false;
    }

    FILE *fp = fopen(szFile, "wb");
    if (nullptr != fp) {
        int64_t towrite = sLen;
        if (nullptr != szData) {
            while (towrite > 0) {
                int64_t nwrite = fwrite(szData + (sLen - towrite), 1, towrite, fp);
                if (nwrite <= 0) {
                    break;
                }
                towrite -= nwrite;
            }
        }
        fclose(fp);
        return towrite <= 0;
    } else {
        ZLog::ErrorV("WriteFile: Failed in fopen! %s, %s\n", szFile, strerror(errno));
    }

    return false;
}

bool WriteFile(const char *szFile, const string &strData) {
    return WriteFile(szFile, strData.data(), strData.size());
}

bool WriteFile(string &strData, const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szPath)
    return WriteFile(szPath, strData);
}

bool WriteFile(const char *szData, size_t sLen, const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szPath)
    return WriteFile(szPath, szData, sLen);
}

bool ReadFile(const char *szFile, string &strData) {
    strData.clear();

    if (!IsFileExists(szFile)) {
        return false;
    }

    FILE *fp = fopen(szFile, "rb");
    if (nullptr != fp) {
        strData.reserve(GetFileSize(fileno(fp)));

        char buf[4096] = {0};
        size_t nread = fread(buf, 1, 4096, fp);
        while (nread > 0) {
            strData.append(buf, nread);
            nread = fread(buf, 1, 4096, fp);
        }
        fclose(fp);
        return true;
    } else {
        ZLog::ErrorV("ReadFile: Failed in fopen! %s, %s\n", szFile, strerror(errno));
    }

    return false;
}

bool ReadFile(string &strData, const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szPath)
    return ReadFile(szPath, strData);
}

bool AppendFile(const char *szFile, const char *szData, size_t sLen) {
    FILE *fp = fopen(szFile, "ab+");
    if (nullptr != fp) {
        int64_t towrite = sLen;
        while (towrite > 0) {
            int64_t nwrite = fwrite(szData + (sLen - towrite), 1, towrite, fp);
            if (nwrite <= 0) {
                break;
            }
            towrite -= nwrite;
        }

        fclose(fp);
        return (towrite > 0) ? false : true;
    } else {
        ZLog::ErrorV("AppendFile: Failed in fopen! %s, %s\n", szFile, strerror(errno));
    }
    return false;
}

bool AppendFile(const char *szFile, const string &strData) {
    return AppendFile(szFile, strData.data(), strData.size());
}

bool IsFolder(const string &szFolder) {
    struct stat st{};
    stat(szFolder.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

bool IsFolderV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFolder)
    return IsFolder(szFolder);
}

bool CreateFolder(const string &szFolder) {
    if (!IsFolder(szFolder)) {
#if defined(WINDOWS)
        return (0 == mkdir(szFolder));
#else
        return (0 == mkdir(szFolder.c_str(), 0755));
#endif
    }
    return false;
}

bool CreateFolders(const string &folderPath) {
    if (folderPath.empty()) {
        return false;
    }
    string folder = folderPath;
    if (folder[folder.size() - 1] != '/') {
        folder += "/";
    }
    size_t pos = 0;
    while (true) {
        pos = folder.find('/', pos + 1);
        if (pos == string::npos) {
            break;
        }
        string subFolder = folder.substr(0, pos);
        if (subFolder != ".") {
            if (!IsFolderExists(subFolder)) {
                if (!CreateFolder(subFolder)) {
                    return false;
                }
            }
        }
    }
    return true;

}

string GenerateUUID() {
    string uuid;
    uuid.resize(36);
    for (int i = 0; i < 36; i++) {
        if (i == 14) {
            uuid[i] = '4';
        } else if (i == 19) {
            uuid[i] = '8';
        } else {
            uuid[i] = "0123456789abcdef"[rand() % 16];
        }
    }
    return uuid;
}

bool CopyFile(const string &srcFile, const string &destFile) {
    // 打开源文件
    std::ifstream sourceFile(srcFile, std::ios::binary);
    if (!sourceFile.is_open()) {
        ZLog::ErrorV("文件不能打开: %s\n", srcFile.c_str());
        return false;
    }

    // 创建目标文件
    std::ofstream destinationFile(destFile, std::ios::binary);
    if (!destinationFile.is_open()) {
        ZLog::ErrorV("文件不能创建: %s\n", destFile.c_str());
        return false;
    }

    // 从源文件读取内容并写入目标文件
    destinationFile << sourceFile.rdbuf();

    // 关闭文件
    sourceFile.close();
    destinationFile.close();
    return true;
}

bool CreateFolderV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFolder)
    return CreateFolder(szFolder);
}

int RemoveFolderCallBack(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    int ret = remove(fpath);
    if (ret) {
        perror(fpath);
    }
    return ret;
}

bool RemoveFolder(const char *szFolder) {
    if (!IsFolder(szFolder)) {
        RemoveFile(szFolder);
        return true;
    }
    return nftw(szFolder, RemoveFolderCallBack, 64, FTW_DEPTH | FTW_PHYS);
}

bool RemoveFolderV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFolder)
    return RemoveFolder(szFolder);
}

bool RemoveFile(const char *szFile) {
    return (0 == remove(szFile));
}

bool RemoveFileV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFile);
    return RemoveFile(szFile);
}

//int on_extract_entry(const char *filename, void *arg) {
//    static int i = 0;
//    int n = *(int *) arg;
//    ZLog::PrintV("解压: %s (%d of %d)\n", filename, ++i, n);
//    return 0;
//}
//
//void unzip(const string &zipFilePath, const string &destFolderPath) {
//    int a = 0;
//    zip_extract(zipFilePath.c_str(), destFolderPath.c_str(), on_extract_entry, &a);
//}
//
//void zip(const string &filePath, const string &destFilePath) {
//
//}

bool IsFileExists(const char *szFile) {
    if (nullptr == szFile) {
        return false;
    }
    return (0 == access(szFile, F_OK));
}

bool IsFolderExists(const string &szFolder) {
    if (szFolder.empty()) {
        return false;
    }
    struct stat st{};
    ZLog::PrintV("IsFolderExists: %s %i\n", szFolder.c_str(), stat(szFolder.c_str(), &st));
    return 1 == stat(szFolder.c_str(), &st);
}

bool IsFileExistsV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFile)
    return IsFileExists(szFile);
}

bool IsZipFile(const string &szFile) {
    if (!szFile.empty() && !IsFolder(szFile)) {
        FILE *fp = fopen(szFile.c_str(), "rb");
        if (nullptr != fp) {
            uint8_t buf[2] = {0};
            fread(buf, 1, 2, fp);
            fclose(fp);
            return (0 == memcmp("PK", buf, 2));
        }
    }
    return false;
}

#define PATH_BUFFER_LENGTH 1024

string GetCanonicalizePath(const char *szPath) {
    string strPath = szPath;
    if (!strPath.empty()) {
        if ('/' != szPath[0]) {
            char path[PATH_MAX] = {0};

#if defined(WINDOWS)

            if (NULL != _fullpath((char *)"./", path, PATH_BUFFER_LENGTH))
            {
                strPath = path;
                strPath += "/";
                strPath += szPath;
            }
#else
            if (nullptr != realpath("./", path)) {
                strPath = path;
                strPath += "/";
                strPath += szPath;
            }
#endif
        }
        StringReplace(strPath, "/./", "/");
    }
    return strPath;
}

int64_t GetFileSize(int fd) {
    int64_t nSize = 0;
    struct stat stbuf{};
    if (0 == fstat(fd, &stbuf)) {
        if (S_ISREG(stbuf.st_mode)) {
            nSize = stbuf.st_size;
        }
    }
    return (nSize < 0 ? 0 : nSize);
}

int64_t GetFileSize(const char *szFile) {
    int64_t nSize = 0;
    int fd = open(szFile, O_RDONLY);
    if (fd >= 0) {
        nSize = GetFileSize(fd);
        close(fd);
    }
    return nSize;
}

int64_t GetFileSizeV(const char *szFormatPath, ...) {
    PARSEVALIST(szFormatPath, szFile)
    return GetFileSize(szFile);
}

string GetFileSizeString(const char *szFile) {
    return FormatSize(GetFileSize(szFile), 1024);
}

string FormatSize(int64_t size, int64_t base) {
    double fsize = 0;
    char ret[64] = {0};
    if (size > base * base * base * base) {
        fsize = (size * 1.0) / (base * base * base * base);
        snprintf(ret, sizeof(ret), "%.2f TB", fsize);
    } else if (size > base * base * base) {
        fsize = (size * 1.0) / (base * base * base);
        snprintf(ret, sizeof(ret), "%.2f GB", fsize);
    } else if (size > base * base) {
        fsize = (size * 1.0) / (base * base);
        snprintf(ret, sizeof(ret), "%.2f MB", fsize);
    } else if (size > base) {
        fsize = (size * 1.0) / (base);
        snprintf(ret, sizeof(ret), "%.2f KB", fsize);
    } else {
        snprintf(ret, sizeof(ret), "%" PRId64 " B", size);
    }
    return ret;
}

bool IsPathSuffix(const string &strPath, const char *suffix) {
    size_t nPos = strPath.rfind(suffix);
    if (string::npos != nPos) {
        if (nPos == (strPath.size() - strlen(suffix))) {
            return true;
        }
    }
    return false;
}

time_t GetUnixStamp() {
    time_t ustime = 0;
    time(&ustime);
    return ustime;
}

uint64_t GetMicroSecond() {
    struct timeval tv = {0};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

bool SystemExec(const char *szFormatCmd, ...) {
    PARSEVALIST(szFormatCmd, szCmd)

    if (strlen(szCmd) <= 0) {
        return false;
    }

    int status = system(szCmd);

    if (-1 == status) {
        ZLog::ErrorV("SystemExec: \"%s\", Error!\n", szCmd);
        return false;
    } else {
#if !defined(WINDOWS)
        if (WIFEXITED(status)) {
            if (0 == WEXITSTATUS(status)) {
                return true;
            } else {
                ZLog::ErrorV("SystemExec: \"%s\", Failed! Exit-Status: %d\n", szCmd, WEXITSTATUS(status));
                return false;
            }
        } else {
            return true;
        }
#endif
    }
    return false;
}

uint16_t _Swap(uint16_t value) {
    return ((value >> 8) & 0x00ff) |
           ((value << 8) & 0xff00);
}

uint32_t _Swap(uint32_t value) {
    value = ((value >> 8) & 0x00ff00ff) |
            ((value << 8) & 0xff00ff00);
    value = ((value >> 16) & 0x0000ffff) |
            ((value << 16) & 0xffff0000);
    return value;
}

uint64_t _Swap(uint64_t value) {
    value = (value & 0x00000000ffffffffULL) << 32 | (value & 0xffffffff00000000ULL) >> 32;
    value = (value & 0x0000ffff0000ffffULL) << 16 | (value & 0xffff0000ffff0000ULL) >> 16;
    value = (value & 0x00ff00ff00ff00ffULL) << 8 | (value & 0xff00ff00ff00ff00ULL) >> 8;
    return value;
}

uint32_t ByteAlign(uint32_t uValue, uint32_t uAlign) {
    return (uValue + (uAlign - uValue % uAlign));
}

const char *StringFormat(string &strFormat, const char *szFormatArgs, ...) {
    PARSEVALIST(szFormatArgs, szFormat)
    strFormat = szFormat;
    return strFormat.c_str();
}

string &StringReplace(string &context, const string &from, const string &to) {
    size_t lookHere = 0;
    size_t foundHere;
    while ((foundHere = context.find(from, lookHere)) != string::npos) {
        context.replace(foundHere, from.size(), to);
        lookHere = foundHere + to.size();
    }
    return context;
}

void StringSplit(const string &src, const string &split, vector<string> &dest) {
    size_t oldPos = 0;
    size_t newPos = src.find(split, oldPos);
    while (newPos != string::npos) {
        dest.push_back(src.substr(oldPos, newPos - oldPos));
        oldPos = newPos + split.size();
        newPos = src.find(split, oldPos);
    }
    if (oldPos < src.size()) {
        dest.push_back(src.substr(oldPos));
    }
}

bool SHA1Text(const string &strData, string &strOutput) {
    string strSHASum;
    SHASum(E_SHASUM_TYPE_1, strData, strSHASum);

    strOutput.clear();
    char buf[16] = {0};
    for (size_t i = 0; i < strSHASum.size(); i++) {
        sprintf(buf, "%02x", (uint8_t) strSHASum[i]);
        strOutput += buf;
    }
    return (!strOutput.empty());
}

void PrintSHASum(const char *prefix, const uint8_t *hash, uint32_t size, const char *suffix) {
    ZLog::PrintV("%s", prefix);
    for (uint32_t i = 0; i < size; i++) {
        ZLog::PrintV("%02x", hash[i]);
    }
    ZLog::PrintV("%s", suffix);
}

void PrintSHASum(const char *prefix, const string &strSHASum, const char *suffix) {
    PrintSHASum(prefix, (const uint8_t *) strSHASum.data(), strSHASum.size(), suffix);
}

void PrintDataSHASum(const char *prefix, int nSumType, const string &strData, const char *suffix) {
    string strSHASum;
    SHASum(nSumType, strData, strSHASum);
    PrintSHASum(prefix, strSHASum, suffix);
}

void PrintDataSHASum(const char *prefix, int nSumType, uint8_t *data, size_t size, const char *suffix) {
    string strSHASum;
    SHASum(nSumType, data, size, strSHASum);
    PrintSHASum(prefix, strSHASum, suffix);
}

bool SHASum(int nSumType, uint8_t *data, size_t size, string &strOutput) {
    strOutput.clear();
    if (1 == nSumType) {
        uint8_t hash[20];
        memset(hash, 0, 20);
        SHA1(data, size, hash);
        strOutput.append((const char *) hash, 20);
    } else {
        uint8_t hash[32];
        memset(hash, 0, 32);
        SHA256(data, size, hash);
        strOutput.append((const char *) hash, 32);
    }
    return true;
}

bool SHASum(int nSumType, const string &strData, string &strOutput) {
    return SHASum(nSumType, (uint8_t *) strData.data(), strData.size(), strOutput);
}

bool SHASum(const string &strData, string &strSHA1, string &strSHA256) {
    SHASum(E_SHASUM_TYPE_1, strData, strSHA1);
    SHASum(E_SHASUM_TYPE_256, strData, strSHA256);
    return (!strSHA1.empty() && !strSHA256.empty());
}

bool SHASumFile(const char *szFile, string &strSHA1, string &strSHA256) {
    size_t sSize = 0;
    uint8_t *pBase = (uint8_t *) MapFile(szFile, 0, 0, &sSize, true);

    SHASum(E_SHASUM_TYPE_1, pBase, sSize, strSHA1);
    SHASum(E_SHASUM_TYPE_256, pBase, sSize, strSHA256);

    if (NULL != pBase && sSize > 0) {
        munmap(pBase, sSize);
    }
    return (!strSHA1.empty() && !strSHA256.empty());
}

bool SHASumBase64(const string &strData, string &strSHA1Base64, string &strSHA256Base64) {
    ZBase64 b64;
    string strSHA1;
    string strSHA256;
    SHASum(strData, strSHA1, strSHA256);
    strSHA1Base64 = b64.Encode(strSHA1);
    strSHA256Base64 = b64.Encode(strSHA256);
    return (!strSHA1Base64.empty() && !strSHA256Base64.empty());
}

bool SHASumBase64File(const char *szFile, string &strSHA1Base64, string &strSHA256Base64) {
    ZBase64 b64;
    string strSHA1;
    string strSHA256;
    SHASumFile(szFile, strSHA1, strSHA256);
    strSHA1Base64 = b64.Encode(strSHA1);
    strSHA256Base64 = b64.Encode(strSHA256);
    return (!strSHA1Base64.empty() && !strSHA256Base64.empty());
}

ZBuffer::ZBuffer() {
    m_pData = nullptr;
    m_uSize = 0;
}

ZBuffer::~ZBuffer() {
    Free();
}

char *ZBuffer::GetBuffer(uint32_t uSize) {
    if (uSize <= m_uSize) {
        return m_pData;
    }

    char *pData = (char *) realloc(m_pData, uSize);
    if (nullptr == pData) {
        Free();
        return nullptr;
    }

    m_pData = pData;
    m_uSize = uSize;
    return m_pData;
}

void ZBuffer::Free() {
    if (nullptr != m_pData) {
        free(m_pData);
    }
    m_pData = nullptr;
    m_uSize = 0;
}

ZTimer::ZTimer() {
    Reset();
}

uint64_t ZTimer::Reset() {
    m_uBeginTime = GetMicroSecond();
    return m_uBeginTime;
}

uint64_t ZTimer::Print(const char *szFormatArgs, ...) {
    PARSEVALIST(szFormatArgs, szFormat)
    uint64_t uElapse = GetMicroSecond() - m_uBeginTime;
    ZLog::PrintV("%s (%.03fs, %lluus)\n", szFormat, uElapse / 1000000.0, uElapse);
    return Reset();
}

uint64_t ZTimer::PrintResult(bool bSuccess, const char *szFormatArgs, ...) {
    PARSEVALIST(szFormatArgs, szFormat)
    uint64_t uElapse = GetMicroSecond() - m_uBeginTime;
    ZLog::PrintResultV(bSuccess, "%s (%.03fs, %lluus)\n", szFormat, uElapse / 1000000.0, uElapse);
    return Reset();
}

int ZLog::g_nLogLevel = ZLog::E_INFO;

void ZLog::SetLogLever(int nLogLevel) {
    g_nLogLevel = nLogLevel;
}

void ZLog::Print(int nLevel, const char *szLog) {
    if (g_nLogLevel >= nLevel) {
        write(STDOUT_FILENO, szLog, strlen(szLog));
    }
}

void ZLog::PrintV(int nLevel, const char *szFormatArgs, ...) {
    if (g_nLogLevel >= nLevel) {
        const char *logPrefix = "sign.ipadump.com：";
        char *szFormatArgsWithPrefix = (char *) malloc(strlen(szFormatArgs) + strlen(logPrefix) + 1);
        strcpy(szFormatArgsWithPrefix, logPrefix);
        strcat(szFormatArgsWithPrefix, szFormatArgs);
        PARSEVALIST(szFormatArgsWithPrefix, szLog)
//        PARSEVALIST(szFormatArgs, szLog)
        write(STDOUT_FILENO, szLog, strlen(szLog));
    }
}

bool ZLog::Error(const char *szLog) {
    write(STDOUT_FILENO, "\033[31m", 5);
    write(STDOUT_FILENO, szLog, strlen(szLog));
    write(STDOUT_FILENO, "\033[0m", 4);
    return false;
}

bool ZLog::ErrorV(const char *szFormatArgs, ...) {
    const char *logPrefix = "sign.ipadump.com：";
    char *szFormatArgsWithPrefix = (char *) malloc(strlen(szFormatArgs) + strlen(logPrefix) + 1);
    strcpy(szFormatArgsWithPrefix, logPrefix);
    strcat(szFormatArgsWithPrefix, szFormatArgs);
    PARSEVALIST(szFormatArgsWithPrefix, szLog)
//    PARSEVALIST(szFormatArgs, szLog)
    write(STDOUT_FILENO, "\033[31m", 5);
    write(STDOUT_FILENO, szLog, strlen(szLog));
    write(STDOUT_FILENO, "\033[0m", 4);
    return false;
}

//bool ZLog::Success(const char *szLog) {
//    write(STDOUT_FILENO, "\033[32m", 5);
//    write(STDOUT_FILENO, szLog, strlen(szLog));
//    write(STDOUT_FILENO, "\033[0m", 4);
//    return true;
//}

bool ZLog::Success(const char *szFormatArgs, ...) {
    const char *logPrefix = "sign.ipadump.com：";
    char *szFormatArgsWithPrefix = (char *) malloc(strlen(szFormatArgs) + strlen(logPrefix) + 1);
    strcpy(szFormatArgsWithPrefix, logPrefix);
    strcat(szFormatArgsWithPrefix, szFormatArgs);
    PARSEVALIST(szFormatArgsWithPrefix, szLog)
    write(STDOUT_FILENO, "\033[32m", 5);
    write(STDOUT_FILENO, szLog, strlen(szLog));
    write(STDOUT_FILENO, "\033[0m", 4);
    write(STDOUT_FILENO, "\n", 1);
    return true;
}

bool ZLog::PrintResult(bool bSuccess, const char *szLog) {
    return bSuccess ? Success(szLog) : Error(szLog);
}

bool ZLog::PrintResultV(bool bSuccess, const char *szFormatArgs, ...) {
    PARSEVALIST(szFormatArgs, szLog)
    return bSuccess ? Success(szLog) : Error(szLog);
}

bool ZLog::Warn(const char *szLog) {
    write(STDOUT_FILENO, "\033[33m", 5);
    write(STDOUT_FILENO, szLog, strlen(szLog));
    write(STDOUT_FILENO, "\033[0m", 4);
    return false;
}

bool ZLog::WarnV(const char *szFormatArgs, ...) {
    const char *logPrefix = "sign.ipadump.com：";
    char *szFormatArgsWithPrefix = (char *) malloc(strlen(szFormatArgs) + strlen(logPrefix) + 1);
    strcpy(szFormatArgsWithPrefix, logPrefix);
    strcat(szFormatArgsWithPrefix, szFormatArgs);
    PARSEVALIST(szFormatArgsWithPrefix, szLog)
//    PARSEVALIST(szFormatArgs, szLog)
    write(STDOUT_FILENO, "\033[33m", 5);
    write(STDOUT_FILENO, szLog, strlen(szLog));
    write(STDOUT_FILENO, "\033[0m", 4);
    return false;
}

void ZLog::Print(const char *szLog) {
    if (g_nLogLevel >= E_INFO) {
        write(STDOUT_FILENO, szLog, strlen(szLog));
    }
}

void ZLog::PrintV(const char *szFormatArgs, ...) {
    if (g_nLogLevel >= E_INFO) {
        const char *logPrefix = "sign.ipadump.com: ";
        char *szFormatArgsWithPrefix = (char *) malloc(strlen(szFormatArgs) + strlen(logPrefix) + 1);
        strcpy(szFormatArgsWithPrefix, logPrefix);
        strcat(szFormatArgsWithPrefix, szFormatArgs);
        PARSEVALIST(szFormatArgsWithPrefix, szLog)
//        PARSEVALIST(szFormatArgs, szLog)
        write(STDOUT_FILENO, szLog, strlen(szLog));
//        write(STDOUT_FILENO, "\n", 1);
    }
}

void ZLog::Debug(const char *szLog) {
    if (g_nLogLevel >= E_DEBUG) {
        write(STDOUT_FILENO, szLog, strlen(szLog));
    }
}

void ZLog::DebugV(const char *szFormatArgs, ...) {
    if (g_nLogLevel >= E_DEBUG) {
        PARSEVALIST(szFormatArgs, szLog)
        write(STDOUT_FILENO, szLog, strlen(szLog));
    }
}

bool ZLog::IsDebug() {
    return (E_DEBUG == g_nLogLevel);
}
