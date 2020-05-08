#pragma once
#include "pch.h"
#include "MyByteOperation.h"



//enum Logi type
typedef enum
{
	COMP = 0,
	AND,//1
	OR,//2
	NOT,//3
	DATE,//4
	VALUE,//5
	ISTUFE,//6
	VALID_FROM,//7
	VALID_TO,//8
	COUNTRY,//9
	ECUGROUP,//a
	ECUVARIANT,//b
	ECUCLIQUE,//c
	EQUIPMENT,//d
	SALAPA,//e
	SIFA,//f
	VARIABLE,//17
	CHARACTERISTIC,//18
	ECUREPRESENTATIVE,//19
	MANUFACTORINGDATE,//20
	ISTUFEX,//21
	LOGINULL
}Logi;

//比较运算符
typedef enum
{
	EQUAL = 0,		//=

	NOT_EQUAL,	//!=		
	
	GREATER,	//>

	GREATER_EQUAL,	//>=

	LESS,		//<
	
	LESS_EQUAL	//<=  27
}ECompareOperator;



//每次解析配对的id
typedef struct {
	std::string strLogi;    //逻辑运算符
	bool bl = false;//压栈标志
	std::string strClassID;
	std::string strValueID;
}tagDeserial;

//date_struct
typedef struct {
	//std::string strCompare;    
	long dateValue;
	ECompareOperator ECompare;//比较运算符
}tagDate;

typedef struct g_tag {
	std::string strkey;
	std::vector<long long> ivalue;
};

std::vector<struct g_tag>G_v;
std::map<std::string, long long > G_mapHash;
typedef enum
{
	// Token: 0x04001A07 RID: 6663
	HO = 0,
	// Token: 0x04001A08 RID: 6664
	Factory,
	// Token: 0x04001A09 RID: 6665
	Ziel
}ILevelyType;

typedef struct 
{
	long iLevelId = 0;//ilevelid
	ILevelyType EiLevelType ;//iLevelType
	ECompareOperator compareOperator = compareOperator;//ECompareOperator
}IStufeXExpression;


//全局vctor  存储每个解析出来的id
std::vector <tagDeserial>strDeserializationManage;
char g_sz[0x1000] = { 0 };
//function code
void SwitchLogType(std::stringstream &stream,std::string &str);
//tagDeserial CompareExpressionDeserialize(std::stringstream stream);
//tagDeserial AndExpressionDeserialize(std::stringstream stream);
//tagDeserial OrExpressionDeserialize(std::stringstream stream);
//tagDeserial NotExpressionDeserialize(std::stringstream stream);
//tagDeserial DateExpressionDeserialize(std::stringstream stream);
//tagDeserial SingleAssignmentExpressionDeserialize(std::stringstream stream);
//tagDeserial CharacteristicExpressionDeserialize(std::stringstream stream);
//tagDeserial ManufactoringDateExpressionDeserialize(std::stringstream stream);
//tagDeserial IStufeXExpressionDeserialize(std::stringstream stream);

//CompareExpressionDeserialize 
//tagDeserial CompareExpressionDeserialize(std::stringstream stream)
//{
//	//todo
//}


void  AndExpressionDeserialize(std::stringstream &stream ,std::string &strSql)
{
	uint32_t num = 0;
	stream.read(reinterpret_cast<char*>(&num), sizeof num);
	//debug
	int npox = stream.tellg();

	tagDeserial tagDeserialID;
	strSql += "(";
	for (int i = 0; i < num; i++)
	{
		//andExpression.AddOperand(RuleExpression.Deserialize(stream_0));
		//tagDeserialID = SwitchLogType(stream, strSql);
		SwitchLogType(stream, strSql);
		//添加逻辑符号
		if (i == num - 1 /*|| 0 == i */)
		{
			tagDeserialID.strLogi = "";
		}
		else
		{
			tagDeserialID.strLogi = "AND";
			strSql += " & ";
		}
		//strDeserializationManage.push_back(tagDeserialID);
	}
	strSql += ")";
	//return tagDeserialID;
}

