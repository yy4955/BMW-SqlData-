#pragma once
#include "pch.h"
/*
表达式计算：
1.先分解string ，
2.按优先级分步计算
3.计算结果汇总
*/


/*
//分析
[classid = datavalueid]
1.classid 查询 XEP_CHARACTERISTICROOTS 的key得到 nodeclassField  //53078923 -- BMW PKW   ,nodeclassField:40144642
2.通过 nodeclassField 进行hash计算 得到num
3. num 找到对应的 case选项 获得name和nodeclassField，然后去找对应的datanalueid
*/


extern  std::vector<struct g_tag>G_v;
extern std::map<std::string, long long > G_mapHash;
std::stack<char>G_stack_Operator;//操作符
std::stack<std::string>G_str_Operator;//操作符
std::stack<bool>G_stack_Operands;//操作数
/*
检测左边是不是运算类型

*/
typedef enum StringValue {
	date = 0,//4
	value,//5
	istufe,//6
	valid_from,//7
	valid_to,//8
	country,//9
	ecugroup,//a
	ecuvariant,//b
	ecuclique,//c
	equipment,//d
	salapa,//e
	sifa,//f
	variable,//17
	characteristic,//18
	ecurepresentative,//19
	manufactoringdate,//20
	istufex,

}streum;

typedef  enum OpratorValue {
	
	equal,		//=

	not_equal,	//!=		

	greater,	//>

	greater_equal,	//>=

	less,		//<

	less_equal,
	ERREO
}Eop;

typedef struct tag1 {
	std::string str;
	StringValue valie;
}tagStr;

typedef struct tag {
	std::string str;
	OpratorValue valie;
}tagOprator;

static std::vector<tagStr> s_mapStringValues;
static std::vector<tagOprator> s_mapOpratorValues;

std::vector <std::string> str{
	"DATE" ,"VALUE","ISTUFE" ,"VALID_FROM" ,"VALID_TO",
	"COUNTRY","ECUGROUP", "ECUVARIANT","ECUCLIQUE","EQUIPMENT",
	"SALAPA","SIFA","VARIABLE","CHARACTERISTIC","ECUREPRESENTATIVE",
	"MANUFACTORINGDATE","ISTUFEX" };

std::vector <std::string> strop{  "EQUAL","NOT_EQUAL" ,"GREATER","GREATER_EQUAL","LESS","LESS_EQUAL" };

//CHARACTERISTIC map查询求布尔值
bool ExpressionCHARACTERISTIC(std::string strLeft, long long lRigh)
{
	//long long ii = G_map.find(lLeft);
	for (int i = 0;i<= G_v.size();i++)
	{
		if (G_v[i].strkey == strLeft)
		{
			for (int j = 0; j <= G_v[i].ivalue.size(); j++)
			{
				if (G_v[i].ivalue[j] == lRigh)
				{
					return true;
				}
			}
		}
	}
	return false;
}


bool ExpressionCHARACTERISTIC_flase(std::string lLeft, long long lRigh)
{
	//long long ii = G_map.find(lLeft);
	auto iters = G_mapHash.find(lLeft);
	if (iters != G_mapHash.end())
	{
		if (iters->second == lRigh)
		{
			return true;
		}
	}
	return false;
}

//分割关键词，分解因式
//1.解析成逆波兰表达式
//2.按顺序解析


bool RelationCalculate(Eop cCalculateType, int nLeft, int nRight)
{
	if (nRight == -1)
	{
		nRight = 0;//考虑not因素，默认为0
	}
	//strR = "0";
	bool flag = false;
	switch (cCalculateType)
	{
	case equal:
		flag = nLeft = nRight;
		break;
	case not_equal:
		flag = nLeft != nRight;
		break;
	case greater:
		flag = nLeft > nRight;
		break;
	case greater_equal:
		flag = nLeft >= nRight;
		break;
	case less:
		flag = nLeft < nRight;
		break;
	case less_equal:
		flag = nLeft <= nRight;
		break;
	}
	return flag;
}
//单个逻辑表达式计算   ！，|，&
bool LogiCalculate(char cCalculateType, int nLeft, int nRight)
{
	//&，|,!计算

	if (nRight == -1)
	{
		nRight = 0;//考虑not因素，默认为0
	}
	//strR = "0";
	bool flag = false;
	//int nLeft = std::stol(strL);
	//int nRight = std::stol(strR);
	
	switch (cCalculateType)
	{
	case '&':
		flag = nLeft & nRight;
		break;
	case '|' :
		flag = nLeft | nRight;
		break;
	case '!':
		flag = !(nLeft | nRight);
		break;
	}
	return flag;
}

