extends StaticBody2D

func _on_hit():
	queue_free()  # Destroy this block when hit
