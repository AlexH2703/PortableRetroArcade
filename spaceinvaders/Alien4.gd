extends Area2D

@export var speed = 150
@export var score_range = [50, 100, 150, 300]

signal ufo_destroyed(points)

func _ready():
	connect("area_entered", Callable(self, "_on_area_entered"))

func _process(delta):
	position.x += speed * delta

	# If UFO leaves the screen, delete it
	var screen_limit = get_viewport_rect().size.x
	if (speed > 0 and position.x > screen_limit + 50) or (speed < 0 and position.x < -50):
		queue_free()

func _on_area_entered(area):
	if area.is_in_group("player_bullet"):
		var points = score_range.pick_random()
		emit_signal("ufo_destroyed", points)
		queue_free()
		
func _on_Hit():
	var points = score_range.pick_random()
	get_node("/root/Main").add_score(points)  # Assuming the GameManager is the parent, or adjust as needed
	#emit_signal("ufo_destroyed", points)
	queue_free()
