#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

// 파일의 갯수 관리하는 전역 변수
int count = 0;

int main(void)
{
	setlocale(LC_ALL, ""); //한글 경로 읽기 위해 로케일을 해당 컴퓨터에 있는 로케일로 바꾼다. (필수)

	char file_path[200];
	//첫 시작화면 렌더링
	draw_border();
	printf("%s\n", ASCII_ART);
	printf("간편 파일이름 변경기 v0.2\n");
	draw_border();

	printf("폴더 경로를 입력해주세요. : ");
	// scanf로 받으면 띄어쓰기를 받을 수 없어서 gets 사용

	gets(file_path);

	//끝에 역슬래시 기호 삽입
	insert_last_symbol(file_path);

	draw_border();

	count = get_file_count(file_path);
	printf("찾은 파일 갯수 : %d개\n", count);

	if (count != 0) //파일갯수가 0개 이상이면
	{
		// 파일 갯수(col)만큼 파일 리스트 (문자열 배열) 동적할당
		// 윈도우에서 파일 이름 글자는 260 글자가 최대. 즉 널문자까지 고려해 row는 최대 + 1 바이트 만큼 잡아주면 된다.
		char **file_list = dynamic_string_array(count, FILE_NAME_SIZE + 1);

		// Call by Reference
		// file_path 경로에서 파일 읽고 file_list에 저장하는 함수
		get_file_name(file_path, file_list);

		//파일 리스트 출력
		print_file_list(file_list, count);

		draw_border();

		char mode = 0;

		do
		{
			printf("작업하실 모드를 선택해주세요 (p : 치환모드, l : 왼쪽에 문자열 삽입, r : 오른쪽에 문자열 삽입, e : 종료) : ");
			scanf(" %c", &mode);
			getchar(); // scanf의 버퍼를 비워준다.

			if (mode == 'p')
			{
				char s1[FILE_NAME_SIZE], s2[FILE_NAME_SIZE], dialog[FILE_NAME_SIZE];
				printf("찾을 문자열을 입력해주세요 : ");
				gets(s1);
				printf("치환할 문자열을 입력해주세요 : ");
				gets(s2);

				printf("해당 경로의 \"%s\" 문자를 찾아서 \"%s\" (으)로 치환해서 이름을 변경합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1, s2);
				scanf(" %s", dialog);

				if (strcmp(dialog, "y") == 0)
				{
					replace_file_name(file_path, file_list, s1, s2, count);
				}
				else
				{
					printf("사용자에 의해 작업이 취소되었습니다.\n");
				}
			}
			else if (mode == 'e')
			{
				printf("사용자에 의해 작업이 종료되었습니다.");
			}
			else if (mode == 'l')
			{
				char s1[100], dialog[100];
				printf("삽입할 문자열을 입력해주세요 : ");
				gets(s1);

				printf("해당 경로의 파일 이름 앞에 \"%s\" 을 모두 삽입합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1);
				gets(dialog);

				if (strcmp(dialog, "y") == 0)
				{
					insert_left_file_name(file_path, file_list, s1, count);
				}
				else
				{
					printf("사용자에 의해 작업이 취소되었습니다.\n");
				}
			}
			else if (mode == 'r')
			{
				char s1[100], dialog[100];
				printf("삽입할 문자열을 입력해주세요 : ");
				gets(s1);

				printf("해당 경로의 파일 이름 끝에 \"%s\" 을 모두 삽입합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1);
				gets(dialog);

				if (strcmp(dialog, "y") == 0)
				{
					insert_right_file_name(file_path, file_list, s1, count);
				}
				else
				{
					printf("사용자에 의해 작업이 취소되었습니다.\n");
				}
			}
			else
			{
				printf("올바른 모드가 아닙니다.");
			}

			draw_border();
			print_file_list(file_list, count);
			draw_border();
		} while (mode != 'e');

		//프로그램 종료시 동적할당된 메모리 제거
		for (int i = 0; i < count; i++)
		{
			free(file_list[i]);
		}
		free(file_list);
	}
	return 0;
}
