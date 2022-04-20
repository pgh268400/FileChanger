#include <stdio.h>

#include <dirent.h> //dirent 구조체를 위해 dirent.h 참조 
#include <string.h>
#include <locale.h>

#define ID_LEN 1000

int count = 0;
const char ascii_art[] = "______  _  _         _____  _                                      \n|  ___|(_)| |       /  __ \\| |                                     \n| |_    _ | |  ___  | /  \\/| |__    __ _  _ __    __ _   ___  _ __ \n|  _|  | || | / _ \\ | |    | '_ \\  / _` || '_ \\  / _` | / _ \\| '__|\n| |    | || ||  __/ | \\__/\\| | | || (_| || | | || (_| ||  __/| |   \n\\_|    |_||_| \\___|  \\____/|_| |_| \\__,_||_| |_| \\__, | \\___||_|   \n                                                  __/ |            \n                                                 |___/\n";

void draw_border() {
	printf("--------------------------------------------------------------------\n");
}

//확장자 가져오기 
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

//해당 경로의 파일 갯수를 return 하는 함수 
int get_file_count(char * file_path){
	int length = 0;
	DIR *d;
	struct dirent *dir;
	d = opendir(file_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG ) { //파일인것만 가져온다. dirent.h 변경 필수
				length++;
			}
		}
		closedir(d);
	}
	return length;
}

//문자열 치환 함수 
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

//경로를 arg[0]으로 받고 arg[1]에 입력된 char 포인터 배열에 저장한다. 
void get_file_name(char * file_path, char * file_list[]) {
	//배열의 경우 Call by reference임을 유의 
	//malloc으로 배열 공간을 할당하고 char포인터로 가리키게 한다. == 배열 
	
	int idx = 0;
	
	DIR *d;
	struct dirent *dir;
	d = opendir(file_path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_REG) { //파일인것만 가져온다. dirent.h 변경 필수
				strcpy(file_list[idx], dir->d_name);   //문자열 복사 
				idx++;
			}
		}
		closedir(d);
		//열린 디렉토리를 닫는다.
	} else {
		printf("Error : 디렉토리를 열 수 없습니다.\n");
	}
}

//경로 끝에 역슬래시 문자를 체크하고 없으면 삽입해줌. 
void insert_last_symbol(char * path){
	int length = strlen(path);
	if(path[length-1] != '\\'){ //끝이 역슬래시로 끝나지 않으면
		strcat(path, "\\"); //끝에 역슬래시를 붙여준다. 
	}
}

//파일 이름 치환 변경 함수 
//base_path : C:역슬래시, file_list : 파일목록, find_str : 찾을 문자열, replace_str : 치환시킬 문자열 
void replace_file_name(char * base_path, char * file_list[], char * find_str, char * replace_str){
	for(int i = 0; i < count; i++){
		char default_path[300] = ""; //기존 경로 저장할 배열 
		strcat(default_path, base_path);  //base_path를 붙여준다. 
		strcat(default_path, file_list[i]); //다시 file_list를 붙여준다. 
		
		char replaced_path[300] = ""; //치환된 배열 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다. 
		file_list[i] = no_ext; //파일리스트에서 반영해준다. 
		
		char * s2 = replaceAll(file_list[i], find_str, replace_str); //파일리스트에서 replace를 해주고 만들어진 문자열을 포인터로 가리킨다.
		file_list[i] = s2; //파일리스트에 반영 
		
		strcat(file_list[i], ext); //작업이 완료되었으면 확장자 붙여준다. 
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, s2);

		 if(rename(default_path,replaced_path)==0)
		  printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"파일 %s의 이름을 변경하는데 실패했습니다.\n",default_path);
	}
}

//파일 이름 오른쪽에 문자열 삽입
void insert_right_file_name(char * base_path, char * file_list[], char * insert_str){
	for(int i = 0; i < count; i++){
		char default_path[1000] = ""; //기존 경로 저장할 배열 
		strcat(default_path, base_path);  //base_path를 붙여준다. 
		strcat(default_path, file_list[i]); //다시 file_list를 붙여준다. 
		
		char replaced_path[1000] = ""; //치환된 배열 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다. 
		file_list[i] = no_ext; //파일리스트에서 반영해준다. 
		
		strcat(file_list[i], insert_str); //문자열 오른쪽에 삽입
		strcat(file_list[i], ext);
		printf("변경된 파일 리스트 : %s\n", file_list[i]);
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, file_list[i]);
		
		 if(rename(default_path,replaced_path)==0)
		  printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"파일 %s의 이름을 변경하는데 실패했습니다.\n",default_path);
	}
}

