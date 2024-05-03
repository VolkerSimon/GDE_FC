extends Node

func _ready() -> void:
	var s = FileInfo.new()
	s.set_file(("c:\\vfcompat.dll"))
	var t=s.get_file_creation_time()
	print("Godot date format:",s.get_file_creation_time())
	
	print("Day:", t["day"]," Month:",t["month"]," Year:",t["year"])
	print ()
	print ("Creation Time:")
	print("ISO Date only:",s.get_fileCreationDate_string())
	print("ISO Date Time:",s.get_fileCreationDateTime_string())
	print ()
	print ("Modify Time:")
	print("ISO Date only:",s.get_fileModifiedDate_string())
	print("ISO Date Time:",s.get_fileModifiedDateTime_string())
	
	print ()
	print ("Last Access Time:")
	print("ISO Date only:",s.get_fileLastAccessDate_string())
	print("ISO Date Time:",s.get_fileLastAccessDateTime_string())
	

	
	

