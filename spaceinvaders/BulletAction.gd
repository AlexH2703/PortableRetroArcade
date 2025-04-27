#extends Node2D
#
## Speed of the bullet
#var speed = 600
## The explosion effect scene
#@export var explosion_scene: PackedScene
#
#@onready var collision_shape = $BulletCollision  # Reference to the collision shape
#
#func _ready():
	## Start the process to move the bullet
	#set_process(true)
#
#func _process(delta):
	## Move the bullet upwards
	#position.y -= speed * delta
#
	## If the bullet goes off-screen (top of the screen), trigger the explosion
	#if position.y < -10:
		#explode()
#
#func _on_Bullet_body_entered(body):
	## This will trigger when the bullet collides with something
	#if body.is_in_group("back_edge"):  # Assuming "back_edge" is a group for the border
		#explode()
	#elif body.is_in_group("enemy"):  # If it hits an enemy
		#queue_free()  # Remove the bullet (you can add logic to handle enemy death here)
#
## Function to trigger an explosion
#func explode():
	#if explosion_scene:
		#var explosion = explosion_scene.instantiate()  # Create the explosion effect
		#explosion.global_position = global_position  # Position it where the bullet is
		#get_parent().add_child(explosion)  # Add the explosion to the scene
	#queue_free()  # Remove the bullet
