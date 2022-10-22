// 헤더 파일 중복 선언 방지
#pragma once

#define FILE_NAME_SIZE 500 //파일 이름의 최대 길이

#define ASCII_ART "______  _  _         _____  _                                      \n|  ___|(_)| |       /  __ \\| |                                     \n| |_    _ | |  ___  | /  \\/| |__    __ _  _ __    __ _   ___  _ __ \n|  _|  | || | / _ \\ | |    | '_ \\  / _` || '_ \\  / _` | / _ \\| '__|\n| |    | || ||  __/ | \\__/\\| | | || (_| || | | || (_| ||  __/| |   \n\\_|    |_||_| \\___|  \\____/|_| |_| \\__,_||_| |_| \\__, | \\___||_|   \n                                                  __/ |            \n                                                 |___/\n"

// 경계선 그리는 함수
void draw_border();

//확장자 가져오기
char *getExt(char *file_list);

//해당 경로의 파일 갯수를 return 하는 함수
int get_file_count(char *file_path);

// file_list를 출력하는 함수
void print_file_list(char **file_list, int count);

//문자열 치환 함수
char *replaceAll(char *orig, char *rep, char *with);

//경로를 arg[0]으로 받고 arg[1]에 입력된 char 포인터 배열에 저장한다.
void get_file_name(char *file_path, char **file_list);

//경로 끝에 역슬래시 문자를 체크하고 없으면 삽입해줌.
void insert_last_symbol(char *path);

//파일 이름 치환 변경 함수
// base_path : C:역슬래시, file_list : 파일목록, find_str : 찾을 문자열, replace_str : 치환시킬 문자열
void replace_file_name(char *base_path, char **file_list, char *find_str, char *replace_str, int count);

//파일 이름 오른쪽에 문자열 삽입
void insert_right_file_name(char *base_path, char **file_list, char *insert_str, int count);

//파일 이름 왼쪽에 문자열 삽입
void insert_left_file_name(char *base_path, char **file_list, char *insert_str, int count);

char **dynamic_string_array(int row, int col);