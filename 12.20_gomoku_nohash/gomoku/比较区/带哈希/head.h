#define FLOOR 6
#define FLOOR_VCX 3
//每次更改FLOOR，需要将board.c中的priority, best_score_of_upper, not_in_the_same_branch这三个数组的大小改一下
#define Consecutive_Five 10000000//连五
#define Open_Four 1000000//活四
#define Double_Chess 900000//双活三、双冲四、冲四活三等
#define Open_Three 100000//活三
#define Gapped_Two_Two 100050//下面三种都是冲四，这种是两个2子之间缺了一个子  ●●？●●
#define Capped_Four 100050//这种冲四是一头被堵住的冲四   ○●●●●和●●●●○
#define Gapped_Four 100050//这种冲四是一个活三与一个单独的子之间空了一格  ●?●●●
#define Gapped_Three 99950//这种是跳活三，是即将形成活四的三  ●？●●
#define Double_Gapped_Three 100020//这种是双跳活三_●_●●_●_，分值应该比活三高一点，比冲四低一点
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
#define infinity 89999900

void myprintf(int array[], int length);
void initial_board();
void DrawBoard(long int value, int mode_choice, int step_count);
int mode_choose(int mode_choice);
void chess_play_ver2(int step_count);
long int evaluation(int step_count, bool my_turn, int raw, int column);
void return_to_normal_chess(int step_count);
bool judgement(int step_count);
void pvp(long int value);
void get_coordinate(int step_count);
void init_genrand64(unsigned long long seed);
void init_by_array64(unsigned long long init_key[], unsigned long long key_length);
unsigned long long genrand64_int64(void);
int indexOf(int chess);
void initTable();
long int Searching_Hashing(int step_count, bool my_turn, long temp_score, bool write);
void pve(long int value);
int offensive();
void auto_play(int chess, int opponent_chess);
long int Minimax3(int step_count, bool my_turn, int floor);
void quick_sort(int temp_priority[][3], int l, int r);
long int deepest(int step_count, bool my_turn);
void shallowest(int step_count, bool my_turn);
int before_evaluation_ver4(int priority_ver2[][2], int floor, int step_count, bool my_turn);
long int line(bool state[], int vector[], int raw, int column, int step_count);
void init_best_score_of_upper();