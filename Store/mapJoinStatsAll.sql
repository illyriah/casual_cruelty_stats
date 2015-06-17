-- By KittyPants/Amie Davis 6/16/15 
-- Returns join stats by map for all players.
SELECT map.map_name, joinTable.joincount, departTable.departcount, joinTable.joincount - departTable.departcount as net
FROM map
INNER JOIN (SELECT map.map_name, count(player_connection.player_connection_id) as departcount FROM match 
	INNER JOIN map ON match.map_id = map.map_id
	INNER JOIN player_connection ON player_connection.match_id = match.match_id
	WHERE connection_end_time > match_start_time AND connection_end_time <= match_end_time
	GROUP BY map.map_name) departTable ON departTable.map_name = map.map_name
INNER JOIN (SELECT map.map_name, count(player_connection.player_connection_id) as joincount FROM match 
	INNER JOIN map ON match.map_id = map.map_id
	INNER JOIN player_connection ON player_connection.match_id = match.match_id
	WHERE connection_start_time >= match_start_time AND connection_start_time < match_end_time
	GROUP BY map.map_name) joinTable ON joinTable.map_name = map.map_name
ORDER BY net DESC, joincount DESC;
