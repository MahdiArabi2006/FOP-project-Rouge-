#include <ncurses.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <locale.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>
char *music_list[3] = {
    "look on down from the bridge.mp3",
    "between the bars.mp3",
    "bache nane.mp3"
};
int choice_mu;
int music_playing = 0;
int size = 0;
int last_throw = 0;
int game_over = 0;
time_t start;
time_t end;
time_t damage;
int first_damage = 0;
int coefficient_health = 1;
int coefficient_damage = 1;
int coefficient_speed = 1;
const char* b = "🗱";//خنجر
const char* a = "⚒️";//گرز
const char* c = "🪄";//عصای جادویی
const char* d = "➳";//تیر عادی
const char* e = "🗡";//شمشیر
const char* f = "🏺";//طلسم سلامتی
const char* g = "🥣";//طلسم سرعت
const char* h = "🧪";//طلسم آسیب
const char* l = "🍞";//غذا
const char* o = "🍕";//غذا اعلا
const char* u = "🥗";//غذای جادویی
const char* n = "🦠";//غذای فاسد
const char* m = "🟡";//طلا
const char* v = "⚫";//طلای سیاه
const char* r = "△";
const char* y = "🏁";
const char* x = "🏆";
#pragma pack(1)
struct Food{
    int x_food;
    int y_food;
    int number_room;
    int type;//1-4
};
#pragma pack()
#pragma pack(1)
struct Spell{
    int x_spell;
    int y_spell;
    int number_room;
    int type;//1-3
    int is_exist;
};
#pragma pack()
#pragma pack(1)
struct Wepon{
    int x_wepon;
    int y_wepon;
    int number_room;
    int type;//1-5
    int is_use;
    int is_exist;
    int cllect;
};
#pragma pack()
#pragma pack(1)
struct Gold{
    int x_gold;
    int y_gold;
    int is_black;
    int worth;
    int number_room;
};
#pragma pack()
#pragma pack(1)
struct Player{
    int x;
    int y;
    int food_number;
    struct Food foods[12];
    int health;
    int wepon_number;
    struct Wepon wepons[12];
    int spell_number;
    struct Spell spells[12];
    int gold;
    int have_master_key;
    int last_map;
    int x_map1;
    int x_map2;
    int x_map3;
    int x_map4;
    int x_map5;
    int y_map1;
    int y_map2;
    int y_map3;
    int y_map4;
    int y_map5;
    int hungry;
    int game_number;
    int total_game;
    time_t priod;
    int win_last_game;
    time_t begin_game;
    int color;
    int level;
    int step;
    int type_wepon_chosen;
    int total_step;
    int total_step_food;
};
#pragma pack()
#pragma pack(1)
struct Trap{
    int x_trap;
    int y_trap;
    int number_room;
};
#pragma pack()
#pragma pack(1)
struct Corridor{
    int x_corridor;
    int y_corrifer;
    int is_seen;
};
#pragma pack()
#pragma pack(1)
struct Pillar{
    int x_pillar;
    int y_pillar;
    int number_room;
};
#pragma pack()
#pragma pack(1)
struct Door{
    int x_door;  
    int y_door;
    int room_number;
    int is_secret_door;
    int is_password_door;
};
#pragma pack()
#pragma pack(1)
struct Room {
    int x;  
    int y;
    int size;
    struct Door doors[2];
    int x_window;
    int y_window;
    int is_seen;
};

struct Demon{
    int healt;
    int type;//1-5
    int x_demon;
    int y_demon;
    int number_of_room;
    int can_move;
};

#pragma pack()
#pragma pack(1)
struct Map{
    int x_stair;
    int y_stair;
    int number_room_stair;
    int number_of_rooms;
    struct Room rooms[8];
    struct Pillar pillors[3];
    int corridor_count;
    struct Corridor corridors[300];
    struct Trap traps[3];
    int x_fight_room;
    int y_fight_room;
    int number_room_fight_room;
    int number_Spell_room;
    int number_Password_Doors_room;
    int x_create_paasword;
    int y_create_paasword;
    int number_Master_Key_room;
    int x_Master_Key;
    int y_Master_Key;
    struct Gold golds[3];
    struct Wepon wepons[3];
    struct Spell spells[3];
    struct Food foods[3];
    int last_password;
    int x_password_door;
    int y_password_door;
    int number_Treasure_Room;
    int x_end;
    int y_end;
    int has_treasure_room;
    struct Demon demons[5];
    int number_of_wepons_use;
    struct Wepon wepons_use[300];
};
#pragma pack()
#pragma pack(1)
struct Game{
    struct Map maps[5];
}game;
#pragma pack()

void generate_map(struct Map *map,char name[100],int number);
int can_have_room(struct Room rooms[8], int current_room);  
void print_map(int number_of_rooms, struct Room rooms[8], struct Map *map);  
int is_path_clear(struct Map *map, int x, int y);
int draw_corridor(struct Map *map, int start_x, int start_y, int end_x, int end_y);
void connect_doors(struct Map *map);
int deciphering();
int is_pillor(struct Map *map,int x, int y);
int is_trap(struct Map *map,int x, int y);
int is_gold(struct Map *map,int x, int y);
int is_spell(struct Map *map,int x, int y);
int is_wepon(struct Map *map,int x, int y);
int is_food(struct Map *map,int x, int y);
int is_demon(struct Map *map,int x, int y);
void move_player(struct Map *map,struct Player *p,int number_map);
int is_not_wall(struct Map *map, int x, int y);
int is_door(struct Map *map,int x, int y);
int is_corridor(struct Map *map,int x, int y);
void move_effect(const char* state,struct Player *p,struct Map* map,int x,int y);
void f_gold(struct Map *map,int x, int y);
void f_spell(struct Map *map,int x, int y);
void f_wepon(struct Map *map,int x, int y);
void f_food(struct Map *map,int x, int y);
int index_gold(struct Map *map,int x, int y);
int index_spell(struct Map *map,int x, int y);
int index_wepon(struct Map *map,int x, int y);
int index_food(struct Map *map,int x, int y);
int is_master_key(struct Map *map,int x, int y);
void f_master_key(struct Map *map,int x, int y);
int is_password_door(struct Map *map,int x, int y);
int is_fight_room(struct Map *map,int x, int y);
void f_fight(struct Map *map,int x,int y);
int is_create_password(struct Map *map,int x,int y);
int is_secret_door(struct Map *map,int x, int y);
void create_password(struct Map *map,time_t time_start);
int panel(struct Player* p,struct Map *map);
void regular_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start);
void f_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start);
void g_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start);
void save_map(char filename[100], struct Map *map,int number);
void load_map(char filename[100], struct Map *map,int number);
int is_room(struct Map *map, int x, int y);
void print_corridor(struct Map *map);
void print_room(struct Map *map,struct Player *p);
void print(struct Map *map,int x, int y,struct Player *p);
void make_corrider_is_seen(struct Map *map,int x, int y);
void check_corrider(struct Map *map,int x, int y);
int is_room_and_index(struct Map *map,int x,int y);
int control_name(char name[100]);
int control_email(char email[20]);
int control_password(char password[20]);
void create_new_player();
int valid_name_and_password(char name[100], char password[20]);
void enter_player();
void create_new_game(char name[100]);
void continue_last_game(char name[100]);
void seting_menu(char name[100]);
void profile(char name[100]);
void table_of_players(char name[100]);
void menus();
void players_menus(char name[100]);
void guest();
void save_player(char filename[100], struct Player * p);
void load_player(char filename[100], struct Player * p);
int is_stair(struct Map *map,int x,int y);
void fightroom(struct Player *p);
void show_wepons(struct Player *p);
void show_spells(struct Player *p);
void show_foods(struct Player *p);
void show_map(struct Map *map);
int is_spell_room(struct Map *map,int x,int y);
int file_exists(char filename[100]);
void remove_file(char filename[100],int number);
void random_password();
void show_password(char name[100]);
void music_menu();
int music_choice();
void *play_music(void *arg);
void stop_music();
void move_demon(struct Map *map,struct Player *p);
int witch_room(struct Map *map, int x, int y);
void print_after_move_demon(struct Map *map,int x, int y);
int have_wepon(struct Player *p,int type);
int witch_demon(struct Map *map,int x,int y);
char* name_of_demon(int type);
int is_wepon_use(struct Map *map,int x, int y);
int index_wepon_use(struct Map *map,int x, int y);
void f_wepon_use(struct Map *map,int x, int y);
void throw_wepon(int type,struct Map *map,struct Player *p,int is_repeat);
int is_win(struct Map *map);
void chose_color(struct Player *p);
void chose_level(struct Player *p);

int main(){
    setlocale(LC_CTYPE, "");
    initscr();
    cbreak();  
    noecho();  
    srand(time(0));
	while (1){
		menus();
		break;
	}
	endwin();
	return 0;
}

