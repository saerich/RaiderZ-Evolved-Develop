#ifndef MUID_H
#define MUID_H

#include <map>
#include <vector>
#include <crtdbg.h>
#include <iostream>
#include "RLib.h"
#include "MTypes.h"

using namespace std;

/// MAIET Unique ID
struct CML2_API MUID{
	union {
		struct 
		{
			unsigned long int	Low;	///< Low 4 Byte
			unsigned long int	High;	///< High 4 Byte
		};
		uint64 Value;
	};

	MUID(void) { Value = 0; }
	MUID(uint64 i) : Value(i) { }
	MUID(unsigned long int h, unsigned long int l)
	{
		High = h;
		Low = l;
	}

	/// MUID 0���� �ʱ�ȭ
	void SetZero(void){
		Value = 0;
	}
	void SetInvalid(void)
	{
		SetZero();
	}

	/// MUID�� nSize��ŭ ����
	MUID Increase(uint64 nSize=1){
		Value += nSize;
		return *this;
	}

	/// MUID�� ��ȿ���� �˻�, 0:0�̸� Invalid
	bool IsInvalid(void) const {
		// 0�̸� Invalid
		if(Value == 0) return true;
		return false;
	}
	/// MUID�� ��ȿ���� �˻�, 0:0�̸� Invalid
	bool IsValid(void) const {
		// 0�̸� Invalid
		if(Value == 0) return false;
		return true;
	}

	/// ũ�� �񱳸� ���� ���۷����� �����ε�
	inline friend bool operator > (const MUID& a, const MUID& b){
		if(a.Value > b.Value) return true;
		return false;
	}
	/// ũ�� �񱳸� ���� ���۷����� �����ε�
	inline friend bool operator >= (const MUID& a, const MUID& b){
		if(a.Value >= b.Value) return true;
		return false;
	}
	/// ũ�� �񱳸� ���� ���۷����� �����ε�
	inline friend bool operator < (const MUID& a, const MUID& b){
		if(a.Value < b.Value) return true;
		return false;
	}
	/// ũ�� �񱳸� ���� ���۷����� �����ε�
	inline friend bool operator <= (const MUID& a, const MUID& b){
		if(a.Value <= b.Value) return true;
		return false;
	}

	/// Stream�� ���� ���۷����� �����ε�
	inline friend ostream& operator<<(ostream& stream, const MUID& rhs){
		stream << rhs.Value;
		return stream;
	}
	
	inline friend wostream& operator<<(wostream& stream, const MUID& rhs){
		stream << rhs.Value;
		return stream;
	}

	/// ���� ���۷����� �����ε�
	inline MUID& operator=(uint64 v){
		Value = v;
		return *this;
	}
	/// ���� ���۷����� �����ε�
	inline MUID& operator=(const MUID& a){
		Value = a.Value;
		return *this;
	}

	/// �� ���۷����� �����ε�
	inline friend bool operator==(const MUID& a, const MUID& b){
		if(a.Value==b.Value) return true;
		return false;
	}
	/// �� ���۷����� �����ε�
	inline friend bool operator!=(const MUID& a, const MUID& b){
		if(a.Value!=b.Value) return true;
		return false;
	}

	MUID operator+() const { return MUID(Value);	}
	MUID operator+(const MUID& a) const 
	{ 
		return MUID(Value + a.Value);	
	}
	MUID& operator+=(const MUID& a)	
	{ 
		Value += a.Value;
		return *this; 
	}
	MUID& operator++()		{ Value++; return (*this); }
	const MUID operator++(int)
	{ 
		const MUID oldValue = *this; 
		++(*this);
		return oldValue; 
	}

	MUID operator-() const { return MUID(Value);  }
	MUID operator-(const MUID& a) const 
	{ 
		return MUID(Value - a.Value);	
	}
	MUID& operator-=(const MUID& a)	
	{ 
		Value -= a.Value;
		return *this; 
	}
	MUID& operator--()		{ Value--; return (*this); }
	const MUID operator--(int)
	{ 
		const MUID oldValue = *this; 
		--(*this);
		return oldValue; 
	}

	/// Invalid�� UID ���
		static MUID Invalid(void);

	static const MUID ZERO;
};

/// MUID ����
struct MUIDRANGE{
	MUID	Start;
	MUID	End;

	bool IsEmpty(void){
		return (Start==End);
	}
	void Empty(void){
		SetZero();
	}
	void SetZero(void){
		Start.SetZero();
		End.SetZero();
	}
};

/// MUID Reference Map
/// - ���������� 1�� �����ϴ� ID�� �� �� �ִ� MUID ����
class MUIDRefMap : protected map<MUID, void*>{
	MUID	m_CurrentMUID;		///< ���� �߱޵� MUID
public:
	MUIDRefMap(void);
	virtual ~MUIDRefMap(void);

	/// ���۷����� MUID�ϳ��� �Ҵ��Ѵ�.
	/// @param pRef	���۷��� ������
	/// @return		�Ҵ�� MUID
	MUID Generate(void* pRef);

	/// MUID�� ���� ���۷��� �����͸� ����.
	/// @param uid	MUID
	/// @return		���۷��� ������, MUID�� �������� ������ NULL�� ����
	void* GetRef(MUID& uid);

	/// ��ϵ� MUID ����.
	/// @param uid	MUID
	/// @return		��ϵǾ��� ���۷��� ������, MUID�� �������� ������ NULL�� ����
	void* Remove(MUID& uid);

	/// nSize��ŭ Ȯ��
	MUIDRANGE Reserve(int nSize);

	/// ���ݱ��� �߱޵� UID ����
	MUIDRANGE GetReservedCount(void);
};

/// MUID Reference Map Cache
/// - ��ü UID�� �˻� ���ϸ� ���̱� ���� ĳ�� Ŭ����
class MUIDRefCache : public map<MUID, void*>{
public:
	MUIDRefCache(void);
	virtual ~MUIDRefCache(void);

	/// ���۷����� MUID�ϳ��� �Ҵ��Ѵ�.
	/// @param pRef	���۷��� ������
	/// @return		�Ҵ�� MUID
	void Insert(const MUID& uid, void* pRef);
	/// MUID�� ���� ���۷��� �����͸� ����.
	/// @param uid	MUID
	/// @return		���۷��� ������, MUID�� �������� ������ NULL�� ����
	void* GetRef(const MUID& uid);
	/// ��ϵ� MUID ����.
	/// @param uid	MUID
	/// @return		��ϵǾ��� ���۷��� ������, MUID�� �������� ������ NULL�� ����
	void* Remove(const MUID& uid);
};

/// MUID ������
class CML2_API MUIDGenerator
{
private:
	MUID	m_CurrentMUID;
public:
	MUIDGenerator(MUID uidSeed=MUID::ZERO) : m_CurrentMUID(uidSeed) {}
	~MUIDGenerator() {}
	void SetSeed(MUID uidSeed) { m_CurrentMUID = uidSeed; }
	MUID Generate()
	{
		m_CurrentMUID.Increase();
		return m_CurrentMUID;
	}
};

/// Random UID Generator
class CML2_API MUIDRandomGenerator
{
private:
	uint32 m_nSeed;
public:
	MUIDRandomGenerator(uint32 nSeed) : m_nSeed(nSeed) {}
	MUID Generate();
};

/// MUID���ٴ� ����(4����Ʈ) ID
typedef unsigned long int		MID;


#endif