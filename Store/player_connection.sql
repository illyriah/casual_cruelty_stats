DROP TABLE IF EXISTS player_connection;
CREATE TABLE player_connection
(
	player_connection_id SERIAL PRIMARY KEY,
	player_id INTEGER NOT NULL,
	match_id INTEGER NOT NULL,
	connection_start_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, 
	connection_end_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

	CONSTRAINT player_connection_player_player_id FOREIGN KEY (player_id)
		REFERENCES player (player_id) MATCH SIMPLE
		ON UPDATE NO ACTION ON DELETE CASCADE,

	CONSTRAINT player_connection_match_match_id FOREIGN KEY (match_id)
		REFERENCES match (match_id) MATCH SIMPLE
		ON UPDATE NO ACTION ON DELETE CASCADE
);