void generate_map(struct Map *map,char name[100],int number) {
    //Rooms
    if(number == 5){
        map->number_of_rooms = 1;
    }else{
        map->number_of_rooms = ((unsigned int)rand() % 3) + 6; // 6-8
    }
    int current_room = 0;  
    while (current_room < map->number_of_rooms) {  
        if(current_room + 1 == 1){
            map->rooms[current_room].x = (unsigned int)rand() % (11 - (0 + 2) + 1) + (0 + 2);  
            map->rooms[current_room].y = (unsigned int)rand() % (10 - 2 + 1) + 2;
            if(number == 5){
                map->rooms[current_room].size = 10;
            }else{
                map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            }
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;
            }
        }
        else if(current_room + 1 == 2){
            map->rooms[current_room].x = (unsigned int)rand() % (25 - (16) + 1) + (16);  
            map->rooms[current_room].y = (unsigned int)rand() % (10 - 2 + 1) + 2;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {
                current_room++;  
            } 
        }
        else if(current_room + 1 == 3){
            map->rooms[current_room].x = (unsigned int)rand() % (11 - (0 + 2) + 1) + (0 + 2);
            map->rooms[current_room].y = (unsigned int)rand() % (23 - 14 + 1) + 14;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;  
            } 
        }
        else if(current_room + 1 == 4){
            map->rooms[current_room].x = (unsigned int)rand() % (25 - (16) + 1) + (16);  
            map->rooms[current_room].y = (unsigned int)rand() % (23 - 14 + 1) + 14;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;  
            } 
        }
        else if(current_room + 1 == 5){
            map->rooms[current_room].x = (unsigned int)rand() % (42 - (30) + 1) + (30);  
            map->rooms[current_room].y = (unsigned int)rand() % (10 - 2 + 1) + 2;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;

            }
        }
        else if(current_room + 1 == 6){
            map->rooms[current_room].x = (unsigned int)rand() % (60 - (47) + 1) + (47);  
            map->rooms[current_room].y = (unsigned int)rand() % (10 - 2 + 1) + 2;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {
                current_room++;  
            } 
        }
        else if(current_room + 1 == 7){
            map->rooms[current_room].x = (unsigned int)rand() % (42 - (30) + 1) + (30);  
            map->rooms[current_room].y = (unsigned int)rand() % (23 - 14 + 1) + 14;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;  
            } 
        }
        else if(current_room + 1 == 8){
            map->rooms[current_room].x = (unsigned int)rand() % (60 - (47) + 1) + (47);  
            map->rooms[current_room].y = (unsigned int)rand() % (23 - 14 + 1) + 14;
            map->rooms[current_room].size = ((unsigned int)rand() % 3) + 6;
            map->rooms[current_room].is_seen = 0;
            if (can_have_room(map->rooms, current_room)) {  
                current_room++;  
            } 
        }
    }
    if(number != 5){
        //Doors
        current_room = 0;
        while(current_room < map->number_of_rooms){
            if(current_room + 1 == 1){
                //2
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[0].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //3
                map->rooms[current_room].doors[1].x_door = rand() % ((map->rooms[current_room].x + map->rooms[current_room].size - 1) - (map->rooms[current_room].x + 1) + 1) + map->rooms[current_room].x + 1;
                map->rooms[current_room].doors[1].y_door = map->rooms[current_room].y + map->rooms[current_room].size;
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 2){
                //2
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[0].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //4
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[1].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 3){
                //1
                map->rooms[current_room].doors[0].x_door = rand() % ((map->rooms[current_room].x + map->rooms[current_room].size - 1) - (map->rooms[current_room].x + 1) + 1) + (map->rooms[current_room].x + 1);
                map->rooms[current_room].doors[0].y_door = map->rooms[current_room].y;
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //2
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[1].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 4){
                //2
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[0].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;

                //4
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[1].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 5){
                //2
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[0].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //4
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[1].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 6){
                //4
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[0].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //3
                map->rooms[current_room].doors[1].x_door = rand() % ((map->rooms[current_room].x + map->rooms[current_room].size - 1) - (map->rooms[current_room].x + 1) + 1) + map->rooms[current_room].x + 1;
                map->rooms[current_room].doors[1].y_door = map->rooms[current_room].y + map->rooms[current_room].size;
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 7){
                //2
                map->rooms[current_room].doors[0].x_door = map->rooms[current_room].x + map->rooms[current_room].size;
                map->rooms[current_room].doors[0].y_door = rand() % ((map->rooms[current_room].y + map->rooms[current_room].size - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //4
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[1].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            if(current_room + 1 == 8){
                //1
                map->rooms[current_room].doors[0].x_door = rand() % ((map->rooms[current_room].x + map->rooms[current_room].size - 1) - (map->rooms[current_room].x + 1) + 1) + (map->rooms[current_room].x + 1);
                map->rooms[current_room].doors[0].y_door = map->rooms[current_room].y;
                map->rooms[current_room].doors[0].room_number = current_room;
                map->rooms[current_room].doors[0].is_secret_door = 0;
                map->rooms[current_room].doors[0].is_password_door = 0;
                //4
                map->rooms[current_room].doors[1].x_door = map->rooms[current_room].x;
                map->rooms[current_room].doors[1].y_door = rand() % (((map->rooms[current_room].y + map->rooms[current_room].size) - 1) - (map->rooms[current_room].y + 1) + 1) + (map->rooms[current_room].y + 1);
                map->rooms[current_room].doors[1].room_number = current_room;
                map->rooms[current_room].doors[1].is_secret_door = 0;
                map->rooms[current_room].doors[1].is_password_door = 0;
            }
            current_room ++;
        }
        //Window
        //Corridor
        connect_doors(map);
        if(map->number_of_rooms == 6){
            map->corridors[size].x_corridor = map->rooms[0].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[0].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[0].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[0].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[0].x_door;
            map->corridors[size].y_corrifer = map->rooms[2].doors[0].y_door - 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[1].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[2].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[0].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[5].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[5].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
        }
        if(map->number_of_rooms == 7){
            map->corridors[size].x_corridor = map->rooms[0].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[0].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[0].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[0].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[0].x_door;
            map->corridors[size].y_corrifer = map->rooms[2].doors[0].y_door - 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[1].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[2].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[0].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[5].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[5].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[6].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[6].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[6].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[6].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
        }
        if(map->number_of_rooms == 8){
            map->corridors[size].x_corridor = map->rooms[0].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[0].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[0].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[0].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[1].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[1].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[0].x_door;
            map->corridors[size].y_corrifer = map->rooms[2].doors[0].y_door - 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[2].doors[1].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[2].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[3].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[3].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[4].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[4].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[0].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[5].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[5].doors[1].x_door;
            map->corridors[size].y_corrifer = map->rooms[5].doors[1].y_door + 1;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[6].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[6].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[6].doors[0].x_door + 1;
            map->corridors[size].y_corrifer = map->rooms[6].doors[0].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[7].doors[1].x_door - 1;
            map->corridors[size].y_corrifer = map->rooms[7].doors[1].y_door;
            map->corridors[size].is_seen = 0;
            size++;
            map->corridors[size].x_corridor = map->rooms[7].doors[0].x_door;
            map->corridors[size].y_corrifer = map->rooms[7].doors[0].y_door - 1;
            map->corridors[size].is_seen = 0;
            size++;
        }
        map->corridor_count = size;
        size = 0;
        map->has_treasure_room = 0;
        if(number == 4){
            //Treasure Room
            map->number_Treasure_Room = rand() % map->number_of_rooms;
            map->has_treasure_room = 1;
        }
        //Spell_room && Secret Doors
        if(number == 4){
            map->number_Spell_room = rand() % map->number_of_rooms;
            while(map->number_Spell_room == map->number_Treasure_Room){
                map->number_Spell_room = rand() % map->number_of_rooms;
            }
        }else{
            map->number_Spell_room = rand() % map->number_of_rooms;
            map->rooms[map->number_Spell_room].doors[0].is_secret_door = 1;
            map->rooms[map->number_Spell_room].doors[1].is_secret_door = 1;
        }
        //Traps
        if(number == 4){
            for(int i = 0; i < 3; i++){
                map->traps[i].number_room = map->number_Treasure_Room;
            }
        }else{
            for(int i = 0; i < 3; i++){
                map->traps[i].number_room = rand() % map->number_of_rooms;
                while(map->traps[i].number_room == map->number_Spell_room){
                    map->traps[i].number_room = rand() % map->number_of_rooms;
                }
            }
        }
        for(int i = 0; i < 3; i++){
            map->traps[i].x_trap = 0;
            map->traps[i].y_trap = 0;
            while(map->traps[i].x_trap == 0 || map->traps[i].y_trap == 0 || (map->traps[i].x_trap == map->x_stair && map->traps[i].y_trap == map->y_stair) || (map->traps[i].x_trap == map->pillors[0].x_pillar && map->traps[i].y_trap == map->pillors[0].y_pillar) || (map->traps[i].x_trap == map->pillors[1].x_pillar && map->traps[i].y_trap == map->pillors[1].y_pillar) || (map->traps[i].x_trap == map->pillors[2].x_pillar && map->traps[i].y_trap == map->pillors[2].y_pillar)){
                map->traps[i].x_trap = rand() % ((map->rooms[map->traps[i].number_room].x + map->rooms[map->traps[i].number_room].size - 1) - (map->rooms[map->traps[i].number_room].x + 1) + 1) + ((map->rooms[map->traps[i].number_room].x + 1));
                map->traps[i].y_trap = rand() % ((map->rooms[map->traps[i].number_room].y + map->rooms[map->traps[i].number_room].size - 1) - (map->rooms[map->traps[i].number_room].y + 1) + 1) + ((map->rooms[map->traps[i].number_room].y + 1));
            }
        }
        //Fight_room
        if(number == 4){
            map->number_room_fight_room = map->number_Treasure_Room;
        }else{
            map->number_room_fight_room = rand() % map->number_of_rooms;
            while(map->number_room_fight_room == map->number_Spell_room){
                    map->number_room_fight_room = rand() % map->number_of_rooms;
            }
        }
        map->x_fight_room = 0;
        map->y_fight_room = 0;
        while(map->x_fight_room == 0 || map->y_fight_room == 0 || (map->x_fight_room == map->x_stair && map->y_fight_room == map->y_stair) || (map->x_fight_room == map->pillors[0].x_pillar && map->y_fight_room == map->pillors[0].y_pillar) || (map->x_fight_room == map->pillors[1].x_pillar && map->y_fight_room == map->pillors[1].y_pillar) || (map->x_fight_room == map->pillors[2].x_pillar && map->y_fight_room == map->pillors[2].y_pillar) || (map->x_fight_room == map->traps[0].x_trap && map->y_fight_room == map->traps[0].y_trap) || (map->x_fight_room == map->traps[1].x_trap && map->y_fight_room == map->traps[1].y_trap) || (map->x_fight_room == map->traps[2].x_trap && map->y_fight_room == map->traps[2].y_trap)){
            map->x_fight_room = rand() % ((map->rooms[map->number_room_fight_room].x + map->rooms[map->number_room_fight_room].size - 1) - (map->rooms[map->number_room_fight_room].x + 1) + 1) + ((map->rooms[map->number_room_fight_room].x + 1));
            map->y_fight_room = rand() % ((map->rooms[map->number_room_fight_room].y + map->rooms[map->number_room_fight_room].size - 1) - (map->rooms[map->number_room_fight_room].y + 1) + 1) + ((map->rooms[map->number_room_fight_room].y + 1));
        }
        //Stair
        map->number_room_stair = rand() % map->number_of_rooms;
        while(map->number_room_stair == map->number_Spell_room){
                map->number_room_stair = rand() % map->number_of_rooms;
        }
        map->x_stair = 0;
        map->y_stair = 0;
        while(map->x_stair == 0 || map->y_stair == 0){
            map->x_stair = rand() % ((map->rooms[map->number_room_stair].x + map->rooms[map->number_room_stair].size - 1) - (map->rooms[map->number_room_stair].x + 1) + 1) + ((map->rooms[map->number_room_stair].x + 1));
            map->y_stair = rand() % ((map->rooms[map->number_room_stair].y + map->rooms[map->number_room_stair].size - 1) - (map->rooms[map->number_room_stair].y + 1) + 1) + ((map->rooms[map->number_room_stair].y + 1));
        }
        //Pillar
        for(int i = 0; i < 3; i++){
            map->pillors[i].number_room = rand() % map->number_of_rooms;
        }
        for(int i = 0; i < 3; i++){
            map->pillors[i].x_pillar = 0;
            map->pillors[i].y_pillar = 0;
            while(map->pillors[i].x_pillar == 0 || map->pillors[i].y_pillar == 0 || (map->pillors[i].x_pillar == map->x_stair && map->pillors[i].y_pillar == map->y_stair)){
                map->pillors[i].x_pillar = rand() % ((map->rooms[map->pillors[i].number_room].x + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].x + 1) + 1) + ((map->rooms[map->pillors[i].number_room].x + 1));
                map->pillors[i].y_pillar = rand() % ((map->rooms[map->pillors[i].number_room].y + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].y + 1) + 1) + ((map->rooms[map->pillors[i].number_room].y + 1));
            }
        }
        //Password Doors
        map->number_Password_Doors_room = rand() % map->number_of_rooms;
        while(map->number_Password_Doors_room == map->number_Spell_room){
            map->number_Password_Doors_room = rand() % map->number_of_rooms;
        }
        map->rooms[map->number_Password_Doors_room].doors[0].is_password_door = 1;
        map->x_password_door = map->rooms[map->number_Password_Doors_room].doors[0].x_door;
        map->y_password_door = map->rooms[map->number_Password_Doors_room].doors[0].y_door;
        //Create_password
        map->x_create_paasword = 0;
        map->y_create_paasword = 0;
        while(map->x_create_paasword == 0 || map->y_create_paasword == 0 || (map->x_create_paasword == map->x_stair && map->y_create_paasword == map->y_stair) || (map->x_create_paasword == map->x_fight_room && map->y_create_paasword == map->y_fight_room) || (map->x_create_paasword == map->pillors[0].x_pillar && map->y_create_paasword == map->pillors[0].y_pillar) || (map->x_create_paasword == map->pillors[1].x_pillar && map->y_create_paasword == map->pillors[1].y_pillar) || (map->x_create_paasword == map->pillors[2].x_pillar && map->y_create_paasword == map->pillors[2].y_pillar) || (map->x_create_paasword == map->traps[0].x_trap && map->y_create_paasword == map->traps[0].y_trap) || (map->x_create_paasword == map->traps[1].x_trap && map->y_create_paasword == map->traps[1].y_trap) || (map->x_create_paasword == map->traps[2].x_trap && map->y_create_paasword == map->traps[2].y_trap)){
            map->x_create_paasword = rand() % ((map->rooms[map->number_Password_Doors_room].x + map->rooms[map->number_Password_Doors_room].size - 1) - (map->rooms[map->number_Password_Doors_room].x + 1) + 1) + ((map->rooms[map->number_Password_Doors_room].x + 1));
            map->y_create_paasword = rand() % ((map->rooms[map->number_Password_Doors_room].y + map->rooms[map->number_Password_Doors_room].size - 1) - (map->rooms[map->number_Password_Doors_room].y + 1) + 1) + ((map->rooms[map->number_Password_Doors_room].y + 1));
        }
        //Master Key
        map->number_Master_Key_room = rand() % map->number_of_rooms;
        map->x_Master_Key = 0;
        map->y_Master_Key = 0;
        while(map->x_Master_Key == 0 || map->y_Master_Key == 0 || (map->x_Master_Key == map->x_stair && map->y_Master_Key == map->y_stair) || (map->x_Master_Key == map->x_create_paasword && map->y_Master_Key == map->y_create_paasword) || (map->x_Master_Key == map->x_fight_room && map->y_Master_Key == map->y_fight_room) || is_pillor(map,map->x_Master_Key,map->y_Master_Key) || is_trap(map,map->x_Master_Key,map->y_Master_Key)){
            map->x_Master_Key = rand() % ((map->rooms[map->number_Master_Key_room].x + map->rooms[map->number_Master_Key_room].size - 1) - (map->rooms[map->number_Master_Key_room].x + 1) + 1) + ((map->rooms[map->number_Master_Key_room].x + 1));
            map->y_Master_Key = rand() % ((map->rooms[map->number_Master_Key_room].y + map->rooms[map->number_Master_Key_room].size - 1) - (map->rooms[map->number_Master_Key_room].y + 1) + 1) + ((map->rooms[map->number_Master_Key_room].y + 1));
        }
        //Foods
        for(int i = 0; i < 3; i++){
            map->foods[i].number_room = rand() % map->number_of_rooms;
            while(map->foods[i].number_room == map->number_Spell_room){
                map->foods[i].number_room = rand() % map->number_of_rooms;
            }
            map->foods[i].type = rand() % 4 + 1;
        }
        for(int i = 0; i < 3; i++){
            map->foods[i].x_food = 0;
            map->foods[i].y_food = 0;
            while(map->foods[i].x_food == 0 || map->foods[i].y_food == 0 || (map->foods[i].x_food == map->x_stair && map->foods[i].y_food == map->y_stair) || (map->foods[i].x_food == map->x_create_paasword && map->foods[i].y_food == map->y_create_paasword) || (map->foods[i].x_food == map->x_fight_room && map->foods[i].y_food == map->y_fight_room) || (map->foods[i].x_food == map->x_Master_Key && map->foods[i].y_food == map->y_Master_Key) || is_pillor(map,map->foods[i].x_food,map->foods[i].y_food) || is_trap(map,map->foods[i].x_food,map->foods[i].y_food)){
                map->foods[i].x_food = rand() % ((map->rooms[map->foods[i].number_room].x + map->rooms[map->foods[i].number_room].size - 1) - (map->rooms[map->foods[i].number_room].x + 1) + 1) + ((map->rooms[map->foods[i].number_room].x + 1));
                map->foods[i].y_food = rand() % ((map->rooms[map->foods[i].number_room].y + map->rooms[map->foods[i].number_room].size - 1) - (map->rooms[map->foods[i].number_room].y + 1) + 1) + ((map->rooms[map->foods[i].number_room].y + 1));
            }
        }
        //Spells
        for(int i = 0; i < 3; i++){
            map->spells[i].number_room = map->number_Spell_room;
            map->spells[i].type = rand() % 3 + 1;
        }
        for(int i = 0; i < 3; i++){
            map->spells[i].x_spell = 0;
            map->spells[i].y_spell = 0;
            while(map->spells[i].x_spell == 0 || map->spells[i].y_spell == 0 || (map->spells[i].x_spell == map->x_stair && map->spells[i].y_spell == map->y_stair) || (map->spells[i].x_spell == map->x_create_paasword && map->spells[i].y_spell == map->y_create_paasword) || (map->spells[i].x_spell == map->x_fight_room && map->spells[i].y_spell == map->y_fight_room) || (map->spells[i].x_spell == map->x_Master_Key && map->spells[i].y_spell == map->y_Master_Key) || is_pillor(map,map->spells[i].x_spell,map->spells[i].y_spell) || is_trap(map,map->spells[i].x_spell,map->spells[i].y_spell) || is_food(map,map->spells[i].x_spell,map->spells[i].y_spell)){
                map->spells[i].x_spell = rand() % ((map->rooms[map->spells[i].number_room].x + map->rooms[map->spells[i].number_room].size - 1) - (map->rooms[map->spells[i].number_room].x + 1) + 1) + ((map->rooms[map->spells[i].number_room].x + 1));
                map->spells[i].y_spell = rand() % ((map->rooms[map->spells[i].number_room].y + map->rooms[map->spells[i].number_room].size - 1) - (map->rooms[map->spells[i].number_room].y + 1) + 1) + ((map->rooms[map->spells[i].number_room].y + 1));
            }
        }
        //Wepons
        for(int i = 0; i < 3; i++){
            map->wepons[i].number_room = rand() % map->number_of_rooms;
            while(map->wepons[i].number_room == map->number_Spell_room){
                map->wepons[i].number_room = rand() % map->number_of_rooms;
            }
            map->wepons[i].type = rand() % 4 + 2;
        }
        for(int i = 0; i < 3; i++){
            map->wepons[i].x_wepon = 0;
            map->wepons[i].y_wepon = 0;
            while(map->wepons[i].x_wepon == 0 || map->wepons[i].y_wepon == 0 || (map->wepons[i].x_wepon == map->x_stair && map->wepons[i].y_wepon == map->y_stair) || (map->wepons[i].x_wepon == map->x_create_paasword && map->wepons[i].y_wepon == map->y_create_paasword) || (map->wepons[i].x_wepon == map->x_fight_room && map->wepons[i].y_wepon == map->y_fight_room) || (map->wepons[i].x_wepon == map->x_Master_Key && map->wepons[i].y_wepon == map->y_Master_Key) || is_pillor(map,map->wepons[i].x_wepon,map->wepons[i].y_wepon) || is_trap(map,map->wepons[i].x_wepon,map->wepons[i].y_wepon) || is_food(map,map->wepons[i].x_wepon,map->wepons[i].y_wepon) || is_spell(map,map->wepons[i].x_wepon,map->wepons[i].y_wepon)){
                map->wepons[i].x_wepon = rand() % ((map->rooms[map->wepons[i].number_room].x + map->rooms[map->wepons[i].number_room].size - 1) - (map->rooms[map->wepons[i].number_room].x + 1) + 1) + ((map->rooms[map->wepons[i].number_room].x + 1));
                map->wepons[i].y_wepon = rand() % ((map->rooms[map->wepons[i].number_room].y + map->rooms[map->wepons[i].number_room].size - 1) - (map->rooms[map->wepons[i].number_room].y + 1) + 1) + ((map->rooms[map->wepons[i].number_room].y + 1));
            }
        }
        //Golds
        int wich_black = rand() % 5;
        for(int i = 0; i < 3; i++){
            map->golds[i].number_room = rand() % map->number_of_rooms;
            while(map->golds[i].number_room == map->number_Spell_room){
                map->golds[i].number_room = rand() % map->number_of_rooms;
            }
            if(i == wich_black){
                map->golds[i].is_black = 1;
                map->golds[i].worth = 500;
            }
            map->golds[i].worth = rand() % (300 - 100 + 1) + 100;
            map->golds[i].is_black = 0;
        }
        for(int i = 0; i < 3; i++){
            map->golds[i].x_gold = 0;
            map->golds[i].y_gold = 0;
            while(map->golds[i].x_gold == 0 || map->golds[i].y_gold == 0 || (map->golds[i].x_gold == map->x_stair && map->golds[i].y_gold == map->y_stair) || (map->golds[i].x_gold == map->x_create_paasword && map->golds[i].y_gold == map->y_create_paasword) || (map->golds[i].x_gold == map->x_fight_room && map->golds[i].y_gold == map->y_fight_room) || (map->golds[i].x_gold == map->x_Master_Key && map->golds[i].y_gold == map->y_Master_Key) || is_pillor(map,map->golds[i].x_gold,map->golds[i].y_gold) || is_trap(map,map->golds[i].x_gold,map->golds[i].y_gold) || is_food(map,map->golds[i].x_gold,map->golds[i].y_gold) || is_spell(map,map->golds[i].x_gold,map->golds[i].y_gold) || is_wepon(map,map->golds[i].x_gold,map->golds[i].y_gold)){
                map->golds[i].x_gold = rand() % ((map->rooms[map->golds[i].number_room].x + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].x + 1) + 1) + ((map->rooms[map->golds[i].number_room].x + 1));
                map->golds[i].y_gold = rand() % ((map->rooms[map->golds[i].number_room].y + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].y + 1) + 1) + ((map->rooms[map->golds[i].number_room].y + 1));
            }
        }
        //treasure
        if(number == 4){
            map->x_end = 0;
            map->y_end = 0;
            while(map->x_end == 0 || map->y_end == 0 || (map->x_end == map->x_stair && map->y_end == map->y_stair) || (map->x_end == map->x_create_paasword && map->y_end == map->y_create_paasword) || (map->x_end == map->x_fight_room && map->y_end == map->y_fight_room) || (map->x_end == map->x_Master_Key && map->y_end == map->y_Master_Key) || is_pillor(map,map->x_end,map->y_end) || is_trap(map,map->x_end,map->y_end) || is_food(map,map->x_end,map->y_end) || is_spell(map,map->x_end,map->y_end) || is_wepon(map,map->x_end,map->y_end) || is_gold(map,map->x_end,map->y_end)){
                map->x_end = rand() % ((map->rooms[map->number_Treasure_Room].x + map->rooms[map->number_Treasure_Room].size - 1) - (map->rooms[map->number_Treasure_Room].x + 1) + 1) + ((map->rooms[map->number_Treasure_Room].x + 1));
                map->y_end = rand() % ((map->rooms[map->number_Treasure_Room].y + map->rooms[map->number_Treasure_Room].size - 1) - (map->rooms[map->number_Treasure_Room].y + 1) + 1) + ((map->rooms[map->number_Treasure_Room].y + 1));
            }
        }
        //demons
        if(number == 4){
            for(int i = 0; i < 5; i++){
                map->demons[i].number_of_room = rand() % map->number_of_rooms;
                while(map->demons[i].number_of_room == map->number_Spell_room || map->demons[i].number_of_room == map->number_Treasure_Room){
                    map->demons[i].number_of_room = rand() % map->number_of_rooms;
                }
                map->demons[i].type = rand() % 5 + 1;
            } 
        }else{
            for(int i = 0; i < 5; i++){
                map->demons[i].number_of_room = rand() % map->number_of_rooms;
                while(map->demons[i].number_of_room == map->number_Spell_room){
                    map->demons[i].number_of_room = rand() % map->number_of_rooms;
                }
                map->demons[i].type = rand() % 5 + 1;
            }
        }
        for(int i = 0; i < 5; i++){
            map->demons[i].x_demon = 0;
            map->demons[i].y_demon = 0;
            map->demons[i].can_move = 1;
            while(map->demons[i].x_demon == 0 || map->demons[i].y_demon == 0 || (map->demons[i].x_demon == map->x_stair && map->demons[i].y_demon == map->y_stair) || (map->demons[i].x_demon == map->x_create_paasword && map->demons[i].y_demon == map->y_create_paasword) || (map->demons[i].x_demon == map->x_fight_room && map->demons[i].y_demon == map->y_fight_room) || (map->demons[i].x_demon == map->x_Master_Key && map->demons[i].y_demon == map->y_Master_Key) || is_pillor(map,map->demons[i].x_demon,map->demons[i].y_demon) || is_trap(map,map->demons[i].x_demon,map->demons[i].y_demon) || is_food(map,map->demons[i].x_demon,map->demons[i].y_demon) || is_spell(map,map->demons[i].x_demon,map->demons[i].y_demon) || is_wepon(map,map->demons[i].x_demon,map->demons[i].y_demon) || is_gold(map,map->demons[i].x_demon,map->demons[i].y_demon)){
                map->demons[i].x_demon = rand() % ((map->rooms[map->demons[i].number_of_room].x + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].x + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].x + 1));
                map->demons[i].y_demon = rand() % ((map->rooms[map->demons[i].number_of_room].y + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].y + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].y + 1));
            }
        }
        for(int i = 0; i < 5; i++){
            if(map->demons[i].type == 1){
                map->demons[i].healt = 5;
            }
            else if(map->demons[i].type == 2){
                map->demons[i].healt = 10;
            }
            else if(map->demons[i].type == 3){
                map->demons[i].healt = 15;
            }
            else if(map->demons[i].type == 4){
                map->demons[i].healt = 20;
            }
            else if(map->demons[i].type == 5){
                map->demons[i].healt = 30;
            }
        }
    }else{
        for(int i = 0; i < 3; i++){
            map->traps[i].number_room = rand() % map->number_of_rooms;
        }
        for(int i = 0; i < 3; i++){
            map->traps[i].x_trap = rand() % ((map->rooms[map->traps[i].number_room].x + map->rooms[map->traps[i].number_room].size - 1) - (map->rooms[map->traps[i].number_room].x + 1) + 1) + ((map->rooms[map->traps[i].number_room].x + 1));
            map->traps[i].y_trap = rand() % ((map->rooms[map->traps[i].number_room].y + map->rooms[map->traps[i].number_room].size - 1) - (map->rooms[map->traps[i].number_room].y + 1) + 1) + ((map->rooms[map->traps[i].number_room].y + 1));
        }
        for(int i = 0; i < 5; i++){
            map->demons[i].number_of_room = rand() % map->number_of_rooms;
            map->demons[i].type = rand() % 5 + 1;
        }
        for(int i = 0; i < 5; i++){
            map->demons[i].can_move = 1;
            map->demons[i].x_demon = rand() % ((map->rooms[map->demons[i].number_of_room].x + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].x + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].x + 1));
            map->demons[i].y_demon = rand() % ((map->rooms[map->demons[i].number_of_room].y + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].y + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].y + 1));
            while(is_trap(map,map->demons[i].x_demon,map->demons[i].y_demon)){
                map->demons[i].x_demon = rand() % ((map->rooms[map->demons[i].number_of_room].x + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].x + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].x + 1));
                map->demons[i].y_demon = rand() % ((map->rooms[map->demons[i].number_of_room].y + map->rooms[map->demons[i].number_of_room].size - 1) - (map->rooms[map->demons[i].number_of_room].y + 1) + 1) + ((map->rooms[map->demons[i].number_of_room].y + 1));          
            }
        }
        for(int i = 0; i < 5; i++){
            if(map->demons[i].type == 1){
                map->demons[i].healt = 5;
            }
            else if(map->demons[i].type == 2){
                map->demons[i].healt = 10;
            }
            else if(map->demons[i].type == 3){
                map->demons[i].healt = 15;
            }
            else if(map->demons[i].type == 4){
                map->demons[i].healt = 20;
            }
            else if(map->demons[i].type == 5){
                map->demons[i].healt = 30;
            }
        }
        int wich_black = rand() % 5;
        for(int i = 0; i < 3; i++){
            map->golds[i].number_room = rand() % map->number_of_rooms;
            if(i == wich_black){
                map->golds[i].is_black = 1;
                map->golds[i].worth = 500;
            }
            map->golds[i].worth = rand() % (300 - 100 + 1) + 100;
            map->golds[i].is_black = 0;
        }
        for(int i = 0; i < 3; i++){
            map->golds[i].x_gold = rand() % ((map->rooms[map->golds[i].number_room].x + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].x + 1) + 1) + ((map->rooms[map->golds[i].number_room].x + 1));
            map->golds[i].y_gold = rand() % ((map->rooms[map->golds[i].number_room].y + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].y + 1) + 1) + ((map->rooms[map->golds[i].number_room].y + 1));
            while(is_trap(map,map->golds[i].x_gold,map->golds[i].y_gold) || is_demon(map,map->golds[i].x_gold,map->golds[i].y_gold)){
                map->golds[i].x_gold = rand() % ((map->rooms[map->golds[i].number_room].x + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].x + 1) + 1) + ((map->rooms[map->golds[i].number_room].x + 1));
                map->golds[i].y_gold = rand() % ((map->rooms[map->golds[i].number_room].y + map->rooms[map->golds[i].number_room].size - 1) - (map->rooms[map->golds[i].number_room].y + 1) + 1) + ((map->rooms[map->golds[i].number_room].y + 1));
            }
        }
        for(int i = 0; i < 3; i++){
            map->pillors[i].number_room = rand() % map->number_of_rooms;
        }
        for(int i = 0; i < 3; i++){
            map->pillors[i].x_pillar = rand() % ((map->rooms[map->pillors[i].number_room].x + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].x + 1) + 1) + ((map->rooms[map->pillors[i].number_room].x + 1));
            map->pillors[i].y_pillar = rand() % ((map->rooms[map->pillors[i].number_room].y + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].y + 1) + 1) + ((map->rooms[map->pillors[i].number_room].y + 1));
            while(is_gold(map,map->pillors[i].x_pillar,map->pillors[i].y_pillar) || is_demon(map,map->pillors[i].x_pillar,map->pillors[i].y_pillar) || is_trap(map,map->pillors[i].x_pillar,map->pillors[i].y_pillar)){
                map->pillors[i].x_pillar = rand() % ((map->rooms[map->pillors[i].number_room].x + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].x + 1) + 1) + ((map->rooms[map->pillors[i].number_room].x + 1));
                map->pillors[i].y_pillar = rand() % ((map->rooms[map->pillors[i].number_room].y + map->rooms[map->pillors[i].number_room].size - 1) - (map->rooms[map->pillors[i].number_room].y + 1) + 1) + ((map->rooms[map->pillors[i].number_room].y + 1));
            }
        }
    }
    map->number_of_wepons_use = 0;
    save_map(name,map,number);
}

int can_have_room(struct Room rooms[8], int current_room) {  
    for (int i = 0; i < current_room; i++) {  
        if ((rooms[current_room].x < rooms[i].x + rooms[i].size + 4) &&   
            (rooms[current_room].x + rooms[current_room].size > rooms[i].x - 4) &&   
            (rooms[current_room].y > rooms[i].y - rooms[i].size - 4) &&   
            (rooms[current_room].y - rooms[current_room].size < rooms[i].y + 4)) {  
            return 0;  
        }  
    }  
    return 1;  
}

int is_path_clear(struct Map *map, int x, int y) {
    for(int i = 0; i < map->number_of_rooms; i++) {
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x >= room_x1 && x <= room_x2 && y >= room_y1 && y <= room_y2) {
            return 0;
        }
    }
    return 1;
}

