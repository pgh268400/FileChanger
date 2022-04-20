#include <stdio.h>

#include <dirent.h> //dirent ����ü�� ���� dirent.h ���� 
#include <string.h>
#include <locale.h>

#define ID_LEN 1000

int count = 0;
const char ascii_art[] = "______  _  _         _____  _                                      \n|  ___|(_)| |       /  __ \\| |                                     \n| |_    _ | |  ___  | /  \\/| |__    __ _  _ __    __ _   ___  _ __ \n|  _|  | || | / _ \\ | |    | '_ \\  / _` || '_ \\  / _` | / _ \\| '__|\n| |    | || ||  __/ | \\__/\\| | | || (_| || | | || (_| ||  __/| |   \n\\_|    |_||_| \\___|  \\____/|_| |_| \\__,_||_| |_| \\__, | \\___||_|   \n                                                  __/ |            \n                                                 |___/\n";

void draw_border() {
	printf("--------------------------------------------------------------------\n");
}

//Ȯ���� �������� 
char* getExt(char* file_list)
{
	static char buf[200] = "";
	char* ptr = NULL;
 
	ptr = strrchr(file_list, '.');
	if (ptr == NULL)
		return NULL;
 
	strcpy(buf, ptr);
 
	return buf;
}

//�ش� ����� ���� ������ return �ϴ� �Լ� 
int get_file_count(char * file_path){
	int length = 0;
	DIR *d;
	struct dirent *dir;
	d = opendir(file_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG ) { //�����ΰ͸� �����´�. dirent.h ���� �ʼ�
				length++;
			}
		}
		closedir(d);
	}
	return length;
}

//���ڿ� ġȯ �Լ� 
char *replaceAll(char *s, const char *olds, const char *news) {
  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);


  if (newlen != oldlen) {
	for (i = 0; s[i] != '\0';) {
	  if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
	  else i++;
	}
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
	if (memcmp(s, olds, oldlen) == 0) {
	  memcpy(sr, news, newlen);
	  sr += newlen;
	  s  += oldlen;
	} else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}

//��θ� arg[0]���� �ް� arg[1]�� �Էµ� char ������ �迭�� �����Ѵ�. 
void get_file_name(char * file_path, char * file_list[]) {
	//�迭�� ��� Call by reference���� ���� 
	//malloc���� �迭 ������ �Ҵ��ϰ� char�����ͷ� ����Ű�� �Ѵ�. == �迭 
	
	int idx = 0;
	
	DIR *d;
	struct dirent *dir;
	d = opendir(file_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) { //�����ΰ͸� �����´�. dirent.h ���� �ʼ�
				strcpy(file_list[idx], dir->d_name);   //���ڿ� ���� 
				idx++;
			}
		}
		closedir(d);
		//���� ���丮�� �ݴ´�.
	} else {
		printf("Error : ���丮�� �� �� �����ϴ�.\n");
	}
}

//��� ���� �������� ���ڸ� üũ�ϰ� ������ ��������. 
void insert_last_symbol(char * path){
	int length = strlen(path);
	if(path[length-1] != '\\'){ //���� �������÷� ������ ������
		strcat(path, "\\"); //���� �������ø� �ٿ��ش�. 
	}
}

//���� �̸� ġȯ ���� �Լ� 
//base_path : C:��������, file_list : ���ϸ��, find_str : ã�� ���ڿ�, replace_str : ġȯ��ų ���ڿ� 
void replace_file_name(char * base_path, char * file_list[], char * find_str, char * replace_str){
	for(int i = 0; i < count; i++){
		char default_path[300] = ""; //���� ��� ������ �迭 
		strcat(default_path, base_path);  //base_path�� �ٿ��ش�. 
		strcat(default_path, file_list[i]); //�ٽ� file_list�� �ٿ��ش�. 
		
		char replaced_path[300] = ""; //ġȯ�� �迭 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����. 
		file_list[i] = no_ext; //���ϸ���Ʈ���� �ݿ����ش�. 
		
		char * s2 = replaceAll(file_list[i], find_str, replace_str); //���ϸ���Ʈ���� replace�� ���ְ� ������� ���ڿ��� �����ͷ� ����Ų��.
		file_list[i] = s2; //���ϸ���Ʈ�� �ݿ� 
		
		strcat(file_list[i], ext); //�۾��� �Ϸ�Ǿ����� Ȯ���� �ٿ��ش�. 
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, s2);

		 if(rename(default_path,replaced_path)==0)
		  printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n",default_path);
	}
}

//���� �̸� �����ʿ� ���ڿ� ����
void insert_right_file_name(char * base_path, char * file_list[], char * insert_str){
	for(int i = 0; i < count; i++){
		char default_path[1000] = ""; //���� ��� ������ �迭 
		strcat(default_path, base_path);  //base_path�� �ٿ��ش�. 
		strcat(default_path, file_list[i]); //�ٽ� file_list�� �ٿ��ش�. 
		
		char replaced_path[1000] = ""; //ġȯ�� �迭 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����. 
		file_list[i] = no_ext; //���ϸ���Ʈ���� �ݿ����ش�. 
		
		strcat(file_list[i], insert_str); //���ڿ� �����ʿ� ����
		strcat(file_list[i], ext);
		printf("����� ���� ����Ʈ : %s\n", file_list[i]);
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, file_list[i]);
		
		 if(rename(default_path,replaced_path)==0)
		  printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n",default_path);
	}
}

