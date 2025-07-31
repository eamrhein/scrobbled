#include <mpd/albumart.h>
#include <mpd/client.h>
#include <mpd/connection.h>
#include <mpd/entity.h>
#include <mpd/idle.h>
#include <mpd/song.h>
#include <mpd/tag.h>
#include <stdio.h>
#include <unistd.h>

void log_song(struct mpd_connection *conn) {
	struct mpd_song *song = mpd_run_current_song(conn);
	if(!song){
		printf("No song playering .\n");
		return;
	}

	const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	const char *title  = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	if (artist && title) {
		printf("Now Playing: %s - %s\n", artist, title);
	} else if (title) {
		printf("Now Playing: %s\n", title);
	} else {
		printf("Now Playing: [Unknown Track]\n");
	}
	mpd_song_free(song);

}

int main(){
	struct mpd_connection *conn;
	conn = mpd_connection_new(NULL, 0, 30000);
	if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
		fprintf(stderr, "MPD conneciton failed: %s\n", mpd_connection_get_error_message(conn));
		mpd_connection_free(conn);
		return 1;
	}
	printf("Listening for MPD playback changes ...\n ");
	while(1){
		if(!mpd_send_idle(conn)){
			fprintf(stderr, "Fai;led to send idle command. \n");
			break;
		}
		enum mpd_idle event = mpd_recv_idle(conn, true);
		if(event & MPD_IDLE_PLAYER){

			log_song(conn);
		}
	}
	mpd_connection_free(conn);
	return 0;

}