int draw_corridor(struct Map *map, int start_x, int start_y, int end_x, int end_y) {
    int x = start_x;
    int y = start_y;
    if(x == end_x && y == end_y){
        return 1;
    }
    if(x > end_x){
        x -= 1;
        if(x == end_x && y != end_y){
            x += 1;
        }
        else if(is_path_clear(map,x,y)){
            map->corridors[size].x_corridor = x;
            map->corridors[size].y_corrifer = y;
            map->corridors[size].is_seen = 0;
            size++;
            draw_corridor(map,x,y,end_x,end_y);
            return 1;
        }
        else{
            x += 1;
        }
    }
    if(x < end_x){
        x += 1;
        if(x == end_x && y != end_y){
            x -= 1;
        }
        else if(is_path_clear(map,x,y)){
            map->corridors[size].x_corridor = x;
            map->corridors[size].y_corrifer = y;
            map->corridors[size].is_seen = 0;
            size++;
            draw_corridor(map,x,y,end_x,end_y);
            return 1;
        }else{
            x -=1 ;
        }
    }
    if(y > end_y){
        y -= 1;
        if(x != end_x && y == end_y){
            y += 1;
        }
        else if(is_path_clear(map,x,y)){
            map->corridors[size].x_corridor = x;
            map->corridors[size].y_corrifer = y;
            map->corridors[size].is_seen = 0;
            size++;
            draw_corridor(map,x,y,end_x,end_y);
            return 1;
        }else{
            y += 1;
        }
    }
    if(y < end_y){
        y += 1;
        if(x != end_x && y == end_y){
            y -= 1;
        }
        else if(is_path_clear(map,x,y)){
            map->corridors[size].x_corridor = x;
            map->corridors[size].y_corrifer = y;
            map->corridors[size].is_seen = 0;
            size++;
            draw_corridor(map,x,y,end_x,end_y);
            return 1;
        }else{
            y -= 1;
        }
    }
    refresh();
    return 0;
}

void connect_doors(struct Map *map) {
    if(map->number_of_rooms == 6){
        draw_corridor(map,map->rooms[0].doors[0].x_door,map->rooms[0].doors[0].y_door,map->rooms[1].doors[1].x_door,map->rooms[1].doors[1].y_door);
        draw_corridor(map,map->rooms[1].doors[0].x_door,map->rooms[1].doors[0].y_door,map->rooms[4].doors[1].x_door,map->rooms[4].doors[1].y_door);
        draw_corridor(map,map->rooms[4].doors[0].x_door,map->rooms[4].doors[0].y_door,map->rooms[5].doors[0].x_door,map->rooms[5].doors[0].y_door);
        draw_corridor(map,map->rooms[5].doors[1].x_door,map->rooms[5].doors[1].y_door,map->rooms[3].doors[0].x_door,map->rooms[3].doors[0].y_door);
        draw_corridor(map,map->rooms[3].doors[1].x_door,map->rooms[3].doors[1].y_door,map->rooms[2].doors[1].x_door,map->rooms[2].doors[1].y_door);
        draw_corridor(map,map->rooms[2].doors[0].x_door,map->rooms[2].doors[0].y_door,map->rooms[0].doors[1].x_door,map->rooms[0].doors[1].y_door);
    }
    if(map->number_of_rooms == 7){
        draw_corridor(map,map->rooms[0].doors[0].x_door,map->rooms[0].doors[0].y_door,map->rooms[1].doors[1].x_door,map->rooms[1].doors[1].y_door);
        draw_corridor(map,map->rooms[1].doors[0].x_door,map->rooms[1].doors[0].y_door,map->rooms[4].doors[1].x_door,map->rooms[4].doors[1].y_door);
        draw_corridor(map,map->rooms[4].doors[0].x_door,map->rooms[4].doors[0].y_door,map->rooms[5].doors[0].x_door,map->rooms[5].doors[0].y_door);
        draw_corridor(map,map->rooms[5].doors[1].x_door,map->rooms[5].doors[1].y_door,map->rooms[6].doors[0].x_door,map->rooms[6].doors[0].y_door);
        draw_corridor(map,map->rooms[6].doors[1].x_door,map->rooms[6].doors[1].y_door,map->rooms[3].doors[0].x_door,map->rooms[3].doors[0].y_door);
        draw_corridor(map,map->rooms[3].doors[1].x_door,map->rooms[3].doors[1].y_door,map->rooms[2].doors[1].x_door,map->rooms[2].doors[1].y_door);
        draw_corridor(map,map->rooms[2].doors[0].x_door,map->rooms[2].doors[0].y_door,map->rooms[0].doors[1].x_door,map->rooms[0].doors[1].y_door);
    }
    if(map->number_of_rooms == 8){
        draw_corridor(map,map->rooms[0].doors[0].x_door,map->rooms[0].doors[0].y_door,map->rooms[1].doors[1].x_door,map->rooms[1].doors[1].y_door);
        draw_corridor(map,map->rooms[1].doors[0].x_door,map->rooms[1].doors[0].y_door,map->rooms[4].doors[1].x_door,map->rooms[4].doors[1].y_door);
        draw_corridor(map,map->rooms[4].doors[0].x_door,map->rooms[4].doors[0].y_door,map->rooms[5].doors[0].x_door,map->rooms[5].doors[0].y_door);
        draw_corridor(map,map->rooms[5].doors[1].x_door,map->rooms[5].doors[1].y_door,map->rooms[7].doors[0].x_door,map->rooms[7].doors[0].y_door);
        draw_corridor(map,map->rooms[7].doors[1].x_door,map->rooms[7].doors[1].y_door,map->rooms[6].doors[0].x_door,map->rooms[6].doors[0].y_door);
        draw_corridor(map,map->rooms[6].doors[1].x_door,map->rooms[6].doors[1].y_door,map->rooms[3].doors[0].x_door,map->rooms[3].doors[0].y_door);
        draw_corridor(map,map->rooms[3].doors[1].x_door,map->rooms[3].doors[1].y_door,map->rooms[2].doors[1].x_door,map->rooms[2].doors[1].y_door);
        draw_corridor(map,map->rooms[2].doors[0].x_door,map->rooms[2].doors[0].y_door,map->rooms[0].doors[1].x_door,map->rooms[0].doors[1].y_door);
    }
}

int deciphering(){
    int key = rand() % (9999 - 1000 + 1) + 1000;
    return key;
}

int is_pillor(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->pillors[i].x_pillar == x && map->pillors[i].y_pillar == y){
            return 1;
        }
    }
    return 0;
}

int is_trap(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->traps[i].x_trap == x && map->traps[i].y_trap == y){
            return 1;
        }
    }
    return 0;
}

int is_gold(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->golds[i].x_gold == x && map->golds[i].y_gold == y){
            return 1;
        }
    }
    return 0;
}

int is_spell(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->spells[i].x_spell == x && map->spells[i].y_spell == y){
            return 1;
        }
    }
    return 0;
}

int is_wepon(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->wepons[i].x_wepon == x && map->wepons[i].y_wepon == y){
            return 1;
        }
    }
    return 0;
}

int is_food(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->foods[i].x_food == x && map->foods[i].y_food == y){
            return 1;
        }
    }
    return 0;
}

