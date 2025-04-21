extends Area2D

var speed = 200

func _ready():
	# Connect collision detection signal
	connect("area_entered", Callable(self, "_on_alien_bullet_area_entered"))
	connect("body_entered", Callable(self, "_on_alien_bullet_body_entered"))

func _process(delta):
	position.y += speed * delta
	if position.y > 600:  # Adjust to match your game’s viewport height
		queue_free()

func _on_alien_bullet_area_entered(area):
	if area.is_in_group("player"):
		print("Ship hit!")
		area.damage_ship()  # Let the ship handle its own damage logic
		queue_free()
	if area.is_in_group("ShieldBlock"):
		area._on_hit()
		queue_free()  # Bullet disappears too

func _on_alien_bullet_body_entered(body):
	if body.is_in_group("ShieldBlock"):
		body._on_hit()
		queue_free()
