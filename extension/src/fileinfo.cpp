/*
#include "fileinfo.h"
#include <windows.h>
#include <string>
#include <fileapi.h>
#include <godot_cpp/core/class_db.hpp>
#include <iostream>
#include <cstdio>
#include <godot_cpp/variant/utility_functions.hpp>
#include <filesystem>
*/
#include "fileinfo.h"
#ifdef _WIN32
    #include <windows.h>
    #include <fileapi.h>
#else
    #include <sys/stat.h>
    #include <unistd.h>
#endif
#include <string>
#include <godot_cpp/core/class_db.hpp>
#include <iostream>
#include <cstdio>
#include <godot_cpp/variant/utility_functions.hpp>
#include <filesystem>


using namespace godot;

FileInfo::FileInfo(){}
FileInfo::~FileInfo(){}


int FileInfo::set_file(godot::String pathStr) {
    FileName = pathStr;
    std::string std_string_filename = pathStr.utf8().get_data();

#ifdef _WIN32
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, std_string_filename.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_UTF8, 0, std_string_filename.c_str(), -1, wstr, wchars_num);
    LPCWSTR lpcwstr_filename = wstr;

    WIN32_FILE_ATTRIBUTE_DATA fInfo;
    GetFileAttributesExW(lpcwstr_filename, GetFileExInfoStandard, &fInfo);
    delete[] wstr;

    FILETIME creationTime = fInfo.ftCreationTime;
    FileTimeToSystemTime(&creationTime, &creation_systemtime);
    creation_time["year"] = creation_systemtime.wYear;
    creation_time["month"] = creation_systemtime.wMonth;
    creation_time["day"] = creation_systemtime.wDay;
    creation_time["hour"] = creation_systemtime.wHour;
    creation_time["minute"] = creation_systemtime.wMinute;
    creation_time["second"] = creation_systemtime.wSecond;

    FILETIME modifiedTime = fInfo.ftLastWriteTime;
    FileTimeToSystemTime(&modifiedTime, &modified_systemtime);
    modified_time["year"] = modified_systemtime.wYear;
    modified_time["month"] = modified_systemtime.wMonth;
    modified_time["day"] = modified_systemtime.wDay;
    modified_time["hour"] = modified_systemtime.wHour;
    modified_time["minute"] = modified_systemtime.wMinute;
    modified_time["second"] = modified_systemtime.wSecond;

    FILETIME lastAccessTime = fInfo.ftLastAccessTime;
    FileTimeToSystemTime(&lastAccessTime, &lastaccess_systemtime);
    lastaccess_time["year"] = lastaccess_systemtime.wYear;
    lastaccess_time["month"] = lastaccess_systemtime.wMonth;
    lastaccess_time["day"] = lastaccess_systemtime.wDay;
    lastaccess_time["hour"] = lastaccess_systemtime.wHour;
    lastaccess_time["minute"] = lastaccess_systemtime.wMinute;
    lastaccess_time["second"] = lastaccess_systemtime.wSecond;

#else
    struct stat fileStat;
    if (stat(std_string_filename.c_str(), &fileStat) != 0) {
        return -1; // Error handling
    }
    
    struct tm creation_tm;
    localtime_r(&fileStat.st_ctime, &creation_tm);
    creation_time["year"] = creation_tm.tm_year + 1900;
    creation_time["month"] = creation_tm.tm_mon + 1;
    creation_time["day"] = creation_tm.tm_mday;
    creation_time["hour"] = creation_tm.tm_hour;
    creation_time["minute"] = creation_tm.tm_min;
    creation_time["second"] = creation_tm.tm_sec;

    struct tm modified_tm;
    localtime_r(&fileStat.st_mtime, &modified_tm);
    modified_time["year"] = modified_tm.tm_year + 1900;
    modified_time["month"] = modified_tm.tm_mon + 1;
    modified_time["day"] = modified_tm.tm_mday;
    modified_time["hour"] = modified_tm.tm_hour;
    modified_time["minute"] = modified_tm.tm_min;
    modified_time["second"] = modified_tm.tm_sec;

    struct tm lastaccess_tm;
    localtime_r(&fileStat.st_atime, &lastaccess_tm);
    lastaccess_time["year"] = lastaccess_tm.tm_year + 1900;
    lastaccess_time["month"] = lastaccess_tm.tm_mon + 1;
    lastaccess_time["day"] = lastaccess_tm.tm_mday;
    lastaccess_time["hour"] = lastaccess_tm.tm_hour;
    lastaccess_time["minute"] = lastaccess_tm.tm_min;
    lastaccess_time["second"] = lastaccess_tm.tm_sec;
