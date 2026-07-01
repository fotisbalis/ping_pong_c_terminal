#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ping_pong.h"

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 20
#define PADDLE_HEIGHT 4
#define LEFT_PADDLE_X 2
#define RIGHT_PADDLE_X (BOARD_WIDTH - 4)
#define WALL_X (BOARD_WIDTH - 1)
#define FRAME_DELAY_MS 16
#define START_BALL_DELAY_FRAMES 6
#define MIN_BALL_DELAY_FRAMES 2
#define SPEED_STEP_FRAMES 1

Paddle left_paddle, right_paddle;
Ball ball;
int score, max_score, max_all_time_score;
int multi_score[2];
int delay, frame_counter = 0;;
GameType game_type;

void make_move(Direction direction, Paddle *paddle) {
	assert(paddle->point >= 0 && paddle->point <= BOARD_HEIGHT - PADDLE_HEIGHT);
	
	switch (direction) {
		case UP:
			if (paddle->point > 0) {
				paddle->point += UP;
			}
			break;
		case DOWN:
			if (paddle->point < BOARD_HEIGHT - PADDLE_HEIGHT) {
				paddle->point += DOWN;
			}
			break;
		default:
			assert(0);
			break;
	}
}

void reset_ball() {

	switch(game_type) {
		case single_player:
			ball.x = BOARD_WIDTH - 1;
			ball.vx = LEFT;

			srand(time(NULL));
			ball.y = rand() % BOARD_HEIGHT;
			ball.vy = rand() % 2 == 0 ? UP : DOWN;
			break;

		case multi_player:
			srand(time(NULL));
			if(rand() % 2 == 0) {
				ball.x = BOARD_WIDTH - 4;
                        	ball.vx = LEFT;
				ball.y = right_paddle.point;
			}
			else {
				ball.x = 4;
                                ball.vx = RIGHT;
				ball.y = left_paddle.point;
			}

                        ball.vy = rand() % 2 == 0 ? UP : DOWN;
			break;

		default:
			assert(0);
			break;
	}
}

void reset_score() {
	
	switch(game_type) {
		case single_player:
			if (score >= max_score)
				max_score = score;
			if (score >= max_all_time_score)
				max_all_time_score = score;
			score = 0;
			break;

		case multi_player:
			break;

		default:
			assert(0);
			break;
	}
}

void update_delay_frames() {
	
	delay = START_BALL_DELAY_FRAMES;
	if(game_type == single_player && score % 3 == 0)
		delay -= (score / 3 * SPEED_STEP_FRAMES);
	
	if (delay < MIN_BALL_DELAY_FRAMES) {
		delay = MIN_BALL_DELAY_FRAMES;
	}
}

void draw_border() {
	int x;

	for (x = 0; x < BOARD_WIDTH; x++) {
		mvaddch(0, x, '-');
		mvaddch(BOARD_HEIGHT + 1, x, '-');
	}
}

void print_score_controls() {
	char score_buffer[32];
        char max_score_buffer[32];
        char max_all_time_score_buffer[64];
        char score1_buffer[64];
	char score2_buffer[64];

	switch(game_type) {
                case single_player:
                        sprintf(score_buffer, "Score: %d", score);
        		sprintf(max_score_buffer, "Highest Score: %d", max_score);
        		sprintf(max_all_time_score_buffer, "Highest All Time Score: %d", max_all_time_score);

        		mvprintw(BOARD_HEIGHT + 3, 0, "%s", score_buffer);
        		mvprintw(BOARD_HEIGHT + 4, 0, "%s", max_score_buffer);
        		mvprintw(BOARD_HEIGHT + 5, 0, "%s", max_all_time_score_buffer);
                        break;

                case multi_player:
			sprintf(score1_buffer, "Player 1: %d", multi_score[0]);
                        sprintf(score2_buffer, "Player 2: %d", multi_score[1]);

              		mvprintw(BOARD_HEIGHT + 3, 0, "Score");
			mvprintw(BOARD_HEIGHT + 4, 0, "%s", score1_buffer);
                        mvprintw(BOARD_HEIGHT + 5, 0, "%s", score2_buffer);
			break;

                default:
                        assert(0);
        }

	mvprintw(BOARD_HEIGHT + 7, 0, "Controls");
       	mvprintw(BOARD_HEIGHT + 8, 0, "Move up: W / Up Arrow");        
	mvprintw(BOARD_HEIGHT + 9, 0, "Move down: S / Down Arrow");                
	mvprintw(BOARD_HEIGHT + 10, 0, "Quit: Q");
}

