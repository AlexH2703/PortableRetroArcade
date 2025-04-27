extends StaticBody2D

func _ready():
	add_to_group("ShieldBlock")
	
func _on_hit():
	queue_free()  # Destroy this block when hit
