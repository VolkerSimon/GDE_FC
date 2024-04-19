#ifndef FileInfo_CLASS_H
#define FileInfo_CLASS_H

#include <windows.h>

#include <godot_cpp/classes/ref.hpp>

using namespace godot;

class FileInfo : public RefCounted
{
    SYSTEMTIME creation_systemtime;
    SYSTEMTIME modified_systemtime;
    SYSTEMTIME lastaccess_systemtime;
    GDCLASS(FileInfo, RefCounted);
    godot::Dictionary creation_time;
    godot::Dictionary modified_time;
    godot::Dictionary lastaccess_time;

protected:
    static void _bind_methods();
    
public:
    FileInfo();
    ~FileInfo();
    int set_file(godot::String pathStr);
    godot::String get_fileCreationDate_string();
    godot::String get_fileCreationDateTime_string();
    godot::Dictionary get_file_creation_time();
    godot::Dictionary get_file_modified_time();
    godot::Dictionary get_file_lastaccess_time();

};


#endif // FileInfo_CLASS_H