//파일 이름 왼쪽에 문자열 삽입 
void insert_left_file_name(char * base_path, char * file_list[], char * insert_str){
	for(int i = 0; i < count; i++){
		char default_path[1000] = ""; //기존 경로 저장할 배열 
		strcat(default_path, base_path);  //base_path를 붙여준다. 
		strcat(default_path, file_list[i]); //다시 file_list를 붙여준다. 
		
		char replaced_path[1000] = ""; //변경된 배열 
		
		char * ext = getExt(file_list[i]);
		char * no_ext = replaceAll(file_list[i], ext, ""); //파일리스트에서 확장자는 걸러낸다. 
		file_list[i] = no_ext; //파일리스트에서 반영해준다. 
		
		char cpy_insert[1000]; //삽입할 문자열 복사용 
		strcpy(cpy_insert, insert_str);
		strcat(cpy_insert, file_list[i]); //문자열 왼쪽에 삽입
		strcat(cpy_insert, ext); //확장자도 넣어주고... 
		
		strcpy(file_list[i], cpy_insert); //파일리스트에 반영 
		
		strcat(replaced_path, base_path);
		strcat(replaced_path, cpy_insert);
		
		 if(rename(default_path,replaced_path)==0)
		  printf("%s가\n %s로 성공적으로 이름이 변경되었습니다.\n", default_path, replaced_path);
		 else
		  fprintf(stderr,"파일 %s의 이름을 변경하는데 실패했습니다.\n",default_path);
	}
}



int main(void) {
	setlocale(LC_ALL, ""); //한글 경로 읽기 위해 로케일을 해당 컴퓨터에 있는 로케일로 바꾼다. (필수)
	 
	char file_path[200];
	//첫 시작화면 렌더링 
	draw_border();
	printf("%s\n", ascii_art);
	printf("간편 파일이름 변경기 v0.1\n");
	draw_border();
	
	printf("폴더 경로를 입력해주세요. : ");
	//scanf로 받으면 띄어쓰기를 받을 수 없어서 gets 사용 

	gets(file_path);
	
	//끝에 역슬래시 기호 삽입 
	insert_last_symbol(file_path); 
	
	draw_border();
	
	
	count = get_file_count(file_path);
	printf("찾은 파일 갯수 : %d개\n", count);
	
	if (count != 0) //파일갯수가 0개 이상이면 
	{
		//파일 갯수만큼 파일리스트 동적할당  (효율성을 위해 포인터 배열로 만든다.)
		char ** file_list = malloc(count * sizeof(char*));
		for (int i = 0; i < count; i++)
			file_list[i] = malloc((ID_LEN+1) * sizeof(char)); 
		
		//Call by Reference
		get_file_name(file_path, file_list);
		
		//파일 리스트 출력 
		for (int i = 0; i < count; i++){
			printf("%s\n", file_list[i]);
		}
		
		draw_border();
		
		char mode;
		
		do{
		printf("작업하실 모드를 선택해주세요 (p : 치환모드, l : 왼쪽에 문자열 삽입, r : 오른쪽에 문자열 삽입, e : 종료) : ");
		scanf(" %c", &mode); 
		getchar(); // scanf의 버퍼를 비워준다. 
		
		
		if (mode == 'p'){ 
			char s1[100], s2[100], dialog[100];
			printf("찾을 문자열을 입력해주세요 : ");
			gets(s1);
			printf("치환할 문자열을 입력해주세요 : ");
			gets(s2);
			
			printf("해당 경로의 \"%s\" 문자를 찾아서 \"%s\" (으)로 치환해서 이름을 변경합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1, s2);
			scanf(" %s", dialog);
			
			if(strcmp(dialog, "y") == 0){
				replace_file_name(file_path, file_list,  s1, s2); 
			} else {
				printf("사용자에 의해 작업이 취소되었습니다.\n");
			}
		} else if (mode == 'e') {
			printf("사용자에 의해 작업이 종료되었습니다.");
		} else if (mode == 'l'){
			char s1[100], dialog[100];
			printf("삽입할 문자열을 입력해주세요 : ");
			gets(s1);
			
			printf("해당 경로의 파일 이름 앞에 \"%s\" 을 모두 삽입합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1);
			gets(dialog);
			
			if(strcmp(dialog, "y") == 0){
				insert_left_file_name(file_path, file_list, s1);
			} else {
				printf("사용자에 의해 작업이 취소되었습니다.\n");
			}
			
		} else if(mode == 'r'){
			char s1[100], dialog[100];
			printf("삽입할 문자열을 입력해주세요 : ");
			gets(s1);
			
			printf("해당 경로의 파일 이름 끝에 \"%s\" 을 모두 삽입합니다. \n정말 수행하시겠습니까 ? (y/n) : ", s1);
			gets(dialog);
			
			if(strcmp(dialog, "y") == 0){
				insert_right_file_name(file_path, file_list, s1);
			} else {
				printf("사용자에 의해 작업이 취소되었습니다.\n");
			}
		} else {
			printf("올바른 모드가 아닙니다.");
		}
		} while(mode != 'e');
	free(file_list); //프로그램 종료시 동적할당된 메모리 제거 
	}
	return 0;
}
