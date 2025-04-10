extends Area2D

@export var speed: float = 20.0

#func _process(delta):
	# This can be used for slow falling behavior
	#position.y += speed * delta

func _on_area_entered(area):
	if area.name == "Bullet":
		queue_free()  # Remove alien if hit by bullet
