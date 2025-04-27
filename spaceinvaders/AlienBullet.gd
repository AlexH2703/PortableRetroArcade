extends Area2D

var speed = 200
var screen_height = 0

func _ready():
	# Connect collision detection signal
	connect("area_entered", Callable(self, "_on_alien_bullet_area_entered"))
	connect("body_entered", Callable(self, "_on_alien_bullet_body_entered"))
	
	# Get the actual screen height
	screen_height = get_viewport_rect().size.y

func _process(delta):
	position.y += speed * delta
	if position.y > screen_height:
		queue_free()

func _on_alien_bullet_area_entered(area):
	if area.is_in_group("player"):
		print("Ship hit!")
		area.damage_ship()
		queue_free()
	elif area.is_in_group("ShieldBlock"):
		area._on_hit()
		queue_free()

func _on_alien_bullet_body_entered(body):
	if body.is_in_group("ShieldBlock"):
		body._on_hit()
		queue_free()
