drop type if exists get_match_summaries_result cascade;

create type get_match_summaries_result AS (max_players int, min_players int, players_start int, players_end int, map_name varchar, match_start timestamp, match_end timestamp, total_connections int, match_length interval);

CREATE OR REPLACE FUNCTION public.get_match_summaries()
RETURNS SETOF get_match_summaries_result
LANGUAGE plpgsql
AS $function$
DECLARE
	result get_match_summaries_result%rowtype;
	match_row match%rowtype;
	match_player player_connection%rowtype;
	max int;
	min int;
	potential_max int;
	potential_min int;
	interval_start timestamp;
	interval_end timestamp;
BEGIN
	for match_row in select * from match 
	loop 
		max := 0;
		min := 100;
		for match_player in 
			select * from player_connection 
			where connection_start_time < match_row.match_end_time 
			and connection_end_time > match_row.match_start_time 
		loop
			interval_start = greatest(match_player.connection_start_time, match_row.match_start_time);
			interval_end = least(match_player.connection_end_time, match_row.match_end_time);

			potential_max := (select count(*) from player_connection WHERE (player_connection.connection_start_time, player_connection.connection_end_time) overlaps (interval_start, interval_end)); 

			potential_min := (select count(*) from player_connection WHERE (player_connection.connection_start_time,player_connection.connection_end_time) overlaps (interval_start, interval_end)); 

			max := greatest(potential_max, max);
			min := least(potential_min, min);


		end loop;

		if min = 100 then
			min := 0;
		end if;          

		result.max_players := max;
		result.min_players := min;

		result.players_start :=  (select count(*) from player_connection WHERE tsrange(player_connection.connection_start_time, player_connection.connection_end_time) @> match_row.match_start_time); 

		result.players_end :=  (select count(*) from player_connection WHERE tsrange(player_connection.connection_start_time, player_connection.connection_end_time) @> match_row.match_end_time); 

		result.map_name := (select map_name from map where map_id = match_row.map_id);

		result.match_start := date_trunc('minute', match_row.match_start_time);
		result.match_end := date_trunc('minute', match_row.match_end_time);

		result.total_connections := (select count(*) from player_connection WHERE tsrange(match_row.match_start_time, match_row.match_end_time) @> player_connection.connection_start_time);


		result.match_length := (match_row.match_end_time - match_row.match_start_time);

		return next result;

	end loop;
END
$function$

