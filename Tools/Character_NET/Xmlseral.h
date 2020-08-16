#pragma once

#using <System.dll>
#using <System.Xml.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "frmMyValue.h"

using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Xml::Serialization;
using namespace System::Xml::Schema;
using namespace System::ComponentModel;
using namespace System::Globalization;
using namespace System::Drawing;
using namespace System::Drawing::Design;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::Design;

namespace XmlSerializable
{
	//----------------------------------------------------------------------------//
	// ��Ʈ�� Ŭ����
	// ��� �ٿ� ����
	public ref class DropDownStringConverter: public StringConverter
	{
	public:
		virtual bool GetStandardValuesSupported(ITypeDescriptorContext^ context) override
		{
			return true;
		}

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return true;
		}
	};

	public ref class DropDownEnumConverter: public EnumConverter
	{
	public:
		DropDownEnumConverter(Type^ T) : EnumConverter(T)
		{

		}

		virtual bool GetStandardValuesSupported(ITypeDescriptorContext^ context) override
		{
			return true;
		}

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return true;
		}
	};

	public ref class DropDownIntConverter: public Int32Converter
	{
	public:
		virtual bool GetStandardValuesSupported(ITypeDescriptorContext^ context) override
		{
			return true;
		}

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return true;
		}
	};

	public ref class DropDownUIConverter : public System::Drawing::Design::UITypeEditor
	{
	public:
		virtual System::Drawing::Design::UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::DropDown;
		}
	};

	// ���
	public ref class ModalUIConverter : public System::Drawing::Design::UITypeEditor
	{
	public:
		virtual System::Drawing::Design::UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::Modal;
		}
	};
	//----------------------------------------------------------------------------//

	public ref class TALENT
	{
	public: TALENT(){}

	private: String^ _id;
	private: String^ _name;
	private: String^ _desc;
	private: String^ _rank;
	private: String^ _skillline;
	private: String^ _HPCost;
	private: String^ _APCost;
	private: String^ _ENCost;
	private: String^ _STACost;
	private: String^ _HPTickCost;
	private: String^ _ENTickCost;
	private: String^ _STATickCost;
	private: String^ _CoolTime;
	private: String^ _Stance;
	private: String^ _UseAniName;
	private: String^ _Type;
	private: String^ _AttackDirection;
	private: String^ _AlTime;
	private: String^ _MF_BEATEN;
	private: String^ _MF_WEIGHT_BEATEN;
	private: String^ _MF_KNOCKBACK;
	private: String^ _MF_WEIGHT_KNOCKBACK;
	private: String^ _MF_STUN;
	private: String^ _MF_WEIGHT_STUN;
	private: String^ _MF_DEFLECT;
	private: String^ _MF_WEIGHT_DEFLECT;
	private: String^ _MF_THROWUP;
	private: String^ _MF_WEIGHT_THROWUP;
	private: String^ _MF_KNOCKDOWN;
	private: String^ _MF_WEIGHT_KNOCKDOWN;
	private: String^ _Damage;
	private: String^ _HitTestType;
	private: String^ _HitRate;
	private: String^ _HitMotionDelay;
	private: String^ _EfficacyType;
	private: String^ _EfficacyRadius;
	private: String^ _NeededTP;
	private: String^ _ReqLevel;
	private: String^ _TalentLine;
	private: String^ _TalentType;
	private: String^ _SkillType;
	private: String^ _MF_UPPERED;
	private: String^ _MF_WEIGHT_UPPERED;
	private: String^ _Guardable;
	private: String^ _Cancelable;
	private: String^ _TargetsAllowed;

	private: String^ _CastingAni;
	private: String^ _CastingLoopAni;
	private: String^ _CastingTime;
	private: String^ _ProjectileType;
	private: String^ _ProjectileSpeed;
	private: String^ _ProjectileRange;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("id"),
			ReadOnlyAttribute(true)]
		//[XmlAttributeAttribute("id")]
		//[XmlAttribute("id")]
		property String^ ���̵�
		{
			String^	get() {return _id;}
			void	set(String^ value) {_id=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("name"),
			ReadOnlyAttribute(true)]
		property String^ �̸�
		{
			String^	get() {return _name;}
			void	set(String^ value) {_name=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Rank"),
			ReadOnlyAttribute(true)]
		property String^ ��ũ
		{
			String^	get() {return _rank;}
			void	set(String^ value) {_rank=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("SkillLine"),
			ReadOnlyAttribute(true)]
		property String^ SkillLine
		{
			String^	get() {return _skillline;}
			void	set(String^ value) {_skillline=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("desc"),
			ReadOnlyAttribute(true)]
		property String^ ����
		{
			String^	get() {return _desc;}
			void	set(String^ value) {_desc=value;}
		}
		[DescriptionAttribute("�� �ŷ�Ʈ�� ����ϱ� ���� �Ҹ�Ǵ� �����"),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("HPCost"),
			ReadOnlyAttribute(true)]
		property String^ HPCost
		{
			String^	get() {return _HPCost;}
			void	set(String^ value) {_HPCost=value;}
		}
		[DescriptionAttribute("AP"),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("APCost"),
			ReadOnlyAttribute(true)]
		property String^ APCost
		{
			String^	get() {return _APCost;}
			void	set(String^ value) {_APCost=value;}
		}
		[DescriptionAttribute("�� �ŷ�Ʈ�� ����ϱ� ���� �Ҹ�Ǵ� ���ŷ�"),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ENCost"),
			ReadOnlyAttribute(true)]
		property String^ EnCost
		{
			String^	get() {return _ENCost;}
			void	set(String^ value) {_ENCost=value;}
		}
		[DescriptionAttribute("�� �ŷ�Ʈ�� ����ϱ� ���� �Ҹ�Ǵ� ������"),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("STACost"),
			ReadOnlyAttribute(true)]
		property String^ STACost
		{
			String^	get() {return _STACost;}
			void	set(String^ value) {_STACost=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("HPTickCost"),
			ReadOnlyAttribute(true)]
		property String^ HPTickCost
		{
			String^	get() {return _HPTickCost;}
			void	set(String^ value) {_HPTickCost=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ENTickCost"),
			ReadOnlyAttribute(true)]
		property String^ ENTickCost
		{
			String^	get() {return _ENTickCost;}
			void	set(String^ value) {_ENTickCost=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("STATickCost"),
			ReadOnlyAttribute(true)]
		property String^ STATickCost
		{
			String^	get() {return _STATickCost;}
			void	set(String^ value) {_STATickCost=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("CoolTime"),
			ReadOnlyAttribute(true)]
		property String^ CoolTime
		{
			String^	get() {return _CoolTime;}
			void	set(String^ value) {_CoolTime=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Stance"),
			ReadOnlyAttribute(true)]
		property String^ Stance
		{
			String^	get() {return _Stance;}
			void	set(String^ value) {_Stance=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("UseAni"),
			ReadOnlyAttribute(true)]
		property String^ UseAniName
		{
			String^	get() {return _UseAniName;}
			void	set(String^ value) {_UseAniName=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Type"),
			ReadOnlyAttribute(true)]
		property String^ Type
		{
			String^	get() {return _Type;}
			void	set(String^ value) {_Type=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("AttackDirection"),
			ReadOnlyAttribute(true),
			DefaultValueAttribute("�������")]
		property String^ AttackDirection
		{
			String^	get() {return _AttackDirection;}
			void	set(String^ value) {_AttackDirection=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("AITime"),
			ReadOnlyAttribute(true)]
		property String^ AITime
		{
			String^	get() {return _AlTime;}
			void	set(String^ value) {_AlTime=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_BEATEN"),
			ReadOnlyAttribute(true)]
		property String^ MF_BEATEN
		{
			String^	get() {return _MF_BEATEN;}
			void	set(String^ value) {_MF_BEATEN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_BEATEN"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_BEATEN
		{
			String^	get() {return _MF_WEIGHT_BEATEN;}
			void	set(String^ value) {_MF_WEIGHT_BEATEN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_KNOCKBACK"),
			ReadOnlyAttribute(true)]
		property String^ MF_KNOCKBACK
		{
			String^	get() {return _MF_KNOCKBACK;}
			void	set(String^ value) {_MF_KNOCKBACK=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_KNOCKBACK"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_KNOCKBACK
		{
			String^	get() {return _MF_WEIGHT_KNOCKBACK;}
			void	set(String^ value) {_MF_WEIGHT_KNOCKBACK=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_STUN"),
			ReadOnlyAttribute(true)]
		property String^ MF_STUN
		{
			String^	get() {return _MF_STUN;}
			void	set(String^ value) {_MF_STUN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_STUN"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_STUN
		{
			String^	get() {return _MF_WEIGHT_STUN;}
			void	set(String^ value) {_MF_WEIGHT_STUN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_DEFLECT"),
			ReadOnlyAttribute(true)]
		property String^ MF_DEFLECT
		{
			String^	get() {return _MF_DEFLECT;}
			void	set(String^ value) {_MF_DEFLECT=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_DEFLECT"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_DEFLECT
		{
			String^	get() {return _MF_WEIGHT_DEFLECT;}
			void	set(String^ value) {_MF_WEIGHT_DEFLECT=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_THROWUP"),
			ReadOnlyAttribute(true)]
		property String^ MF_THROWUP
		{
			String^	get() {return _MF_THROWUP;}
			void	set(String^ value) {_MF_THROWUP=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_THWOWUP"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_THROWUP
		{
			String^	get() {return _MF_WEIGHT_THROWUP;}
			void	set(String^ value) {_MF_WEIGHT_THROWUP=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_KNOCKDOWN"),
			ReadOnlyAttribute(true)]
		property String^ MF_KNOCKDOWN
		{
			String^	get() {return _MF_KNOCKDOWN;}
			void	set(String^ value) {_MF_KNOCKDOWN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_KNOCKDOWN"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_KNOCKDOWN
		{
			String^	get() {return _MF_WEIGHT_KNOCKDOWN;}
			void	set(String^ value) {_MF_WEIGHT_KNOCKDOWN=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Damage"),
			ReadOnlyAttribute(true)]
		property String^ Damage
		{
			String^	get() {return _Damage;}
			void	set(String^ value) {_Damage=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("HitTestType"),
			ReadOnlyAttribute(true)]
		property String^ HitTestType
		{
			String^	get() {return _HitTestType;}
			void	set(String^ value) {_HitTestType=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("HitRate"),
			ReadOnlyAttribute(true)]
		property String^ HitRate
		{
			String^	get() {return _HitRate;}
			void	set(String^ value) {_HitRate=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("HitMotionDelay"),
			ReadOnlyAttribute(true)]
		property String^ HitMotionDelay
		{
			String^	get() {return _HitMotionDelay;}
			void	set(String^ value) {_HitMotionDelay=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("EfficacyType"),
			ReadOnlyAttribute(true)]
		property String^ EfficacyType
		{
			String^	get() {return _EfficacyType;}
			void	set(String^ value) {_EfficacyType=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("EfficacyRadius"),
			ReadOnlyAttribute(true)]
		property String^ EfficacyRadius
		{
			String^	get() {return _EfficacyRadius;}
			void	set(String^ value) {_EfficacyRadius=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("NeededTP"),
			ReadOnlyAttribute(true)]
		property String^ NeededTP
		{
			String^	get() {return _NeededTP;}
			void	set(String^ value) {_NeededTP=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ReqLevel"),
			ReadOnlyAttribute(true)]
		property String^ ReqLevel
		{
			String^	get() {return _ReqLevel;}
			void	set(String^ value) {_ReqLevel=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("TalentLine"),
			ReadOnlyAttribute(true)]
		property String^ TalentLine
		{
			String^	get() {return _TalentLine;}
			void	set(String^ value) {_TalentLine=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("TalentType"),
			ReadOnlyAttribute(true)]
		property String^ TalentType
		{
			String^	get() {return _TalentType;}
			void	set(String^ value) {_TalentType=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("SkillType"),
			ReadOnlyAttribute(true)]
		property String^ SkillType
		{
			String^	get() {return _SkillType;}
			void	set(String^ value) {_SkillType=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_UPPERED"),
			ReadOnlyAttribute(true)]
		property String^ MF_UPPERED
		{
			String^	get() {return _MF_UPPERED;}
			void	set(String^ value) {_MF_UPPERED=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MF_WEIGHT_UPPERED"),
			ReadOnlyAttribute(true)]
		property String^ MF_WEIGHT_UPPERED
		{
			String^	get() {return _MF_WEIGHT_UPPERED;}
			void	set(String^ value) {_MF_WEIGHT_UPPERED=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Guardable"),
			ReadOnlyAttribute(true)]
		property String^ Guardable
		{
			String^	get() {return _Guardable;}
			void	set(String^ value) {_Guardable=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("Cancelable"),
			ReadOnlyAttribute(true)]
		property String^ Cancelable
		{
			String^	get() {return _Cancelable;}
			void	set(String^ value) {_Cancelable=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("TargetsAllowed"),
			ReadOnlyAttribute(true)]
		property String^ TargetsAllowed
		{
			String^	get() {return _TargetsAllowed;}
			void	set(String^ value) {_TargetsAllowed=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("CastingAni"),
			ReadOnlyAttribute(true)]
		property String^ CastingAni
		{
			String^	get() {return _CastingAni;}
			void	set(String^ value) {_CastingAni=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("CastingLoopAni"),
			ReadOnlyAttribute(true)]
		property String^ CastingLoopAni
		{
			String^	get() {return _CastingLoopAni;}
			void	set(String^ value) {_CastingLoopAni=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("CastingTime"),
			ReadOnlyAttribute(true)]
		property String^ CastingTime
		{
			String^	get() {return _CastingTime;}
			void	set(String^ value) {_CastingTime=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ProjectileType"),
			ReadOnlyAttribute(true)]
		property String^ ProjectileType
		{
			String^	get() {return _ProjectileType;}
			void	set(String^ value) {_ProjectileType=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ProjectileSpeed"),
			ReadOnlyAttribute(true)]
		property String^ ProjectileSpeed
		{
			String^	get() {return _ProjectileSpeed;}
			void	set(String^ value) {_ProjectileSpeed=value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("ProjectileRange"),
			ReadOnlyAttribute(true)]
		property String^ ProjectileRange
		{
			String^	get() {return _ProjectileRange;}
			void	set(String^ value) {_ProjectileRange=value;}
		}

	};

	public ref class NPC
	{
	public: NPC(){}

	private: String^ _id;
	private: String^ _name;
	private: String^ _MeshName;
	private: String^ _desc;
	private: String^ _level;
	private: String^ _MaxHP;
	private: String^ _MaxEN;
	private: String^ _MaxSTA;
	private: String^ _MaxAP;
	private: String^ _Radius;
	private: String^ _Speed;
	private: String^ _RunSpeed;
	private: String^ _Rooted;
	private: String^ _Attackable;
	private: String^ _CounterAttack;
	private: String^ _AA;
	private: String^ _SightRange;
	private: String^ _SightAngle;
	private: String^ _Earshot;
	private: String^ _NameInLua;
	private: String^ _grade;
	private: String^ _scale;
	private: String^ _Occupation;

	private: String^ _Type;
	private: String^ _MeshPath;
	private: String^ _MinLevel;
	private: String^ _MaxLevel;
	private: String^ _AP;
	private: String^ _CHA;
	private: String^ _MR;
	private: String^ _FR;
	private: String^ _CR;
	private: String^ _LR;
	private: String^ _PR;
	private: String^ _Huge;
	private: String^ _RotateSpeed;
	private: String^ _Interact;
	private: String^ _ShowName;
	private: String^ _AlignTerrain;
	private: String^ _BeatenModValue;
	private: String^ _KnockbackModValue;
	private: String^ _StunModValue;
	private: String^ _DeflectModValue;
	private: String^ _ThrowupModValue;
	private: String^ _KnockdownModValue;
	private: String^ _UpperedModValue;
	private: String^ _BeatenModWeight;
	private: String^ _KnockbackModWeight;
	private: String^ _StunModWeight;
	private: String^ _DeflectModWeight;
	private: String^ _ThrowupModWeight;
	private: String^ _KnockdownModWeight;
	private: String^ _UpperedModWeight;
	private: String^ _Item_Loot;
	private: String^ _TitleBone;
	private: String^ _TitleHeight;

	private: String^ _ICond1;
	private: String^ _IType1;
	private: String^ _IAct1;
	private: String^ _ICond2;
	private: String^ _IType2;
	private: String^ _IAct2;
	private: String^ _ICond3;
	private: String^ _IType3;
	private: String^ _IAct3;
	private: String^ _ICond4;
	private: String^ _IType4;
	private: String^ _IAct4;
	private: String^ _ICond5;
	private: String^ _IType5;
	private: String^ _IAct5;
	private: String^ _ICond6;
	private: String^ _IType6;
	private: String^ _IAct6;

	private: String^ _ILootAni;
	private: String^ _ILootTime;

	private: String^ _BloodEffectName;

	private: String^ _PartsHair;
	private: String^ _PartsFace;
	private: String^ _PartsHead;
	private: String^ _PartsLeg;
	private: String^ _PartsFeet;
	private: String^ _PartsWeaponR;
	private: String^ _PartsChest;
	private: String^ _PartsHands;
	private: String^ _OpeningStatement;

	private: String^ _ArmorType;
	private: String^ _CriticalDmgAmp;
	private: String^ _CriticalMelee;
	private: String^ _CriticalRange;
	private: String^ _CriticalMagic;
	private: String^ _TexColor;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("id"),
			ReadOnlyAttribute(true)]
		//[XmlAttributeAttribute("id")]
		//[XmlAttribute("id")]
		property String^ ���̵�
		{
			String^	get() {return _id;}
			void	set(String^ value) {_id=value;}
		}
		[CategoryAttribute("Subtitle"),
			XmlAttribute("Desc"),
			ReadOnlyAttribute(true)]
		property String^ ����
		{
			String^	get() {return _desc;}
			void	set(String^ value) {_desc=value;}
		}
		[CategoryAttribute("Subtitle"),
			XmlAttribute("Level"),
			ReadOnlyAttribute(true)]
		property String^ Level
		{
			String^	get() {return _level;}
			void	set(String^ value) {_level=value;}
		}
		[CategoryAttribute("Name"),
			XmlAttribute("meshname"),
			ReadOnlyAttribute(true)]
		property String^ �޽��̸�
		{
			String^	get() {return _MeshName;}
			void	set(String^ value) {_MeshName=value;}
		}
		[CategoryAttribute("Name"),
			XmlAttribute("name"),
			ReadOnlyAttribute(true)]
		property String^ �̸�
		{
			String^	get() {return _name;}
			void	set(String^ value) {_name=value;}
		}
		[DescriptionAttribute("ĳ������ �ִ� �����."),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MaxHP"),
			ReadOnlyAttribute(true)]
		property String^ �ִ�HP
		{
			String^	get() {return _MaxHP;}
			void	set(String^ value) {_MaxHP=value;}
		}
		[DescriptionAttribute("ĳ������ �ִ� ���ŷ�."),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MaxEN"),
			ReadOnlyAttribute(true)]
		property String^ �ִ�EN
		{
			String^	get() {return _MaxEN;}
			void	set(String^ value) {_MaxEN=value;}
		}
		[DescriptionAttribute("ĳ������ �ִ� �ൿ��."),
			CategoryAttribute("Basic Attributes"),
			XmlAttribute("MaxSTA"),
			ReadOnlyAttribute(true)]
		property String^ �ִ�STA
		{
			String^	get() {return _MaxSTA;}
			void	set(String^ value) {_MaxSTA=value;}
		}
		[DescriptionAttribute("ĳ������ �ִ� �Ƹ� ����Ʈ. �������� ������ ��Ÿ����."),
			CategoryAttribute("Defensive Property"),
			XmlAttribute("MaxAP"),
			ReadOnlyAttribute(true)]
		property String^ �ִ�AP
		{
			String^	get() {return _MaxAP;}
			void	set(String^ value) {_MaxAP=value;}
		}
		[XmlAttribute("RADIUS")]
		property String^ RADIUS
		{
			String^	get() {return _Radius;}
			void	set(String^ value) {_Radius=value;}
		}
		[XmlAttribute("���ǵ�")]
		property String^ ���ǵ�
		{
			String^	get() {return _Speed;}
			void	set(String^ value) {_Speed=value;}
		}
		[XmlAttribute("�޸���ӵ�")]
		property String^ �޸���ӵ�
		{
			String^	get() {return _RunSpeed;}
			void	set(String^ value) {_RunSpeed=value;}
		}
		[XmlAttribute("Rooted")]
		property String^ Rooted
		{
			String^	get() {return _Rooted;}
			void	set(String^ value) {_Rooted=value;}
		}
		[DescriptionAttribute("���ݰ��ɿ���. �������� NPC���� �ƴ����� �Ǻ��ϴ� ������ ������ NPC���� ���� �� �÷��װ� FALSE�� �Ǿ��ִ�. Attackable�� FALSE�� ���, �ش� NPC�� Invincible�Ӽ��� ������ �Ǹ� ��� ������ε� �׿��� �������� ������ ���� �Ұ����ϴ�."),
			CategoryAttribute("Attackable"),
			XmlAttribute("Attackable"),
			ReadOnlyAttribute(true)]
		property String^ ���ݰ���
		{
			String^	get() {return _Attackable;}
			void	set(String^ value) {_Attackable=value;}
		}
		[XmlAttribute("ī����")]
		property String^ ī����
		{
			String^	get() {return _CounterAttack;}
			void	set(String^ value) {_CounterAttack=value;}
		}
		[DescriptionAttribute("Always Aggressive"),
			CategoryAttribute("AA"),
			XmlAttribute("AA"),
			ReadOnlyAttribute(true)]
		property String^ AA
		{
			String^	get() {return _AA;}
			void	set(String^ value) {_AA=value;}
		}
		[DescriptionAttribute("NPC�� �� �� �ִ� �Ÿ�, ���ͷ� ��õ�"),
			CategoryAttribute("SightRange"),
			XmlAttribute("SightRange"),
			ReadOnlyAttribute(true)]
		property String^ SightRange
		{
			String^	get() {return _SightRange;}
			void	set(String^ value) {_SightRange=value;}
		}
		[DescriptionAttribute("NPC�� �� �� �ִ� ����, Degree�� ��õ�"),
			CategoryAttribute("SightAngle"),
			XmlAttribute("SightAngle"),
			ReadOnlyAttribute(true)]
		property String^ SightAngle
		{
			String^	get() {return _SightAngle;}
			void	set(String^ value) {_SightAngle=value;}
		}
		[DescriptionAttribute("NPC�� ���� �� �ִ� ��û�Ÿ�, ���ͷ� ��õ�"),
			CategoryAttribute("Earshot"),
			XmlAttribute("Earshot"),
			ReadOnlyAttribute(true)]
		property String^ Earshot
		{
			String^	get() {return _Earshot;}
			void	set(String^ value) {_Earshot=value;}
		}
		[DescriptionAttribute("���� ���� �ൿ�� ���� Callbacks�� ������ ���ϰ�γ� ���� �����̽��� ��ũ�Ѵ�. �̰��� Ŭ���̾�Ʈ�� ��ġ�� �ʿ䰡 ���� ����."),
			CategoryAttribute("Character Script"),
			XmlAttribute("NameInLua"),
			ReadOnlyAttribute(true)]
		property String^ NameInLua
		{
			String^	get() {return _NameInLua;}
			void	set(String^ value) {_NameInLua=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("Subtitle"),
			XmlAttribute("grade"),
			ReadOnlyAttribute(true)]
		property String^ grade
		{
			String^	get() {return _grade;}
			void	set(String^ value) {_grade=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("Scale"),
			XmlAttribute("Scale"),
			ReadOnlyAttribute(true)]
		property String^ Scale
		{
			String^	get() {return _scale;}
			void	set(String^ value) {_scale=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("Occupation"),
			XmlAttribute("Occupation"),
			ReadOnlyAttribute(true)]
		property String^ Occupation
		{
			String^	get() {return _Occupation;}
			void	set(String^ value) {_Occupation=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("Type"),
			ReadOnlyAttribute(true)]
		property String^ Type
		{
			String^	get() {return _Type;}
			void	set(String^ value) {_Type=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("MeshPath"),
			ReadOnlyAttribute(true)]
		property String^ MeshPath
		{
			String^	get() {return _MeshPath;}
			void	set(String^ value) {_MeshPath=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("MinLevel"),
			ReadOnlyAttribute(true)]
		property String^ MinLevel
		{
			String^	get() {return _MinLevel;}
			void	set(String^ value) {_MinLevel=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("MaxLevel"),
			ReadOnlyAttribute(true)]
		property String^ MaxLevel
		{
			String^	get() {return _MaxLevel;}
			void	set(String^ value) {_MaxLevel=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("AP"),
			ReadOnlyAttribute(true)]
		property String^ AP
		{
			String^	get() {return _AP;}
			void	set(String^ value) {_AP=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("CHA"),
			ReadOnlyAttribute(true)]
		property String^ CHA
		{
			String^	get() {return _CHA;}
			void	set(String^ value) {_CHA=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("MR"),
			ReadOnlyAttribute(true)]
		property String^ MR
		{
			String^	get() {return _MR;}
			void	set(String^ value) {_MR=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("FR"),
			ReadOnlyAttribute(true)]
		property String^ FR
		{
			String^	get() {return _FR;}
			void	set(String^ value) {_FR=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("CR"),
			ReadOnlyAttribute(true)]
		property String^ CR
		{
			String^	get() {return _CR;}
			void	set(String^ value) {_CR=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("LR"),
			ReadOnlyAttribute(true)]
		property String^ LR
		{
			String^	get() {return _LR;}
			void	set(String^ value) {_LR=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("PR"),
			ReadOnlyAttribute(true)]
		property String^ PR
		{
			String^	get() {return _PR;}
			void	set(String^ value) {_PR=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("Huge"),
			ReadOnlyAttribute(true)]
		property String^ Huge
		{
			String^	get() {return _Huge;}
			void	set(String^ value) {_Huge=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("RotateSpeed"),
			ReadOnlyAttribute(true)]
		property String^ RotateSpeed
		{
			String^	get() {return _RotateSpeed;}
			void	set(String^ value) {_RotateSpeed=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("Interact"),
			ReadOnlyAttribute(true)]
		property String^ Interact
		{
			String^	get() {return _Interact;}
			void	set(String^ value) {_Interact=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ShowName"),
			ReadOnlyAttribute(true)]
		property String^ ShowName
		{
			String^	get() {return _ShowName;}
			void	set(String^ value) {_ShowName=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("AlignTerrain"),
			ReadOnlyAttribute(true)]
		property String^ AlignTerrain
		{
			String^	get() {return _AlignTerrain;}
			void	set(String^ value) {_AlignTerrain=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("BeatenModValue"),
			ReadOnlyAttribute(true)]
		property String^ BeatenModValue
		{
			String^	get() {return _BeatenModValue;}
			void	set(String^ value) {_BeatenModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("KnockbackModValue"),
			ReadOnlyAttribute(true)]
		property String^ KnockbackModValue
		{
			String^	get() {return _KnockbackModValue;}
			void	set(String^ value) {_KnockbackModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("StunModValue"),
			ReadOnlyAttribute(true)]
		property String^ StunModValue
		{
			String^	get() {return _StunModValue;}
			void	set(String^ value) {_StunModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("DeflectModValue"),
			ReadOnlyAttribute(true)]
		property String^ DeflectModValue
		{
			String^	get() {return _DeflectModValue;}
			void	set(String^ value) {_DeflectModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ThrowupModValue"),
			ReadOnlyAttribute(true)]
		property String^ ThrowupModValue
		{
			String^	get() {return _ThrowupModValue;}
			void	set(String^ value) {_ThrowupModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("KnockdownModValue"),
			ReadOnlyAttribute(true)]
		property String^ KnockdownModValue
		{
			String^	get() {return _KnockdownModValue;}
			void	set(String^ value) {_KnockdownModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("UpperedModValue"),
			ReadOnlyAttribute(true)]
		property String^ UpperedModValue
		{
			String^	get() {return _UpperedModValue;}
			void	set(String^ value) {_UpperedModValue=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("BeatenModWeight"),
			ReadOnlyAttribute(true)]
		property String^ BeatenModWeight
		{
			String^	get() {return _BeatenModWeight;}
			void	set(String^ value) {_BeatenModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("KnockbackModWeight"),
			ReadOnlyAttribute(true)]
		property String^ KnockbackModWeight
		{
			String^	get() {return _KnockbackModWeight;}
			void	set(String^ value) {_KnockbackModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("StunModWeight"),
			ReadOnlyAttribute(true)]
		property String^ StunModWeight
		{
			String^	get() {return _StunModWeight;}
			void	set(String^ value) {_StunModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("DeflectModWeight"),
			ReadOnlyAttribute(true)]
		property String^ DeflectModWeight
		{
			String^	get() {return _DeflectModWeight;}
			void	set(String^ value) {_DeflectModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ThrowupModWeight"),
			ReadOnlyAttribute(true)]
		property String^ ThrowupModWeight
		{
			String^	get() {return _ThrowupModWeight;}
			void	set(String^ value) {_ThrowupModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("KnockdownModWeight"),
			ReadOnlyAttribute(true)]
		property String^ KnockdownModWeight
		{
			String^	get() {return _KnockdownModWeight;}
			void	set(String^ value) {_KnockdownModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("UpperedModWeight"),
			ReadOnlyAttribute(true)]
		property String^ UpperedModWeight
		{
			String^	get() {return _UpperedModWeight;}
			void	set(String^ value) {_UpperedModWeight=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("Item_Loot"),
			ReadOnlyAttribute(true)]
		property String^ Item_Loot
		{
			String^	get() {return _Item_Loot;}
			void	set(String^ value) {_Item_Loot=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("TitleBone"),
			ReadOnlyAttribute(true)]
		property String^ TitleBone
		{
			String^	get() {return _TitleBone;}
			void	set(String^ value) {_TitleBone=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("TitleHeight"),
			ReadOnlyAttribute(true)]
		property String^ TitleHeight
		{
			String^	get() {return _TitleHeight;}
			void	set(String^ value) {_TitleHeight=value;}
		}
		//////////////////////////////////////////////////////////////////////////
		[DescriptionAttribute("."),
			XmlAttribute("ICond1"),
			ReadOnlyAttribute(true)]
		property String^ ICond1
		{
			String^	get() {return _ICond1;}
			void	set(String^ value) {_ICond1=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType1"),
			ReadOnlyAttribute(true)]
		property String^ IType1
		{
			String^	get() {return _IType1;}
			void	set(String^ value) {_IType1=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct1"),
			ReadOnlyAttribute(true)]
		property String^ IAct1
		{
			String^	get() {return _IAct1;}
			void	set(String^ value) {_IAct1=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ICond2"),
			ReadOnlyAttribute(true)]
		property String^ ICond2
		{
			String^	get() {return _ICond2;}
			void	set(String^ value) {_ICond2=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType2"),
			ReadOnlyAttribute(true)]
		property String^ IType2
		{
			String^	get() {return _IType2;}
			void	set(String^ value) {_IType2=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct2"),
			ReadOnlyAttribute(true)]
		property String^ IAct2
		{
			String^	get() {return _IAct2;}
			void	set(String^ value) {_IAct2=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ICond3"),
			ReadOnlyAttribute(true)]
		property String^ ICond3
		{
			String^	get() {return _ICond3;}
			void	set(String^ value) {_ICond3=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType3"),
			ReadOnlyAttribute(true)]
		property String^ IType3
		{
			String^	get() {return _IType3;}
			void	set(String^ value) {_IType3=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct3"),
			ReadOnlyAttribute(true)]
		property String^ IAct3
		{
			String^	get() {return _IAct3;}
			void	set(String^ value) {_IAct3=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ICond4"),
			ReadOnlyAttribute(true)]
		property String^ ICond4
		{
			String^	get() {return _ICond4;}
			void	set(String^ value) {_ICond4=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType4"),
			ReadOnlyAttribute(true)]
		property String^ IType4
		{
			String^	get() {return _IType4;}
			void	set(String^ value) {_IType4=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct4"),
			ReadOnlyAttribute(true)]
		property String^ IAct4
		{
			String^	get() {return _IAct4;}
			void	set(String^ value) {_IAct4=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ICond5"),
			ReadOnlyAttribute(true)]
		property String^ ICond5
		{
			String^	get() {return _ICond5;}
			void	set(String^ value) {_ICond5=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType5"),
			ReadOnlyAttribute(true)]
		property String^ IType5
		{
			String^	get() {return _IType5;}
			void	set(String^ value) {_IType5=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct5"),
			ReadOnlyAttribute(true)]
		property String^ IAct5
		{
			String^	get() {return _IAct5;}
			void	set(String^ value) {_IAct5=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ICond6"),
			ReadOnlyAttribute(true)]
		property String^ ICond6
		{
			String^	get() {return _ICond6;}
			void	set(String^ value) {_ICond6=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IType6"),
			ReadOnlyAttribute(true)]
		property String^ IType6
		{
			String^	get() {return _IType6;}
			void	set(String^ value) {_IType6=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("IAct6"),
			ReadOnlyAttribute(true)]
		property String^ IAct6
		{
			String^	get() {return _IAct6;}
			void	set(String^ value) {_IAct6=value;}
		}

		//////////////////////////////////////////////////////////////////////////
		[DescriptionAttribute("."),
			XmlAttribute("ILootAni"),
			ReadOnlyAttribute(true)]
		property String^ ILootAni
		{
			String^	get() {return _ILootAni;}
			void	set(String^ value) {_ILootAni=value;}
		}
		[DescriptionAttribute("."),
			XmlAttribute("ILootTime"),
			ReadOnlyAttribute(true)]
		property String^ ILootTime
		{
			String^	get() {return _ILootTime;}
			void	set(String^ value) {_ILootTime=value;}
		}

		[DescriptionAttribute("."),
			XmlAttribute("BloodEffectName"),
			ReadOnlyAttribute(true)]
		property String^ BloodEffectName
		{
			String^	get() {return _BloodEffectName;}
			void	set(String^ value) {_BloodEffectName=value;}
		}
		//////////////////////////////////////////////////////////////////////////
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsHair"),
			ReadOnlyAttribute(true)]
		property String^ PartsHair
		{
			String^	get() {return _PartsHair;}
			void	set(String^ value) {_PartsHair=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsFace"),
			ReadOnlyAttribute(true)]
		property String^ PartsFace
		{
			String^	get() {return _PartsFace;}
			void	set(String^ value) {_PartsFace=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsHead"),
			ReadOnlyAttribute(true)]
		property String^ PartsHead
		{
			String^	get() {return _PartsHead;}
			void	set(String^ value) {_PartsHead=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsLeg"),
			ReadOnlyAttribute(true)]
		property String^ PartsLeg
		{
			String^	get() {return _PartsLeg;}
			void	set(String^ value) {_PartsLeg=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsFeet"),
			ReadOnlyAttribute(true)]
		property String^ PartsFeet
		{
			String^	get() {return _PartsFeet;}
			void	set(String^ value) {_PartsFeet=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsWeaponR"),
			ReadOnlyAttribute(true)]
		property String^ PartsWeaponR
		{
			String^	get() {return _PartsWeaponR;}
			void	set(String^ value) {_PartsWeaponR=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsChest"),
			ReadOnlyAttribute(true)]
		property String^ PartsChest
		{
			String^	get() {return _PartsChest;}
			void	set(String^ value) {_PartsChest=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("PartsHands"),
			ReadOnlyAttribute(true)]
		property String^ PartsHands
		{
			String^	get() {return _PartsHands;}
			void	set(String^ value) {_PartsHands=value;}
		}
		[DescriptionAttribute("."),
			CategoryAttribute("NPC"),
			XmlAttribute("OpeningStatement"),
			ReadOnlyAttribute(true)]
		property String^ OpeningStatement
		{
			String^	get() {return _OpeningStatement;}
			void	set(String^ value) {_OpeningStatement=value;}
		}
		//////////////////////////////////////////////////////////////////////////
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("ArmorType"),
			 ReadOnlyAttribute(true)]
		 property String^ ArmorType
		 {
			 String^	get() {return _ArmorType;}
			 void	set(String^ value) {_ArmorType=value;}
		 }
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("CriticalDmgAmp"),
			 ReadOnlyAttribute(true)]
		 property String^ CriticalDmgAmp
		 {
			 String^	get() {return _CriticalDmgAmp;}
			 void	set(String^ value) {_CriticalDmgAmp=value;}
		 }
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("CriticalMelee"),
			 ReadOnlyAttribute(true)]
		 property String^ CriticalMelee
		 {
			 String^	get() {return _CriticalMelee;}
			 void	set(String^ value) {_PartsFace=value;}
		 }
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("CriticalRange"),
			 ReadOnlyAttribute(true)]
		 property String^ CriticalRange
		 {
			 String^	get() {return _CriticalRange;}
			 void	set(String^ value) {_CriticalRange=value;}
		 }
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("CriticalMagic"),
			 ReadOnlyAttribute(true)]
		 property String^ CriticalMagic
		 {
			 String^	get() {return _CriticalMagic;}
			 void	set(String^ value) {_CriticalMagic=value;}
		 }
		 [DescriptionAttribute("."),
			 CategoryAttribute("Monster"),
			 XmlAttribute("TexColor"),
			 ReadOnlyAttribute(true)]
		 property String^ TexColor
		 {
			 String^	get() {return _TexColor;}
			 void	set(String^ value) {_TexColor=value;}
		 }

	};

	public ref class maiet
	{
	public:
		cli::array<NPC^>^npcs;
	};

	//----------------------------------------------------------------------------//
	// ����Ʈ Ÿ��
	public ref class EffectTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALEffectType = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_TalentEffectTabController->GetTalentEffectTypeList(ALEffectType);

			return gcnew StandardValuesCollection(ALEffectType);
		} 
	};
	public ref class EffectBaseTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALEffectType = gcnew ArrayList;
			ALEffectType->Add("Melee");
			ALEffectType->Add("MeleeSecondary");
			ALEffectType->Add("Magic");
			ALEffectType->Add("Archery");
			ALEffectType->Add("Gun");
			ALEffectType->Add("Tackle");
			ALEffectType->Add("Defense");
			ALEffectType->Add("NoShieldDefense");
			ALEffectType->Add("MonsterDefense");

			return gcnew StandardValuesCollection(ALEffectType);
		} 
	};
	// ����Ÿ��
	public ref class DirectionAttributeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALImpactPosition = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_TalentEffectTabController->GetTalentEffectDirectionList(ALImpactPosition);

			return gcnew StandardValuesCollection(ALImpactPosition);
		} 
	};
	// �浹 ��ġ
	public ref class ImpactPositionConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALImpactPosition = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_TalentEffectTabController->GetTalentEffectImpactList(ALImpactPosition);

			return gcnew StandardValuesCollection(ALImpactPosition);
		} 
	};

	// ����Ʈ �̸�
	public ref class EffectNameConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ EffectArray = gcnew ArrayList;
			g_pMainApp->GetEffectInfoList(EffectArray);

			return gcnew StandardValuesCollection(EffectArray);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	//----------------------------------------------------------------------------//
	//scroolbar�� maximun ũ�Ⱑ �޶� 1,2,3 ���� ��,��;;;;
	public ref class MyValueEditor1 : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::DropDown;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			//http://msdn2.microsoft.com/en-us/library/system.windows.forms.design.iwindowsformseditorservice.aspx
			IWindowsFormsEditorService^ wfes = dynamic_cast<IWindowsFormsEditorService^>(provider->GetService( IWindowsFormsEditorService::typeid ));

			if (wfes != nullptr)
			{
				frmMyValue^ _frm = gcnew frmMyValue();
				_frm->trackBar1->Maximum = 500;
				_frm->trackBar1->Value = Convert::ToInt32( value );	//int�� ĳ�����ϸ� �������� �ʴ´�.
				_frm->MyValue = _frm->trackBar1->Value;
				_frm->_wfes = wfes;

				wfes->DropDownControl(_frm);
				value = _frm->MyValue;
			}

			return value;
		}
	};

	public ref class MyValueEditor2 : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::DropDown;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			//http://msdn2.microsoft.com/en-us/library/system.windows.forms.design.iwindowsformseditorservice.aspx
			IWindowsFormsEditorService^ wfes = dynamic_cast<IWindowsFormsEditorService^>(provider->GetService( IWindowsFormsEditorService::typeid ));

			if (wfes != nullptr)
			{
				frmMyValue^ _frm = gcnew frmMyValue();
				_frm->trackBar1->Maximum = 2000;
				_frm->trackBar1->Minimum = -2000;
				_frm->trackBar1->Value = Convert::ToInt32( value );	//int�� ĳ�����ϸ� �������� �ʴ´�.
				_frm->MyValue = _frm->trackBar1->Value;
				_frm->_wfes = wfes;

				wfes->DropDownControl(_frm);
				value = _frm->MyValue;
			}

			return value;
		}
	};

	public ref class MyValueEditor3 : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::DropDown;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			//http://msdn2.microsoft.com/en-us/library/system.windows.forms.design.iwindowsformseditorservice.aspx
			IWindowsFormsEditorService^ wfes = dynamic_cast<IWindowsFormsEditorService^>(provider->GetService( IWindowsFormsEditorService::typeid ));

			if (wfes != nullptr)
			{
				frmMyValue^ _frm = gcnew frmMyValue();
				_frm->bAni = true;
				_frm->trackBar1->Maximum = GlobalObjects::g_pMainForm->AnitrackBar->Maximum;
				_frm->trackBar1->Value = (int)((float)value*4.8f*(g_pMainApp->GetActor()->GetSpeed()*1000.f));
				_frm->MyValue = _frm->trackBar1->Value;
				_frm->_wfes = wfes;

				wfes->DropDownControl(_frm);
				value = _frm->MyValue/4.8f/(g_pMainApp->GetActor()->GetSpeed()*1000.f);
			}

			return value;
		}
	};

	public ref class MyValueEditorDegree : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::DropDown;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			//http://msdn2.microsoft.com/en-us/library/system.windows.forms.design.iwindowsformseditorservice.aspx
			IWindowsFormsEditorService^ wfes = dynamic_cast<IWindowsFormsEditorService^>(provider->GetService( IWindowsFormsEditorService::typeid ));

			if (wfes != nullptr)
			{
				frmMyValue^ _frm = gcnew frmMyValue();
				_frm->bAni = true;
				_frm->trackBar1->Maximum = 0;
				_frm->trackBar1->Minimum = 360;
				_frm->trackBar1->Value = Convert::ToInt32( value );	//int�� ĳ�����ϸ� �������� �ʴ´�.
				_frm->MyValue = _frm->trackBar1->Value;
				_frm->_wfes = wfes;

				wfes->DropDownControl(_frm);
				value = _frm->MyValue/4.8f/(g_pMainApp->GetActor()->GetSpeed()*1000.f);
			}

			return value;
		}
	};

	// TODO: ������Ʈ ���̾�α� �������ֻ�.
	public ref class ModelBoneList : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::Modal;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			return value;
		}
	};

	public ref class SoundNameConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALEffectType = gcnew ArrayList;
/*
			for (XSoundMap::iterator itor = g_pMainApp->m_pSoundMgr.GetSoundData()->begin();
				itor != g_pMainApp->m_pSoundMgr.GetSoundData()->end(); ++itor)
			{
				string str = (*itor).first;
				ALEffectType->Add(gcnew String(str.c_str()));
			}
*/
			return gcnew StandardValuesCollection(ALEffectType);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	public ref class CustomEffectList : UITypeEditor
	{
	public:
		virtual UITypeEditorEditStyle GetEditStyle(System::ComponentModel::ITypeDescriptorContext^ context) override
		{
			return UITypeEditorEditStyle::Modal;
		}

		virtual Object^ EditValue(System::ComponentModel::ITypeDescriptorContext^ context, System::IServiceProvider^ provider, System::Object^ value) override
		{
			Object^ objTemp = g_pMainApp->GetSearchCustomEffectInfoName();
			if(objTemp != nullptr)
				value = objTemp;

			return value;
		}
	};

	//------------------------------------------------------------------------
	// �⺻ ����Ʈ
	public ref class TALENTEFFECT
	{
	public: TALENTEFFECT(){}

	private: 
		String^		_TalentEffectInfoName;
		String^		_EffectType;
		String^		_EffectName;
		String^		_EffectDirection;

		String^		_BoneName;
		String^		_Parameter;
	public:
		[DescriptionAttribute(""),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("TalentEffectName")]
		property String^ TalentEffectInfoName
		{
			String^		get() {return _TalentEffectInfoName;}
			void		set(String^ value) {_TalentEffectInfoName = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("����Ʈ Ÿ��"),
			XmlAttribute("EffectType"),
			TypeConverter(EffectTypeConverter::typeid)]
		property String^ EffectType
		{
			String^		get() {return _EffectType;}
			void		set(String^ value) {_EffectType = value;}
		}
		[DescriptionAttribute("dummy_loc�� ���� �ȵ˴ϴ�. ����ġ�� ������ �ҷ��� �� �̸��� �ۼ����� ������!"),
			CategoryAttribute("�⺻ �Ӽ�"),
			XmlAttribute("�� �̸�"),
			EditorAttribute(ModelBoneList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ���̸�
		{
			String^		get() {return _BoneName;}
			void		set(String^ value) {_BoneName = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�⺻ �Ӽ�"),
			XmlAttribute("����Ʈ ���� �̸�"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ EffectName
		{
			String^		get() {return _EffectName;}
			void		set(String^ value) {_EffectName = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("��Ÿ �Ӽ�"),
			XmlAttribute("Parameter")]
		property String^ Parameter
		{
			String^		get() {return _Parameter;}
			void		set(String^ value) {_Parameter = value;}
		}
		[DescriptionAttribute("'Į ����'�� �ǰ� ����Ʈ���� �ش��ϸ� �� �˱��� �־�� �۵��˴ϴ�."),
			CategoryAttribute("��Ÿ �Ӽ�"),
			XmlAttribute("dir_attr"),
			TypeConverter(DirectionAttributeConverter::typeid)]
		property String^ ����
		{
			String^		get() {return _EffectDirection;}
			void		set(String^ value) {_EffectDirection = value;}
		}
	};

	public ref class TALENTEFFECT_EX : public TALENTEFFECT
	{
	public: TALENTEFFECT_EX(){}

	private: 

		String^		_ImpactPosition;

	public:
		[DescriptionAttribute("ĳ���� �浹�� ���� �Ӽ�.�´���ġ�Ӽ��ϰ�캻�̾�����⺻����������."),
			CategoryAttribute("��Ÿ �Ӽ�"),
			XmlAttribute("ImpactPosition"),
			TypeConverter(ImpactPositionConverter::typeid)]
		property String^ �浹��ġ���
		{
			String^		get() {return _ImpactPosition;}
			void		set(String^ value) {_ImpactPosition = value;}
		}

	};

	// ��Ʈ ����Ʈ ��ġ ����
	public ref class TALENTEFFECT_HITEFFECT_POSDIR
	{
	public: TALENTEFFECT_HITEFFECT_POSDIR(){}

	private: 
		float			up_x;
		float			up_y;
		float			up_z;
		float			pos_x;
		float			pos_y;
		float			pos_z;

	public:
		[DescriptionAttribute("���� ��ġ X"),
			CategoryAttribute("��ġ"),
			XmlAttribute("X"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float X
		{
			float		get() {return pos_x;}
			void		set(float value) {pos_x= value;}
		}
		[DescriptionAttribute("���� ��ġ Y"),
			CategoryAttribute("��ġ"),
			XmlAttribute("Y"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float Y
		{
			float		get() {return pos_y;}
			void		set(float value) {pos_y= value;}
		}
		[DescriptionAttribute("���� ��ġ Z"),
			CategoryAttribute("��ġ"),
			XmlAttribute("Z"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float Z
		{
			float		get() {return pos_z;}
			void		set(float value) {pos_z= value;}
		}

		[DescriptionAttribute("���� X"),
			CategoryAttribute("����"),
			XmlAttribute("dir_X")]
		property float dir_X
		{
			float		get() {return up_x;}
		}
		[DescriptionAttribute("���� Y"),
			CategoryAttribute("����"),
			XmlAttribute("dir_Y")]
		property float dir_Y
		{
			float		get() {return up_y;}
		}
		[DescriptionAttribute("���� Z"),
			CategoryAttribute("����"),
			XmlAttribute("dir_Z")]
		property float dir_Z
		{
			float		get() {return up_z;}
		}

		void Init_Dir(float x, float y, float z) { up_x = x; up_y = y; up_z = z; }

	};

	//------------------------------------------------------------------------
	// �⺻
	public ref class TALENTEFFECT_BASE
	{
	public: TALENTEFFECT_BASE(){}

	private: 
		String^		_EffectBaseType;

		String^		_EffectName;
		String^		_ImpactPosition;
		String^		_EffectDirection;

		String^		_BoneName;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("����Ʈ �⺻"),
			XmlAttribute("EffectBase"),
			TypeConverter(EffectBaseTypeConverter::typeid)]
		property String^ EffectBaseType
		{
			String^		get() {return _EffectBaseType;}
			void		set(String^ value) {_EffectBaseType = value;}
		}
		[DescriptionAttribute("ĳ���� �浹�� ���� �Ӽ�.�´���ġ�Ӽ��ϰ�캻�̾�����⺻����������."),
			CategoryAttribute("��Ÿ �Ӽ�"),
			XmlAttribute("ImpactPosition"),
			TypeConverter(ImpactPositionConverter::typeid)]
		property String^ �浹��ġ���
		{
			String^		get() {return _ImpactPosition;}
			void		set(String^ value) {_ImpactPosition = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�⺻ �Ӽ�"),
			XmlAttribute("����Ʈ ���� �̸�"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ EffectFileName
		{
			String^		get() {return _EffectName;}
			void		set(String^ value) {_EffectName = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("��Ÿ �Ӽ�"),
			XmlAttribute("dir_attr"),
			TypeConverter(DirectionAttributeConverter::typeid)]
		property String^ ����
		{
			String^		get() {return _EffectDirection;}
			void		set(String^ value) {_EffectDirection = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�⺻ �Ӽ�"),
			XmlAttribute("�� �̸�"),
			EditorAttribute(ModelBoneList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ���̸�
		{
			String^		get() {return _BoneName;}
			void		set(String^ value) {_BoneName = value;}
		}
	};

	//------------------------------------------------------------------------
	// ����
	public ref class PartsIndexConverter: public DropDownIntConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			int n = g_pMainApp->BreakablePartsCount();
			ArrayList^ ALPartsIndex = gcnew ArrayList;

			for(int i = 1; i <= n; ++i)
				ALPartsIndex->Add(i);

			return gcnew StandardValuesCollection(ALPartsIndex);
		} 
	};

	public ref class CAPSULE
	{
	public: CAPSULE(){}
	private: 
		float			_radius;
		float			_height;
		float			_1X;
		float			_1Y;
		float			_1Z;
		float			_2X;
		float			_2Y;
		float			_2Z;
	public:
		[DescriptionAttribute("������"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("Radius"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������
		{
			float		get() {return _radius;}
			void		set(float value) {_radius= value;}
		}
		[DescriptionAttribute("����"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("Height"),
			ReadOnlyAttribute(true)]
		property float ����
		{
			float		get() {return _height;}
			void		set(float value) {_height= value;}
		}
		[DescriptionAttribute("������2_X"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("2X"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������2_X
		{
			float		get() {return _2X;}
			void		set(float value) {_2X= value;}
		}
		[DescriptionAttribute("������2_Y"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("2Y"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������2_Y
		{
			float		get() {return _2Y;}
			void		set(float value) {_2Y= value;}
		}
		[DescriptionAttribute("������2_Z"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("2Z"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������2_Z
		{
			float		get() {return _2Z;}
			void		set(float value) {_2Z= value;}
		}
		[DescriptionAttribute("������1_X"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("1X"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������1_X
		{
			float		get() {return _1X;}
			void		set(float value) {_1X= value;}
		}
		[DescriptionAttribute("������1_Y"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("1Y"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������1_Y
		{
			float		get() {return _1Y;}
			void		set(float value) {_1Y= value;}
		}
		[DescriptionAttribute("������1_Z"),
			CategoryAttribute("ĸ�� �Ķ����"),
			XmlAttribute("1Z"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ������1_Z
		{
			float		get() {return _1Z;}
			void		set(float value) {_1Z= value;}
		}
	};

	public ref class PARTSHITCAPSULE: public CAPSULE 
	{
	public: PARTSHITCAPSULE() : _ActiveTimeBegin(0.0f), _ActiveTimeEnd(0.0f)  {}
	private: 
		int			_hitEffectIndex;

		float		_ActiveTimeBegin;
		float		_ActiveTimeEnd;

		int			_LinkedBPart;

	public:
		[DescriptionAttribute("�κ�����ĸ���� ���� �� �浹ĸ���ε���. -1�� ���� ���̴�."),
			CategoryAttribute("��Ʈ����Ʈĸ���ε���"),
			XmlAttribute("HitEffectIndex")]
		property int �ε���
		{
			int			get() {return _hitEffectIndex;}
			void		set(int nIndex) {_hitEffectIndex = nIndex;}
		}

		[DescriptionAttribute("��ȿ ���� �ð�(1=1��)"),
			CategoryAttribute("��Ÿ"),
			XmlAttribute("ActiveTimeBegin")]
		property float a��ȿ���۽ð�
		{
			float			get() {return _ActiveTimeBegin;}
			void			set(float value) {_ActiveTimeBegin = value;}
		}
		[DescriptionAttribute("��ȿ �� �ð�(1=1��)"),
			CategoryAttribute("��Ÿ"),
			XmlAttribute("ActiveTimeEnd")]
		property float b��ȿ���ð�
		{
			float			get() {return _ActiveTimeEnd;}
			void			set(float value) {_ActiveTimeEnd = value;}
		}

		[DescriptionAttribute("����� �극��Ŀ�� ���� (�켱 ����)"),
			CategoryAttribute("��Ÿ"),
			XmlAttribute("LinkedBPart"),
			TypeConverter(PartsIndexConverter::typeid)]
		property int LinkedBPart
		{
			int		get() {return _LinkedBPart;}
			void		set(int value) {_LinkedBPart = value;}
		}
	};

	public ref class MESHCOLCAPSULE: public CAPSULE 
	{
	public: MESHCOLCAPSULE() {}
	private: 
		String^		_BoneName1;
		String^		_BoneName2;
	public:
		[DescriptionAttribute("�ִϸ��̼��� �Ǳ� ���� ������ �ϱ� ���� ��. (����) �̰��� �Է��� �ϸ� ĸ�� ��ǥ���� ���� ��ǥ���� ���� ��ǥ�� ���մϴ�."),
			CategoryAttribute("�ִϸ��̼�"),
			XmlAttribute("BoneName1")]
		property String^ ��1
		{
			String^			get() {return _BoneName1;}
			void			set(String^ value) {_BoneName1 = value;}
		}
		[DescriptionAttribute("�ִϸ��̼��� �Ǳ� ���� ������ �ϱ� ���� ��. (����) �̰��� �Է��� �ϸ� ĸ�� ��ǥ���� ���� ��ǥ���� ���� ��ǥ�� ���մϴ�."),
			CategoryAttribute("�ִϸ��̼�"),
			XmlAttribute("BoneName2")]
		property String^ ��2
		{
			String^			get() {return _BoneName2;}
			void			set(String^ value) {_BoneName2 = value;}
		}

	};

	public ref class COLLISION_BOX
	{
	public: COLLISION_BOX(){}
	private: 
		float			_center_X;
		float			_center_Y;
		float			_center_Z;
		float			_dir_X;
		float			_dir_Y;
		float			_dir_Z;
		float			_dir;
		float			_up_X;
		float			_up_Y;
		float			_up_Z;
		float			_max_X;
		float			_max_Y;
		float			_max_Z;
		float			_min_X;
		float			_min_Y;
		float			_min_Z;

	public:
		[DescriptionAttribute("�߽� ���� x��"),
			CategoryAttribute("Center"),
			XmlAttribute("cneter_x"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float center_x
		{
			float		get() {return _center_X;}
			void		set(float value) {_center_X= value;}
		}
		[DescriptionAttribute("�߽� ���� y��"),
			CategoryAttribute("Center"),
			XmlAttribute("cneter_y"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float center_y
		{
			float		get() {return _center_Y;}
			void		set(float value) {_center_Y= value;}
		}
		[DescriptionAttribute("�߽� ���� z��"),
			CategoryAttribute("Center"),
			XmlAttribute("cneter_z"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float center_z
		{
			float		get() {return _center_Z;}
			void		set(float value) {_center_Z= value;}
		}
		[DescriptionAttribute("���� x��"),
			CategoryAttribute("Dir"),
			XmlAttribute("dir_x"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float dir_x
		{
			float		get() {return _dir_X;}
			void		set(float value) {_dir_X= value;}
		}
		[DescriptionAttribute("���� y��"),
			CategoryAttribute("Dir"),
			XmlAttribute("dir_y"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float dir_y
		{
			float		get() {return _dir_Y;}
			void		set(float value) {_dir_Y= value;}
		}
		[DescriptionAttribute("���� z��"),
			CategoryAttribute("Dir"),
			XmlAttribute("dir_z"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float dir_z
		{
			float		get() {return _dir_Z;}
			void		set(float value) {_dir_Z= value;}
		}
		//[DescriptionAttribute("���Ⱚ"),
		//	CategoryAttribute("Dir"),
		//	XmlAttribute("dir"),
		//	EditorAttribute(MyValueEditorDegree::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		//property float dir
		//{
		//	float		get() {return _dir;}
		//	void		set(float value) {_dir= value;}
		//}

		[DescriptionAttribute("Up x��"),
			CategoryAttribute("Up"),
			XmlAttribute("up_x"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float up_x
		{
			float		get() {return _up_X;}
			void		set(float value) {_up_X= value;}
		}
		[DescriptionAttribute("Up y��"),
			CategoryAttribute("Up"),
			XmlAttribute("up_y"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float up_y
		{
			float		get() {return _up_Y;}
			void		set(float value) {_up_Y= value;}
		}
		[DescriptionAttribute("Up z��"),
			CategoryAttribute("Up"),
			XmlAttribute("up_z"),
			EditorAttribute(MyValueEditor1::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float up_z
		{
			float		get() {return _up_Z;}
			void		set(float value) {_up_Z= value;}
		}

		[DescriptionAttribute("X�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("min_x"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float min_x
		{
			float		get() {return _min_X;}
			void		set(float value) {_min_X= value;}
		}
		[DescriptionAttribute("Y�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("min_y"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float min_y
		{
			float		get() {return _min_Y;}
			void		set(float value) {_min_Y= value;}
		}
		[DescriptionAttribute("Z�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("min_z"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float min_z
		{
			float		get() {return _min_Z;}
			void		set(float value) {_min_Z= value;}
		}
		[DescriptionAttribute("X�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("max_x"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float max_x
		{
			float		get() {return _max_X;}
			void		set(float value) {_max_X= value;}
		}
		[DescriptionAttribute("Y�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("max_y"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float max_y
		{
			float		get() {return _max_Y;}
			void		set(float value) {_max_Y= value;}
		}
		[DescriptionAttribute("Z�� ����"),
			CategoryAttribute("����"),
			XmlAttribute("max_z"),
			EditorAttribute(MyValueEditor2::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float max_z
		{
			float		get() {return _max_Z;}
			void		set(float value) {_max_Z= value;}
		}

	};
	//----------------------------------------------------------------------------//
	// ����Ʈ Ÿ��
	public ref class EventTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALEventType = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetTalentEventTypeList(ALEventType);

			return gcnew StandardValuesCollection(ALEventType);
		} 
	};
	//----------------------------------------------------------------------------//
	// custom Ÿ��
	public ref class CustomEventTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALEventType = gcnew ArrayList;
			g_pMainApp->GetEffectInfoList(ALEventType);

			return gcnew StandardValuesCollection(ALEventType);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};
	//----------------------------------------------------------------------------//
	// �߻�ü Ÿ��
	public ref class ProjectileTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ProjectileType = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetProjectileType(ProjectileType);

			return gcnew StandardValuesCollection(ProjectileType);
		} 
	};
	//----------------------------------------------------------------------------//
	// ����� Ÿ��
	public ref class ProjectileVisualTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ProjectileVisualType = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetProjectileVisualType(ProjectileVisualType);

			return gcnew StandardValuesCollection(ProjectileVisualType);
		} 
	};
	//----------------------------------------------------------------------------//
	// ���׸�Ʈ�ε��� Ÿ��
	public ref class SegmentIndexConverter: public DropDownIntConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ SetmentIndexArray = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetTalentHitInfoSegmentIndex(SetmentIndexArray);

			return gcnew StandardValuesCollection(SetmentIndexArray);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	// ĸ���ε��� Ÿ��
	public ref class CapsuleIndexConverter: public DropDownIntConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ CapsuleIndexArray = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetTalentHitInfoCapsuleIndex(CapsuleIndexArray, 2);

			return gcnew StandardValuesCollection(CapsuleIndexArray);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	// ĸ���ε��� Ÿ��(����Ʈ)
	public ref class CapsuleIndexConverterByEffect: public DropDownIntConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ CapsuleIndexArray = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetTalentHitInfoCapsuleIndex(CapsuleIndexArray, 3);

			return gcnew StandardValuesCollection(CapsuleIndexArray);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	// ĸ���ε��� Ÿ��(sh_custom)
	public ref class CapsuleIndexConverterByCustom: public DropDownIntConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ CapsuleIndexArray = gcnew ArrayList;
			GlobalObjects::g_pMainForm->m_EventTabPage->m_TalentEventController->GetTalentHitInfoCapsuleIndex(CapsuleIndexArray, 7);

			return gcnew StandardValuesCollection(CapsuleIndexArray);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};

	//------------------------------------------------------------------------
	// �⺻ �̺�Ʈ
	public ref class TALENTEVENT
	{
	public: TALENTEVENT(){}

	private:
		String^		_EventType;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("Ÿ��"),
			XmlAttribute("type"),
			TypeConverter(EventTypeConverter::typeid)]
		property String^ �̺�ƮŸ��
		{
			String^		get() {return _EventType;}
			void		set(String^ value) {_EventType = value;}
		}
	};
	//------------------------------------------------------------------------
	// Ÿ�� ����
	public ref class TALENTEVENT_NONE : public TALENTEVENT
	{
	public: TALENTEVENT_NONE(){}

	private: 
		float		_fTime;
		String^		_strParam1;
		String^		_strParam2;
		int			_nParam1;
		int			_nParam2;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1")]
		property String^ ���ڿ�1
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam2")]
		property String^ ���ڿ�2
		{
			String^		get() {return _strParam2;}
			void		set(String^ value) {_strParam2 = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam1")]
		property int ����1
		{
			int			get() {return _nParam1;}
			void		set(int value) {_nParam1 = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam2")]
		property int ����2
		{
			int			get() {return _nParam2;}
			void		set(int value) {_nParam2 = value;}
		}

	};

	//------------------------------------------------------------------------
	// �߻�
	public ref class TALENTEVENT_FIRE_EFFECT : public TALENTEVENT
	{
	public: TALENTEVENT_FIRE_EFFECT(){}

	private: 
		float		_fTime;
		float		_fTime2;
		String^		_strParam1;
		String^		_strParam2;
		int			_nParam2;

	public:
		[DescriptionAttribute("����Ʈ �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1"),
			TypeConverter(EffectNameConverter::typeid),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ����Ʈ�̸�
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute("������ ���� ������... �⺻ �� ��ġ�Դϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam2")]
		property String^ ���̸�
		{
			String^		get() {return _strParam2;}
			void		set(String^ value) {_strParam2 = value;}
		}
		[DescriptionAttribute("Loop ����Ʈ�ϰ�쿡�� ���Դϴ�. Loop ����Ʈ�ε� ������ ���� ������ �ŷ�Ʈ�� ���� ������ �������� �ʽ��ϴ�."),
			CategoryAttribute("�ð�"),
			XmlAttribute("time2")]
		property float ����ð�
		{
			float		get() {return _fTime2;}
			void		set(float value) {_fTime2 = value;}
		}
		[DescriptionAttribute("Loop ����Ʈ�� ������� ����Դϴ�. '1'�̸� ��� �����, '0'�̸� ������ �����"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam2")]
		property int �������
		{
			int			get() {return _nParam2;}
			void		set(int value) {_nParam2 = value;}
		}
	};

	//------------------------------------------------------------------------
	// �߻�ü
	public ref class TALENTEVENT_PROJECTILE : public TALENTEVENT
	{
	public: TALENTEVENT_PROJECTILE(){}

	private: 
		String^		_ProjectileType;
		String^		_ProjectileVisualType;
		String^		_BoneName;

		String^		_ProjectileEffect;
		String^		_AttackEffect;
		String^		_HitGroundEffect;
		String^		_DefenceEffect;
		String^		_FireEffect;

		int			_SegmentIndex;
		int			_CapsuleIndex;

		float		_fSpeed;
		float		_fHitRadius;
		float		_fRange;
	
		float		_fTime;

		float		_fx;
		float		_fy;
		float		_fz;

	public:
		[DescriptionAttribute("��ġ�����̴�."),
			CategoryAttribute("�߻�ü �Ӽ�"),
			XmlAttribute("hitRadius")]
		property float ����ũ��
		{
			float		get() {return _fHitRadius;}
			void		set(float value) {_fHitRadius = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�߻�ü �Ӽ�"),
			XmlAttribute("speed")]
		property float ���ǵ�
		{
			float		get() {return _fSpeed;}
			void		set(float value) {_fSpeed = value;}
		}
		[DescriptionAttribute("���ʹ����̴�."),
			CategoryAttribute("�߻�ü �Ӽ�"),
			XmlAttribute("range")]
		property float ��Ÿ�
		{
			float		get() {return _fRange;}
			void		set(float value) {_fRange = value;}
		}
		[DescriptionAttribute("�߻�ü�� ���ư��� ����̴�."),
			CategoryAttribute("�߻�ü �Ӽ�"),
			XmlAttribute("ProjectilVisualType"),
			TypeConverter(ProjectileVisualTypeConverter::typeid)]
		property String^ �����Ÿ��
		{
			String^		get() {return _ProjectileVisualType;}
			void		set(String^ value) {_ProjectileVisualType = value;}
		}
		[DescriptionAttribute("�ŷ�Ʈ ȿ����ġ���� ���� ���� ������ �ֽ��ϴ�."),
			CategoryAttribute("�߻�ü �Ӽ�"),
			XmlAttribute("ProjectilType"),
		TypeConverter(ProjectileTypeConverter::typeid)]
		property String^ �߻�üŸ��
		{
			String^		get() {return _ProjectileType;}
			void		set(String^ value) {_ProjectileType = value;}
		}
		[DescriptionAttribute("�̺�Ʈ�� Act�� �ؼ� ����� �ּ���!!"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float �߻�ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("���� �𵨷� �ϼž� ������ �˴ϴ�. ���� ���� �����Դϴ�."),
			CategoryAttribute("��ġ �Ӽ�"),
			XmlAttribute("z")]
		property float z
		{
			float		get() {return _fz;}
			void		set(float value) {_fz = value;}
		}
		[DescriptionAttribute("���� �𵨷� �ϼž� ������ �˴ϴ�. ���� ���� �����Դϴ�."),
			CategoryAttribute("��ġ �Ӽ�"),
			XmlAttribute("x")]
		property float x
		{
			float		get() {return _fx;}
			void		set(float value) {_fx = value;}
		}
		[DescriptionAttribute("���� �𵨷� �ϼž� ������ �˴ϴ�. ���� ���� �����Դϴ�."),
			CategoryAttribute("��ġ �Ӽ�"),
			XmlAttribute("y")]
		property float y
		{
			float		get() {return _fy;}
			void		set(float value) {_fy = value;}
		}
		[DescriptionAttribute("���� �𵨷� �ϼž� ������ �˴ϴ�. ���� ���� �����Դϴ�."),
			CategoryAttribute("��ġ �Ӽ�"),
			XmlAttribute("�� �̸�"),
			EditorAttribute(ModelBoneList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ���̸�
		{
			String^		get() {return _BoneName;}
			void		set(String^ value) {_BoneName = value;}
		}
		[DescriptionAttribute("�߻�üŸ���� \"hitcapsule_guided\"�϶����� ���Դϴ�."),
			CategoryAttribute("���� ����"),
			XmlAttribute("CapsuleIndex"),
			TypeConverter(CapsuleIndexConverter::typeid)]
		property int ĸ���ε���
		{
			int			get() {return _CapsuleIndex;}
			void		set(int value) {_CapsuleIndex = value;}
		}
		[DescriptionAttribute("�߻�üŸ���� \"hitcapsule_guided\"�϶����� ���Դϴ�."),
			CategoryAttribute("���� ����"),
			XmlAttribute("SegmentIndex"),
			TypeConverter(SegmentIndexConverter::typeid)]
		property int �����ε���
		{
			int			get() {return _SegmentIndex;}
			void		set(int value) {_SegmentIndex = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("HitGroundEffect"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ �����ǰ�
		{
			String^		get() {return _HitGroundEffect;}
			void		set(String^ value) {_HitGroundEffect = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("DefenceEffect"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ���
		{
			String^		get() {return _DefenceEffect;}
			void		set(String^ value) {_DefenceEffect = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("AttackEffect"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ �ǰ�
		{
			String^		get() {return _AttackEffect;}
			void		set(String^ value) {_AttackEffect = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("FireEffect"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ �߻�
		{
			String^		get() {return _FireEffect;}
			void		set(String^ value) {_FireEffect = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("Projectile"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ �߻�ü
		{
			String^		get() {return _ProjectileEffect;}
			void		set(String^ value) {_ProjectileEffect = value;}
		}
	};

	//------------------------------------------------------------------------
	// ����Ʈ
	public ref class TALENTEVENT_EFFECT : public TALENTEVENT
	{
	public: TALENTEVENT_EFFECT(){}

	private: 
		float		_fTime;
		float		_fTime2;
		String^		_strParam1;
		String^		_strParam2;
		int			_nParam2;
		bool		_bFollow;

		int			_SegmentIndex;
		int			_CapsuleIndex;

	public:
		[DescriptionAttribute("����Ʈ �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("����Ʈ Ȯ����(.Effect.xml)�� ���ּ���"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1")]
		property String^ ����Ʈ�̸�
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute("������ ���� ������... �⺻ �� ��ġ�Դϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam2")]
		property String^ ���̸�
		{
			String^		get() {return _strParam2;}
			void		set(String^ value) {_strParam2 = value;}
		}
		[DescriptionAttribute("Loop ����Ʈ�ϰ�쿡�� ���Դϴ�. Loop ����Ʈ�ε� ������ ���� ������ �ŷ�Ʈ�� ���� ������ �������� �ʽ��ϴ�."),
			CategoryAttribute("�ð�"),
			XmlAttribute("time2")]
		property float ����ð�
		{
			float		get() {return _fTime2;}
			void		set(float value) {_fTime2 = value;}
		}
		[DescriptionAttribute("Loop ����Ʈ�� ������� ����Դϴ�. '1'�̸� ��� �����, '0'�̸� ������ �����"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam2")]
		property int �������
		{
			int			get() {return _nParam2;}
			void		set(int value) {_nParam2 = value;}
		}
		[DescriptionAttribute("����Ʈ�� ���� ���󰡰� �ϴ� ���̴�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("Follow")]
		property bool Follow
		{
			bool		get() {return _bFollow;}
			void		set(bool value) {_bFollow = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("���� ����"),
			XmlAttribute("CapsuleIndex"),
			TypeConverter(CapsuleIndexConverterByEffect::typeid)]
		property int ĸ���ε���
		{
			int			get() {return _CapsuleIndex;}
			void		set(int value) {_CapsuleIndex = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("���� ����"),
			XmlAttribute("SegmentIndex"),
			TypeConverter(SegmentIndexConverter::typeid)]
		property int �����ε���
		{
			int			get() {return _SegmentIndex;}
			void		set(int value) {_SegmentIndex = value;}
		}
	};

	//------------------------------------------------------------------------
	// ����
	public ref class TALENTEVENT_SOUND : public TALENTEVENT
	{
	public: TALENTEVENT_SOUND(){}

	private: 
		float		_fTime;
		String^		_strParam1;
		String^		_strParam2;
		int			_nParam1;

	public:
		[DescriptionAttribute("���� �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time"),
			EditorAttribute(MyValueEditor3::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("���� �̸��� �Է��ϼ���"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1"),
			TypeConverter(SoundNameConverter::typeid)]
		property String^ �����̸�
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute("������ ���� ������... �⺻ �� ��ġ�Դϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam2")]
		property String^ ���̸�
		{
			String^		get() {return _strParam2;}
			void		set(String^ value) {_strParam2 = value;}
		}
		[DescriptionAttribute("Loop �����ϰ�쿡�� ���Դϴ�. Loop �����ε� ������ ���� ������ �ŷ�Ʈ�� ���� ������ �������� �ʽ��ϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam1"),
			EditorAttribute(MyValueEditor3::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property int ����ð�
		{
			int			get() {return _nParam1;}
			void		set(int value) {_nParam1 = value;}
		}
	};

	//------------------------------------------------------------------------
	// ī�޶�
	public ref class TALENTEVENT_CAMERA : public TALENTEVENT
	{
	public: TALENTEVENT_CAMERA(){}

	private: 
		float		_fTime;
		float		_fTime2;
		String^		_strParam1;
		int			_nParam1;

	public:
		[DescriptionAttribute("ī�޶� ȿ�� �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("ī�޶� ����Ʈ Ȯ����(.elu.ani)�� ���ּ���"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1")]
		property String^ ī�޶�ȿ���̸�
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute("�ݰ��� cm�� ǥ���� �ּ���(��:100 -> 1����)"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam1")]
		property int �ݰ�
		{
			int			get() {return _nParam1;}
			void		set(int value) {_nParam1 = value;}
		}
		[DescriptionAttribute("Loop ī�޶� ȿ���ϰ�쿡�� ���Դϴ�. Loop ī�޶� ȿ���ε� ������ ���� ������ �ŷ�Ʈ�� ���� ������ �������� �ʽ��ϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("intparam2")]
		property float ����ð�
		{
			float		get() {return _fTime2;}
			void		set(float value) {_fTime2 = value;}
		}
	};
	//------------------------------------------------------------------------
	// �ǰ�
	public ref class TALENTEVENT_DAMAGE : public TALENTEVENT
	{
	public: TALENTEVENT_DAMAGE(){}

	private:
		float		_fTime;
		int			_nRate;
		String^		_strEffect;
		String^		_strBone;

	public:
		[DescriptionAttribute("������ ȿ�� �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float �������ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("�ǰ� ����Ʈ �� ��ġ"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("Bone")]
		property String^ ��
		{
			String^		get() {return _strBone;}
			void		set(String^ value) {_strBone = value;}
		}
		[DescriptionAttribute("�ǰ��ڿ��� ������ �ǰ� ����Ʈ"),
			CategoryAttribute("����Ʈ"),
			XmlAttribute("effect"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ ����Ʈ
		{
			String^		get() {return _strEffect;}
			void		set(String^ value) {_strEffect = value;}
		}
		[DescriptionAttribute("�� �ǰݽ� ���� ������������ ũ���Դϴ�. ������ ũ��� (%)�� ǥ���մϴ�."),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("Rate")]
		property int Rate
		{
			int			get() {return _nRate;}
			void		set(int value) {_nRate = value;}
		}
	};
	//------------------------------------------------------------------------
	// sh_costom
	public ref class TALENTEVENT_CUSTOM : public TALENTEVENT
	{
	public: TALENTEVENT_CUSTOM(){}

	private: 
		float		_fTime;
		float		_fTime2;
		String^		_strEventName;
		String^		_strParam;
		String^		_strBone;

		int			_SegmentIndex;
		int			_CapsuleIndex;

		bool		_bServerDir;

	public:
		[DescriptionAttribute("�̺�Ʈ �̸�"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("EventName"),
			EditorAttribute(CustomEffectList::typeid, System::Drawing::Design::UITypeEditor::typeid)]
		property String^ �̺�Ʈ�̸�
		{
			String^		get() {return _strEventName;}
			void		set(String^ value) {_strEventName = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam2")]
		property String^ PARAM
		{
			String^		get() {return _strParam;}
			void		set(String^ value) {_strParam = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("Bone")]
		property String^ ��
		{
			String^		get() {return _strBone;}
			void		set(String^ value) {_strBone = value;}
		}
		[DescriptionAttribute("�̺�Ʈ �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("starttime")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("�̺�Ʈ ���� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("endtime")]
		property float ����ð�
		{
			float		get() {return _fTime2;}
			void		set(float value) {_fTime2 = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("���� ����"),
			XmlAttribute("CapsuleIndex"),
			TypeConverter(CapsuleIndexConverterByCustom::typeid)]
		property int ĸ���ε���
		{
			int			get() {return _CapsuleIndex;}
			void		set(int value) {_CapsuleIndex = value;}
		}
		[DescriptionAttribute(""),
			CategoryAttribute("���� ����"),
			XmlAttribute("SegmentIndex"),
			TypeConverter(SegmentIndexConverter::typeid)]
		property int �����ε���
		{
			int			get() {return _SegmentIndex;}
			void		set(int value) {_SegmentIndex = value;}
		}
		[DescriptionAttribute("���� �������� �߻��մϴ�. Follow�� false�� ���ּ���"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("Follow")]
		property bool ServerDir
		{
			bool		get() {return _bServerDir;}
			void		set(bool value) {_bServerDir = value;}
		}
	};
	//------------------------------------------------------------------------
	// ������
	public ref class TALENTEVENT_EXTRA_ACTIVE : public TALENTEVENT
	{
	public: TALENTEVENT_EXTRA_ACTIVE(){}

	private: 
		float		_fTime;
		float		_fx;
		float		_fy;

	public:
		[DescriptionAttribute("�̺�Ʈ �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
				[DescriptionAttribute("�̺�Ʈ �߻� ��ġ"),
			CategoryAttribute("��ġ"),
			XmlAttribute("x")]
		property float x
		{
			float		get() {return _fx;}
			void		set(float value) {_fx = value;}
		}
		[DescriptionAttribute("�̺�Ʈ �߻� ��ġ"),
			CategoryAttribute("��ġ"),
			XmlAttribute("y")]
		property float y
		{
			float		get() {return _fy;}
			void		set(float value) {_fy = value;}
		}
	};

	//------------------------------------------------------------------------
	// ī�޶� ��
	public ref class TALENTEVENT_CAMERA_LOCK : public TALENTEVENT
	{
	public: TALENTEVENT_CAMERA_LOCK(){}

	private: 
		float		_fStartTime;
		float		_fRadius;
		float		_fTime;
		String^		_strBone;

	public:
		[DescriptionAttribute(""),
			CategoryAttribute("�ð�"),
			XmlAttribute("starttime")]
		property float ���۽ð�
		{
			float		get() {return _fStartTime;}
			void		set(float value) {_fStartTime = value;}
		}
		[DescriptionAttribute("�ݰ��� cm�� ǥ���� �ּ���(��:100 -> 1����)"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("radius")]
		property float �ݰ�
		{
			float		get() {return _fRadius;}
			void		set(float value) {_fRadius = value;}
		}
		[DescriptionAttribute("���� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���ӽð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("�ٶ� �� �� �̸�"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("_strBone")]
		property String^ ��
		{
			String^		get() {return _strBone;}
			void		set(String^ value) {_strBone = value;}
		}
	};

	//------------------------------------------------------------------------
	// ���� �߻�
	public ref class TALENTEVENT_GROUND_FIRE_EFFECT : public TALENTEVENT
	{
	public: TALENTEVENT_GROUND_FIRE_EFFECT(){}

	private: 
		float		_fTime;
		float		_fTime2;
		String^		_strParam1;

	public:
		[DescriptionAttribute("����Ʈ �߻� �ð�"),
			CategoryAttribute("�ð�"),
			XmlAttribute("time")]
		property float ���۽ð�
		{
			float		get() {return _fTime;}
			void		set(float value) {_fTime = value;}
		}
		[DescriptionAttribute("[effect.xml] �� �ִ� Effect Name�� ������"),
			CategoryAttribute("�Ӽ�"),
			XmlAttribute("strparam1"),
			TypeConverter(EffectNameConverter::typeid)]
		property String^ ����Ʈ�̸�
		{
			String^		get() {return _strParam1;}
			void		set(String^ value) {_strParam1 = value;}
		}
		[DescriptionAttribute("Loop ����Ʈ�ϰ�쿡�� ���Դϴ�. Loop ����Ʈ�ε� ������ ���� ������ �ŷ�Ʈ�� ���� ������ �������� �ʽ��ϴ�."),
			CategoryAttribute("�ð�"),
			XmlAttribute("time2")]
		property float ����ð�
		{
			float		get() {return _fTime2;}
			void		set(float value) {_fTime2 = value;}
		}
	};

	//------------------------------------------------------------------------
	// �޽� ����
	public ref class MeshInfoAnimationTypeConverter: public DropDownStringConverter 
	{  

	public:
		virtual TypeConverter::StandardValuesCollection^ GetStandardValues(ITypeDescriptorContext^ context) override
		{
			ArrayList^ ALAnimation = gcnew ArrayList;
			g_pMainApp->GetAnimationList(ALAnimation);
			
			return gcnew StandardValuesCollection(ALAnimation);
		} 

		virtual bool GetStandardValuesExclusive(ITypeDescriptorContext^ context) override
		{
			return false;
		}
	};


	public ref class MESHINFO
	{
	public: MESHINFO(){}
	private: 
		String^		_StartSound1;
		String^		_CollisionSound1;

		String^		_StartSound2;
		String^		_CollisionSound2;

		String^		_StartSound3;
		String^		_CollisionSound3;

		String^		_StartSound4;
		String^		_CollisionSound4;

	public:
		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �浹�Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("collision_sound")]
		property String^ �����浹��_04
		{
			String^	get() {return _CollisionSound4;}
			void	set(String^ value) {_CollisionSound4=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �����Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("start_sound")]
		property String^ �ı���_04
		{
			String^	get() {return _StartSound4;}
			void	set(String^ value) {_StartSound4=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �浹�Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("collision_sound")]
		property String^ �����浹��_03
		{
			String^	get() {return _CollisionSound3;}
			void	set(String^ value) {_CollisionSound3=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �����Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("start_sound")]
		property String^ �ı���_03
		{
			String^	get() {return _StartSound3;}
			void	set(String^ value) {_StartSound3=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �浹�Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("collision_sound")]
		property String^ �����浹��_02
		{
			String^	get() {return _CollisionSound2;}
			void	set(String^ value) {_CollisionSound2=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �����Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("start_sound")]
		property String^ �ı���_02
		{
			String^	get() {return _StartSound2;}
			void	set(String^ value) {_StartSound2=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �浹�Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("collision_sound")]
		property String^ �����浹��_01
		{
			String^	get() {return _CollisionSound1;}
			void	set(String^ value) {_CollisionSound1=value;}
		}

		[ReadOnly(true)]
		[DescriptionAttribute("�극��Ŀ�� ���� �����Ҷ� ����"),
			CategoryAttribute("Breakable Parts"),
			XmlAttribute("start_sound")]
		property String^ �ı���_01
		{
			String^	get() {return _StartSound1;}
			void	set(String^ value) {_StartSound1=value;}
		}
	};
}
