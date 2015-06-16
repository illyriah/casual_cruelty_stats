DROP TABLE IF EXISTS player;
CREATE TABLE player
(
	player_id serial PRIMARY KEY,
	player_name VARCHAR(20) UNIQUE NOT NULL,
	is_rsm BOOLEAN NOT NULL, 
	is_regular BOOLEAN NOT NULL
);
