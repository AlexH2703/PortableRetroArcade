extends Node2D

@export var speed := 300.0

# Path to Bullet scene (make sure to adjust if the bullet scene path is different)
var bullet_scene = preload("res://Bullet.tscn")

# Fire rate settings
var fire_rate = 0.5
var time_since_last_shot = 0

# Screen size variable to dynamically adjust boundaries
var screen_size: Vector2

func _ready():
	update_screen_size()  # Initialize screen size

func _process(delta):
	# Handle spaceship movement (left and right)
	var direction = 0
	if Input.is_action_pressed("ui_left"):
		direction = -1
	elif Input.is_action_pressed("ui_right"):
		direction = 1

	position.x += direction * speed * delta

	# Clamp position to stay within screen bounds
	var half_width = $Sprite2D.texture.get_width() / 2
	position.x = clamp(position.x, half_width, screen_size.x - half_width)

	# Handle shooting (spacebar)
	time_since_last_shot += delta
	if Input.is_action_pressed("ui_shoot") and time_since_last_shot >= fire_rate:
		shoot_bullet()
		time_since_last_shot = 0  # Reset the shooting timer

# Function to shoot bullets
func shoot_bullet():
	# Create a new instance of the Bullet scene
	var bullet = bullet_scene.instantiate()

	# Position the bullet slightly above the player (adjust based on your ship size)
	bullet.position = position + Vector2(0, -40)

	# Add the bullet to the scene
	get_parent().add_child(bullet)

# Update screen size when window resizes
func _notification(what):
	if what == NOTIFICATION_WM_SIZE_CHANGED:
		update_screen_size()

func update_screen_size():
	screen_size = get_viewport_rect().size
