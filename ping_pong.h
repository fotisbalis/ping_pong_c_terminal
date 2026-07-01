#ifndef PING_PONG_H
#define PING_PONG_H

typedef struct Paddle {
        int point;
} Paddle;

typedef struct Ball {
        int x;
        int y;
        int vx;
        int vy;
} Ball;

typedef enum Direction {
        UP = -1,
        DOWN = 1,
        RIGHT = 1,
        LEFT = -1
} Direction;

typedef enum GameType {
        single_player,
        multi_player,
        unspecified
} GameType;

void make_move(Direction direction, Paddle *paddle);
void reset_ball();
void reset_score();
void reset_game_board();
void update_delay_frames();
void draw_border();
void print_score_controls();
void render_board();
void move_ball();
void single_player_game();
void multi_player_game();
void main_menu();

#endif
