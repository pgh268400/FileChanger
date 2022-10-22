#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

// ������ ���� �����ϴ� ���� ����
int count = 0;

int main(void)
{
	setlocale(LC_ALL, ""); //�ѱ� ��� �б� ���� �������� �ش� ��ǻ�Ϳ� �ִ� �����Ϸ� �ٲ۴�. (�ʼ�)

	char file_path[200];
	//ù ����ȭ�� ������
	draw_border();
	printf("%s\n", ASCII_ART);
	printf("���� �����̸� ����� v0.2\n");
	draw_border();

	printf("���� ��θ� �Է����ּ���. : ");
	// scanf�� ������ ���⸦ ���� �� ��� gets ���

	gets(file_path);

	//���� �������� ��ȣ ����
	insert_last_symbol(file_path);

	draw_border();

	count = get_file_count(file_path);
	printf("ã�� ���� ���� : %d��\n", count);

	if (count != 0) //���ϰ����� 0�� �̻��̸�
	{
		// ���� ����(col)��ŭ ���� ����Ʈ (���ڿ� �迭) �����Ҵ�
		// �����쿡�� ���� �̸� ���ڴ� 260 ���ڰ� �ִ�. �� �ι��ڱ��� ����� row�� �ִ� + 1 ����Ʈ ��ŭ ����ָ� �ȴ�.
		char **file_list = dynamic_string_array(count, FILE_NAME_SIZE + 1);

		// Call by Reference
		// file_path ��ο��� ���� �а� file_list�� �����ϴ� �Լ�
		get_file_name(file_path, file_list);

		//���� ����Ʈ ���
		print_file_list(file_list, count);

		draw_border();

		char mode = 0;

		do
		{
			printf("�۾��Ͻ� ��带 �������ּ��� (p : ġȯ���, l : ���ʿ� ���ڿ� ����, r : �����ʿ� ���ڿ� ����, e : ����) : ");
			scanf(" %c", &mode);
			getchar(); // scanf�� ���۸� ����ش�.

			if (mode == 'p')
			{
				char s1[FILE_NAME_SIZE], s2[FILE_NAME_SIZE], dialog[FILE_NAME_SIZE];
				printf("ã�� ���ڿ��� �Է����ּ��� : ");
				gets(s1);
				printf("ġȯ�� ���ڿ��� �Է����ּ��� : ");
				gets(s2);

				printf("�ش� ����� \"%s\" ���ڸ� ã�Ƽ� \"%s\" (��)�� ġȯ�ؼ� �̸��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1, s2);
				scanf(" %s", dialog);

				if (strcmp(dialog, "y") == 0)
				{
					replace_file_name(file_path, file_list, s1, s2, count);
				}
				else
				{
					printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
				}
			}
			else if (mode == 'e')
			{
				printf("����ڿ� ���� �۾��� ����Ǿ����ϴ�.");
			}
			else if (mode == 'l')
			{
				char s1[100], dialog[100];
				printf("������ ���ڿ��� �Է����ּ��� : ");
				gets(s1);

				printf("�ش� ����� ���� �̸� �տ� \"%s\" �� ��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1);
				gets(dialog);

				if (strcmp(dialog, "y") == 0)
				{
					insert_left_file_name(file_path, file_list, s1, count);
				}
				else
				{
					printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
				}
			}
			else if (mode == 'r')
			{
				char s1[100], dialog[100];
				printf("������ ���ڿ��� �Է����ּ��� : ");
				gets(s1);

				printf("�ش� ����� ���� �̸� ���� \"%s\" �� ��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1);
				gets(dialog);

				if (strcmp(dialog, "y") == 0)
				{
					insert_right_file_name(file_path, file_list, s1, count);
				}
				else
				{
					printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
				}
			}
			else
			{
				printf("�ùٸ� ��尡 �ƴմϴ�.");
			}

			draw_border();
			print_file_list(file_list, count);
			draw_border();
		} while (mode != 'e');

		//���α׷� ����� �����Ҵ�� �޸� ����
		for (int i = 0; i < count; i++)
		{
			free(file_list[i]);
		}
		free(file_list);
	}
	return 0;
}
