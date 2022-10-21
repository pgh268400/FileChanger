#include "dirent.h" //dirent 구조체를 위해 dirent.h 참조
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void draw_border()
{
    printf("--------------------------------------------------------------------\n");
}

//확장자 가져오기
char *getExt(char *file_list)
{
    static char buf[FILE_NAME_SIZE] = "";
    char *ptr = NULL;

    ptr = strrchr(file_list, '.');
    if (ptr == NULL)
        return "";

    strcpy(buf, ptr);

    return buf;
}

//해당 경로의 파일 갯수를 return 하는 함수
int get_file_count(char *file_path)
{
    int length = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(file_path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            { //파일인것만 가져온다. dirent.h 변경 필수
                length++;
            }
        }
        closedir(d);
    }
    return length;
}

void print_file_list(char **file_list, int count)
{
    //파일 리스트 출력
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", file_list[i]);
    }
}

//문자열 치환 함수

/*
char* replaceAll(char* s, const char* olds, const char* news)
{
    char* result, * sr;
    size_t i, count = 0;
    size_t oldlen = strlen(olds);
    if (oldlen < 1)
        return s;
    size_t newlen = strlen(news);

    if (newlen != oldlen)
    {
        for (i = 0; s[i] != '\0';)
        {
            if (memcmp(&s[i], olds, oldlen) == 0)
                count++, i += oldlen;
            else
                i++;
        }
    }
    else
        i = strlen(s);

    result = (char*)malloc(i + 1 + count * (newlen - oldlen));
    if (result == NULL)
        return NULL;

    sr = result;
    while (*s)
    {
        if (memcmp(s, olds, oldlen) == 0)
        {
            memcpy(sr, news, newlen);
            sr += newlen;
            s += oldlen;
        }
        else
            *sr++ = *s++;
    }
    *sr = '\0';

    return result;
}
*/

// You must free the result if result is non-NULL.
char *replaceAll(char *orig, char *rep, char *with)
{
    char *result;  // the return string
    char *ins;     // the next insert point
    char *tmp;     // varies
    int len_rep;   // length of rep (the string to remove)
    int len_with;  // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;     // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count)
    {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--)
    {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

//경로를 arg[0]으로 받고 arg[1]에 입력된 char 포인터 배열에 저장한다.
void get_file_name(char *file_path, char **file_list)
{
    //배열의 경우 Call by reference임을 유의
    // malloc으로 배열 공간을 할당하고 char포인터로 가리키게 한다. == 배열

    int idx = 0;

    DIR *d;
    struct dirent *dir;
    d = opendir(file_path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {                                        //파일인것만 가져온다. dirent.h 변경 필수
                strcpy(file_list[idx], dir->d_name); //문자열 복사
                idx++;
            }
        }
        closedir(d);
        //열린 디렉토리를 닫는다.
    }
    else
    {
        printf("Error : 디렉토리를 열 수 없습니다.\n");
    }
}

//경로 끝에 역슬래시 문자를 체크하고 없으면 삽입해줌.
void insert_last_symbol(char *path)
{
    int length = strlen(path);
    if (path[length - 1] != '\\')
    {                       //끝이 역슬래시로 끝나지 않으면
        strcat(path, "\\"); //끝에 역슬래시를 붙여준다.
    }
}

//파일 이름 치환 변경 함수
// base_path : C:역슬래시, file_name_list : 파일목록, find_str : 찾을 문자열, replace_str : 치환시킬 문자열
void replace_file_name(char *base_path, char **file_name_list, char *find_str, char *replace_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //기존 경로 저장할 배열
        strcat(default_path, base_path);         // base_path를 붙여준다.
        strcat(default_path, file_name_list[i]); //다시 file_list를 붙여준다.

        char replaced_path[FILE_NAME_SIZE] = ""; //치환된 배열

        char *ext = getExt(file_name_list[i]);

        // print ext
        // printf("ext : %s\n", ext);

        // 확장자 존재시
        if (strlen(ext) != 0)
        {

            char *no_ext = replaceAll(file_name_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //파일리스트에서 반영해준다. (no repointing)
            free(no_ext);
        }

        // 파일 이름에 치환할 문자열이 존재해야지만 작업 수행
        if (strstr(file_name_list[i], find_str) != NULL)
        {

            // printf("%s의 경로에서 %s를 찾아서 %s로 치환합니다.", file_name_list[i], find_str, replace_str);

            char *s2 = replaceAll(file_name_list[i], find_str, replace_str); //파일리스트에서 replace를 해주고 만들어진 문자열을 포인터로 가리킨다.
            // free(file_name_list[i]);
            // file_name_list[i] = s2;

            //파일리스트에 반영
            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], s2); //파일리스트에서 반영해준다. (no repointing)

            strcat(file_name_list[i], ext); //작업이 완료되었으면 확장자 붙여준다.

            strcat(replaced_path, base_path);
            strcat(replaced_path, s2);

            if (rename(default_path, replaced_path) == 0)
                printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
            else
                fprintf(stderr, "파일 %s의 이름을 변경하는데 실패했습니다.\n", default_path);

            free(s2);
            // contains
        }
    }
}

