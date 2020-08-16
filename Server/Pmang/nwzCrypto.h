#ifdef NWZ_CRYPTO_LIB
#else
#define NWZ_CRYPTO_LIB extern "C" __declspec(dllimport)
#endif

//BLOWFISH=0,DES=1

// �����ڵ带 ����ϴ� ���
// encryptedBuffer = NULL , encryptedBufferSize = 0 �� �Է��ϸ� �ʿ��� encryptedBuffer ũ�⸦ ����
NWZ_CRYPTO_LIB int encryptPwdUC(const TCHAR * plain, const int plainSize, TCHAR * encryptedBuffer,int encryptedBufferSize, int alg=0);
// plainBuffer = NULL , plainBufferSize = 0 �� �Է��ϸ� �ʿ��� plainBuffer ũ�⸦ ����
NWZ_CRYPTO_LIB int decryptPwdUC(const TCHAR * encrypted, const int encryptedSize, TCHAR * plainBuffer, int plainBufferSize, int alg=0);

// ANSI char�� ����ϴ� ���
// encrypted = NULL , encryptedSize = 0 �� �Է��ϸ� �ʿ��� encrypted ũ�⸦ ����
NWZ_CRYPTO_LIB int encryptPwd(const char * plain, const int plainSize, char * encrypted, int encryptedSize, int alg=0);
// plain = NULL , plainSize = 0 �� �Է��ϸ� �ʿ��� plain ũ�⸦ ����
NWZ_CRYPTO_LIB int decryptPwd(const char * encrypted, const int encryptedSize, char * plain, int plainSize, int alg=0);