//���� �̸� ���ʿ� ���ڿ� ���� 
void insert_left_file_name(char * base_path, char * file_list[], char * insert_str){
	for(int i = 0; i < count; i++){
		char default_path[1000] = ""; //���� ��� ������ �迭 
		strcat(default_path, base_path);  //base_path�� �ٿ��ش�. 
		strcat(default_path, file_list[i]); //�ٽ� file_list�� �ٿ��ش�. 
		
		char replaced_path[1000] = ""; //����� �迭 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //���ϸ���Ʈ���� Ȯ���ڴ� �ɷ�����. 
		file_list[i] = no_ext; //���ϸ���Ʈ���� �ݿ����ش�. 
		
		char cpy_insert[1000]; //������ ���ڿ� ����� 
		strcpy(cpy_insert, insert_str);
		strcat(cpy_insert, file_list[i]); //���ڿ� ���ʿ� ����
		strcat(cpy_insert, ext); //Ȯ���ڵ� �־��ְ�... 
		
		strcpy(file_list[i], cpy_insert); //���ϸ���Ʈ�� �ݿ� 
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, cpy_insert);
		
		 if(rename(default_path,replaced_path)==0)
		  printf("%s��\n %s�� ���������� �̸��� ����Ǿ����ϴ�.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"���� %s�� �̸��� �����ϴµ� �����߽��ϴ�.\n",default_path);
	}
}



int main(void) {
	setlocale(LC_ALL, ""); //�ѱ� ��� �б� ���� �������� �ش� ��ǻ�Ϳ� �ִ� �����Ϸ� �ٲ۴�. (�ʼ�)
	 
	char file_path[200];
	//ù ����ȭ�� ������ 
	draw_border();
	printf("%s\n", ascii_art);
	printf("���� �����̸� ����� v0.1\n");
	draw_border();
	
	printf("���� ��θ� �Է����ּ���. : ");
	//scanf�� ������ ���⸦ ���� �� ��� gets ��� 

	gets(file_path);
	
	//���� �������� ��ȣ ���� 
	insert_last_symbol(file_path); 
	
	draw_border();
	
	
	count = get_file_count(file_path);
	printf("ã�� ���� ���� : %d��\n", count);
	
	if (count != 0) //���ϰ����� 0�� �̻��̸� 
	{
		//���� ������ŭ ���ϸ���Ʈ �����Ҵ�  (ȿ������ ���� ������ �迭�� �����.)
		char ** file_list = malloc(count * sizeof(char*));
		for (int i = 0; i < count; i++)
			file_list[i] = malloc((ID_LEN+1) * sizeof(char)); 
		
		//Call by Reference
		get_file_name(file_path, file_list);
		
		//���� ����Ʈ ��� 
		for (int i = 0; i < count; i++){
			printf("%s\n", file_list[i]);
		}
		
		draw_border();
		
		char mode;
		
		do{
		printf("�۾��Ͻ� ��带 �������ּ��� (p : ġȯ���, l : ���ʿ� ���ڿ� ����, r : �����ʿ� ���ڿ� ����, e : ����) : ");
		scanf(" %c", &mode); 
		getchar(); // scanf�� ���۸� ����ش�. 
		
		
		if (mode == 'p'){ 
			char s1[100], s2[100], dialog[100];
			printf("ã�� ���ڿ��� �Է����ּ��� : ");
			gets(s1);
			printf("ġȯ�� ���ڿ��� �Է����ּ��� : ");
			gets(s2);
			
			printf("�ش� ����� \"%s\" ���ڸ� ã�Ƽ� \"%s\" (��)�� ġȯ�ؼ� �̸��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1, s2);
			scanf(" %s", dialog);
			
			if(strcmp(dialog, "y") == 0){
				replace_file_name(file_path, file_list,  s1, s2); 
			} else {
				printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
			}
		} else if (mode == 'e') {
			printf("����ڿ� ���� �۾��� ����Ǿ����ϴ�.");
		} else if (mode == 'l'){
			char s1[100], dialog[100];
			printf("������ ���ڿ��� �Է����ּ��� : ");
			gets(s1);
			
			printf("�ش� ����� ���� �̸� �տ� \"%s\" �� ��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1);
			gets(dialog);
			
			if(strcmp(dialog, "y") == 0){
				insert_left_file_name(file_path, file_list, s1);
			} else {
				printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
			}
			
		} else if(mode == 'r'){
			char s1[100], dialog[100];
			printf("������ ���ڿ��� �Է����ּ��� : ");
			gets(s1);
			
			printf("�ش� ����� ���� �̸� ���� \"%s\" �� ��� �����մϴ�. \n���� �����Ͻðڽ��ϱ� ? (y/n) : ", s1);
			gets(dialog);
			
			if(strcmp(dialog, "y") == 0){
				insert_right_file_name(file_path, file_list, s1);
			} else {
				printf("����ڿ� ���� �۾��� ��ҵǾ����ϴ�.\n");
			}
		} else {
			printf("�ùٸ� ��尡 �ƴմϴ�.");
		}
		} while(mode != 'e');
	free(file_list); //���α׷� ����� �����Ҵ�� �޸� ���� 
	}
	return 0;
}
