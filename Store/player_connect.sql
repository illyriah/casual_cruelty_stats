CREATE OR REPLACE FUNCTION player_connect(in_player_name VARCHAR(15), in_player_regular BOOLEAN, in_player_RSM BOOLEAN) 
RETURNS void AS $$
DECLARE
	new_player_id integer;
	current_match_id integer;

BEGIN
	SELECT player.player_id INTO new_player_id FROM player WHERE player.player_name = in_player_name;

	IF new_player_id IS NULL THEN
		INSERT INTO player (player_name, is_regular, is_rsm) VALUES(in_player_name, in_player_regular, in_player_RSM); 
		SELECT player.player_id INTO new_player_id FROM player WHERE player.player_name = in_player_name;
	END IF;
	
	SELECT match_id INTO current_match_id FROM match ORDER BY match_id DESC LIMIT 1;
	
	INSERT INTO player_connection (player_id, match_id) VALUES (new_player_id, current_match_id);
END
$$ LANGUAGE plpgsql