void move_player(struct Map *map,struct Player *p,int number_map){
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    if(game_over){
        clear();
        refresh();
        return;
    }
    if(p->win_last_game == 1){
        clear();
        refresh();
        return;
    }
    if(p->win_last_game == 0){
        clear();
        refresh();
        return;
    }
    if(p->win_last_game == -1){
        clear();
        refresh();
        return;
    }
    raw(); 
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    if(number_map == 1){
        if(p->x_map1 == 0 && p->y_map1 == 0){
            int start_room = rand() % map->number_of_rooms;
            while(start_room == map->number_room_stair){
                start_room = rand() % map->number_of_rooms;
            }
            p->x = 0;
            p->y = 0;
            p->last_map = number_map;
            move(0,0);
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            while(p->x == 0 || p->y == 0 || (p->x == map->x_stair && p->y == map->y_stair) || (p->x == map->x_create_paasword && p->y == map->y_create_paasword) || (p->x == map->x_fight_room && p->y == map->y_fight_room) || (p->x == map->x_Master_Key && p->y == map->y_Master_Key) || is_pillor(map,p->x,p->y) || is_trap(map,p->x,p->y) || is_food(map,p->x,p->y) || is_spell(map,p->x,p->y) || is_wepon(map,p->x,p->y) || is_gold(map,p->x,p->y)){
                p->x = rand() % ((map->rooms[start_room].x + map->rooms[start_room].size - 1) - (map->rooms[start_room].x + 1) + 1) + ((map->rooms[start_room].x + 1));
                p->y = rand() % ((map->rooms[start_room].y + map->rooms[start_room].size - 1) - (map->rooms[start_room].y + 1) + 1) + ((map->rooms[start_room].y + 1));
            }
        }else{
            p->x = p->x_map1;
            p->y = p->y_map1;
        }
    }
    else if(number_map == 2){
        if(p->x_map2 == 0 && p->y_map2 == 0){
            int start_room = rand() % map->number_of_rooms;
            while(start_room == map->number_room_stair){
                start_room = rand() % map->number_of_rooms;
            }
            p->x = 0;
            p->y = 0;
            p->last_map = number_map;
            move(0,0);
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            while(p->x == 0 || p->y == 0 || (p->x == map->x_stair && p->y == map->y_stair) || (p->x == map->x_create_paasword && p->y == map->y_create_paasword) || (p->x == map->x_fight_room && p->y == map->y_fight_room) || (p->x == map->x_Master_Key && p->y == map->y_Master_Key) || is_pillor(map,p->x,p->y) || is_trap(map,p->x,p->y) || is_food(map,p->x,p->y) || is_spell(map,p->x,p->y) || is_wepon(map,p->x,p->y) || is_gold(map,p->x,p->y)){
                p->x = rand() % ((map->rooms[start_room].x + map->rooms[start_room].size - 1) - (map->rooms[start_room].x + 1) + 1) + ((map->rooms[start_room].x + 1));
                p->y = rand() % ((map->rooms[start_room].y + map->rooms[start_room].size - 1) - (map->rooms[start_room].y + 1) + 1) + ((map->rooms[start_room].y + 1));
            }
        }else{
            p->x = p->x_map2;
            p->y = p->y_map2;
        }
    }
    else if(number_map == 3){
        if(p->x_map3 == 0 && p->y_map3 == 0){
            int start_room = rand() % map->number_of_rooms;
            while(start_room == map->number_room_stair){
                start_room = rand() % map->number_of_rooms;
            }
            p->x = 0;
            p->y = 0;
            p->last_map = number_map;
            move(0,0);
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            while(p->x == 0 || p->y == 0 || (p->x == map->x_stair && p->y == map->y_stair) || (p->x == map->x_create_paasword && p->y == map->y_create_paasword) || (p->x == map->x_fight_room && p->y == map->y_fight_room) || (p->x == map->x_Master_Key && p->y == map->y_Master_Key) || is_pillor(map,p->x,p->y) || is_trap(map,p->x,p->y) || is_food(map,p->x,p->y) || is_spell(map,p->x,p->y) || is_wepon(map,p->x,p->y) || is_gold(map,p->x,p->y)){
                p->x = rand() % ((map->rooms[start_room].x + map->rooms[start_room].size - 1) - (map->rooms[start_room].x + 1) + 1) + ((map->rooms[start_room].x + 1));
                p->y = rand() % ((map->rooms[start_room].y + map->rooms[start_room].size - 1) - (map->rooms[start_room].y + 1) + 1) + ((map->rooms[start_room].y + 1));
            }
        }else{
            p->x = p->x_map3;
            p->y = p->y_map3;
        }
    }
    else if(number_map == 4){
        if(p->x_map4 == 0 && p->y_map4 == 0){
            int start_room = rand() % map->number_of_rooms;
            while(start_room == map->number_room_stair){
                start_room = rand() % map->number_of_rooms;
            }
            p->x = 0;
            p->y = 0;
            p->last_map = number_map;
            move(0,0);
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            while(p->x == 0 || p->y == 0 || (p->x == map->x_stair && p->y == map->y_stair) || (p->x == map->x_create_paasword && p->y == map->y_create_paasword) || (p->x == map->x_fight_room && p->y == map->y_fight_room) || (p->x == map->x_Master_Key && p->y == map->y_Master_Key) || is_pillor(map,p->x,p->y) || is_trap(map,p->x,p->y) || is_food(map,p->x,p->y) || is_spell(map,p->x,p->y) || is_wepon(map,p->x,p->y) || is_gold(map,p->x,p->y)){
                p->x = rand() % ((map->rooms[start_room].x + map->rooms[start_room].size - 1) - (map->rooms[start_room].x + 1) + 1) + ((map->rooms[start_room].x + 1));
                p->y = rand() % ((map->rooms[start_room].y + map->rooms[start_room].size - 1) - (map->rooms[start_room].y + 1) + 1) + ((map->rooms[start_room].y + 1));
            }
        }else{
            p->x = p->x_map4;
            p->y = p->y_map4;
        }
    }else if(number_map == 5){
        if(p->x_map5 == 0 && p->y_map5 == 0){
            int start_room = rand() % map->number_of_rooms;
            p->x = 0;
            p->y = 0;
            p->last_map = number_map;
            move(0,0);
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            while(p->x == 0 || p->y == 0|| is_pillor(map,p->x,p->y) || is_trap(map,p->x,p->y) || is_gold(map,p->x,p->y)){
                p->x = rand() % ((map->rooms[start_room].x + map->rooms[start_room].size - 1) - (map->rooms[start_room].x + 1) + 1) + ((map->rooms[start_room].x + 1));
                p->y = rand() % ((map->rooms[start_room].y + map->rooms[start_room].size - 1) - (map->rooms[start_room].y + 1) + 1) + ((map->rooms[start_room].y + 1));
            }
        }else{
            p->x = p->x_map5;
            p->y = p->y_map5;
        }
    }
    move(0,0);
    printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
    if(p->color == 0){
        mvprintw(p->y,p->x,"P");
    }
    else if(p->color == 1){
        attron(COLOR_PAIR(1));
        mvprintw(p->y,p->x,"P");
        attroff(COLOR_PAIR(1));
    }
    else if(p->color == 2){
        attron(COLOR_PAIR(2));
        mvprintw(p->y,p->x,"P");
        attroff(COLOR_PAIR(2));
    }
    refresh();
    print(map,p->x,p->y,p);
    time_t time_start;
    while (1){
        if(number_map == 5 && is_win(map)){
            game_over = 1;
            p->win_last_game = 1;
            clear();
            refresh();
            return;
        }
        if(number_map == 4 && p->x == map->x_end && p->y == map->y_end){
            clear();
            refresh();
            p->last_map = 5;
            p->step = 0;
            move_player(&game.maps[4],p,5);
            return;
        }
        if(number_map != 5 && is_spell_room(map,p->x,p->y)){
            p->health -= 1;
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        if(p->total_step >= 10){
            coefficient_health = 1;
            coefficient_damage = 1;
            coefficient_speed = 1;
            p->total_step = 0;
        }
        if(p->total_step_food >= 20){
            coefficient_damage = 1;
            coefficient_speed = 1;
            p->total_step_food = 0;
        }
        if(p->health <= 0){
            p->win_last_game = 0;
            game_over = 1;
            return;
        }
        time_t time_end;
        time(&time_end);
        time(&end);
        if(time_end - p->priod >= 60){
            if(p->hungry >= 1){
                p->hungry -= 1;
                time(&p->priod);
                move(0,0);
                clrtoeol();
                printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
                refresh();
            }
            time(&p->priod);
        }
        if(end - damage >= 10 && p->hungry >= 5 && first_damage){
            p->health += 3 * coefficient_health;
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        if(time_end - time_start >= 10){
            move(0,60);
            clrtoeol();
        }
        if(p->hungry <= 2){
            p->health -= 1;
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        int c = getch();
        if(c == 'w'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,0,-1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'x'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,0,1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'a'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,-1,0,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'd'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,1,0,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'e'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,1,-1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'q'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,-1,-1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'c'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,1,1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'z'){
            p->total_step += 1;
            p->total_step_food += 1;
            regular_move(p,map,-1,1,time_start);
            move_demon(map,p);
            print(map,p->x,p->y,p);
            if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
            refresh();
        }
        if(c == 'f'){
            p->total_step += 1;
            p->total_step_food += 1;
            int c2 = getch();
            if(c2 == 'w'){
                f_move(p,map,0,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'x'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,0,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'd'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,1,0,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'a'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,-1,0,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'e'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,1,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'q'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,-1,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'c'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,1,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'z'){
                p->total_step += 1;
                p->total_step_food += 1;
                f_move(p,map,-1,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
        }
        if(c == 'g'){
            int c2 = getch();
            if(c2 == 'w'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,0,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'x'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,0,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'a'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,-1,0,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'd'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,1,0,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'e'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,1,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'q'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,-1,-1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'c'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,1,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
            if(c2 == 'z'){
                p->total_step += 1;
                p->total_step_food += 1;
                g_move(p,map,-1,1,time_start);
                move_demon(map,p);
                print(map,p->x,p->y,p);
                if(p->color == 0){
                mvprintw(p->y,p->x,"P");
            }
            else if(p->color == 1){
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(1));
            }
            else if(p->color == 2){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"P");
                attroff(COLOR_PAIR(2));
            }
                refresh();
            }
        }
        if(number_map != 5 && c == 's'){
            int index_room;
            index_room = is_room_and_index(map,p->x,p->y);
            if(index_room){
                if(index_room - 1 == 0){
                    for(int i = 0; i < 3; i++){
                        if(map->traps[i].number_room == 1 || map->traps[i].number_room == 2){
                            mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                            refresh();
                        }
                    }
                    if(map->number_room_fight_room == 1 || map->number_room_fight_room == 2){
                        mvprintw(map->y_fight_room,map->y_fight_room,"F");
                    }
                    if(map->number_Spell_room == 1 || map->number_Spell_room == 2){
                        mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                        mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                        refresh();
                    }
                }
                if(index_room - 1 == 1){
                    for(int i = 0; i < 3; i++){
                        if(map->traps[i].number_room == 0 || map->traps[i].number_room == 4){
                            mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                            refresh();
                        }
                    }
                    if(map->number_room_fight_room == 0 || map->number_room_fight_room == 4){
                        mvprintw(map->y_fight_room,map->y_fight_room,"F");
                    }
                    if(map->number_Spell_room == 0 || map->number_Spell_room == 4){
                        mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                        mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                        refresh();
                    }
                }
                if(index_room - 1 == 2){
                    for(int i = 0; i < 3; i++){
                        if(map->traps[i].number_room == 0 || map->traps[i].number_room == 3){
                            mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                            refresh();
                        }
                    }
                    if(map->number_room_fight_room == 0 || map->number_room_fight_room == 3){
                        mvprintw(map->y_fight_room,map->y_fight_room,"F");
                    }
                    if(map->number_Spell_room == 0 || map->number_Spell_room == 3){
                        mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                        mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                        refresh();
                    }
                }
                if(index_room - 1 == 3){
                    if(map->number_of_rooms >= 7){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 2 || map->traps[i].number_room == 6){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 2 || map->number_room_fight_room == 6){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 2 || map->number_Spell_room == 6){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }else{
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 2 || map->traps[i].number_room == 5){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 2 || map->number_room_fight_room == 5){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 2 || map->number_Spell_room == 5){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                }
                if(index_room - 1 == 4){
                    for(int i = 0; i < 3; i++){
                        if(map->traps[i].number_room == 1 || map->traps[i].number_room == 5){
                            mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                            refresh();
                        }
                    }
                    if(map->number_room_fight_room == 1 || map->number_room_fight_room == 5){
                        mvprintw(map->y_fight_room,map->y_fight_room,"F");
                    }
                    if(map->number_Spell_room == 1 || map->number_Spell_room == 5){
                        mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                        mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                        refresh();
                    }
                }
                if(index_room - 1 == 5){
                    if(map->number_of_rooms == 6){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 4 || map->traps[i].number_room == 3){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 4 || map->number_room_fight_room == 3){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 4 || map->number_Spell_room == 3){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                    if(map->number_of_rooms == 7){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 4 || map->traps[i].number_room == 6){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 4 || map->number_room_fight_room == 6){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 4 || map->number_Spell_room == 6){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                    if(map->number_of_rooms == 8){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 4 || map->traps[i].number_room == 7){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 4 || map->number_room_fight_room == 7){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 4 || map->number_Spell_room == 7){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                }
                if(index_room - 1 == 6){
                    if(map->number_of_rooms == 7){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 3 || map->traps[i].number_room == 5){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 3 || map->number_room_fight_room == 5){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 3 || map->number_Spell_room == 5){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                    if(map->number_of_rooms == 8){
                        for(int i = 0; i < 3; i++){
                            if(map->traps[i].number_room == 3 || map->traps[i].number_room == 7){
                                mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                                refresh();
                            }
                        }
                        if(map->number_room_fight_room == 3 || map->number_room_fight_room == 7){
                            mvprintw(map->y_fight_room,map->y_fight_room,"F");
                        }
                        if(map->number_Spell_room == 3 || map->number_Spell_room == 7){
                            mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                            mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                            refresh();
                        }
                    }
                }
                if(index_room - 1 == 7){
                    for(int i = 0; i < 3; i++){
                        if(map->traps[i].number_room == 5 || map->traps[i].number_room == 6){
                            mvprintw(map->traps[i].y_trap,map->traps[i].x_trap,"^");
                            refresh();
                        }
                    }
                    if(map->number_room_fight_room == 5 || map->number_room_fight_room == 6){
                        mvprintw(map->y_fight_room,map->y_fight_room,"F");
                    }
                    if(map->number_Spell_room == 5 || map->number_Spell_room == 6){
                        mvprintw(map->rooms[map->number_Spell_room].doors[0].y_door,map->rooms[map->number_Spell_room].doors[0].x_door,"?");
                        mvprintw(map->rooms[map->number_Spell_room].doors[1].y_door,map->rooms[map->number_Spell_room].doors[1].x_door,"?");
                        refresh();
                    }
                }
            }
        }
        if(number_map != 5 && c == KEY_RIGHT){
            if(is_stair(map,p->x,p->y)){
                p->step = 0;
                if(p->last_map == 1){
                    clear();
                    p->last_map = 2;
                    p->x_map1 = p->x;
                    p->y_map1 = p->y;
                    move_player(&game.maps[1],p,2);
                    return;
                }
                if(p->last_map == 2){
                    clear();
                    p->last_map = 3;
                    p->x_map2 = p->x;
                    p->y_map2 = p->y;
                    move_player(&game.maps[2],p,3);
                    return;
                }
                if(p->last_map == 3){
                    clear();
                    p->last_map = 4;
                    p->x_map3 = p->x;
                    p->y_map3 = p->y;
                    move_player(&game.maps[3],p,4);
                    return;
                }
            }
        }
        if(number_map != 5 && c == KEY_LEFT){
            if(is_stair(map,p->x,p->y)){
                p->step = 0;
                if(p->last_map == 2){
                    clear();
                    p->last_map = 1;
                    p->x_map2 = p->x;
                    p->y_map2 = p->y;
                    move_player(&game.maps[0],p,1);
                    return;
                }
                if(p->last_map == 3){
                    clear();
                    p->last_map = 2;
                    p->x_map3 = p->x;
                    p->y_map3 = p->y;
                    move_player(&game.maps[1],p,2);
                    return;
                }
                if(p->last_map == 4){
                    clear();
                    p->last_map = 3;
                    p->x_map4 = p->x;
                    p->y_map4 = p->y;
                    move_player(&game.maps[2],p,3);
                    return;
                }
            }
        }
        if(c == 'E'){
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            if(p->last_map == 5){
                p->x_map5 = p->x;
                p->y_map5 = p->y;
            }
            clear();
            refresh();
            show_foods(p);
            move_player(map,p,p->last_map);
            return;
        }
        if(c == 'i'){
            if(game_over){
                clear();
                refresh();
                return;
            }
            if(p->win_last_game == 1){
                clear();
                refresh();
                return;
            }
            if(p->win_last_game == 0){
                clear();
                refresh();
                return;
            }
            if(p->win_last_game == -1){
                clear();
                refresh();
                return;
            }
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            if(p->last_map == 5){
                p->x_map5 = p->x;
                p->y_map5 = p->y;
            }
            clear();
            refresh();
            show_wepons(p);
            move_player(map,p,p->last_map);
            return;
        }
        if(number_map != 5 && c == 'm'){
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            clear();
            refresh();
            show_map(map);
            move_player(map,p,p->last_map);
            return;
        }
        if(c == 't'){
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            if(p->last_map == 5){
                p->x_map5 = p->x;
                p->y_map5 = p->y;
            }
            clear();
            refresh();
            show_spells(p);
            move_player(map,p,p->last_map);
            return;
        }
        if(c == 'r'){
            for(int j = 0; j < p->spell_number; j++){
                if(p->spells[j].type == 1 && p->spells[j].is_exist){
                    coefficient_health = 2;
                    p->spells[j].is_exist = 0;
                    p->total_step = 0;
                    break;
                }
            }
        }
        if(c == 'p'){
            for(int j = 0; j < p->spell_number; j++){
                if(p->spells[j].type == 3 && p->spells[j].is_exist){
                    coefficient_damage = 2;
                    p->spells[j].is_exist = 0;
                    p->total_step = 0;
                    break;
                }
            }
        }
        if(c == 'y'){
            for(int j = 0; j < p->spell_number; j++){
                if(p->spells[j].type == 2 && p->spells[j].is_exist){
                    coefficient_speed = 2;
                    p->spells[j].is_exist = 0;
                    p->total_step = 0;
                    break;
                }
            }
        }
        if(c == 'o'){
            for(int j = p->food_number - 1; j >= 0; j--){
                if(p->foods[j].type == 1){
                    p->hungry += 1;
                    p->health += 200;
                    p->foods[j].type = 0;
                    p->food_number -= 1;
                    move(0,0);
                    clrtoeol();
                    printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
                    break;
                }
                else if(p->foods[j].type == 4){
                    p->health -= 200;
                    p->foods[j].type = 0;
                    p->food_number -= 1;
                    move(1,0);
                    printw("it was a poison food!");
                    refresh();
                    sleep(1);
                    move(1,0);
                    clrtoeol();
                    move(0,0);
                    clrtoeol();
                    printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
                    break;
                }
            }
        }
        if(c == 'u'){
            for(int j = p->food_number - 1; j >= 0; j--){
                if(p->foods[j].type == 2){
                    p->hungry += 1;
                    p->health += 200;
                    p->foods[j].type = 0;
                    p->food_number -= 1;
                    coefficient_damage = 2;
                    p->total_step_food = 0;
                    move(0,0);
                    clrtoeol();
                    printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
                    break;
                }
            }
        }
        if(c == 'b'){
            for(int j = p->food_number - 1; j >= 0; j--){
                if(p->foods[j].type == 3){
                    p->hungry += 1;
                    p->health += 200;
                    p->foods[j].type = 0;
                    p->food_number -= 1;
                    coefficient_speed = 2;
                    p->total_step_food = 0;
                    move(0,0);
                    clrtoeol();
                    printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
                    break;
                }
            }
        }
        if(c == KEY_F(3)){
            clear();
            refresh();
            p->win_last_game = -1;
            game_over = 1;
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            if(p->last_map == 5){
                p->x_map5 = p->x;
                p->y_map5 = p->y;
            }
            return;
        }
        if(c == 'W'){
            if(p->type_wepon_chosen == 0){
                move(1,0);
                printw("You are not carry a wepon!");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                p->type_wepon_chosen = 0;
                move(1,0);
                printw("Your weapon is placed in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }
        }
        if(c == 'l'){
            if(p->type_wepon_chosen != 0){
                move(1,0);
                printw("First, put the previous weapon in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                p->type_wepon_chosen = 1;
                move(1,0);
                printw("Weapon changed to mace");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }
        }
        if(c == 'k'){
            if(p->type_wepon_chosen != 0){
                move(1,0);
                printw("First, put the previous weapon in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                int found = 0;
                for(int j = 0; j < p->wepon_number; j++){
                    if(p->wepons[j].type == 2 && p->wepons[j].is_exist){
                        found = 1;
                        p->type_wepon_chosen = 2;
                        move(1,0);
                        printw("Weapon changed to dagger");
                        refresh();
                        sleep(1);
                        move(1,0);
                        clrtoeol();
                        break;
                    }
                }
                if(!found){
                   move(1,0);
                    printw("You have used up all your daggers.");
                    refresh();
                    sleep(1);
                    move(1,0);
                    clrtoeol();
                    refresh(); 
                }
            }
        }
        if(c == 'v'){
            if(p->type_wepon_chosen != 0){
                move(1,0);
                printw("First, put the previous weapon in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                int found = 0;
                for(int j = 0; j < p->wepon_number; j++){
                    if(p->wepons[j].type == 3 && p->wepons[j].is_exist){
                        found = 1;
                        p->type_wepon_chosen = 3;
                        move(1,0);
                        printw("Weapon changed to magic wand");
                        refresh();
                        sleep(1);
                        move(1,0);
                        clrtoeol();
                        refresh();
                        break;
                    }
                }
                if(!found){
                   move(1,0);
                    printw("You have used up all your magic wand.");
                    refresh();
                    sleep(1);
                    move(1,0);
                    clrtoeol();
                    refresh(); 
                }
            }
        }
        if(c == 'n'){
            if(p->type_wepon_chosen != 0){
                move(1,0);
                printw("First, put the previous weapon in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                int found = 0;
                for(int j = 0; j < p->wepon_number; j++){
                    if(p->wepons[j].type == 4 && p->wepons[j].is_exist){
                        found = 1;
                        p->type_wepon_chosen = 4;
                        move(1,0);
                        printw("Weapon changed to normal arrow");
                        refresh();
                        sleep(1);
                        move(1,0);
                        clrtoeol();
                        refresh();
                        break;
                    }
                }
                if(!found){
                   move(1,0);
                    printw("You have used up all your normal arrow.");
                    refresh();
                    sleep(1);
                    move(1,0);
                    clrtoeol();
                    refresh(); 
                }
            }
        }
        if(c == 'h'){
            if(p->type_wepon_chosen != 0){
                move(1,0);
                printw("First, put the previous weapon in the backpack.");
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
            }else{
                int found = 0;
                for(int j = 0; j < p->wepon_number; j++){
                    if(p->wepons[j].type == 5){
                        found = 1;
                        p->type_wepon_chosen = 5;
                        move(1,0);
                        printw("Weapon changed to sword");
                        refresh();
                        sleep(1);
                        move(1,0);
                        clrtoeol();
                        refresh();
                        break;
                    }
                }
                if(!found){
                    move(1,0);
                    printw("You have used up all your sword.");
                    refresh();
                    sleep(1);
                    move(1,0);
                    clrtoeol();
                    refresh(); 
                }
            }
        }
        if(c == 32){
            int dam;
            if(p->type_wepon_chosen == 1){
                for(int j = 0; j < 5; j++){
                    if(p->x + 1 == map->demons[j].x_demon && p->y == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x + 1 == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x + 1 == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 5 * coefficient_damage;
                        map->demons[j].healt -= 5 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                }
                move_demon(map,p);
                print(map,p->x,p->y,p);
            }
            if(p->type_wepon_chosen == 5){
                for(int j = 0; j < 5; j++){
                    if(p->x + 1 == map->demons[j].x_demon && p->y == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x + 1 == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x + 1 == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y - 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                    if(p->x - 1 == map->demons[j].x_demon && p->y + 1 == map->demons[j].y_demon){
                        dam = 10 * coefficient_damage;
                        map->demons[j].healt -= 10 * coefficient_damage;
                        if(map->demons[j].healt <= 0){
                            move(1,0);
                            printw("You kill the demon!");
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                            print_after_move_demon(map,map->demons[j].x_demon,map->demons[j].y_demon);
                            map->demons[j].x_demon = 0;
                            map->demons[j].y_demon = 0;
                        }else{
                            move(1,0);
                            printw("You damage the demon %d",dam);
                            refresh();
                            sleep(1);
                            move(1,0);
                            clrtoeol();
                            refresh();
                        }
                        continue;
                    }
                }
                move_demon(map,p);
                print(map,p->x,p->y,p);
            }
            if(p->type_wepon_chosen == 2){
                throw_wepon(2,map,p,0);
                move_demon(map,p);
                print(map,p->x,p->y,p);
            }
            if(p->type_wepon_chosen == 3){
                throw_wepon(3,map,p,0);
                move_demon(map,p);
                print(map,p->x,p->y,p);
            }
            if(p->type_wepon_chosen == 4){
                throw_wepon(4,map,p,0);
                move_demon(map,p);
                print(map,p->x,p->y,p);
            }
        }
        if(c == 'A'){
            if(last_throw != 0){
                if(p->type_wepon_chosen == 2){
                    throw_wepon(2,map,p,1);
                    move_demon(map,p);
                    print(map,p->x,p->y,p);
                }
                if(p->type_wepon_chosen == 3){
                    throw_wepon(3,map,p,1);
                    move_demon(map,p);
                    print(map,p->x,p->y,p);
                }
                if(p->type_wepon_chosen == 4){
                    throw_wepon(4,map,p,1);
                    move_demon(map,p);
                    print(map,p->x,p->y,p);
                }
            }
        }
    }
}

int is_not_wall(struct Map *map, int x, int y) {
    for(int i = 0; i < map->number_of_rooms; i++) {
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x > room_x1 && x < room_x2 && y > room_y1 && y < room_y2) {
            return 1;
        }
    }
    return 0;
}

int is_door(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_rooms; i++){
        if((x == map->rooms[i].doors[0].x_door && y == map->rooms[i].doors[0].y_door) || (x == map->rooms[i].doors[1].x_door && y == map->rooms[i].doors[1].y_door)){
            return 1;
        }
    }
    return 0;
}

int is_corridor(struct Map *map,int x, int y){
    for(int i = 0; i < map->corridor_count; i++){
        if(x == map->corridors[i].x_corridor && y == map->corridors[i].y_corrifer){
            return 1;
        }
    }
    return 0;
}

void move_effect(const char* state,struct Player *p,struct Map* map,int x,int y){
    if(!strcmp(state,"food")){
        p->foods[p->food_number].type = map->foods[index_food(map,x,y)].type;
        f_food(map,x,y);
        move(1,0);
        printw("You got food!");
        refresh();
        sleep(1);
        move(1,0);
        clrtoeol();
        move(0,0);
        clrtoeol();
        p->food_number += 1;
        printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
        refresh();
    }
    else if(!strcmp(state,"trap")){
        move(1,0);
        printw("This was a trap!");
        refresh();
        sleep(1);
        move(1,0);
        clrtoeol();
        move(0,0);
        clrtoeol();
        p->health -= 100;
        printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
        refresh();
    }
    else if(!strcmp(state,"spell")){
        p->spells[p->spell_number].type = map->spells[index_spell(map,x,y)].type;
        p->spells[p->spell_number].is_exist = 1;
        f_spell(map,x,y);
        move(1,0);
        printw("You got a spell!");
        refresh();
        sleep(1);
        move(1,0);
        clrtoeol();
        p->spell_number += 1;
        refresh();
    }
    else if(!strcmp(state,"wepon")){
        p->wepons[p->wepon_number].type = map->wepons[index_wepon(map,x,y)].type;
        p->wepons[p->wepon_number].is_exist = 1;
        if(p->wepons[p->wepon_number].type == 2){
            p->wepons[p->wepon_number].cllect = 10;
        }
        if(p->wepons[p->wepon_number].type == 3){
            p->wepons[p->wepon_number].cllect = 8;
        }
        if(p->wepons[p->wepon_number].type == 4){
            p->wepons[p->wepon_number].cllect = 20;
        }
        f_wepon(map,x,y);
        move(1,0);
        printw("You got a wepon");
        refresh();
        sleep(1);
        move(1,0);
        clrtoeol();
        p->wepon_number += 1;
        refresh();
    }
    else if(!strcmp(state,"wepon_use")){
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == map->wepons_use[index_wepon_use(map,x,y)].type){
                p->wepons[i].cllect += 1;
                break;
            }
        }
        f_wepon_use(map,x,y);
    }
    if(!strcmp(state,"gold")){
        move(1,0);
        if(map->golds[index_gold(map,x,y)].is_black){
            printw("You got black gold!");
            refresh();
        }else{
            printw("You got regular gold!");
            refresh();
        }
        sleep(1);
        move(1,0);
        clrtoeol();
        p->gold += map->golds[index_gold(map,x,y)].worth;
        f_gold(map,x,y);
        move(0,0);
        printw("Helth: %d  Hunger: %d Gold: %d",p->health,p->hungry,p->gold);
        refresh();
    }
    if(!strcmp(state,"master")){
        move(1,0);
        printw("You got Master Key!");
        refresh();
        sleep(1);
        move(1,0);
        clrtoeol();
        p->have_master_key = 1;
        f_master_key(map,x,y);
    }
    if(!strcmp(state,"fight")){
        move(1,0);
        printw("You will enter to fight room");
        refresh();
        sleep(2);
        move(1,0);
        clrtoeol();
        f_fight(map,x,y);
        clear();
        refresh();
        fightroom(p);
        move_player(map,p,p->last_map);
        return;
    }
}

void f_gold(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->golds[i].x_gold == x && map->golds[i].y_gold == y){
            map->golds[i].x_gold = 0;
            map->golds[i].y_gold = 0;
            break;
        }
    }
}

void f_spell(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->spells[i].x_spell == x && map->spells[i].y_spell == y){
            map->spells[i].x_spell = 0;
            map->spells[i].y_spell = 0;
            break;
        }
    }
}

void f_wepon(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->wepons[i].x_wepon == x && map->wepons[i].y_wepon == y){
            map->wepons[i].x_wepon = 0;
            map->wepons[i].y_wepon = 0;
            break;
        }
    }
}

void f_food(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->foods[i].x_food == x && map->foods[i].y_food == y){
            map->foods[i].x_food = 0;
            map->foods[i].y_food = 0;
            break;
        }
    }
}

int index_gold(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->golds[i].x_gold == x && map->golds[i].y_gold == y){
            return i;
        }
    }
}

int index_spell(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->spells[i].x_spell == x && map->spells[i].y_spell == y){
            return i;
        }
    }
}

int index_wepon(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->wepons[i].x_wepon == x && map->wepons[i].y_wepon == y){
            return i;
        }
    }
}

int index_food(struct Map *map,int x, int y){
    for(int i = 0; i < 3; i++){
        if(map->foods[i].x_food == x && map->foods[i].y_food == y){
            return i;
        }
    }
}

int is_master_key(struct Map *map,int x, int y){
    if(map->x_Master_Key == x && map->y_Master_Key == y){
        return 1;
    }
    return 0;
}

void f_master_key(struct Map *map,int x, int y){
    map->x_Master_Key = 0;
    map->y_Master_Key = 0;
    return;
}

void f_fight(struct Map *map,int x,int y){
    map->x_fight_room = 0;
    map->y_fight_room = 0;
    return;
}

int is_password_door(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_rooms; i++){
        if(i == map->number_Password_Doors_room){
            if(x == map->rooms[i].doors[0].x_door && y == map->rooms[i].doors[0].y_door){
                return 1;
            }
        }
    }
    return 0;
}

int is_fight_room(struct Map *map,int x, int y){
     if(map->x_fight_room == x && map->y_fight_room == y){
        return 1;
    }
    return 0;
}

int is_create_password(struct Map *map,int x,int y){
    if(map->x_create_paasword == x && map->y_create_paasword == y){
        return 1;
    }
    return 0;
}

int panel(struct Player* p, struct Map *map) {
    clear();
    refresh();
    const char *choices[3] = {
        "Use Master Key",
        "Enter the password",
        "Exit"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    WINDOW *menu = newwin(5,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 3 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				nocbreak();
    			echo();
    			curs_set(1);
				if(current_item == 0){
					if (p->have_master_key) {
                        clear();
                        refresh();
                        p->have_master_key = 0;
                        map->x_password_door = 0;
                        map->y_password_door = 0;
                        int random = rand() % 10 + 1;//1-10
                        if(random == 1){
                            return 0;
                        }
                        return 1;
                    }else{
                        clear();
					    refresh();
                        return 0;
                    }
				}
				if(current_item == 1){
					clear();
					refresh();
                    move(7, 20);
                    printw("Enter The password");
                    refresh();
                    move(8, 20);
                    int password;
                    nocbreak();
                    echo();
                    scanw("%d", &password);
                    int random = rand() % 5 + 1;//1-5
                    if(random == 1){ // رمز معکوس
                        int temp = map->last_password;
                        int digit1 = temp % 10;
                        temp /= 10;
                        int digit2 = temp % 10;
                        temp /= 10;
                        int digit3 = temp % 10;
                        temp /= 10;
                        int digit4 = temp % 10;
                        map->last_password = digit1 * 1000 + digit2 * 100 + digit3 * 10 + digit4;
                    }
                    if(random == 2){//رمز متغیر
                        map->last_password = rand() % (9999 - 1000 + 1) + 1000;
                    }
                    for (int i = 0; i < 3; i++) {
                        if (map->last_password == password) {
                            clear();
                            refresh();
                            cbreak();
                            noecho();
                            map->x_password_door = 0;
                            map->y_password_door = 0;
                            return 1;
                        } else {
                            clear();
                            refresh();
                            if (i == 0) {
                                attron(COLOR_PAIR(3));  // Set color pair to yellow
                                mvprintw(9, 50, "Wrong password!");
                                attroff(COLOR_PAIR(3)); // Turn off yellow color
                                refresh();
                                sleep(3);
                                move(7, 20);
                                printw("Enter The again password!");
                                refresh();
                                move(8, 20);
                                scanw("%d", &password);
                            }
                            if (i == 1) {
                                attron(COLOR_PAIR(4));  // Set color pair to red
                                mvprintw(9, 50, "Wrong password!");
                                attroff(COLOR_PAIR(4)); // Turn off red color
                                refresh();
                                sleep(3);
                                move(7, 20);
                                printw("Enter The again password!");
                                refresh();
                                move(8, 20);
                                scanw("%d", &password);
                            }
                            if (i == 2) {
                                clear();
                                refresh();
                                cbreak();
                                noecho();
                                return 0;
                            }
                        }
                    }
				}
                if(current_item == 2){
                    clear();
					refresh();
                    return 0;
                }
        
        }
    }
}

void create_password(struct Map *map,time_t time_start){
    int password = deciphering();
    map->last_password = password;
    move(0,60);
    printw("The password: %d",password);
    time(&time_start);
    time(&start);
}