#endif

    uint32_t height = 0;
    uint32_t width = 0;
    String fileType = "";

    bool result = FileInfo::fileTypeDetails(std_string_filename.c_str(), &width, &height, &fileType);
    imageHeight = height;
    imageWidth = width;
    imageFileType = fileType;

    return 1;
}


bool FileInfo::fileTypeDetails(const char *fn, uint32_t *x, uint32_t *y, String *fileType) {
    FILE *file;
#ifdef _WIN32
    wchar_t wfn[256];
    MultiByteToWideChar(CP_UTF8, 0, fn, -1, wfn, 256);
    file = _wfopen(wfn, L"rb");
#else
    file = fopen(fn, "rb");
#endif

    if (file == nullptr) return false;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t buf[128];
    fread(buf, 1, 120, file);

    // JPEG
    if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF) {
        uint8_t marker[4];
        fseek(file, 0, SEEK_SET);
        fread(marker, 1, 2, file); // Start Marker
        while (fread(marker, 1, 2, file) == 2) {
            if (marker[0] == 0xFF && marker[1] != 0x00 && marker[1] != 0xFF) {
                if (marker[1] == 0xC0 || marker[1] == 0xC1 || marker[1] == 0xC2 || marker[1] == 0xC3) {
                    fseek(file, 3, SEEK_CUR); // Skip segment length and precision
                    fread(marker, 1, 4, file); // Read height and width
                    *y = (marker[0] << 8) | marker[1];
                    *x = (marker[2] << 8) | marker[3];
                    fclose(file);
                    *fileType = (marker[1] == 0xC0) ? "JIFF" : "JPG";
                    return true;
                } else if (marker[1] == 0xD8) {
                    fseek(file, 1, SEEK_CUR);
                } else {
                    uint16_t segment_length;
                    fread(&segment_length, 1, 2, file);
                    segment_length = (segment_length << 8) | (segment_length >> 8); // Correct endianness
                    fseek(file, segment_length - 2, SEEK_CUR);
                }
            } else {
                fseek(file, 1, SEEK_CUR);
            }
        }
        return true;
    }

    // GIF
    if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F') {
        *x = buf[6] + (buf[7] << 8);
        *y = buf[8] + (buf[9] << 8);
        *fileType = "GIF";
        return true;
    }

    // PNG
    if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R') {
        *x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + buf[19];
        *y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + buf[23];
        *fileType = "PNG";
        return true;
    }

    // AVI
    if (buf[0] == 'R' && buf[1] == 'I' && buf[2] == 'F' && buf[3] == 'F') {
        *x = buf[64] + (buf[65] << 8) + (buf[66] << 16) + (buf[67] << 24);
        *y = buf[68] + (buf[69] << 8) + (buf[70] << 16) + (buf[71] << 24);
        *fileType = "AVI";
        return true;
    }

    // TIFF
    if (buf[0] == 0x49 && buf[1] == 0x49 && buf[2] == 0x2A) {
        uint32_t ifdOffset = (buf[4] << 0) + (buf[5] << 8) + (buf[6] << 16) + (buf[7] << 24);
        fseek(file, ifdOffset, SEEK_SET);

        uint16_t numEntries;
        fread(&numEntries, sizeof(numEntries), 1, file);

        for (int i = 0; i < numEntries; ++i) {
            uint16_t tag;
            fread(&tag, sizeof(tag), 1, file);

            if (tag == 0x100) { // Width
                uint32_t width;
                fseek(file, 6, SEEK_CUR);
                fread(&width, sizeof(width), 1, file);
                *x = width;
            } else if (tag == 0x101) { // Height
                uint32_t height;
                fseek(file, 6, SEEK_CUR);
                fread(&height, sizeof(height), 1, file);
                *y = height;
            } else {
                fseek(file, 10, SEEK_CUR);
            }
        }
        *fileType = "TIFF";
        return true;
    }

    fclose(file);
    return false;
}