void OrExpressionDeserialize(std::stringstream &stream,std::string &strSql)
{
	uint32_t num = 0;
	stream.read(reinterpret_cast<char*>(&num), sizeof num);
	//debug
	int npox = stream.tellg();

	//OrExpression orExpression = new OrExpression();
	tagDeserial tagDeserialID;
	strSql += "(";
	for (int i = 0; i < num; i++)
	{
		//orExpression.AddOperand(RuleExpression.Deserialize(stream_0));
		//tagDeserialID = SwitchLogType(stream, strSql);
		SwitchLogType(stream, strSql);
		//添加逻辑符号
		if (i == num - 1 /*|| 0 == i*/ )
		{
			tagDeserialID.strLogi = "";
		}
		else
		{
			tagDeserialID.strLogi = "OR";
			strSql += " | ";
		}
		//strDeserializationManage.push_back(tagDeserialID);
	}
	strSql += ")";
	//return tagDeserialID;
}
//
void  NotExpressionDeserialize(std::stringstream &stream ,std::string &strSql)
{
	//return new NotExpression(RuleExpression.Deserialize(stream_0));
	tagDeserial tagDeserialID;
	strSql += "(";
	//添加逻辑符号
	tagDeserialID.strLogi = "NOT";
	strSql += " ! ";
	//tagDeserialID = SwitchLogType(stream, strSql);
	SwitchLogType(stream, strSql);
	strSql += ")";

	//strDeserializationManage.push_back(tagDeserialID);
	//return tagDeserialID;
}

//
std::string switchType(ECompareOperator type)
{
	switch (type)
	{
	case EQUAL:
		//return " EQUAL ";
		return " DATE - EQUAL -";
	case NOT_EQUAL:
		//return " Baustand != ";
		//return " NOT_EQUAL ";
		return " DATE - NOT_EQUAL - ";
	case GREATER:
		//return " GREATER ";
		return " DATE - GREATER - ";
	case GREATER_EQUAL:
		//return " GREATER_EQUAL ";
		return " DATE - GREATER_EQUAL - ";
	case LESS:
		//return " LESS ";
		return " DATE - LESS - ";
	case LESS_EQUAL:
		//return " LESS_EQUAL ";
		return " DATE - LESS_EQUAL - ";
	}

}

void DateExpressionDeserialize(std::stringstream &stream,  std::string &strSql)
{
	ECompareOperator type = (ECompareOperator)stream.get();
	std::stringstream ss;
	long long lDateID = 0;
	stream.read(reinterpret_cast<char*>(&lDateID), sizeof lDateID);
	//debug
	int npox = stream.tellg();
	strSql += "[";
	ss << std::dec << lDateID;
	//strSql += ;
	tagDate tagDateInfo;
	tagDateInfo.dateValue = lDateID;
	tagDateInfo.ECompare = type;
	strSql += switchType(type);
	strSql += ss.str();
	ss.clear();
	ss.str("");
	strSql += "]";
	//return tagDateInfo;
}

typedef struct SingleAssignment
{
	Logi flag = LOGINULL;
	long long value = 0;
}SingleAssignmentExpression;
//
void SingleAssignmentExpressionDeserialize(std::stringstream &stream, Logi type, std::string &strSql)
{
	//byte[] buffer = new byte[8];
	//stream_0.Read(buffer, 0, 8);
	long long lExpression = 0;
	stream.read(reinterpret_cast<char*>(&lExpression), sizeof lExpression);
	
	//SingleAssignmentExpression singleAssignmentExpression;
	strSql += "[";
	switch (type)
	{
	case ISTUFE:
		strSql += " ISTUFE = ";
		break;
	case VALID_FROM:
		strSql += " VALID_FROM = ";
		break;
	case VALID_TO:
		strSql += " VALID_TO = ";
		break;
	case COUNTRY:
		strSql += " COUNTRY = ";
	case ECUGROUP:
		strSql += " ECUGROUP = ";
		break;
	case ECUVARIANT:
		strSql += " ECUVARIANT = ";
		break;
	case ECUCLIQUE:
		strSql += " ECUCLIQUE = ";
		break;
	case EQUIPMENT:
		strSql += " EQUIPMENT = ";
		break;
	case SALAPA:
		strSql += " SALAPA = ";
		break;
	case SIFA:
		strSql += " SIFA = ";
		break;
	case ECUREPRESENTATIVE:
		strSql += " ECUREPRESENTATIVE = ";
		break;
	}

	//singleAssignmentExpression.value = lExpression;
	std::stringstream ss;
	ss << std::dec << lExpression;
	strSql += ss.str();
	ss.str("");        //将内容置空
	ss.clear();        //将读写状态重置
	strSql += "]";
	//return singleAssignmentExpression;
}