void regular_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start){
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    if(is_not_wall(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && !is_pillor(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && !is_demon(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
        if(map->number_of_rooms != 1 && is_food(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("food",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(is_trap(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("trap",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_spell(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("spell",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_wepon(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("wepon",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(is_wepon_use(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("wepon_use",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(is_gold(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("gold",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_master_key(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("master",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_fight_room(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            if(p->last_map == 1){
                p->x_map1 = p->x;
                p->y_map1 = p->y;
            }
            if(p->last_map == 2){
                p->x_map2 = p->x;
                p->y_map2 = p->y;
            }
            if(p->last_map == 3){
                p->x_map3 = p->x;
                p->y_map3 = p->y;
            }
            if(p->last_map == 4){
                p->x_map4 = p->x;
                p->y_map4 = p->y;
            }
            move_effect("fight",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_create_password(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && (map->x_password_door != 0 && map->y_password_door != 0)){
            create_password(map,time_start);
        }
        if(is_trap(map,p->x,p->y)){
            mvprintw(p->y,p->x,"^");
            refresh();
        }
        else if(map->number_of_rooms != 1 && is_stair(map,p->x,p->y)){
            mvprintw(p->y,p->x,"<");
            refresh();
        }
        else if(map->number_of_rooms != 1 && is_door(map,p->x,p->y) && !is_password_door(map,p->x,p->y) && !is_secret_door(map,p->x,p->y)){
            mvprintw(p->y,p->x,"+");
            refresh();
        }
        else if(map->number_of_rooms != 1 && is_secret_door(map,p->x,p->y)){
            mvprintw(p->y,p->x,"?");
            refresh();
        }
        else if(map->number_of_rooms != 1 && is_password_door(map,p->x,p->y)){
            if(map->x_password_door == 0 && map->y_password_door == 0){
                attron(COLOR_PAIR(2));
                mvprintw(p->y,p->x,"@");
                attroff(COLOR_PAIR(2));
                refresh();
            }else{
                attron(COLOR_PAIR(1));
                mvprintw(p->y,p->x,"@");
                attroff(COLOR_PAIR(1));
                refresh();
            }
        }
        else{
            mvprintw(p->y,p->x," ");
            refresh();
        }
        p->y += second_y * coefficient_speed;
        p->x += second_x * coefficient_speed;
        mvprintw(p->y,p->x,"P");
        p->step += 1;
        refresh();
    }if(map->number_of_rooms != 1 && !is_not_wall(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
        if(is_corridor(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            if(is_corridor(map,p->x,p->y)){
                mvprintw(p->y,p->x,"#");
                refresh();
                p->y += second_y * coefficient_speed;
                p->x += second_x * coefficient_speed;
                p->step = 0;
                mvprintw(p->y,p->x,"P");
                refresh();
            }
            else if(is_door(map,p->x,p->y) && !is_password_door(map,p->x,p->y) && !is_secret_door(map,p->x,p->y)){
                mvprintw(p->y,p->x,"+");
                refresh();
                p->y += second_y * coefficient_speed;
                p->x += second_x * coefficient_speed;
                p->step = 0;
                mvprintw(p->y,p->x,"P");
                refresh();
            }
            else if(is_secret_door(map,p->x,p->y)){
                mvprintw(p->y,p->x,"?");
                refresh();
                p->y += second_y * coefficient_speed;
                p->x += second_x * coefficient_speed;
                p->step = 0;
                mvprintw(p->y,p->x,"P");
                refresh();
            }
            else if(is_password_door(map,p->x,p->y)){
                if(map->x_password_door == 0 && map->y_password_door == 0){
                    attron(COLOR_PAIR(2));
                    mvprintw(p->y,p->x,"@");
                    attroff(COLOR_PAIR(2));
                    refresh();
                    p->y += second_y * coefficient_speed;
                    p->x += second_x * coefficient_speed;
                    p->step = 0;
                    mvprintw(p->y,p->x,"P");
                    refresh();
                }else{
                    attron(COLOR_PAIR(1));
                    mvprintw(p->y,p->x,"@");
                    attroff(COLOR_PAIR(1));
                    refresh();
                    p->y += second_y * coefficient_speed;
                    p->x += second_x * coefficient_speed;
                    p->step = 0;
                    mvprintw(p->y,p->x,"P");
                    refresh();
                }
            }
        }
        if(is_door(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            if(is_password_door(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && (map->x_create_paasword != 0 && map->y_password_door != 0)){
                if(p->last_map == 1){
                    p->x_map1 = p->x;
                    p->y_map1 = p->y;
                }
                if(p->last_map == 2){
                    p->x_map2 = p->x;
                    p->y_map2 = p->y;
                }
                if(p->last_map == 3){
                    p->x_map3 = p->x;
                    p->y_map3 = p->y;
                }
                if(p->last_map == 4){
                    p->x_map4 = p->x;
                    p->y_map4 = p->y;
                }
                int resualt = panel(p,map);
                move_player(map,p,p->last_map);
                return;
                if(resualt){
                    if(is_corridor(map,p->x,p->y)){
                        mvprintw(p->y,p->x,"#");
                        refresh();
                        p->y += second_y * coefficient_speed;
                        p->x += second_x * coefficient_speed;
                        p->step = 0;
                        mvprintw(p->y,p->x,"P");
                        refresh();
                    }else{
                        attron(COLOR_PAIR(2));
                        mvprintw(p->y,p->x,"@");
                        attroff(COLOR_PAIR(2));
                        refresh();
                        p->y += second_y * coefficient_speed;
                        p->x += second_x * coefficient_speed;
                        p->step = 0;
                        mvprintw(p->y,p->x,"P");
                        refresh();
                    }
                }
            }
            if(is_corridor(map,p->x,p->y)){
                mvprintw(p->y,p->x,"#");
                refresh();
                p->y += second_y * coefficient_speed;
                p->x += second_x * coefficient_speed;
                p->step = 0;
                mvprintw(p->y,p->x,"P");
                refresh();
            }
            else{
                mvprintw(p->y,p->x," ");
                refresh();
                p->y += second_y * coefficient_speed;
                p->x += second_x * coefficient_speed;
                p->step = 0;
                mvprintw(p->y,p->x,"P");
                refresh();
            }
        }
    }
}

void f_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start){
    int x_before = p->x;
    int y_before = p->y;
    while (is_not_wall(map,p->x + second_x,p->y + second_y) && !is_pillor(map,p->x + second_x,p->y + second_y) && !is_demon(map,p->x + second_x,p->y + second_y)){
        p->y += second_y;
        p->x += second_x;
        p->step += 1;
    }
    mvprintw(y_before,x_before," ");
    if(map->number_of_rooms != 1 && is_food(map,p->x,p->y)){
        move_effect("food",p,map,p->x,p->y);
    }
    if(is_trap(map,p->x,p->y)){
        move_effect("trap",p,map,p->x,p->y);
    }
    if(map->number_of_rooms != 1 && is_spell(map,p->x,p->y)){
        move_effect("spell",p,map,p->x,p->y);
    }
    if(map->number_of_rooms != 1 && is_wepon(map,p->x,p->y)){
        move_effect("wepon",p,map,p->x,p->y);
    }
    if(is_wepon_use(map,p->x,p->y)){
        move_effect("wepon_use",p,map,p->x,p->y);
    }
    if(is_gold(map,p->x,p->y)){
        move_effect("gold",p,map,p->x,p->y);
    }
    if(map->number_of_rooms != 1 && is_master_key(map,p->x,p->y)){
        move_effect("master",p,map,p->x,p->y);
    }
    if(map->number_of_rooms != 1 && is_fight_room(map,p->x,p->y)){
        if(p->last_map == 1){
            p->x_map1 = p->x;
            p->y_map1 = p->y;
        }
        if(p->last_map == 2){
            p->x_map2 = p->x;
            p->y_map2 = p->y;
        }
        if(p->last_map == 3){
            p->x_map3 = p->x;
            p->y_map3 = p->y;
        }
        if(p->last_map == 4){
            p->x_map4 = p->x;
            p->y_map4 = p->y;
        }
        move_effect("fight",p,map,p->x,p->y);
    }
    if(map->number_of_rooms != 1 && is_create_password(map,p->x,p->y)){
        create_password(map,time_start);
    }
    mvprintw(p->y,p->x,"P");
    refresh();
    while(map->number_of_rooms != 1 && is_corridor(map,p->x + second_x,p->y + second_y)){
    p->y += second_y;
    p->x += second_x;
    p->step = 0;
    }
    if(map->number_of_rooms != 1 && is_corridor(map,x_before,y_before)){
        mvprintw(y_before,x_before,"#");
        refresh();
    }
    else if(map->number_of_rooms != 1 && is_door(map,x_before,y_before) && !is_password_door(map,x_before,y_before) && !is_secret_door(map,x_before,y_before)){
        mvprintw(y_before,x_before,"+");
        refresh();
    }
    else if(map->number_of_rooms != 1 && is_secret_door(map,x_before,y_before)){
        mvprintw(y_before,x_before,"?");
        refresh();
    }
    else if(map->number_of_rooms != 1 && is_password_door(map,x_before,y_before)){
        if(map->x_password_door == 0 && map->y_password_door == 0){
            start_color();
            init_pair(1,COLOR_GREEN,COLOR_WHITE);
            attron(COLOR_PAIR(1));
            mvprintw(y_before,x_before,"@");
            attroff(COLOR_PAIR(1));
            refresh();
        }else{
            start_color();
            init_pair(1,COLOR_RED,COLOR_WHITE);
            attron(COLOR_PAIR(1));
            mvprintw(y_before,x_before,"@");
            attroff(COLOR_PAIR(1));
            refresh();
        }
    }else{

    }
    mvprintw(p->y,p->x,"P");
    refresh();
}

void g_move(struct Player *p,struct Map *map,int second_x,int second_y,time_t time_start){
    if(is_not_wall(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && !is_pillor(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed) && !is_demon(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
        if(is_trap(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("trap",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(map->number_of_rooms != 1 && is_fight_room(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("fight",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(is_gold(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            move_effect("gold",p,map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed);
        }
        if(is_trap(map,p->x,p->y)){
            mvprintw(p->y,p->x,"^");
            refresh();
        }
        else{
            mvprintw(p->y,p->x," ");
            refresh();
        }
        p->y += second_y * coefficient_speed;
        p->x += second_x * coefficient_speed;
        p->step += 1;
        mvprintw(p->y,p->x,"P");
        refresh();
    }if(map->number_of_rooms != 1 && !is_not_wall(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
        if(is_corridor(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            mvprintw(p->y,p->x,"#");
            refresh();
            p->y += second_y * coefficient_speed;
            p->x += second_x * coefficient_speed;
            p->step = 0;
            mvprintw(p->y,p->x,"P");
            refresh();
        }
        if(is_door(map,p->x + second_x * coefficient_speed,p->y + second_y * coefficient_speed)){
            mvprintw(p->y,p->x," ");
            refresh();
            p->y += second_y * coefficient_speed;
            p->x += second_x * coefficient_speed;
            p->step = 0;
            mvprintw(p->y,p->x,"P");
            refresh();
        }
    }
}

int is_secret_door(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_rooms; i++){
        if(map->rooms[i].doors[0].is_secret_door){
            if((map->rooms[i].doors[0].x_door == x && map->rooms[i].doors[0].y_door == y) || (map->rooms[i].doors[1].x_door == x && map->rooms[i].doors[1].y_door == y)){
                return 1;
            }
        }
    }
    return 0;
}

void save_map(char filename[100], struct Map *map,int number) {
    char perfix[20];
    switch (number)
    {
    case 1:
        strcpy(perfix, "-map1.bin");
        break;
    case 2:
        strcpy(perfix, "-map2.bin");
        break;
    case 3:
        strcpy(perfix, "-map3.bin");
        break;
    case 4:
        strcpy(perfix, "-map4.bin");
        break;
    case 5:
        strcpy(perfix, "-map5.bin");
        break;
    }
    strcat(filename,perfix);   
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file");
        return;
    }
    fwrite(map, sizeof(struct Map), 1, file);
    fclose(file);
}

void load_map(char filename[100], struct Map *map,int number) {
    char perfix[20];
    switch (number)
    {
    case 1:
        strcpy(perfix, "-map1.bin");
        break;
    case 2:
        strcpy(perfix, "-map2.bin");
        break;
    case 3:
        strcpy(perfix, "-map3.bin");
        break;
    case 4:
        strcpy(perfix, "-map4.bin");
        break;
    case 5:
        strcpy(perfix, "-map5.bin");
        break;
    }
    strcat(filename,perfix);   
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file");
        return;
    }
    fread(map, sizeof(struct Map), 1, file);
    fclose(file);
}

int is_room(struct Map *map, int x, int y) {
    for(int i = 0; i < map->number_of_rooms; i++) {
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x >= room_x1 && x <= room_x2 && y >= room_y1 && y <= room_y2) {
            map->rooms[i].is_seen = 1;
            return 1;
        }
    }
    return 0;
}

void print_corridor(struct Map *map){
    for(int i = 0; i < map->corridor_count; i++){
        if(map->corridors[i].is_seen){
            mvprintw(map->corridors[i].y_corrifer,map->corridors[i].x_corridor,"#");
            refresh();
        }
    }
}

void print_room(struct Map *map,struct Player *p){
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    if(map->number_of_rooms != 1){
        if(end - start < 10 && map->last_password != 0){
            move(0,60);
            printw("The password: %d",map->last_password);
        }
        for(int i = 0; i < map->number_of_rooms; i++){
            if(map->rooms[i].is_seen){
                int x_1 = map->rooms[i].x;  
                int x_2 = map->rooms[i].x + map->rooms[i].size;  
                int y_1 = map->rooms[i].y;  
                int y_3 = map->rooms[i].y + map->rooms[i].size;  
                if(i == map->number_Spell_room){
                    for (int j = 0; j <= map->rooms[i].size; j++) {  
                        attron(COLOR_PAIR(1));
                        move(y_1, x_1 + j);
                        printw("_");  
                        move(y_3, x_1 + j);  
                        printw("_");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }  
                    for (int j = 0; j < map->rooms[i].size; j++){  
                        attron(COLOR_PAIR(1));
                        move(y_1 + j + 1, x_1);  
                        printw("|");
                        move(y_1 + j + 1, x_2);  
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                }else if(map->has_treasure_room && i == map->number_Treasure_Room){
                    for (int j = 0; j <= map->rooms[i].size; j++) {  
                        attron(COLOR_PAIR(4));
                        move(y_1, x_1 + j);
                        printw("_");  
                        move(y_3, x_1 + j);  
                        printw("_");
                        attroff(COLOR_PAIR(4));
                        refresh();
                    }  
                    for (int j = 0; j < map->rooms[i].size; j++){  
                        attron(COLOR_PAIR(4));
                        move(y_1 + j + 1, x_1);  
                        printw("|");
                        move(y_1 + j + 1, x_2);  
                        printw("|");
                        attroff(COLOR_PAIR(4));
                        refresh();
                    }
                }
                else{
                    for (int j = 0; j <= map->rooms[i].size; j++) {  
                        move(y_1, x_1 + j);
                        printw("_");  
                        move(y_3, x_1 + j);  
                        printw("_");  
                    }  
                    for (int j = 0; j < map->rooms[i].size; j++){  
                        move(y_1 + j + 1, x_1);  
                        printw("|");
                        move(y_1 + j + 1, x_2);  
                        printw("|");  
                    }
                }
                for(int j = 0; j < 2; j++){
                    mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"+");
                    mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
                }
                if(i == map->number_Password_Doors_room){
                    if(map->x_password_door == 0 && map->y_password_door == 0){
                        start_color();
                        attron(COLOR_PAIR(2));
                        mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"@");
                        attroff(COLOR_PAIR(2));
                        mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
                        refresh();
                    }else{
                        start_color();
                        attron(COLOR_PAIR(1));
                        mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"@");
                        attroff(COLOR_PAIR(1));
                        mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
                        refresh();
                    }
                }
                if(i == map->number_Spell_room){
                    if(i == 0){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("_");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 1){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 2){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("_");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 3){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 4){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 5){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("_");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 6){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("|");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                    if(i == 7){
                        attron(COLOR_PAIR(1));
                        move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                        printw("_");
                        move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                        printw("|");
                        attroff(COLOR_PAIR(1));
                        refresh();
                    }
                }
                if(map->has_treasure_room && i == map->number_Treasure_Room){
                    move(map->y_end,map->x_end);
                    printw("%s",y);
                    refresh();
                }
                for(int j = 0; j < 3; j++){
                    if(map->foods[j].number_room == i && (map->foods[j].x_food != 0)){
                        move(map->foods[j].y_food,map->foods[j].x_food);
                        if(map->foods[j].type == 1 || map->foods[j].type == 4){
                            printw("%s",l);
                        }
                        if(map->foods[j].type == 2){
                            printw("%s",o);
                        }
                        if(map->foods[j].type == 3){
                            printw("%s",u);
                        }
                    }
                }
                for(int j = 0; j < 3; j++){
                    if(map->spells[j].number_room == i && (map->spells[j].x_spell != 0)){
                        move(map->spells[j].y_spell,map->spells[j].x_spell);
                        if(map->spells[j].type == 1){
                            printw("%s",f);
                        }
                        if(map->spells[j].type == 2){
                            printw("%s",g);
                        }
                        if(map->spells[j].type == 3){
                            printw("%s",h);
                        }
                    }
                }
                for(int j = 0; j < 3; j++){
                    if(map->wepons[j].number_room == i && (map->wepons[j].x_wepon != 0)){
                        move(map->wepons[j].y_wepon,map->wepons[j].x_wepon);
                        if(map->wepons[j].type == 1){
                            printw("%s",a);
                        }
                        if(map->wepons[j].type == 2){
                            printw("%s",b);
                        }
                        if(map->wepons[j].type == 3){
                            printw("%s",c);
                        }
                        if(map->wepons[j].type == 4){
                            printw("%s",d);
                        }
                        if(map->wepons[j].type == 5){
                            printw("%s",e);
                        }
                    }
                }
                for(int j = 0; j < map->number_of_wepons_use; j++){
                    if(map->wepons_use[j].x_wepon != 0){
                        move(map->wepons_use[j].y_wepon,map->wepons_use[j].x_wepon);
                        if(map->wepons_use[j].type == 2){
                            printw("%s",b);
                        }
                        if(map->wepons_use[j].type == 3){
                            printw("%s",c);
                        }
                        if(map->wepons_use[j].type == 4){
                            printw("%s",d);
                        }
                    }
                }
                for(int j = 0; j < 5; j++){
                    if(map->demons[j].number_of_room == i && (map->demons[j].x_demon != 0)){
                        move(map->demons[j].y_demon,map->demons[j].x_demon);
                        if(map->demons[j].type == 1){
                            printw("D");
                        }
                        if(map->demons[j].type == 2){
                            printw("F");
                        }
                        if(map->demons[j].type == 3){
                            printw("G");
                        }
                        if(map->demons[j].type == 4){
                            printw("S");
                        }
                        if(map->demons[j].type == 5){
                            printw("U");
                        }
                    }
                }
                for(int j = 0; j < 3; j++){
                    if(map->golds[j].number_room == i && (map->golds[j].x_gold != 0)){
                        move(map->golds[j].y_gold,map->golds[j].x_gold);
                        if(map->golds[j].is_black){
                            printw("%s",v);
                        }else{
                            printw("%s",m);
                        }
                    }
                }
                for(int j = 0; j < 3; j++){
                    if(map->pillors[j].number_room == i){
                        mvprintw(map->pillors[j].y_pillar,map->pillors[j].x_pillar,"O");
                    }
                }
                if(map->number_room_stair == i){
                    mvprintw(map->y_stair,map->x_stair,"<");
                }
                if(map->number_Password_Doors_room == i){
                    mvprintw(map->y_create_paasword,map->x_create_paasword,"&");
                }
                if(map->number_Master_Key_room == i && (map->x_Master_Key != 0)){
                    move(map->y_Master_Key,map->x_Master_Key);
                    attron(COLOR_PAIR(3));
                    printw("%s",r);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
            }
        }
    }else{
        int x_1 = map->rooms[0].x;  
        int x_2 = map->rooms[0].x + map->rooms[0].size;  
        int y_1 = map->rooms[0].y;  
        int y_3 = map->rooms[0].y + map->rooms[0].size; 
        for (int j = 0; j <= map->rooms[0].size; j++) {  
            attron(COLOR_PAIR(4));
            move(y_1, x_1 + j);
            printw("_");  
            move(y_3, x_1 + j);  
            printw("_");
            attroff(COLOR_PAIR(4));
            refresh();
        }  
        for (int j = 0; j < map->rooms[0].size; j++){  
            attron(COLOR_PAIR(4));
            move(y_1 + j + 1, x_1);  
            printw("|");
            move(y_1 + j + 1, x_2);  
            printw("|");
            attroff(COLOR_PAIR(4));
            refresh();
        }
        for(int j = 0; j < 5; j++){
            if(map->demons[j].x_demon != 0){
                move(map->demons[j].y_demon,map->demons[j].x_demon);
                if(map->demons[j].type == 1){
                    printw("D");
                }
                if(map->demons[j].type == 2){
                    printw("F");
                }
                if(map->demons[j].type == 3){
                    printw("G");
                }
                if(map->demons[j].type == 4){
                    printw("S");
                }
                if(map->demons[j].type == 5){
                    printw("U");
                }
            }
        }
        for(int j = 0; j < 3; j++){
            if(map->golds[j].x_gold != 0){
                move(map->golds[j].y_gold,map->golds[j].x_gold);
                if(map->golds[j].is_black){
                    printw("%s",v);
                }else{
                    printw("%s",m);
                }
            }
        }
        for(int j = 0; j < 3; j++){
            mvprintw(map->pillors[j].y_pillar,map->pillors[j].x_pillar,"O");
        }
        for(int j = 0; j < map->number_of_wepons_use; j++){
            if(map->wepons_use[j].x_wepon != 0){
                move(map->wepons_use[j].y_wepon,map->wepons_use[j].x_wepon);
                if(map->wepons_use[j].type == 2){
                    printw("%s",b);
                }
                if(map->wepons_use[j].type == 3){
                    printw("%s",c);
                }
                if(map->wepons_use[j].type == 4){
                    printw("%s",d);
                }
            }
        }
    }
}

void make_corrider_is_seen(struct Map *map,int x, int y){
    for(int i = 0; i < map->corridor_count; i++){
        if(map->corridors[i].x_corridor == x && map->corridors[i].y_corrifer == y){
            map->corridors[i].is_seen = 1;
        }
    }
}

void check_corrider(struct Map *map,int x, int y){
    if(is_corridor(map,x + 1,y)){
        make_corrider_is_seen(map,x + 1,y);        
    }
    if(is_corridor(map,x + 2,y)){
        make_corrider_is_seen(map,x + 2,y);        
    }
    if(is_corridor(map,x + 3,y)){
        make_corrider_is_seen(map,x + 3,y);        
    }
    if(is_corridor(map,x + 4,y)){
        make_corrider_is_seen(map,x + 4,y);        
    }
    if(is_corridor(map,x + 5,y)){
        make_corrider_is_seen(map,x + 5,y);        
    }
    if(is_corridor(map,x - 1,y)){
        make_corrider_is_seen(map,x - 1,y);        
    }
    if(is_corridor(map,x - 2,y)){
        make_corrider_is_seen(map,x - 2,y);        
    }
    if(is_corridor(map,x - 3,y)){
        make_corrider_is_seen(map,x - 3,y);        
    }
    if(is_corridor(map,x - 4,y)){
        make_corrider_is_seen(map,x - 4,y);        
    }
    if(is_corridor(map,x - 5,y)){
        make_corrider_is_seen(map,x - 5,y);        
    }
    if(is_corridor(map,x,y + 1)){
        make_corrider_is_seen(map,x,y + 1);        
    }
    if(is_corridor(map,x,y + 2)){
        make_corrider_is_seen(map,x,y + 2);        
    }
    if(is_corridor(map,x,y + 3)){
        make_corrider_is_seen(map,x,y + 3);        
    }
    if(is_corridor(map,x,y + 4)){
        make_corrider_is_seen(map,x,y + 4);        
    }
    if(is_corridor(map,x,y + 5)){
        make_corrider_is_seen(map,x,y + 5);        
    }
    if(is_corridor(map,x,y - 1)){
        make_corrider_is_seen(map,x,y - 1);        
    }
    if(is_corridor(map,x,y - 2)){
        make_corrider_is_seen(map,x,y - 2);        
    }
    if(is_corridor(map,x,y - 3)){
        make_corrider_is_seen(map,x,y - 3);        
    }
    if(is_corridor(map,x,y - 4)){
        make_corrider_is_seen(map,x,y - 4);        
    }
    if(is_corridor(map,x,y - 5)){
        make_corrider_is_seen(map,x,y - 5);        
    }
}

void print(struct Map *map,int x, int y,struct Player *p){
    if(map->number_of_rooms != 1){
        print_corridor(map);
    }
    print_room(map,p);
    if(is_room(map,x,y)){
        print_room(map,p);
    }
    else{
        if(map->number_of_rooms != 1){
            check_corrider(map,x,y);
            print_corridor(map); 
        }
    }
}

int is_room_and_index(struct Map *map,int x,int y){
    for(int i = 0; i < map->number_of_rooms; i++){
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x >= room_x1 && x <= room_x2 && y >= room_y1 && y <= room_y2) {
            return i + 1;
        }
    }
    return 0;
}

void create_new_player() {  
    clear();  
    refresh();  
    FILE* players = fopen("Players.txt", "a");  
    move(1, 0);  
    char name[100];  
    printw("Name: ");  
    move(1, 7);  
    scanw("%s", name);  
    while (!control_name(name)) {  
        move(1, 0);  
        clrtoeol();  
        printw("Name: ");  
        move(1, 7);  
        scanw("%s", name);  
    }     
    char password[20];  
    int c;  
    while (1) {  
        move(0, 0);  
        printw("Press 'a' to create a random password or 'b' to enter your password");  
        cbreak();  
        noecho();  
        c = getch();    
        if (c == 'a') {  
            random_password();  
            break;  
        } else if (c == 'b') {  
            break;  
        }  
    }
    move(2, 0);  
    printw("password: ");  
    move(2, 11);  
    nocbreak();  
    echo();  
    scanw("%s", password);  
    while (!control_password(password)) {  
        move(2, 0);  
        clrtoeol();  
        printw("password: ");  
        move(2, 11);  
        scanw("%s", password);  
    }
    move(0,0);
    clrtoeol();
    move(10,10);
    clrtoeol();
    refresh();
	char email[20];
	move(3,0);
	printw("email: ");
	move(3,8);
	scanw("%s",email);
	while(!control_email(email)){
		move(3,0);
		clrtoeol();
		printw("email: ");
		move(3,8);
		scanw("%s",email);
	}
    struct Player p;
    p.level = 2;
    p.food_number = 0;
    p.gold = 0;
    p.have_master_key = 0;
    p.spell_number = 0;
    p.wepon_number = 1;
    p.wepons[0].type = 1;
    p.x_map1 = 0;
    p.x_map2 = 0;
    p.x_map3 = 0;
    p.x_map4 = 0;
    p.x_map5 = 0;
    p.y_map1 = 0;
    p.y_map2 = 0;
    p.y_map3 = 0;
    p.y_map4 = 0;
    p.y_map5 = 0;
    p.hungry = 5;
    p.game_number = 0;
    p.total_game = 0;
    p.win_last_game = 2;
    p.color = 0;
    p.step = 0;
    p.type_wepon_chosen = 1;
    p.total_step = 0;
    p.total_step_food = 0;
    time(&p.begin_game);
    save_player(name,&p);
	fprintf(players,"%s %s %s %d %d %d %d\n",name,password,email,p.gold,p.gold,p.game_number,0);
    fclose(players);  
    getch();  
    return;  
}

int control_name(char name[100]) {
	FILE* ptr = fopen("Players.txt", "r");
	char name_existed[20];
	while(fscanf(ptr, "%s", name_existed) != EOF) {
		if (strcmp(name, name_existed) == 0) {
			mvprintw(10, 10, "This Username is already exist, please try another name");
			if(getch()){
				move(10,10);
				clrtoeol();
			}
			fclose(ptr);
			return 0;
		}
	}
	fclose(ptr); 
	return 1; 
}

int control_password(char password[20]){
	int len_password = strlen(password);
	int control1 = 0;
	int control2 = 0;
	if(len_password < 7){
		mvprintw(10, 10, "This password is too short!");
		if(getch()){
			move(10,10);
			clrtoeol();
		}
		return 0;
	}
	for(int i = 0; i < len_password; i++){
		if(password[i] >= 'a' && password[i] <= 'z'){
			control1 = 1;
			break;
		}
	}
	for(int i = 0; i < len_password; i++){
		if(password[i] >= 'A' && password[i] <= 'Z'){
			control2 = 1;
			break;
		}
	}
	if(control1 && control2){
		return 1;
	}
	mvprintw(10, 10, "You have to use atleast one lower or upper charector!");
	if(getch()){
		move(10,10);
		clrtoeol();
	}
	return 0;
}

int control_email(char email[20]) {
    int pointer = 0;
    while (email[pointer] != '@') {
        if (email[pointer] == '.' || email[pointer] == '\0') {
            mvprintw(10, 10, "Not valid!");
    		if (getch()) {
        		move(10, 10);
        		clrtoeol();
    		}
            return 0;
        }
        pointer++;
    }
    if (pointer == 0 || pointer == strlen(email) - 1) {
        mvprintw(10, 10, "Not valid!");
    	if (getch()) {
        	move(10, 10);
    		clrtoeol();
		}
        return 0;
    }    
    int point_at = pointer;
    pointer++;
    while (email[pointer] != '.') {
        if (email[pointer] == '@' || email[pointer] == '\0') {
            mvprintw(10, 10, "Not valid!");
    		if (getch()) {
        		move(10, 10);
        		clrtoeol();
    		}
            return 0;
        }
        pointer++;
    }
    if (pointer == point_at + 1 || pointer == strlen(email) - 1) {
        mvprintw(10, 10, "Not valid!");
    	if (getch()) {
    		move(10, 10);
    		clrtoeol();
		}
        return 0;
    }    
    int point_dot = pointer;
    pointer++;
    while (email[pointer] != '\0') {
        pointer++;
    }    
    if (pointer == point_dot + 1) {
        mvprintw(10, 10, "Not valid!");
    	if (getch()) {
    		move(10, 10);
    		clrtoeol();
		}
        return 0;
    }
    return 1;
}

int valid_name_and_password(char name[100], char password[20]) {
    FILE* players = fopen("Players.txt", "r");
    char name_existed[20];
    char password_existed[20];
    char email[20];
    int score,gild,number,time;
    while (fscanf(players, "%s %s %s %d %d %d %d", name_existed, password_existed,email,&score,&gild,&number,&time) != EOF) {
        if (strcmp(name, name_existed) == 0 && strcmp(password, password_existed) == 0) {
            fclose(players);
            return 1;
        }
    }
    mvprintw(10, 10, "%s", "Invalid name or password");
    if (getch()) {
        move(10, 10);
        clrtoeol();
    }
	fclose(players);
    return 0;
}

void enter_player() {
    clear();
	label:
    const char *choices[3] = {
        "sign in",
        "Guest",
        "back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(5,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 3 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				nocbreak();
    			echo();
    			curs_set(1);
				if(current_item == 0){
					refresh();
					clear();
					move(1,0);
					printw("Name: ");
					char name[100];
					move(1,7);
					scanw("%s", name);
					move(2,0);
					printw("Password: ");
					char password[20];
                    move(0,0);
                    printw("Press 'a' if you forget your password(it will show the first) or 'b' to enter your password");
                    cbreak();
                    noecho();
                    int c;
                    while(1){
                        c = getch();
                        if(c == 'a'){
                            show_password(name);
                            break;
                        }else{
                            break;
                        }
                    }
                    nocbreak();
                    echo();
					move(2,11);
					scanw("%s", password);
					while (!valid_name_and_password(name, password)) {
						move(1, 0);
						clrtoeol();
						printw("Name: ");
						move(1,7);
						scanw("%s", name);
						move(2, 0);
						clrtoeol();
						printw("Password: ");
						move(2,11);
						scanw("%s", password);
					}
					getch();
					players_menus(name);
					clear();
					refresh();
					goto label;
				}
				if(current_item == 1){
					guest();
					clear();
					refresh();
					goto label;
				}
				if(current_item == 2){
                	return;
				}
                break;
        }
    }
}

void table_of_players(char name[100]) {
	clear();
	refresh();
	FILE* players = fopen("Players.txt", "r");
	typedef struct {
    	char name_[100];
    	char password[20];
    	char email[20];
    	int score;
    	int gold;
    	int number_of_game;
    	int time;
    	int rank;
	} user;
    user* users = (user*)malloc(20 * sizeof(user));
    int size_users = 0;
    while (fscanf(players, "%s %s %s %d %d %d %d",
                  users[size_users].name_,
                  users[size_users].password,
                  users[size_users].email,
                  &users[size_users].score,
                  &users[size_users].gold,
                  &users[size_users].number_of_game,
                  &users[size_users].time) != EOF) {
        users[size_users].rank = size_users + 1;
        size_users++;
    }
    fclose(players);
    for (int i = 0; i < size_users - 1; i++) {
        for (int j = 0; j < size_users - i - 1; j++) {
            if (users[j].score < users[j + 1].score) {
                user temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
				int temp2 = users[j].rank;
                users[j].rank = users[j + 1].rank;
                users[j + 1].rank = temp2;
            }
        }
    }
	start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
	move(0,0);
	printw("Press any key to back");
    move(4, 14);
    printw("-------------------------------------------------------------");
    move(5, 14);
    printw("| Rank |         Name         | Score | Gold | Games | exp |");
    for (int i = 0; i < size_users; i++) {
        move(6 + i, 14);
        if (i == 0) {
            attron(A_REVERSE | COLOR_PAIR(1));
            if(!strcmp(name,users[i].name_)){
                move(6 + i,1);
                printw("->");
            }
			move(6 + i,4);
			printw("(Goat)");
			move(6 + i, 14);
            printw("|%-6d|%-20s%s|%-7d|%-6d|%-7d|%-5d|", users[i].rank, users[i].name_,x, users[i].score, users[i].gold, users[i].number_of_game, users[i].time);
            attroff(A_REVERSE | COLOR_PAIR(1));
        } else if (i == 1) {
            attron(A_BOLD | COLOR_PAIR(2));
			if(!strcmp(name,users[i].name_)){
                move(6 + i,1);
                printw("->");
            }
            move(6 + i,4);
			printw("(Legend)");
			move(6 + i,14);
            printw("|%-6d|%-20s%s|%-7d|%-6d|%-7d|%-5d|", users[i].rank, users[i].name_,x, users[i].score, users[i].gold, users[i].number_of_game, users[i].time);
            attroff(A_BOLD | COLOR_PAIR(2));
        } else if (i == 2) {
            attron(A_DIM | COLOR_PAIR(3));
			if(!strcmp(name,users[i].name_)){
                move(6 + i,1);
                printw("->");
            }
            move(6 + i,4);
			printw("(Good)");
			move(6 + i,14);
            printw("|%-6d|%-20s%s|%-7d|%-6d|%-7d|%-5d|", users[i].rank, users[i].name_,x, users[i].score, users[i].gold, users[i].number_of_game, users[i].time);
            attroff(A_DIM | COLOR_PAIR(3));
        } else {
            if(!strcmp(name,users[i].name_)){
                move(6 + i,1);
                printw("->");
            }
            move(6 + i,14);
            printw("|%-6d|%-20s  |%-7d|%-6d|%-7d|%-5d|", users[i].rank, users[i].name_, users[i].score, users[i].gold, users[i].number_of_game, users[i].time);
        }
    }
	move(6 + size_users,14);
	printw("-------------------------------------------------------------");
    free(users);
	getch();
	return;
}

void menus(){
	clear();
	label:
    const char *choices[3] = {
        "create new player",
        "Enter player",
        "Exit"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(5,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 3 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				nocbreak();
    			echo();
    			curs_set(1);
				if(current_item == 0){
					create_new_player();
					clear();
					refresh();
					goto label;
				}
				if(current_item == 1){
					enter_player();
					clear();
					refresh();
					goto label;
				}
                if(current_item == 2){
                    clear();
					refresh();
                    return;
                }
        
        }
    }
}

void players_menus(char name[100]){
	clear();
    refresh();
    const char *choices[6] = {
        "Create new game",
		"Continue last game",
        "Setting menu",
        "Profile",
        "Table of players",
        "Back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(7,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 6; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 6 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1 || current_item == 2 || current_item == 3 || current_item == 4){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				if(current_item == 0){
					create_new_game(name);
					clear();
					refresh();
				}
				if(current_item == 1){
					continue_last_game(name);
					clear();
					refresh();
				}
				if(current_item == 2){
					seting_menu(name);
					clear();
					refresh();
				}
				if(current_item == 3){
					profile(name);
					clear();
					refresh();
				}
                if(current_item == 4){
                    table_of_players(name);
                    clear();
                    refresh();
                }
                if(current_item == 5){
                    clear();
                    refresh();
                    return;
                }
                break;
        }
    }	
}

void create_new_game(char name[100]){
    start_color();
    // init_pair(1,COLOR_GREEN,COLOR_BLACK);
    // init_pair(2,COLOR_RED,COLOR_BLACK);
    first_damage = 0;
    char name1[100];
    char name2[100]; 
    char name3[100];
    char name4[100];
    char name5[100];
    char name6[100]; 
    char name7[100];
    char name8[100];
    char name9[100];
    char name10[100];
    char name11[100];
    char name12[100];
    char name13[100];
    char name14[100];
    char name15[100];
    char name16[100];
    strcpy(name1,name);
    strcpy(name2,name);
    strcpy(name3,name);
    strcpy(name4,name);
    strcpy(name5,name);
    strcpy(name6,name);
    strcpy(name7,name);
    strcpy(name8,name);
    strcpy(name9,name);
    strcpy(name10,name);
    strcpy(name11,name);
    strcpy(name12,name);
    strcpy(name13,name);
    strcpy(name14,name);
    strcpy(name15,name);
    strcpy(name16,name);
    remove_file(name9,1);
    remove_file(name10,2);
    remove_file(name11,3);
    remove_file(name12,4);
    remove_file(name16,5);
    generate_map(&game.maps[0],name1,1);
    generate_map(&game.maps[1],name2,2);
    generate_map(&game.maps[2],name3,3);
    generate_map(&game.maps[3],name4,4);
    generate_map(&game.maps[4],name13,5);
    game.maps[0].last_password = 0;
    game.maps[1].last_password = 0;
    game.maps[2].last_password = 0;
    game.maps[3].last_password = 0;
    struct Player p;
    load_player(name,&p);
    for(int i = 0; i < p.food_number; i++){
        p.foods[i].type = 0;
    }
    for(int i = 0; i < p.wepon_number; i++){
        p.wepons[i].type = 0;
    }
    for(int i = 0; i < p.spell_number; i++){
        p.spells[i].type = 0;
    }
    p.health = p.level * 1000;
    p.food_number = 0;
    p.gold = 0;
    p.have_master_key = 0;
    p.spell_number = 0;
    p.wepon_number = 1;
    p.wepons[0].type = 1;
    p.x_map1 = 0;
    p.x_map2 = 0;
    p.x_map3 = 0;
    p.x_map4 = 0;
    p.x_map5 = 0;
    p.y_map1 = 0;
    p.y_map2 = 0;
    p.y_map3 = 0;
    p.y_map4 = 0;
    p.y_map5 = 0;
    p.hungry = 5;
    p.win_last_game = 2;
    p.last_map = 1;
    p.step = 0;
    p.type_wepon_chosen = 1;
    p.total_step = 0;
    p.total_step_food = 0;
    time(&p.priod);
    p.win_last_game = 2;
    move_player(&game.maps[0],&p,1);
    clear();
    refresh();
    game_over = 0;
    if(p.win_last_game == 1){
        move(10,10);
        attron(COLOR_PAIR(2));
        printw("Congratulations. You won!\n          total gold you get: %d\n          the score you get: %d",p.gold,p.gold * (p.game_number + 1));
        attroff(COLOR_PAIR(2));
        refresh();
        getch();
        clear();
        refresh();
        p.game_number += 1;
        p.total_game += 1;
        p.last_map = 1;
        p.win_last_game = 2;
        FILE* players = fopen("Players.txt","r");
        FILE* temp = fopen("temp.txt","w");
        char line[256];
        while (fgets(line, sizeof(line), players) != NULL) {
            char name_x[100], password[100], email[100];
            int gold, gold2, game_number, time_of_play;
            sscanf(line, "%s %s %s %d %d %d %d", name_x, password, email, &gold, &gold2, &game_number, &time_of_play);
            if(!strcmp(name,name_x)){
                gold += p.gold;
                gold2 += p.gold * p.game_number;
                game_number = p.game_number;
                time_of_play = p.total_game;
                fprintf(temp, "%s %s %s %d %d %d %d\n", name_x, password, email, gold, gold2, game_number, time_of_play);
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(players);
        fclose(temp);
        remove("Players.txt");
        rename("temp.txt","Players.txt");
        for(int i = 0; i < p.food_number; i++){
            p.foods[i].type = 0;
        }
        for(int i = 0; i < p.wepon_number; i++){
            p.wepons[i].type = 0;
        }
        for(int i = 0; i < p.spell_number; i++){
            p.spells[i].type = 0;
        }
        p.health = p.level * 1000;
        p.food_number = 0;
        p.gold = 0;
        p.have_master_key = 0;
        p.spell_number = 0;
        p.wepon_number = 1;
        p.wepons[0].type = 1;
        p.x_map1 = 0;
        p.x_map2 = 0;
        p.x_map3 = 0;
        p.x_map4 = 0;
        p.x_map5 = 0;
        p.y_map1 = 0;
        p.y_map2 = 0;
        p.y_map3 = 0;
        p.y_map4 = 0;
        p.y_map5 = 0;
        p.hungry = 5;
        p.step = 0;
        p.type_wepon_chosen = 1;
        p.total_step = 0;
        p.total_step_food = 0;
        save_player(name,&p);
        remove_file(name5,1);
        remove_file(name6,2);
        remove_file(name7,3);
        remove_file(name8,4);
        remove_file(name14,5);
        return;
    }
    else if(p.win_last_game == 0){
        move(10,10);
        attron(COLOR_PAIR(1));
        printw("Sorry. You lost the game!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        clear();
        refresh();
        p.total_game += 1;
        p.last_map  = 1;
        p.win_last_game = 2;
        FILE* players = fopen("Players.txt","r");
        FILE* temp = fopen("temp.txt","w");
        char line[256];
        while (fgets(line, sizeof(line), players) != NULL) {
            char name_x[100], password[100], email[100];
            int gold, gold2, game_number, time_of_play;
            sscanf(line, "%s %s %s %d %d %d %d", name_x, password, email, &gold, &gold2, &game_number, &time_of_play);
            if(!strcmp(name,name_x)){
                time_of_play = p.total_game;
                fprintf(temp, "%s %s %s %d %d %d %d\n", name_x, password, email, gold, gold2, game_number, time_of_play);
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(players);
        fclose(temp);
        remove("Players.txt");
        rename("temp.txt","Players.txt");
        for(int i = 0; i < p.food_number; i++){
            p.foods[i].type = 0;
        }
        for(int i = 0; i < p.wepon_number; i++){
            p.wepons[i].type = 0;
        }
        for(int i = 0; i < p.spell_number; i++){
            p.spells[i].type = 0;
        }
        p.health = p.level * 1000;
        p.food_number = 0;
        p.gold = 0;
        p.have_master_key = 0;
        p.spell_number = 0;
        p.wepon_number = 1;
        p.wepons[0].type = 1;
        p.x_map1 = 0;
        p.x_map2 = 0;
        p.x_map3 = 0;
        p.x_map4 = 0;
        p.x_map5 = 0;
        p.y_map1 = 0;
        p.y_map2 = 0;
        p.y_map3 = 0;
        p.y_map4 = 0;
        p.y_map5 = 0;
        p.hungry = 5;
        p.step = 0;
        p.type_wepon_chosen = 1;
        p.total_step = 0;
        p.total_step_food = 0;
        save_player(name,&p);
        remove_file(name5,1);
        remove_file(name6,2);
        remove_file(name7,3);
        remove_file(name8,4);
        remove_file(name14,5);
        return;
    }
    else if(p.win_last_game == -1){
        save_player(name,&p);
        save_map(name5,&game.maps[0],1);
        save_map(name6,&game.maps[1],2);
        save_map(name7,&game.maps[2],3);
        save_map(name8,&game.maps[3],4);
        save_map(name14,&game.maps[4],5);
        return;
    }
}

void continue_last_game(char name[100]){
    start_color();
    // init_pair(1,COLOR_GREEN,COLOR_BLACK);
    // init_pair(2,COLOR_RED,COLOR_BLACK);
    char name1[100];
    char name2[100]; 
    char name3[100];
    char name4[100];
    char name5[100];
    char name6[100]; 
    char name7[100];
    char name8[100];
    char name9[100];
    char name10[100];
    char name11[100];
    strcpy(name1,name);
    strcpy(name2,name);
    strcpy(name3,name);
    strcpy(name4,name);
    strcpy(name5,name);
    strcpy(name6,name);
    strcpy(name7,name);
    strcpy(name8,name);
    strcpy(name9,name);
    strcpy(name10,name);
    strcpy(name11,name);
    load_map(name1,&game.maps[0],1);
    load_map(name2,&game.maps[1],2);
    load_map(name3,&game.maps[2],3);
    load_map(name4,&game.maps[3],4);
    load_map(name10,&game.maps[4],5);
    struct Player p;
    load_player(name,&p);
    if(p.win_last_game != -1){
        move(10,10);
        printw("There is no last game!");
        clear();
        refresh();
        return;
    }
    if(!file_exists(name9)){
        move(10,10);
        printw("There is no last game!");
        clear();
        refresh();
        return;
    }
    time(&p.priod);
    p.win_last_game = 2;
    move_player(&game.maps[p.last_map - 1],&p,p.last_map);
    clear();
    refresh();
    game_over = 0;
    if(p.win_last_game == 1){
        move(10,10);
        attron(COLOR_PAIR(2));
        printw("Congratulations. You won!\n          total gold you get: %d\n          the score you get: %d",p.gold,p.gold * (p.game_number + 1));
        attroff(COLOR_PAIR(2));
        refresh();
        getch();
        clear();
        refresh();
        p.game_number += 1;
        p.total_game += 1;
        FILE* players = fopen("Players.txt","r");
        FILE* temp = fopen("temp.txt","w");
        char line[256];
        while (fgets(line, sizeof(line), players) != NULL) {
            char name_x[100], password[100], email[100];
            int gold, gold2, game_number, time_of_play;
            sscanf(line, "%s %s %s %d %d %d %d", name_x, password, email, &gold, &gold2, &game_number, &time_of_play);
            if(!strcmp(name,name_x)){
                gold += p.gold;
                gold2 += p.gold * p.game_number;
                game_number = p.game_number;
                time_of_play = p.total_game;
                fprintf(temp, "%s %s %s %d %d %d %d\n", name_x, password, email, gold, gold2, game_number, time_of_play);
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(players);
        fclose(temp);
        remove("Players.txt");
        rename("temp.txt","Players.txt");
        p.last_map = 1;
        p.win_last_game = 2;
        for(int i = 0; i < p.food_number; i++){
            p.foods[i].type = 0;
        }
        for(int i = 0; i < p.wepon_number; i++){
            p.wepons[i].type = 0;
        }
        for(int i = 0; i < p.spell_number; i++){
            p.spells[i].type = 0;
        }
        p.health = p.level * 1000;
        p.food_number = 0;
        p.gold = 0;
        p.have_master_key = 0;
        p.spell_number = 0;
        p.wepon_number = 1;
        p.wepons[0].type = 1;
        p.x_map1 = 0;
        p.x_map2 = 0;
        p.x_map3 = 0;
        p.x_map4 = 0;
        p.x_map5 = 0;
        p.y_map1 = 0;
        p.y_map2 = 0;
        p.y_map3 = 0;
        p.y_map4 = 0;
        p.y_map5 = 0;
        p.hungry = 5;
        p.step = 0;
        p.type_wepon_chosen = 1;
        p.total_step = 0;
        p.total_step_food = 0;
        save_player(name,&p);
        remove_file(name5,1);
        remove_file(name6,2);
        remove_file(name7,3);
        remove_file(name8,4);
        remove_file(name11,5);
        return;
    }
    else if(p.win_last_game == 0){
        move(10,10);
        attron(COLOR_PAIR(1));
        printw("Sorry. You lost the game!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        clear();
        refresh();
        p.total_game += 1;
        FILE* players = fopen("Players.txt","r");
        FILE* temp = fopen("temp.txt","w");
        char line[256];
        while (fgets(line, sizeof(line), players) != NULL) {
            char name_x[100], password[100], email[100];
            int gold, gold2, game_number, time_of_play;
            sscanf(line, "%s %s %s %d %d %d %d", name_x, password, email, &gold, &gold2, &game_number, &time_of_play);
            if(!strcmp(name,name_x)){
                time_of_play = p.total_game;
                fprintf(temp, "%s %s %s %d %d %d %d\n", name_x, password, email, gold, gold2, game_number, time_of_play);
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(players);
        fclose(temp);
        remove("Players.txt");
        rename("temp.txt","Players.txt");
        p.last_map  = 1;
        p.win_last_game = 2;
        for(int i = 0; i < p.food_number; i++){
            p.foods[i].type = 0;
        }
        for(int i = 0; i < p.wepon_number; i++){
            p.wepons[i].type = 0;
        }
        for(int i = 0; i < p.spell_number; i++){
            p.spells[i].type = 0;
        }
        p.health = p.level * 1000;
        p.food_number = 0;
        p.gold = 0;
        p.have_master_key = 0;
        p.spell_number = 0;
        p.wepon_number = 1;
        p.wepons[0].type = 1;
        p.x_map1 = 0;
        p.x_map2 = 0;
        p.x_map3 = 0;
        p.x_map4 = 0;
        p.x_map5 = 0;
        p.y_map1 = 0;
        p.y_map2 = 0;
        p.y_map3 = 0;
        p.y_map4 = 0;
        p.y_map5 = 0;
        p.hungry = 5;
        p.step = 0;
        p.type_wepon_chosen = 1;
        p.total_step = 0;
        p.total_step_food = 0;
        save_player(name,&p);
        remove_file(name5,1);
        remove_file(name6,2);
        remove_file(name7,3);
        remove_file(name8,4);
        remove_file(name11,5);
        return;
    }
    else if(p.win_last_game == -1){
        save_player(name,&p);
        save_map(name5,&game.maps[0],1);
        save_map(name6,&game.maps[1],2);
        save_map(name7,&game.maps[2],3);
        save_map(name8,&game.maps[3],4);
        save_map(name11,&game.maps[4],5);
        return;
    }
}

void seting_menu(char name[100]){
    struct Player p;
    load_player(name,&p);
    const char *choices[4] = {
        "change color hero",
		"level",
        "music",
        "back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(7,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 4; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 4 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1 || current_item == 2){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				if(current_item == 0){
					chose_color(&p);
					clear();
					refresh();
				}
				if(current_item == 1){
					chose_level(&p);
					clear();
					refresh();
				}
				if(current_item == 2){
                    music_menu();
					clear();
					refresh();
				}
				if(current_item == 3){
					save_player(name,&p);
                    return;
					clear();
					refresh();
				}
                break;
        }
    }
}

void profile(char name[100]){
    clear();
    refresh();
    struct Player p;
    load_player(name,&p);
    move(10,10);
    printw("Name: %s",name);
    move(11,10);
    printw("Number of game: %d",p.game_number);
    move(12,10);
    printw("Golds: %d",p.gold);
    move(13,10);
    printw("Date of begin: %s",ctime(&p.begin_game));
    getch();
    clear();
    refresh();
    return;
}

void guest(){
    start_color();
    // init_pair(1,COLOR_GREEN,COLOR_BLACK);
    // init_pair(2,COLOR_RED,COLOR_BLACK);
    char name1[100] = "guest"; 
    char name2[100] = "guest"; 
    char name3[100] = "guest"; 
    char name4[100] = "guest";
    char name5[100] = "guest";
    generate_map(&game.maps[0],name1,1);
    generate_map(&game.maps[1],name2,2);
    generate_map(&game.maps[2],name3,3);
    generate_map(&game.maps[3],name4,4);
    generate_map(&game.maps[4],name4,5);
    struct Player guest_player;
    guest_player.health = 1000;
    guest_player.food_number = 0;
    guest_player.gold = 0;
    guest_player.have_master_key = 0;
    guest_player.spell_number = 0;
    guest_player.wepon_number = 1;
    guest_player.wepons[0].type = 1;
    guest_player.x_map1 = 0;
    guest_player.x_map2 = 0;
    guest_player.x_map3 = 0;
    guest_player.x_map4 = 0;
    guest_player.x_map5 = 0;
    guest_player.y_map1 = 0;
    guest_player.y_map2 = 0;
    guest_player.y_map3 = 0;
    guest_player.y_map4 = 0;
    guest_player.y_map5 = 0;
    guest_player.hungry = 5;
    guest_player.game_number = 0;
    guest_player.win_last_game = 2;
    guest_player.step = 0;
    game.maps[0].last_password = 0;
    game.maps[1].last_password = 0;
    game.maps[2].last_password = 0;
    game.maps[3].last_password = 0;
    time(&guest_player.priod);
    move_player(&game.maps[0],&guest_player,1);
    clear();
    refresh();
    if(guest_player.win_last_game == 1){
        move(10,10);
        attron(COLOR_PAIR(2));
        printw("Congratulations. You won!\n          gold you get: %d",guest_player.gold);
        attroff(COLOR_PAIR(2));
        refresh();
        getch();
        clear();
        refresh();
        return;
    }
    else if(guest_player.win_last_game == 0){
        move(10,10);
        attron(COLOR_PAIR(1));
        printw("Sorry. You lost the game!");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        clear();
        refresh();
        return;
    }
    else if(guest_player.win_last_game == -1){
        return;
    }
}

void save_player(char filename[100], struct Player * p){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file");
        return;
    }
    fwrite(p, sizeof(struct Player), 1, file);
    fclose(file);
}

void load_player(char filename[100], struct Player * p){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file");
        return;
    }
    fread(p, sizeof(struct Player), 1, file);
    fclose(file);
}

int is_stair(struct Map *map,int x,int y){
    if(x == map->x_stair && y == map->y_stair){
        return 1;
    }
    return 0;
}

void fightroom(struct Player *p){
    int x_1 = 25;  
    int x_2 = 25 + 20;  
    int y_1 = 5;  
    int y_3 = 5 + 20;  
    for (int j = 0; j <= 20; j++) {  
        move(y_1, x_1 + j);
        printw("_");  
        move(y_3, x_1 + j);  
        printw("_");  
    }  
    for (int j = 0; j < 20; j++){  
        move(y_1 + j + 1, x_1);  
        printw("|");
        move(y_1 + j + 1, x_2);  
        printw("|");  
    }
    mvprintw(24,26,"P");
    mvprintw(6,44,"<");
    refresh();
    sleep(5);
    p->health -= 500;
    clear();
    return;
}

void show_wepons(struct Player *p){
    int type1 = 0,type2 = 0,type3 = 0,type4 = 0,type5 = 0;
    for(int i = 0; i < p->wepon_number; i++){
        type1 = 1;
        if(p->wepons[i].type == 2 && p->wepons[i].is_exist){
            type2 += p->wepons[i].cllect;
        }
        if(p->wepons[i].type == 3 && p->wepons[i].is_exist){
            type3 += p->wepons[i].cllect;
        }
        if(p->wepons[i].type == 4 && p->wepons[i].is_exist){
            type4 += p->wepons[i].cllect;
        }
        if(p->wepons[i].type == 5){
            if(type5 == 0){
                type5 += 1;
            }
        }
    }
    if(p->type_wepon_chosen == 1){
        move(9,16);
        printw("-->");
    }
    if(p->type_wepon_chosen == 2){
        move(12,16);
        printw("-->");
    }
    if(p->type_wepon_chosen == 3){
        move(13,16);
        printw("-->");
    }
    if(p->type_wepon_chosen == 4){
        move(14,16);
        printw("-->");
    }
    if(p->type_wepon_chosen == 5){
        move(10,16);
        printw("-->");
    }
    move(8,20);
    printw("Short range wepons:");
    move(9,20);
    printw("%s  -count:  %d  -key; l  -Power: 12",a,type1);
    move(10,20);
    printw("%s  -count:  %d  -key; h  -Power: 10",e,type5);
    move(11,20);
    printw("Long range wepons:");
    move(12,20);
    printw("%s  -count:  %d  -key; k  -Distance: 5  -Power: 12",b,type2);
    move(13,20);
    printw("%s  -count:  %d  -key; v  -Distance: 10  -Power: 15",c,type3);
    move(14,20);
    printw("%s  -count:  %d  -key; n  -Distance: 5  -Power: 5",d,type4);
    refresh();
    getch();
    clear();
}

void show_spells(struct Player *p){
    int type1 = 0,type2 = 0,type3 = 0;
    for(int i = 0; i < p->spell_number; i++){
        if(p->spells[i].type == 1 && p->spells[i].is_exist){
            type1 += 1;
        }
        if(p->spells[i].type == 2 && p->spells[i].is_exist){
            type2 += 1;
        }
        if(p->spells[i].type == 3 && p->spells[i].is_exist){
            type3 += 1;
        }
    }
    move(8,20);
    printw("%s  -count:  %d  -key; r  -Health",f,type1);
    move(9,20);
    printw("%s  -count:  %d  -key; y  -Speed",g,type2);
    move(10,20);
    printw("%s  -count:  %d  -key; p  -Damage",h,type3);
    refresh();
    getch();
    clear();
}

void show_foods(struct Player *p){
    int type1 = 0,type2 = 0,type3 = 0;
    for(int i = 0; i < p->food_number; i++){
        if(p->foods[i].type == 1 || p->foods[i].type == 4){
            type1 += 1;
        }
        if(p->foods[i].type == 2){
            type2 += 1;
        }
        if(p->foods[i].type == 3){
            type3 += 1;
        }
    }
    move(8,20);
    printw("%d  %s  -regular food  -key: o",type1,l);
    move(9,20);
    printw("%d  %s  -alla food  -key: u",type2,o);
    move(10,20);
    printw("%d  %s  -magic food  -key: b",type3,u);
    refresh();
    getch();
    clear();
}

void show_map(struct Map *map){
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    for(int i = 0; i < map->number_of_rooms; i++){
        int x_1 = map->rooms[i].x;  
        int x_2 = map->rooms[i].x + map->rooms[i].size;  
        int y_1 = map->rooms[i].y;  
        int y_3 = map->rooms[i].y + map->rooms[i].size;  
        if(i == map->number_Spell_room){
            for (int j = 0; j <= map->rooms[i].size; j++) {  
                attron(COLOR_PAIR(1));
                move(y_1, x_1 + j);
                printw("_");  
                move(y_3, x_1 + j);  
                printw("_");
                attroff(COLOR_PAIR(1));
                refresh();
            }  
            for (int j = 0; j < map->rooms[i].size; j++){  
                attron(COLOR_PAIR(1));
                move(y_1 + j + 1, x_1);  
                printw("|");
                move(y_1 + j + 1, x_2);  
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
        }else if(map->has_treasure_room && i == map->number_Treasure_Room){
            for (int j = 0; j <= map->rooms[i].size; j++) {  
                attron(COLOR_PAIR(4));
                move(y_1, x_1 + j);
                printw("_");  
                move(y_3, x_1 + j);  
                printw("_");
                attroff(COLOR_PAIR(4));
                refresh();
            }  
            for (int j = 0; j < map->rooms[i].size; j++){  
                attron(COLOR_PAIR(4));
                move(y_1 + j + 1, x_1);  
                printw("|");
                move(y_1 + j + 1, x_2);  
                printw("|");
                attroff(COLOR_PAIR(4));
                refresh();
            }
        }
        else{
            for (int j = 0; j <= map->rooms[i].size; j++) {  
                move(y_1, x_1 + j);
                printw("_");  
                move(y_3, x_1 + j);  
                printw("_");  
            }  
            for (int j = 0; j < map->rooms[i].size; j++){  
                move(y_1 + j + 1, x_1);  
                printw("|");
                move(y_1 + j + 1, x_2);  
                printw("|");  
            }
        }
        for(int j = 0; j < 2; j++){
            mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"+");
            mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
        }
        if(i == map->number_Password_Doors_room){
            if(map->x_password_door == 0 && map->y_password_door == 0){
                start_color();
                attron(COLOR_PAIR(2));
                mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"@");
                attroff(COLOR_PAIR(2));
                mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
                refresh();
            }else{
                start_color();
                attron(COLOR_PAIR(1));
                mvprintw(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door,"@");
                attroff(COLOR_PAIR(1));
                mvprintw(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door,"+");
                refresh();
            }
        }
        if(i == map->number_Spell_room){
            if(i == 0){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("_");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 1){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 2){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("_");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 3){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 4){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 5){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("_");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 6){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("|");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
            if(i == 7){
                attron(COLOR_PAIR(1));
                move(map->rooms[i].doors[0].y_door,map->rooms[i].doors[0].x_door);
                printw("_");
                move(map->rooms[i].doors[1].y_door,map->rooms[i].doors[1].x_door);
                printw("|");
                attroff(COLOR_PAIR(1));
                refresh();
            }
        }
        if(map->has_treasure_room){
            move(map->y_end,map->x_end);
            printw("%s",y);
            refresh();
        }
        for(int j = 0; j < 3; j++){
            if(map->foods[j].number_room == i && map->foods[j].x_food != 0){
                move(map->foods[j].y_food,map->foods[j].x_food);
                if(map->foods[j].type == 1){
                    printw("%s",l);
                }
                if(map->foods[j].type == 2){
                    printw("%s",o);
                }
                if(map->foods[j].type == 3){
                    printw("%s",u);
                }
                if(map->foods[j].type == 4){
                    printw("%s",n);
                }
            }
        }
        for(int j = 0; j < 3; j++){
            if(map->spells[j].number_room == i && map->spells[i].x_spell != 0){
                move(map->spells[j].y_spell,map->spells[j].x_spell);
                if(map->spells[j].type == 1){
                    printw("%s",f);
                }
                if(map->spells[j].type == 2){
                    printw("%s",g);
                }
                if(map->spells[j].type == 3){
                    printw("%s",h);
                }
            }
        }
        for(int j = 0; j < 3; j++){
            if(map->wepons[j].number_room == i && map->wepons[i].x_wepon != 0){
                move(map->wepons[j].y_wepon,map->wepons[j].x_wepon);
                if(map->wepons[j].type == 1){
                    printw("%s",a);
                }
                if(map->wepons[j].type == 2){
                    printw("%s",b);
                }
                if(map->wepons[j].type == 3){
                    printw("%s",c);
                }
                if(map->wepons[j].type == 4){
                    printw("%s",d);
                }
                if(map->wepons[j].type == 5){
                    printw("%s",e);
                }
            }
        }
        for(int j = 0; j < 3; j++){
            if(map->golds[j].number_room == i && map->golds[i].x_gold != 0){
                move(map->golds[j].y_gold,map->golds[j].x_gold);
                if(map->golds[j].is_black){
                    printw("%s",v);
                }else{
                    printw("%s",m);
                }
            }
        }
        for(int j = 0; j < 5; j++){
            if(map->demons[j].number_of_room == i && map->demons[i].x_demon != 0){
                move(map->demons[j].y_demon,map->demons[j].x_demon);
                if(map->demons[j].type == 1){
                    printw("D");
                }
                if(map->demons[j].type == 2){
                    printw("F");
                }
                if(map->demons[j].type == 3){
                    printw("G");
                }
                if(map->demons[j].type == 4){
                    printw("S");
                }
                if(map->demons[j].type == 5){
                    printw("U");
                }
            }
        }
        for(int j = 0; j < 3; j++){
            if(map->pillors[j].number_room == i){
                mvprintw(map->pillors[j].y_pillar,map->pillors[j].x_pillar,"O");
            }
        }
        for(int j = 0; j < 3; j++){
            mvprintw(map->traps[j].y_trap,map->traps[j].x_trap,"^");
        }
        if(map->number_room_fight_room == i){
            mvprintw(map->y_fight_room,map->x_fight_room,"F");
        }
        if(map->number_room_stair == i){
            mvprintw(map->y_stair,map->x_stair,"<");
        }
        if(map->number_Password_Doors_room == i){
            mvprintw(map->y_create_paasword,map->x_create_paasword,"&");
        }
        if(map->number_Master_Key_room == i && (map->x_Master_Key != 0)){
            move(map->y_Master_Key,map->x_Master_Key);
            attron(COLOR_PAIR(3));
            printw("%s",r);
            attroff(COLOR_PAIR(3));
            refresh();
        }
    }
    for(int i = 0; i < map->corridor_count; i++){
        mvprintw(map->corridors[i].y_corrifer,map->corridors[i].x_corridor,"#");
    }
    refresh();
    getch();
    clear();
}

int is_spell_room(struct Map *map,int x,int y){
    for(int i = 0; i < map->number_of_rooms; i++) {
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x >= room_x1 && x <= room_x2 && y >= room_y1 && y <= room_y2) {
            if(i == map->number_Spell_room){
                return 1;
            }
        }
    }
    return 0;
}

int file_exists(char filename[100]) {
    char perfix[20];
    strcpy(perfix, "-map1.bin");
    strcat(filename,perfix);
    FILE *file;
    if((file = fopen(filename,"rb"))){
        fclose(file);
        return 1;
    }
    return 0;
}

void remove_file(char filename[100],int number){
    char perfix[20];
    switch (number)
    {
    case 1:
        strcpy(perfix, "-map1.bin");
        break;
    case 2:
        strcpy(perfix, "-map2.bin");
        break;
    case 3:
        strcpy(perfix, "-map3.bin");
        break;
    case 4:
        strcpy(perfix, "-map4.bin");
        break;
    case 5:
        strcpy(perfix, "-map5.bin");
        break;
    }
    strcat(filename,perfix);
    remove(filename);
}

void random_password(){
    char lower[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char captal[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char number[10] = {'0','1','2','3','4','5','6','7','8','9'};
    char password[20];
    int low_count = rand() % 4 + 3;
    int cap_count = rand() % 4 + 3;
    int num_count = rand() % 4 + 3;
    int size = 0;
    int pointer = 0;
    while(pointer != low_count){
        int indx = rand() % 26;
        password[size++] = lower[indx];
        pointer += 1;
    }
    pointer = 0;
    while(pointer != cap_count){
        int indx = rand() % 26;
        password[size++] = captal[indx];
        pointer += 1;
    }
    pointer = 0;
    while(pointer != num_count){
        int indx = rand() % 10;
        password[size++] = number[indx];
        pointer += 1;
    }
    move(10,10);
    printw("%s",password);
    refresh();
    return;
}

void show_password(char name[100]){
    FILE* players = fopen("Players.txt", "r");
    char name_existed[20];
    char password_existed[20];
    char email[20];
    int score,gild,number,time;
    while (fscanf(players, "%s %s %s %d %d %d %d", name_existed, password_existed,email,&score,&gild,&number,&time) != EOF) {
        if (strcmp(name, name_existed) == 0) {
            fclose(players);
            break;
        }
    }
    char s_password[4];
    for(int i = 0; i < 4; i++){
        s_password[i] = password_existed[i];
    }
    s_password[4] = '\0';
    move(10,10);
    printw("%s",s_password);
    return;
}

void* play_music(void* arg) {
    char* music_file = (char*)arg;
    music_playing = 1;
    char command[512];
    snprintf(command, sizeof(command), "cvlc --play-and-exit \"%s\" >/dev/null 2>&1 &", music_file);
    system(command);
    return NULL;
}

void stop_music() {
    if (music_playing) {
        system("pkill -f vlc");
        music_playing = 0;
    } else {
        mvprintw(0, 0, "No music is currently playing.");
        refresh();
    }
}

int music_choice() {
    clear();
    refresh();
    char *music_list[3] = {
    "look on down from the bridge.mp3",
    "between the bars.mp3",
    "bache nane.mp3"
    };
    mvprintw(0, 0, "Select a music:");
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(5, 40, 4, 4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", music_list[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", music_list[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
                current_item = (current_item == 0) ? 2 : current_item - 1;
                break;
            case KEY_DOWN:
                current_item = (current_item == 2) ? 0 : current_item + 1;
                break;
            case 10:
                delwin(menu);
                clear();
                refresh();
                return current_item;
        }
    }
}

void music_menu() {
    clear();
    refresh();
    label:
    const char* choices[3] = {
        "Select a music",
        "Stop the music",
        "Back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(6, 40, 4, 4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 3; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
                current_item = (current_item == 0) ? 2 : current_item - 1;
                break;
            case KEY_DOWN:
                current_item = (current_item == 2) ? 0 : current_item + 1;
                break;
            case 10:
                if (current_item == 0) {
                    int choice = music_choice();
                    pthread_t music_thread;
                    pthread_create(&music_thread, NULL, play_music, (void*)music_list[choice]);
                    goto label;
                } else if (current_item == 1) {
                    stop_music();
                    goto label;
                } else if (current_item == 2) {
                    delwin(menu);
                    return;
                }
                break;
        }
    }
}

int is_demon(struct Map *map,int x, int y){
    for(int i = 0; i < 5; i++){
        if(map->demons[i].x_demon == x && map->demons[i].y_demon == y){
            return 1;
        }
    }
    return 0;    
}

void move_demon(struct Map *map,struct Player *p){
    for(int i = 0; i < 5; i++){
        if(map->demons[i].x_demon + 1 == p->x && map->demons[i].y_demon == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon + 1 == p->x && map->demons[i].y_demon - 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon + 1 == p->x && map->demons[i].y_demon + 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon == p->x && map->demons[i].y_demon - 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon == p->x && map->demons[i].y_demon + 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon - 1 == p->x && map->demons[i].y_demon - 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon - 1 == p->x && map->demons[i].y_demon == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
        else if(map->demons[i].x_demon - 1 == p->x && map->demons[i].y_demon + 1 == p->y){
            first_damage = 1;
            if(map->demons[i].type == 1){
                p->health -= 10;
                time(&damage);
            }
            if(map->demons[i].type == 2){
                p->health -= 20;
                time(&damage);
            }
            if(map->demons[i].type == 3){
                p->health -= 30;
                time(&damage);
            }
            if(map->demons[i].type == 4){
                p->health -= 40;
                time(&damage);
            }
            if(map->demons[i].type == 5){
                p->health -= 50;
                time(&damage);
            }
            move(0,0);
            clrtoeol();
            printw("Helth: %d  Hunger: %d  Gold: %d",p->health,p->hungry,p->gold);
            refresh();
        }
    }
    for(int i = 0; i < 5; i++){
        if(map->demons[i].can_move && map->demons[i].x_demon != 0){
            int resualt_or_index_room = witch_room(map,p->x,p->y);
            if(resualt_or_index_room){
                if(map->demons[i].number_of_room == resualt_or_index_room - 1){
                    if(map->demons[i].type == 3 || map->demons[i].type == 5){
                        if(p->step <= 5){
                            if(map->demons[i].x_demon < p->x){
                                if(is_not_wall(map,map->demons[i].x_demon + 1,map->demons[i].y_demon) && !is_pillor(map,map->demons[i].x_demon + 1,map->demons[i].y_demon) && (map->demons[i].x_demon + 1 != p->x || map->demons[i].y_demon != p->y)){
                                    print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                    map->demons[i].x_demon += 1;
                                    continue;
                                }
                            }
                            if(map->demons[i].x_demon > p->x){
                                if(is_not_wall(map,map->demons[i].x_demon - 1,map->demons[i].y_demon) && !is_pillor(map,map->demons[i].x_demon - 1,map->demons[i].y_demon) && (map->demons[i].x_demon - 1 != p->x || map->demons[i].y_demon != p->y)){
                                    print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                    map->demons[i].x_demon -= 1;
                                    continue;
                                }
                            }
                            if(map->demons[i].y_demon < p->y){
                                if(is_not_wall(map,map->demons[i].x_demon,map->demons[i].y_demon + 1) && !is_pillor(map,map->demons[i].x_demon,map->demons[i].y_demon + 1) && (map->demons[i].x_demon != p->x || map->demons[i].y_demon + 1 != p->y)){
                                    print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                    map->demons[i].y_demon += 1;
                                    continue;
                                }
                            }
                            if(map->demons[i].y_demon > p->y){
                                if(is_not_wall(map,map->demons[i].x_demon,map->demons[i].y_demon - 1) && !is_pillor(map,map->demons[i].x_demon,map->demons[i].y_demon - 1) && (map->demons[i].x_demon != p->x || map->demons[i].y_demon - 1 != p->y)){
                                    print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                    map->demons[i].y_demon -= 1;
                                    continue;
                                }
                            }
                        }
                    }
                    if(map->demons[i].type == 4){
                        if(map->demons[i].x_demon < p->x){
                            if(is_not_wall(map,map->demons[i].x_demon + 1,map->demons[i].y_demon) && !is_pillor(map,map->demons[i].x_demon + 1,map->demons[i].y_demon) && (map->demons[i].x_demon + 1 != p->x || map->demons[i].y_demon != p->y)){
                                print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                map->demons[i].x_demon += 1;
                                continue;
                            }
                        }
                        if(map->demons[i].x_demon > p->x){
                            if(is_not_wall(map,map->demons[i].x_demon - 1,map->demons[i].y_demon) && !is_pillor(map,map->demons[i].x_demon - 1,map->demons[i].y_demon) && (map->demons[i].x_demon - 1 != p->x || map->demons[i].y_demon != p->y)){
                                print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                map->demons[i].x_demon -= 1;
                                continue;
                            }
                        }
                        if(map->demons[i].y_demon < p->y){
                            if(is_not_wall(map,map->demons[i].x_demon,map->demons[i].y_demon + 1) && !is_pillor(map,map->demons[i].x_demon,map->demons[i].y_demon + 1) && (map->demons[i].x_demon != p->x || map->demons[i].y_demon + 1 != p->y)){
                                print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                map->demons[i].y_demon += 1;
                                continue;
                            }
                        }
                        if(map->demons[i].y_demon > p->y){
                            if(is_not_wall(map,map->demons[i].x_demon,map->demons[i].y_demon - 1) && !is_pillor(map,map->demons[i].x_demon,map->demons[i].y_demon - 1) && (map->demons[i].x_demon != p->x || map->demons[i].y_demon - 1 != p->y)){
                                print_after_move_demon(map,map->demons[i].x_demon,map->demons[i].y_demon);
                                map->demons[i].y_demon -= 1;
                                continue;
                            }
                        }    
                    }
                }
            }
        }
    }
}

int witch_room(struct Map *map, int x, int y) {
    for(int i = 0; i < map->number_of_rooms; i++) {
        int room_x1 = map->rooms[i].x;
        int room_x2 = map->rooms[i].x + map->rooms[i].size;
        int room_y1 = map->rooms[i].y;
        int room_y2 = map->rooms[i].y + map->rooms[i].size;

        if(x >= room_x1 && x <= room_x2 && y >= room_y1 && y <= room_y2) {
            return i + 1;
        }
    }
    return 0;
}

void print_after_move_demon(struct Map *map,int x, int y){
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);
    if(map->number_of_rooms != 1 && x == map->x_create_paasword && y == map->y_create_paasword){
        move(y,x);
        printw("&");
        return;
    }
    if(map->number_of_rooms != 1 && x == map->x_stair && y == map->y_stair){
        move(y,x);
        printw("<");
        return;
    }
    if(map->number_of_rooms != 1 && x == map->x_Master_Key && y == map->y_Master_Key){
        move(y,x);
        attron(COLOR_PAIR(3));
        printw("%s",r);
        attroff(COLOR_PAIR(3));
        refresh();
        return;
    }
    for(int j = 0; j < 3; j++){
        if(map->number_of_rooms != 1 && x == map->foods[j].x_food && y == map->foods[j].y_food){
            move(map->foods[j].y_food,map->foods[j].x_food);
            if(map->foods[j].type == 1){
                printw("%s",l);
                return;
            }
            if(map->foods[j].type == 2){
                printw("%s",o);
                return;
            }
            if(map->foods[j].type == 3){
                printw("%s",u);
                return;
            }
            if(map->foods[j].type == 4){
                printw("%s",n);
                return;
            }
        }
    }
    for(int j = 0; j < 3; j++){
        if(map->number_of_rooms != 1 && x == map->spells[j].x_spell && y == map->spells[j].y_spell){
            move(map->spells[j].y_spell,map->spells[j].x_spell);
            if(map->spells[j].type == 1){
                printw("%s",f);
                return;
            }
            if(map->spells[j].type == 2){
                printw("%s",g);
                return;
            }
            if(map->spells[j].type == 3){
                printw("%s",h);
                return;
            }
        }
    }
    for(int j = 0; j < 3; j++){
        if(map->number_of_rooms != 1 && x == map->wepons[j].x_wepon && y == map->spells[j].y_spell){
            move(map->wepons[j].y_wepon,map->wepons[j].x_wepon);
            if(map->wepons[j].type == 1){
                printw("%s",a);
                return;
            }
            if(map->wepons[j].type == 2){
                printw("%s",b);
                return;
            }
            if(map->wepons[j].type == 3){
                printw("%s",c);
                return;
            }
            if(map->wepons[j].type == 4){
                printw("%s",d);
                return;
            }
            if(map->wepons[j].type == 5){
                printw("%s",e);
                return;
            }
        }
    }
    for(int j = 0; j < 3; j++){
        if(x == map->golds[j].x_gold && y == map->golds[j].y_gold){
            move(map->golds[j].y_gold,map->golds[j].x_gold);
            if(map->golds[j].is_black){
                printw("%s",v);
                return;
            }else{
                printw("%s",m);
                return;
            }
        }
    }
    move(y,x);
    printw(" ");
    return;
}

int have_wepon(struct Player *p,int type){
    for(int i = 0; i < p->wepon_number; i++){
        if(p->wepons[i].type == type){
            if(p->wepons[i].cllect > 0){
                return 1;
            }
        }
    }
    return 0;
}

int witch_demon(struct Map *map,int x,int y){
    for(int i = 0; i < 5; i++){
        if(map->demons[i].x_demon == x && map->demons[i].y_demon == y){
            return i;
        }
    }
}

char* name_of_demon(int type){
    if(type == 1){
        return "deamon";
    }
    if(type == 2){
        return "Monster Breathing Fire";
    }
    if(type == 3){
        return "Giant";
    }
    if(type == 4){
        return "Snake";
    }
    if(type == 5){
        return "Undeed";
    }
}

int is_wepon_use(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_wepons_use; i++){
        if(map->wepons_use[i].x_wepon == x && map->wepons_use[i].y_wepon == y){
            return 1;
        }
    }
    return 0;
}

int index_wepon_use(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_wepons_use; i++){
        if(map->wepons_use[i].x_wepon == x && map->wepons_use[i].y_wepon == y){
            return i;
        }
    }
}

void f_wepon_use(struct Map *map,int x, int y){
    for(int i = 0; i < map->number_of_wepons_use; i++){
        if(map->wepons_use[i].x_wepon == x && map->wepons_use[i].y_wepon == y){
            map->wepons_use[i].x_wepon = 0;
            map->wepons_use[i].y_wepon = 0;
            break;
        }
    }
}

void throw_wepon(int type,struct Map *map,struct Player *p,int is_repeat){
    int damage;
    int range;
    if(type == 2){
        damage = 12;
        range = 5;
    }
    if(type == 3){
        damage = 15;
        range = 10;
    }
    if(type == 4){
        damage = 5;
        range = 5;
    }
    int c2;
    if(is_repeat){
        c2 = last_throw;
    }else{
        c2 = getch();
    }
    if(c2 == 'w'){
        last_throw = 'w';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x,p->y - i) && !is_pillor(map,p->x,p->y - i) && !is_demon(map,p->x, p->y - i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x, p->y - distance_available - 1) || is_pillor(map,p->x,p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x, p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].x_demon,map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].y_demon);
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available - 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'd'){
        last_throw = 'd';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x + i,p->y) && !is_pillor(map,p->x + i,p->y) && !is_demon(map,p->x + i, p->y)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x + distance_available + 1, p->y) || is_pillor(map,p->x + distance_available + 1,p->y)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x + distance_available + 1, p->y)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].x_demon,map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].y_demon);
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].x_demon = 0;
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available + 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'x'){
        last_throw = 'x';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x,p->y + i) && !is_pillor(map,p->x,p->y + i) && !is_demon(map,p->x, p->y + i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x, p->y + distance_available + 1) || is_pillor(map,p->x,p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x, p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].x_demon,map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].y_demon);
                map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x,p->y + distance_available + 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available + 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'a'){
        last_throw = 'a';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x - i,p->y) && !is_pillor(map,p->x - i,p->y) && !is_demon(map,p->x - i, p->y)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x - distance_available - 1, p->y) || is_pillor(map,p->x - distance_available - 1,p->y)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x - distance_available - 1, p->y)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].x_demon,map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].y_demon);
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].x_demon = 0;
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available - 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'q'){
        last_throw = 'q';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x - i,p->y - i) && !is_pillor(map,p->x - i,p->y - i) && !is_demon(map,p->x - i, p->y - i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x - distance_available - 1, p->y - distance_available - 1) || is_pillor(map,p->x - distance_available - 1,p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x - distance_available - 1, p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].x_demon,map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].y_demon);
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y - distance_available - 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available - 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available - 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'e'){
        last_throw = 'e';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x + i,p->y - i) && !is_pillor(map,p->x + i,p->y - i) && !is_demon(map,p->x + i, p->y - i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x + distance_available + 1, p->y - distance_available - 1) || is_pillor(map,p->x + distance_available + 1,p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x + distance_available + 1, p->y - distance_available - 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].x_demon,map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].y_demon);
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y - distance_available - 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available + 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y - distance_available - 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'c'){
        last_throw = 'c';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x + i,p->y + i) && !is_pillor(map,p->x + i,p->y + i) && !is_demon(map,p->x + i, p->y + i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x + distance_available + 1, p->y + distance_available + 1) || is_pillor(map,p->x + distance_available + 1,p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x + distance_available + 1, p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].x_demon,map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].y_demon);
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x + distance_available + 1,p->y + distance_available + 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x + distance_available + 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available + 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
    if(c2 == 'z'){
        last_throw = 'z';
        int distance_available = 0;
        for(int i = 1; i <= range - 1; i++){
            if(is_not_wall(map,p->x - i,p->y + i) && !is_pillor(map,p->x - i,p->y + i) && !is_demon(map,p->x - i, p->y + i)){
                distance_available = i;
            }else{
                break;
            }
        }
        int found = 0;
        for(int i = 0; i < p->wepon_number; i++){
            if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                found = 1;
                break;
            }
        }
        if(!found){
            move(1,0);
            printw("Your weapon is finished!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(!is_not_wall(map,p->x - distance_available - 1, p->y + distance_available + 1) || is_pillor(map,p->x - distance_available - 1,p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
        if(is_demon(map,p->x - distance_available - 1, p->y + distance_available + 1)){
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            if(type == 3){
                map->demons[witch_demon(map,p->x,p->y - distance_available - 1)].can_move = 0;
            }
            map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].healt -= damage * coefficient_damage;
            if(map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].healt <= 0){
                move(1,0);
                printw("the wepon hit to %s and damage %d and kill it",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                print_after_move_demon(map,map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].x_demon,map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].y_demon);
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].x_demon = 0;
                map->demons[witch_demon(map,p->x - distance_available - 1,p->y + distance_available + 1)].y_demon = 0;
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }else{
                move(1,0);
                printw("the wepon hit to %s and damage %d",name_of_demon(type),damage);
                refresh();
                sleep(1);
                move(1,0);
                clrtoeol();
                refresh();
                move_demon(map,p);
                print(map,p->x,p->y,p);
                return;
            }
        }else{
            for(int i = 0; i < p->wepon_number; i++){
                if(p->wepons[i].type == type && p->wepons[i].cllect > 0){
                    p->wepons[i].cllect -= 1;
                    break;
                }
            }
            map->wepons_use[map->number_of_wepons_use].type = type;
            map->wepons_use[map->number_of_wepons_use].x_wepon = p->x - distance_available - 1;
            map->wepons_use[map->number_of_wepons_use].y_wepon = p->y + distance_available + 1;
            map->number_of_wepons_use += 1;
            move(1,0);
            printw("The wepon did not hit the enemy!");
            refresh();
            sleep(1);
            move(1,0);
            clrtoeol();
            refresh();
            move_demon(map,p);
            print(map,p->x,p->y,p);
            return;
        }
    }
}

int is_win(struct Map *map){
    for(int i = 0; i < 3; i++){
        if(map->golds[i].x_gold != 0){
            return 0;
        }
    }
    for(int i = 0; i < 5; i++){
        if(map->demons[i].x_demon != 0){
            return 0;
        }
    }
    return 1;
}

void chose_color(struct Player *p){
    const char *choices[4] = {
        "red",
		"green",
        "regular",
        "back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(7,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 4; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 4 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1 || current_item == 2){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				if(current_item == 0){
					p->color = 1;
					return;
				}
				if(current_item == 1){
					p->color = 2;
                    return;
				}
				if(current_item == 2){
                    p->color = 3;
                    return;
				}
				if(current_item == 3){
					return;
				}
                break;
        }
    }
}

