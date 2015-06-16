CREATE OR REPLACE FUNCTION player_disconnect(in_player_name VARCHAR(15)) 
RETURNS void AS $$
DECLARE
	new_player_id integer;
	current_match_id integer;

BEGIN
	SELECT player.player_id INTO new_player_id FROM player WHERE player.player_name = in_player_name;

	UPDATE player_connection SET connection_end_time = CURRENT_TIMESTAMP WHERE player_connection_id =
	(
		SELECT player_connection_id 
		FROM player_connection 
		WHERE player_id = new_player_id 
		ORDER BY player_connection_id DESC LIMIT 1
	);

END
$$ LANGUAGE plpgsql