//日期求布尔值
bool ExpressionDate(std::string strLeft, long long lRigh)
{
	auto iters = G_mapHash.find(strLeft);
	bool flag = false;
	if (iters != G_mapHash.end())
	{
		Eop ctype =(Eop) G_stack_Operator.top();
		G_stack_Operator.pop();
		flag = RelationCalculate(ctype, iters->second, lRigh);
	}
	return flag;
}

std::string GetStrtoIntValue(std::stringstream &str,int& nPos)
{
	std::string stemp;
	for (int i = nPos+1; i < str.str().length(); i++)
	{
		//char c = str.get();
		if (str.str()[i ]=='(' || str.str()[i] == '[' || str.str()[i] == 0x20)
		{
			nPos = i;
			continue;
		}
	
		//if (str.str()[i] == '!=' || str.str()[i] == '>'
		//	|| str.str()[i] == '>=' || str.str()[i] == '<'
		//	|| str.str()[i] == '<=')
		//{
		//	//std::string s("");
		//	//s = str.str()[i];
		//	//s+= str.str()[i++];
		//	nPos = i;
		//	G_stack_Operator.push(s);
		//	continue;
		//}

		if (str.str()[i ] == '=' || str.str()[i] == ']' || str.str()[i] == '-')
		{
			nPos = i;
			break;
		}
	
		stemp += str.str()[i];
		int j = str.tellg();
		nPos = i;
	}
	
	return stemp;
}


void Initialize()
{
	tagStr stag;
	tagOprator otag;

    streum  Estr = date;
	Eop  Eoprator= equal;
	for (int i = 0;i<str.size();i++)
	{
		stag.str = str[i];
		stag.valie = Estr;
		s_mapStringValues.push_back(stag);
		Estr= (streum)(Estr+1);
	}

	for (int i = 0; i < strop.size(); i++)
	{
		otag.str = strop[i];
		otag.valie = Eoprator;
		s_mapOpratorValues.push_back(otag);
		Eoprator=(Eop)(Eoprator+1);
	}

}

//>=,<=,>,<,==,!=判断运算符进行压栈
void LogiOpration(std::string strL)
{
	Eop eOprator = ERREO;
	for each (tagOprator var in s_mapOpratorValues)
	{
		if (var.str == strL)
		{
			eOprator = var.valie;
		}
	}
	switch (eOprator)
	{
		//逻辑运算符压栈
	case equal:
		G_stack_Operator.push(equal);
		break;
	case not_equal:
		G_stack_Operator.push(not_equal);
		break;
	case greater:
		G_stack_Operator.push(greater);
		break;
	case greater_equal:
		G_stack_Operator.push(greater_equal);
		break;
	case less:
		G_stack_Operator.push(less);
		break;
	case less_equal:
		G_stack_Operator.push(less_equal);
		break;
	}
}

bool CheckOperator(std::stringstream &stream, std::string strL, int &nPos)
{
	bool b = false;
	std::string strR("");
	std::string strOp("");
	int nPos1 = 0;
	unsigned long long  lRigh = 0;
	//OpratorValue Value = (OpratorValue)s_mapStringValues[strL];
	streum Etype = characteristic;
	for each (tagStr var in s_mapStringValues)
	{
		if (var.str == strL)
		{
			Etype = var.valie;
		}
	}
	switch (Etype)
	{
	case date:
		//date 获取运算符
		strOp = GetStrtoIntValue(stream, nPos);
		stream.seekg(nPos, std::ios_base::beg);
		nPos1 = stream.tellg();
		LogiOpration(strOp);

		//获取右操作数
		strR = GetStrtoIntValue(stream, nPos);
		stream.seekg(nPos, std::ios_base::beg);
		nPos1 = stream.tellg();

		lRigh = std::stoll(strR);
		b = ExpressionDate(strL, lRigh);
		break;
	case characteristic:
		//characteristic
				//获取右操作数
		strR = GetStrtoIntValue(stream, nPos);
		stream.seekg(nPos, std::ios_base::beg);
		lRigh = std::stoll(strR);
		b = ExpressionCHARACTERISTIC_flase(strL, lRigh);
		break;
	case value:
		break;
	case istufe:
		break;
	case valid_from:
		break;
	case valid_to:
		break;
	case country:
		break;
	case ecugroup:
		break;
	case ecuvariant:
		break;
	case ecuclique:
		strR = GetStrtoIntValue(stream, nPos);
		stream.seekg(nPos, std::ios_base::beg);
		lRigh = std::stoll(strR);
		b = ExpressionCHARACTERISTIC_flase(strL, lRigh);
		break;
	case equipment:
		break;
	case salapa:
		break;
	case sifa:
		break;
	case variable:
		break;
	case ecurepresentative:
		break;
	case manufactoringdate:
		break;
	case istufex:
		break;
	}
	return b;
}

