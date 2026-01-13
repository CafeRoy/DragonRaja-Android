#pragma once

#define MAX_KIND					10			//����
#define MAX_ONE_LINE_CHAR_NUM		4096		//���ٿ� �� �� �ִ� �ִ��� ����
#define MAX_FILE_LENGTH				2048		//txtȭ���� �ִ� ���� path�� �ִ밪
#include "SDL.h"

class CLanguage
{
private:	
	// 儲存 emoji 範圍的容器
	std::map<uint32_t, uint32_t> emoji_ranges;

	DWORD		AreaCode;

	char		LoadedFileName[MAX_FILE_LENGTH] = {0};
	
	char		szNullString[2] = {0};


	DWORD		m_dwTotalLength;						//DataAddress�� ������ ����.�� ������ ������ �� + m_dwSumOfTotalLine;

	DWORD		m_dwSumOfTotalLine;						//m_dwTotalLineOfKind�� ��
	DWORD		m_dwTotalLineOfKind[MAX_KIND] = {0};			//�� �������� ������ �ִ� ������ ��

	DWORD		m_dwSumOfMaxLineNumber;					//m_dwMaxLineNumberOfKind�� ��
	DWORD		m_dwMaxLineNumberOfKind[MAX_KIND] = {0};		//�� �������� ������ �ִ� ���� ū ��ȣ�� ����( ��ȣ )

	char*		DataAddress = nullptr;
	char*		IndexAddress = nullptr;	

	int			CheckArea();							//� �� ������ üũ�ϴ� �Լ�
	int			LoadingByAreaCode();					//AreaCode�� ���ؼ� �ش��ϴ� ȭ���� �о� ���δ�.

	int			Preloading(SDL_RWops* fp);							//�󸶸�ŭ�� �޸𸮰� �ʿ����� ����Ѵ�
	int			Loading(SDL_RWops* fp);								//�޸𸮸� ��� �ش��ϴ� ���� �ؽ�Ʈ�� ���� �ִ´�.

	int			CheckHeader(uint32_t& Header, char* data);				//�����ΰ��� �Ǵ�	.���° ��ȣ���� ��ȯ
	int			CheckNumber(uint32_t& Number, char* data);				//��ȣ�� ������� Ȯ���Ѵ�.��ȣ ��ȯ
	int			CheckSentence(void * sentence,char* data);			//������ �󸶳� �䰡�� Ȯ���Ѵ�.������ ���� ��ȯ

	int			GetOneLine(void * data,SDL_RWops* fp);

public:
	char		ReturnBuffer[MAX_ONE_LINE_CHAR_NUM];
	char*		OutputMessage(int NumOfSentence);	//�ش��ϴ� �޽����� Sentence���� �ְ� ������ Sentence���� �ƹ����� ����.
	char*		OutputMessage(int Kind,int Num);	//�ش��ϴ� �޽����� Sentence���� �ְ� ������ Sentence���� �ƹ����� ����.

	int			Init();

	void		DisplayAllData();

	bool is_emoji(uint32_t code_point);

	bool contains_emoji_in_name(const std::string& name);

	bool load_emoji_data(const std::string& filename);

	CLanguage(int lang);
	~CLanguage();
};

typedef CLanguage LanguagePack;

extern LanguagePack* lan;
extern bool LoadLanguageText(int lang);