//파일 이름 오른쪽에 문자열 삽입
void insert_right_file_name(char *base_path, char **file_name_list, char *insert_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //기존 경로 저장할 배열 (1)
        strcat(default_path, base_path);         // base_path를 붙여준다.
        strcat(default_path, file_name_list[i]); // file_name을 붙여준다.

        char replaced_path[FILE_NAME_SIZE] = ""; //치환된 배열 (2)

        char *ext = getExt(file_name_list[i]);

        char *no_ext = NULL;
        if (strlen(ext) != 0)
        {
            // 확장자 존재시
            no_ext = replaceAll(file_name_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다.
                                                             //// replaceAll 은 인자로 들어온 문자열에서 find_str을 replace_str로 치환 후 힙에 동적할당된 공간을 반환한다.
                                                             // file_name_list[i] 에 포인팅 위치를 replaceAll 이 반환한 내용으로 바꿀것이므로 기존에 file_name_list 공간에 메모리를 해제해준다.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //파일리스트에서 반영해준다. (no repointing)
            free(no_ext);
        }

        strcat(file_name_list[i], insert_str); //문자열 오른쪽에 삽입
        strcat(file_name_list[i], ext);
        // printf("변경된 파일 리스트 : %s\n", file_name_list[i]);

        strcat(replaced_path, base_path);
        strcat(replaced_path, file_name_list[i]);

        // default_path 출력
        printf("default_path : %s\n", default_path);

        // replaced_path 출력
        printf("replaced_path : %s\n", replaced_path);

        if (rename(default_path, replaced_path) == 0)
            printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
        else
            fprintf(stderr, "파일 %s의 이름을 변경하는데 실패했습니다.\n", default_path);
    }
}

//파일 이름 왼쪽에 문자열 삽입
void insert_left_file_name(char *base_path, char **file_name_list, char *insert_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //기존 경로 저장할 배열
        strcat(default_path, base_path);         // base_path를 붙여준다.
        strcat(default_path, file_name_list[i]); //다시 file_list를 붙여준다.

        char replaced_path[FILE_NAME_SIZE] = ""; //변경된 배열

        char *ext = getExt(file_name_list[i]);
        // char *no_ext = replaceAll(file_name_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다.

        // 확장자 존재시
        if (strlen(ext) != 0)
        {
            char *no_ext = replaceAll(file_name_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //파일리스트에서 반영해준다. (no repointing)
            free(no_ext);
        }

        char cpy_insert[FILE_NAME_SIZE]; //삽입할 문자열 복사용
        strcpy(cpy_insert, insert_str);
        strcat(cpy_insert, file_name_list[i]); //문자열 왼쪽에 삽입
        strcat(cpy_insert, ext);               //확장자도 넣어주고...

        strcpy(file_name_list[i], cpy_insert); //파일리스트에 반영

        strcat(replaced_path, base_path);
        strcat(replaced_path, cpy_insert);

        if (rename(default_path, replaced_path) == 0)
            printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
        else
            fprintf(stderr, "파일 %s의 이름을 변경하는데 실패했습니다.\n", default_path);
    }
}

char **dynamic_string_array(int row, int col)
{
    // 문자열 배열 동적 할당 (heap에 할당하므로 나중에 사용하는쪽에서 free()
    // 필수)
    char **result = (char **)malloc(sizeof(char *) * row);
    for (int i = 0; i < row; i++)
    {
        result[i] = (char *)malloc(sizeof(char) * col);
    }
    return result;
}