//string => RPN
bool splitRPN (std::string str)
{
	//
	if (str.empty())
	{
		printf("split(std::string str):%s\r\n",strerror(errno));
	}
	std::stringstream ss;
	ss << str;

	char cOperator;
	int strLen = str.length();
	bool bFlag = false; 
	std::string strFlag("");
	std::string strExpression;
	int nPos = 0;
	long long lLeft = 0;
	long long lRigh = 0;
	while (true)
	{
		if (nPos == strLen)
		{
			break;
		}
		
		char cType = ss.get();

		int strL;
		int strR = -1;//是not的时候默认为-1
		std::string strtmpLeft("");
		std::string strtmpRigh("");
		strtmpLeft.clear();
		strtmpRigh.clear();

		int ValuePos = 0;
		int &i= ValuePos;

		if (cType == 0x20)
		{
			continue;
		}

		switch (cType)
		{
		case '[':
			//ss.read(reinterpret_cast<char*>(&lLeft), sizeof lLeft);
			//ss.read(reinterpret_cast<char*>(&lRigh), sizeof lRigh);
			ValuePos += nPos;
			nPos = ss.tellg();
			strtmpLeft = GetStrtoIntValue(ss, i);
			//CheckOperator(strtmp);
			//lLeft = std::stoll(strtmp);
			ss.seekg(ValuePos, std::ios_base::beg);
			nPos = ss.tellg();
			//strtmpRigh = GetStrtoIntValue(ss, i);
			//lRigh = std::stoll(strtmp);
			//ss.seekg(ValuePos, std::ios_base::beg);
			
			//根据id判断运算类型
			bFlag = CheckOperator(ss,strtmpLeft, i);
			//bFlag = ExpressionCHARACTERISTIC_flase(lLeft, lRigh);
			break;
		case ']':
			//strFlag = std::to_string(bFlag);
			G_stack_Operands.push(bFlag);
			break;
		case '(':
			G_stack_Operator.push('(');
			break;
		case ')':
			//把第一段（）的值拿出来计算，计算完压栈 todo
			//str_stack.pop();
			cOperator = G_stack_Operator.top();
			if (cOperator == '!')
			{
				strL = G_stack_Operands.top();
				G_stack_Operands.pop();
				bool Flag = LogiCalculate(cOperator, strL, strR);
				G_stack_Operands.push(Flag);
				G_stack_Operator.pop();
			}

			cOperator = G_stack_Operator.top();

			while (cOperator !=  '(')
			{
				//1.是否需要检测运算符是！的情况 todo
				G_stack_Operator.pop();
			//2.取值顺序，
				strR = G_stack_Operands.top();
				G_stack_Operands.pop();

				strL = G_stack_Operands.top();
				G_stack_Operands.pop();

				bool Flag = LogiCalculate(cOperator, strL, strR);
				G_stack_Operands.push(Flag);
				 
				cOperator = G_stack_Operator.top();
			}

			G_stack_Operator.pop();
			break;

		case '&':
			G_stack_Operator.push('&');
			break;
		case '|':
			G_stack_Operator.push('|');
			break;
		case '!':
			G_stack_Operator.push('!');
			break;
			/*
		case '>':
			G_stack_Operator.push('>');
			break;
		case '<':
			G_stack_Operator.push('<');
			break;
		case '=':
			G_stack_Operator.push('=');
			break;
		case '>=':
			G_stack_Operator.push('>=');
			break;
		case '<=':
			G_stack_Operator.push('<=');
			break;*/
		}
		nPos = ss.tellg();
	}
	bool b = G_stack_Operands.top();
	G_stack_Operands.pop();
	//释放资源
	if (G_stack_Operands.empty())
	{
		//std::cout << "stack_Operands.size"<<stack_Operands.size << std::endl;
		//std::cout << "stack_Operator.size" << stack_Operator.size << std::endl;

		return b;
	}
	return b;
}



//表达式结果汇总
void CalculateSum()
{
}