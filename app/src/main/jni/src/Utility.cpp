// Utility.cpp: implementation of the Utility.
//
//////////////////////////////////////////////////////////////////////

#include "Utility.h"
#include "SharedDefine.h"

///////////////////////////////////////////////////////////////////////////////
// Debug Function
///////////////////////////////////////////////////////////////////////////////

char* Str_Format(char* pFormat, ...)
{	// ���ڿ� ���� ����� �Լ�
	static char szBuffer[MAX_BUFFER_LINE][260];
	static int nIndex = 0;
	// �ΰ��� ���۸� ����� ����� ���
	char* pBuffer = szBuffer[nIndex++&1];
	// �������ڰ��� ���ۿ� ����
	va_list vaList;
	va_start(vaList, pFormat);
	vsprintf(pBuffer, pFormat, vaList);
	va_end(vaList);
	return pBuffer;
}
