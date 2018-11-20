#define FLOOR 5
#define FLOOR_VCX 3
//每次更改FLOOR，需要将board.c中的priority, best_score_of_upper, not_in_the_same_branch这三个数组的大小改一下
#define Consecutive_Five 10000000//连五
#define Open_Four 1000000//活四
#define Open_Three 100000//活三
#define Gapped_Two_Two 100050//下面三种都是冲四，这种是两个2子之间缺了一个子  ●●？●●
#define Capped_Four 100050//这种冲四是一头被堵住的冲四   ○●●●●和●●●●○
#define Gapped_Four 100050//这种冲四是一个活三与一个单独的子之间空了一格  ●?●●●
#define Gapped_Three 99950//这种是跳活三，是即将形成活四的三  ●？●●
#define Capped_Three 10000//眠三, 最多只能形成冲四  ○●●●
//需要连二的打分吗？ 大概1000分
/*眠三一共有这几种形态：
1. ○●●●__
2. ○●●_●_
3. ○●_●●_
4. ●●__●
5. ●_●_●
6. ○_●●●_○
*/
#define Open_two 1000//连二
#define depth_of_hashing 100000
void DrawBoard(char board[][17][3], int i, long int value, int mode_choice, int coordinate[], int step_count);
void chess_play(char **board, int step_count);
bool judgement(char board[][17][3], int step_count);
long int evaluation(char board[][17][3], int step_count, bool my_turn, int raw, int column);
void get_coordinate(int coordinate[], char board[][17][3], int step_count);
void chess_play_ver2(char board[][17][3], int step_count, int coordinate[]);
long int Minimax2(char board[][17][3], int step_count, bool my_turn, bool ai_first, int floor, int coordinate[], long int best_score_of_upper[], int priority[][26][2], bool not_in_the_same_branch[], unsigned long long hashValue, unsigned long long ZobristTable[15][15][2], unsigned long long hashing_value2[depth_of_hashing][3], int fatal_priority[][32][2], long int fatal_best_score_of_upper[], bool fatal_not_in_the_same_branch[]);
void auto_play(char board[][17][3], char chess[], char opponent_chess[], int coordinate[]);
bool verify_coordinate(char board[][17][3], int raw, int column, char chess, char opponent_chess);
bool before_evaluation(char board[][17][3], int priority[][26][2], int floor, int step_count, bool my_turn);
void return_to_normal_chess(char board[][17][3], int step_count, int coordinate[]);
void initialize_hashing_sheet(long long int key[][15][2]);
long int Zobrist_hashing(long long int hashing_value[][3], long long int key[][15][2], int best_raw, int best_column, bool write_new_coordinate, int step_count, char board[][17][3], bool my_turn, long long int hashing_value_now);
bool find_fatal_point(char board[][17][3], int fatal_priority[][32][2], int floor_vcx, int step_count, bool my_turn);
long int fatal_step(char board[][17][3], int step_count, bool my_turn, bool ai_first, int floor_vcx, int coordinate[], long int fatal_best_score_of_upper[], int fatal_priority[][32][2], bool fatal_not_in_the_same_branch[]);
void init_genrand64(unsigned long long seed);
unsigned long long genrand64_int64(void);
int indexOf(char chess[3]);
void initTable(unsigned long long ZobristTable[15][15][2]);
unsigned long long computeHash(char board2[15][17][3], unsigned long long ZobristTable[15][15][2]);
long int Searching_Hashing(unsigned long long hashing_value2[][3], unsigned long long ZobristTable[15][15][2], int step_count, unsigned long long hashValue, bool my_turn, long temp_score, bool write);
void init_genrand64(unsigned long long seed);
int before_evaluation_ver2(char board[][17][3], int priority[][26][2], int floor, int step_count, bool my_turn);