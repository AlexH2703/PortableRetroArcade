#extends Node2D
#
#@export var speed := 300.0
#
#var bullet_scene = preload("res://Bullet.tscn")
#var fire_rate = 0.5
#var time_since_last_shot = 0
#var screen_size: Vector2
#
#@onready var spaceship = $SpaceShip  # Adjust this to match your Sprite2D node's actual name!
#
#func _ready():
	#update_screen_size()
#
#func _process(delta):
	## Handle movement
	#var direction = 0
	#if Input.is_action_pressed("ui_left"):
		#direction = -1
	#elif Input.is_action_pressed("ui_right"):
		#direction = 1
#
	#position.x += direction * speed * delta
#
	## Clamp to screen bounds using sprite size
	#if spaceship.texture:
		#var half_width = spaceship.texture.get_size().x / 2
		#position.x = clamp(position.x, half_width, screen_size.x - half_width)
#
	## Shooting logic
	#time_since_last_shot += delta
	#if Input.is_action_pressed("ui_shoot") and time_since_last_shot >= fire_rate:
		#shoot_bullet()
		#time_since_last_shot = 0
#
#func shoot_bullet():
	#var bullet = bullet_scene.instantiate()
	#
	## Position bullet to the center top of the ship
	#if spaceship.texture:
		#var half_width = spaceship.texture.get_size().x / 2
		#bullet.position = Vector2(position.x, position.y - spaceship.texture.get_size().y / 2)
	#
	#get_parent().add_child(bullet)
#
#func _notification(what):
	#if what == NOTIFICATION_WM_SIZE_CHANGED:
		#update_screen_size()
#
#func update_screen_size():
	#screen_size = get_viewport_rect().size
 #
