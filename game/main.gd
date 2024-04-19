extends Node

func _ready() -> void:
	print("Hallo")
	var t
	var s = FileInfo.new()
	s.set_file(("c:\\vfcompat.dll"))
	t=s.get_file_creation_time()
	print(t["day"])
	print(s.get_file_creation_time()["month"])
	print(s.get_file_creation_time())
	print(s.get_file_modified_time())
	print(s.get_file_lastaccess_time())

	
	

