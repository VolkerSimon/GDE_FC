extends Node

func _ready() -> void:
	pass

func _on_file_dialog_file_selected(path):
	var s = FileInfo.new()
	s.set_file(path)
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
	
	print ()
	print ("Dimensions:")
	print("Height:",s.get_image_height())
	print("Width:",s.get_image_width())
	print("size:",s.get_file_size())





func _on_button_pressed():
	$"FileDialog".visible=true;