void render_board() {
	int i;
	
	clear();
	draw_border();

	for (i = 1; i <= PADDLE_HEIGHT; i++) {
		mvaddch(left_paddle.point + i, 1 + LEFT_PADDLE_X, '|');
	}

	mvaddch(1 + ball.y, 1 + ball.x, 'O');

	switch(game_type) {
		case single_player:
			for (i = 1; i <= BOARD_HEIGHT; i++) {
				mvaddch(i, WALL_X, '|');
			}
			break;

		case multi_player:
			for (i = 1; i <= PADDLE_HEIGHT; i++) {
         	       		mvaddch(right_paddle.point + i, 1 + RIGHT_PADDLE_X, '|');
        		}
			break;

		default:
			assert(0);
	}

	print_score_controls();
	refresh();
}

void move_ball() {
        ball.x += ball.vx;
        ball.y += ball.vy;

        if (ball.y <= 0) {
                ball.y = 0;
                ball.vy = RIGHT;
        }
        if (ball.y >= BOARD_HEIGHT - 1) {
                ball.y = BOARD_HEIGHT - 1;
                ball.vy = LEFT;
        }

        if (ball.x == LEFT_PADDLE_X + 1 && ball.y >= left_paddle.point && ball.y < left_paddle.point + PADDLE_HEIGHT) {
                ball.x = LEFT_PADDLE_X + 1;
                ball.vx = RIGHT;
                score++;
        }

	if(game_type == multi_player && ball.x == RIGHT_PADDLE_X - 1 && ball.y >= right_paddle.point && ball.y < right_paddle.point + PADDLE_HEIGHT) {
                ball.x = RIGHT_PADDLE_X - 1;
                ball.vx = LEFT;
        }

        if (ball.x >= WALL_X - 1) {
                switch(game_type) {
			case single_player:
				ball.x = WALL_X - 1;
                		ball.vx = LEFT;
				break;

			case multi_player:
				multi_score[0]++;
                		reset_ball();
				break;

			default:
				assert(0);
				break;
		}
        }

        if (ball.x < 0) {
                reset_score();
		multi_score[1]++;
                reset_ball();
        }
}

void single_player_game() {
	int input;
	int running = 1;

	reset_game_board();
	
	while (running) {
        	input = getch();
		switch (input) {
			case 'w':
			case 'W':
			case KEY_UP:
				make_move(UP, &left_paddle);
				break;
			case 's':
			case 'S':
			case KEY_DOWN:
				make_move(DOWN, &left_paddle);
				break;
			case 'q':
			case 'Q':
				running = 0;
				break;
			default:
				break;
		}

                update_delay_frames();
                if (frame_counter >= delay) {
                       	move_ball();
                        frame_counter = 0;        
		}

              	render_board();
              	napms(FRAME_DELAY_MS);
               	frame_counter++;
    	}

	main_menu();
}

void multi_player_game() {
	int input;
        int running = 1;

	reset_game_board();

        while (running) {
                input = getch();
                switch (input) {
                        case 'w':
                        case 'W':
                                make_move(UP, &left_paddle);
                                break;

                        case 's':
                        case 'S':
                                make_move(DOWN, &left_paddle);
                                break;

			case KEY_UP:
                                make_move(UP, &right_paddle);
                                break;

			case KEY_DOWN:
                                make_move(DOWN, &right_paddle);
                                break;

                        case 'q':
                        case 'Q':
                                running = 0;
                                break;

                        default:
                                break;
                }

		update_delay_frames();
                if (frame_counter >= delay) {
                        move_ball();
                        frame_counter = 0;
                }

                render_board();
                napms(FRAME_DELAY_MS);
                frame_counter++;
        }
	
	main_menu();
}

void main_menu() {
	int input;

	clear();
	refresh();

	game_type = unspecified;
        mvprintw(1, 0, "Singleplayer or Multiplayer? (type S/M or Q to quit) ");
        while(game_type == unspecified) {
                input = getch();
                switch (input) {
                        case 'S':
                        case 's':
                                game_type = single_player;
                                break;
                        case 'M':
                        case 'm':
                                game_type = multi_player;
				break;
                        case 'Q':
                        case 'q':
                                return;
                        default:
                                break;
                }
        }

	switch(game_type) {
                case single_player:
                        single_player_game();
                        break;
                case multi_player:
                        multi_player_game();
                        break;
                default:
                        break;
        }
}

void reset_game_board() {
	left_paddle.point = (BOARD_HEIGHT - PADDLE_HEIGHT) / 2;
        right_paddle.point = (BOARD_HEIGHT - PADDLE_HEIGHT) / 2;
        reset_ball();
        reset_score();

	multi_score[0] = 0;
        multi_score[1] = 0;
	score = 0;
}

int main() {

	FILE *score_f;
	char buffer[32];

	score_f = fopen("highest_score.txt", "r");
	if(score_f && fgets(buffer, sizeof(buffer), score_f)) {
		max_all_time_score = atoi(buffer);
		fclose(score_f);
	}
	else
		max_all_time_score = 0;

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	main_menu();

	score_f = fopen("highest_score.txt", "w+");
	fprintf(score_f, "%d", max_all_time_score);
	fclose(score_f);

	endwin();

	return 0;
}
