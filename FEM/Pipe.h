#define NOMINMAX
#include "windows.h"
#include "ATLComTime.h"
class CPipeException
{
public:
	CPipeException(int nummer) { Fehlernummer = nummer; }
	~CPipeException() {};
	int Fehlernummer;
};

class CPipe
{
public:
	CPipe(void);
	~CPipe(void);
	int PipeTest;
	bool VerbindePipes(LPCTSTR einName, LPCTSTR ausName);
	void GetI(long int &value);
	void GetI2(short &value);
	void GetF(float &value);
	void GetD(double &value);
	void PutI(long int value);
	void PutI2(short value);
	void PutF(float value);
	void PutD(double value);
	void PutS(char *value);
	void GetS(char *value);
	void PutTime(COleDateTime zeit);
	void GetTime(COleDateTime &zeit);
	HANDLE ein;
	HANDLE aus;
	virtual bool Exit();
	CString m_Fehlertext;
private:
	HANDLE ConnectPipe(LPCTSTR name, DWORD access);
	void GetBytes(void *buffer, DWORD length);
	void PutBytes(void *buffer, DWORD length);
};
