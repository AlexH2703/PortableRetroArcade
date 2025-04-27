extends HTTPRequest  # Script is now directly attached to the HTTPRequest node

# Declare member variables
@onready var scores_label = $LeaderboardLabel  # Your label node for scores
@onready var overlay = $GameOverOverlay  # Your overlay node

# URL of your API endpoint
var api_url = "http://localhost:7291/api/highscores/top5"

func _ready():
	self.connect("request_completed", Callable(self, "_on_HTTPRequest_request_completed"))

# Function to refresh the top 5 scores
func refresh_high_scores():
	# Send GET request to fetch the top 5 scores
	print("Sending request to fetch top 5 scores...")
	var error = self.request(api_url)  # Use 'self' to call 'request' on this node
	if error != OK:
		print("Error requesting data from API.")
	else:
		print("Request sent successfully.")

# This function will be called when the request completes
func _on_HTTPRequest_request_completed(result, response_code, headers, body):
	print("Request completed. Response code: ", response_code)  # Debug the response code

	if response_code == 200:
		# Print the raw body to see the response data
		print("Response Body: ", body.get_string_from_utf8())  # Debug the raw response

		# Create an instance of the JSON class to parse the response
		var json = JSON.new()
		var parse_result = json.parse(body.get_string_from_utf8())  # Use the instance to parse
		print("JSON parse result: ", parse_result)  # Check the result of parsing

		if parse_result == OK:
			var top_scores = json.get_data()  # Use the instance to get the parsed data
			print("Top Scores: ", top_scores)  # Check if the top_scores list is populated

			# Check if the structure is correct (it should be an array of dictionaries)
			if typeof(top_scores) == TYPE_ARRAY:
				print("Data is an array, proceeding with processing.")
			else:
				print("Data is not an array. Check the response format.")
			
			var score_text = "Top 5 High Scores:\n"

			# Loop through the scores and format them into the label text
			for score in top_scores:
				print("Score: ", score)  # Debug each score
				score_text += "%s - %d\n" % [score["Initial"], score["Score"]]

			# Update the label with the top 5 scores
			print("Updating label with score text: ", score_text)  # Debug label text
			scores_label.text = score_text
		else:
			print("Error parsing JSON.")
	else:
		print("Failed to fetch data. Response code: %d" % response_code)

# Function to show overlay and refresh scores
func show_game_over_overlay():
	print("Attempting to show game over overlay...")  # Debug overlay visibility
	if overlay != null:
		# Show the overlay
		print("Overlay found. Showing overlay...")
		overlay.visible = true

		# Refresh the high scores
		print("Refreshing high scores...")
		refresh_high_scores()

		# Optionally, you can pause the game or do any other logic here
		get_tree().paused = true  # This will pause the game
	else:
		print("GameOverOverlay is not found.")

# Example of calling this function when the game ends
func _on_game_end():
	print("Game ended. Showing game over overlay...")  # Debug game over event
	show_game_over_overlay()
