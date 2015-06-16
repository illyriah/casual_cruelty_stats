CREATE OR REPLACE FUNCTION public.get_match_averages()
RETURNS table  (avg_max_players int, avg_min_players int, avg_players_start int, avg_players_end int, map_name varchar, map_count int)
LANGUAGE plpgsql
AS $function$
BEGIN
		return query 
		select * from 
		(
			select 
				avg(max_players)::integer	as avg_max_players, 
				avg(min_players)::integer	as avg_min_players, 
				avg(players_start)::integer as avg_players_start, 
				avg(players_end)::integer	as avg_players_end, 
				get_match_summaries.map_name  as map_name_avg,
				count(get_match_summaries.map_name)::integer as map_count
			from get_match_summaries() 
			group by get_match_summaries.map_name
		) as p 
		order by p.avg_players_start - p.avg_players_end;
END
$function$