void chose_level(struct Player *p){
    const char *choices[4] = {
        "easy",
		"medium",
        "hard",
        "back"
    };
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);
    WINDOW *menu = newwin(7,40,4,4);
    keypad(menu, TRUE);
    int current_item = 0;
    int ch;
    while (1) {
        for (int i = 0; i < 4; i++) {
            if (i == current_item) {
                wattron(menu, A_BOLD | COLOR_PAIR(2));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_BOLD | COLOR_PAIR(2));
            } else {
                wattron(menu, COLOR_PAIR(1));
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, COLOR_PAIR(1));
            }
        }
        wrefresh(menu);
        ch = wgetch(menu);
        switch (ch) {
            case KEY_UP:
				if(current_item == 0){
					current_item = 4 - 1;
				}else{
					current_item -= 1;
				}
                break;
            case KEY_DOWN:
				if(current_item == 0 || current_item == 1 || current_item == 2){
					current_item += 1;
				}else{
					current_item = 0;
				}
                break;
            case 10:
                refresh();
				clear();
				if(current_item == 0){
					p->level = 3;
                    p->health = p->level * 1000;
					return;
				}
				if(current_item == 1){
					p->level = 2;
                    p->health = p->level * 1000;
                    return;
				}
				if(current_item == 2){
                    p->level = 1;
                    p->health = p->level * 1000;
                    return;
				}
				if(current_item == 3){
					return;
				}
                break;
        }
    }
}