void mapInster(std::string  l,long long r)
{
	//std::map <long long, std::vector<long long>>::iterator iter;
	struct g_tag tagTmp;
	tagTmp.ivalue.push_back(r);
	tagTmp.strkey = l;
			for (int i =0;i< G_v.size();i++)
			{
				if (G_v[i].strkey == l)
				{
					for (int j = 0; j < G_v[i].ivalue.size(); j++)
					{
						if (G_v[i].ivalue[j]== r)
						{
							return;
						}
					}
					G_v[i].ivalue.push_back(r);
					return;
				}
			}
			G_v.push_back(tagTmp);
	//int i = iter->first;
	//int ii = iter->second;
}
//
void  CharacteristicExpressionDeserialize(std::stringstream &stream, std::string &strSql)
{
	//todo
	/*byte[] array = new byte[16];
	stream_0.Read(array, 0, 16);
	long num = BitConverter.ToInt64(array, 0);
	long num2 = BitConverter.ToInt64(array, 8);
	return new CharacteristicExpression(num, num2);*/
	 //char ClassID[8] = { 0 };
	 //char ValueID[8] = { 0 };
    long long lClassID = 0;
	long long lValueID = 0;
	stream.read(reinterpret_cast<char*>(&lClassID), sizeof lClassID);
	stream.read(reinterpret_cast<char*>(&lValueID), sizeof lValueID);
	std::stringstream ss;
	ss << std::dec << lClassID;
	//存全局
	G_mapHash.insert(std::make_pair(ss.str(), lValueID));
	mapInster(ss.str(), lValueID);
	//debug
	int npox = stream.tellg();

	
	tagDeserial tagDeserialID;

	tagDeserialID.strClassID = ss.str();
	strSql +=  "[" + ss.str();
	ss.str("");        //将内容置空
	ss.clear();        //将读写状态重置
	ss << std::dec << lValueID;
	tagDeserialID.strValueID = ss.str();
	strSql += "=" + ss.str()+"]" ;
	ss.clear();
	ss.str("");
	//return tagDeserialID;
}
//
void ManufactoringDateExpressionDeserialize(std::stringstream &stream, std::string &strSql)
{
	
}

void  IStufeXExpressionDeserialize(std::stringstream &stream,  std::string &strSql)
{
	//ECompareOperator ecompareOperator = (ECompareOperator)((byte)stream_0.ReadByte());
	ECompareOperator ecompareOperator =(ECompareOperator)stream.get();
	std::stringstream ss;
	//IStufeXExpression.ILevelyType levelyType = (IStufeXExpression.ILevelyType)stream_0.ReadByte();
	ILevelyType levelyType=(ILevelyType)stream.get();
	/*byte[] array = new byte[8];
	stream_0.Read(array, 0, 8);*/
	long long ilevelid = 0;
	stream.read(reinterpret_cast<char*>(&ilevelid), sizeof ilevelid);
	//debug
	int npox = stream.tellg();

	//return new IStufeXExpression(ecompareOperator, ilevelid, levelyType);
	//IStufeX: HO-I-Stufe GREATER_EQUAL 'F001-10-09-500' [40564650763] 
	strSql += "IStufeX:";
	ss << std::dec << ilevelid;
	strSql += levelyType + "-I-Stufe" + ecompareOperator  ;
	strSql += "[" + ss.str();
	strSql += "]";
}

//switch-LogitType
void SwitchLogType(std::stringstream &stream,std::string &strSql)
{
	memcpy(g_sz, stream.str().c_str(), stream.str().length());
	Logi type = (Logi)stream.get();
	int npox = stream.tellg();

	switch (type)
	{
	case COMP:
		//return CompareExpressionDeserialize(stream);
	case AND:
		 AndExpressionDeserialize(stream, strSql);
		// std::cout << strSql << std::endl;
		 break;
	case OR:
		 OrExpressionDeserialize(stream, strSql);
		 //std::cout << strSql << std::endl;
		 break;
	case NOT:
		 NotExpressionDeserialize(stream, strSql);
		 //std::cout << strSql << std::endl;
		 break;
	case DATE:
		 DateExpressionDeserialize(stream, strSql);
		 //std::cout << strSql << std::endl;
		 break;
	case ISTUFE:
	case VALID_FROM:
	case VALID_TO:
	case COUNTRY:
	case ECUGROUP:
	case ECUVARIANT:
	case ECUCLIQUE:
	case EQUIPMENT:
	case SALAPA:
	case SIFA:
	case ECUREPRESENTATIVE:
		SingleAssignmentExpressionDeserialize(stream, type, strSql);
		break;
	case CHARACTERISTIC:
		return CharacteristicExpressionDeserialize(stream, strSql);
		break;
	case MANUFACTORINGDATE:
		//return ManufactoringDateExpressionDeserialize(stream);
	case ISTUFEX:
		IStufeXExpressionDeserialize(stream, strSql);
		//std::cout << strSql << std::endl;
		break;
	}
	//Log.Error("RuleExpression.Deserialize()", "Unknown Expression-Type", Array.Empty<object>());
	//throw new Exception("Unknown Expression-Type");
}