godot::Dictionary  FileInfo::get_file_creation_time() 
{
    return creation_time;
}

godot::Dictionary  FileInfo::get_file_modified_time() 
{
    return modified_time;
}
godot::Dictionary  FileInfo::get_file_lastaccess_time() 
{
    return lastaccess_time;
}

String FileInfo::get_fileCreationDate_string() 
{
    return CreateISODateString(creation_systemtime);
}

String FileInfo::get_fileCreationDateTime_string() 
{
    return CreateISODateTimeString(creation_systemtime);
}


String FileInfo::get_fileModifiedDateTime_string() 
{
   
    return CreateISODateTimeString(modified_systemtime);
}

String FileInfo::get_fileModifiedDate_string() 
{
    return CreateISODateString(modified_systemtime);
}

String FileInfo::get_fileLastAccessDateTime_string() 
{
   
    return CreateISODateTimeString(lastaccess_systemtime);
}

String FileInfo::get_fileLastAccessDate_string() 
{
    return CreateISODateString(lastaccess_systemtime);
}

String FileInfo::CreateISODateTimeString(SYSTEMTIME inTime)
{
    char buf[26];
    std::sprintf(buf,"%04d-%02d-%02dT%02d:%02d:%02d+00:00",inTime.wYear,inTime.wMonth,inTime.wDay,inTime.wHour,inTime.wMinute,inTime.wSecond);

    //std::string s = std::to_string(creation_systemtime.wYear)+"-"+ std::to_string(creation_systemtime.wMonth)+"-"+std::to_string(creation_systemtime.wDay)+"T"+std::to_string(creation_systemtime.wHour)+":"+std::to_string(creation_systemtime.wMinute)+":"+std::to_string(creation_systemtime.wSecond)+"+00:00";
    return buf;
}

String FileInfo::CreateISODateString(SYSTEMTIME inTime)
{
   
    char buf[11];
    std::sprintf(buf,"%04d-%02d-%02d",inTime.wYear,inTime.wMonth,inTime.wDay);
    //std::string s = std::to_string(creation_systemtime.wYear)+"-"+ std::to_string(creation_systemtime.wMonth)+"-"+std::to_string(creation_systemtime.wDay)+"T"+std::to_string(creation_systemtime.wHour)+":"+std::to_string(creation_systemtime.wMinute)+":"+std::to_string(creation_systemtime.wSecond)+"+00:00";
    return buf;
}


int FileInfo::get_image_height() 
{
    return FileInfo::imageHeight;
}

int FileInfo::get_image_width() 
{
    return FileInfo::imageWidth;
}

int FileInfo::get_file_size() 
{
    return FileInfo::fileSize;
}


String FileInfo::get_imageFileType() 
{
   
    return FileInfo::imageFileType;
}


void FileInfo::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_file"), &FileInfo::set_file);
    ClassDB::bind_method(D_METHOD("get_imageFileType"), &FileInfo::get_imageFileType);

    ClassDB::bind_method(D_METHOD("get_fileCreationDate_string"), &FileInfo::get_fileCreationDate_string);
    ClassDB::bind_method(D_METHOD("get_fileCreationDateTime_string"), &FileInfo::get_fileCreationDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDate_string"), &FileInfo::get_fileModifiedDate_string);
    ClassDB::bind_method(D_METHOD("get_fileModifiedDateTime_string"), &FileInfo::get_fileModifiedDateTime_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDate_string"), &FileInfo::get_fileLastAccessDate_string);
    ClassDB::bind_method(D_METHOD("get_fileLastAccessDateTime_string"), &FileInfo::get_fileLastAccessDateTime_string);
    ClassDB::bind_method(D_METHOD("get_file_creation_time"), &FileInfo::get_file_creation_time);
    ClassDB::bind_method(D_METHOD("get_file_modified_time"), &FileInfo::get_file_modified_time);
    ClassDB::bind_method(D_METHOD("get_file_lastaccess_time"), &FileInfo::get_file_lastaccess_time);
    ClassDB::bind_method(D_METHOD("get_file_size"), &FileInfo::get_file_size);
    ClassDB::bind_method(D_METHOD("get_image_height"), &FileInfo::get_image_height);
    ClassDB::bind_method(D_METHOD("get_image_width"), &FileInfo::get_image_width);
    
}