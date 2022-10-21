#include "dirent.h" //dirent ����ü�� ���� dirent.h ����
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void draw_border()
{
    printf("--------------------------------------------------------------------\n");
}

//Ȯ���� ��������
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

//�ش� ����� ���� ������ return �ϴ� �Լ�
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
            { //�����ΰ͸� �����´�. dirent.h ���� �ʼ�
                length++;
            }
        }
        closedir(d);
    }
    return length;
}

void print_file_list(char **file_list, int count)
{
    //���� ����Ʈ ���
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", file_list[i]);
    }
}

//���ڿ� ġȯ �Լ�

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

//��θ� arg[0]���� �ް� arg[1]�� �Էµ� char ������ �迭�� �����Ѵ�.
void get_file_name(char *file_path, char **file_list)
{
    //�迭�� ��� Call by reference���� ����
    // malloc���� �迭 ������ �Ҵ��ϰ� char�����ͷ� ����Ű�� �Ѵ�. == �迭

    int idx = 0;

    DIR *d;
    struct dirent *dir;
    d = opendir(file_path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {                                        //�����ΰ͸� �����´�. dirent.h ���� �ʼ�
                strcpy(file_list[idx], dir->d_name); //���ڿ� ����
                idx++;
            }
        }
        closedir(d);
        //���� ���丮�� �ݴ´�.
    }
    else
    {
        printf("Error : ���丮�� �� �� �����ϴ�.\n");
    }
}

//��� ���� �������� ���ڸ� üũ�ϰ� ������ ��������.
void insert_last_symbol(char *path)
{
    int length = strlen(path);
    if (path[length - 1] != '\\')
    {                       //���� �������÷� ������ ������
        strcat(path, "\\"); //���� �������ø� �ٿ��ش�.
    }
}

//���� �̸� ġȯ ���� �Լ�
// base_path : C:��������, file_name_list : ���ϸ��, find_str : ã�� ���ڿ�, replace_str : ġȯ��ų ���ڿ�
void replace_file_name(char *base_path, char **file_name_list, char *find_str, char *replace_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //���� ��� ������ �迭
        strcat(default_path, base_path);         // base_path�� �ٿ��ش�.
        strcat(default_path, file_name_list[i]); //�ٽ� file_list�� �ٿ��ش�.

        char replaced_path[FILE_NAME_SIZE] = ""; //ġȯ�� �迭

        char *ext = getExt(file_name_list[i]);

        // print ext
        // printf("ext : %s\n", ext);

        // Ȯ���� �����
        if (strlen(ext) != 0)
        {

            char *no_ext = replaceAll(file_name_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //���ϸ���Ʈ���� �ݿ����ش�. (no repointing)
            free(no_ext);
        }

        // ���� �̸��� ġȯ�� ���ڿ��� �����ؾ����� �۾� ����
        if (strstr(file_name_list[i], find_str) != NULL)
        {

            // printf("%s�� ��ο��� %s�� ã�Ƽ� %s�� ġȯ�մϴ�.", file_name_list[i], find_str, replace_str);

            char *s2 = replaceAll(file_name_list[i], find_str, replace_str); //���ϸ���Ʈ���� replace�� ���ְ� ������� ���ڿ��� �����ͷ� ����Ų��.
            // free(file_name_list[i]);
            // file_name_list[i] = s2;

            //���ϸ���Ʈ�� �ݿ�
            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], s2); //���ϸ���Ʈ���� �ݿ����ش�. (no repointing)

            strcat(file_name_list[i], ext); //�۾��� �Ϸ�Ǿ����� Ȯ���� �ٿ��ش�.

            strcat(replaced_path, base_path);
            strcat(replaced_path, s2);

            if (rename(default_path, replaced_path) == 0)
                printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
            else
                fprintf(stderr, "���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n", default_path);

            free(s2);
            // contains
        }
    }
}

//���� �̸� �����ʿ� ���ڿ� ����
void insert_right_file_name(char *base_path, char **file_name_list, char *insert_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //���� ��� ������ �迭 (1)
        strcat(default_path, base_path);         // base_path�� �ٿ��ش�.
        strcat(default_path, file_name_list[i]); // file_name�� �ٿ��ش�.

        char replaced_path[FILE_NAME_SIZE] = ""; //ġȯ�� �迭 (2)

        char *ext = getExt(file_name_list[i]);

        char *no_ext = NULL;
        if (strlen(ext) != 0)
        {
            // Ȯ���� �����
            no_ext = replaceAll(file_name_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����.
                                                             //// replaceAll �� ���ڷ� ���� ���ڿ����� find_str�� replace_str�� ġȯ �� ���� �����Ҵ�� ������ ��ȯ�Ѵ�.
                                                             // file_name_list[i] �� ������ ��ġ�� replaceAll �� ��ȯ�� �������� �ٲܰ��̹Ƿ� ������ file_name_list ������ �޸𸮸� �������ش�.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //���ϸ���Ʈ���� �ݿ����ش�. (no repointing)
            free(no_ext);
        }

        strcat(file_name_list[i], insert_str); //���ڿ� �����ʿ� ����
        strcat(file_name_list[i], ext);
        // printf("����� ���� ����Ʈ : %s\n", file_name_list[i]);

        strcat(replaced_path, base_path);
        strcat(replaced_path, file_name_list[i]);

        // default_path ���
        printf("default_path : %s\n", default_path);

        // replaced_path ���
        printf("replaced_path : %s\n", replaced_path);

        if (rename(default_path, replaced_path) == 0)
            printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
        else
            fprintf(stderr, "���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n", default_path);
    }
}

//���� �̸� ���ʿ� ���ڿ� ����
void insert_left_file_name(char *base_path, char **file_name_list, char *insert_str, int count)
{
    for (int i = 0; i < count; i++)
    {
        char default_path[FILE_NAME_SIZE] = "";  //���� ��� ������ �迭
        strcat(default_path, base_path);         // base_path�� �ٿ��ش�.
        strcat(default_path, file_name_list[i]); //�ٽ� file_list�� �ٿ��ش�.

        char replaced_path[FILE_NAME_SIZE] = ""; //����� �迭

        char *ext = getExt(file_name_list[i]);
        // char *no_ext = replaceAll(file_name_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����.

        // Ȯ���� �����
        if (strlen(ext) != 0)
        {
            char *no_ext = replaceAll(file_name_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����.

            memset(file_name_list[i], 0, sizeof(file_name_list[i]));
            strcpy(file_name_list[i], no_ext); //���ϸ���Ʈ���� �ݿ����ش�. (no repointing)
            free(no_ext);
        }

        char cpy_insert[FILE_NAME_SIZE]; //������ ���ڿ� �����
        strcpy(cpy_insert, insert_str);
        strcat(cpy_insert, file_name_list[i]); //���ڿ� ���ʿ� ����
        strcat(cpy_insert, ext);               //Ȯ���ڵ� �־��ְ�...

        strcpy(file_name_list[i], cpy_insert); //���ϸ���Ʈ�� �ݿ�

        strcat(replaced_path, base_path);
        strcat(replaced_path, cpy_insert);

        if (rename(default_path, replaced_path) == 0)
            printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
        else
            fprintf(stderr, "���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n", default_path);
    }
}

char **dynamic_string_array(int row, int col)
{
    // ���ڿ� �迭 ���� �Ҵ� (heap�� �Ҵ��ϹǷ� ���߿� ����ϴ��ʿ��� free()
    // �ʼ�)
    char **result = (char **)malloc(sizeof(char *) * row);
    for (int i = 0; i < row; i++)
    {
        result[i] = (char *)malloc(sizeof(char) * col);
    }
    return result;
}