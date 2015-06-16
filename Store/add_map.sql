CREATE OR REPLACE FUNCTION add_map(in_map_name VARCHAR(15)) RETURNS void AS $$
DECLARE
	new_map_id integer;
	old_map_id integer;

BEGIN
	SELECT match.map_id INTO old_map_id FROM match ORDER BY match_id DESC LIMIT 1;
	SELECT map.map_id INTO new_map_id FROM map WHERE map.map_name = in_map_name;

	IF old_map_id <> new_map_id OR new_map_id IS NULL THEN
		IF new_map_id IS NULL THEN
			INSERT INTO map (map_name) VALUES(in_map_name); 
			SELECT map.map_id INTO new_map_id FROM map WHERE map.map_name = in_map_name;
		END IF;
	END IF;
	
	UPDATE match SET match_end_time = CURRENT_TIMESTAMP WHERE match_id = 
	(
		SELECT match_id FROM match ORDER BY match_id DESC LIMIT 1
	);
	
	INSERT INTO match (map_id) VALUES (new_map_id);
END
$$ LANGUAGE plpgsql
