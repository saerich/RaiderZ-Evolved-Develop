/*
 * SoulHunt.cs
 * SoulHunt와 관련된 각종 자료형들을 구현
 */
#region Using문

using System;
using System.ComponentModel;
using System.Linq;
using System.Xml;
using System.Xml.Serialization;
using System.Collections;
using System.Collections.Generic;

#endregion

namespace SoulHunt
{
    #region SHConditionsXml
    
    [Serializable]
    [XmlRoot("maiet")]
    public sealed class SHConditionsXml : SHXmlDataObject
    {
        [XmlElement(Type = typeof(SHConditions), ElementName="CONDITIONS")]
        public ArrayList dataList;
        
        private Dictionary<int, SHConditions> Index;

        public override bool IsValid(int nIndex)
        {
            return Index.ContainsKey(nIndex);
        }

        public override bool IsValid(String szIndex)
        {
            if (szIndex == null) return false;
            try
            {
                return Index.ContainsKey(Int32.Parse(szIndex));
            }
            catch (FormatException)
            {
                return false;
            }
        }

        public SHConditions this[int nIndex]
        {
            get
            {
                try
                {
                    return Index[nIndex];
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
        }

        public SHConditions this[string szIndex]
        {
            get
            {
                try
                {
                    int nIndex = Convert.ToInt32(szIndex);
                    return Index[nIndex];
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
        }

        public SHConditionsXml() : base()
        {
            dataList = new ArrayList();
            Index = new Dictionary<int, SHConditions>();
        }

        [XmlInclude(typeof(SHConditions))]
        public void Add(SHConditions i)
        {
            dataList.Add(i);
        }

        public override bool Compile()
        {
            _bPassed = true;

            foreach (SHConditions conds in dataList)
            {
                Index[conds.id] = conds;
                if (!conds.Compile()) _bPassed = false;
            }

            _szCompiled = (_bPassed) ? SHGlobal.COK : SHGlobal.CERROR;
            return _bPassed;
        }

        public override bool Build(SHXmlCore core)
        {
            foreach (SHConditions conds in dataList) if (!conds.Build(core)) _bPassed = false;
            _szCompiled = (_bPassed) ? SHGlobal.COK : SHGlobal.CERROR;
            return _bPassed;
        }

        public override string ToBody(SHXmlCore core)
        {
            throw new NotImplementedException();
        }

        class ConditionsComparer : IComparer
        {
            public ConditionsComparer()
            {
            }

            public int Compare(object x, object y)
            {
                SHConditions qFirst = (SHConditions)x;
                SHConditions qSecond = (SHConditions)y;

                return qFirst.id.CompareTo(qSecond.id);
            }
        }

        public override void Sort()
        {
            dataList.Sort(new ConditionsComparer());
        }

    }

    [Serializable]
    public sealed class SHConditions : SHDataObject
    {
        private int _id;
        [XmlAttribute]
        [Category("공통"), DescriptionAttribute("조건을 구분할 수 있는 id를 지정합니다. 이 값은 유일해야하며, 유일하지 않다면 문제가 발생될 수 있습니다.(Velixian은 이를 확인하지 못합니다.)"), DisplayName("(아이디)")]
        public int id
        {
            get { return _id; }
            set { idSpecified = (value != 0) ? true : false; _id = value; }
        }
        [XmlIgnoreAttribute]
        public bool idSpecified;

        private string _comment;
        [XmlAttribute, Editor(typeof(VCETextEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("공통"), DescriptionAttribute("조건에는 별도의 타이틀이 없기 때문에 주석을 설정하여 편집에 편리하도록 합니다. 이 값은 게임에서 참조되지 않습니다."), DisplayName("(주석)")]
        public string comment
        {
            get { return _comment; }
            set { _comment = (value != String.Empty) ? value : null; }
        }

        [XmlElement("AND")]
        public SHAnd and;

        [XmlElement("OR")]
        public SHOr or;

        [XmlElement(Type = typeof(SHCondition), ElementName = "CONDITION")]
        public ArrayList dataList;

        public SHConditions():base()
        {
            id = 0;
            comment = "";
            dataList = new ArrayList();
        }

        public SHConditions(int nID, string szComment, int nLineNumber):base()
        {
            id = nID;
            comment = szComment;
            dataList = new ArrayList();
        }

        [XmlInclude(typeof(SHCondition))]
        public void Add(SHCondition newCond)
        {
            dataList.Add(newCond);
        }

        public void Remove(SHCondition cond)
        {
            dataList.Remove(cond);
        }

        public SHCondition this[int nIndex]
        {
            get
            {
                return (SHCondition)this.dataList[nIndex];
            }
        }

        public override bool Compile()
        {
            _szCompiled = SHGlobal.COK;
            _bPassed = true;

            if (dataList.Count <= 0 && null == or && null == and)
            {
                return Error("CONDITION, AND, OR이 하나도 포함되어 있지 않습니다.");
            }

            if (null != and)
            {
                if (!and.Compile()) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Compile()) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Compile()) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override bool Build(SHXmlCore core)
        {
            if (!_bPassed) return _bPassed;

            if (null != and)
            {
                if (!and.Build(core)) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Build(core)) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Build(core)) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override string ToBody(SHXmlCore core)
        {
            throw new NotImplementedException();
        }

        public string GetString(SHXmlCore core)
        {
            string ret = id.ToString();            

            return ret;
        }

        public void Assign(SHConditions srcConds)
        {
            _comment = srcConds._comment;

            if (srcConds.and != null)
                this.and = (SHAnd)srcConds.and.Clone();
            else this.and = null;

            if (srcConds.or != null)
                this.or = (SHOr)srcConds.or.Clone();
            else this.or = null;


            dataList.Clear();

            foreach (SHCondition obj in srcConds.dataList)
            {
                dataList.Add((SHCondition)obj.Clone());
            }
        }
    }
    #endregion

    public sealed class SHAnd : SHDataObject
    {
        [XmlElement(Type = typeof(SHCondition), ElementName = "CONDITION")]
        public ArrayList dataList;

        [XmlElement("AND")]
        public SHAnd and;

        [XmlElement("OR")]
        public SHOr or;

        public SHAnd()
        {
            dataList = new ArrayList();
        }

        public override bool Compile()
        {
            _szCompiled = SHGlobal.COK;
            _bPassed = true;

            if (dataList.Count <= 0 && null == or && null == and)
            {
                return Error("CONDITION, AND, OR이 하나도 포함되어 있지 않습니다.");
            }

            if (null != and)
            {
                if (!and.Compile()) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Compile()) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Compile()) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override bool Build(SHXmlCore core)
        {
            if (!_bPassed) return _bPassed;

            if (null != and)
            {
                if (!and.Build(core)) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Build(core)) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Build(core)) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override string ToBody(SHXmlCore core)
        {
            throw new NotImplementedException();
        }

        public string GetString(SHXmlCore core)
        {
            string ret = "AND";

            return ret;
        }

        public object Clone()
        {
            SHAnd clone = new SHAnd();


            if (this.and != null)
                clone.and = (SHAnd)this.and.Clone();
            else clone.and = null;

            if (this.or != null)
                clone.or = (SHOr)this.or.Clone();
            else clone.or = null;


            foreach (SHCondition obj in dataList)
            {
                clone.dataList.Add((SHCondition)obj.Clone());
            }

            return clone;
        }

    }

    public sealed class SHOr : SHDataObject
    {
        [XmlElement(Type = typeof(SHCondition), ElementName = "CONDITION")]
        public ArrayList dataList;

        [XmlElement("AND")]
        public SHAnd and;

        [XmlElement("OR")]
        public SHOr or;

        public SHOr()
        {
            dataList = new ArrayList();
        }

        public override bool Compile()
        {
            _szCompiled = SHGlobal.COK;
            _bPassed = true;

            if (dataList.Count <= 0 && null == or && null == and)
            {
                return Error("CONDITION, AND, OR이 하나도 포함되어 있지 않습니다.");
            }

            if (null != and)
            {
                if (!and.Compile()) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Compile()) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Compile()) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override bool Build(SHXmlCore core)
        {
            if (!_bPassed) return _bPassed;

            if (null != and)
            {
                if (!and.Build(core)) return Error(and.CompiledMessage);
            }

            if (null != or)
            {
                if (!or.Build(core)) return Error(or.CompiledMessage);
            }

            foreach (SHCondition cond in dataList) if (!cond.Build(core)) return Error(cond.CompiledMessage);

            return _bPassed;
        }

        public override string ToBody(SHXmlCore core)
        {
            throw new NotImplementedException();
        }

        public string GetString(SHXmlCore core)
        {
            string ret = "OR";

            return ret;
        }

        public object Clone()
        {
            SHOr clone = new SHOr();

            if (this.and != null)
                clone.and = (SHAnd)this.and.Clone();
            else clone.and = null;

            if (this.or != null)
                clone.or = (SHOr)this.or.Clone();
            else clone.or = null;

            foreach (SHCondition obj in dataList)
            {
                clone.dataList.Add((SHCondition)obj.Clone());
            }

            return clone;
        }
    }  


    [Serializable]
    public sealed class SHCondition : SHDataObject
    {
        [Serializable]
        //[TypeConverter(typeof(SHCT_CondType))]
        public enum ConditionType
        {
            level,
            quest,
            quest_var,
            quest_objective_complete,
            quest_objective_incomplete,
            item,
            noitem,
            manyitem,
            fewitem,
            equip,
            race,
            sex,
            faction,
            title,
            date,
            talent,
            talent_style_tp,
            inn,
            kill,
            qpvp_team,
        }

        public sealed class SHCTString
        {
            [XmlIgnore]
            public SHCondition.ConditionType Type;
            [XmlIgnore]
            public String Text;

            public SHCTString(SHCondition.ConditionType type, String text)
            {
                Type = type;
                Text = text;
            }
        }

        [XmlIgnore]
        public static SHCTString[] TypeNaming = new SHCTString[] {
            new SHCTString(SHCondition.ConditionType.level, "레벨"),
            new SHCTString(SHCondition.ConditionType.quest, "퀘스트"),
            new SHCTString(SHCondition.ConditionType.quest_var, "퀘스트 변수"),
            new SHCTString(SHCondition.ConditionType.quest_objective_complete, "퀘스트 목적 완료"),
            new SHCTString(SHCondition.ConditionType.quest_objective_incomplete, "퀘스트 목적 미완료"),
            new SHCTString(SHCondition.ConditionType.item, "아이템 소지"),
            new SHCTString(SHCondition.ConditionType.noitem, "아이템 없음"),
            new SHCTString(SHCondition.ConditionType.manyitem, "아이템 초과"),
            new SHCTString(SHCondition.ConditionType.fewitem, "아이템 미만"),
            new SHCTString(SHCondition.ConditionType.equip, "아이템 장착"),
            new SHCTString(SHCondition.ConditionType.race, "종족"),
            new SHCTString(SHCondition.ConditionType.sex, "성별"),
            new SHCTString(SHCondition.ConditionType.faction, "우호도"),
            new SHCTString(SHCondition.ConditionType.title, "타이틀"),
            new SHCTString(SHCondition.ConditionType.date, "기간한정(이벤트)"),
            new SHCTString(SHCondition.ConditionType.talent, "탤런트"),
            new SHCTString(SHCondition.ConditionType.talent_style_tp, "스타일 TP"),
            new SHCTString(SHCondition.ConditionType.inn, "여관"),
            new SHCTString(SHCondition.ConditionType.kill, "죽이기"),
            new SHCTString(SHCondition.ConditionType.qpvp_team, "QPVP팀"),
        };

        private SHCondition.ConditionType _type;
        [XmlAttribute]
        [Category("기본"), DescriptionAttribute("조건문의 종류를 정의합니다. 조건문의 종류에 따라서 param1과 param2는 다르게 해석됩니다. 자세한 내용은 MDN을 참조해주시기 바랍니다."), DisplayName("종류")]
        //[TypeConverter(typeof(SHCT_CondType))]
        public SHCondition.ConditionType type
        {
            get { return _type; }
            set { typeSpecified = true; _type = value; }
        }
        [XmlIgnore]
        public bool typeSpecified;

        private string _param1
        {
            get { return _param1; }
            set { _param2 = (value != String.Empty) ? value : null; }
        }
        [XmlAttribute]
        [Category("기본"), DescriptionAttribute("조건문의 종류에 맞는 파라미터를 설정합니다. 이 값은 조건문의 종류에 따라서 다릅니다. 자세한 내용은 MDN을 참조해주시기 바랍니다."), DisplayName("파라미터 1")]
        public string param1 { get; set; }

        private string _param2;
        [XmlAttribute]
        [Category("기본"), DescriptionAttribute("조건문의 종류에 맞는 파라미터를 설정합니다. 이 값은 조건문의 종류에 따라서 다릅니다. 자세한 내용은 MDN을 참조해주시기 바랍니다."), DisplayName("파라미터 2")]
        public string param2
        {
            get { return _param2; }
            set { _param2 = (value != String.Empty) ? value : null; }
        }

        [XmlElement("AND")]
        public SHAnd and;

        [XmlElement("OR")]
        public SHOr or;

        public object Clone()
        {
            SHCondition clone = new SHCondition();

            clone._type = this._type;
            clone.typeSpecified = this.typeSpecified;

            clone.param1 = this.param1;
            clone._param2 = this._param2;

            if (this.and != null)
                clone.and = (SHAnd)this.and.Clone();
            else clone.and = null;

            if (this.or != null)
                clone.or = (SHOr)this.or.Clone();
            else clone.or = null;

            return clone;
        }

        #region GetString
        public string GetString(SHXmlCore core)
        {
            string ret = "";

            if (!_bPassed) return _szCompiled;

            try
            {
                switch (type)
                {
                    case SHCondition.ConditionType.level:
                        if (param1 != null)
                        {
                            ret = "레벨 " + param1 + " 이상";
                            if (param2 != null) ret += "이고, ";
                        }
                        if (param2 != null)
                        {
                            ret += "레벨" + param2 + "이하";
                        }
                        break;
                    case SHCondition.ConditionType.quest:
                        ret += "[" + core.GetSafeString(core.Quests[param1].title) + "] 퀘스트";

                        if (param2 == "notexist")
                        {
                            ret += "를 가지고 있지 않음";
                        }
                        else if (param2 == "exist")
                        {
                            ret += "를 가지고 있음";
                        }
                        else if (param2 == "complete")
                        {
                            ret += "의 보상받기가능";
                        }
                        else if (param2 == "notcomplete")
                        {
                            ret += "의 보상받기 불가능";
                        }
                        else if (param2 == "done")
                        {
                            ret += "를 완료";
                        }
                        else if (param2 == "notdone")
                        {
                            ret += "를 미완료";
                        }
                        else if (param2 == "active")
                        {
                            ret += "의 (도전필드) 활성화";
                        }
                        break;
                    case SHCondition.ConditionType.quest_var:
                        ret += "[" + core.GetSafeString(core.Quests[param1].title) + "] 퀘스트 변수가 " + param2;
                        break;
                    case SHCondition.ConditionType.quest_objective_complete:
                        ret += "[" + core.GetSafeString(core.Quests[param1].title) + "] 퀘스트의 " + param2 + "번 단계가 완료";
                        break;
                    case SHCondition.ConditionType.quest_objective_incomplete:
                        ret += "[" + core.GetSafeString(core.Quests[param1].title) + "] 퀘스트의 " + param2 + "번 단계가 미완료";
                        break;
                    case SHCondition.ConditionType.item:
                    case SHCondition.ConditionType.noitem:
                    case SHCondition.ConditionType.manyitem:
                    case SHCondition.ConditionType.fewitem:
                    case SHCondition.ConditionType.equip:
                        string[] itemsID = param1.Split(new Char[] { ',' });
                        for (int i = 0; i < itemsID.Length; i++)
                        {
                            string item = itemsID[i];

                            item.Trim();
                            if (item != "")
                            {
                                ret += "[" + core.GetSafeString(core.Items[param1].name) + "]";
                                if (i != itemsID.Length - 1) ret += ", ";
                            }
                        }
                        if (type == SHCondition.ConditionType.item) ret += " 아이템을 가지고 있음";
                        else if (type == SHCondition.ConditionType.noitem) ret += " 아이템이 없음";
                        else if (type == SHCondition.ConditionType.manyitem) ret += param2 + "개 초과";
                        else if (type == SHCondition.ConditionType.fewitem) ret += param2 + "개 미만";
                        else if (type == SHCondition.ConditionType.equip) ret += " 아이템을 장착중이면";

                        break;
                    case SHCondition.ConditionType.race:
                        if (param1 == "HUMAN") ret += "종족이 인간"; else if (param1 == "VOLO") ret += "종족이 볼로"; else if (param1 == "NEBIT") ret += "종족이 네빗";
                        break;
                    case SHCondition.ConditionType.sex:
                        if (param1 == "MALE") ret += "성별이 남자"; else if (param1 == "FEMALE") ret += "성별이 여자";
                        break;
                    case SHCondition.ConditionType.faction:
                        ret += param1 + "번 팩션이 " + param2 + " 이상";
                        break;
                    case SHCondition.ConditionType.title:
                        string[] titlesID = param1.Split(new Char[] { ',' });
                        for (int i = 0; i < titlesID.Length; i++)
                        {
                            string titleID = titlesID[i];
                            titleID = titleID.Trim();
                            if (titleID != "")
                            {
                                ret += "[" + titleID + "]";
                                if (i != titlesID.Length - 1) ret += ", ";
                            }
                        }
                        ret += " 타이틀을 사용중";
                        break;
                    case SHCondition.ConditionType.date:
                        if (param1 != null)
                        {
                            ret += param1 + "이후";
                            if (param2 != null) ret += " 그리고 ";
                        }
                        if (param2 != null) ret += param2 + "이전";
                        break;
                    case SHCondition.ConditionType.talent:
                        string[] talentsID = param1.Split(new Char[] { ',' });
                        for (int i = 0; i < talentsID.Length; i++)
                        {
                            string talentID = talentsID[i];
                            talentID = talentID.Trim();
                            if (talentID != "")
                            {
                                ret += "[" + core.GetSafeString(core.Talents[talentID].name) + "]";
                                if (i != talentsID.Length - 1) ret += ", ";
                            }
                        }
                        ret += " 탤런트를 익힘";
                        break;
                    case SHCondition.ConditionType.talent_style_tp:                        
                        ret += "[" + param1 + "] 스타일의 탤런트를 " + param2 + " 개 익힘";
                        break;
                    case SHCondition.ConditionType.inn:
                        ret += "여관 " + param1 + "번에 등록";
                        break;
                    case SHCondition.ConditionType.kill:
                        ret += core.GetSafeString(core.NPCs[param1].Name) + "NPC를 죽임";
                        break;
                    case SHCondition.ConditionType.qpvp_team:
                        ret += "QPVP에서" + param1 + "번 팀";
                        break;
                    default:
                        break;
                }
            }
            catch (KeyNotFoundException)
            {
                _bPassed = false;
                ret = _szCompiled = "[" + type.ToString().ToUpper() + "] 아직 생성되지 않은 개체를 참조하고 있습니다.";
            }
            catch (Exception ex)
            {
                _bPassed = false;
                ret = _szCompiled = "[" + type.ToString().ToUpper() + "] 오류를 포함하고 있습니다. " + ex.Message;
            }
            
            return ret;
        }
        #endregion

        #region 컴파일

        public override bool Compile()
        {
            _szCompiled = SHGlobal.COK;
            _bPassed = true;

            switch (type)
            {
                case SHCondition.ConditionType.level:
                    if (param1 == null && param2 == null) return Error("[" + type.ToString().ToUpper() + "] param1과 param2가 모두 명시되지 않았음.");
                    break;
                case SHCondition.ConditionType.quest:
                case SHCondition.ConditionType.quest_var:
                case SHCondition.ConditionType.faction:
                case SHCondition.ConditionType.date:
                case SHCondition.ConditionType.quest_objective_complete:
                case SHCondition.ConditionType.quest_objective_incomplete:
                case SHCondition.ConditionType.inn:
                case SHCondition.ConditionType.manyitem:
                case SHCondition.ConditionType.fewitem:
                case SHCondition.ConditionType.talent_style_tp:
                    if (param1 == null || param2 == null) return Error("[" + type.ToString().ToUpper() + "] param1과 param2가 모두 필요함.");
                    if (type == SHCondition.ConditionType.quest && param2 != "notexist" && param2 != "exist" && param2 != "complete" && param2 != "notcomplete" && param2 != "active" && param2 != "done" && param2 != "notdone") return Error("[" + type.ToString().ToUpper() + "] 잘못된 param2를 사용.");
                    if (type == SHCondition.ConditionType.date && ((param1.Length != 6 && param1.Length != 4) || (param2.Length != 6 && param2.Length != 4))) return Error("[" + type.ToString().ToUpper() + "] 날짜 형식이 잘못되어 있음.");
                    if (type == SHCondition.ConditionType.inn && param2 != "regist" && param2 != "notregist") return Error("[" + type.ToString().ToUpper() + "] 잘못된 param2를 사용.");
                    break;
                case SHCondition.ConditionType.item:
                case SHCondition.ConditionType.noitem:
                case SHCondition.ConditionType.equip:
                case SHCondition.ConditionType.race:
                case SHCondition.ConditionType.sex:
                case SHCondition.ConditionType.title:
                case SHCondition.ConditionType.talent:                
                case SHCondition.ConditionType.kill:
                case SHCondition.ConditionType.qpvp_team:
                    if (param1 == null || param2 != null) return Error("[" + type.ToString().ToUpper() + "] param1이 명시되지 않거나 사용되지 않는 param2에 값이 할당되어 있음.");
                    if ((type == SHCondition.ConditionType.race && (param1 != "HUMAN" && param1 != "NEBIT" && param1 == "VOLO")) ||
                        (type == SHCondition.ConditionType.sex && (param1 != "MALE" && param1 != "FEMALE"))) return Error("[" + type.ToString().ToUpper() + "] param1이 명세에 벗어난 값을 사용중");
                    break;
                default:
                    return Error("사용할 수 없는 type을 사용하였음.");
            }

            return _bPassed;
        }

        public override bool Build(SHXmlCore e)
        {
            if (!_bPassed) return false;

            switch (type)
            {
                case SHCondition.ConditionType.quest:
                case SHCondition.ConditionType.quest_var:
                    if (!e.Quests.IsValid(param1)) _bPassed = false;
                    break;
                case SHCondition.ConditionType.quest_objective_complete:
                case SHCondition.ConditionType.quest_objective_incomplete:
                    if (!e.Quests.IsValid(param1)) _bPassed = false;
                    break;
                case SHCondition.ConditionType.item:
                case SHCondition.ConditionType.noitem:
                case SHCondition.ConditionType.equip:
                case SHCondition.ConditionType.manyitem:
                case SHCondition.ConditionType.fewitem:
                    string[] itemsID = param1.Split(new Char[] { ',' });
                    for (int i = 0; i < itemsID.Length; i++)
                    {
                        string szItem = itemsID[i].Trim();
                        if (szItem != String.Empty && !e.Items.IsValid(param1))
                        {
                            _bPassed = false;
                            break;
                        }
                    }
                    break;
                case SHCondition.ConditionType.talent:
                    string[] talentID = param1.Split(new Char[] { ',' });
                    for (int i = 0; i < talentID.Length; i++)
                    {
                        string szTalent = talentID[i].Trim();
                        if(szTalent != String.Empty && !e.Talents.IsValid(szTalent))
                        {
                            _bPassed = false;
                            break;
                        }
                    }
                    break;
            }

            if (!_bPassed) return Error("[" + type.ToString().ToUpper() + "] 아직 생성되지 않은 개체를 참조하고 있습니다.");

            return _bPassed;
        }

        public override string ToBody(SHXmlCore core)
        {
            throw new NotImplementedException();
        }

        #endregion
    }
}
