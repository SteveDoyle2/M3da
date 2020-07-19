#include "G_Object.h"
#include "M3Da.h"
#define D2R  0.01745329251994
#define R2D  57.2957795130931
const double Pi = 3.1415926535;
const double dTol = 0.00000001;  //unsed for Surface tolerance
//Constant Related to Auto-Meshing
CONST double PARA_NEAR_ZERO = 0.000001;
CONST double PARA_MAX_ERROR = 0.0001;
const int ISO_TEST_INC = 10;								 
//#include "math.h"
#include <cmath>

#pragma warning(disable:4477)
#pragma warning(disable:4244)
#pragma warning(disable:4474)

GLubyte BMPX[13] = {0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3};
GLubyte BMPY[13] = {0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3};
GLubyte BMPZ[13] = {0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff};
GLubyte BMPT[13] = {0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x24, 0x18};
GLubyte BMPR[13] = {0x00, 0x00, 0x42, 0x42, 0x44, 0x72, 0x50, 0x7c, 0x42, 0x42, 0x62, 0x58, 0x00};
GLubyte BMPP[13] = {0x00, 0x00, 0x3e, 0x08, 0x08, 0x08, 0x3e, 0x49, 0x49, 0x49, 0x08, 0x08, 0x3e};
GLubyte BMPSP[13] ={0x04, 0x04, 0x04, 0x0a, 0x0a, 0x09, 0x91, 0x90, 0x50, 0x50, 0x20, 0x20, 0x20};
GLubyte BMPU[13] = {0x01,0x3D, 0x43, 0x81,  0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00};
GLubyte BMPV[13] = {0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3 };


//Colour bar 
float cBarMax= (float) -1e+20;
float cBarMin= (float) 1e+20;
float cBarVecMax;
float cBarVecMin;

void SetColBar(float fMin,float fMax)
{
//if (fMax>cBarMax)
  cBarMax=fMax;
//if (fMin<cBarMin)
  cBarMin=fMin;
}

void SetColBarVec(float fMin, float fMax)
{
if (fMax>cBarMax)
	cBarVecMax = fMax;
if (fMin < cBarMin)
	cBarVecMin = fMin;
}

double VecMaxMag()  //For normalisation of vector lengths
{
	double dRet = 1.0;
	dRet = abs(cBarVecMax);
	if (abs(cBarVecMin) > dRet)
		dRet = abs(cBarVecMin);
	return (dRet);
}


float GetContourCol(float Val)
{
float fN;
fN=(Val-cBarMin)/(cBarMax-cBarMin);
if (fN>=0.999)
{fN=(float) 0.999;}
if (fN<=0.001)
{fN=(float) 0.001;}
return (fN);
}

float GetContourColVec(float Val)
{
	float fN;
	fN = (Val - cBarVecMin) / (cBarVecMax - cBarVecMin);
	if (fN >= 0.999)
	{
		fN = (float) 0.999;
	}
	if (fN <= 0.001)
	{
		fN = (float) 0.001;
	}
	return (fN);
}

CString RemTrailingZeros(CString sIn)
{
int iLen;
int i;
CString sRet;
int iLPos;
iLen=sIn.GetLength();
iLPos=iLen;
sRet=sIn;
CString sC;

for (i=iLen;i>0;i--)
{
  sC=sRet.Mid(i-1,1);
  if (sC=="0")
  {

  }
  else
  {
    iLPos=i;
    break;
  }
}
sRet=sRet.Left(iLPos);
return(sRet);
}

CString ncr(CString sIn)
{

sIn.Replace("\n",""); 
sIn.Replace(" ",""); 
return(sIn);
}

CString f8(double dIn)
{

char s80[80];
  CString s8;
if (dIn==0)
{
  s8="0.0";
}
else
{
  sprintf_s (s80, "%-f",dIn);
  s8=s80;
  s8=s8.Left(8);
}
return(s8);
}

CString f4(double dIn)
{

  char s80[80];
  CString s8;

  sprintf_s (s80, "%-f",dIn);
  s8=s80;
  s8=s8.Left(5);
  s8+="   ";
return(s8);
}


CString e8(double dIn)
{

char s80[80];
sprintf_s (s80, "%e",dIn);
CString s8;
CString sRet;
CString sExp;
CString sNum;
double dExp;
s8=s80;
int iD;
int iL;
if (dIn==DBL_MAX)
{
   sRet="        ";
}
else
{
if (dIn==0)
{
  sRet="0.0";
}
else
{
  iD=s8.Find('e');
  iL=s8.GetLength();
  sExp=s8.Right(3);
  dExp=atoi(sExp);
  if ((dExp>4) || (dExp < -4))
  {
    if (dExp>99)
     {sNum=s8.Left(4);
      sNum=RemTrailingZeros(sNum);
      sExp=s8.Right(3);}
    else if (dExp>9)
     {sNum=s8.Left(5);
      sNum=RemTrailingZeros(sNum);
      sExp=s8.Right(2);}
    else
     {sNum=s8.Left(6);
      sNum=RemTrailingZeros(sNum);
      sExp=s8.Right(1);} 
    if (s8.Find('+')>-1)
    {
      sRet=sNum+'+'+sExp;
    }
    else
    {
      sRet=sNum+'-'+sExp;
    }
  }
  else
  {
    sRet=f8(dIn);
    sRet=RemTrailingZeros(sRet);
  }
}
}
return(sRet);
}




void OglString(int iDspFlgs,double x, double y, double z,char *s)
{
int iLen;
int i;
int iC;
iLen = (int) strlen(s);
  if (iDspFlgs & DSP_BLACK)
  {
    glColor3fv(cols[124]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
glRasterPos3f ((float) x,(float) y,(float) z);
for (i=0;i<iLen;i++)
{
  iC=s[i];
  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, Font1[iC-32]);
}
}


int Factorial(int n)
{
int izp;
int iRetzVal;
iRetzVal = 1;
for(izp = 0;izp < n;izp++)
  {
  iRetzVal = iRetzVal*(izp+1);
  }
return (iRetzVal);
};

CString float8NAS(double d)
{
char s1[80];
CString sRet;
sprintf_s(s1,"%8.7f",d);
sRet=s1;
sRet=sRet.Left(8);
return(sRet);
}

int minimum(int a,int b)
{
int irc;
irc = a;
if (b<a)
{
  irc = b;
}
return (irc);
}

// implementation of linked list
// for storing objects derived from G_Object
IMPLEMENT_DYNAMIC(cLinkedListB , CObject)

cLinkedListB::cLinkedListB()
{
iCnt=0;
Head=NULL;
pCur=NULL;
iLabel=-1;
iColour=-1;
sTitle="";
iObjType=201;
}

void cLinkedListB::Info()
{
  char S1[200];
  outtext1("**************************************");
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("BOUNDARY SET");
  outtext1(S1); 
  sprintf_s(S1,"ID: %i TITLE: %s",iLabel,sTitle);
  outtext1(S1);
  outtext1("**************************************");
}

IMPLEMENT_DYNAMIC(cLinkedListT , CObject)

cLinkedListT::cLinkedListT()
{
iCnt=0;
Head=NULL;
pCur=NULL;
iLabel=-1;
iColour=-1;
sTitle="";
iObjType=202;
}

void cLinkedListT::Info()
{
  char S1[200];
  outtext1("**************************************");
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("TEMPERATURE SET");
  outtext1(S1); 
  sprintf_s(S1,"ID: %i TITLE: %s",iLabel,sTitle);
  outtext1(S1);
  outtext1("**************************************");
}


IMPLEMENT_DYNAMIC(cLinkedList , CObject)

cLinkedList::cLinkedList()
{
iCnt=0;
Head=NULL;
pCur=NULL;
iLabel=-1;
iColour=-1;
sTitle="";
iObjType=200;
}

void cLinkedList::Clear()
{
	iCnt = 0;
	Head = NULL;
	pCur = NULL;
	iLabel = -1;
	iColour = -1;
	sTitle = "";
	iObjType = 200;
}

//Results set class
cLinkedList::~cLinkedList()
{
	//THIS SHOULD NOT DELETE THE OBJECTS IN THE LIST
	//CALL DELETE ALL TO ACTUALLY DELETE ALL ELEMENTS

	//G_Object* pC;
	//G_Object* pNext = NULL;
	//pC = Head;
	//while (pC != NULL)
	//{
	//	pNext = pC->next;
	//	delete(pC);
	//	pC = pNext;
	//}
	//outtext1("CHECK THE DELETE MTHOD IS BEING USED CORRECTLY IN THIS CONTEXT");
	//outtext1("AS CODE DELETE HAS CHANGED NOT TO DELETE ITEMS IN THE DESRUCTOR");
	iCnt = 0;
	Head = NULL;
}

void cLinkedList::DeleteAll()
{
	G_Object* pC;
	G_Object* pNext = NULL;
	pC = Head;
	while (pC != NULL)
	{
		    pNext = pC->next;
			delete(pC);
			pC = pNext;
	}
	iCnt = 0;
	Head = NULL;
}

void cLinkedList::RemNoDelete(G_Object* inItem)
{
	if (inItem != NULL)
	{
		if ((inItem->before != NULL) && (inItem->next != NULL))       //in middle with a befor and after
		{
			inItem->before->next = inItem->next;
			inItem->next->before = inItem->before;
			iCnt--;
		}
		else if ((inItem->before == NULL) && (inItem->next == NULL))
		{
			Head->before = NULL;
			Head->next = NULL;
			Head = NULL;
			iCnt--;
		}
		else if (inItem->before == NULL)
		{
			Head = (cLink*)inItem->next;
			Head->before = NULL;
			iCnt--;
		}
		else if (inItem->next == NULL)
		{
			pCur = (cLink*)inItem->before;
			pCur->next = NULL;
			iCnt--;
		}
	}
}

void cLinkedList::Remove(G_Object* inItem)
{
if ((inItem!=NULL) && (inItem->pParent==this))
{
  if ((inItem->before!=NULL) && (inItem->next!=NULL))       //in middle with a befor and after
  {
    inItem->before->next=inItem->next;
    inItem->next->before=inItem->before;
	delete(inItem);
    iCnt--;
  }
  else if ((inItem->before==NULL) && (inItem->next==NULL))
  {
    Head->before=NULL;
    Head->next=NULL;
    Head=NULL;
	delete(inItem);
    iCnt--;
  }
  else if (inItem->before==NULL)
  {
	Head=(cLink*) inItem->next;
    Head->before=NULL;
	delete(inItem);
    iCnt--;
  }
  else if (inItem->next==NULL)
  {
	pCur=(cLink*) inItem->before;
    pCur->next=NULL;
	delete(inItem);
    iCnt--;
  }
}
}


void cLinkedList::Remove2(G_Object* inItem)
{
	if (inItem != NULL) 
	{
		if ((inItem->before != NULL) && (inItem->next != NULL))       //in middle with a befor and after
		{
			inItem->before->next = inItem->next;
			inItem->next->before = inItem->before;
			delete(inItem);
			iCnt--;
		}
		else if ((inItem->before == NULL) && (inItem->next == NULL))
		{

			Head->before = NULL;
			Head->next = NULL;
			Head = NULL;
			delete(inItem);
			iCnt--;
		}
		else if (inItem->before == NULL)
		{
			Head = (cLink*)inItem->next;
			Head->before = NULL;
			delete(inItem);
			iCnt--;
		}
		else if (inItem->next == NULL)
		{
			pCur = (cLink*)inItem->before;
			pCur->next = NULL;
			delete(inItem);
			iCnt--;
		}
	}
}

G_Object* cLinkedList::IsIn(G_Object* inItem)
{
	G_Object* pRet = NULL;
	G_Object* pNext;
	pNext = Head;
	while (pNext != NULL)
	{
		if (pNext == inItem)
		{
			pRet = pNext;
			break;
		}
		pNext = (G_Object*)pNext->next;
	}
	return (pRet);
}

void cLinkedList::AddEx(G_Object* inItem)
{
	if (!IsIn(inItem))
		Add(inItem);
}

void cLinkedList::Add(G_Object* inItem)
{
  if (Head==NULL)
  {
     Head=inItem;
     Head->before=NULL;
     pCur=Head;
     iCnt++;
  }
  else
  {
     inItem->before=pCur;
     pCur->next = inItem;
     pCur=inItem;
     pCur->next=NULL;
     iCnt++;
  }
 
}


G_Object* cLinkedList::GetByPtr(G_Object* pThis)
{
	int i;
	G_Object* pC;
	G_Object* pR = NULL;
	pC = Head;
	for (i = 0; i < iCnt; i++)
	{
		if (pC == pThis)
		{
			pR = pC;
			break;
		}
		else
		{
			pC = pC->next;
		}
	}
	return (pR);
}

G_Object* cLinkedList::Get(int ID)
{
int i;
G_Object* pC;
G_Object* pR=NULL;
pC=Head;
for (i=0;i<iCnt;i++)
{
  if (pC->iLabel==ID) 
  {
     pR=pC;
     break;
  }
  else
  {
     pC=pC->next;
  }
}
return (pR);
}

G_Object* cLinkedList::GetObj(int iType,int iLab)
{
int i;
G_Object* pC;
G_Object* pR=NULL;
pC=Head;
for (i=0;i<iCnt;i++)
{
  if ((pC->iLabel==iLab) && (pC->iObjType==iType))
  {
     pR=pC;
     break;
  }
  else
  {
     pC=pC->next;
  }
}
return (pR);
}

void cLinkedList::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->SetToScr(pModMat,pScrTran);
  pNext=(G_Object*) pNext->next;
}
}

void cLinkedList::GenIDS(int& iS)
{
	G_Object* pNext;
	pNext = Head;
	while (pNext != NULL)
	{
		pNext->iLabel = iS;
		iS++;
		pNext = (G_Object*)pNext->next;
	}
}


void cLinkedList::Draw(CDC* pDC,int iDrawmode)
{
G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->Draw(pDC,iDrawmode);
  pNext=(G_Object*) pNext->next;
}
}

void cLinkedList::HighLight(CDC* pDC)
{
G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->HighLight(pDC);
  pNext=(G_Object*) pNext->next;
}
}


void cLinkedList::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->OglDrawW(iDspFlgs,dS1,dS2);
  pNext=(G_Object*) pNext->next;
}
}


void cLinkedList::OglDraw(int iDspFlgs,double dS1,double dS2)
{
OglDrawW(iDspFlgs,dS1,dS2);
}


void cLinkedList::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
	if (pNext->isSelectable() == 1)
	{
		pNext->S_Box(P1, P2, pSel);
	}
  pNext=(G_Object*) pNext->next;
}

}

G_ObjectD cLinkedList::SelDist(CPoint InPT,Filter FIL)
{
G_Object* pSel=NULL;
G_Object* pMinSel=NULL;
G_ObjectD Ret;

double dDist = 1e36;
Ret.pObj=NULL;
Ret.Dist=dDist;

G_Object* pNext;
pNext=Head;
while (pNext!=NULL)
{
  Ret = pNext->SelDist(InPT,FIL);
  if (Ret.Dist < dDist)
  {
	pMinSel=Ret.pObj;
	dDist=Ret.Dist;
  }
  pNext=(G_Object*) pNext->next;
}


if ((FIL.isFilter(321)==TRUE) 
 || (FIL.isFilter(322)==TRUE)
 || (FIL.isFilter(323)==TRUE)
 || (FIL.isFilter(324)==TRUE)
 || (FIL.isFilter(325)==TRUE)
 || (FIL.isFilter(326)==TRUE)
 || (FIL.isFilter(327)==TRUE)
 || (FIL.isFilter(328)==TRUE)
 || (FIL.isFilter(329)==TRUE))
{
  Ret.pObj=pMinSel;
  Ret.Dist=dDist;
}
else
{
  Ret.pObj=this;
  Ret.Dist=dDist;
}
return (Ret);
}

//iType is resulatant entity type
//pThis the object that has the association
void cLinkedList::RelTo(G_Object* pThis,ObjList* pList,int iType)
{

  pThis->RelTo(pThis,pList,iType);

}

void cLinkedList::Info()
{
  char S1[200];
  outtext1("**************************************");
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("LOAD SET");
  outtext1(S1); 
  sprintf_s(S1,"ID: %i TITLE: %s",iLabel,sTitle);
  outtext1(S1);
  outtext1("**************************************");
}

void cLinkedList::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int i;
  int iType;
  BCLD* pNext;
  BCLD* pO;
  pNext=(BCLD*) Head;
  if (ar.IsStoring())
  {
	G_Object::Serialize(ar,iV);
	ar << iCnt;
	ar << sTitle;
    while (pNext!=NULL)
    {
      ar<<pNext->iObjType;
      pNext->Serialize(ar,iV,MESH);
      pNext=(BCLD*) pNext->next;
    }
  }
  else
  {
	G_Object::Serialize(ar,iV);
	int iNo;
	ar >> iNo;
	ar >> sTitle;
	for(i=0;i<iNo;i++)
	{
	ar>>iType;
    switch(iType) 
	{
	case 321:
          pO = new Force;
          break;
	case 322:
          pO = new Restraint;
          break;
	case 323:
          pO = new Moment;
          break;
    case 324:
          pO = new Pressure;
          break;
	case 325:
          pO = new Temperature;
          break;
	case 326:
          pO = new FluxLoad;
          break;
	case 327:
          pO = new TemperatureBC;
          break;		
    case 328:
          pO = new AccelLoad;
          break;	
    case 329:
      pO = new RotationLoad;
      break;
    }
      pO->Serialize(ar,iV,MESH);
      pO->pParent=this;
	  this->Add(pO);
	}
  }
}



//End of linked list implementation

//Results set class
ResSet::ResSet()
{
iCnt=0;
iNoV=0;
Head=NULL;
pCur=NULL;
sTitle="";
sSubTitle="";
sOpName="None";
iDefID=0;
iSecondaryID=-1;
int iNoResDef = 0;
}


//Results set class
ResSet::~ResSet()
{
int i;
Res* pC;
Res* pR=NULL;
pC=Head;
for (i=0;i<iCnt;i++)
{
  pR=pC->next;
  delete(pC);
  pC=pR;
}

for (i = 0; i < iNoResDef; i++)
{
	delete (ResDefintion[i]);
	ResDefintion[i] = NULL;
}

}

Res* ResSet::Get(int ID,int iOpt)
{
int i;
int iSecVar=0;
Res* pC;
Res* pR=NULL;
pC=Head;

//iOpt is a sesondary selection ID like layer for composite elements
for (i=0;i<iCnt;i++)
{
    
  if (iSecondaryID!=-1)   //This is layered element also need to select the correct layer
    {iSecVar=(int) *pC->GetAddress(iSecondaryID);}	   //Variable 0 is the layer iD
  else
    {iSecVar=0;}  //all other datatset read so far have no secondary ID
  if ((pC->ID==ID) && (iSecVar==iOpt))
  {
     pR=pC;
     break;
  }
  else
  {
     pC=pC->next;
  }
}
return pR;
}

int ResSet::getMaxSecondaryID()
{
int i;
int irc=0;
int iSecVar=0;
Res* pC;
pC=Head;
if (iSecondaryID!=-1)
{
  for (i=0;i<iCnt;i++)
  {
    iSecVar=(int) *pC->GetAddress(iSecondaryID);
	if (iSecVar>irc)
		irc=iSecVar;
    pC=pC->next;
  }
}
return(irc);
}

CString ResSet::ToStringDL(Res* pR)
{
	char S1[1000];
	CString outT;
	int j;
	if (pR != NULL)
	{
		outT = "";
		sprintf_s(S1, "%i,", pR->ID);
		outT += S1;
		for (j = 0; j < iNoV; j++)
		{
			sprintf_s(S1, "%e,", *pR->GetAddress(j));
			outT += S1;
		}
	}
	return (outT);
}

CString ResSet::ToStringHead()
{
	char S1[1000];
	CString outT;
	int i;
	outT = "";
	sprintf_s(S1, "%s,", "ID");
	outT += S1;
	for (i = 0; i < iNoV; i++)
	{
		sprintf_s(S1, "%s,", lab[i]);
		outT += S1;
	}
	return (outT);
}

void ResSet::PrintToFile(FILE* pFile)
{
int i;
fprintf(pFile,"%s\n","START OF DATA SET");
fprintf(pFile, "%s%s\n","FILE, ",sFile);
fprintf(pFile, "%s%i%s,%s\n","LC, ",LC ," ",sName );
fprintf(pFile, "%s%s\n","TITLE, ",sTitle);
fprintf(pFile, "%s%s\n","SUBTITLE, ",sSubTitle);
fprintf(pFile, "%s","ID,");
for (i=0;i<iNoV;i++)
{
   fprintf(pFile, "%s,",lab[i]);
}
fprintf(pFile,"\n");
int j;
Res* pC;
pC=Head;
for (i=0;i<iCnt;i++)
{
  if (pC!=NULL)
  {
    fprintf(pFile, "%i,",pC->ID);
    for (j=0;j<iNoV;j++)
    {
      fprintf(pFile, "%e,",*pC->GetAddress(j));
    }
    fprintf(pFile,"\n");
    pC=pC->next;
  }
}
fprintf(pFile,"%s\n","END OF DATA SET");
fprintf(pFile,"%s\n","");
}

ResDef* ResSet::GetResDef(int iN)
{
	ResDef* pRD = NULL;
	if ((iN > -1) && (iN < iNoResDef))
	{
		pRD = ResDefintion[iN];
	}
	return (pRD);
}

void ResSet::AddResDef(ResDef* rD)
{
	//MAX_RES_DEF = 5;
	int iNoRes = -1;
	if (iNoRes < MAX_RES_DEF)
	{
		ResDefintion[iNoResDef] = rD;
		iNoResDef++;
	}
	else
	{
		outtext1("Error Adding Results Definition");
	}
}

void ResSet::Add(Res* inRes)
{
  if (Head==NULL)
  {
     Head=inRes;
     pCur=inRes;
     iCnt++;
  }
  else
  {
     pCur->next = inRes;
     pCur=inRes;
     pCur->next=NULL;
     iCnt++;
  }
 
}

Filter::Filter()
{
iNo=0;

iType[0] = (0);		//POINT	
iType[1] =(1);		//NODE
iType[2] =(2);		//LINE
iType[3] =(3);		//ELEMENT
iType[4] =(4);		//MESH
iType[5] =(5);		//SYMBOL
iType[6] =(6);		//TEXT
iType[7] =(7);		//NURBS CURVE
iType[8] =(12);		//COORDSYS
iType[9] =(13);		//CURVE ON SURFACE
iType[10] =(14);		//SOLID SECTION
iType[11] =(15);	//NURBS SURFACE
iType[12] =(18);	//FACE
iType[13] =(19);	//SHELL
iType[14] =(20);	//PART
iType[15] =(200);	//LSET
iType[16] =(201);	//BSET
iType[17] =(202);	//TSET
iType[18] =(321);	//FORCE
iType[19] =(323);	//MOMENT
iType[20] =(324);	//PRESSURE
iType[21] =(322);	//RESTRAINT
iType[22] =(325);	//TEMPERATURE
iType[23] =(326);	//FLUX LOAD
iType[24] =(327);	//T BC
iType[25] =(328);	//ACCEL LOAD
iType[26] =(329);	//ROTATION ACCEL LOAD
iType[27] =(330);	//RESULTS VECTOR

sType[0] = "POINT";
sType[1] = "NODE";
sType[2] = "LINE NOT USED";
sType[3] = "ELEMENT";
sType[4] = "MESH";
sType[5] = "SYMBOL";
sType[6] = "TEXT";
sType[7] = "CURVE";
sType[8] = "COORDSYS";
sType[9] = "CURVE ON SURFACE";
sType[10] = "SECTION";
sType[11] = "SURFACE";
sType[12] = "FACE";
sType[13] = "SHELL";
sType[14] = "PART";
sType[15] = "LOAD SET";        // LOAD SET
sType[16] = "BC SET";          //SET
sType[17] = "TEMPERATURE SET"; //SET
sType[18] = "FORCE";
sType[19] = "MOMENT";
sType[20] = "PRESSURE";
sType[21] = "RESTRAINT";
sType[22] ="TEMP STRUCTURAL";
sType[23] ="NET FLUX Q";
sType[24] ="TEMP BC T";
sType[25] ="ACCEL BODY LOAD";
sType[26] = "ROTATIONAL BODY LOAD";
sType[27] = "RESULTS VECTOR";
iNoOfType=28;

//USED IN ASTRIUMS QUANTA PROGRAM
//iType[17] =(500); //WG DEF
//iType[18] =(600); //PART
//iType[19] =(601); //ELBOW
//iType[20] =(602); //BRK
//iType[21] =(603); //SWEEP
//iType[22] =(604); //SWEEP
//iType[23] =(605); //BRK INT
}

void Filter::SetAll()
{
iNo=0;
int i;
for (i=0;i<iNoOfType;i++)
{
  SetFilter(iType[i]);
}
}

void Filter::Clear()
{
iNo=0;
}

int Filter::isIn(int iThisType)
{
int iRC=-1;
int i;
for(i=0;i<iNo;i++)
{
  if (Filt[i]==iThisType)
  {
     iRC=i;
	 break;
  }
}
return (iRC);
}


void Filter::SetFilter(int iThisType)
{
if (isIn(iThisType)==-1)
{
	Filt[iNo]=iThisType;
	iNo++;
} 
}

void Filter::RemFilter(int iThisType)
{
int ind;
ind = isIn(iThisType);
if (ind!=-1)
{
	Filt[ind]=Filt[iNo-1];
	iNo--;
} 
}

BOOL Filter::isFilter(int iThisType)
{
if (isIn(iThisType)!=-1)
{
  return(TRUE);
}
else
{
  return (FALSE);
}
}




dList::dList()
{
iNo=0;
}

void dList::Init()
{
  iNo=0;
}

void dList::Add(double indbl)
{
  if (iNo<1000)
  {
    d[iNo]=indbl;
    iNo++;
  }
  else
  {
     outtext1("ERROR: Max List Items Exceeded.");
  }
}

void dList::AddEx(double indbl)
{
  if (iNo<1000)
  {
    if (IsIn(indbl)==-1)
    {
      d[iNo]=indbl;
      iNo++;
    }
  }
  else
  {
     outtext1("ERROR: Max List Items Exceeded.");
  }
}


void dList::Sort()
{
int i,j;
double tD;

for (i=0;i<iNo;i++)
{
  for (j=i;j<iNo;j++)
  {
    if (d[j]<d[i])
    {
      tD=d[i];
      d[i]=d[j];
      d[j]=tD;
    }
  }
}
}

int dList::IsIn(double indbl)
{
int iRet=-1;
int i;
for (i=0;i<iNo;i++)
{
  if (d[i]==indbl)
  {
     iRet=i;
     break;
  }
}
return (iRet);
}


NEList::NEList()
{
Title="NULL";
iNo=0;
}

int NEList::Get(int iD)
{
int iRet=-1;
int i;
for (i=0;i<iNo;i++)
{
  if (ids[i]==iD)
  {
    iRet=iType[i];
    break;
  }
}
return (iRet);
}

void NEList::Add(int iP,int iT)
{
if (iNo<MAX_LITEMS)
{
  ids[iNo]=iP;
  iType[iNo]=iT;
  iNo++;
}
else
{
   outtext1("ERROR: Max NEList Items Exceeded.");
}

}




ObjList::ObjList()
{
iNo=0;
}

ObjList::~ObjList()
{
	//DeleteAll();
}

void ObjList::Add(G_Object* inItem)
{
if (iNo<MAX_GP_ITEMS)
{
  Objs[iNo] = inItem;
  iNo++;
}
}

void ObjList::AddEx(G_Object* inItem)
{
BOOL bIsIn;
bIsIn=IsIn(inItem);
if (bIsIn==FALSE)
{
  Objs[iNo] = inItem;
  iNo++;
}
}

G_Object* ObjList::GetMaxLabItem()
{
G_Object* pRet=NULL;
int i;
int iMax=0;
for (i=0;i<iNo;i++)
{
  if (Objs[i]->iLabel>iMax)
  {
    iMax=Objs[i]->iLabel;
    pRet=Objs[i];
  }
}
return(pRet);
}


G_Object* ObjList::GetMinLabItem()
{
G_Object* pRet=NULL;
int i;
int iMin=99999999;
for (i=0;i<iNo;i++)
{
  if (Objs[i]->iLabel<iMin)
  {
    iMin=Objs[i]->iLabel;
    pRet=Objs[i];
  }
}
return(pRet);
}

G_Object* ObjList::GetByLab(int iLab)
{
int i;
G_Object* pRet=NULL;
for (i=0;i<iNo;i++)
{
  if (Objs[i]->iLabel==iLab)
  {
    pRet=Objs[i];
    break;
  }
}
return (pRet);
}


BOOL ObjList::IsIn(G_Object* inItem)
{
int i;
BOOL bRet=FALSE;
for (i=0;i<iNo;i++)
{
  if (Objs[i]==inItem)
  {
    bRet=TRUE;
    break;
  }
}
return (bRet);
}

int ObjList::IsIn2(G_Object* inItem)
{
int i;
int iRet=-1;
for (i=0;i<iNo;i++)
{
  if (Objs[i]==inItem)
  {
    iRet=i;
    break;
  }
}
return (iRet);
}

void ObjList::RemoveGP(ObjList* inRemGp)
{
  int i;
  for (i=0;i<inRemGp->iNo;i++)
  {
    this->Remove(inRemGp->Objs[i]);
  }
}


void ObjList::Remove(G_Object* inItem)
{
  int iO=IsIn2(inItem);
  int i;
  if (iO!=-1)
  {
    for (i=iO+1;i<iNo;i++)
	{
      Objs[i-1]=Objs[i];
	}
    Objs[iNo-1]=NULL;
    iNo--;
  }
}




//Sort items in list based on min distance of centroid to vPt
void ObjList::MinDistSort(C3dVector vPt)
{
  int i;
  int j;
  double dMinDist;
  double dDist;
  G_Object* oT;

  

  for (i=0;i<iNo-1;i++)
  {
	dMinDist = vPt.Dist(this->Objs[i]->Get_Centroid());
    for (j=i+1;j<iNo;j++)
    {
      dDist = vPt.Dist(this->Objs[j]->Get_Centroid());
	  if (dDist<dMinDist)
	  {
         dMinDist=dDist;
         oT=Objs[i];
         Objs[i]=Objs[j];
         Objs[j]=oT;
	  }
    }
  }
}

void ObjList::MinSizeSort()
{
  int i;
  int j;
  double dMinSize;
  double dSize;
  G_Object* oT;

  for (i=0;i<iNo-1;i++)
  {
	dMinSize = this->Objs[i]->GetCharSize();
    for (j=i+1;j<iNo;j++)
    {
      dSize = this->Objs[j]->GetCharSize();
	  if (dSize<dMinSize)
	  {
         dMinSize=dSize;
         oT=Objs[i];
         Objs[i]=Objs[j];
         Objs[j]=oT;
	  }
    }
  }
}

void ObjList::Clear()
{
iNo=0;
}


void ObjList::DeleteAll()
{
	int i;
	for (i = 0; i < iNo; i++)
	{
		if (Objs[i]!=NULL)
		  delete (Objs[i]);
		Objs[i] = NULL;
	}
	iNo = 0;
}

void ObjList::GenIDS(int& iS)
{
	int i;
	for (i = 0; i < iNo; i++)
	{
		Objs[i]->iLabel = iS;
		iS++;
	}
}

void ObjList::ReOrder(int iStart)
{
	int i;
	for (i = 0; i < iStart; i++)
	{
		Objs[iNo+i] = Objs[i];
	}
	for (i = 0; i < iNo; i++)
	{
		Objs[i] = Objs[i+iStart];
	}
	for (i = 0; i < iStart; i++)
	{
		Objs[iNo + i] = NULL;
	}
}

void ObjList::ListIDs()
{
  char S1[200];
  CString outT;
  int i;
  outtext1("LAB");
  for (i=0;i<iNo;i++)
  {
    sprintf_s(S1,"%i,",Objs[i]->iLabel);
	//outtext1(S1);
    outT+=S1;
  }
  outtext1(outT);
  //outtext1("D");
}

ObjGp::ObjGp()
{
Title="";
iNo=0;
}



ObjGp::ObjGp(CString inTit)
{
Title=inTit;
iNo=0;
}

void ObjGp::Clear()
{
iNo=0;
}


void ObjGp::Add(G_Object* inItem)
{
if (IsIn(inItem)==-1)
{
  if (iNo<MAX_GP_ITEMS)
  {
    Objs[iNo] = inItem;
    iNo++;
  }
  else
  {
    outtext1("ERROR: Group Overflow.");
  }
}
}

void ObjGp::Rem(G_Object* inItem)
{

int i;
for (i=0;i<iNo;i++)
{
  if (Objs[i] == inItem)
  {
    Objs[i] = Objs[iNo-1];
	iNo--;
	break;
  }
}
}

int ObjGp::IsIn(G_Object* inItem)
{

int i;
int iRet=-1;
for (i=0;i<iNo;i++)
{
  if (Objs[i] == inItem)
  {
    iRet=i;
	break;
  }
}
return (iRet);
}

IMPLEMENT_DYNAMIC(BMP, CObject)



BMP::BMP()
{
    // Data read from the header of the BMP file
    dataPos=0;
    width=0;
    height = 0;
    imageSize = 0;
    data=NULL;
}
 
BMP::~BMP()
{
    free(data);
    dataPos = 0;
    width = 0;
    height = 0;
    imageSize = 0;
    data = NULL;
}



// Create Shade Representation of object

IMPLEMENT_DYNAMIC( G_Object, CObject )


void G_Object::SetVisable(int iOnOff)
{
Visable = iOnOff;
}

void G_Object::SetSelectable(int iOnOff)
{
Selectable = iOnOff;
}

int G_Object::GetCol()
{
int irc;
if ((iColour>=0) && (iColour<=17))
{
  irc=iColour;
}
else
{
  irc=7;
}
return (iColour);
}

void G_Object::SetDrawn(int iOnOff)
{
Drawn = iOnOff;
}


G_Object* G_Object::GetObj(int iType,int iLab)
{

return (NULL);
}


int G_Object::isVisable()
{
return (Visable);
}

int G_Object::isDrawn()
{
return (Drawn);
}

int G_Object::isSelectable()
{
return (Selectable);
}

G_Object::G_Object()
{
iType = -1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
bDrawLab = FALSE;
pParent=NULL;
next=NULL;

}

void G_Object::GetBoundingBox(C3dVector& vll,C3dVector& vur)
{
vll.Set(0,0,0);
vur.Set(0,0,0);
}

double G_Object::GetCharSize()
{
return (1.0);
}

CString G_Object::GetName()
{
	return("Base Object");
}

int G_Object::GetVarHeaders(CString sVar[])
{
	sVar[0] = "NULL";
	return(1);
}


int G_Object::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", -1);
	sVar[0] = S1;
	iNo++;
	return (iNo);
}

void G_Object::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{

}

G_Object::~G_Object()
{

}

// Create Object
		
void G_Object::Create()
{
iType = -1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
pParent=NULL;
bDrawLab = FALSE;
}


C3dVector G_Object::MinPt(C3dVector inPt)
{
C3dVector vRet;
return vRet;
}


G_Object* G_Object::Copy(G_Object* parrent)
{
//G_Object* gret = new G_Object;
//gret->iObjType=iObjType;
//gret->iLabel=iLabel;
//gret->iColour=iColour;
//gret->Drawn = Drawn;
//gret->Selectable  = Selectable; 
//gret->Visable  = Visable;
return (NULL);
}


void G_Object::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    ar << iObjType;
	ar << iType;
    ar << iLabel;
    ar << iColour;
    ar << Drawn;
    ar << Selectable; 
    ar << Visable;
	}
	else
	{
    ar >> iObjType;
	if (iV < -52)
		ar >> iType;
	else
		iType = -1;
    ar >> iLabel;
    ar >> iColour;
    ar >> Drawn;
    ar >> Selectable; 
    ar >> Visable;
	}
}

CString G_Object::ToString()
{
return("");
}

void G_Object::Info()
{
  char S1[80];
  CString OutT;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  OutT+=S1;
  outtext1(OutT); 
}

C3dVector G_Object::Get_Centroid()
{
C3dVector vRet;
vRet.Set(0,0,0);
return (vRet);
}

C3dVector G_Object::Get_Normal()
{
C3dVector vRet;
vRet.Set(0,0,1);
return (vRet);
}

void G_Object::Draw(CDC* pDC,int iDrawmode)
{
}

void G_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
}

void G_Object::OglDrawW(int DspFlags,double dS1,double dS2)
{
}



void G_Object::Transform(C3dMatrix TMat)
{

}

void G_Object::Translate(C3dVector vIn)
{


}




void G_Object::Move(C3dVector vM)
{

}

G_Object* G_Object::OffSet(C3dVector vN,C3dVector vDir,double Dist)
{
return (NULL);
}










C3dVector G_Object::GetPt(double w)
{
C3dVector vR;
return (vR);
}

C3dVector G_Object::GetInPt()
{
C3dVector vR;
return (vR);
}


C3dVector G_Object::GetDir(double w)
{
C3dVector vR;
return (vR);
}

double G_Object::getLen()
{
return (0);
}

void G_Object::HighLight(CDC* pDC)
{
pDC->Ellipse(int(SelPt.x+8),int(SelPt.y+8),int(SelPt.x-8),int(SelPt.y-8));
}

void G_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
C3dVector vC;
vC=Get_Centroid();
vC.SetToScr(pModMat,pScrTran);
SelPt=vC;
}

G_ObjectD G_Object::SelDist(CPoint InPT,Filter FIL)
{
G_ObjectD Ret;
double x,y,d;
x =  SelPt.x-InPT.x;
y =  SelPt.y-InPT.y;
d=(x*x+y*y);
Ret.Dist=d;
Ret.pObj=this;
return (Ret);
}

void G_Object::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
if ((SelPt.x > P1.x) && 
	(SelPt.x < P2.x) &&
    (SelPt.y > P1.y) &&
    (SelPt.y < P2.y))
{
	pSel->Add(this);
}
}

void G_Object::Colour(int iCol)
{
iColour = iCol;
}


void G_Object::RelTo(G_Object* pThis,ObjList* pList,int iType)
{
  if (pThis->pParent!=NULL)
  {
     if (pThis->pParent->iObjType==iType)
     {
        pList->AddEx(pThis->pParent);
     }
  }
}

//**********************************************************************
//Test class for the planet
//**********************************************************************
IMPLEMENT_DYNAMIC(Planet, CObject)


Planet::Planet()
{
  Drawn = 0;
  Selectable = 1;
  Visable = 1;
  iObjType = 1;
  pParent = NULL;
  pTexture=NULL;
}

Planet::~Planet()
{
  pTexture = NULL;
}

void Planet::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
  GLuint textureID;
  glColor3fv(cols[124]);
  glGenTextures(1, &textureID);
  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);
  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pTexture->width, pTexture->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pTexture->data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_NORMALIZE);
  double dDec;
  double dDec1;
  double dRA;
  double dRA1;
  C3dVector n0;
  C3dVector n1;
  C3dVector n2;
  C3dVector n3;
  float R=1;
  float x0,y0,z0;
  float x1, y1, z1;
  float x2, y2, z2;
  float x3, y3, z3;
  float tRA,tRA1,tDEC,tDEC1;
  int i,j;
  i=0,j=0;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glShadeModel(GL_SMOOTH);
  for (i=0;i<48;i++)
  {
    for (j=0;j<36;j++)
    {
      dRA=i*7.5;
      dRA1 = (i+1) * 7.5;
      dDec=j*5-90;
      dDec1=(j+1)*5-90;
      tRA=dRA/360;
      tRA1=dRA1/360;
      tDEC=(dDec+90)/180;
      tDEC1 = (dDec1 + 90) / 180;
      x0 = R*cos(D2R*dDec)*cos(D2R*dRA);
      y0 = R*cos(D2R*dDec)*sin(D2R*dRA);
      z0 = R*sin(D2R*dDec);
      x1 = R*cos(D2R*dDec)*cos(D2R*dRA1);
      y1 = R*cos(D2R*dDec)*sin(D2R*dRA1);
      z1 = R*sin(D2R*dDec);
      x2 = R*cos(D2R*dDec1)*cos(D2R*dRA1);
      y2 = R*cos(D2R*dDec1)*sin(D2R*dRA1);
      z2 = R*sin(D2R*dDec1);
      x3 = R*cos(D2R*dDec1)*cos(D2R*dRA);
      y3 = R*cos(D2R*dDec1)*sin(D2R*dRA);
      z3 = R*sin(D2R*dDec1);
      n0.Set(x0,y0,z0);
      n1.Set(x1, y1, z1);
      n2.Set(x2, y2, z2);
      n3.Set(x3, y3, z3);
      n0.Normalize();
      n1.Normalize();
      n2.Normalize();
      n3.Normalize();
      glBegin(GL_POLYGON);
        glTexCoord2f(tRA, tDEC);
        glVertex3f(x0, y0, z0);
        glNormal3f((float)n0.x, (float)n0.y, (float)n0.z);
        glTexCoord2f(tRA1, tDEC);
        glVertex3f(x1, y1, z1);
        glNormal3f((float)n1.x, (float)n1.y, (float)n1.z);
        glTexCoord2f(tRA1, tDEC1);
        glVertex3f(x2, y2, z2);
        glNormal3f((float)n2.x, (float)n2.y, (float)n2.z);
        glTexCoord2f(tRA, tDEC1);
        glVertex3f(x3, y3, z3);
        glNormal3f((float)n3.x, (float)n3.y, (float)n3.z);
      glEnd();
    }
  }

}

void Planet::OglDraw(int iDspFlgs, double dS1, double dS2)
{
  OglDrawW(iDspFlgs, dS1, dS2);
}

void Planet::Draw(CDC* pDC, int iDrawmode)
{
  
}

void Planet::AttachTexture(BMP* pT)
{
  pTexture = pT;
}


//**********************************************
IMPLEMENT_DYNAMIC( Pt_Object, CObject )
// Create Object

Pt_Object::Pt_Object()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 1;
pResV=NULL;
pResD=NULL;
pParent=NULL;
Pt_Point = NULL;
DSP_Point = NULL;
dof[0]=0;
dof[1]=0;
dof[2]=0;
dof[3]=0;
dof[4]=0;
dof[5]=0;
pN=NULL;
}

Pt_Object::~Pt_Object()
{
 delete(Pt_Point);
 delete(DSP_Point);
 Pt_Point=NULL;
 DSP_Point=NULL;
}
		
void Pt_Object::Create(C3dVector InPt, int iLab,int i2,int i3, int iC,int iDef,int iOut,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 1;
iColour = iC;
iLabel = iLab;
Pt_Point = new C3dVector(InPt.x,InPt.y,InPt.z);
DSP_Point = new C3dVector();
pParent=Parrent;
dof[0]=0;
dof[1]=0;
dof[2]=0;
dof[3]=0;
dof[4]=0;
dof[5]=0;
DefSys=iDef;
OutSys=iOut;
pResV=NULL;
pResD=NULL;
pN=NULL;
}

void Pt_Object::Info()
{
  char S1[80];
  G_Object::Info();
  sprintf_s(S1,"GRID %8i X %s Y %s Z %s DEFSYS %i OUTSYS %i",iLabel,float8NAS(Pt_Point->x),float8NAS(Pt_Point->y),float8NAS(Pt_Point->z),DefSys,OutSys);
  outtext1(S1); 
}

CString Pt_Object::ToString()
{
CString sRT;
char S1[80];
sprintf_s(S1,"%8s%8i%8i%8s%8s%8s%8i\n","GRID    ",iLabel,DefSys,e8(Pt_Point->x),e8(Pt_Point->y),e8(Pt_Point->z),OutSys);
sRT=S1;
return(sRT);
}

C3dVector Pt_Object::MinPt(C3dVector inPt)
{
C3dVector vRet;
vRet.Set(Pt_Point->x,Pt_Point->y,Pt_Point->z);
return (vRet);
}


void Pt_Object::ExportUNV(FILE* pFile)
{
fprintf(pFile,"%10i%10i%10i%10i\n",iLabel,1,1,iColour-150);
fprintf(pFile,"%25.16E%25.16E%25.16E\n",Pt_Point->x,Pt_Point->y,Pt_Point->z);
}

void Pt_Object::ExportNAS(FILE* pFile, CoordSys* pD)
{
   //New to handle the DEF system eventually
  if (iLabel==3000039)
  {
    int kk;
    kk=0;
  }
  C3dVector pt(Pt_Point->x,Pt_Point->y,Pt_Point->z);
  if (pD!=NULL)
  {
    C3dMatrix A=pD->mOrientMat;
    A.Transpose();
    if (pD->CysType==1)
    {
      pt-=pD->Origin;
	    pt = A*pt;
    }
    else if (pD->CysType==2)
    {
      pt-=pD->Origin;
	    pt = A*pt;
      C3dVector pCyl;
      pCyl.x=sqrt(pt.x*pt.x+pt.y*pt.y);
      pCyl.y=atan2(pt.y,pt.x)*R2D;
      pCyl.z=pt.z;
      pt=pCyl;
    }
    else if (pD->CysType==3)
    {
      pt-=pD->Origin;
	    pt = A*pt;
      C3dVector pCyl;
      pCyl.x=sqrt(pt.x*pt.x+pt.y*pt.y+pt.z*pt.z);
      pCyl.y=acos(pt.z/pCyl.x)*R2D;
      pCyl.z=atan2(pt.y,pt.x)*R2D;
      pt=pCyl;
    }
  }
   fprintf(pFile,"%8s%8i%8i%8s%8s%8s%8i\n","GRID    ",iLabel,DefSys,e8(pt.x),e8(pt.y),e8(pt.z),OutSys);
}

G_Object* Pt_Object::Copy(G_Object* Parrent)
{
Pt_Object* PtRet = new Pt_Object;
PtRet->pParent=Parrent;
PtRet->Drawn = Drawn;
PtRet->Selectable  = Selectable; 
PtRet->Visable  = Visable;
PtRet->iColour = iColour;
PtRet->iObjType = iObjType;
PtRet->iLabel = iLabel;
PtRet->Pt_Point = new C3dVector(Pt_Point->x,Pt_Point->y,Pt_Point->z);
PtRet->DSP_Point = new C3dVector();
PtRet->OutSys = OutSys;
PtRet->DefSys = DefSys;

pResV=NULL;
pResD=NULL;
return (PtRet);
}

void Pt_Object::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
	ar<<DefSys;
    ar<<OutSys;
    ar<<Pt_Point->x;
    ar<<Pt_Point->y;
    ar<<Pt_Point->z;  
	}
	else
	{
    G_Object::Serialize(ar,iV);
	  ar>>DefSys;
    ar>>OutSys;
    Pt_Point = new C3dVector();
    DSP_Point = new C3dVector();
    ar>>Pt_Point->x;
    ar>>Pt_Point->y;
    ar>>Pt_Point->z; 
    pResV=NULL;
    pResD=NULL;
	}
}





void Pt_Object::Clear()
{

}

C3dVector Pt_Object::GetCoords() 
{
C3dVector vRet;
vRet.Set(Pt_Point->x,Pt_Point->y,Pt_Point->z);
return (vRet);
}

C3dVector Pt_Object::Get_Centroid()
{
return (GetCoords());
}



// Draw Object line
void Pt_Object::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) DSP_Point->x-4,(int) DSP_Point->y+4);
pDC->LineTo((int) DSP_Point->x+4,(int) DSP_Point->y-4);
pDC->MoveTo((int) DSP_Point->x+4,(int) DSP_Point->y+4);
pDC->LineTo((int) DSP_Point->x-4,(int) DSP_Point->y-4);
pDC->MoveTo((int) DSP_Point->x+4,(int) DSP_Point->y);
pDC->LineTo((int) DSP_Point->x-5,(int) DSP_Point->y);
pDC->MoveTo((int) DSP_Point->x,(int) DSP_Point->y-4);
pDC->LineTo((int) DSP_Point->x,(int) DSP_Point->y+4);
//pDC->SetPixel(DSP_Point->x, DSP_Point->y+4, 255 );

//pDC->Ellipse(DSP_Point->x+3,DSP_Point->y+3,DSP_Point->x-3,DSP_Point->y-3);
}




GLubyte BmpND[22] = {0x04,0x00,0x44,0x40,0x24,0x80,0x15,0x00,0x0e,0x00,0xff,0xe0,0x0e,0x00,0x15,0x00,0x24,0x80,0x44,0x40,0x04,0x00};

void Pt_Object::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
double x;
double y;
double z;
double dS=0;
double dRF = 1.0;
int ind;
C3dVector vD;
ME_Object* ME=(ME_Object*) this->pParent;
dS=ME->dScale;
dRF = ME->dResFactor;
ind=ME->iCVar;
if ((iDspFlgs & DSP_NODES)>0)
{
  x=Pt_Point->x;
  y=Pt_Point->y;
  z=Pt_Point->z;
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    if (pResD!=NULL)
    {
       vD=pResD->GetVec();
	   vD -= ME->vRelDispOff;
       x+=vD.x*dS*dRF;
       y+=vD.y*dS*dRF;
       z+=vD.z*dS*dRF;
    }
  }

	Selectable=1;
	glColor3fv(cols[GetCol()]);
    glPointSize(10.0f); 
	if ((iDspFlgs & DSP_NODES_ASK)>0)
	{
	glBegin(GL_POINTS);
      glVertex3f((float) x,(float) y,(float) z);
    glEnd();   

	}
	else
	{
     glRasterPos3f ((float) x,(float) y,(float) z);
     glBitmap(11,11, 5.0, 5.0, 0.0, 0.0, BmpND);
	}

	if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"N%i",iLabel);
	  OglString(iDspFlgs,(float) x,(float) y,(float) z,&sLab[0]);
    }
  if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
  {
    sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
    OglString(iDspFlgs,(float) x,(float) y,(float) z,&sLab[0]);
  }
}
else
{
   Selectable=0;
}
}

void Pt_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
   OglDrawW(iDspFlgs,dS1,dS2);
}

void Pt_Object::Move(C3dVector vM)
{
Pt_Point->x +=vM.x;
Pt_Point->y +=vM.y;
Pt_Point->z +=vM.z;
}

void Pt_Object::Transform(C3dMatrix TMAt)
{
C3dVector R;
R.x =  TMAt.m_00 * Pt_Point->x +  TMAt.m_01 * Pt_Point->y +  TMAt.m_02 * Pt_Point->z +  TMAt.m_30;
R.y =  TMAt.m_10 * Pt_Point->x +  TMAt.m_11 * Pt_Point->y +  TMAt.m_12 * Pt_Point->z +  TMAt.m_31;
R.z =  TMAt.m_20 * Pt_Point->x +  TMAt.m_21 * Pt_Point->y +  TMAt.m_22 * Pt_Point->z +  TMAt.m_32;
Pt_Point->x =R.x;
Pt_Point->y =R.y;
Pt_Point->z =R.z;

}

void Pt_Object::Translate(C3dVector vIn)
{
Pt_Point->x +=vIn.x;
Pt_Point->y +=vIn.y;
Pt_Point->z +=vIn.z;
}



void Pt_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{

C3dVector V;
C3dVector R;
G_Object::SetToScr(pModMat, pScrTran);
  
R.x = Pt_Point->x;
R.y = Pt_Point->y;
R.z = Pt_Point->z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;

R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;


DSP_Point->x = R.x;
DSP_Point->y = R.y;
DSP_Point->z = R.z;
}

void Pt_Object::SetTo(C3dVector cInVect)
{
Pt_Point->x = cInVect.x;
Pt_Point->y = cInVect.y;
Pt_Point->z = cInVect.z;
}

CString Pt_Object::GetName()
{
	return ("Node (GRID)");
}

int Pt_Object::GetVarHeaders(CString sVar[])
{
	int iNo = 0;

	sVar[iNo] = "Def CYS";
	iNo++;
	sVar[iNo] = "Out Angle";
	iNo++;
	sVar[iNo] = "X";
	iNo++;
	sVar[iNo] = "Y";
	iNo++;
	sVar[iNo] = "Z";
	iNo++;
	return(iNo);
}


int Pt_Object::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", DefSys);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", OutSys);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", Pt_Point->x);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", Pt_Point->y);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", Pt_Point->z);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void Pt_Object::PutVarValues(PropTable* PT,int iNo, CString sVar[])
{

	ME_Object* pMe = (ME_Object*)this->pParent;

	DefSys = atoi(sVar[0]);
	OutSys = atof(sVar[1]);;
	Pt_Point->x = atof(sVar[2]);;;
	Pt_Point->y = atof(sVar[3]);;;
	Pt_Point->z = atof(sVar[4]);;;
}

cFace::cFace()
{
iColour=1;
NoVert=0;
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
pParent=NULL;
next=NULL;
}

cFace::~cFace()
{

NoVert=0;
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
pParent=NULL;
next=NULL;
}

BOOL cFace::isSame(cFace* inFace)
{
BOOL brc=FALSE;
int i,j,iC;
iC=0;
if (NoVert==inFace->NoVert)
{
  for (i=0;i<NoVert;i++)
  {
    for (j=0;j<NoVert;j++)
    {
      if (pVertex[i]==inFace->pVertex[j])
	  {
        iC++;
	  }
    }
  }
  if (iC==NoVert)
  {
    brc=TRUE;
  }
}
return (brc);
}

void cFace::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
C3dVector v1;
C3dVector v2;
C3dVector Vn;
glLineWidth(5.0);
glColor3fv(cols[124]);
if (NoVert==3)
{
  glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x),(float) (pVertex[0]->Pt_Point->y),(float) (pVertex[0]->Pt_Point->z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x),(float) (pVertex[1]->Pt_Point->y),(float) (pVertex[1]->Pt_Point->z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x),(float) (pVertex[1]->Pt_Point->y),(float) (pVertex[1]->Pt_Point->z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x),(float) (pVertex[2]->Pt_Point->y),(float) (pVertex[2]->Pt_Point->z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x),(float) (pVertex[2]->Pt_Point->y),(float) (pVertex[2]->Pt_Point->z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x),(float) (pVertex[0]->Pt_Point->y),(float) (pVertex[0]->Pt_Point->z));
  glEnd();
}
else if (NoVert==4)
{
  glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x),(float) (pVertex[0]->Pt_Point->y),(float) (pVertex[0]->Pt_Point->z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x),(float) (pVertex[1]->Pt_Point->y),(float) (pVertex[1]->Pt_Point->z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x),(float) (pVertex[1]->Pt_Point->y),(float) (pVertex[1]->Pt_Point->z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x),(float) (pVertex[2]->Pt_Point->y),(float) (pVertex[2]->Pt_Point->z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x),(float) (pVertex[2]->Pt_Point->y),(float) (pVertex[2]->Pt_Point->z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x),(float) (pVertex[3]->Pt_Point->y),(float) (pVertex[3]->Pt_Point->z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x),(float) (pVertex[3]->Pt_Point->y),(float) (pVertex[3]->Pt_Point->z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x),(float) (pVertex[0]->Pt_Point->y),(float) (pVertex[0]->Pt_Point->z));
  glEnd();
}
glLineWidth(2.0);
}

void cFace::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector v1;
C3dVector v2;
C3dVector Vn;
if (NoVert==3)
{
  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

  v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
  v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
  v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
  Vn = v1.Cross(v2);

  Vn.Normalize();
  glBegin(GL_POLYGON);
  glColor3fv(cols[iColour]);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[0]->Pt_Point->x,(float) pVertex[0]->Pt_Point->y,(float) pVertex[0]->Pt_Point->z);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[1]->Pt_Point->x,(float) pVertex[1]->Pt_Point->y,(float) pVertex[1]->Pt_Point->z);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[2]->Pt_Point->x,(float) pVertex[2]->Pt_Point->y,(float) pVertex[2]->Pt_Point->z);
  glEnd();
}
else if (NoVert==4)
{
  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;
  v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
  v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
  v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
  Vn = v1.Cross(v2);
  Vn.Normalize();
  glBegin(GL_POLYGON);
  glColor3fv(cols[iColour]);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[0]->Pt_Point->x,(float) pVertex[0]->Pt_Point->y,(float) pVertex[0]->Pt_Point->z);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[1]->Pt_Point->x,(float) pVertex[1]->Pt_Point->y,(float) pVertex[1]->Pt_Point->z);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[2]->Pt_Point->x,(float) pVertex[2]->Pt_Point->y,(float) pVertex[2]->Pt_Point->z);
  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
  glVertex3f((float) pVertex[3]->Pt_Point->x,(float) pVertex[3]->Pt_Point->y,(float) pVertex[3]->Pt_Point->z);
  glEnd();
}
}


cLink::cLink()
{
iColour=-1;
pVertex[0]=NULL;
pVertex[1]=NULL;
next=NULL;
before=NULL;
pParent=NULL;
}

cLink::~cLink()
{
iColour=-1;
pVertex[0]=NULL;
pVertex[1]=NULL;
next=NULL;
before=NULL;
pParent=NULL;
}

BOOL cLink::isSame(cLink* inLink)
{
BOOL brc=FALSE;
if (inLink != NULL)
{
  if ((pVertex[0] == inLink->pVertex[0]) &&
    (pVertex[1] == inLink->pVertex[1]))
  {
    brc = TRUE;
  }
  else if ((pVertex[1] == inLink->pVertex[0]) &&
    (pVertex[0] == inLink->pVertex[1]))
  {
    brc = TRUE;
  }
}
return (brc);
}


//0 not the same
//1 the same
//2 the same bit wrond direction

int cLink::isSameWithDir(cLink* inLink)
{
  int irc = 0;
  if ((pVertex[0] == inLink->pVertex[0]) &&
      (pVertex[1] == inLink->pVertex[1]))
  {
    irc = 1;
  }
  else if ((pVertex[1] == inLink->pVertex[0]) &&
           (pVertex[0] == inLink->pVertex[1]))
  {
    irc = 2;
  }
  return (irc);
}

void cLink::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
glLineWidth(5.0);
glColor3fv(cols[124]);
glBegin(GL_LINES);
glVertex3f((float) pVertex[0]->Pt_Point->x,(float) pVertex[0]->Pt_Point->y,(float) pVertex[0]->Pt_Point->z);
glVertex3f((float) pVertex[1]->Pt_Point->x,(float) pVertex[1]->Pt_Point->y,(float) pVertex[1]->Pt_Point->z);
glEnd();
glLineWidth(2.0);
}

void cLink::OglDraw(int iDspFlgs,double dS1,double dS2)
{
  OglDrawW(iDspFlgs,dS1,dS2);
}

c2dParPt::c2dParPt()
{
	pParent = NULL;
	next = NULL;
	before = NULL;
}

c2dParPt::c2dParPt(double x, double y)
{
	pParent = NULL;
	next = NULL;
	before = NULL;
	PP.x = x;
	PP.y = y;
}


c2dParPt::~c2dParPt()
{
	next = NULL;
	before = NULL;
	pParent = NULL;
}


//Segment class used for 2d mesh generation
cSeg::cSeg()
{
	pParent = NULL;
	pt[0] = NULL;
	pt[1] = NULL;
}

cSeg::cSeg(G_Object* pP)
{
	iColour = 4;
	iObjType = 14;
	next = NULL;
	before = NULL;
	pParent = pP;
	pt[0] = NULL;
	pt[1] = NULL;
}

cSeg::cSeg(G_Object* pP, double x1, double y1, double x2, double y2)
{
	iColour = 4;
	iObjType = 14;
	next = NULL;
	before = NULL;
	pParent = pP;
	pt[0] = new c2dParPt(x1,y1);
	pt[1] = new c2dParPt(x2,y2);
	CalcMids();
}

cSeg::~cSeg()
{
	next = NULL;
	before = NULL;
	pParent = NULL;
	pt[0] = NULL;
	pt[1] = NULL;
}

void cSeg::CalcMids()
{
	double x, y;
	MpT.x = 0.5*(pt[0]->PP.x + pt[1]->PP.x);
	MpT.y = 0.5*(pt[0]->PP.y + pt[1]->PP.y);
	x = pt[1]->PP.x - pt[0]->PP.x;
	y = pt[1]->PP.y - pt[0]->PP.y;
	dL=pow(x*x+y*y,0.5);
}

BOOL cSeg::HasCommonVert(c2dParPt* p1, c2dParPt* p2)
{
	BOOL bRet = FALSE;
	if ((pt[0] == p1) || (pt[1] == p1) ||
		(pt[0] == p2) || (pt[1] == p2))
	{
		bRet = TRUE;
	}
	return(bRet);
}

void cSeg::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	C3dVector vA, vB, vV;
	C3dMatrix mT;
	int i;
	double X, Y, Z;
	C3dVector p2;
	C3dVector Pts[7];
	if (pParent != NULL)
	{
		if ((pParent->iObjType == 15) 
		 || (pParent->iObjType == 16) 
		 || (pParent->iObjType == 17))
		{
			NSurf* pS = (NSurf*)pParent;
			if ((pt[0] != NULL) && (pt[1] != NULL))
			{
				vA = pS->GetPt(pt[0]->PP.x, pt[0]->PP.y);
				vB = pS->GetPt(pt[1]->PP.x, pt[1]->PP.y);
				p2 = vB;
				vV = vB - vA;
				vV.Normalize();
				mT = vV.GetTMat();
				//Create Arrow Head
				for (i = 0; i < 7; i++)
				{
					Pts[i].Set(AHead[i][0]-2.0, AHead[i][1], AHead[i][2]);
					Pts[i] = mT.Mult(Pts[i]);
					Pts[i] *= 0.2;
				}
				X = Pts[0].x;
				Y = Pts[0].y;
				Z = Pts[0].z;
				glLineWidth(5.0);
				glColor3fv(cols[iColour]);
				glBegin(GL_LINES);
				glVertex3f((float)vA.x, (float)vA.y, (float)vA.z);
				glVertex3f((float)vB.x, (float)vB.y, (float)vB.z);
				glEnd();
				glLineWidth(2.0);
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
				glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
				glEnd();
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
				glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
				glEnd();
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
				glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
				glEnd();
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
				glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
				glEnd();
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
				glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
				glEnd();
				glBegin(GL_POLYGON);
				glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
				glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
				glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
				glEnd();
			}
		}
	}
}

C2dVector cSeg::Get_Mid()
{
	return(MpT);
}

void cSeg::OglDraw(int iDspFlgs, double dS1, double dS2)
{
	OglDrawW(iDspFlgs, dS1, dS2);
}


c2dFront::c2dFront()
{
	pParent = NULL;
	dA=180;
	iPID=-1;
	fNodes=new cLinkedList();
}

c2dFront::~c2dFront()
{
	if (fNodes != NULL)
	{
		fNodes->DeleteAll();
		fNodes = NULL;
	}
}

Pt_Object* c2dFront::isSegBet(int pL, int pH)
{
	Pt_Object* oRet = NULL;
	Pt_Object* pN;
	pN = (Pt_Object*)fNodes->Head;
	while (pN != NULL)
	{
		if (pL == pN->iColour)
		{
			if (pN->next->iColour != pH)
			{
				oRet = (Pt_Object*)pN->next;
				break;
			}
		}
		pN = (Pt_Object*)pN->next;
	}
	return(oRet);
}


Pt_Object* c2dFront::GetNodeByGID(int iGID)
{
	//node colour has been used to identify the GPLY ID
	Pt_Object* oRet=NULL;
	Pt_Object* pN;
	pN = (Pt_Object*) fNodes->Head;
	while (pN != NULL)
	{
		if (iGID == pN->iColour)
		{
			oRet = (Pt_Object*) pN;
			break;
		}
		pN = (Pt_Object*)pN->next;
	}
	return(oRet);
}


void c2dFront::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
	fNodes->SetToScr(pModMat, pScrTran);
}

void c2dFront::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	fNodes->OglDrawW(iDspFlgs, dS1, dS2);
}

void c2dFront::OglDraw(int iDspFlgs, double dS1, double dS2)
{
	fNodes->OglDraw(iDspFlgs, dS1, dS2);
}






cFaceList::cFaceList()
{
Head=NULL;
pCur=NULL;
iNo=0;
}

cFaceList::~cFaceList()
{
cFace* pNext;
cFace* p;
p=Head;
while (p!=NULL)
{ 
  pNext=(cFace*) p->next;
  delete (p);
  p=pNext;
}
Head=NULL;
pCur=NULL;
}

cFace* cFaceList::IsIn(cFace* inFace)
{
cFace* pRet=NULL;
cFace* pNext;
pNext=Head;
while (pNext!=NULL)
{
  if (pNext->isSame(inFace)==TRUE)
  {
    pRet=pNext;
	break;
  }
  pNext=(cFace*) pNext->next;
}
return (pRet);
}

void cFaceList::Add(cFace* inFace)
{

cFace* ind;
ind=IsIn(inFace);
if (ind==NULL)
{
  if (Head==NULL)
  {
    inFace->before=NULL;
	inFace->next=NULL;
    Head=inFace;
    pCur=inFace;
	iNo++;
  }
  else
  {
    inFace->before=pCur;
    pCur->next=inFace;
    pCur=(cFace*) inFace;
    pCur->next=NULL;
    iNo++;
  }
}
else
{
  Remove(ind);
}
}

void cFaceList::Remove(cFace* inFace)
{

if (inFace!=NULL)
{

  if ((inFace->before!=NULL) && (inFace->next!=NULL))       //in middle with a befor and after
  {
    inFace->before->next=inFace->next;
    inFace->next->before=inFace->before;
	delete(inFace);
    iNo--;
  }
  else if (inFace->before==NULL)
  {
	Head=(cFace*) inFace->next;
    Head->before=NULL;
	delete(inFace);
    iNo--;
  }
  else if (inFace->next==NULL)
  {
	pCur=(cFace*) inFace->before;
    pCur->next=NULL;
	delete(inFace);
    iNo--;
  }
}
}

void cFaceList::OglDraw(int iDspFlgs,double dS1,double dS2)
{
cFace* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->OglDraw(iDspFlgs,dS1,dS2);
  pNext=(cFace*) pNext->next;
}
}

void cFaceList::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
cFace* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->OglDrawW(iDspFlgs,dS1,dS2);
  pNext=(cFace*) pNext->next;
}
}

cLinkList::cLinkList()
{
Head=NULL;
pCur=NULL;
iNo=0;
}

cLinkList::~cLinkList()
{
cLink* pNext;
cLink* p;
p=Head;
while (p!=NULL)
{ 
  pNext=(cLink*) p->next;
  delete (p);
  p=pNext;
}
Head=NULL;
pCur=NULL;
}

cLink* cLinkList::IsIn(cLink* inLink)
{
cLink* pRet=NULL;
cLink* pNext;
pNext=Head;
while (pNext!=NULL)
{
  if (pNext->isSame(inLink)==TRUE)
  {
    pRet=pNext;
	break;
  }
  pNext=(cLink*) pNext->next;
}
return (pRet);
}

void cLinkList::AddGp(int iN, cLink* inLink[])
{
  int i;
  for (i=0;i<iN;i++)
  {
      Add(inLink[i]);
  }
}

void cLinkList::Add(cLink* inLink)
{

cLink* ind;
ind=IsIn(inLink);
if (ind==NULL)
{
  if (Head==NULL)
  {
    inLink->before=NULL;
	  inLink->next=NULL;
    Head=inLink;
    pCur=inLink;
	  iNo++;
  }
  else
  {
    inLink->before=pCur;
    pCur->next=inLink;
    pCur=(cLink*) inLink;
    pCur->next=NULL;
    iNo++;
  }
}
else
{
  Remove(ind);
}
}



void cLinkList::Remove(cLink* inLink)
{
if (inLink!=NULL)
{
  if ((inLink->before!=NULL) && (inLink->next!=NULL))       //in middle with a befor and after
  {
    inLink->before->next=inLink->next;
    inLink->next->before=inLink->before;
	  delete(inLink);
    iNo--;
  }
  else if ((inLink->before==NULL) && (inLink->next==NULL))
  {
    Head->before=NULL;
    Head->next=NULL;
    Head=NULL;
	  delete(inLink);
    iNo--;
  }
  else if (inLink->before==NULL)
  {
	  Head=(cLink*) inLink->next;
    Head->before=NULL;
	  delete(inLink);
    iNo--;
  }
  else if (inLink->next==NULL)
  {
	  pCur=(cLink*) inLink->before;
    pCur->next=NULL;
	  delete(inLink);
    iNo--;
  }
}
}




void cLinkList::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
cLink* pNext;
pNext=Head;
while (pNext!=NULL)
{
  pNext->OglDrawW(iDspFlgs,dS1,dS2);
  pNext=(cLink*) pNext->next;
}
}

void cLinkList::OglDraw(int iDspFlgs,double dS1,double dS2)
{
OglDrawW(iDspFlgs,dS1,dS2);
}

//****************************************************************************
//26/09/2016
//Link class simples way of storing lists of lines
//used for symbols
//****************************************************************************

Link::Link()
{
pNext=NULL;
}

Link::Link(double x1, double y1, double z1,
	       double x2, double y2, double z2)
{
pNext=NULL;
p1 =new CvPt_Object();
p2 =new CvPt_Object();

p1->Pt_Point->x=x1; p1->Pt_Point->y=y1; p1->Pt_Point->z=z1;
p2->Pt_Point->x=x2; p2->Pt_Point->y=y2; p2->Pt_Point->z=z2;
}

Link::~Link()
{
if (p1!=NULL)
  delete(p1);
if (p2!=NULL)
  delete(p2);
pNext=NULL;
}



IMPLEMENT_DYNAMIC( Line_Object, CObject )


Line_Object::~Line_Object()
{
delete (pVertex1);
delete (pVertex2);
}



void Line_Object::Create(C3dVector* pInVertex1,C3dVector* pInVertex2, int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
int i=0;
iObjType = 2;
iLabel = iLab;
C3dVector InVect;
InVect.x = pInVertex1->x;
InVect.y = pInVertex1->y;
InVect.z = pInVertex1->z;
pVertex1 = new CvPt_Object;
pVertex1->Create(InVect,0,0,0,0,11,this);
pVertex1->SetVisable(0);
InVect.x = pInVertex2->x;
InVect.y = pInVertex2->y;
InVect.z = pInVertex2->z;
pVertex2 = new CvPt_Object;
pVertex2->Create(InVect,0,0,0,0,11,this);
pVertex2->SetVisable(0);
pParent=Parrent;
}

void Line_Object::SetP1(double dx,double dy,double dz)
{
pVertex1->Pt_Point->x=dx;
pVertex1->Pt_Point->y=dy;
pVertex1->Pt_Point->z=dz;
}

void Line_Object::SetP2(double dx,double dy,double dz)
{
pVertex2->Pt_Point->x=dx;
pVertex2->Pt_Point->y=dy;
pVertex2->Pt_Point->z=dz;
}

C3dVector Line_Object::MinPt(C3dVector inPt)
{
C3dVector vRet;
C3dVector v1;
C3dVector v2;
C3dVector P1;
C3dVector P2;
double dm;
double dDot;
P1.Set(pVertex1->Pt_Point->x,
       pVertex1->Pt_Point->y,
       pVertex1->Pt_Point->z);
P2.Set(pVertex2->Pt_Point->x,
       pVertex2->Pt_Point->y,
       pVertex2->Pt_Point->z);
v1=P2;
v1-=P1;
v2=inPt;
v2-=P1;
dm=v2.Mag();
v1.Normalize();
v2.Normalize();
dDot=v2.Dot(v1);
dm=dm*dDot;
vRet=P1+v1*dm;
return vRet;
}




void Line_Object::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    pVertex1->Serialize(ar,iV);
    pVertex2->Serialize(ar,iV); 
	}
	else
	{
    G_Object::Serialize(ar,iV);
    pVertex1= new CvPt_Object;
    pVertex2= new CvPt_Object;
    pVertex1->Serialize(ar,iV);
    pVertex2->Serialize(ar,iV);
	}
}

C3dVector Line_Object::Get_Centroid()
{
return (GetPt(0.5));
}

G_Object* Line_Object::Copy(G_Object* parrent)
{
Line_Object* rLn = new Line_Object;
rLn->Drawn = Drawn;
rLn->Selectable  = Selectable; 
rLn->Visable  = Visable;
rLn->iColour = iColour;
rLn->iObjType = iObjType;
rLn->iLabel = iLabel;
rLn->pVertex1 = (CvPt_Object *) pVertex1->Copy(rLn);
rLn->pVertex2 = (CvPt_Object *) pVertex2->Copy(rLn);
pParent = parrent;
return  (rLn);
}

void Line_Object::Info()
{
  char S1[80];
  CString OutT;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  OutT+=S1;
  outtext1("LINE OBJECT");
  outtext1(OutT); 
  outtext1("    ****");
}

CvPt_Object* Line_Object::GetTEnd()
{
if (dUsel < 0.5)
  {
  return (pVertex1);
  }
else
  {
  return (pVertex2);
  }
}




void Line_Object::Draw(CDC* pDC,int iDrawmode)
{
pDC->MoveTo((int) pVertex1->DSP_Point->x,(int) pVertex1->DSP_Point->y);
pDC->LineTo((int) pVertex2->DSP_Point->x,(int) pVertex2->DSP_Point->y);
}

void Line_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
glColor3fv(cols[GetCol()]);
glBegin(GL_LINES);
glVertex3f((float) pVertex1->Pt_Point->x,(float) pVertex1->Pt_Point->y,(float) pVertex1->Pt_Point->z);
glVertex3f((float) pVertex2->Pt_Point->x,(float) pVertex2->Pt_Point->y,(float) pVertex2->Pt_Point->z);
glEnd();
}

void Line_Object::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
glColor3fv(cols[GetCol()]);
glBegin(GL_LINES);
glVertex3f((float) pVertex1->Pt_Point->x,(float) pVertex1->Pt_Point->y,(float) pVertex1->Pt_Point->z);
glVertex3f((float) pVertex2->Pt_Point->x,(float) pVertex2->Pt_Point->y,(float) pVertex2->Pt_Point->z);
glEnd();
}

void Line_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
pVertex1->SetToScr(pModMat, pScrTran);
pVertex2->SetToScr(pModMat, pScrTran);
}

void Line_Object::HighLight(CDC* pDC)
{
pDC->MoveTo((int) pVertex1->DSP_Point->x,(int) pVertex1->DSP_Point->y);
pDC->LineTo((int) pVertex2->DSP_Point->x,(int) pVertex2->DSP_Point->y);
}

void Line_Object::Transform(C3dMatrix TMat)
{
pVertex1->Transform(TMat);
pVertex2->Transform(TMat);
}

G_ObjectD Line_Object::SelDist(CPoint InPT,Filter FIL)
{
CPoint cPt;
G_ObjectD Ret;
double MinDist = 100000000000000.0;
double dDist = 0;
double fred;
int iCnt1;

  for (iCnt1 = 5; iCnt1 < 16; iCnt1++)
     {
	 fred = iCnt1*0.05;
     cPt.x = (long) (fred*(pVertex2->DSP_Point->x-pVertex1->DSP_Point->x)+pVertex1->DSP_Point->x)-InPT.x;
     cPt.y = (long) (fred*(pVertex2->DSP_Point->y-pVertex1->DSP_Point->y)+pVertex1->DSP_Point->y)-InPT.y;
     dDist = cPt.x*cPt.x+cPt.y*cPt.y;

	 if (dDist < MinDist)
	   {
       MinDist = dDist;
	   dUsel = fred;
	   }
     //pDC->Ellipse(ThePoint.DSP_Point->x+5,ThePoint.DSP_Point->y+5,ThePoint.DSP_Point->x-5,ThePoint.DSP_Point->y-5);
     }
Ret.Dist=MinDist;
Ret.pObj=this;
return (Ret);
}

C3dVector Line_Object::GetPt(double dU)
{
C3dVector vRet;
double dX,dY,dZ;

dX = (pVertex2->Pt_Point->x-pVertex1->Pt_Point->x)*dU;
dY = (pVertex2->Pt_Point->y-pVertex1->Pt_Point->y)*dU;
dZ = (pVertex2->Pt_Point->z-pVertex1->Pt_Point->z)*dU;
vRet.x = pVertex1->Pt_Point->x + dX;
vRet.y = pVertex1->Pt_Point->y + dY;
vRet.z = pVertex1->Pt_Point->z + dZ;
return (vRet);
}



C3dVector Line_Object::GetDir(double w)
{
C3dVector DirA;
C3dVector DirB;
DirA = this->GetPt(0.0);
DirB = this->GetPt(1.0);
DirB-=DirA;
DirB.Normalize();
return (DirB);
}


double Line_Object::MinDist(C3dVector vInPt)
{
C3dVector vRet;
double dX,dY,dZ,dU;
int iCnt1;
double MinDist = 100000000000000.0;
double dDist = 0;

for (iCnt1 = 1; iCnt1 < 1000; iCnt1++)
{
dU = iCnt1*0.001;
dX = (pVertex2->Pt_Point->x-pVertex1->Pt_Point->x)*dU;
dY = (pVertex2->Pt_Point->y-pVertex1->Pt_Point->y)*dU;
dZ = (pVertex2->Pt_Point->z-pVertex1->Pt_Point->z)*dU;
vRet.x = pVertex1->Pt_Point->x + dX;
vRet.y = pVertex1->Pt_Point->y + dY;
vRet.z = pVertex1->Pt_Point->z + dZ;
dDist = pow((pow((vRet.x - vInPt.x),2)+pow((vRet.y - vInPt.y),2)+pow((vRet.z - vInPt.z),2)),0.5);
if (dDist < MinDist)
  {
  MinDist = dDist;
  }
}
return (MinDist);
}





double Line_Object::getLen()
{
double dRet=0;
int i;
int iIncs=1;
double dStp;
double w;
C3dVector p1;
C3dVector p2;

dStp = 1/iIncs;
w=0;
p1=GetPt(w);
for (i=0;i<iIncs;i++)
{
  w+=dStp;
  p2=GetPt(w);
  dRet+=p2.Dist(p1);
  p1=p2;
}
return (dRet);
}



IMPLEMENT_DYNAMIC(Curve, CObject )

void Curve::Draw(CDC* pDC,int iDrawmode)
{
  ContrPolyW::Draw(pDC,iDrawmode,0,1);
}

void Curve::OglDraw(int iDspFlgs,double dS1,double dS2)
{
ContrPolyW::OglDraw(0.0,1.0);
}

void Curve::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
ContrPolyW::OglDrawW(0.0,1.0);
}


void Curve::HighLight(CDC* pDC)
{
  ContrPolyW::Draw(pDC,4,0,1);
}

IMPLEMENT_DYNAMIC( ContrPolyW, CObject )


ContrPolyW::ContrPolyW()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iNoVerts = 0;
iLabel = -1;
iObjType = 6;

}


    // CvPt_Object* pVertex[20];
void ContrPolyW::Create(int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iNoVerts = 0;
iLabel = iLab;
iObjType = 6;
pParent=Parrent;
}

C3dVector ContrPolyW::Get_Centroid()
{
return (GetPt(0.5));
}

void ContrPolyW::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar << iNoVerts;
	  for (i=0;i<iNoVerts;i++)
    {
      ar << pCVertex[i]->xw;
      ar << pCVertex[i]->yw;
      ar << pCVertex[i]->zw;
      ar << pCVertex[i]->w;
    }
	}
	else
	{
    G_Object::Serialize(ar,iV);
    ar >> iNoVerts;
	  for (i=0;i<iNoVerts;i++)
    {
      pCVertex [i] = new C4dVector();
      ar >> pCVertex[i]->xw;
      ar >> pCVertex[i]->yw;
      ar >> pCVertex[i]->zw;
      ar >> pCVertex[i]->w;
    }
	}
}

G_Object* ContrPolyW::Copy(G_Object* parrent)
{
int i;
ContrPolyW* cPoly = new ContrPolyW;
cPoly->Drawn = Drawn;
cPoly->Selectable  = Selectable; 
cPoly->Visable  = Visable;
cPoly->iColour = iColour;
cPoly->iObjType = iObjType;
cPoly->iLabel = iLabel;
cPoly->iNoVerts = iNoVerts;
for (i=0;i<iNoVerts;i++)
{
  cPoly->pCVertex [i] = new C4dVector();
  cPoly->pCVertex [i]->xw = pCVertex[i]->xw;
  cPoly->pCVertex [i]->yw = pCVertex[i]->yw;
  cPoly->pCVertex [i]->zw = pCVertex[i]->zw;
  cPoly->pCVertex [i]->w = pCVertex[i]->w;
}
cPoly->pParent = parrent;
return (cPoly);
}

void ContrPolyW::AddVert(C3dVector* pInVertex1, double dWght)
{
if (dWght == 0)
  {
  dWght = 1;
  }
if (iNoVerts < 20)
  {
  pCVertex [iNoVerts] = new C4dVector();
  pCVertex [iNoVerts]->xw = pInVertex1->x*dWght;
  pCVertex [iNoVerts]->yw = pInVertex1->y*dWght;
  pCVertex [iNoVerts]->zw = pInVertex1->z*dWght;
  pCVertex [iNoVerts]->w = dWght;
  iNoVerts++;
  }
}

void ContrPolyW::CPolyRot(double dAng)
{
int i;
C3dVector vT1,a,b;

for (i = 0; i < iNoVerts; i++)
   {
   vT1.x = pCVertex [i]->xw;
   vT1.y = pCVertex [i]->yw;
   vT1.z = pCVertex [i]->zw;
   vT1.Rotate(a,b,dAng);
   pCVertex [i]->xw = vT1.x;
   pCVertex [i]->yw = vT1.y;
   pCVertex [i]->zw = vT1.z;
   }
}

void ContrPolyW::CWeight (double dW)
{
int i;
C3dVector vT1,a,b;

for (i = 0; i < iNoVerts; i++)
   {
   vT1.x = pCVertex [i]->xw / pCVertex [i]->w;
   vT1.y = pCVertex [i]->yw / pCVertex [i]->w;
   vT1.z = pCVertex [i]->zw / pCVertex [i]->w;
   pCVertex [i]->xw = vT1.x * dW;
   pCVertex [i]->yw = vT1.y * dW;
   pCVertex [i]->zw = vT1.z * dW;
   pCVertex [i]->w = dW;

   }

}


void ContrPolyW::AddVertW(C4dVector pInVertex1)
{
if (iNoVerts < 20)
  {
  pCVertex [iNoVerts] = new C4dVector();
  pCVertex [iNoVerts]->xw = pInVertex1.xw;
  pCVertex [iNoVerts]->yw = pInVertex1.yw;
  pCVertex [iNoVerts]->zw = pInVertex1.zw;
  pCVertex [iNoVerts]->w = pInVertex1.w;
  iNoVerts++;
  }
}
void ContrPolyW::Set2(ContrPolyW* vS4)
{
int i;

for  (i = 0; i < vS4->iNoVerts; i++)
  {
  AddVert(&vS4->pCVertex[i]->GetPoint(),vS4->pCVertex[i]->GetWght());
  }
}

void ContrPolyW::Translate (C3dVector vTVect)
{
int i;

for (i = 0; i < iNoVerts; i++)
   {
   pCVertex [i]->Translate(vTVect);
   }
}


void ContrPolyW::Clear()
{
int i;
for (i = 0; i<iNoVerts; i++)
   {
   delete(pCVertex[i]);
   }
iNoVerts = 0;
}


int ContrPolyW::GetOrder()
{

return (iNoVerts - 2);
}


C4dVector ContrPolyW::deCastelJau1(double u)
{

C4dVector Q[20]; 
int i;
int k;

for (i=0;i<=(iNoVerts-1);i++)
  {
  Q[i].xw= pCVertex[i]->xw;
  Q[i].yw= pCVertex[i]->yw;
  Q[i].zw= pCVertex[i]->zw;
  Q[i].w= pCVertex[i]->w;
  }
for (k=1;k<=(iNoVerts-1);k++)
  {
  for (i=0;i<=((iNoVerts-1)-k);i++)
    {
    Q[i] =Q[i]*(1.0-u)+Q[i+1]*u;
	//Q[i].x=(1.0-u)*Q[i].x+u*Q[i+1].x;
	//Q[i].y=(1.0-u)*Q[i].y+u*Q[i+1].y;
	//Q[i].z=(1.0-u)*Q[i].z+u*Q[i+1].z;
    }
  }

C4dVector vRetVect;
vRetVect.xw = Q[0].xw;
vRetVect.yw = Q[0].yw;
vRetVect.zw = Q[0].zw;
vRetVect.w = Q[0].w;

return (vRetVect);
}

  
void ContrPolyW::Draw(CDC* pDC,int iDrawmode,
                      double sw,double ew)
{
const double dInc = 0.01;
double w;

C3dVector pt;
pt=GetPt(sw);
Pt_Object* ThePoint = new Pt_Object;
ThePoint->Create(pt,1,0,0,11,0,0,NULL);
ThePoint->Pt_Point->x = pt.x; 
ThePoint->Pt_Point->y = pt.y;
ThePoint->Pt_Point->z = pt.z;
ThePoint->SetToScr(pModZ,pScrZ);
pDC->MoveTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);
w=sw;

do
{
  w=w+dInc;
  if (w>ew)
  {
    w = ew;
  }
  pt=GetPt(w);
  ThePoint->Pt_Point->x = pt.x; 
  ThePoint->Pt_Point->y = pt.y;
  ThePoint->Pt_Point->z = pt.z;
  ThePoint->SetToScr(pModZ,pScrZ);
  pDC->LineTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);  
}
while (w<ew);
delete(ThePoint);
}


void ContrPolyW::OglDraw(double sw,double ew)
{
GLfloat* ctrlpts;
int iu;
int icu;
int iInd = 0;
iu = iNoVerts;


ctrlpts = (GLfloat*) malloc(iu*4*sizeof(GLfloat));
for (icu = 0;icu < iu;icu++)
{
  ctrlpts[iInd]   = (float) pCVertex[icu]->xw;
  ctrlpts[iInd+1] = (float) pCVertex[icu]->yw;
  ctrlpts[iInd+2] = (float) pCVertex[icu]->zw;
  ctrlpts[iInd+3] = (float) pCVertex[icu]->w;
  iInd = iInd+4;
}  

glEnable(GL_MAP1_VERTEX_4);
glMap1f(GL_MAP1_VERTEX_4,0.0f,1.0f,4,iu,ctrlpts);
//glEnable(GL_MAP1_VERTEX_4);
glMapGrid1f(20,(float) sw,(float) ew);
glEvalMesh1(GL_LINE,0,20);
//glFlush();
free(ctrlpts);
}

void ContrPolyW::OglDrawW(double sw,double ew)
{
OglDraw(sw,ew);
}


void ContrPolyW::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
//int iCnt;
//for (iCnt=0;iCnt<iNoVerts;iCnt++)
//    {
//    pVertex[iCnt]->SetToScr(pScrTran);
//    }
pModZ = pModMat;
pScrZ = pScrTran;
}

void ContrPolyW::Transform(C3dMatrix TMat)
{
int i;
for (i=0;i<iNoVerts;i++)
{
  pCVertex[i]->Transform(TMat);
}
}


void ContrPolyW::HighLight(CDC* pDC,double sw,double ew)
{
Draw(pDC,4,sw,ew);
}

G_ObjectD ContrPolyW::SelDist(CPoint InPT,Filter FIL)
{
int iCnt1;
G_ObjectD Ret;
double MinDist = 1e36;
double dDist = 0;
double fred;
C3dVector vDrawPt;
C4dVector vWpt;
Pt_Object ThePoint;
ThePoint.Create(vDrawPt,0,0,0,11,0,0,NULL);
for (iCnt1 = 1; iCnt1 < 21; iCnt1++)
{
	fred = iCnt1*0.05;
  //vDrawPt = GetParaPoint(fred);	 
	vWpt = deCastelJau1 (fred);
	ThePoint.Pt_Point->x = vWpt.xw/vWpt.w;
    ThePoint.Pt_Point->y = vWpt.yw/vWpt.w;
    ThePoint.Pt_Point->z = vWpt.zw/vWpt.w;
	ThePoint.SetToScr(pModZ,pScrZ);
    dDist = pow((ThePoint.DSP_Point->x-InPT.x),2)+pow((ThePoint.DSP_Point->y-InPT.y),2);
	if (dDist < MinDist)
	{
      MinDist = dDist;
	}
  //pDC->Ellipse(ThePoint.DSP_Point->x+5,ThePoint.DSP_Point->y+5,ThePoint.DSP_Point->x-5,ThePoint.DSP_Point->y-5);
}
ThePoint.Clear();
Ret.Dist=MinDist;
Ret.pObj=this;
return (Ret);
}


C3dVector ContrPolyW::GetPt(double w)
{
C3dVector vR;
C4dVector vWpt;
if (iNoVerts > 2)
  {
	 vWpt = deCastelJau1 (w);
	 vR.x = vWpt.xw/vWpt.w;
     vR.y = vWpt.yw/vWpt.w;
     vR.z = vWpt.zw/vWpt.w;     
  }
return (vR);
}


C3dVector ContrPolyW::GetDir(double w)
{
C3dVector DirA;
C3dVector DirB;
if (w < 0.99999)
{
  DirA = this->GetPt(w);
  DirB = this->GetPt(w+0.00001);
}
else
{
  DirA = this->GetPt(w-0.00001);
  DirB = this->GetPt(w);
}
DirB-=DirA;
DirB.Normalize();
return (DirB);
}











IMPLEMENT_DYNAMIC(Circ1, CObject )
//	 int iNoVerts;
  //   CvPt_Object* pVertex[8]; //control polygon
//	 double dRadius;
//	 C3dMatrix* pScrZ;


void Circ1::Create(C3dVector vN,C3dVector vC,double dRad,int iLab,G_Object* Parrent)
{
dS=0;
dE=1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iObjType = 5;
dRadius = dRad;
C3dVector Pt;
pParent=Parrent;
vNorm=new C3dVector(0,0,1);
vCent=new C3dVector(0,0,0);

pSegs[0] = new ContrPolyW();
pSegs[0]->Create(iLab,this);
Pt.x=dRadius;Pt.y=0;Pt.z=0;
pSegs[0]->AddVert(&Pt,1);
Pt.x=dRadius;Pt.y=dRadius;Pt.z=0;
pSegs[0]->AddVert(&Pt,1);
Pt.x=0;Pt.y=dRadius;Pt.z=0;
pSegs[0]->AddVert(&Pt,2);

pSegs[1] = new ContrPolyW();
pSegs[1]->Create(iLab,this);
pSegs[1]->AddVert(&Pt,1);
Pt.x=-dRadius;Pt.y=dRadius;Pt.z=0;
pSegs[1]->AddVert(&Pt,1);
Pt.x=-dRadius;Pt.y=0;Pt.z=0;
pSegs[1]->AddVert(&Pt,2);

pSegs[2] = new ContrPolyW();
pSegs[2]->Create(iLab,this);
pSegs[2]->AddVert(&Pt,1);
Pt.x=-dRadius;Pt.y=-dRadius;Pt.z=0;
pSegs[2]->AddVert(&Pt,1);
Pt.x=0;Pt.y=-dRadius;Pt.z=0;
pSegs[2]->AddVert(&Pt,2);

pSegs[3] = new ContrPolyW();
pSegs[3]->Create(iLab,this);
pSegs[3]->AddVert(&Pt,1);
Pt.x=dRadius;Pt.y=-dRadius;Pt.z=0;
pSegs[3]->AddVert(&Pt,1);
Pt.x=dRadius;Pt.y=0;Pt.z=0;
pSegs[3]->AddVert(&Pt,2);
C3dMatrix RMat;
RMat=RMat.CalcTran(vN);
C3dMatrix TMat;
this->Transform(RMat);
TMat.Translate(vC.x,vC.y,vC.z);
this->Transform(TMat);
}


void Circ1::Reverse()
{
C4dVector* p[12];
vNorm->x = -vNorm->x;
vNorm->y = -vNorm->y;
vNorm->z = -vNorm->z;
p[0] = pSegs[0]->pCVertex[0];
p[1] = pSegs[0]->pCVertex[1];
p[2] = pSegs[0]->pCVertex[2];

p[3] = pSegs[1]->pCVertex[0];
p[4] = pSegs[1]->pCVertex[1];
p[5] = pSegs[1]->pCVertex[2];

p[6] = pSegs[2]->pCVertex[0];
p[7] = pSegs[2]->pCVertex[1];
p[8] = pSegs[2]->pCVertex[2];

p[9] = pSegs[3]->pCVertex[0];
p[10] = pSegs[3]->pCVertex[1];
p[11] = pSegs[3]->pCVertex[2];

pSegs[0]->pCVertex[0] = p[11];
pSegs[0]->pCVertex[1] = p[10];
pSegs[0]->pCVertex[2] = p[9];

pSegs[1]->pCVertex[0] = p[8];
pSegs[1]->pCVertex[1] = p[7];
pSegs[1]->pCVertex[2] = p[6];

pSegs[2]->pCVertex[0] = p[5];
pSegs[2]->pCVertex[1] = p[4];
pSegs[2]->pCVertex[2] = p[3];

pSegs[3]->pCVertex[0] = p[2];
pSegs[3]->pCVertex[1] = p[1];
pSegs[3]->pCVertex[2] = p[0];


}


void Circ1::Info()
{
  char S1[80];
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("CIRCLE OBJECT");
  outtext1(S1); 
  sprintf_s(S1,"%s%f","Radius : ",dRadius);
  outtext1(S1); 
  sprintf_s(S1,"%s%f%s%f%s%f","Centre : ",vCent->x,",",vCent->y,",",vCent->z);
  outtext1(S1); 
  sprintf_s(S1,"%s%f%s%f%s%f","Normal : ",vNorm->x,",",vNorm->y,",",vNorm->z);
  outtext1(S1);
  outtext1("    ****");
}



void Circ1::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar<<dRadius;
	ar<<dS;
	ar<<dE;
    ar << vNorm->x;
    ar << vNorm->y;
    ar << vNorm->z;
    ar << vCent->x;
    ar << vCent->y;
    ar << vCent->z;
    pSegs[0]->Serialize(ar,iV);
    pSegs[1]->Serialize(ar,iV);
    pSegs[2]->Serialize(ar,iV);
    pSegs[3]->Serialize(ar,iV);
	}
	else
	{
    G_Object::Serialize(ar,iV);
    ar>>dRadius;
	ar>>dS;
	ar>>dE;
    vNorm=new C3dVector(0,0,1);
    vCent=new C3dVector(0,0,0);
    ar >> vNorm->x;
    ar >> vNorm->y;
    ar >> vNorm->z;
    ar >> vCent->x;
    ar >> vCent->y;
    ar >> vCent->z;
    pSegs[0] = new ContrPolyW();
    pSegs[1] = new ContrPolyW();
    pSegs[2] = new ContrPolyW();
    pSegs[3] = new ContrPolyW();
    pSegs[0]->Serialize(ar,iV);
    pSegs[1]->Serialize(ar,iV);
    pSegs[2]->Serialize(ar,iV);
    pSegs[3]->Serialize(ar,iV);
	}
}

G_Object* Circ1::Copy(G_Object* Parrent)
{
Circ1* rCir = new Circ1;
rCir->dS=dS;
rCir->dE=dE;
rCir->Drawn = Drawn;
rCir->Selectable  = Selectable; 
rCir->Visable  = Visable;
rCir->iColour = iColour;
rCir->iObjType = iObjType;
rCir->iLabel = iLabel;
rCir-> pModZ = pModZ;
rCir-> pScrZ = pScrZ;
rCir->dRadius=dRadius;
rCir->vNorm=new C3dVector(0,0,1);
rCir->vCent=new C3dVector(0,0,0);
rCir->vNorm->x=vNorm->x;
rCir->vNorm->y=vNorm->y;
rCir->vNorm->z=vNorm->z;
rCir->vCent->x=vCent->x;
rCir->vCent->y=vCent->y;
rCir->vCent->z=vCent->z;
rCir->pSegs[0] = new ContrPolyW();
rCir->pSegs[1] = new ContrPolyW();
rCir->pSegs[2] = new ContrPolyW();
rCir->pSegs[3] = new ContrPolyW();
rCir->pSegs[0]=(ContrPolyW *) pSegs[0]->Copy(this);
rCir->pSegs[1]=(ContrPolyW *) pSegs[1]->Copy(this);
rCir->pSegs[2]=(ContrPolyW *) pSegs[2]->Copy(this);
rCir->pSegs[3]=(ContrPolyW *) pSegs[3]->Copy(this);
return (rCir);
}

double Circ1::Bernstein(double dU,int ii,int inn)
{
double dCoeA;
double dCoeB;
double dRet;
dCoeA = Factorial(inn)/(Factorial(ii)*Factorial(inn-ii));
dCoeB = 1-dU;
dRet = dCoeA*pow(dU,ii)*pow(dCoeB,inn-ii);
return dRet;
}


void Circ1::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
pModZ = pModMat;
pScrZ = pScrTran;

pSegs[0]->SetToScr(pModMat,pScrTran);
pSegs[1]->SetToScr(pModMat,pScrTran);
pSegs[2]->SetToScr(pModMat,pScrTran);
pSegs[3]->SetToScr(pModMat,pScrTran);

}

void Circ1::Transform(C3dMatrix TMat)
{

pSegs[0]->Transform(TMat);
pSegs[1]->Transform(TMat);
pSegs[2]->Transform(TMat);
pSegs[3]->Transform(TMat);
*vCent=TMat.Mult(*vCent);
TMat.Translate2(0,0,0);
*vNorm=TMat.Mult(*vNorm);
}

C3dVector Circ1::Get_Centroid()
{
C3dVector vT;
vT.Set(vCent->x,vCent->y,vCent->z);
return (vT);
}


void Circ1::Draw(CDC* pDC,int iDrawmode)
{
double dLS;
double dLE;

if (dS<=0.25)
{
  dLS=4*(dS);
  dLE=4*(dE);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[0]->Draw(pDC,iDrawmode,dLS,dLE);
}
if ((dS<=0.5) && (dE>0.25))
{
  dLS=4*(dS-0.25);
  dLE=4*(dE-0.25);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[1]->Draw(pDC,iDrawmode,dLS,dLE);
}
if ((dS<=0.75) && (dE>0.5))
{
  dLS=4*(dS-0.5);
  dLE=4*(dE-0.5);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[2]->Draw(pDC,iDrawmode,dLS,dLE);
}
if ((dS<=1) && (dE>0.75))
{
  dLS=4*(dS-0.75);
  dLE=4*(dE-0.75);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[3]->Draw(pDC,iDrawmode,dLS,dLE);
}

}

void Circ1::OglDraw(int iDspFlgs)
{
double dLS;
double dLE;

if (dS<=0.25)
{
  dLS=4*(dS);
  dLE=4*(dE);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[0]->OglDraw(dLS,dLE);
}
if ((dS<=0.5) && (dE>0.25))
{
  dLS=4*(dS-0.25);
  dLE=4*(dE-0.25);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[1]->OglDraw(dLS,dLE);
}
if ((dS<=0.75) && (dE>0.5))
{
  dLS=4*(dS-0.5);
  dLE=4*(dE-0.5);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[2]->OglDraw(dLS,dLE);
}
if ((dS<=1) && (dE>0.75))
{
  dLS=4*(dS-0.75);
  dLE=4*(dE-0.75);
  if (dLS<0){dLS=0;}
  if (dLE>1){dLE=1;}
  pSegs[3]->OglDraw(dLS,dLE);
}
}

void Circ1::OglDrawW(int iDspFlgs)
{
  OglDraw(iDspFlgs);
}



void Circ1::HighLight(CDC* pDC)
{
Draw(pDC,4);
}

G_ObjectD Circ1::SelDist(CPoint InPT,Filter FIL)
{
int iC;
double dDist = 1e36;
G_ObjectD Ret;
for (iC = 0;iC < 4;iC++)
{
if (pSegs[iC]->SelDist(InPT,FIL).Dist < dDist)
  {
  Ret = pSegs[iC]->SelDist(InPT,FIL);
  dDist=Ret.Dist;
  }
}
Ret.pObj=this;
return (Ret);
}

C3dVector Circ1::MinPt(C3dVector inPt)
{
C3dVector vRet;
const double dTol = 0.000000001;
const double dIncs = 36;
int i;
double dStp = 1/dIncs;
double dlStp;
double dW=0;
double dMinW=0;
C3dVector cPt;
C3dVector tPt;
C3dVector cNextPt;
double dWScl;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;
double dM;
double dDot;
cPt=GetPt(dW);
tPt=GetPt(dW+dStp);
dWScl = cPt.Dist(tPt);
for(i=0;i<dIncs;i++)
{
  dW=i*dStp;
  cPt=GetPt(dW);
  Dist=cPt.Dist(inPt);
  if (Dist<minDist)
  {
    minDist=Dist;
    dMinW=dW;
  }
}
//start of iteration
dW=dMinW;
dlStp=dStp;
i=0;
do
{
  cPt=GetPt(dW);
  cNextPt=GetPt(dW+0.000001);
  v1=cNextPt;
  v1-=cPt;
  v2=inPt;
  v2-=cPt;
  dM=v2.Mag();
  v1.Normalize();
  v2.Normalize();
  dDot=v2.Dot(v1);
  dlStp=(dM*dDot/dWScl)*1*dStp;
  dW=dW+dlStp;
  i++;
}
while ((pow((dlStp*dlStp),0.5)>dTol) & (i<10000));

vRet=GetPt(dW);
return (vRet);
}

double Circ1::MinWPt(C3dVector inPt)
{
C3dVector vRet;
const double dIncs = 36;
int i;
double dStp = 1/dIncs;
double dlStp;
double dW=0;
double dMinW=0;
C3dVector cPt;
C3dVector tPt;
C3dVector cNextPt;
double dWScl;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;
double dM;
double dDot;
cPt=GetPt(dW);
tPt=GetPt(dW+dStp);
dWScl = cPt.Dist(tPt);
for(i=0;i<dIncs;i++)
{
  dW=i*dStp;
  cPt=GetPt(dW);
  Dist=cPt.Dist(inPt);
  if (Dist<minDist)
  {
    minDist=Dist;
    dMinW=dW;
  }
}
//start of iteration
dW=dMinW;
dlStp=dStp;
i=0;
do
{
  cPt=GetPt(dW);
  cNextPt=GetPt(dW+0.000001);
  v1=cNextPt;
  v1-=cPt;
  v2=inPt;
  v2-=cPt;
  dM=v2.Mag();
  v1.Normalize();
  v2.Normalize();
  dDot=v2.Dot(v1);
  dlStp=(dM*dDot/dWScl)*1*dStp;
  dW=dW+dlStp;
  i++;
}
while ((pow((dlStp*dlStp),0.5)>dTol) & (i<10000));
return (dW);
}

C3dVector Circ1::GetPt(double w)
{
C3dVector vR;
vR.x=0;vR.y=0,vR.z=0;;
int TheSeg = -1;
double aw;
if (w>1)
{
	w=w-1;
}
if ((w>=0) && (w<0.25))
{
  TheSeg=0;
  aw =w*4;
}
else if ((w>=0.25) && (w<0.5))
{
  TheSeg=1; 
  aw =(w-0.25)*4;
}
else if ((w>=0.5) && (w<0.75))
{
  TheSeg=2; 
  aw =(w-0.5)*4;
}
else if ((w>=0.75) && (w<=1))
{
  TheSeg=3; 
  aw =(w-0.75)*4;
}
if (TheSeg!=-1)
{
vR = pSegs[TheSeg]->GetPt(aw);
}
return (vR);
}


C3dVector Circ1::GetDir(double w)
{
C3dVector vR;
vR.x=0;vR.y=0,vR.z=0;;
int TheSeg = -1;
double aw;
if (w>1)
{w-=1;}
if ((w>=0) && (w<0.25))
{
  TheSeg=0;
  aw =w*4;
}
else if ((w>=0.25) && (w<0.5))
{
  TheSeg=1; 
  aw =(w-0.25)*4;
}
else if ((w>=0.5) && (w<0.75))
{
  TheSeg=2; 
  aw =(w-0.5)*4;
}
else if ((w>=0.75) && (w<=1))
{
  TheSeg=3; 
  aw =(w-0.75)*4;
}
if (TheSeg!=-1)
{
vR = pSegs[TheSeg]->GetDir(aw);
}
return (vR);
}



//Extruded Surface
IMPLEMENT_DYNAMIC( Surf_Ex1, CObject )

//ContrPolyW* pWCurve[2];
//C3dVector vTranVec;
//C3dMatrix* pScrZ;

void Surf_Ex1::Create(ContrPolyW* pWCurveIn, C3dVector vTranVecIn, int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iObjType = 10;
pParent=Parrent;
vTranVec = vTranVecIn;

pWCurve[0] = pWCurveIn;
pWCurve[1] = new ContrPolyW;
pWCurve[1]->Create(1,this);
pWCurve[1]->Set2(pWCurve[0]);
pWCurve[1]->Translate(vTranVecIn);


pWCurve[2] = new ContrPolyW;
pWCurve[2]->Create(1,this);
pWCurve[2]->AddVert(&pWCurve[0]->pCVertex[0]->GetPoint(),1);
pWCurve[2]->AddVert(&pWCurve[1]->pCVertex[0]->GetPoint(),1);


pWCurve[3] = new ContrPolyW;
pWCurve[3]->Create(1,this);
pWCurve[3]->AddVert(&pWCurve[0]->pCVertex[pWCurve[0]->iNoVerts-1]->GetPoint(),1);
pWCurve[3]->AddVert(&pWCurve[1]->pCVertex[pWCurve[0]->iNoVerts-1]->GetPoint(),1);

}

void Surf_Ex1::Draw(CClientDC* pDC,int iDrawmode)
{
pWCurve[0]->SetToScr(pModZ,pScrZ);
pWCurve[1]->SetToScr(pModZ,pScrZ);
pWCurve[2]->SetToScr(pModZ,pScrZ);
pWCurve[3]->SetToScr(pModZ,pScrZ);
pWCurve[0]->Draw(pDC,4,0,1);
pWCurve[1]->Draw(pDC,4,0,1);
pWCurve[2]->Draw(pDC,4,0,1);
pWCurve[3]->Draw(pDC,4,0,1);

int iCnt1;
double fred;
C3dVector vDrawPt;
C4dVector vWpt;
Pt_Object ThePoint;
ThePoint.Create(vDrawPt,0,0,0,11,0,0,NULL);
vDrawPt = deCastelJau2(0,0.5).GetPoint();
ThePoint.Pt_Point->x  = vDrawPt.x;
ThePoint.Pt_Point->y  = vDrawPt.y;
ThePoint.Pt_Point->z  = vDrawPt.z;
ThePoint.SetToScr(pModZ,pScrZ);
pDC->MoveTo((int) ThePoint.DSP_Point->x,(int) ThePoint.DSP_Point->y);
for (iCnt1 = 1; iCnt1 < 21; iCnt1++)
{
	fred = iCnt1*0.05;
	vDrawPt = deCastelJau2(fred,0.5).GetPoint();
	ThePoint.Pt_Point->x  = vDrawPt.x;
  ThePoint.Pt_Point->y  = vDrawPt.y;
  ThePoint.Pt_Point->z  = vDrawPt.z;
	ThePoint.SetToScr(pModZ,pScrZ);
  pDC->LineTo((int) ThePoint.DSP_Point->x,(int)ThePoint.DSP_Point->y);
}
vDrawPt = deCastelJau2(0.5,0).GetPoint();
ThePoint.Pt_Point->x  = vDrawPt.x;
ThePoint.Pt_Point->y  = vDrawPt.y;
ThePoint.Pt_Point->z  = vDrawPt.z;
ThePoint.SetToScr(pModZ,pScrZ);
pDC->MoveTo((int) ThePoint.DSP_Point->x,(int) ThePoint.DSP_Point->y);
vDrawPt = deCastelJau2(0.5,1).GetPoint();
ThePoint.Pt_Point->x  = vDrawPt.x;
ThePoint.Pt_Point->y  = vDrawPt.y;
ThePoint.Pt_Point->z  = vDrawPt.z;
ThePoint.SetToScr(pModZ,pScrZ);
pDC->LineTo((int) ThePoint.DSP_Point->x,(int) ThePoint.DSP_Point->y);

}


void Surf_Ex1::OglDraw(int iDspFlgs)
{
GLfloat* ctrlpts;
int iu;
int iv;
int icu,icv;
int iInd = 0;
iu = pWCurve[0]->iNoVerts;
iv = pWCurve[2]->iNoVerts;

ctrlpts = (GLfloat*) malloc(iv*iu*4*sizeof(GLfloat));

for (icv = 0;icv < iv; icv++)
{
  for (icu = 0;icu < iu;icu++)
  {
  ctrlpts[iInd]   = (float) pWCurve[icv]->pCVertex[icu]->xw;
  ctrlpts[iInd+1] = (float) pWCurve[icv]->pCVertex[icu]->yw;
  ctrlpts[iInd+2] = (float) pWCurve[icv]->pCVertex[icu]->zw;
  ctrlpts[iInd+3] = (float) pWCurve[icv]->pCVertex[icu]->w;
  iInd = iInd+4;
  }  
}


glColor3fv(cols[iColour]);
glEnable(GL_MAP2_VERTEX_4);
//glBegin(GL_POINTS);
//glVertex4fv(&ctrlpts[0]);
//glVertex4fv(&ctrlpts[4]);
//glVertex4fv(&ctrlpts[8]);
//glVertex4fv(&ctrlpts[12]);
//glVertex4fv(&ctrlpts[16]);
//glVertex4fv(&ctrlpts[20]);
//glEnd();

glMap2f(GL_MAP2_VERTEX_4,0.0f,1.0f,4,iu,0.0f,1.0f,iu*4,iv,ctrlpts);
glEnable(GL_MAP2_VERTEX_4);
glMapGrid2f(20,0.0f,1.0f,20,0.0f,1.0f);
glEvalMesh2(GL_FILL,0,20,0,20);
glFlush();
free(ctrlpts);
pWCurve[1]->OglDraw(0,1);
}

void Surf_Ex1::OglDrawW()
{
GLfloat* ctrlpts;
int iu;
int iv;
int icu,icv;
int iInd = 0;
iu = pWCurve[0]->iNoVerts;
iv = pWCurve[2]->iNoVerts;

ctrlpts = (GLfloat*) malloc(iv*iu*4*sizeof(GLfloat));

for (icv = 0;icv < iv; icv++)
{
  for (icu = 0;icu < iu;icu++)
  {
  ctrlpts[iInd]   = (float) pWCurve[icv]->pCVertex[icu]->xw;
  ctrlpts[iInd+1] = (float) pWCurve[icv]->pCVertex[icu]->yw;
  ctrlpts[iInd+2] = (float) pWCurve[icv]->pCVertex[icu]->zw;
  ctrlpts[iInd+3] = (float) pWCurve[icv]->pCVertex[icu]->w;
  iInd = iInd+4;
  }  
}


glColor3fv(cols[iColour]);
glEnable(GL_MAP2_VERTEX_4);

glMap2f(GL_MAP2_VERTEX_4,0.0f,1.0f,4,iu,0.0f,1.0f,iu*4,iv,ctrlpts);
glEnable(GL_MAP2_VERTEX_4);
glMapGrid2f(20,0.0f,1.0f,20,0.0f,1.0f);
glEvalMesh2(GL_LINE,0,20,0,20);
glFlush();
free(ctrlpts);
pWCurve[1]->OglDraw(0,1);
}


void Surf_Ex1::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
pModZ = pModMat;
pScrZ = pScrTran;
}

void Surf_Ex1::HighLight(CClientDC* pDC)
{
Draw(pDC,4);
}

G_ObjectD Surf_Ex1::SelDist(CPoint InPT,Filter FIL)
{
double dSelD;
G_ObjectD Ret;
Pt_Object ThePoint;
C3dVector vDrawPt;
ThePoint.Create(vDrawPt,0,0,0,11,0,0,NULL);
vDrawPt = deCastelJau2(0.5,0.5).GetPoint();
ThePoint.Pt_Point->x  = vDrawPt.x;
ThePoint.Pt_Point->y  = vDrawPt.y;
ThePoint.Pt_Point->z  = vDrawPt.z;
ThePoint.SetToScr(pModZ,pScrZ);
dSelD = pow(ThePoint.DSP_Point->x-InPT.x,2)+pow(ThePoint.DSP_Point->y-InPT.y,2);
ThePoint.Clear();
Ret.Dist=dSelD;
Ret.pObj=this;
return (Ret);
}

C4dVector Surf_Ex1::deCastelJau2(double u,double v)
	 
{
C4dVector vRetPt;

ContrPolyW cLast;
					
cLast.Create(1,NULL);

cLast.AddVertW(pWCurve[0]->deCastelJau1(u));
cLast.AddVertW(pWCurve[1]->deCastelJau1(u));  
vRetPt = cLast.deCastelJau1(v);

cLast.Clear();
return (vRetPt);
}



//Extruded Surface
IMPLEMENT_DYNAMIC( Surf_R, CObject )

//ContrPolyW* pWCurve[2];
//C3dVector vTranVec;
//C3dMatrix* pScrZ;

void Surf_R::Create(ContrPolyW* pWCurveIn, C3dVector vRotVecIn, int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iObjType = 11;
pParent=Parrent;
vRotVec = vRotVecIn;


pWCurve[0] = pWCurveIn;
pWCurve[1] = new ContrPolyW;
pWCurve[1]->Create(1,this);
pWCurve[1]->Set2(pWCurve[0]);
pWCurve[1]->CPolyRot(45);


pWCurve[2] = new ContrPolyW;
pWCurve[2]->Create(1,this);
pWCurve[2]->Set2(pWCurve[0]);
pWCurve[2]->CPolyRot(90);
pWCurve[2]->CWeight(2);

pWCurve[3] = new ContrPolyW;
pWCurve[3]->Create(1,this);
pWCurve[3]->AddVertW(*pWCurve[0]->pCVertex[0]);
pWCurve[3]->AddVertW(*pWCurve[1]->pCVertex[0]);
pWCurve[3]->AddVertW(*pWCurve[2]->pCVertex[0]);


pWCurve[4] = new ContrPolyW;
pWCurve[4]->Create(1,this);
pWCurve[4]->AddVertW(*pWCurve[0]->pCVertex[pWCurve[0]->iNoVerts-1]);
pWCurve[4]->AddVertW(*pWCurve[1]->pCVertex[pWCurve[0]->iNoVerts-1]);
pWCurve[4]->AddVertW(*pWCurve[2]->pCVertex[pWCurve[0]->iNoVerts-1]);

}

void Surf_R::Draw(CDC* pDC,int iDrawmode)
{
pWCurve[0]->SetToScr(pModZ,pScrZ);
pWCurve[1]->SetToScr(pModZ,pScrZ);
pWCurve[2]->SetToScr(pModZ,pScrZ);
pWCurve[3]->SetToScr(pModZ,pScrZ);
pWCurve[4]->SetToScr(pModZ,pScrZ);
pWCurve[0]->Draw(pDC,4,0,1);
//pWCurve[1]->Draw(pDC,4);
pWCurve[2]->Draw(pDC,4,0,1);
pWCurve[3]->Draw(pDC,4,0,1);
pWCurve[4]->Draw(pDC,4,0,1);


C3dVector vDrawPt;
C4dVector vWpt;
//Pt_Object ThePoint;


//  ThePoint.Create(vDrawPt,0);
//  vDrawPt = deCastelJau2(0,0.5).GetPoint();
//  ThePoint.Pt_Point->x  = vDrawPt.x;
//  ThePoint.Pt_Point->y  = vDrawPt.y;
//  ThePoint.Pt_Point->z  = vDrawPt.z;
//  ThePoint.SetToScr(pScrZ);
//  pDC->MoveTo(ThePoint.DSP_Point->x,ThePoint.DSP_Point->y);
//   for (iCnt1 = 1; iCnt1 < 21; iCnt1++)
//       {
//	   fred = iCnt1*0.05;
//	   vDrawPt = deCastelJau2(fred,0.5).GetPoint();
//	   ThePoint.Pt_Point->x  = vDrawPt.x;
//       ThePoint.Pt_Point->y  = vDrawPt.y;
//       ThePoint.Pt_Point->z  = vDrawPt.z;
//	   ThePoint.SetToScr(pScrZ);
//       pDC->LineTo(ThePoint.DSP_Point->x,ThePoint.DSP_Point->y);
//       }


}


void Surf_R::OglDraw(int iDspFlgs)
{
GLfloat* ctrlpts;
int iu;
int iv;
int icu,icv;
int iInd = 0;
iu = pWCurve[0]->iNoVerts;
iv = pWCurve[3]->iNoVerts;

ctrlpts = (GLfloat*) malloc(iv*iu*4*sizeof(GLfloat));

for (icv = 0;icv < iv; icv++)
{
  for (icu = 0;icu < iu;icu++)
  {
	 
  ctrlpts[iInd]   = (float) pWCurve[icv]->pCVertex[icu]->xw;
  ctrlpts[iInd+1] = (float) pWCurve[icv]->pCVertex[icu]->yw;
  ctrlpts[iInd+2] = (float)-pWCurve[icv]->pCVertex[icu]->zw;
  ctrlpts[iInd+3] = (float) pWCurve[icv]->pCVertex[icu]->w;
  iInd = iInd+4;
  }  
}



glEnable(GL_MAP2_VERTEX_4);
//glBegin(GL_POINTS);
//glVertex4fv(&ctrlpts[0]);
//glVertex4fv(&ctrlpts[4]);
//glVertex4fv(&ctrlpts[8]);
//glVertex4fv(&ctrlpts[12]);
//glVertex4fv(&ctrlpts[16]);
//glVertex4fv(&ctrlpts[20]);
//glEnd();

glMap2f(GL_MAP2_VERTEX_4,0.0f,1.0f,4,iu,0.0f,1.0f,iu*4,iv,ctrlpts);
glEnable(GL_MAP2_VERTEX_4);
glMapGrid2f(20,0.0f,1.0f,20,0.0f,1.0f);
glEvalMesh2(GL_FILL,0,20,0,20);
glFlush();
free(ctrlpts);
}

void Surf_R::OglDrawW(int iDspFlgs)
{
GLfloat* ctrlpts;
int iu;
int iv;
int icu,icv;
int iInd = 0;
iu = pWCurve[0]->iNoVerts;
iv = pWCurve[3]->iNoVerts;

ctrlpts = (GLfloat*) malloc(iv*iu*4*sizeof(GLfloat));

for (icv = 0;icv < iv; icv++)
{
  for (icu = 0;icu < iu;icu++)
  {
	 
  ctrlpts[iInd]   = (float) pWCurve[icv]->pCVertex[icu]->xw;
  ctrlpts[iInd+1] = (float) pWCurve[icv]->pCVertex[icu]->yw;
  ctrlpts[iInd+2] = (float)-pWCurve[icv]->pCVertex[icu]->zw;
  ctrlpts[iInd+3] = (float) pWCurve[icv]->pCVertex[icu]->w;
  iInd = iInd+4;
  }  
}



glEnable(GL_MAP2_VERTEX_4);
//glBegin(GL_POINTS);
//glVertex4fv(&ctrlpts[0]);
//glVertex4fv(&ctrlpts[4]);
//glVertex4fv(&ctrlpts[8]);
//glVertex4fv(&ctrlpts[12]);
//glVertex4fv(&ctrlpts[16]);
//glVertex4fv(&ctrlpts[20]);
//glEnd();

glMap2f(GL_MAP2_VERTEX_4,0.0f,1.0f,4,iu,0.0f,1.0f,iu*4,iv,ctrlpts);
glEnable(GL_MAP2_VERTEX_4);
glMapGrid2f(20,0.0f,1.0f,20,0.0f,1.0f);
glEvalMesh2(GL_LINE,0,20,0,20);
glFlush();
free(ctrlpts);
}


void Surf_R::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
pModZ =	pModMat;
pScrZ = pScrTran;
}

void Surf_R::HighLight(CDC* pDC)
{
Draw(pDC,4);
}

G_ObjectD Surf_R::SelDist(CPoint InPT,Filter FIL)
{
double dSelD;
G_ObjectD Ret;
Pt_Object ThePoint;
C3dVector vDrawPt;
ThePoint.Create(vDrawPt,0,0,0,11,0,0,NULL);
vDrawPt = deCastelJau2(0.5,0.5).GetPoint();
ThePoint.Pt_Point->x  = vDrawPt.x;
ThePoint.Pt_Point->y  = vDrawPt.y;
ThePoint.Pt_Point->z  = vDrawPt.z;
ThePoint.SetToScr(pModZ, pScrZ);
dSelD = pow(ThePoint.DSP_Point->x-InPT.x,2)+pow(ThePoint.DSP_Point->y-InPT.y,2);
ThePoint.Clear();

Ret.pObj=this;
Ret.Dist=dSelD;
return (Ret);
}

C4dVector Surf_R::deCastelJau2(double u,double v)
	 
{
C4dVector vRetPt;
ContrPolyW cLast;					
cLast.Create(1,this);
cLast.AddVertW(pWCurve[0]->deCastelJau1(u));
cLast.AddVertW(pWCurve[1]->deCastelJau1(u));  
cLast.AddVertW(pWCurve[2]->deCastelJau1(u)); 
vRetPt = cLast.deCastelJau1(v);
cLast.Clear();
return (vRetPt);
}


//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object38, CObject )


E_Object38::~E_Object38()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
pVertex[4]=NULL;
pVertex[5]=NULL;
pVertex[6]=NULL;
pVertex[7]=NULL;
}

void E_Object38::Info()
{
  char S1[80];
  E_Object::Info();
  sprintf_s(S1,"NODES %i %i %i %i",pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel,pVertex[3]->iLabel);
  outtext1(S1); 
  sprintf_s(S1,"NODES %i %i %i %i",pVertex[4]->iLabel,pVertex[5]->iLabel,pVertex[6]->iLabel,pVertex[7]->iLabel);
  outtext1(S1); 
}

void E_Object38::Create(Pt_Object* pInVertex[100], int iLab, int iCol, int iType, int iPID, int iMat, int iNo, G_Object* Parrent, Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<8;i++)
{
  pVertex[i] = pInVertex[i];
}


}

void E_Object38::Reverse()
{
Pt_Object* pT[8];
pT[0]=pVertex[0];
pT[1]=pVertex[1];
pT[2]=pVertex[2];
pT[3]=pVertex[3];
pT[4]=pVertex[4];
pT[5]=pVertex[5];
pT[6]=pVertex[6];
pT[7]=pVertex[7];
pVertex[0] = pT[0];
pVertex[1] = pT[3];
pVertex[2] = pT[2];
pVertex[3] = pT[1];
pVertex[4] = pT[4];
pVertex[5] = pT[7];
pVertex[6] = pT[6];
pVertex[7] = pT[5];
}

G_Object* E_Object38::GetNode(int i)
{
return (pVertex[i]);
}

int E_Object38::GetLinkList(cLink* Links[12])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
Links[0]->iColour=iColour;
Links[1]=new cLink;
Links[1]->pParent=this;
Links[1]->pVertex[0]=pVertex[1];
Links[1]->pVertex[1]=pVertex[2];
Links[1]->iColour=iColour;
Links[2]=new cLink;
Links[2]->pParent=this;
Links[2]->pVertex[0]=pVertex[2];
Links[2]->pVertex[1]=pVertex[3];
Links[2]->iColour=iColour;
Links[3]=new cLink;
Links[3]->pParent=this;
Links[3]->pVertex[0]=pVertex[3];
Links[3]->pVertex[1]=pVertex[0];
Links[3]->iColour=iColour;

Links[4]=new cLink;
Links[4]->pParent=this;
Links[4]->pVertex[0]=pVertex[4];
Links[4]->pVertex[1]=pVertex[5];
Links[4]->iColour=iColour;
Links[5]=new cLink;
Links[5]->pParent=this;
Links[5]->pVertex[0]=pVertex[5];
Links[5]->pVertex[1]=pVertex[6];
Links[5]->iColour=iColour;
Links[6]=new cLink;
Links[6]->pParent=this;
Links[6]->pVertex[0]=pVertex[6];
Links[6]->pVertex[1]=pVertex[7];
Links[6]->iColour=iColour;
Links[7]=new cLink;
Links[7]->pParent=this;
Links[7]->pVertex[0]=pVertex[7];
Links[7]->pVertex[1]=pVertex[4];
Links[7]->iColour=iColour;

Links[8]=new cLink;
Links[8]->pParent=this;
Links[8]->pVertex[0]=pVertex[0];
Links[8]->pVertex[1]=pVertex[4];
Links[8]->iColour=iColour;
Links[9]=new cLink;
Links[9]->pParent=this;
Links[9]->pVertex[0]=pVertex[1];
Links[9]->pVertex[1]=pVertex[5];
Links[9]->iColour=iColour;
Links[10]=new cLink;
Links[10]->pParent=this;
Links[10]->pVertex[0]=pVertex[2];
Links[10]->pVertex[1]=pVertex[6];
Links[10]->iColour=iColour;
Links[11]=new cLink;
Links[11]->pParent=this;
Links[11]->pVertex[0]=pVertex[3];
Links[11]->pVertex[1]=pVertex[7];
Links[11]->iColour=iColour;

return (12);
}

int E_Object38::GetfaceList(cFace* Faces[6])
{
int ic;
ic=GetCol();
Faces[0]=new cFace();
Faces[0]->pParent=this;
Faces[0]->pVertex[0]=pVertex[0];
Faces[0]->pVertex[1]=pVertex[3];
Faces[0]->pVertex[2]=pVertex[2];
Faces[0]->pVertex[3]=pVertex[1];
Faces[0]->NoVert=4;
Faces[0]->iColour=ic;

Faces[1]=new cFace();
Faces[1]->pParent=this;
Faces[1]->pVertex[0]=pVertex[4];
Faces[1]->pVertex[1]=pVertex[5];
Faces[1]->pVertex[2]=pVertex[6];
Faces[1]->pVertex[3]=pVertex[7];
Faces[1]->NoVert=4;
Faces[1]->iColour=ic;

Faces[2]=new cFace();
Faces[2]->pParent=this;
Faces[2]->pVertex[0]=pVertex[0];
Faces[2]->pVertex[1]=pVertex[1];
Faces[2]->pVertex[2]=pVertex[5];
Faces[2]->pVertex[3]=pVertex[4];
Faces[2]->NoVert=4;
Faces[2]->iColour=ic;

Faces[3]=new cFace();
Faces[3]->pParent=this;
Faces[3]->pVertex[0]=pVertex[1];
Faces[3]->pVertex[1]=pVertex[2];
Faces[3]->pVertex[2]=pVertex[6];
Faces[3]->pVertex[3]=pVertex[5];
Faces[3]->NoVert=4;
Faces[3]->iColour=ic;

Faces[4]=new cFace();
Faces[4]->pParent=this;
Faces[4]->pVertex[0]=pVertex[2];
Faces[4]->pVertex[1]=pVertex[3];
Faces[4]->pVertex[2]=pVertex[7];
Faces[4]->pVertex[3]=pVertex[6];
Faces[4]->NoVert=4;
Faces[4]->iColour=ic;

Faces[5]=new cFace();
Faces[5]->pParent=this;
Faces[5]->pVertex[0]=pVertex[0];
Faces[5]->pVertex[1]=pVertex[4];
Faces[5]->pVertex[2]=pVertex[7];
Faces[5]->pVertex[3]=pVertex[3];
Faces[5]->NoVert=4;
Faces[5]->iColour=ic;

return (6);
}

void E_Object38::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}


BOOL E_Object38::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object38::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
    ar << pVertex[3]->iLabel;
    ar << pVertex[4]->iLabel;
    ar << pVertex[5]->iLabel;
    ar << pVertex[6]->iLabel;
    ar << pVertex[7]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[3] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[4] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[5] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[6] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[7] = MESH->GetNode(iNd);
	}
}

G_Object* E_Object38::Copy(G_Object* Parrent)
{
ME_Object* MESH = (ME_Object*) Parrent;
E_Object38* gret = new E_Object38;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pVertex[3] = MESH->GetNode(pVertex[3]->iLabel);
gret->pVertex[4] = MESH->GetNode(pVertex[4]->iLabel);
gret->pVertex[5] = MESH->GetNode(pVertex[5]->iLabel);
gret->pVertex[6] = MESH->GetNode(pVertex[6]->iLabel);
gret->pVertex[7] = MESH->GetNode(pVertex[7]->iLabel);
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object38::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object38* gret = new E_Object38;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[3]);
gret->pVertex[3] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[4]);
gret->pVertex[4] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[5]);
gret->pVertex[5] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[6]);
gret->pVertex[6] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[7]);
gret->pVertex[7] = Target->pNodes[EInd+iSInd];
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object38::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object38* gret = new E_Object38;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] =  pInVertex[0];
gret->pVertex[1] =  pInVertex[1];
gret->pVertex[2] =  pInVertex[2];
gret->pVertex[3] =  pInVertex[3];
gret->pVertex[4] =  pInVertex[4];
gret->pVertex[5] =  pInVertex[5];
gret->pVertex[6] =  pInVertex[6];
gret->pVertex[7] =  pInVertex[7];
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object38::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);

pDC->LineTo((int) pVertex[4]->DSP_Point->x,(int) pVertex[4]->DSP_Point->y);
pDC->LineTo((int) pVertex[5]->DSP_Point->x,(int) pVertex[5]->DSP_Point->y);
pDC->LineTo((int) pVertex[6]->DSP_Point->x,(int) pVertex[6]->DSP_Point->y);
pDC->LineTo((int) pVertex[7]->DSP_Point->x,(int) pVertex[7]->DSP_Point->y);
pDC->LineTo((int) pVertex[4]->DSP_Point->x,(int) pVertex[4]->DSP_Point->y);

pDC->MoveTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[5]->DSP_Point->x,(int) pVertex[5]->DSP_Point->y);

pDC->MoveTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[6]->DSP_Point->x,(int) pVertex[6]->DSP_Point->y);

pDC->MoveTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->LineTo((int) pVertex[7]->DSP_Point->x,(int) pVertex[7]->DSP_Point->y);
}

void E_Object38::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector d[8];
int i;
int ind;

for (i=0;i<8;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
ind=ME->iCVar;
glLineWidth(1.0);
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<8;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S*dFS;
       }
    }
  }
  Selectable=1;
  if ((iDspFlgs & DSP_LINE) > 0)
  {
    glColor3fv(cols[iColour]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
	glBegin(GL_LINES);
	glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));



	glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));

	glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
  glEnd();
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"E%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
  if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
  {
    sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
    OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  } 
  if ((iDspFlgs & DSP_ELSYS) == 0)
  {
	  C3dMatrix mS = GetElSys();
	  C3dVector vC = Get_Centroid();
	  mS.Transpose();
	  C3dVector vX = mS.GetColVec(1);
	  C3dVector vY = mS.GetColVec(2);;
	  C3dVector vZ = mS.GetColVec(3);;

	  vX *= 0.5*dS1; vY *= 0.5*dS1; vZ *= 0.5*dS1;
	  vX += vC; vY += vC; vZ += vC;

	  glBegin(GL_LINES);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glEnd();
	  glRasterPos3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
	  glRasterPos3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
	  glRasterPos3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }
}
else
{
  Selectable=0;
}
}

void E_Object38::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector d[8];
int i;
for (i=0;i<8;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS = 1.0;
dFS = ME->dResFactor;
glLineWidth(2.0);
      float fCols[8]={0,0,0,0,0,0,0,0};
      BOOL bD=FALSE;
      int iVar;
      iVar=ME->iCVar;
      //Nodal data

      if ((pVertex[0]->pResV != NULL) &&
          (pVertex[1]->pResV != NULL) &&
          (pVertex[2]->pResV != NULL) &&
          (pVertex[3]->pResV != NULL) &&
          (pVertex[4]->pResV != NULL) &&
          (pVertex[5]->pResV != NULL) &&
          (pVertex[6]->pResV != NULL) &&
          (pVertex[7]->pResV != NULL))
      {
        bD=TRUE;
        fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
        fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
        fCols[2]=GetContourCol(*pVertex[2]->pResV->GetAddress(iVar)*dFS);
        fCols[3]=GetContourCol(*pVertex[3]->pResV->GetAddress(iVar)*dFS);
        fCols[4]=GetContourCol(*pVertex[4]->pResV->GetAddress(iVar)*dFS);
        fCols[5]=GetContourCol(*pVertex[5]->pResV->GetAddress(iVar)*dFS);
        fCols[6]=GetContourCol(*pVertex[6]->pResV->GetAddress(iVar)*dFS);
        fCols[7]=GetContourCol(*pVertex[7]->pResV->GetAddress(iVar)*dFS);
      }
      if (pResV!=NULL)
      {
        bD=TRUE;
        float fC;
        if (iLabel==106)
        {
           bD=TRUE;   
        }
        fC=GetContourCol(*pResV->GetAddress(iVar)*dFS);
        fCols[0]=fC;
        fCols[1]=fC;
        fCols[2]=fC;
        fCols[3]=fC;
        fCols[4]=fC;
        fCols[5]=fC;
        fCols[6]=fC;
        fCols[7]=fC;
      }

C3dVector v1;
C3dVector v2;
C3dVector Vn;
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<8;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S * dFS;
       }
    }
  }
	Selectable=1;
  glColor3fv(cols[iColour]);
  if (((iDspFlgs & DSP_CONT)>0) || (bD==FALSE))
  {
	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
  	
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();

	  v1.x = pVertex[5]->Pt_Point->x-pVertex[4]->Pt_Point->x;
	  v1.y = pVertex[5]->Pt_Point->y-pVertex[4]->Pt_Point->y;
	  v1.z = pVertex[5]->Pt_Point->z-pVertex[4]->Pt_Point->z;

	  v2.x = pVertex[6]->Pt_Point->x-pVertex[5]->Pt_Point->x;
	  v2.y = pVertex[6]->Pt_Point->y-pVertex[5]->Pt_Point->y;
	  v2.z = pVertex[6]->Pt_Point->z-pVertex[5]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	  glEnd();

	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[5]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[5]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[5]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glEnd();

	  v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	  v2.x = pVertex[5]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v2.y = pVertex[5]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v2.z = pVertex[5]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glEnd();

	  v1.x = pVertex[6]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v1.y = pVertex[6]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v1.z = pVertex[6]->Pt_Point->z-pVertex[2]->Pt_Point->z;

	  v2.x = pVertex[7]->Pt_Point->x-pVertex[6]->Pt_Point->x;
	  v2.y = pVertex[7]->Pt_Point->y-pVertex[6]->Pt_Point->y;
	  v2.z = pVertex[7]->Pt_Point->z-pVertex[6]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();

	  v1.x = pVertex[3]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[3]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[3]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[7]->Pt_Point->x-pVertex[3]->Pt_Point->x;
	  v2.y = pVertex[7]->Pt_Point->y-pVertex[3]->Pt_Point->y;
	  v2.z = pVertex[7]->Pt_Point->z-pVertex[3]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glEnd();
  }
  else
  {

      if (bD)
      {
        glColor3fv(cols[124]);
        glEnable(GL_TEXTURE_1D);
	    glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[0]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	      glTexCoord1f(fCols[1]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	      glTexCoord1f(fCols[2]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	      glTexCoord1f(fCols[3]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	      glEnd();

	      v1.x = pVertex[5]->Pt_Point->x-pVertex[4]->Pt_Point->x;
	      v1.y = pVertex[5]->Pt_Point->y-pVertex[4]->Pt_Point->y;
	      v1.z = pVertex[5]->Pt_Point->z-pVertex[4]->Pt_Point->z;

	      v2.x = pVertex[6]->Pt_Point->x-pVertex[5]->Pt_Point->x;
	      v2.y = pVertex[6]->Pt_Point->y-pVertex[5]->Pt_Point->y;
	      v2.z = pVertex[6]->Pt_Point->z-pVertex[5]->Pt_Point->z;
	      Vn = v1.Cross(v2);

	      Vn.Normalize();
	      glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[4]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	      glTexCoord1f(fCols[5]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	      glTexCoord1f(fCols[6]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	      glTexCoord1f(fCols[7]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	      glEnd();

	      v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	      v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	      v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	      v2.x = pVertex[5]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	      v2.y = pVertex[5]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	      v2.z = pVertex[5]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	      Vn = v1.Cross(v2);

	      Vn.Normalize();
	      glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[0]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
          glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	      glTexCoord1f(fCols[1]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	      glTexCoord1f(fCols[5]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	      glTexCoord1f(fCols[4]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	      glEnd();

	      v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	      v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	      v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	      v2.x = pVertex[5]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	      v2.y = pVertex[5]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	      v2.z = pVertex[5]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	      Vn = v1.Cross(v2);

	      Vn.Normalize();
	      glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[1]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	      glTexCoord1f(fCols[2]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	      glTexCoord1f(fCols[6]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	      glTexCoord1f(fCols[5]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	      glEnd();

	      v1.x = pVertex[6]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	      v1.y = pVertex[6]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	      v1.z = pVertex[6]->Pt_Point->z-pVertex[2]->Pt_Point->z;

	      v2.x = pVertex[7]->Pt_Point->x-pVertex[6]->Pt_Point->x;
	      v2.y = pVertex[7]->Pt_Point->y-pVertex[6]->Pt_Point->y;
	      v2.z = pVertex[7]->Pt_Point->z-pVertex[6]->Pt_Point->z;
	      Vn = v1.Cross(v2);

	      Vn.Normalize();
	      glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[2]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	      glTexCoord1f(fCols[6]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[6]->Pt_Point->x+d[6].x),(float) (pVertex[6]->Pt_Point->y+d[6].y),(float) (pVertex[6]->Pt_Point->z+d[6].z));
	      glTexCoord1f(fCols[7]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	      glTexCoord1f(fCols[3]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	      glEnd();

	      v1.x = pVertex[3]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	      v1.y = pVertex[3]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	      v1.z = pVertex[3]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	      v2.x = pVertex[7]->Pt_Point->x-pVertex[3]->Pt_Point->x;
	      v2.y = pVertex[7]->Pt_Point->y-pVertex[3]->Pt_Point->y;
	      v2.z = pVertex[7]->Pt_Point->z-pVertex[3]->Pt_Point->z;
	      Vn = v1.Cross(v2);

	      Vn.Normalize();
	      glBegin(GL_POLYGON);
	      glTexCoord1f(fCols[0]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	      glTexCoord1f(fCols[3]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	      glTexCoord1f(fCols[7]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[7]->Pt_Point->x+d[7].x),(float) (pVertex[7]->Pt_Point->y+d[7].y),(float) (pVertex[7]->Pt_Point->z+d[7].z));
	      glTexCoord1f(fCols[4]);
          glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	      glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	      glEnd();
        glDisable(GL_TEXTURE_1D);
      }
  }
}
else
{
  Selectable=0;
}
}

void E_Object38::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}

void E_Object38::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s%8i%8i","CHEXA   ",iLabel,PID);
fprintf(pFile,"%8i",pVertex[0]->iLabel);
fprintf(pFile,"%8i",pVertex[1]->iLabel);
fprintf(pFile,"%8i",pVertex[2]->iLabel);
fprintf(pFile,"%8i",pVertex[3]->iLabel);
fprintf(pFile,"%8i",pVertex[4]->iLabel);
fprintf(pFile,"%8i",pVertex[5]->iLabel);
fprintf(pFile,"\n","");
fprintf(pFile,"%8s","        ");
fprintf(pFile,"%8i",pVertex[6]->iLabel);
fprintf(pFile,"%8i",pVertex[7]->iLabel);
fprintf(pFile,"\n","");
}



Mat E_Object38::getCoords3d()
{
int i;
Mat coord(iNoNodes,3);
for (i=0;i<iNoNodes;i++)
{
  *coord.mn(i+1, 1) = pVertex[i]->Pt_Point->x;
  *coord.mn(i+1, 2) = pVertex[i]->Pt_Point->y;
  *coord.mn(i+1, 3) = pVertex[i]->Pt_Point->z;
}
return (coord);
}

double E_Object38::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  double T = 0;
  int iDOFID;
  int j;
  Mat fun;
  Mat FunPnt(1, 3);
  *FunPnt.mn(1, 1) = 0.0;
  *FunPnt.mn(1, 2) = 0.0;
  *FunPnt.mn(1, 3) = 0.0;
  fun = this->ShapeFun(FunPnt, 1);
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T**fun.mn(1, j + 1);
  }
  fun.clear();
  FunPnt.clear();
  return(dTemp);
}

C3dVector E_Object38::Get_Centroid()
{
Mat fun;
Mat FunPnt(1,3);
*FunPnt.mn(1,1)=0.0;
*FunPnt.mn(1,2)=0.0;
*FunPnt.mn(1,3)=0.0;
fun=ShapeFun(FunPnt,1);
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}

fun.clear();
FunPnt.clear();
return (vT);
}

C3dMatrix E_Object38::GetElSys()
{
	C3dVector vX;
	C3dVector vZ;
	C3dVector vY;
	vX = pVertex[1]->GetCoords();
	vX -= pVertex[0]->GetCoords();
	vX.Normalize();
	vZ = pVertex[4]->GetCoords();
	vZ -= pVertex[0]->GetCoords();  //Approx z direction
	vZ.Normalize();
	vY = vZ.Cross(vX);
	vY.Normalize();
	vZ = vX.Cross(vY);
	C3dMatrix vR;
	vR.SetColVec(1, vX);
	vR.SetColVec(2, vY);
	vR.SetColVec(3, vZ);
	vR.Transpose();
	return (vR);
}

int E_Object38::MaxBW()
{
int i;
int j;
int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<6;j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	  {
       MaxDof=pVertex[i]->dof[j];
    }
	  if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	  {
       MinDof=pVertex[i]->dof[j];
	  }
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}

Mat E_Object38::Sample(int iNo)
{
Mat Pts(iNo,4);

if (iNo ==8)
{
  double r3 = 1.0 / sqrt(3.0);
  double weight = 1.0;
  *Pts.mn(1,1) = -r3;   *Pts.mn(1,2) = -r3;   *Pts.mn(1,3) = -r3;   *Pts.mn(1,4) = weight;
  *Pts.mn(2,1) =  r3;   *Pts.mn(2,2) = -r3;   *Pts.mn(2,3) = -r3;   *Pts.mn(2,4) = weight;
  *Pts.mn(3,1) =  r3;   *Pts.mn(3,2) =  r3;   *Pts.mn(3,3) = -r3;   *Pts.mn(3,4) = weight;
  *Pts.mn(4,1) = -r3;   *Pts.mn(4,2) =  r3;   *Pts.mn(4,3) = -r3;   *Pts.mn(4,4) = weight;
  *Pts.mn(5,1) = -r3;   *Pts.mn(5,2) = -r3;   *Pts.mn(5,3) =  r3;   *Pts.mn(5,4) = weight;
  *Pts.mn(6,1) =  r3;   *Pts.mn(6,2) = -r3;   *Pts.mn(6,3) =  r3;   *Pts.mn(6,4) = weight;
  *Pts.mn(7,1) =  r3;   *Pts.mn(7,2) =  r3;   *Pts.mn(7,3) =  r3;   *Pts.mn(7,4) = weight;
  *Pts.mn(8,1) = -r3;   *Pts.mn(8,2) =  r3;   *Pts.mn(8,3) =  r3;   *Pts.mn(8,4) = weight;
}
return (Pts);
}

Mat E_Object38::ShapeFun(Mat Points, int i)
{
Mat fun(1,8);
double xi,eta,zeta;
xi = *Points.mn(i, 1);
eta = *Points.mn(i, 2);
zeta = *Points.mn(i, 3);

*fun.mn(1,1) = 0.125*(1.0-xi)*(1.0-eta)*(1.0-zeta);
*fun.mn(1,2) = 0.125*(1.0+xi)*(1.0-eta)*(1.0-zeta);
*fun.mn(1,3) = 0.125*(1.0+xi)*(1.0+eta)*(1.0-zeta);
*fun.mn(1,4) = 0.125*(1.0-xi)*(1.0+eta)*(1.0-zeta);
*fun.mn(1,5) = 0.125*(1.0-xi)*(1.0-eta)*(1.0+zeta);
*fun.mn(1,6) = 0.125*(1.0+xi)*(1.0-eta)*(1.0+zeta);
*fun.mn(1,7) = 0.125*(1.0+xi)*(1.0+eta)*(1.0+zeta);
*fun.mn(1,8) = 0.125*(1.0-xi)*(1.0+eta)*(1.0+zeta);

return(fun);
}

Mat E_Object38::ShapeDer(Mat Points, int i)
{
Mat der(3,8);

  *der.mn(1,1) = -0.125 * (1 - *Points.mn(i,2)) * (1 - *Points.mn(i,3));
  *der.mn(1,2) =  0.125 * (1 - *Points.mn(i,2)) * (1 - *Points.mn(i,3));
  *der.mn(1,3) =  0.125 * (1 + *Points.mn(i,2)) * (1 - *Points.mn(i,3));
  *der.mn(1,4) = -0.125 * (1 + *Points.mn(i,2)) * (1 - *Points.mn(i,3));
  *der.mn(1,5) = -0.125 * (1 - *Points.mn(i,2)) * (1 + *Points.mn(i,3));
  *der.mn(1,6) =  0.125 * (1 - *Points.mn(i,2)) * (1 + *Points.mn(i,3));
  *der.mn(1,7) =  0.125 * (1 + *Points.mn(i,2)) * (1 + *Points.mn(i,3));
  *der.mn(1,8) = -0.125 * (1 + *Points.mn(i,2)) * (1 + *Points.mn(i,3));
  
  *der.mn(2,1) = -0.125 * (1 - *Points.mn(i,1)) * (1 - *Points.mn(i,3));
  *der.mn(2,2) = -0.125 * (1 + *Points.mn(i,1)) * (1 - *Points.mn(i,3));
  *der.mn(2,3) =  0.125 * (1 + *Points.mn(i,1)) * (1 - *Points.mn(i,3));
  *der.mn(2,4) =  0.125 * (1 - *Points.mn(i,1)) * (1 - *Points.mn(i,3));
  *der.mn(2,5) = -0.125 * (1 - *Points.mn(i,1)) * (1 + *Points.mn(i,3));
  *der.mn(2,6) = -0.125 * (1 + *Points.mn(i,1)) * (1 + *Points.mn(i,3));
  *der.mn(2,7) =  0.125 * (1 + *Points.mn(i,1)) * (1 + *Points.mn(i,3));
  *der.mn(2,8) =  0.125 * (1 - *Points.mn(i,1)) * (1 + *Points.mn(i,3));
  
  *der.mn(3,1) = -0.125 * (1 - *Points.mn(i,1)) * (1 - *Points.mn(i,2));
  *der.mn(3,2) = -0.125 * (1 + *Points.mn(i,1)) * (1 - *Points.mn(i,2));
  *der.mn(3,3) = -0.125 * (1 + *Points.mn(i,1)) * (1 + *Points.mn(i,2));
  *der.mn(3,4) = -0.125 * (1 - *Points.mn(i,1)) * (1 + *Points.mn(i,2));
  *der.mn(3,5) =  0.125 * (1 - *Points.mn(i,1)) * (1 - *Points.mn(i,2));
  *der.mn(3,6) =  0.125 * (1 + *Points.mn(i,1)) * (1 - *Points.mn(i,2));
  *der.mn(3,7) =  0.125 * (1 + *Points.mn(i,1)) * (1 + *Points.mn(i,2));
  *der.mn(3,8) =  0.125 * (1 - *Points.mn(i,1)) * (1 + *Points.mn(i,2));  
return(der);
}

Vec<int> E_Object38::GetSteerVec3d()
{
Vec<int> V(8*3);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[1]->dof[0];
*V.nn(5)=pVertex[1]->dof[1];
*V.nn(6)=pVertex[1]->dof[2];
*V.nn(7)=pVertex[2]->dof[0];
*V.nn(8)=pVertex[2]->dof[1];
*V.nn(9)=pVertex[2]->dof[2];
*V.nn(10)=pVertex[3]->dof[0];
*V.nn(11)=pVertex[3]->dof[1];
*V.nn(12)=pVertex[3]->dof[2];
*V.nn(13)=pVertex[4]->dof[0];
*V.nn(14)=pVertex[4]->dof[1];
*V.nn(15)=pVertex[4]->dof[2];
*V.nn(16)=pVertex[5]->dof[0];
*V.nn(17)=pVertex[5]->dof[1];
*V.nn(18)=pVertex[5]->dof[2];
*V.nn(19)=pVertex[6]->dof[0];
*V.nn(20)=pVertex[6]->dof[1];
*V.nn(21)=pVertex[6]->dof[2];
*V.nn(22)=pVertex[7]->dof[0];
*V.nn(23)=pVertex[7]->dof[1];
*V.nn(24)=pVertex[7]->dof[2];
return(V);
}

Vec<int> E_Object38::GetSteerVec1d()
{
Vec<int> V(8*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
*V.nn(3)=pVertex[2]->dof[0];
*V.nn(4)=pVertex[3]->dof[0];
*V.nn(5)=pVertex[4]->dof[0];
*V.nn(6)=pVertex[5]->dof[0];
*V.nn(7)=pVertex[6]->dof[0];
*V.nn(8)=pVertex[7]->dof[0];
return(V);
}

int E_Object38::noDof()
{
return(3);
}


CString E_Object38::GetName()
{
	return ("8 Node BRICK (CHEXA)");
}

int E_Object38::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "Mat ID";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	sVar[iNo] = "N3";
	iNo++;
	sVar[iNo] = "N4";
	iNo++;
	sVar[iNo] = "N5";
	iNo++;
	sVar[iNo] = "N6";
	iNo++;
	sVar[iNo] = "N7";
	iNo++;
	sVar[iNo] = "N8";
	iNo++;
	return(iNo);
}


int E_Object38::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iMatID);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[2]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[3]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[4]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[5]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[6]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[7]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object38::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;

	int newPID = atoi(sVar[0]);
	if (newPID != PID)
	{

		Property* pr = PT->GetItem(newPID);
		if (pr != NULL)
		{
			BOOL bC = SetProperty(pr);
			if (bC)
				outtext1("Property has Been Changed");
			else
				outtext1("Invalid Property");
		}
		int N1 = atoi(sVar[2]);
		int N2 = atoi(sVar[3]);
		int N3 = atoi(sVar[4]);
		int N4 = atoi(sVar[5]);
		int N5 = atoi(sVar[6]);
		int N6 = atoi(sVar[7]);
		int N7 = atoi(sVar[8]);
		int N8 = atoi(sVar[9]);
		if (pVertex[0]->iLabel != N1)
		{
			pN = pMe->GetNode(N1);
			if (pN != NULL)
				pVertex[0] = pN;

		}
		if (pVertex[1]->iLabel != N2)
		{
			pN = pMe->GetNode(N2);
			if (pN != NULL)
				pVertex[1] = pN;

		}
		if (pVertex[2]->iLabel != N3)
		{
			pN = pMe->GetNode(N3);
			if (pN != NULL)
				pVertex[2] = pN;

		}
		if (pVertex[3]->iLabel != N4)
		{
			pN = pMe->GetNode(N4);
			if (pN != NULL)
				pVertex[3] = pN;
		}
		if (pVertex[4]->iLabel != N5)
		{
			pN = pMe->GetNode(N5);
			if (pN != NULL)
				pVertex[4] = pN;
		}
		if (pVertex[5]->iLabel != N6)
		{
			pN = pMe->GetNode(N6);
			if (pN != NULL)
				pVertex[5] = pN;
		}
		if (pVertex[6]->iLabel != N7)
		{
			pN = pMe->GetNode(N7);
			if (pN != NULL)
				pVertex[6] = pN;
		}
		if (pVertex[7]->iLabel != N8)
		{
			pN = pMe->GetNode(N8);
			if (pN != NULL)
				pVertex[7] = pN;
		}
	}
}



//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object36, CObject )

E_Object36::~E_Object36()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
pVertex[4]=NULL;
pVertex[5]=NULL;
}

void E_Object36::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<6;i++)
   {
    pVertex[i] = pInVertex[i];
   }

}

void E_Object36::Reverse()
{
Pt_Object* pT[8];
pT[0]=pVertex[0];
pT[1]=pVertex[1];
pT[2]=pVertex[2];
pT[3]=pVertex[3];
pT[4]=pVertex[4];
pT[5]=pVertex[5];
pVertex[0] = pT[0];
pVertex[1] = pT[2];
pVertex[2] = pT[1];
pVertex[3] = pT[3];
pVertex[4] = pT[5];
pVertex[5] = pT[4];
}

void E_Object36::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

BOOL E_Object36::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object36::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
    ar << pVertex[3]->iLabel;
    ar << pVertex[4]->iLabel;
    ar << pVertex[5]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[3] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[4] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[5] = MESH->GetNode(iNd);
	}
}

G_Object* E_Object36::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object36* gret = new E_Object36;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pVertex[3] = MESH->GetNode(pVertex[3]->iLabel);
gret->pVertex[4] = MESH->GetNode(pVertex[4]->iLabel);
gret->pVertex[5] = MESH->GetNode(pVertex[5]->iLabel);
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object36::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object36* gret = new E_Object36;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[3]);
gret->pVertex[3] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[4]);
gret->pVertex[4] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[5]);
gret->pVertex[5] = Target->pNodes[EInd+iSInd];
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object36::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object36* gret = new E_Object36;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv=inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->pVertex[2] = pInVertex[2];
gret->pVertex[3] = pInVertex[3];
gret->pVertex[4] = pInVertex[4];
gret->pVertex[5] = pInVertex[5];
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}
// Draw Object line
void E_Object36::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);

pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->LineTo((int) pVertex[4]->DSP_Point->x,(int) pVertex[4]->DSP_Point->y);
pDC->LineTo((int) pVertex[5]->DSP_Point->x,(int) pVertex[5]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);


pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);

pDC->MoveTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[4]->DSP_Point->x,(int) pVertex[4]->DSP_Point->y);

pDC->MoveTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[5]->DSP_Point->x,(int) pVertex[5]->DSP_Point->y);
}

void E_Object36::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector d[6];
int i;
for (i=0;i<6;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS = 1.0;
dFS = ME->dResFactor;
glLineWidth(2.0);
      float fCols[6]={0,0,0,0,0,0};
      BOOL bD=FALSE;
      int iVar;
      iVar=ME->iCVar;
      //Nodal data
      if ((pVertex[0]->pResV != NULL) &&
          (pVertex[1]->pResV != NULL) &&
          (pVertex[2]->pResV != NULL) &&
          (pVertex[3]->pResV != NULL) &&
          (pVertex[4]->pResV != NULL) &&
          (pVertex[5]->pResV != NULL))
      {
        bD=TRUE;
        fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
        fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
        fCols[2]=GetContourCol(*pVertex[2]->pResV->GetAddress(iVar)*dFS);
        fCols[3]=GetContourCol(*pVertex[3]->pResV->GetAddress(iVar)*dFS);
        fCols[4]=GetContourCol(*pVertex[4]->pResV->GetAddress(iVar)*dFS);
        fCols[5]=GetContourCol(*pVertex[5]->pResV->GetAddress(iVar)*dFS);
      }
      if (pResV!=NULL)
      {
        bD=TRUE;
        fCols[0]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
		fCols[1] = fCols[0];
        fCols[2] = fCols[0];
        fCols[3] = fCols[0];
        fCols[4] = fCols[0];
        fCols[5] = fCols[0];

      }
C3dVector v1;
C3dVector v2;
C3dVector Vn;
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<6;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S * dFS;
       }
    }
  }

	Selectable=1;
  if (((iDspFlgs & DSP_CONT)>0) || (bD==FALSE))
  {
	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glColor3fv(cols[iColour]);
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glEnd();

	  v1.x = pVertex[4]->Pt_Point->x-pVertex[3]->Pt_Point->x;
	  v1.y = pVertex[4]->Pt_Point->y-pVertex[3]->Pt_Point->y;
	  v1.z = pVertex[4]->Pt_Point->z-pVertex[3]->Pt_Point->z;

	  v2.x = pVertex[5]->Pt_Point->x-pVertex[4]->Pt_Point->x;
	  v2.y = pVertex[5]->Pt_Point->y-pVertex[4]->Pt_Point->y;
	  v2.z = pVertex[5]->Pt_Point->z-pVertex[4]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glEnd();

	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[4]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[4]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[4]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();

	  v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	  v2.x = pVertex[5]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v2.y = pVertex[5]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v2.z = pVertex[5]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	  glEnd();

	  v1.x = pVertex[0]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v1.y = pVertex[0]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v1.z = pVertex[0]->Pt_Point->z-pVertex[2]->Pt_Point->z;

	  v2.x = pVertex[3]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v2.y = pVertex[3]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v2.z = pVertex[3]->Pt_Point->z-pVertex[0]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	  glEnd();
  }
  else
  {

   if (bD)
   {
      glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);
	    v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	    v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glEnd();

	    v1.x = pVertex[4]->Pt_Point->x-pVertex[3]->Pt_Point->x;
	    v1.y = pVertex[4]->Pt_Point->y-pVertex[3]->Pt_Point->y;
	    v1.z = pVertex[4]->Pt_Point->z-pVertex[3]->Pt_Point->z;

	    v2.x = pVertex[5]->Pt_Point->x-pVertex[4]->Pt_Point->x;
	    v2.y = pVertex[5]->Pt_Point->y-pVertex[4]->Pt_Point->y;
	    v2.z = pVertex[5]->Pt_Point->z-pVertex[4]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[4]);
	    glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[5]);
	    glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	    glEnd();

	    v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	    v2.x = pVertex[4]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v2.y = pVertex[4]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v2.z = pVertex[4]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
      glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[4]);
	    glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glEnd();

	    v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	    v2.x = pVertex[5]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	    v2.y = pVertex[5]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	    v2.z = pVertex[5]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[5]);
	    glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[4]);
	    glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
	    glEnd();

	    v1.x = pVertex[0]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	    v1.y = pVertex[0]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	    v1.z = pVertex[0]->Pt_Point->z-pVertex[2]->Pt_Point->z;

	    v2.x = pVertex[3]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v2.y = pVertex[3]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v2.z = pVertex[3]->Pt_Point->z-pVertex[0]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[5]);
	    glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
	    glEnd();
      glDisable(GL_TEXTURE_1D);
    }
  }
}
else
{
  Selectable=0;
}
}

void E_Object36::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector d[6];
int i;
int ind;

for (i=0;i<6;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
ind=ME->iCVar;
glLineWidth(1.0);
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<6;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S*dFS;
       }
    }
  }
  Selectable=1;
  if ((iDspFlgs & DSP_LINE) > 0)
  {
    glColor3fv(cols[iColour]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
  glBegin(GL_LINES);
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[4]->Pt_Point->x+d[4].x),(float) (pVertex[4]->Pt_Point->y+d[4].y),(float) (pVertex[4]->Pt_Point->z+d[4].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[5]->Pt_Point->x+d[5].x),(float) (pVertex[5]->Pt_Point->y+d[5].y),(float) (pVertex[5]->Pt_Point->z+d[5].z));
  glEnd();
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
  {
     sprintf_s(sLab,"E%i",iLabel);
     OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
  {
     sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
     OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if ((iDspFlgs & DSP_ELSYS) == 0)
  {
	  C3dMatrix mS = GetElSys();
	  C3dVector vC = Get_Centroid();
	  mS.Transpose();
	  C3dVector vX = mS.GetColVec(1);
	  C3dVector vY = mS.GetColVec(2);;
	  C3dVector vZ = mS.GetColVec(3);;

	  vX *= 0.5*dS1; vY *= 0.5*dS1; vZ *= 0.5*dS1;
	  vX += vC; vY += vC; vZ += vC;

	  glBegin(GL_LINES);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glEnd();
	  glRasterPos3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
	  glRasterPos3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
	  glRasterPos3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }
}
else
{
  Selectable=0;
}
}

void E_Object36::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}


void E_Object36::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s%8i%8i","CPENTA  ",iLabel,PID);
fprintf(pFile,"%8i",pVertex[0]->iLabel);
fprintf(pFile,"%8i",pVertex[1]->iLabel);
fprintf(pFile,"%8i",pVertex[2]->iLabel);
fprintf(pFile,"%8i",pVertex[3]->iLabel);
fprintf(pFile,"%8i",pVertex[4]->iLabel);
fprintf(pFile,"%8i",pVertex[5]->iLabel);
fprintf(pFile,"\n","");
}


double E_Object36::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  double T = 0;
  int iDOFID;
  int j;
  Mat fun;
  Mat FunPnt(1, 3);
  *FunPnt.mn(1, 1) = 0.333;
  *FunPnt.mn(1, 2) = 0.333;
  *FunPnt.mn(1, 3) = 0.0;
  fun = this->ShapeFun(FunPnt, 1);
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T**fun.mn(1, j + 1);
  }
  fun.clear();
  FunPnt.clear();
  return(dTemp);
}

C3dVector E_Object36::Get_Centroid()
{
Mat fun;
Mat FunPnt(1,3);
*FunPnt.mn(1,1)=0.333;
*FunPnt.mn(1,2)=0.333;
*FunPnt.mn(1,3)=0.0;
fun=ShapeFun(FunPnt,1);
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}

fun.clear();
FunPnt.clear();
return (vT);
}

C3dMatrix E_Object36::GetElSys()
{
	C3dVector vX;
	C3dVector vZ;
	C3dVector vY;
	vX = pVertex[1]->GetCoords();
	vX -= pVertex[0]->GetCoords();
	vX.Normalize();
	vZ = pVertex[3]->GetCoords();
	vZ -= pVertex[0]->GetCoords();  //Approx z direction
	vZ.Normalize();
	vY = vZ.Cross(vX);
	vY.Normalize();
	vZ = vX.Cross(vY);
	C3dMatrix vR;
	vR.SetColVec(1, vX);
	vR.SetColVec(2, vY);
	vR.SetColVec(3, vZ);
	vR.Transpose();
	return (vR);
}

G_Object* E_Object36::GetNode(int i)
{
return (pVertex[i]);
}


int E_Object36::GetLinkList(cLink* Links[200])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
Links[0]->iColour=iColour;
Links[1]=new cLink;
Links[1]->pParent=this;
Links[1]->pVertex[0]=pVertex[1];
Links[1]->pVertex[1]=pVertex[2];
Links[1]->iColour=iColour;
Links[2]=new cLink;
Links[2]->pParent=this;
Links[2]->pVertex[0]=pVertex[2];
Links[2]->pVertex[1]=pVertex[0];
Links[2]->iColour=iColour;
Links[3]=new cLink;
Links[3]->pParent=this;
Links[3]->pVertex[0]=pVertex[3];
Links[3]->pVertex[1]=pVertex[4];
Links[3]->iColour=iColour;

Links[4]=new cLink;
Links[4]->pParent=this;
Links[4]->pVertex[0]=pVertex[4];
Links[4]->pVertex[1]=pVertex[5];
Links[4]->iColour=iColour;
Links[5]=new cLink;
Links[5]->pParent=this;
Links[5]->pVertex[0]=pVertex[5];
Links[5]->pVertex[1]=pVertex[3];
Links[5]->iColour=iColour;
Links[6]=new cLink;
Links[6]->pParent=this;
Links[6]->pVertex[0]=pVertex[0];
Links[6]->pVertex[1]=pVertex[3];
Links[6]->iColour=iColour;
Links[7]=new cLink;
Links[7]->pParent=this;
Links[7]->pVertex[0]=pVertex[1];
Links[7]->pVertex[1]=pVertex[4];
Links[7]->iColour=iColour;

Links[8]=new cLink;
Links[8]->pParent=this;
Links[8]->pVertex[0]=pVertex[2];
Links[8]->pVertex[1]=pVertex[5];
Links[8]->iColour=iColour;
return (9);
}
int E_Object36::GetfaceList(cFace* Faces[6])
{
int ic;
ic=GetCol();
Faces[0]=new cFace();
Faces[0]->pParent=this;
Faces[0]->pVertex[0]=pVertex[0];
Faces[0]->pVertex[1]=pVertex[1];
Faces[0]->pVertex[2]=pVertex[2];
Faces[0]->NoVert=3;
Faces[0]->iColour=ic;

Faces[1]=new cFace();
Faces[1]->pParent=this;
Faces[1]->pVertex[0]=pVertex[3];
Faces[1]->pVertex[1]=pVertex[4];
Faces[1]->pVertex[2]=pVertex[5];
Faces[1]->NoVert=3;
Faces[1]->iColour=ic;

Faces[2]=new cFace();
Faces[2]->pParent=this;
Faces[2]->pVertex[0]=pVertex[1];
Faces[2]->pVertex[1]=pVertex[2];
Faces[2]->pVertex[2]=pVertex[5];
Faces[2]->pVertex[3]=pVertex[4];
Faces[2]->NoVert=4;
Faces[2]->iColour=ic;

Faces[3]=new cFace();
Faces[3]->pParent=this;
Faces[3]->pVertex[0]=pVertex[0];
Faces[3]->pVertex[1]=pVertex[3];
Faces[3]->pVertex[2]=pVertex[5];
Faces[3]->pVertex[3]=pVertex[2];
Faces[3]->NoVert=4;
Faces[3]->iColour=ic;

Faces[4]=new cFace();
Faces[4]->pParent=this;
Faces[4]->pVertex[0]=pVertex[0];
Faces[4]->pVertex[1]=pVertex[1];
Faces[4]->pVertex[2]=pVertex[4];
Faces[4]->pVertex[3]=pVertex[3];
Faces[4]->NoVert=4;
Faces[4]->iColour=ic;
return (5);
}



int E_Object36::noDof()
{
return(3);
}

Vec<int> E_Object36::GetSteerVec3d()
{
Vec<int> V(6*3);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[1]->dof[0];
*V.nn(5)=pVertex[1]->dof[1];
*V.nn(6)=pVertex[1]->dof[2];
*V.nn(7)=pVertex[2]->dof[0];
*V.nn(8)=pVertex[2]->dof[1];
*V.nn(9)=pVertex[2]->dof[2];
*V.nn(10)=pVertex[3]->dof[0];
*V.nn(11)=pVertex[3]->dof[1];
*V.nn(12)=pVertex[3]->dof[2];
*V.nn(13)=pVertex[4]->dof[0];
*V.nn(14)=pVertex[4]->dof[1];
*V.nn(15)=pVertex[4]->dof[2];
*V.nn(16)=pVertex[5]->dof[0];
*V.nn(17)=pVertex[5]->dof[1];
*V.nn(18)=pVertex[5]->dof[2];
return(V);
}

Vec<int> E_Object36::GetSteerVec1d()
{
Vec<int> V(6*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
*V.nn(3)=pVertex[2]->dof[0];
*V.nn(4)=pVertex[3]->dof[0];
*V.nn(5)=pVertex[4]->dof[0];
*V.nn(6)=pVertex[5]->dof[0];
return(V);
}


int E_Object36::MaxBW()
{
int i;
int j;
int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<6;j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	{
       MaxDof=pVertex[i]->dof[j];
    }
	if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	{
       MinDof=pVertex[i]->dof[j];
	}
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}

Mat E_Object36::getCoords3d()
{
int i;
Mat coord(iNoNodes,3);
for (i=0;i<iNoNodes;i++)
{
  *coord.mn(i+1, 1) = pVertex[i]->Pt_Point->x;
  *coord.mn(i+1, 2) = pVertex[i]->Pt_Point->y;
  *coord.mn(i+1, 3) = pVertex[i]->Pt_Point->z;
}
return (coord);
}

Mat E_Object36::Sample(int iNo)
{
Mat Pts(iNo,4);
if (iNo ==6)
{
  double r16 = 1.0/6.0;
  double r23 = 2.0/3.0;
  double weight = 1.0/6.0;   //unknown at moment
  *Pts.mn(1,1)=r16;*Pts.mn(1,2)=r16;*Pts.mn(1,3)=-0.5;*Pts.mn(1,4)=weight;
  *Pts.mn(2,1)=r23;*Pts.mn(2,2)=r16;*Pts.mn(2,3)=-0.5;*Pts.mn(2,4)=weight;
  *Pts.mn(3,1)=r16;*Pts.mn(3,2)=r23;*Pts.mn(3,3)=-0.5;*Pts.mn(3,4)=weight;
  *Pts.mn(4,1)=r16;*Pts.mn(4,2)=r16;*Pts.mn(4,3)=0.5;*Pts.mn(4,4)=weight;
  *Pts.mn(5,1)=r23;*Pts.mn(5,2)=r16;*Pts.mn(5,3)=0.5;*Pts.mn(5,4)=weight;
  *Pts.mn(6,1)=r16;*Pts.mn(6,2)=r23;*Pts.mn(6,3)=0.5;*Pts.mn(6,4)=weight;
}
return (Pts);
}

Mat E_Object36::ShapeFun(Mat Points, int i)
{
Mat fun(1,6);
double xi,eta,zeta;
double r,s,t;
eta = *Points.mn(i, 2);
xi = *Points.mn(i, 1);
zeta = *Points.mn(i, 3);
r=xi;
s=eta;
t=1-xi-eta;

*fun.mn(1,1) = 0.5*(1-zeta)*r;
*fun.mn(1,2) = 0.5*(1-zeta)*s;
*fun.mn(1,3) = 0.5*(1-zeta)*t;
*fun.mn(1,4) = 0.5*(1+zeta)*r;
*fun.mn(1,5) = 0.5*(1+zeta)*s;
*fun.mn(1,6) = 0.5*(1+zeta)*t;

return(fun);
}

Mat E_Object36::ShapeDer(Mat Points, int i)
{
Mat der(3,6);
double xi,eta,zeta;
double r,s,t;
eta = *Points.mn(i, 2);
xi = *Points.mn(i, 1);
zeta = *Points.mn(i, 3);
r=xi;
s=eta;
t=1-xi-eta;
*der.mn(1,1) =  0.5*(1-zeta);
*der.mn(1,2) =  0;
*der.mn(1,3) = -0.5*(1-zeta);
*der.mn(1,4) =  0.5*(1+zeta);
*der.mn(1,5) =  0;
*der.mn(1,6) =  -0.5*(1+zeta);

*der.mn(2,1) =  0;
*der.mn(2,2) =  0.5*(1-zeta);
*der.mn(2,3) = -0.5*(1-zeta);
*der.mn(2,4) =  0;
*der.mn(2,5) =  0.5*(1+zeta);
*der.mn(2,6) = -0.5*(1+zeta);

*der.mn(3,1) = -0.5*r;
*der.mn(3,2) = -0.5*s;
*der.mn(3,3) = -0.5*t;
*der.mn(3,4) =  0.5*r;
*der.mn(3,5) =  0.5*s;
*der.mn(3,6) =  0.5*t;

return(der);
}

CString E_Object36::GetName()
{
	return ("6 Node WEDGE (CPENTA)");
}

int E_Object36::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "Mat ID";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	sVar[iNo] = "N3";
	iNo++;
	sVar[iNo] = "N4";
	iNo++;
	sVar[iNo] = "N5";
	iNo++;
	sVar[iNo] = "N6";
	iNo++;
	return(iNo);
}


int E_Object36::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iMatID);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[2]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[3]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[4]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[5]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object36::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{
	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;

	int newPID = atoi(sVar[0]);
	if (newPID != PID)
	{

		Property* pr = PT->GetItem(newPID);
		if (pr != NULL)
		{
			BOOL bC = SetProperty(pr);
			if (bC)
				outtext1("Property has Been Changed");
			else
				outtext1("Invalid Property");
		}

	}

	int N1 = atoi(sVar[2]);
	int N2 = atoi(sVar[3]);
	int N3 = atoi(sVar[4]);
	int N4 = atoi(sVar[5]);
	int N5 = atoi(sVar[6]);
	int N6 = atoi(sVar[7]);
	if (pVertex[0]->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex[0] = pN;

	}
	if (pVertex[1]->iLabel != N2)
	{
		pN = pMe->GetNode(N2);
		if (pN != NULL)
			pVertex[1] = pN;

	}
	if (pVertex[2]->iLabel != N3)
	{
		pN = pMe->GetNode(N3);
		if (pN != NULL)
			pVertex[2] = pN;

	}
	if (pVertex[3]->iLabel != N4)
	{
		pN = pMe->GetNode(N4);
		if (pN != NULL)
			pVertex[3] = pN;
	}
	if (pVertex[4]->iLabel != N5)
	{
		pN = pMe->GetNode(N5);
		if (pN != NULL)
			pVertex[4] = pN;
	}
	if (pVertex[5]->iLabel != N6)
	{
		pN = pMe->GetNode(N6);
		if (pN != NULL)
			pVertex[5] = pN;
	}
}

//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object34, CObject )

E_Object34::~E_Object34()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
}

void E_Object34::Info()
{
  char S1[80];
  E_Object::Info();
  sprintf_s(S1,"NODES %i %i %i %i",pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel,pVertex[3]->iLabel);
  outtext1(S1); 
}

void E_Object34::Create(Pt_Object* pInVertex[100], int iLab, int iCol, int iType, int iPID, int iMat, int iNo, G_Object* Parrent, Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<4;i++)
{
  pVertex[i] = pInVertex[i];
}

}

//Tetrahedron collapse calculation quality check
//min all combiations of (h*1.24/A) altairs definition
//http://www.altairuniversity.com/wp-content/uploads/2014/02/elemquality.pdf
double E_Object34::height(int n1, int n2, int n3, int p)
{
  C3dVector v1;
  C3dVector v2;
  C3dVector v3;
  C3dVector v;
  C3dVector w;
  C3dVector n;
  C3dVector vPt;
  double a;
  //Nornal to plain
  v1 = this->pVertex[n1]->Get_Centroid();
  v2 = this->pVertex[n2]->Get_Centroid();
  v3 = this->pVertex[n3]->Get_Centroid();
  vPt = this->pVertex[p]->Get_Centroid();
  v = v2 - v1;
  w = v3 - v1;
  n = v.Cross(w);
  n.Normalize();
  vPt -= v1;
  a = abs(n.Dot(vPt));
  return (a);
}

double E_Object34::longEdge()
{
  C3dVector v0;
  C3dVector v1;
  C3dVector v2;
  C3dVector v3;
  C3dVector w;
  double a;
  double dLen;
  v0 = this->pVertex[0]->Get_Centroid();
  v1 = this->pVertex[1]->Get_Centroid();
  v2 = this->pVertex[2]->Get_Centroid();
  v3 = this->pVertex[3]->Get_Centroid();
  a = (v0 - v1).Mag();
  dLen = a;
  a = (v1 - v2).Mag();
  if (a > dLen)
    dLen = a;
  a = (v2 - v0).Mag();
  if (a > dLen)
    dLen = a;
  
  a = (v1 - v3).Mag();
  if (a > dLen)
    dLen = a;
  a = (v2 - v3).Mag();
  if (a > dLen)
    dLen = a;
  a = (v0 - v3).Mag();
  if (a > dLen)
    dLen = a;

  return (a);
}

double E_Object34::area(int n1, int n2, int n3)
{
  C3dVector v1;
  C3dVector v2;
  C3dVector v3;
  C3dVector v;
  C3dVector w;
  double a;
  v1 = this->pVertex[n1]->Get_Centroid();
  v2 = this->pVertex[n2]->Get_Centroid();
  v3 = this->pVertex[n3]->Get_Centroid();
  v = v2 - v1;
  w = v3 - v1;
  a = (v.Cross(w).Mag())*0.5;
  return (a);
}

double E_Object34::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  double T = 0;
  int iDOFID;
  int j;
  Mat fun;
  Mat FunPnt(1, 4);
  *FunPnt.mn(1, 1) = 0.25;
  *FunPnt.mn(1, 2) = 0.25;
  *FunPnt.mn(1, 3) = 0.25;
  *FunPnt.mn(1, 4) = 0.25;   //redundanta 
  fun = this->ShapeFun(FunPnt, 1);
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T**fun.mn(1, j + 1);
  }
  fun.clear();
  FunPnt.clear();
  return(dTemp);
}

double E_Object34::TetCollapse()
{
  double c0;
  double c1;
  double c2;
  double c3;
  double dm;

  c0 = height(0, 1, 2, 3);
  c1 = height(1, 2, 3, 0);
  c2 = height(0, 2, 3, 1);
  c3 = height(0, 1, 3, 2);
  dm = c0;
  if (c0 < dm)
    dm = c0;
  if (c1 < dm)
    dm = c1;
  if (c2 < dm)
    dm = c2;
  if (c3 < dm)
    dm = c3;

  dm = (dm*1.2247) / longEdge();

  return (dm);
}



void E_Object34::GetBoundingBox(C3dVector& vll,C3dVector& vur)
{
  int i=0;
  vll.x=pVertex[i]->Pt_Point->x; vll.y=pVertex[i]->Pt_Point->y; vll.z=pVertex[i]->Pt_Point->z;
  for(i=0;i<4;i++)
  {
    if (pVertex[i]->Pt_Point->x<vll.x)
      vll.x=pVertex[i]->Pt_Point->x;
	if (pVertex[i]->Pt_Point->x>vur.x)
      vur.x=pVertex[i]->Pt_Point->x;

	if (pVertex[i]->Pt_Point->y<vll.y)
      vll.y=pVertex[i]->Pt_Point->y;
	if (pVertex[i]->Pt_Point->y>vur.y)
      vur.y=pVertex[i]->Pt_Point->y;

	if (pVertex[i]->Pt_Point->z<vll.z)
      vll.z=pVertex[i]->Pt_Point->z;
	if (pVertex[i]->Pt_Point->z>vur.z)
      vur.z=pVertex[i]->Pt_Point->z;
  }
}

double E_Object34::GetTETHeight(C3dVector vFCent)
{
double dS;
C3dVector v1;
C3dVector v2;
C3dVector A;
C3dVector B;
C3dVector H;
A=this->pVertex[1]->Get_Centroid();
v2=this->pVertex[0]->Get_Centroid();
A-=v2;
A.Normalize();
B=this->pVertex[1]->Get_Centroid();
v2=this->pVertex[2]->Get_Centroid();
B-=v2;
B.Normalize();
H=B.Cross(A);
H.Normalize();

B=this->pVertex[3]->Get_Centroid();
B-=vFCent;
dS=B.Dot(H);
return (dS);
}

double E_Object34::GetCharSize()
{
double dS;
double dMax;

C3dVector vT;
C3dVector vT2;
vT=pVertex[3]->Pt_Point;
vT2=pVertex[0]->Pt_Point;
vT-=vT2;
dS=vT.Mag();
dMax=dS;
vT=pVertex[3]->Pt_Point;
vT2=pVertex[1]->Pt_Point;
vT-=vT2;
dS=vT.Mag();
if (dS>dMax)
  dMax=dS;
vT=pVertex[3]->Pt_Point;
vT2=pVertex[2]->Pt_Point;
vT-=vT2;
dS=vT.Mag();
if (dS>dMax)
  dMax=dS;


//dS=(pVertex[0]->Pt_Point-pVertex[1]->Pt_Point)->Mag();
return (dMax);
}

void E_Object34::Reverse()
{
Pt_Object* pT[8];
pT[0]=pVertex[0];
pT[1]=pVertex[1];
pT[2]=pVertex[2];
pT[3]=pVertex[3];
pVertex[0] = pT[0];
pVertex[1] = pT[2];
pVertex[2] = pT[1];
pVertex[3] = pT[3];
}


BOOL E_Object34::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object34::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

G_Object* E_Object34::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object34* gret = new E_Object34;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pVertex[3] = MESH->GetNode(pVertex[3]->iLabel);
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object34::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object34* gret = new E_Object34;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[3]);
gret->pVertex[3] = Target->pNodes[EInd+iSInd];
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object34::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object34* gret = new E_Object34;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv= inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->pVertex[2] = pInVertex[2];
gret->pVertex[3] = pInVertex[3];
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

void E_Object34::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
    ar << pVertex[3]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);    
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[3] = MESH->GetNode(iNd);
	}
}

int E_Object34::GetLinkList(cLink* Links[200])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
Links[0]->iColour=iColour;
Links[1]=new cLink;
Links[1]->pParent=this;
Links[1]->pVertex[0]=pVertex[1];
Links[1]->pVertex[1]=pVertex[2];
Links[1]->iColour=iColour;
Links[2]=new cLink;
Links[2]->pParent=this;
Links[2]->pVertex[0]=pVertex[2];
Links[2]->pVertex[1]=pVertex[0];
Links[2]->iColour=iColour;
Links[3]=new cLink;
Links[3]->pParent=this;
Links[3]->pVertex[0]=pVertex[0];
Links[3]->pVertex[1]=pVertex[3];
Links[3]->iColour=iColour;
Links[4]=new cLink;
Links[4]->pParent=this;
Links[4]->pVertex[0]=pVertex[1];
Links[4]->pVertex[1]=pVertex[3];
Links[4]->iColour=iColour;
Links[5]=new cLink;
Links[5]->pParent=this;
Links[5]->pVertex[0]=pVertex[2];
Links[5]->pVertex[1]=pVertex[3];
Links[5]->iColour=iColour;
return (6);
}



// Draw Object line
void E_Object34::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);


pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);

pDC->MoveTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->MoveTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);

}

void E_Object34::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector d[4];
int i;
int ind;

for (i=0;i<4;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
ind=ME->iCVar;
glLineWidth(1.0);
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<4;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S*dFS;
       }
    }
  }
  Selectable=1;
  if ((iDspFlgs & DSP_LINE) > 0)
  {
    glColor3fv(cols[iColour]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
  glBegin(GL_LINES);
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
  glEnd();
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
  {
	sprintf_s(sLab,"E%i",iLabel);
	OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
  {
    sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
    OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if ((iDspFlgs & DSP_ELSYS) == 0)
  {
	  C3dMatrix mS = GetElSys();
	  C3dVector vC = Get_Centroid();
	  mS.Transpose();
	  C3dVector vX = mS.GetColVec(1);
	  C3dVector vY = mS.GetColVec(2);;
	  C3dVector vZ = mS.GetColVec(3);;

	  vX *= 0.5*dS1; vY *= 0.5*dS1; vZ *= 0.5*dS1;
	  vX += vC; vY += vC; vZ += vC;

	  glBegin(GL_LINES);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glVertex3f((float)vC.x, (float)vC.y, (float)vC.z);
	  glVertex3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glEnd();
	  glRasterPos3f((float)vX.x, (float)vX.y, (float)vX.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
	  glRasterPos3f((float)vY.x, (float)vY.y, (float)vY.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
	  glRasterPos3f((float)vZ.x, (float)vZ.y, (float)vZ.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }
}
else
{
  Selectable=0;
}
}


void E_Object34::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector d[4];
int i;
for (i=0;i<4;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
glLineWidth(2);
    float fCols[4]={0,0,0,0};
    BOOL bD=FALSE;
    int iVar;
    iVar=ME->iCVar;
    //Nodal data
    if ((pVertex[0]->pResV != NULL) &&
        (pVertex[1]->pResV != NULL) &&
        (pVertex[2]->pResV != NULL) &&
        (pVertex[3]->pResV != NULL))
    {
      bD=TRUE;
      fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
      fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
      fCols[2]=GetContourCol(*pVertex[2]->pResV->GetAddress(iVar)*dFS);
      fCols[3]=GetContourCol(*pVertex[3]->pResV->GetAddress(iVar)*dFS);
    }
    if (pResV!=NULL)
    {
      bD=TRUE;
      fCols[0]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
	  fCols[1] = fCols[0];
	  fCols[2] = fCols[0];
	  fCols[3] = fCols[0];
    }

C3dVector v1;
C3dVector v2;
C3dVector Vn;
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<4;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S*dFS;
       }
    }
  }
	Selectable=1;
	glColor3fv(cols[iColour]);
  if (((iDspFlgs & DSP_CONT)>0) || (bD==FALSE))
  {
	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glEnd();

	  v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[3]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v2.y = pVertex[3]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v2.z = pVertex[3]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();

	  v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	  v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	  v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	  v2.x = pVertex[3]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v2.y = pVertex[3]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v2.z = pVertex[3]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();

	  v1.x = pVertex[2]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	  v1.y = pVertex[2]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	  v1.z = pVertex[2]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	  v2.x = pVertex[3]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	  v2.y = pVertex[3]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	  v2.z = pVertex[3]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	  Vn = v1.Cross(v2);

	  Vn.Normalize();
	  glBegin(GL_POLYGON);
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	  glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	  glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	  glEnd();
  }
  else
  {

    if (bD)
    {
      glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);
	    v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	    v2.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v2.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v2.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);

	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glEnd();

	    v1.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v1.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v1.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	    v2.x = pVertex[3]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v2.y = pVertex[3]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v2.z = pVertex[3]->Pt_Point->z-pVertex[1]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glEnd();

	    v1.x = pVertex[2]->Pt_Point->x-pVertex[1]->Pt_Point->x;
	    v1.y = pVertex[2]->Pt_Point->y-pVertex[1]->Pt_Point->y;
	    v1.z = pVertex[2]->Pt_Point->z-pVertex[1]->Pt_Point->z;

	    v2.x = pVertex[3]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	    v2.y = pVertex[3]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	    v2.z = pVertex[3]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[1]);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glEnd();

	    v1.x = pVertex[2]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	    v1.y = pVertex[2]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	    v1.z = pVertex[2]->Pt_Point->z-pVertex[0]->Pt_Point->z;

	    v2.x = pVertex[3]->Pt_Point->x-pVertex[2]->Pt_Point->x;
	    v2.y = pVertex[3]->Pt_Point->y-pVertex[2]->Pt_Point->y;
	    v2.z = pVertex[3]->Pt_Point->z-pVertex[2]->Pt_Point->z;
	    Vn = v1.Cross(v2);

	    Vn.Normalize();
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[0]);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[2]);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+d[2].x),(float) (pVertex[2]->Pt_Point->y+d[2].y),(float) (pVertex[2]->Pt_Point->z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
        glTexCoord1f(fCols[3]);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+d[3].x),(float) (pVertex[3]->Pt_Point->y+d[3].y),(float) (pVertex[3]->Pt_Point->z+d[3].z));
	    glEnd();
      glDisable(GL_TEXTURE_1D);
    }
  }
}
else
{
  Selectable=0;
}
}

G_Object* E_Object34::GetNode(int i)
{
return (pVertex[i]);
}

C3dVector E_Object34::GetNodalCoords(int i)
{
	C3dVector v;
	v.x = pVertex[i]->Pt_Point->x;
	v.y = pVertex[i]->Pt_Point->y;
	v.z = pVertex[i]->Pt_Point->z;
	return (v);
}

int E_Object34::GetfaceList(cFace* Faces[6])
{
int ic;
ic=GetCol();
Faces[0]=new cFace();
Faces[0]->pParent=this;
Faces[0]->pVertex[0]=pVertex[0];
Faces[0]->pVertex[1]=pVertex[2];
Faces[0]->pVertex[2]=pVertex[1];
Faces[0]->NoVert=3;
Faces[0]->iColour=iColour;

Faces[1]=new cFace();
Faces[1]->pParent=this;
Faces[1]->pVertex[0]=pVertex[0];
Faces[1]->pVertex[1]=pVertex[1];
Faces[1]->pVertex[2]=pVertex[3];
Faces[1]->NoVert=3;
Faces[1]->iColour=iColour;

Faces[2]=new cFace;
Faces[2]->pParent=this;
Faces[2]->pVertex[0]=pVertex[1];
Faces[2]->pVertex[1]=pVertex[2];
Faces[2]->pVertex[2]=pVertex[3];
Faces[2]->NoVert=3;
Faces[2]->iColour=iColour;

Faces[3]=new cFace;
Faces[3]->pParent=this;
Faces[3]->pVertex[0]=pVertex[0];
Faces[3]->pVertex[1]=pVertex[3];
Faces[3]->pVertex[2]=pVertex[2];
Faces[3]->NoVert=3;
Faces[3]->iColour=iColour;
return (4);
}

void E_Object34::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s%8i%8i","CTETRA  ",iLabel,PID);
fprintf(pFile,"%8i",pVertex[0]->iLabel);
fprintf(pFile,"%8i",pVertex[1]->iLabel);
fprintf(pFile,"%8i",pVertex[2]->iLabel);
fprintf(pFile,"%8i",pVertex[3]->iLabel);
fprintf(pFile,"\n","");
}

void E_Object34::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}




C3dVector E_Object34::Get_Centroid()
{
Mat fun;
Mat FunPnt(1,4);
*FunPnt.mn(1,1)=0.25;
*FunPnt.mn(1,2)=0.25;
*FunPnt.mn(1,3)=0.25;
fun=ShapeFun(FunPnt,1);
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}
fun.clear();
FunPnt.clear();
return (vT);
}


C3dMatrix E_Object34::GetElSys()
{
	C3dVector vX;
	C3dVector vZ;
	C3dVector vY;
	vX = pVertex[1]->GetCoords();
	vX -= pVertex[0]->GetCoords();
	vX.Normalize();
	vZ = pVertex[3]->GetCoords();
	vZ -= pVertex[0]->GetCoords();  //Approx z direction
	vZ.Normalize();
	vY = vZ.Cross(vX);
	vY.Normalize();
	vZ = vX.Cross(vY);
	C3dMatrix vR;
	vR.SetColVec(1, vX);
	vR.SetColVec(2, vY);
	vR.SetColVec(3, vZ);
	vR.Transpose();
	return (vR);
}

Mat E_Object34::getCoords3d()
{
int i;
Mat coord(iNoNodes,3);
for (i=0;i<iNoNodes;i++)
{
  *coord.mn(i+1, 1) = pVertex[i]->Pt_Point->x;
  *coord.mn(i+1, 2) = pVertex[i]->Pt_Point->y;
  *coord.mn(i+1, 3) = pVertex[i]->Pt_Point->z;
}
return (coord);
}

Mat E_Object34::Sample(int iNo)
{
Mat Pts(iNo,4);
if (iNo==1)
{
*Pts.mn(1,1) = 0.25;
*Pts.mn(1,2) = 0.25;   
*Pts.mn(1,3) = 0.25;   
*Pts.mn(1,4) = 1.0/6.0;
}
else if (iNo==4)
{
	double coord1 = 0.585410196624968;
	double coord2 = 0.138196601125010;
	//the weight is divided by 6 because the determinant gives the volume of a
	//qube and a tetraedron has a 6 times smaller volume (0.25/6.0 = 0.04166666) 
	double weight = 0.0416666666666666;
	*Pts.mn(1,1) = coord1; *Pts.mn(1,2) = coord2; *Pts.mn(1,3) = coord2; *Pts.mn(1,4) = weight;
	*Pts.mn(2,1) = coord2; *Pts.mn(2,2) = coord1; *Pts.mn(2,3) = coord2; *Pts.mn(2,4) = weight;
	*Pts.mn(3,1) = coord2; *Pts.mn(3,2) = coord2; *Pts.mn(3,3) = coord1; *Pts.mn(3,4) = weight;
	*Pts.mn(4,1) = coord2; *Pts.mn(4,2) = coord2; *Pts.mn(4,3) = coord2; *Pts.mn(4,4) = weight;
}
return (Pts);
}



Mat E_Object34::ShapeFun(Mat Points, int i)
{
Mat fun(1,4);
double v1,v2,v3,v4;

v2=*Points.mn(i,1);
v3=*Points.mn(i,2);
v4=*Points.mn(i,3);
v1=(1-v2-v3-v4);

//v1=*Points.mn(i,1);  This how it was in reverse to nastran
//v2=*Points.mn(i,2);
//v3=*Points.mn(i,3);
//v4=(1-v1-v2-v3);
*fun.mn(1,1)=v1;
*fun.mn(1,2)=v2;
*fun.mn(1,3)=v3;
*fun.mn(1,4)=v4;
return(fun);
}

int E_Object34::noDof()
{
return(3);
}

Mat E_Object34::ShapeDer(Mat Points, int i)
{
Mat der(3,4);
*der.mn(1,1) = -1;
*der.mn(1,2) = 1;
*der.mn(1,3) = 0;
*der.mn(1,4) = 0;

*der.mn(2,1) = -1;
*der.mn(2,2) = 0;
*der.mn(2,3) = 1;
*der.mn(2,4) = 0;

*der.mn(3,1) = -1;
*der.mn(3,2) = 0;
*der.mn(3,3) = 0;
*der.mn(3,4) = 1;


//*der.mn(1,1) = 1;  //WAS this way
//*der.mn(1,2) = 0;
//*der.mn(1,3) = 0;
//*der.mn(1,4) = -1;
//
//*der.mn(2,1) = 0;
//*der.mn(2,2) = 1;
//*der.mn(2,3) = 0;
//*der.mn(2,4) = -1;
//
//*der.mn(3,1) = 0;
//*der.mn(3,2) = 0;
//*der.mn(3,3) = 1;
//*der.mn(3,4) = -1;
return(der);
}

int E_Object34::MaxBW()
{
int i;
int j;
int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<6;j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	{
       MaxDof=pVertex[i]->dof[j];
    }
	if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	{
       MinDof=pVertex[i]->dof[j];
	}
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}

Vec<int> E_Object34::GetSteerVec3d()
{
Vec<int> V(4*3);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[1]->dof[0];
*V.nn(5)=pVertex[1]->dof[1];
*V.nn(6)=pVertex[1]->dof[2];
*V.nn(7)=pVertex[2]->dof[0];
*V.nn(8)=pVertex[2]->dof[1];
*V.nn(9)=pVertex[2]->dof[2];
*V.nn(10)=pVertex[3]->dof[0];
*V.nn(11)=pVertex[3]->dof[1];
*V.nn(12)=pVertex[3]->dof[2];
return(V);
}

Vec<int> E_Object34::GetSteerVec1d()
{
Vec<int> V(4*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
*V.nn(3)=pVertex[2]->dof[0];
*V.nn(4)=pVertex[3]->dof[0];
return(V);
}

CString E_Object34::GetName()
{
	return ("4 Node TET (CTETRA)");
}

int E_Object34::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "Mat ID";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	sVar[iNo] = "N3";
	iNo++;
	sVar[iNo] = "N4";
	iNo++;
	return(iNo);
}


int E_Object34::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iMatID);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[2]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[3]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object34::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;

	int newPID = atoi(sVar[0]);
	if (newPID != PID)
	{

		Property* pr = PT->GetItem(newPID);
		if (pr != NULL)
		{
			BOOL bC = SetProperty(pr);
			if (bC)
				outtext1("Property has Been Changed");
			else
				outtext1("Invalid Property");
		}

	}

	int N1 = atoi(sVar[2]);
	int N2 = atoi(sVar[3]);
	int N3 = atoi(sVar[4]);
	int N4 = atoi(sVar[5]);
	if (pVertex[0]->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex[0] = pN;

	}
	if (pVertex[1]->iLabel != N2)
	{
		pN = pMe->GetNode(N2);
		if (pN != NULL)
			pVertex[1] = pN;

	}
	if (pVertex[2]->iLabel != N3)
	{
		pN = pMe->GetNode(N3);
		if (pN != NULL)
			pVertex[2] = pN;

	}
	if (pVertex[3]->iLabel != N4)
	{
		pN = pMe->GetNode(N4);
		if (pN != NULL)
			pVertex[3] = pN;

	}
}

IMPLEMENT_DYNAMIC(BCLD, CObject )
void BCLD::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
	}
	else
	{
    G_Object::Serialize(ar,iV);
	}
}

void BCLD::ExportNAS(FILE* pFile)
{

}

void BCLD::RelTo(G_Object* pThis,ObjList* pList,int iType)
{
  //return the parent (SET) 
  if (pThis->pParent->iObjType==iType)
  {
    if (pThis->pParent!=NULL)
    {
      pList->AddEx(pThis->pParent);
    }
  }
  //return node related to this force
  if (this->pObj->iObjType==iType)
  {
      pList->AddEx(this->pObj);
  }
}

BOOL BCLD::NodeIn(Pt_Object* pN)
{
BOOL brc=FALSE;
if (pObj==pN)
{
  brc=TRUE;
}
return (brc);
}
//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object, CObject )

E_Object::E_Object()
{
dTemp=0;

}

void E_Object::Create( int iLab,int iCol,int iType, int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
	this->Drawn = 0;
	this->Selectable  = 1;
	this->Visable  = 1;
	this->iObjType = 3;
	this->iType = iType;
	this->iLabel = iLab;
	this->iColour = iCol;
	this->PID=iPID;
	this->iMatID = iMat;
	this->iNoNodes = iNo;
	this->pParent=Parrent;
	this->pPr=inPr;
	this->PIDunv=PIDunv;
	this->pResV=NULL;
}

Mat E_Object::Sample(int iNo)
{
Mat M(0,0);
return(M);
}

BOOL E_Object::NodeInEl(Pt_Object* pN)
{
return (FALSE);
}

void E_Object::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

}

int E_Object::noDof()
{
return(3);
}

Mat E_Object::ShapeDer(Mat Points, int i)
{
Mat M(0,0);
return(M);
}

Mat E_Object::ShapeFun(Mat Points, int i)
{
Mat M(0,0);
return(M);
}

Mat E_Object::getCoords3d()
{
Mat M(0,0);
return(M);
}

Mat E_Object::getCoords2d()
{
Mat M(0,0);
return(M);
}


Mat E_Object::bmat2d(Mat& coord,
                   Mat& deriv)
{
Mat M(0,0);
return(M);
}

int E_Object::GetfaceList(cFace* Faces[6])
{
return (0);
}

int E_Object::GetLinkList(cLink* Links[200])
{
return (0);
}

G_Object* E_Object::GetNode(int i)
{
return (NULL);
}

C3dVector E_Object::GetNodalCoords(int i)
{
	C3dVector v;
	return (v);
}


void E_Object::Transform(C3dMatrix TMat)
{

}

CString E_Object::GetDOFString(int iDOF)
{
  char sDOF[8];
  CString sRet;
  sDOF[0]=' ';
  sDOF[1]=' ';
  sDOF[2]=' ';
  sDOF[3]=' ';
  sDOF[4]=' ';
  sDOF[5]=' ';
  int iN=0;
  if (iDOF& DOF_1)
  {
    sDOF[iN]='1';
    iN++;
  }
  if (iDOF & DOF_2)
  {
    sDOF[iN]='2';
    iN++;
  }
  if (iDOF & DOF_3)
  {
    sDOF[iN]='3';
    iN++;
  }
  if (iDOF & DOF_4)
  {
    sDOF[iN]='4';
    iN++;
  }
  if (iDOF & DOF_5)
  {
    sDOF[iN]='5';
    iN++;
  }
  if (iDOF & DOF_6)
  {
    sDOF[iN]='6';
    iN++;
  }
  sDOF[iN]=NULL;
  sRet=sDOF;
  return (sRet);
}

void E_Object::Reverse()
{

}

C3dVector  E_Object:: GetFirstEdge()
{
C3dVector V(0,0,1);
return (V);
}

double E_Object::QualAspect()
{
  return (0.0);
}

double E_Object::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  return(0);
}


C3dMatrix E_Object::GetElSys()
{
C3dMatrix vR;
vR.MakeUnit();
return (vR);
}

int E_Object::GetDOFInt(CString sDOF)
{
  int iDOF=0;
  if (sDOF.GetLength()>0)
  {
    if (sDOF.Find('1')>-1)
    {
       iDOF=iDOF^DOF_1;
    }
    if (sDOF.Find('2')>-1)
    {
       iDOF=iDOF^DOF_2;
    }
    if (sDOF.Find('3')>-1)
    {
       iDOF=iDOF^DOF_3;
    }
    if (sDOF.Find('4')>-1)
    {
       iDOF=iDOF^DOF_4;
    }
    if (sDOF.Find('5',0)>-1)
    {
       iDOF=iDOF^DOF_5;
    }
    if (sDOF.Find('6',0)>-1)
    {
       iDOF=iDOF^DOF_6;
    }
  }
  return(iDOF);
}

BOOL E_Object::SetProperty(Property* Pr)
{

BOOL bC=FALSE;
if (Pr!=NULL)
{
  if (iType == 115)
  {
    if (Pr->iType==3)
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 112)
  {
    if (Pr->iType==3)
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 94)
  {
      if ((Pr->iType==1) || (Pr->iType==2))
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 91)
  {
      if ((Pr->iType==1) || (Pr->iType==2))
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 21)
  {
      if ((Pr->iType==4) || (Pr->iType==5) || (Pr->iType == 222)) //Remove 222 for test
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if  (iType == 22)
  {
      if (Pr->iType==6) 
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if  (iType == 11)
  {
      if (Pr->iType==11) 
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if  (iType == 4)
  {
      if (Pr->iType==4) 
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 161)
  {
    PID = Pr->iID;
    bC = TRUE;
    pPr = Pr;
  }
  else if (iType == 136)
  {
      if (Pr->iType==136) 
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 137)
  {
      if (Pr->iType==137) 
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 121) 
  {

  }
  else if ((iType == 21) || (iType == 22))
  {

  }
  else if (iType == 111)
  {
    if (Pr->iType==3)
      {
       PID=Pr->iID;
       bC=TRUE;
       pPr=Pr;
      }
  }
  else if (iType == 122)
  {

  }
}
return (bC);
}

void E_Object::Info()
{
  char S1[80];
  sprintf_s(S1,"%s%i%s%i%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour," PID; ",PID," ELTYPE; ",iType);
  outtext1(S1); 
}


Mat E_Object::bmat(Mat& coord,
                   Mat& deriv,
				   int iD,
				   int iDof)
{
int nod,k,l,m;
double x,y;
nod = deriv.n;
Mat bee(iD,iDof*iNoNodes);
if (iD==3)
{
  for (m=1;m<nod+1;m++)
  {
   k = 2 * m;
   l = k - 1;
   x = *deriv.mn(1, m);
   *bee.mn(1, l) = x;
   *bee.mn(3, k) = x;
   y = *deriv.mn(2, m);
   *bee.mn(2, k) = y;
   *bee.mn(3, l) = y;
  }
}
else if (iD==6)
{
  for (k=1;k<nod+1;k++)//was m
  {
   *bee.mn(1,(k)*3-3+1) = *deriv.mn(1,k);
   *bee.mn(2,(k)*3-2+1) = *deriv.mn(2,k);
   *bee.mn(3,(k)*3-1+1) = *deriv.mn(3,k);
   *bee.mn(4,(k)*3-3+1) = *deriv.mn(2,k);
   *bee.mn(4,(k)*3-2+1) = *deriv.mn(1,k);
   *bee.mn(5,(k)*3-2+1) = *deriv.mn(3,k);
   *bee.mn(5,(k)*3-1+1) = *deriv.mn(2,k);
   *bee.mn(6,(k)*3-3+1) = *deriv.mn(3,k);
   *bee.mn(6,(k)*3-1+1) = *deriv.mn(1,k);
  }
}

return (bee);
}

BOOL E_Object::ChkNegJac()
{
BOOL brc=FALSE;
Mat coord;
Mat deriv;
Mat Points;
Mat jac;
double det;
int iDof;


iDof=noDof();
if ((iType==111) ||
    (iType==112) ||
    (iType==115))
{
  if (iDof==2)  
  {
     coord=getCoords2d();   
  }
  else
  {
     coord=getCoords3d();
  }

  Points=Sample(1);
  det = 0;
  deriv=ShapeDer(Points, 1);
  jac = deriv*coord;
  jac=jac.InvertJac(det);
  if (det<0)
  {
    brc=TRUE;
  }
}
else
{
  brc=FALSE;
}
return(brc);
}

Mat E_Object::GetThermalStrainMat3d(PropTable* PropsT,MatTable* MatT,double dT)
{
Mat bee;   //strain displacement matrix
Mat dee;   //stress strain
int nip=0;
Mat coord;
Mat coord2;
Mat deriv;
Mat deriv2;
Mat fun;
Mat Points;
Mat jac;
int i;
double det;
Mat bT;
Mat dS;
Mat bdT;
int iDof;
int iS;
int MID=-1;
double dCTE=0;
double dE=0;
double dv=0;

double dthk=0.001;
Property* pS=NULL;
Material* pM=NULL;
// Get Shell Thicknes and thermal coeff
// ************NEED TO DO BEAMS LATTER************ 
if (PropsT!=NULL)
  pS=PropsT->GetItem(PID);

  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
    pM=MatT->GetItem(MID);
    dthk=pS->GetThk();
  }
  else
  {
    dCTE=1;  //for debugging
  }
  if (pM!=NULL)
  {
    dCTE=pM->GetCTE();
    dE=pM->GetE();
    dv=pM->GetV();
  }

  if (iType==91)
  {
    iDof=2; nip=1; iS=3;
  }
  else if (iType==94)
  {
    iDof=2; nip=4; iS=3;   //4 4 4 4 4 4  4 4 4 4  4 4  4 4
  }
  else if (iType==115)
  {
    iDof=3; nip=8; iS=6;
  }
  else if (iType==111)
  {
    iDof=3; nip=1; iS=6;
  }
  else if (iType==112)
  {
    iDof=3; nip=6; iS=6;
  }
  else if (iType == 136)       //Translational Spring zero vec
  {
    iDof = 3; nip = 0; iS = 0;
  }
  else if (iType == 137)       //Rotational Spring zero vec
  {
    iDof = 3; nip = 0; iS = 0;
  }
//*********************JUST FOR TEST*******************************
Mat FS(iDof*iNoNodes,1);
Mat S;
if (iDof==3)
{
  S.Create(6,1);
  *S.mn(1, 1) = dCTE*dT;
  *S.mn(2, 1) = dCTE*dT;
  *S.mn(3, 1) = dCTE*dT;
  *S.mn(4, 1) = 0;
  *S.mn(5, 1) = 0;
  *S.mn(6, 1) = 0;
}
else if (iDof==2)
{
  S.Create(3,1);
  *S.mn(1, 1) = dCTE*dT;
  *S.mn(2, 1) = dCTE*dT;
  *S.mn(3, 1) = 0;
}

if ((iType==91) || (iType==94) || (iType==115) || (iType==111) || (iType==112))
{
  Mat dee=DeeMat(dE,dv,iS);    //***********************
  if (iDof==2)
    {coord=getCoords3d();}     //was {coord=getCoords3d();} the 3d version includes transform
  else
    {coord=getCoords3d();}
  Points=Sample(nip);
  for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee=bmat(coord, deriv2,iS,iDof);
    bT = bee;
    bT.Transpose();
    dS = dee*S;
    bdT = bT*dS;
	  if (iDof==3)
      {det*=*Points.mn(i,4);}
	  else if (iDof==2)
      {det*=*Points.mn(i,3);}
    bdT*=det;
    FS+=bdT;
  }
}
//for plain stress problems factor shells KM by dthk
if ((iType==91) || (iType==94))
{
  FS*=dthk;
}
//FS.diag();
dTemp=dCTE*dT;
return (FS);
}

Mat E_Object::GetElNodalMass(PropTable* PropsT,MatTable* MatT)
{
Mat bee;   //strain displacement matrix
int nip=0;
Mat coord;
Mat deriv;
Mat fun;
Mat NT;
Mat Points;
Mat jac;
int i;
double det;
Mat bT;
Mat MM;
int iDof;
int iS;
int MID=-1;
double dthk=1.0;
double dRho=1;
char S1[80];
Property*pS=NULL;
Material* pM=NULL;

// Get Shell Thicknes and Density
// ************NEED TO DO BEAMS LATTER************ 

  pS=PropsT->GetItem(PID);
  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
    if (MatT!=NULL)
      pM=MatT->GetItem(MID);
  }

  if (pM!=NULL)
    dRho=pM->GetDensity();

if (((iType==91) || (iType==94)) && (pS!=NULL))
{
  if (pS!=NULL)
  {
    PSHELL* pSh = (PSHELL*) pS;
    dthk=pSh->dT;
  }
  else
  {
    sprintf_s(S1,"ERROR: Property Not Found For EL %i",iLabel);
    outtext1(S1);
  }
}


if (iType==91)
{
iDof=2; nip=1; iS=3;
}
else if (iType==94)
{
iDof=2; nip=4; iS=3;
}
else if (iType==115)
{
iDof=3; nip=8; iS=6;
}
else if (iType==111)
{
iDof=3; nip=1; iS=6;
}
else if (iType==112)
{
iDof=3; nip=6; iS=6;
}
//*********************JUST FOR TEST*******************************
Mat FS(iNoNodes,1);
Mat S(1,1);
*S.mn(1, 1) = dRho;
if ((iType==91) || (iType==94) || (iType==115) || (iType==111) || (iType==112))
{
  if (iDof==2)
    {coord=getCoords3d();}  //WAS {coord=getCoords2d();}
  else
    {coord=getCoords3d();}
  Points=Sample(nip);
  for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    NT= fun;
    NT.Transpose();
    MM = NT*S;
	if (iDof==3)
      {det*=*Points.mn(i,4);}
	else if (iDof==2)
      {det*=*Points.mn(i,3);}
    MM*=det;
    FS+=MM;
  }
}
//for plain stress problems factor shells KM by dthk
if ((iType==91) || (iType==94))
{
  FS*=dthk;
}
//FS.diag();
return (FS);
}

Mat E_Object::GetStiffMat(PropTable* PropsT,MatTable* MatT)
{
Mat bee;   //strain displacement matrix
Mat dee;   //stress strain
int nip=0;
Mat coord;
Mat coord2;
Mat deriv;
Mat deriv2;
Mat fun;
Mat Points;
Mat jac;
int i;
double det;
Mat bT;
Mat db;
Mat bdb;
int iDof;
int iS;
int MID=-1;
double dE=210e9;
double dv=0.29;
char S1[80];
double dthk=0.001;

Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if (pS==NULL)
{
  sprintf_s(S1,"PROPERTY NOT FOUND FOR EL %i",iLabel);
  outtext1(S1);
}
if (pM==NULL)
{  
  sprintf_s(S1,"MATERIAL NOT FOUND FOR EL %i",iLabel);
  outtext1(S1); 
}
//Get Shell thickness

if (((iType==91) || (iType==94)) && (pS!=NULL))
{
  PSHELL* pSh = (PSHELL*) pS;
  dthk=pSh->dT;
}

if ((pM!=NULL) && (pM->iType = 1))
{
  MAT1* pIsen = (MAT1*) pM;
  dE=pIsen->dE;
  dv=pIsen->dNU;
}

if (iType==91)
{
iDof=2;
nip=1;
iS=3;
}
else if (iType==94)
{
iDof=2;
nip=4;
iS=3;
}
else if (iType==115)
{
iDof=3;
nip=8;
iS=6;
}
else if (iType==111)
{
iDof=3;
nip=1;
iS=6;
}
else if (iType==112)
{
iDof=3;
nip=6;
iS=6;
}
Mat KM(iDof*iNoNodes,iDof*iNoNodes);

if ((iType==91) || (iType==94) || (iType==115) || (iType==111) || (iType==112))
{
  Mat dee=DeeMat(dE,dv,iS);
  if (iDof==2)
  {
    coord=getCoords2d();
  }
  else
  {
    coord=getCoords3d();
  }
  //coord.diag();
  Points=Sample(nip);
  for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee=bmat(coord, deriv2,iS,iDof);
    bT = bee;
    bT.Transpose();
    db = dee*bee;
    bdb = bT*db;
	if (iDof==3){det*=*Points.mn(i,4);}
	else if (iDof==2){det*=*Points.mn(i,3);}
    bdb*=det;
    KM+=bdb;
  }
}
//for plain stress problems factor shells KM by dthk
if ((iType==91) || (iType==94))
{
  KM*=dthk;
}
//KM.diag();
return (KM);
}

Mat E_Object::GetThermMat(PropTable* PropsT,MatTable* MatT)
{
  
Mat Kay;   //Conductivity
int nip=0;
Mat coord;
Mat coord2;
Mat deriv;
Mat deriv2;
Mat fun;
Mat Points;
Mat jac;
int i;
double det;
Mat bT;
Mat db;
Mat bdb;
int iDof;
int MID=-1;
double dK=200;
char S1[80];
double dthk=0.001;
BOOL is2dEl=FALSE;
Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if (pS==NULL)
{
  sprintf_s(S1,"PROPERTY NOT FOUND FOR EL %i",iLabel);
  outtext1(S1);
}
if (pM==NULL)
{  
  sprintf_s(S1,"MATERIAL NOT FOUND FOR EL %i",iLabel);
  outtext1(S1); 
}
//Get Shell thickness

if (((iType==91) || (iType==94)) && (pS!=NULL))
{
  PSHELL* pSh = (PSHELL*) pS;
  dthk=pSh->dT;
}

if ((pM!=NULL) && (pM->iType = 1))
{
  MAT1* pIsen = (MAT1*) pM;
  dK=pIsen->dk;
}

if (iType==91)
{
iDof=1;
nip=1;
is2dEl=TRUE;
}
else if (iType==94)
{
iDof=1;
nip=4;
is2dEl=TRUE;
}
else if (iType==115)
{
iDof=1;
nip=8;
}
else if (iType==111)
{
iDof=1;
nip=1;
}
else if (iType==112)
{
iDof=1;
nip=6;
}
Mat KM(iDof*iNoNodes,iDof*iNoNodes);

if ((iType==91) || (iType==94) || (iType==115) || (iType==111) || (iType==112))
{
  Mat Kay;
  if (is2dEl)
  {
    Kay=KayMat(dK,2);
    coord=getCoords3d();
  }
  else
  {
    Kay=KayMat(dK,3);
    coord=getCoords3d();
  }
  //coord.diag();
  Points=Sample(nip);
  for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bT = deriv2;
    bT.Transpose();
    db = Kay*deriv2;
    bdb = bT*db;
    if (is2dEl)
      det*=*Points.mn(i,3);
    else
      det*=*Points.mn(i,4);   //ASSUME ITS A 3D PROBLEM FOR NOW
	//if (iDof==3){det*=*Points.mn(i,4);}
	//else if (iDof==2){det*=*Points.mn(i,3);}
    bdb*=det;
    KM+=bdb;
  }
}
//for plain stress problems factor shells KM by dthk
if ((iType==91) || (iType==94))
{
  KM*=dthk;
}
//KM.diag();
return (KM);
}

Vec<int> E_Object::GetSteerVec3d()
{
Vec<int> V(0);
return(V);
}

Vec<int> E_Object::GetSteerVec1d()
{
Vec<int> V(0);
return(V);
}

int E_Object::MaxBW()
{
return (-1);
}


void E_Object::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar<<PID;
    ar<<PIDunv;
    ar<<iMatID;
    ar<<iNoNodes;
	}
	else
	{
    G_Object::Serialize(ar,iV);
	if (iV>=-52)
      ar>>iType;
    ar>>PID;
    ar>>PIDunv;
    ar>>iMatID;
    ar>>iNoNodes;
    pResV=NULL;
    pPr=NULL;
	}

}

G_Object* E_Object::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object* gret = new E_Object;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pPr = NULL;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
E_Object* gret = new E_Object;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{

E_Object* gret = new E_Object;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

void E_Object::ExportNAS(FILE* pFile)
{
  fprintf(pFile,"%s %i\n","$ERROR MISSING ELEMENT DEFINITION - ELEMENT:",iLabel);
}

void E_Object::ExportUNV(FILE* pFile)
{

}

Mat E_Object::DeeMat(double E, double v,int iD)
{
double C,v2,vv;
int i;
Mat dee(iD,iD);
if (iD==3)  //Plain Stress
{
  C = E / (1-v*v);
  *dee.mn(1, 1) = C;
  *dee.mn(2, 2) = C;
  *dee.mn(3, 3) = 0.5*(1-v)*C;
  *dee.mn(1, 2) = v*C;
  *dee.mn(2, 1) = v*C;
}
else if(iD==6)
{
  //v2=v/(1-v);
  //vv=0.5*(1-2*v)/(1-v);
  //for (i=1;i<4;i++)
  //{
  //  *dee.mn(i, i)=1;
  //}
  //for (i=4;i<7;i++)
  //{
  //  *dee.mn(i, i)=vv;
  //}
  //*dee.mn(1, 2)=vv;
  //*dee.mn(2, 1)=vv;
  //*dee.mn(1, 3)=vv;
  //*dee.mn(3, 1)=vv;
  //*dee.mn(2, 3)=vv;
  //*dee.mn(3, 2)=vv;
  //dee*=E/(2*(1+v)*vv);

  v2=v/(1-v);
  vv=0.5*(1-2*v)/(1-v);
  for (i=1;i<4;i++)
  {
    *dee.mn(i, i)=1;
  }
  for (i=4;i<7;i++)
  {
    *dee.mn(i, i)=vv;
  }
  *dee.mn(1, 2)=v2;
  *dee.mn(2, 1)=v2;
  *dee.mn(1, 3)=v2;
  *dee.mn(3, 1)=v2;
  *dee.mn(2, 3)=v2;
  *dee.mn(3, 2)=v2;
  dee*= E*(1-v)/((1+v)*(1-2*v));
}
return (dee);
}

//********************************************************************************
// THERMAL CONDUCTIVITY MATRIX K
//********************************************************************************
Mat E_Object::KayMat(double K, int iD)
{

Mat Kay(iD,iD);
if (iD==2) 
{
  *Kay.mn(1, 1) = K;
  *Kay.mn(2, 2) = K;
}
if (iD==3) 
{
  *Kay.mn(1, 1) = K;
  *Kay.mn(2, 2) = K;
  *Kay.mn(3, 3) = K;
}
return (Kay);
}

//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object2, CObject )

E_Object2::E_Object2()
{
//(iType == 136) || (iType == 137)
iCSYS=-1;
}

E_Object2::~E_Object2()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
}

void E_Object2::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType, int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<2;i++)
{
  pVertex[i] = pInVertex[i];
}
A=0;
B=0;
C=0;
iCSYS=-1;
}

int E_Object2::noDof()
{
return(3);
}

void E_Object2::Info()
{
  char S1[80];
  sprintf_s(S1,"%s%i%s%i%s%i%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour," PID; ",PID," ELTYPE; ",iType," CSYS; ",iCSYS);
  outtext1(S1); 
  sprintf_s(S1, "%s%i%s%i", "ND1 ", pVertex[0]->iLabel, "ND2 ", pVertex[1]->iLabel);
  outtext1(S1);
}

C3dMatrix E_Object2::GetSpringSys(CoordSys* pC)
{
C3dMatrix mRC;
C3dMatrix mInvRC;
C3dVector pt;
C3dVector pO;
double ang;

  if (pC!=NULL)
  {
    mRC=pC->GetTMat();
    if (pC->CysType==2)
    {
      pt=this->Get_Centroid();
      pO=pC->Get_Centroid();
      pt-=pO;
      mRC.Transpose();
      pt=mRC*pt;
      ang=atan2(pt.y,pt.x)*180/Pi;
      mInvRC.MakeUnit();
      mInvRC.Rotate(0,0,ang);
      mRC.Transpose();
      mRC*=mInvRC;
      mInvRC=mRC;
    }
    else if (pC->CysType==1)
    {
      mInvRC=mRC;
    }
    else
    {
      outtext1("ERROR: Spherical Coordinate Sys Not Supported.");
    }
  }
  else
  {
    outtext1("ERROR: Coordinateord Sys Not Found.");
  }

return (mInvRC);
}

Mat E_Object2::GetSpringTMat(CoordSys* pCSYS)
{
  int i,j;
  C3dVector K;
  C3dMatrix r;
  Vec<int> V(2*3);

  Mat KM(6,6);
  Mat t(6,6);
  if (pCSYS!=NULL)
  {
    //This composes the stiffness tranformation matrix
    r=GetSpringSys(pCSYS);
    r.Transpose();
    Mat mr(3,3);
    *mr.mn(1,1)=r.m_00; *mr.mn(1,2)=r.m_01; *mr.mn(1,3)=r.m_02;
    *mr.mn(2,1)=r.m_10; *mr.mn(2,2)=r.m_11; *mr.mn(2,3)=r.m_12;
    *mr.mn(3,1)=r.m_20; *mr.mn(3,2)=r.m_21; *mr.mn(3,3)=r.m_22;
    for (i=1;i<4;i++)
    {
      for (j=1;j<4;j++)
      {
         *t.mn(i,j) = *mr.mn(i,j);
         *t.mn(i+3,j+3) = *mr.mn(i,j);
      }
    }
  }
  return (t);
}

Mat E_Object2::GetThermMat(PropTable* PropsT,MatTable* MatT)
{
  char S1[80];
  double K;
  int iDof=1;
  Property* pS=PropsT->GetItem(this->PID);
  if (pS!=NULL)
  {
    if (pS->iType==136)
    {
      PSPRINGT* pSP=(PSPRINGT*) pS;
      K=pSP->dkcoeff;
    }
  }
  else
  {
    sprintf_s(S1,"%s%i","ERROR: No Property Found for Spring Element ",iLabel);
    outtext1(S1); 
  }

Mat KM(iDof*iNoNodes,iDof*iNoNodes);
C3dVector p1;
C3dVector p2;
double l;
p1=pVertex[0]->GetCoords();
p2=pVertex[1]->GetCoords();
p2-=p1;
l=p2.Mag();
*KM.mn(1,1)=K;
*KM.mn(2,1)=-K;
*KM.mn(1,2)=-K;
*KM.mn(2,2)=K;
//KM.diag();
return (KM);
}


Mat E_Object2::GetStiffMat(PropTable* PropsT,MatTable* MatT)
{
  double kx=1e3;
  double ky=1e3;
  double kz=1e3;
  char S1[80];
  C3dMatrix r;
  BOOL bFail=FALSE;
  Mat TMat;
  Mat TTMat(6,6);
  Mat Kmt;
  Mat tKmt;
  Property* pS=PropsT->GetItem(this->PID);
  if (pS!=NULL)
  {
    if (pS->iType==136)
    {
      PSPRINGT* pSP=(PSPRINGT*) pS;
      kx=pSP->dkx; ky=pSP->dky; kz=pSP->dkz;
    }
    else if (pS->iType==137)
    {
      PSPRINGR* pSP=(PSPRINGR*) pS;
      kx=pSP->dkx; ky=pSP->dky; kz=pSP->dkz;
    }
  }
  else
  {
    sprintf_s(S1,"%s%i","ERROR: No Property Found for Spring Element ",iLabel);
    outtext1(S1); 
  }
  Mat KM(6,6);
  KM*=0;
  //DEFUALT MATRIX GLOBAL
  *KM.mn(1,1)=kx;*KM.mn(4,4)=kx;*KM.mn(1,4)=-kx;*KM.mn(4,1)=-kx;
  *KM.mn(2,2)=ky;*KM.mn(5,5)=ky;*KM.mn(2,5)=-ky;*KM.mn(5,2)=-ky;
  *KM.mn(3,3)=kz;*KM.mn(6,6)=kz;*KM.mn(3,6)=-kz;*KM.mn(6,3)=-kz;

  //TRANSFORM ELEMENT TO GLOBAL FROM LOCAL DEFINITION
  CoordSys* pCSYS=NULL;
  if (this->iCSYS!=-1)
  {
    ME_Object* ME = (ME_Object*) this->pParent;
    if (ME!=NULL)
    {
       pCSYS=ME->GetSys(iCSYS);
       if (pCSYS!=NULL)
       {
         r=GetSpringSys(pCSYS);
         TMat=GetSpringTMat(pCSYS);  
         TTMat=TMat; TTMat.Transpose();
         Kmt.clear(); tKmt.clear();
         Kmt=KM*TMat;
         tKmt=TTMat*Kmt;
         KM=tKmt;
         Kmt.clear(); tKmt.clear();
         TMat.clear(); TTMat.clear();
       }
       else
       {
         sprintf_s(S1,"%s%i","ERROR: Coordinate System Not Found for Spring Element ",iLabel);
         outtext1(S1); 
         bFail=TRUE;
       }
    }
    else
    {
      outtext1("ERROR: Orphaned Element.");
      bFail=TRUE;
    }
  }
  else
  {
    bFail=TRUE;
  }


  
return (KM);
}


Vec<int> E_Object2::GetSteerVec3d()
{
Vec<int> V(2*3);
if (iType==136)
{
  *V.nn(1)=pVertex[0]->dof[0];
  *V.nn(2)=pVertex[0]->dof[1];
  *V.nn(3)=pVertex[0]->dof[2];
  *V.nn(4)=pVertex[1]->dof[0];
  *V.nn(5)=pVertex[1]->dof[1];
  *V.nn(6)=pVertex[1]->dof[2];
}
else if (iType==137)
{
  *V.nn(1)=pVertex[0]->dof[3];
  *V.nn(2)=pVertex[0]->dof[4];
  *V.nn(3)=pVertex[0]->dof[5];
  *V.nn(4)=pVertex[1]->dof[3];
  *V.nn(5)=pVertex[1]->dof[4];
  *V.nn(6)=pVertex[1]->dof[5];
}

return(V);
}

Vec<int> E_Object2::GetSteerVec1d()
{
  Vec<int> V(2*1);
  *V.nn(1)=pVertex[0]->dof[0];
  *V.nn(2)=pVertex[1]->dof[0];
  return(V);
}

BOOL E_Object2::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object2::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

void E_Object2::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
	  vUp.Serialize(ar,iV);
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << A;
    ar << B;
	  ar << C;
    ar << iCSYS;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
	  vUp.Serialize(ar,iV);
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
	  ar >> A;
    ar >> B;
	  ar >> C;
    ar >> iCSYS;
	}
}


G_Object* E_Object2::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2* gret = new E_Object2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->vUp=vUp;
gret->A=A;
gret->B=B;
gret->C=C;
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
gret->iCSYS = iCSYS;
return (gret);
}

G_Object* E_Object2::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object2* gret = new E_Object2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iCSYS = iCSYS;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
gret->iCSYS = iCSYS;
gret->vUp=vUp;
Pt_Object* nA=Source->GetNode(A);
if (nA!=NULL)
{
  EInd=Source->GetNodeInd(nA);
  gret->A = Target->pNodes[EInd+iSInd]->iLabel;  //Orientation Node
}
else
{
  gret->A=A;
}
gret->B=B;
gret->C=C;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object2::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2* gret = new E_Object2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->vUp=vUp;
gret->A=A;
gret->B=B;
gret->C=C;
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object2::Draw(CDC* pDC,int iDrawmode)
{
pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
}

int E_Object2::GetLinkList(cLink* Links[200])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->iColour=iColour;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
return (1);
}

G_Object* E_Object2::GetNode(int i)
{
return (pVertex[i]);
}

C3dMatrix E_Object2::GetElSys()
{
C3dMatrix vR;
C3dVector vX;
C3dVector vY;
C3dVector vZ;

vX.x=pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
vX.y=pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
vX.z=pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;
vY=vUp;
vX.Normalize();
vY.Normalize();
vZ=vX.Cross(vY);
vZ.Normalize();
vY=vZ.Cross(vX);
vR.SetColVec(1,vX);
vR.SetColVec(2,vY);
vR.SetColVec(3,vZ);
vR.Transpose();
return (vR);
}

void E_Object2::OglDraw(int iDspFlgs,double dS1,double dS2)
{
	OglDrawW(iDspFlgs,dS1,dS2);
}

void E_Object2::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int ind;
BOOL bD=FALSE;
float fCols[2]={0,0};
C3dVector d[2];
double S;
double dFS = 1.0;
int i;

for (i=0;i<2;i++)
  {d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
S=ME->dScale;
dFS = ME->dResFactor;
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
   if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<2;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S*dFS;
       }
    }
  }

	Selectable=1;
	ind=ME->iCVar;
  if ((pVertex[0]->pResV != NULL) &&
      (pVertex[1]->pResV != NULL))
  {
    bD=TRUE;
    fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(ind)*dFS);
    fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(ind)*dFS);
  }
  if (pResV!=NULL)
  {
    bD=TRUE;
    fCols[0]=GetContourCol(*pResV->GetAddress(ind)*dFS);
    fCols[1]=fCols[0];
  }
	C3dVector vCent;
	char sLab[20];
	vCent=Get_Centroid();
	if (((iDspFlgs & DSP_CONT)==0) && (bD==TRUE))
	{
	  glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glTexCoord1f(fCols[0]);
      glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
      glTexCoord1f(fCols[1]);
      glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
      glEnd();
	  glRasterPos3f ((float) vCent.x,(float) vCent.y,(float) vCent.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPSP);
      glDisable(GL_TEXTURE_1D);
	}
	else
	{
	  glColor3fv(cols[iColour]);
      glLineWidth(2.0);
	  glBegin(GL_LINES);
      glVertex3f((float) (pVertex[0]->Pt_Point->x+d[0].x),(float) (pVertex[0]->Pt_Point->y+d[0].y),(float) (pVertex[0]->Pt_Point->z+d[0].z));
      glVertex3f((float) (pVertex[1]->Pt_Point->x+d[1].x),(float) (pVertex[1]->Pt_Point->y+d[1].y),(float) (pVertex[1]->Pt_Point->z+d[1].z));
	  glEnd();
	  glRasterPos3f ((float) vCent.x,(float) vCent.y,(float) vCent.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPSP);
	}

	if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"E%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
	if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
	{
	   sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
       OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
	if ((iDspFlgs & DSP_ELSYS)==0)
	{
		//C3dMatrix mS = GetElSys(); 
	  glColor3fv(cols[iColour]);
      C3dMatrix mS; //  For spring its global or assigned
      mS.MakeUnit();
      if (iCSYS > -1)
      {
        if (this->pParent != NULL)
        {
          if (this->pParent->iObjType == 4)
          {
            ME_Object* pM = (ME_Object*) this->pParent;
            CoordSys* pSYS = (CoordSys*)pM->GetSys(iCSYS);
            if (pSYS != NULL)
            {
              mS = pSYS->GetTMat();
            }
          }
        }
      }
		C3dVector vC = Get_Centroid();
		mS.Transpose();
		C3dVector vX=mS.GetColVec(1);
		C3dVector vY=mS.GetColVec(2);;
		C3dVector vZ=mS.GetColVec(3);;
		vX*=0.5*dS1; vY*=0.5*dS1; vZ*=0.5*dS1;
		vX+=vC;vY+=vC;vZ+=vC;
		glBegin(GL_LINES);
			glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
			glVertex3f((float) vX.x,(float) vX.y,(float) vX.z);
			glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
			glVertex3f((float) vY.x,(float) vY.y,(float) vY.z);
			glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
			glVertex3f((float) vZ.x,(float) vZ.y,(float) vZ.z);
		glEnd();
		glRasterPos3f ((float) vX.x,(float) vX.y,(float) vX.z);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
		glRasterPos3f ((float) vY.x,(float) vY.y,(float) vY.z);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
		glRasterPos3f ((float) vZ.x,(float) vZ.y,(float) vZ.z);
		glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
	}
}
else
{
  Selectable=0;
}
}



double E_Object2::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  int iDOFID;
  double T;
  int j;
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T;
  }
  dTemp /= iNoNodes;
  return(dTemp);
}

C3dVector E_Object2::Get_Centroid()
{
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid();
}
vT.x/=iNoNodes;
vT.y/=iNoNodes;
vT.z/=iNoNodes;
return (vT);
}


void E_Object2::SetUpVec(C3dVector vIn)
{
 vUp=vIn;
}

void E_Object2::SetSec(int iA,int iB,int iC)
{
A=iA;
B=iB;
C=iC;
}


void E_Object2::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
if ((iType==21) || (iType==22))
{
  fprintf(pFile,"%10i%10i%10i\n",A,B,C);
}
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}


void E_Object2::ExportNAS(FILE* pFile)
{
  C3dMatrix TMat;
  int iCS;
  if (iCSYS < 0)
	  iCS = 0;
  else
	  iCS = iCSYS;

  if (iType == 136)
  {
    //fprintf(pFile, "%8s%8i%8i%8i%8i%8s%8s%8s%8i\n", "CBUSH   ", iLabel, PID, pVertex[0]->iLabel, pVertex[1]->iLabel,"","","", iCS);
	ME_Object* pM = (ME_Object*)this->pParent;
	fprintf(pFile, "%8s%8i%8s%8i%8s%8i%8s\n", "CELAS2  ", pM->iElementLab, "1000.0", pVertex[0]->iLabel, "1", pVertex[1]->iLabel, "1");
	pM->iElementLab++;
	fprintf(pFile, "%8s%8i%8s%8i%8s%8i%8s\n", "CELAS2  ", pM->iElementLab, "1.0E8", pVertex[0]->iLabel, "2", pVertex[1]->iLabel, "2");
	pM->iElementLab++;
	fprintf(pFile, "%8s%8i%8s%8i%8s%8i%8s\n", "CELAS2  ", pM->iElementLab, "1000.0", pVertex[0]->iLabel, "3", pVertex[1]->iLabel, "3");
	pM->iElementLab++;

  }
}


void E_Object2::ExportUPVecs(FILE* pFile)
{
double dA1;
C3dVector Vz;
C3dVector Vy;
C3dVector Vx;
C3dVector Vu;
C3dMatrix M;
if (A==0)
{
  M.MakeUnit();
  Vu.Set(vUp.x,vUp.y,vUp.z);
	Vz.x=pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	Vz.y=pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	Vz.z=pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;
	Vz.Normalize();
	Vy.Set(0,0,1);
	double dDot;
	dDot = abs(Vy.Dot(Vz));
	if (dDot==1) 
	{
	  Vx.Set(0,1,0);
	  Vy=Vz.Cross(Vx);
	  Vy.Normalize();
	  Vx=Vy.Cross(Vz);
	  Vx.Normalize();

	}
	else
	{
	  Vx=Vy.Cross(Vz);
	  Vx.Normalize();
	  Vy=Vz.Cross(Vx);
	  Vy.Normalize();
	}
	M.SetColVec(1,Vx);
	M.SetColVec(2,Vy);
	M.SetColVec(3,Vz);
	M.Transpose();
	Vu=M.Mult(Vu);
	if (Vu.y!=0)
	{
	   dA1=atan2(Vu.x,Vu.y);
	   dA1=dA1*180/3.1415926535897932384626433832795;
	}
	else
	{
		dA1=0;
	}
	dA1+=90.0;
	fprintf(pFile,"%10i%10i\n",iLabel,21);
	fprintf(pFile,"%10i%10i%10i%10i%10i\n",8,0,0,0,0);
	fprintf(pFile,"%15.6E%15.6E%15.6E\n",dA1,0.0,0.0);
}
else
{
	fprintf(pFile,"%10i%10i\n",iLabel,21);
	fprintf(pFile,"%10i%10i%10i%10i%10i\n",0,1,0,0,0);
}
}

CString E_Object2::GetName()
{
  return("Spring (CBUSH)");
}

int E_Object2::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "CID";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	return(iNo);
}


int E_Object2::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iCSYS);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object2::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;
	PID = atoi(sVar[0]);
	iCSYS = atoi(sVar[1]);
	int N1 = atof(sVar[2]);
	int N2 = atof(sVar[3]);
	if (pVertex[0]->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex[0] = pN;
	}
	if (pVertex[1]->iLabel != N2)
	{
		pN = pMe->GetNode(N2);
		if (pN != NULL)
			pVertex[1] = pN;
	}
}
//----------------------------------------------------------------------------
//    ROD E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object2R, CObject )

E_Object2R::E_Object2R()
{
iDOFA=0;
iDOFB=0;
iONID = -1;
iType=11;
this->iNoNodes=2;
}

E_Object2R::~E_Object2R()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
}

void E_Object2R::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType, int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<2;i++)
{
  pVertex[i] = pInVertex[i];
}
A=0;
B=0;
C=0;
iONID=-1;
OffA*=0;
OffB*=0;
}

void E_Object2R::SetDOFStringA(CString sDOF)
{
  iDOFA=GetDOFInt(sDOF);
}

void E_Object2R::SetDOFStringB(CString sDOF)
{
  iDOFB=GetDOFInt(sDOF);
}

void E_Object2R::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

BOOL E_Object2R::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object2R::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
      E_Object::Serialize(ar,iV,MESH);
	  vUp.Serialize(ar,iV);
      OffA.Serialize(ar,iV);
      OffB.Serialize(ar,iV);
      ar << pVertex[0]->iLabel;
      ar << pVertex[1]->iLabel;
      ar << A;
      ar << B;
	  ar << C;
      ar << iDOFA;
      ar << iDOFB;
	}
	else
	{
      E_Object::Serialize(ar,iV,MESH);
	  vUp.Serialize(ar,iV);
      OffA.Serialize(ar,iV);
      OffB.Serialize(ar,iV);
      ar>>iNd;
      pVertex[0] = MESH->GetNode(iNd);
      ar>>iNd;
      pVertex[1] = MESH->GetNode(iNd);
	  ar >> A;
      ar >> B;
	  ar >> C;
      ar >> iDOFA;
      ar >> iDOFB;
	}
}


G_Object* E_Object2R::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2R* gret = new E_Object2R;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
gret->A=A;
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->iONID=iONID;
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object2R::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object2R* gret = new E_Object2R;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
Pt_Object* pT=Source->GetNode(iONID);
if (pT!=NULL)
{
  EInd=Source->GetNodeInd(pT);
  gret->iONID=Target->pNodes[iSInd+EInd]->iLabel;
}
else
{
  gret->iONID=-1;
}
Pt_Object* nA=Source->GetNode(A);
if (nA!=NULL)
{
  EInd=Source->GetNodeInd(nA);
  gret->A = Target->pNodes[EInd+iSInd]->iLabel;  //Orientation Node
}
else
{
  gret->A=A;
}
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->pPr=NULL;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object2R::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2R* gret = new E_Object2R;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
gret->A=A;
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object2R::Draw(CDC* pDC,int iDrawmode)
{
pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
}

void E_Object2R::Info()
{
	char S1[80];
	sprintf_s(S1, "%s%i%s%i%s%i%s%i%s%i%s%i", "Type ", iObjType, "; Label ", iLabel, " Col; ", iColour, " PID; ", PID, " ELTYPE; ", iType, " CSYS; ", iCSYS);
	outtext1(S1);
	sprintf_s(S1, "%s%i%s%i", "ND1 ", pVertex[0]->iLabel, "ND2 ", pVertex[1]->iLabel);
	outtext1(S1);
	sprintf_s(S1, "Beam Up Vector %g %g %g", vUp.x,vUp.y,vUp.z);
	outtext1(S1);
	CString sEndR;
	sEndR = this->GetDOFString(iDOFA);
	sprintf_s(S1, "End A Release %s ", sEndR);
	outtext1(S1);
	sEndR = this->GetDOFString(iDOFB);
	sprintf_s(S1, "End B Release %s ", sEndR);
	outtext1(S1);
	sprintf_s(S1, "Beam End A Offset %g %g %g", OffA.x, OffA.y, OffA.z);
	outtext1(S1);
	sprintf_s(S1, "Beam End B Offset %g %g %g", OffB.x, OffB.y, OffB.z);
	outtext1(S1);
}



int E_Object2R::GetLinkList(cLink* Links[200])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->iColour=iColour;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
return (1);
}

G_Object* E_Object2R::GetNode(int i)
{
return (pVertex[i]);
}

void E_Object2R::OglDraw(int iDspFlgs,double dS1,double dS2)
{

glColor3fv(cols[iColour]);
C3dVector d[3];
int i;
for (i=0;i<2;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S = ME->dScale;
double dFS = 1.0;
dFS = ME->dResFactor;
//Contour colours bit
float fCols[2]={0,0};
BOOL bD=FALSE;
int iVar;
iVar=ME->iCVar;
//Nodal data
if ((pVertex[0]->pResV != NULL) &&
    (pVertex[1]->pResV != NULL))
{
  bD=TRUE;
  fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
  fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
}
if (pResV!=NULL)
{
  bD=TRUE;
  fCols[0]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
  fCols[1]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
}

if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<2;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S * dFS;
       }
    }
  }
  Selectable=1;
  if (pPr!=NULL)
  {
    BSec* pS = pPr->GetSec();
    if (pS!=NULL)
    {
      C3dMatrix TA=GetBeamTformA();
      C3dMatrix TB=GetBeamTformB();
      pS->OglDraw(iDspFlgs,TA,TB,d[0],d[1],fCols[0],fCols[1],bD);
    }
  }
  else
  {
    if (((iDspFlgs & DSP_CONT)==0) || (bD=TRUE))
    {
      glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);

      glBegin(GL_LINES);
        glTexCoord1f(fCols[0]);
        glVertex3f((float)(pVertex[0]->Pt_Point->x+d[0].x),(float)(pVertex[0]->Pt_Point->y+d[0].y),(float)(pVertex[0]->Pt_Point->z+d[0].z));
        glTexCoord1f(fCols[1]);
        glVertex3f((float)(pVertex[1]->Pt_Point->x+d[1].x),(float)(pVertex[1]->Pt_Point->y+d[1].y),(float)(pVertex[1]->Pt_Point->z+d[1].z));
      glEnd();
      glDisable(GL_TEXTURE_1D);
    }
    
  }
}
else
{
  Selectable=0;
}

}

void E_Object2R::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector d[2];
int i;
int ind;
for (i=0;i<2;i++)
  {d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
double dFS;
S=ME->dScale;
dFS = ME->dResFactor;
ind=ME->iCVar;

if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<2;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S* dFS;
       }
    }
  }
  Selectable=1;
  if ((iDspFlgs & DSP_LINE) > 0)
  {
    glColor3fv(cols[iColour]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
  glBegin(GL_LINES);
    glVertex3f((float) (pVertex[0]->Pt_Point->x+OffA.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+OffA.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+OffA.z+d[0].z));
    glVertex3f((float) (pVertex[1]->Pt_Point->x+OffB.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+OffB.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+OffB.z+d[1].z));
  glEnd();
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
  {
	  sprintf_s(sLab,"E%i",iLabel);
    OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
  {
     sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
     OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
  if (pPr!=NULL)
  {
    BSec* pS = pPr->GetSec();
    if (pS!=NULL)
    {
      C3dMatrix TMat=GetBeamTform();
      pS->OglDrawW(iDspFlgs,TMat,d[0],d[1]);
    }
  }
  if ((iDspFlgs & DSP_ELSYS)==0)
  {
	  glColor3fv(cols[iColour]);
    C3dMatrix mS = GetElSys();
    C3dVector vC = Get_Centroid();
    mS.Transpose();
    C3dVector vX=mS.GetColVec(1);
    C3dVector vY=mS.GetColVec(2);;
    C3dVector vZ=mS.GetColVec(3);;

    vX*=0.5*dS1; vY*=0.5*dS1; vZ*=0.5*dS1;
    vX+=vC;vY+=vC;vZ+=vC;

    glBegin(GL_LINES);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vX.x,(float) vX.y,(float) vX.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vY.x,(float) vY.y,(float) vY.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glEnd();
    glRasterPos3f ((float) vX.x,(float) vX.y,(float) vX.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
    glRasterPos3f ((float) vY.x,(float) vY.y,(float) vY.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
    glRasterPos3f ((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }
}
else
{
  Selectable=0;
}
}




C3dVector E_Object2R::Get_Centroid()
{
C3dVector v1;
C3dVector v2;
C3dVector vC;
v1=pVertex[0]->Get_Centroid();
v2=pVertex[1]->Get_Centroid();
v1+=OffA;
v2+=OffB;
vC=v2;
vC+=v1;
vC*=0.5;

return (vC);
}

C3dVector E_Object2R::GetDir()
{
C3dVector vRet;
C3dVector v1;
C3dVector v2;
v1=pVertex[0]->GetCoords();
v1+=OffA;
v2=pVertex[1]->GetCoords();
v2+=OffB;
vRet=v2;
vRet-=v1;
vRet.Normalize();
return (vRet);
}

void E_Object2R::SetUpVec(C3dVector vIn)
{
 vUp=vIn;
}

C3dMatrix E_Object2R::GetBeamTform()
{
C3dMatrix TRet;
C3dVector vX;
C3dVector vY;
C3dVector vZ;

vX=this->GetDir();;
vY=vUp;
vY.Normalize();
vZ=vY.Cross(vX);
vZ.Normalize();
vY=vZ.Cross(vX);
TRet.SetColVec(1,vZ);
TRet.SetColVec(2,vY);
TRet.SetColVec(3,vX);
C3dVector vC = Get_Centroid();
TRet.Translate2(vC.x,vC.y,vC.z);
return (TRet);
}

C3dMatrix E_Object2R::GetBeamTformA()
{
C3dMatrix TRet;
C3dVector vX;
C3dVector vY;
C3dVector vZ;

vX=this->GetDir();;
vY=vUp;
vY.Normalize();
vZ=vY.Cross(vX);
vZ.Normalize();
vY=vZ.Cross(vX);
TRet.SetColVec(1,vZ);
TRet.SetColVec(2,vY);
TRet.SetColVec(3,vX);

C3dVector vC = pVertex[0]->Get_Centroid();
vC+=OffA;
TRet.Translate2(vC.x,vC.y,vC.z);
return (TRet);
}

C3dMatrix E_Object2R::GetBeamTformB()
{
C3dMatrix TRet;
C3dVector vX;
C3dVector vY;
C3dVector vZ;

vX=this->GetDir();;
vY=vUp;
vY.Normalize();
vZ=vY.Cross(vX);
vZ.Normalize();
vY=vZ.Cross(vX);
TRet.SetColVec(1,vZ);
TRet.SetColVec(2,vY);
TRet.SetColVec(3,vX);
C3dVector vC = pVertex[1]->Get_Centroid();
vC+=OffB;
TRet.Translate2(vC.x,vC.y,vC.z);
return (TRet);
}

C3dMatrix E_Object2R::GetElSys()
{
C3dMatrix vR;
C3dVector vX;
C3dVector vY;
C3dVector vZ;

vX.x=pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
vX.y=pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
vX.z=pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;
vY=vUp;
vX.Normalize();
vY.Normalize();
vZ=vX.Cross(vY);
vZ.Normalize();
vY=vZ.Cross(vX);
vR.SetColVec(1,vX);
vR.SetColVec(2,vY);
vR.SetColVec(3,vZ);
vR.Transpose();
return (vR);
}

void E_Object2R::Transform(C3dMatrix TMat)
{
TMat.ClearTranslations();
vUp=TMat*vUp;
OffA=TMat*OffA;
OffB=TMat*OffB;
}

int E_Object2R::noDof()
{
return (3);
}

int E_Object2R::MaxBW()
{
int i;
int j;

int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<noDof();j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	  {
       MaxDof=pVertex[i]->dof[j];
    }
	  if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	  {
       MinDof=pVertex[i]->dof[j];
	  }
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}


Mat E_Object2R::GetElNodalMass(PropTable* PropsT,MatTable* MatT)
{
  int MID=-1;
  double Rho=0;
  double A=0;
  double L=0;
  char S1[80];
  Property* pS=PropsT->GetItem(PID);
  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
  }
  Material* pM=MatT->GetItem(MID);
  if (pS==NULL)
  {
    sprintf_s(S1,"ERROR PROPERTY NOT FOUND FOR EL %i",iLabel);
    outtext1(S1);
  }
  if (pM==NULL)
  {  
    sprintf_s(S1,"ERROR MATERIAL NOT FOUND FOR EL %i",iLabel);
    outtext1(S1); 
  }
  if ((pS!=NULL) && (pM!=NULL))
  {
    if (pS->iType==11) 
    {
      PROD* pB=(PROD*) pS;
      MAT1* pM1=(MAT1*) pM;
      A=pB->A;
      Rho=pM1->dRHO;
    }
    else if (pS->iType==5) 
    {
      PBARL* pB=(PBARL*) pS;
      MAT1* pM1=(MAT1*) pM;
      A=pB->A;
      Rho=pM1->dRHO;
    }
	else if (pS->iType == 4)
	{
		PBAR* pB = (PBAR*)pS;
		MAT1* pM1 = (MAT1*)pM;
		A = pB->dA;
		Rho = pM1->dRHO;
	}
    else
    {
      sprintf_s(S1,"ERROR INVALID PROPERTY FOR EL %i",iLabel);
      outtext1(S1);
    }
  }
  else
  {
    sprintf_s(S1,"ERROR UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
    outtext1(S1); 
  }
//Lenght

  double dMass;
  C3dVector n1;
  C3dVector n2;
  n1=this->pVertex[0]->GetCoords();
  n2=this->pVertex[1]->GetCoords();
  L=n2.Dist(n1);
  dMass=A*L*Rho/2;
  Mat FS(iNoNodes,1);
  *FS.mn(1,1)=dMass;
  *FS.mn(2,1)=dMass;
  return(FS);
}


Mat E_Object2R::GetThermalStrainMat3d(PropTable* PropsT,MatTable* MatT,double dT)
{

  int MID=-1;
  double CTE=0;
  double A=0;
  double L=0;
  double E=0;
  char S1[80];
  Property* pS=PropsT->GetItem(PID);
  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
  }
  Material* pM=MatT->GetItem(MID);
  if (pS==NULL)
  {
    sprintf_s(S1,"ERROR PROPERTY NOT FOUND FOR EL %i",iLabel);
    outtext1(S1);
  }
  if (pM==NULL)
  {  
    sprintf_s(S1,"ERROR MATERIAL NOT FOUND FOR EL %i",iLabel);
    outtext1(S1); 
  }
  if ((pS!=NULL) && (pM!=NULL))
  {
    if (pS->iType==11) 
    {
      PROD* pB=(PROD*) pS;
      MAT1* pM1=(MAT1*) pM;
      A=pB->A;
      E=pM1->dE;
      CTE=pM1->GetCTE();
    }
    else if (pS->iType==5) 
    {
      PBARL* pB=(PBARL*) pS;
      MAT1* pM1=(MAT1*) pM;
      A=pB->A;
      E=pM1->dE;
      CTE=pM1->GetCTE();
    }
	else if (pS->iType == 4)
	{
		PBAR* pB = (PBAR*)pS;
		MAT1* pM1 = (MAT1*)pM;
		A = pB->dA;
		E = pM1->dE;
		CTE = pM1->GetCTE();
	}
    else
    {
      sprintf_s(S1,"ERROR INVALID PROPERTY FOR EL %i",iLabel);
      outtext1(S1);
    }
  }
  else
  {
    sprintf_s(S1,"ERROR UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
    outtext1(S1); 
  }
  double dF;
  C3dVector n1;
  C3dVector n2;
  n1=this->pVertex[0]->GetCoords();
  n2=this->pVertex[1]->GetCoords();
  L=n2.Dist(n1);
  dF=A*E*CTE*dT;
  Mat FS(iNoNodes,1);
  *FS.mn(1,1)=-dF;
  *FS.mn(2,1)=dF;
  dTemp=dF;
  return (FS);
}

Mat E_Object2R::GetThermMat(PropTable* PropsT,MatTable* MatT)
{
char S1[80];
double A;
double K;
int MID;
int iDof=1;
Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if ((pS!=NULL) && (pM!=NULL))
{
  if (pS->iType==11)
  {
    PROD* pB=(PROD*) pS;
    MAT1* pM1=(MAT1*) pM;
    A=pB->A;
    K=pM1->dk;
  }
  else
  {
    sprintf_s(S1,"ERROR INVALID PROPERTY FOR EL %i",iLabel);
    outtext1(S1);
  }
}
else
{
  sprintf_s(S1,"ERROR UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
  outtext1(S1); 
}

Mat KM(iDof*iNoNodes,iDof*iNoNodes);
C3dVector p1;
C3dVector p2;
double l;
p1=pVertex[0]->GetCoords();
p2=pVertex[1]->GetCoords();
p2-=p1;
l=p2.Mag();
*KM.mn(1,1)=K*A/l;
*KM.mn(2,1)=-K*A/l;
*KM.mn(1,2)=-K*A/l;
*KM.mn(2,2)=K*A/l;
//KM.diag();
return (KM);
}

Mat E_Object2R::GetStiffMat(PropTable* PropsT,MatTable* MatT)
{
double ea=10000;
double eal=10000;
double x1,y1,z1,x2,y2,z2;
double ell;
int MID=-1;
char S1[80];
Mat KM(6,6);
Mat t(6,6);
Mat tt(6,6);
KM.MakeZero();
t.MakeZero();
tt.MakeZero();

Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if (pS==NULL)
{
  sprintf_s(S1,"ERROR PROPERTY NOT FOUND FOR EL %i",iLabel);
  outtext1(S1);
}
if (pM==NULL)
{  
  sprintf_s(S1,"ERROR MATERIAL NOT FOUND FOR EL %i",iLabel);
  outtext1(S1); 
}
if ((pS!=NULL) && (pM!=NULL))
{
  if (pS->iType==11)
  {
    PROD* pB=(PROD*) pS;
    MAT1* pM1=(MAT1*) pM;
    ea=pB->A*pM1->dE;
  }
  else
  {
    sprintf_s(S1,"ERROR INVALID PROPERTY FOR EL %i",iLabel);
    outtext1(S1);
  }
}
else
{
  sprintf_s(S1,"ERROR UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
  outtext1(S1); 
}



x1=pVertex[0]->Pt_Point->x;
y1=pVertex[0]->Pt_Point->y;
z1=pVertex[0]->Pt_Point->z;
x2=pVertex[1]->Pt_Point->x;
y2=pVertex[1]->Pt_Point->y;
z2=pVertex[1]->Pt_Point->z;
x1-=x2;y1-=y2;z1-=z2;
ell=pow(x1*x1+y1*y1+z1*z1,0.5);
eal=ea/ell;

*KM.mn(1,1)=eal;
*KM.mn(4,4)=eal;
*KM.mn(1,4)=-eal;
*KM.mn(4,1)=-eal;
//KM.diag();
C3dMatrix r;
r=GetElSys();
Mat mr(3,3);
*mr.mn(1,1)=r.m_00; *mr.mn(1,2)=r.m_01; *mr.mn(1,3)=r.m_02;
*mr.mn(2,1)=r.m_10; *mr.mn(2,2)=r.m_11; *mr.mn(2,3)=r.m_12;
*mr.mn(3,1)=r.m_20; *mr.mn(3,2)=r.m_21; *mr.mn(3,3)=r.m_22;


int i,j;
for (i=1;i<4;i++)
{
  for (j=1;j<4;j++)
  {
     *t.mn(i,j) = *mr.mn(i,j);
     *t.mn(i+3,j+3) = *mr.mn(i,j);
     *tt.mn(i,j) = *mr.mn(i,j);
     *tt.mn(i+3,j+3) = *mr.mn(i,j);
  }
}
tt.Transpose();
Mat Kmt;
Mat tKmt;
Kmt=KM*t;
tKmt=tt*Kmt;

KM=tKmt;
t.clear();
tt.clear();
Kmt.clear();
tKmt.clear();
mr.clear();
return (KM);
}

Vec<int> E_Object2R::GetSteerVec3d()
{
Vec<int> V(2*3);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[1]->dof[0];
*V.nn(5)=pVertex[1]->dof[1];
*V.nn(6)=pVertex[1]->dof[2];

return(V);
}

Vec<int> E_Object2R::GetSteerVec1d()
{
Vec<int> V(2*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
return(V);
}




void E_Object2R::SetSec(int iA,int iB,int iC)
{
A=iA;
B=iB;
C=iC;
}

void E_Object2R::ExportNAS(FILE* pFile)
{
C3dMatrix TMat;
if (iType==11)
{
  fprintf(pFile,"%8s%8i%8i%8i%8i\n","CROD    ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel);
}
}

CString E_Object2R::ToString()
{
CString sRT;
char S1[80];
if (iType==11)
{
  sprintf_s(S1,"%8s%8i%8i%8i%8i\n","CROD    ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel);
}
sRT=S1;
return (sRT);
}


void E_Object2R::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
if ((iType==21) || (iType==22))
{
  fprintf(pFile,"%10i%10i%10i\n",A,B,C);
}
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}



void E_Object2R::ExportUPVecs(FILE* pFile)
{
double dA1;
C3dVector Vz;
C3dVector Vy;
C3dVector Vx;
C3dVector Vu;
C3dMatrix M;
if (A==0)
{
  M.MakeUnit();
  Vu.Set(vUp.x,vUp.y,vUp.z);
	Vz.x=pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
	Vz.y=pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
	Vz.z=pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;
	Vz.Normalize();
	Vy.Set(0,0,1);
	double dDot;
	dDot = abs(Vy.Dot(Vz));
	if (dDot==1) 
	{
	  Vx.Set(0,1,0);
	  Vy=Vz.Cross(Vx);
	  Vy.Normalize();
	  Vx=Vy.Cross(Vz);
	  Vx.Normalize();

	}
	else
	{
	  Vx=Vy.Cross(Vz);
	  Vx.Normalize();
	  Vy=Vz.Cross(Vx);
	  Vy.Normalize();
	}
	M.SetColVec(1,Vx);
	M.SetColVec(2,Vy);
	M.SetColVec(3,Vz);
	M.Transpose();
	Vu=M.Mult(Vu);
	if (Vu.y!=0)
	{
	   dA1=atan2(Vu.x,Vu.y);
	   dA1=dA1*180/3.1415926535897932384626433832795;
	}
	else
	{
		dA1=0;
	}
	dA1+=90.0;
	fprintf(pFile,"%10i%10i\n",iLabel,21);
	fprintf(pFile,"%10i%10i%10i%10i%10i\n",8,0,0,0,0);
	fprintf(pFile,"%15.6E%15.6E%15.6E\n",dA1,0.0,0.0);
}
else
{
	fprintf(pFile,"%10i%10i\n",iLabel,21);
	fprintf(pFile,"%10i%10i%10i%10i%10i\n",0,1,0,0,0);
}
}

CString E_Object2R::GetName()
{
	return ("Rod (CROD)");
}
//----------------------------------------------------------------------------
//    CBEAM E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(E_Object2B, CObject )

E_Object2B::E_Object2B()
{
iDOFA=0;
iDOFB=0;
iONID = -1;
iType=21;
this->iNoNodes=2;
}

G_Object* E_Object2B::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2B* gret = new E_Object2B;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
gret->A=A;
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->iONID=iONID;
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object2B::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object2B* gret = new E_Object2B;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
Pt_Object* pT=Source->GetNode(iONID);
if (pT!=NULL)
{
  EInd=Source->GetNodeInd(pT);
  gret->iONID=Target->pNodes[iSInd+EInd]->iLabel;
}
else
{
  gret->iONID=-1;
}
Pt_Object* nA=Source->GetNode(A);
if (nA!=NULL)
{
  EInd=Source->GetNodeInd(nA);
  gret->A = Target->pNodes[EInd+iSInd]->iLabel;  //Orientation Node
}
else
{
  gret->A=A;
}
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->pPr=NULL;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object2B::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object2B* gret = new E_Object2B;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->vUp=vUp;
gret->OffA=OffA;
gret->OffB=OffB;
gret->A=A;
gret->B=B;
gret->C=C;
gret->iDOFA=iDOFA;
gret->iDOFB=iDOFB;
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

void E_Object2B::ExportNAS(FILE* pFile)
{
C3dMatrix TMat;
if (iLabel== 29007)
  iLabel= iLabel;
if (iType==21)
{
  fprintf(pFile,"%8s%8i%8i%8i%8i","CBAR    ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel);
}
else if (iType==22)
{
  fprintf(pFile,"%8s%8i%8i%8i%8i","CBEAM   ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel);
}
if (iONID>0)
{
  fprintf(pFile,"%8i\n",iONID);
}
else
{
  TMat.MakeUnit();
  if (pVertex[0]->OutSys>0)
  {
    ME_Object* pM = (ME_Object*) this->pParent;
    CoordSys* pSys=pM->GetSys(pVertex[0]->OutSys);
    if (pSys!=NULL)
    {
      TMat=pSys->GetTMat();
      TMat.Transpose();
    }
  }
  C3dVector vU;
  vU=TMat*vUp;
  fprintf(pFile,"%8s%8s%8s\n",e8(vU.x),e8(vU.y),e8(vU.z));
}
fprintf(pFile,"%8s%8s%8s","        ",GetDOFString(iDOFA),GetDOFString(iDOFB));

  TMat.MakeUnit();
  if (pVertex[0]->OutSys>0)
  {
    ME_Object* pM = (ME_Object*) this->pParent;
    CoordSys* pSys=pM->GetSys(pVertex[0]->OutSys);
    if (pSys!=NULL)
    {
      TMat=pSys->GetTMat();
      TMat.Transpose();
    }
  }
  C3dVector vD1;
  vD1=TMat*OffA;
  fprintf(pFile,"%8s%8s%8s",e8(vD1.x),e8(vD1.y),e8(vD1.z));
  TMat.MakeUnit();
  if (pVertex[0]->OutSys>0)
  {
    ME_Object* pM = (ME_Object*) this->pParent;
    CoordSys* pSys=pM->GetSys(pVertex[1]->OutSys);
    if (pSys!=NULL)
    {
      TMat=pSys->GetTMat();
      TMat.Transpose();
    }
  }
  C3dVector vD2;
  vD2=TMat*OffB;
  fprintf(pFile,"%8s%8s%8s\n",e8(vD2.x),e8(vD2.y),e8(vD2.z));

}

int E_Object2B::noDof()
{
return (6);
}


Mat E_Object2B::GetThermMat(PropTable* PropsT,MatTable* MatT)
{
char S1[80];
double A;
double K;
int MID;
int iDof=1;
Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if ((pS!=NULL) && (pM!=NULL))
{
  if (pS->iType==11)
  {
    PROD* pB=(PROD*) pS;
    MAT1* pM1=(MAT1*) pM;
    A=pB->A;
    K=pM1->dk;
  }
  else if (pS->iType==5)
  {
    PBARL* pB=(PBARL*) pS;
    MAT1* pM1=(MAT1*) pM;
    A=pB->A;
    K=pM1->dk;
  }
  else if (pS->iType == 4)
  {
	  PBAR* pB = (PBAR*)pS;
	  MAT1* pM1 = (MAT1*)pM;
	  A = pB->dA;
	  K = pM1->dk;
  }
  else
  {
    sprintf_s(S1,"ERROR INVALID PROPERTY FOR EL %i",iLabel);
    outtext1(S1);
  }
}
else
{
  sprintf_s(S1,"ERROR UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
  outtext1(S1); 
}

Mat KM(iDof*iNoNodes,iDof*iNoNodes);
C3dVector p1;
C3dVector p2;
double l;
p1=pVertex[0]->GetCoords();
p2=pVertex[1]->GetCoords();
p2-=p1;
l=p2.Mag();
*KM.mn(1,1)=K*A/l;
*KM.mn(2,1)=-K*A/l;
*KM.mn(1,2)=-K*A/l;
*KM.mn(2,2)=K*A/l;
//KM.diag();
return (KM);
}

Mat E_Object2B::GetStiffMat(PropTable* PropsT,MatTable* MatT)
{
BOOL AX = FALSE;
BOOL BX = FALSE;
Mat KM(12,12);
Mat t(12,12);
Mat tt(12,12);
KM.MakeZero();
t.MakeZero();
tt.MakeZero();
double ea=50000;
double eiy=10000;
double eiz=10000;
double gj=10000;
double x1,y1,z1,x2,y2,z2;
double a1,a2,a3,a4,a5,a6,a7,a8;
double ell;
int MID=-1;
char S1[80];
Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if (pS==NULL)
{
  sprintf_s(S1,"ERROR: PROPERTY NOT FOUND FOR EL %i",iLabel);
  outtext1(S1);
}
if (pM==NULL)
{  
  sprintf_s(S1,"ERROR: MATERIAL NOT FOUND FOR EL %i",iLabel);
  outtext1(S1); 
}
if ((pS!=NULL) && (pM!=NULL))
{
  if (pS->iType==5)
  {
	PBARL* pB=(PBARL*) pS;
	MAT1* pM1=(MAT1*) pM;
	ea=pB->A*pM1->dE;
	eiy=pB->Iyy*pM1->dE;
	eiz=pB->Izz*pM1->dE;
	double g;
	g=pM1->dE/(2*(1+pM1->dNU));
	gj=g*pB->J;

  }
  else if (pS->iType == 4)
  {
	PBAR* pB = (PBAR*)pS;
	MAT1* pM1 = (MAT1*)pM;
	ea = pB->dA*pM1->dE;
	eiy = pB->dI1*pM1->dE;
	eiz = pB->dI2*pM1->dE;
	double g;
	g = pM1->dE / (2 * (1 + pM1->dNU));
	gj = g * pB->dJ;
  }
  else
  {
    sprintf_s(S1,"ERROR: INVALID PROPERTY FOR EL %i",iLabel);
    outtext1(S1);
  }
}
else
{
  sprintf_s(S1,"ERROR: UNABLE TO CALCULATE PROPERTIES FOR EL %i",iLabel);
  outtext1(S1); 
}

x1=pVertex[0]->Pt_Point->x;
y1=pVertex[0]->Pt_Point->y;
z1=pVertex[0]->Pt_Point->z;
x2=pVertex[1]->Pt_Point->x;
y2=pVertex[1]->Pt_Point->y;
z2=pVertex[1]->Pt_Point->z;

x1-=x2;y1-=y2;z1-=z2;
ell=pow(x1*x1+y1*y1+z1*z1,0.5);


a1=ea/ell; a2=12.0*eiz/(ell*ell*ell);
a3=12.0*eiy/(ell*ell*ell);a4=6.0*eiz/(ell*ell);
a5=6.0*eiy/(ell*ell);a6=4.0*eiz/ell;
a7=4.0*eiy/ell;a8=gj/ell;
//End Releases

*KM.mn(1,1)=a1;
*KM.mn(7,7)=a1;
*KM.mn(1,7)=-a1;
*KM.mn(7,1)=-a1;
*KM.mn(2,2)=a2;
*KM.mn(8,8)=a2;
////**********Note sure about this yet*************
// This does not converge
//if (iDOFA & DOF_1) //End A release
//{
//	outtext1("End A X release");
//	*KM.mn(1, 1) = 1000;
//	*KM.mn(7, 1) = 1000;
//}
//***********************************************
*KM.mn(2,8)=-a2;
*KM.mn(8,2)=-a2;
*KM.mn(3,3)=a3;
*KM.mn(9,9)=a3;
*KM.mn(3,9)=-a3;
*KM.mn(9,3)=-a3;

*KM.mn(4,4)=a8;
*KM.mn(10,10)=a8;
*KM.mn(4,10)=-a8;
*KM.mn(10,4)=-a8;
*KM.mn(5,5)=a7;

*KM.mn(11,11)=a7;
*KM.mn(5,11)=0.5*a7;
*KM.mn(11,5)=0.5*a7;
*KM.mn(6,6)=a6;
*KM.mn(12,12)=a6;

*KM.mn(6,12)=0.5*a6;
*KM.mn(12,6)=0.5*a6;
*KM.mn(2,6)=a4;
*KM.mn(6,2)=a4;
*KM.mn(2,12)=a4;

*KM.mn(12,2)=a4;
*KM.mn(6,8)=-a4;
*KM.mn(8,6)=-a4;
*KM.mn(8,12)=-a4;
*KM.mn(12,8)=-a4;

*KM.mn(5,9)=a5;
*KM.mn(9,5)=a5;
*KM.mn(9,11)=a5;
*KM.mn(11,9)=a5;
*KM.mn(3,5)=-a5;
*KM.mn(5,3)=-a5;
*KM.mn(3,11)=-a5;
*KM.mn(11,3)=-a5;
C3dMatrix r;
r=GetElSys();
Mat mr(3,3);
*mr.mn(1,1)=r.m_00; *mr.mn(1,2)=r.m_01; *mr.mn(1,3)=r.m_02;
*mr.mn(2,1)=r.m_10; *mr.mn(2,2)=r.m_11; *mr.mn(2,3)=r.m_12;
*mr.mn(3,1)=r.m_20; *mr.mn(3,2)=r.m_21; *mr.mn(3,3)=r.m_22;


int i,j;
for (i=1;i<4;i++)
{
  for (j=1;j<4;j++)
  {
     *t.mn(i,j) = *mr.mn(i,j);
     *t.mn(i+3,j+3) = *mr.mn(i,j);
     *t.mn(i+6,j+6) = *mr.mn(i,j);
     *t.mn(i+9,j+9) = *mr.mn(i,j);
     *tt.mn(i,j) = *mr.mn(i,j);
     *tt.mn(i+3,j+3) = *mr.mn(i,j);
     *tt.mn(i+6,j+6) = *mr.mn(i,j);
     *tt.mn(i+9,j+9) = *mr.mn(i,j);
  }
}
tt.Transpose();
Mat Kmt;
Mat tKmt;
Kmt=KM*t;
tKmt=tt*Kmt;

KM=tKmt;
t.clear();
tt.clear();
Kmt.clear();
tKmt.clear();
mr.clear();
return (KM);
}

Vec<int> E_Object2B::GetSteerVec3d()
{
Vec<int> V(2*6);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[0]->dof[3];
*V.nn(5)=pVertex[0]->dof[4];
*V.nn(6)=pVertex[0]->dof[5];
*V.nn(7)=pVertex[1]->dof[0];
*V.nn(8)=pVertex[1]->dof[1];
*V.nn(9)=pVertex[1]->dof[2];
*V.nn(10)=pVertex[1]->dof[3];
*V.nn(11)=pVertex[1]->dof[4];
*V.nn(12)=pVertex[1]->dof[5];
return(V);
}

Vec<int> E_Object2B::GetSteerVec1d()
{
Vec<int> V(2*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
return(V);
}

CString E_Object2B::GetName()
{
	return("Beam (CBAR)");
}

IMPLEMENT_DYNAMIC( E_Object3, CObject )
//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//-
E_Object3::~E_Object3()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
}

E_Object3::E_Object3()
{
this->iNoNodes=3;
this->iType=91;
this->pParent=NULL;
}

void E_Object3::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,int inMCys,double inMAng,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
iMCys=inMCys;
MAng=inMAng;
int i=0;
for (i=0;i<3;i++)
   {
    pVertex[i] = pInVertex[i];
   }
dZOFFS=0;
}

void E_Object3::Reverse()
{
Pt_Object* pT[8];
pT[0]=pVertex[0];
pT[1]=pVertex[1];
pT[2]=pVertex[2];
pVertex[0] = pT[0];
pVertex[1] = pT[2];
pVertex[2] = pT[1];
}


void E_Object3::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}

}

BOOL E_Object3::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object3::GetKMat()
{
//No integeration needed for
//this element as constant strain	

//Bmem[0][0] = Y2 - Y3;
//Bmem[0][1] = 0;
//Bmem[0][2] = Y3 - Y1;
//Bmem[0][3] = 0;
//Bmem[0][4] = Y1 - Y2;
//Bmem[0][5] = 0;

//Bmem[1][0] = 0;
//Bmem[1][1] = X3 - X2;
//Bmem[1][2] = 0;
//Bmem[1][3] = X1 - X3;
//Bmem[1][4] = 0;
//Bmem[1][5] = X2 - X1;

//Bmem[2][0] = X3 - X2;
//Bmem[2][1] = Y2 - Y3;
//Bmem[2][2] = X1 - X3;
//Bmem[2][3] = Y3 - Y1;
//Bmem[2][4] = X2 - X1;
//Bmem[2][5] = Y1 - Y2;
}

void E_Object3::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar<<iMCys;
    ar<<MAng;
    ar<<dZOFFS;
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    ar>>iMCys;
    ar>>MAng;
    ar>>dZOFFS;
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
	}
  pPr=NULL;
}

void E_Object3::ExportNAS(FILE* pFile)
{
fprintf(pFile,"%8s",ToString());
}

void E_Object3::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}

G_Object* E_Object3::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object3* gret = new E_Object3;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pParent=MESH;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object3::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object3* gret = new E_Object3;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;

EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];

gret->pParent=Target;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=NULL;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object3::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object3* gret = new E_Object3;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->pVertex[2] = pInVertex[2];
gret->pParent=Parrent;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object3::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
}

int E_Object3::GetLinkList(cLink* Links[200])
{
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
Links[0]->iColour=iColour;
Links[1]=new cLink;
Links[1]->pParent=this;
Links[1]->pVertex[0]=pVertex[1];
Links[1]->pVertex[1]=pVertex[2];
Links[1]->iColour=iColour;
Links[2]=new cLink;
Links[2]->pParent=this;
Links[2]->pVertex[0]=pVertex[2];
Links[2]->pVertex[1]=pVertex[0];
Links[2]->iColour=iColour;
return (3);
}

G_Object* E_Object3::GetNode(int i)
{
return (pVertex[i]);
}

void E_Object3::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector d[3];
int i;
int ind;
for (i=0;i<3;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;

S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
ind=ME->iCVar;
glLineWidth(1.0);
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<3;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S* dFS;
       }
    }
  }
Selectable=1;
  if ((iDspFlgs & DSP_ELEMENTS)>0)
  {

    if ((iDspFlgs & DSP_LINE) > 0)
    {
      glColor3fv(cols[iColour]);
    }
    else
    {
      glColor3fv(cols[0]);
    }
    C3dVector vN;
    C3dVector vOff;
    vN=Get_Normal();
    //Need to calculate the offsets here
    if (((iDspFlgs & DSP_OFF)>0) && (dZOFFS!=DBL_MAX))
    {
       vOff=vN;
       vOff*=dZOFFS;
    }
    else
    {
       vOff*=0; 
    }
    double dt=0;
    if (pPr!=NULL)
    {
      dt=pPr->GetThk();
      dt*=0.5;
    }
    vN*=dt;
    if ((iDspFlgs & DSP_THK)>0)
    {
  	  glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	   
        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));

        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
        glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
        glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
      glEnd();
    }
    else
    {
      vN*=0;
      glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
      glEnd();
    }
	  C3dVector vCent;
    vCent=Get_Centroid();
    if (bDrawLab==TRUE)
	  {
	    sprintf_s(sLab,"E%i",iLabel);
	    OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	  }
    if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
    {
       sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
       OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
    }
  }
  if ((iDspFlgs & DSP_ELSYS)==0)
  {
	glColor3fv(cols[iColour]);
    C3dMatrix mS = GetElSys();
    C3dVector vC = Get_Centroid();
    mS.Transpose();
    C3dVector vX=mS.GetColVec(1);
    C3dVector vY=mS.GetColVec(2);;
    C3dVector vZ=mS.GetColVec(3);;

    vX*=0.5*dS1; vY*=0.5*dS1; vZ*=0.5*dS1;
    vX+=vC;vY+=vC;vZ+=vC;

    glBegin(GL_LINES);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vX.x,(float) vX.y,(float) vX.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vY.x,(float) vY.y,(float) vY.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glEnd();
    glRasterPos3f ((float) vX.x,(float) vX.y,(float) vX.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
    glRasterPos3f ((float) vY.x,(float) vY.y,(float) vY.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
    glRasterPos3f ((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }
  if ((iDspFlgs & DSP_MATL)==0)
  {
    C3dMatrix mS = GetElSys();
	C3dMatrix mR;

    C3dVector vD = GetFirstEdge();
	C3dVector vC = Get_Centroid();
	vD.Normalize();
	mR.Rotate(0,0,MAng);
	vD=mS*vD;
	vD=mR*vD;
	mS.Transpose();
	vD=mS*vD;
	vD*=0.5*dS1;
    vD+=vC;
    glBegin(GL_LINES);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vD.x,(float) vD.y,(float) vD.z);
    glEnd();
  }
}
else
{
  Selectable=0;
}
}

void E_Object3::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector d[3];
int i;
for (i=0;i<3;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS;
dFS = ME->dResFactor;
glLineWidth(2.0);
      BOOL bD=FALSE;
      float fCols[3]={0,0,0};
      int iVar;
      iVar=ME->iCVar;
      //Nodal data
      if ((pVertex[0]->pResV != NULL) &&
          (pVertex[1]->pResV != NULL) &&
          (pVertex[2]->pResV != NULL))
      {
        bD=TRUE;
        fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
        fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
        fCols[2]=GetContourCol(*pVertex[2]->pResV->GetAddress(iVar)*dFS);
      }
      if (pResV!=NULL)
      {
        bD=TRUE;
        fCols[0]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
        fCols[1]=fCols[0];
        fCols[2]=fCols[0];
      }

C3dVector v1;
C3dVector v2;
C3dVector Vn;

C3dVector vN;
C3dVector vOff;
vN=Get_Normal();
if (((iDspFlgs & DSP_OFF)>0) && (dZOFFS!=DBL_MAX))
{
  vOff=vN;
  vOff*=dZOFFS;
}
else
{
  vOff*=0; 
}

double dt=0;
if (pPr!=NULL)
{
  dt=pPr->GetThk();
  dt*=0.5;
}
vN*=dt;
Vn =Get_Normal();
Vn.Normalize();

if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<3;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S * dFS;
       }
    }
  }
	Selectable=1;
	glColor3fv(cols[iColour]);
  if (((iDspFlgs & DSP_CONT)>0) || (bD==FALSE))
  {
    if ((iDspFlgs & DSP_THK)>0)
    {
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glEnd();
      glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
      glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	  glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
        glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	  glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
        glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	  glEnd();

    }
    else
    {
      vN*=0;
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glEnd();
    }
  }
  else
  {
      vN*=0;

      if (bD)
      {
      glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);
	    glBegin(GL_POLYGON);
        glTexCoord1f(fCols[0]);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
        glTexCoord1f(fCols[1]);
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
        glTexCoord1f(fCols[2]);
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glEnd();
      glDisable(GL_TEXTURE_1D);
      }
  }
}
else
{
  Selectable=0;
}
}

int E_Object3::GetfaceList(cFace* Faces[6])
{
Faces[0]=new cFace();
Faces[0]->pParent=this;
Faces[0]->pVertex[0]=pVertex[0];
Faces[0]->pVertex[1]=pVertex[1];
Faces[0]->pVertex[2]=pVertex[2];
Faces[0]->NoVert=3;
Faces[0]->iColour=GetCol();
return(1);
}

//void E_Object3::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
//{
////int j=0;
////for (j=0;j<iNoNodes;j++)
////{
////   pVertex[j]->SetToScr(pModMat,pScrTran);
////}
//C3dVector vC;
//vC=Get_Centroid();
//vC.SetToScr(pModMat,pScrTran);
//SelPt=vC;
//
//}



double E_Object3::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  double T = 0;
  int iDOFID;
  int j;
  Mat fun;
  Mat FunPnt(1, 2);
  *FunPnt.mn(1, 1) = 0.333;
  *FunPnt.mn(1, 2) = 0.333;
  fun = this->ShapeFun(FunPnt, 1);
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T**fun.mn(1, j + 1);
  }
  fun.clear();
  FunPnt.clear();
  return(dTemp);
}

C3dVector E_Object3::Get_Centroid()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0.333;
*FunPnt.mn(1,2)=0.333;
fun=ShapeFun(FunPnt,1);
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}
fun.clear();
FunPnt.clear();
return (vT);
}






C3dMatrix E_Object3::GetElSys()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0.33;
*FunPnt.mn(1,2)=0.33;
fun=ShapeDer(FunPnt,1);
C3dVector vX,vY,vT;
vX.Set(0,0,0);
vY.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
   vY+=pVertex[j]->Get_Centroid()**fun.mn(2,j+1);
}
vT=vX.Cross(vY);
vT.Normalize();
vX=pVertex[1]->GetCoords();
vX-=pVertex[0]->GetCoords();
vX.Normalize();
vY=vT.Cross(vX);
fun.clear();
FunPnt.clear();
C3dMatrix vR;
vR.SetColVec(1,vX);
vR.SetColVec(2,vY);
vR.SetColVec(3,vT);
vR.Transpose();
return (vR);
}

C3dVector E_Object3::GetFirstEdge()
{

C3dVector vD;
vD.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
vD.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
vD.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

return (vD);
}


void E_Object3::GetBoundingBox(C3dVector& vll,C3dVector& vur)
{
  int i;
  vll.x=pVertex[0]->Pt_Point->x; vll.y=pVertex[0]->Pt_Point->y; vll.z=pVertex[0]->Pt_Point->z;
  vur.x=pVertex[0]->Pt_Point->x; vur.y=pVertex[0]->Pt_Point->y; vur.z=pVertex[0]->Pt_Point->z;



  for(i=0;i<3;i++)
  {
    if (pVertex[i]->Pt_Point->x<vll.x)
      vll.x=pVertex[i]->Pt_Point->x;
	if (pVertex[i]->Pt_Point->x>vur.x)
      vur.x=pVertex[i]->Pt_Point->x;

	if (pVertex[i]->Pt_Point->y<vll.y)
      vll.y=pVertex[i]->Pt_Point->y;
	if (pVertex[i]->Pt_Point->y>vur.y)
      vur.y=pVertex[i]->Pt_Point->y;

	if (pVertex[i]->Pt_Point->z<vll.z)
      vll.z=pVertex[i]->Pt_Point->z;
	if (pVertex[i]->Pt_Point->z>vur.z)
      vur.z=pVertex[i]->Pt_Point->z;
  }
}


double E_Object3::QualAspect()
{
  double drc = 1;
  C3dVector v1;
  C3dVector v2;
  double de1;
  double de2;
  double de3;
  double deMax;
  double deMin;
  v1 = pVertex[0]->Get_Centroid();
  v2 = pVertex[1]->Get_Centroid();
  de1 = (v1 - v2).Mag();
  v1 = pVertex[1]->Get_Centroid();
  v2 = pVertex[2]->Get_Centroid();
  de2 = (v1 - v2).Mag();
  v1 = pVertex[2]->Get_Centroid();
  v2 = pVertex[0]->Get_Centroid();
  de3 = (v1 - v2).Mag();
  //Find Maximum value
  deMax = de1;
  if (de2 > deMax)
    deMax = de2;
  if (de3 > deMax)
    deMax = de3;
  //Minimum
  deMin = de1;
  if (de2 < deMin)
    deMin = de2;
  if (de3 < deMin)
    deMin = de3;

  if (deMin != 0)
    drc = deMax / deMin;
  else
    drc = 100000;
  return(drc);
}


double E_Object3::GetCharSize()
{
double dS;
C3dVector vT;
C3dVector vT2;
vT=pVertex[0]->Pt_Point;
vT2=pVertex[1]->Pt_Point;
vT-=vT2;
dS=vT.Mag();
vT=pVertex[1]->Pt_Point;
vT2=pVertex[2]->Pt_Point;
vT-=vT2;
dS+=vT.Mag();
vT=pVertex[2]->Pt_Point;
vT2=pVertex[0]->Pt_Point;
vT-=vT2;
dS+=vT.Mag();
dS/=3;
//dS=(pVertex[0]->Pt_Point-pVertex[1]->Pt_Point)->Mag();
return (dS);
}

C3dVector E_Object3::GetTestPt()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=1;
*FunPnt.mn(1,2)=0;
fun=ShapeFun(FunPnt,1);
C3dVector vX;
vX.Set(0,0,0);

int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}
return (vX);
}


void E_Object3::Info()
{
  char S1[80];
  sprintf_s(S1,"LAB: %i COL: %i PID: %i ELTP: %i MCID: %i ANG: %f ",iLabel,iColour,PID,iType,iMCys,MAng);
  outtext1(S1); 
  sprintf_s(S1,"NODES %i %i %i",pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel);
  outtext1(S1); 
}

CString E_Object3::ToString()
{
CString sRT;
char S1[80];
CString sDir;
if (iMCys==-1)
{
  sDir=e8(MAng);
}
else
{
  sprintf_s(S1,"%8i",iMCys);
  sDir=S1;
}
sprintf_s(S1,"%8s%8i%8i%8i%8i%8i%8s%8s\n","CTRIA3  ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel,sDir,e8(dZOFFS));
sRT=S1;
return(sRT);
}

C3dVector E_Object3::Get_Normal()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0.333;
*FunPnt.mn(1,2)=0.333;
fun=ShapeDer(FunPnt,1);
C3dVector vX,vY,vT;
vX.Set(0,0,0);
vY.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
   vY+=pVertex[j]->Get_Centroid()**fun.mn(2,j+1);
}
vT=vX.Cross(vY);
fun.clear();
FunPnt.clear();
vT.Normalize();
return (vT);
}



//void E_Object3::HighLight(CDC* pDC)
//{
////CPoint cPt1;
////CPoint cPt2;
////cPt1.x = (long) 0.5*((long) pVertex[2]->DSP_Point->x-(long) pVertex[0]->DSP_Point->x)+(long) pVertex[0]->DSP_Point->x;
////cPt1.y = (long) 0.5*((long) pVertex[2]->DSP_Point->y-(long) pVertex[0]->DSP_Point->y)+(long) pVertex[0]->DSP_Point->y;
////cPt2.x = (long) (0.3*((long) pVertex[1]->DSP_Point->x-cPt1.x)+cPt1.x);
////cPt2.y = (long) (0.3*((long) pVertex[1]->DSP_Point->y-cPt1.y)+cPt1.y);
//pDC->Ellipse(SelPt.x+5,SelPt.y+5,SelPt.x-5,SelPt.y-5);
//}


//G_ObjectD E_Object3::SelDist(CPoint InPT,Filter FIL)
//{
//G_ObjectD Ret;
//double x,y,d;
//x =  SelPt.x-InPT.x;
//y =  SelPt.y-InPT.y;
//d=(x*x+y*y);
//Ret.Dist=d;
//Ret.pObj=this;
//return (Ret);
//}



Mat E_Object3::Sample(int iNo)
{
Mat Pts(iNo,3);
if (iNo == 1)
{
  *Pts.mn(1,1) = (double) 1/3;
  *Pts.mn(1,2) = (double) 1/3;
  *Pts.mn(1,3) = (double) 0.5;
}
else if (iNo == 3)
{

  *Pts.mn(1,1) = (double) 0;
  *Pts.mn(1,2) = (double) 0.5;
  *Pts.mn(1,3) = (double) 1/6;

  *Pts.mn(2,1) = (double) 0.5;
  *Pts.mn(2,2) = (double) 0;
  *Pts.mn(2,3) = (double) 1/6;

  *Pts.mn(3,1) = (double) 0.5;
  *Pts.mn(3,2) = (double) 0.5;
  *Pts.mn(3,3) = (double) 1/6;
}
return (Pts);
}


int E_Object3::MaxBW()
{
int i;
int j;
int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<6;j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	{
       MaxDof=pVertex[i]->dof[j];
    }
	if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	{
       MinDof=pVertex[i]->dof[j];
	}
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}

// Used for MPM example for particle position in element
Mat E_Object3::GetParticlePos()
{
  Mat Cent(2, 1);
  C3dVector vC = this->Get_Centroid();
  *Cent.mn(1, 1) = vC.x;
  *Cent.mn(2, 1) = vC.y;
  return (Cent);
}


// Used for MPM example for area calc
Mat E_Object3::DetCoords2d()
{
  Mat DetCoord(iNoNodes, 3);

  *DetCoord.mn(1, 1) = 1.0;
  *DetCoord.mn(1, 2) = 1.0;
  *DetCoord.mn(1, 3) = 1.0;

  *DetCoord.mn(2, 1) = pVertex[0]->Pt_Point->x;
  *DetCoord.mn(2, 2) = pVertex[1]->Pt_Point->x;
  *DetCoord.mn(2, 3) = pVertex[2]->Pt_Point->x;

  *DetCoord.mn(3, 1) = pVertex[0]->Pt_Point->y;
  *DetCoord.mn(3, 2) = pVertex[1]->Pt_Point->y;
  *DetCoord.mn(3, 3) = pVertex[2]->Pt_Point->y;

  return (DetCoord);
}


Mat E_Object3::getCoords2d()
{
int i;
Mat coord(iNoNodes,2);
for (i=0;i<iNoNodes;i++)
{
  *coord.mn(i+1, 1) = pVertex[i]->Pt_Point->x;
  *coord.mn(i+1, 2) = pVertex[i]->Pt_Point->y;
}
return (coord);
}

//*******************************************************************
//Get The coordinates after transforming the element to the XY plain
//********************************************************************
Mat E_Object3::getCoords3d()
{
int i;
C3dVector p;
Mat coord(iNoNodes,2);
C3dMatrix M3=this->GetElSys();
//M3.Transpose();
for (i=0;i<iNoNodes;i++)
{
  C3dVector p,v;
  p.x=pVertex[i]->Pt_Point->x;
  p.y=pVertex[i]->Pt_Point->y;
  p.z=pVertex[i]->Pt_Point->z;
  v=M3.Mult(p);
  *coord.mn(i+1,1)=v.x;
  *coord.mn(i+1,2)=v.y;
}
return (coord);
}


Mat E_Object3::ShapeDer(Mat Points, int i)
{
Mat der(2,3);
*der.mn(1, 1) = 1;
*der.mn(1, 2) = 0;
*der.mn(1, 3) = -1;
*der.mn(2, 1) = 0;
*der.mn(2, 2) = 1;
*der.mn(2, 3) = -1;
return(der);
}

Vec<int> E_Object3::GetSteerVec3d()
{
Vec<int> V(18);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[0]->dof[3];
*V.nn(5)=pVertex[0]->dof[4];
*V.nn(6)=pVertex[0]->dof[5];

*V.nn(7)=pVertex[1]->dof[0];
*V.nn(8)=pVertex[1]->dof[1];
*V.nn(9)=pVertex[1]->dof[2];
*V.nn(10)=pVertex[1]->dof[3];
*V.nn(11)=pVertex[1]->dof[4];
*V.nn(12)=pVertex[1]->dof[5];

*V.nn(13)=pVertex[2]->dof[0];
*V.nn(14)=pVertex[2]->dof[1];
*V.nn(15)=pVertex[2]->dof[2];
*V.nn(16)=pVertex[2]->dof[3];
*V.nn(17)=pVertex[2]->dof[4];
*V.nn(18)=pVertex[2]->dof[5];

return(V);
}

Vec<int> E_Object3::GetSteerVec1d()
{
Vec<int> V(3*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
*V.nn(3)=pVertex[2]->dof[0];
return(V);
}


Mat E_Object3::ShapeFun(Mat Points, int i)
{
Mat fun(1,3);
double xi,eta;
double c1,c2,c3;
eta = *Points.mn(i, 2);
xi = *Points.mn(i, 1);
c1=xi;
c2=eta;
c3=1-xi-eta;

*fun.mn(1,1) = c1;
*fun.mn(1,2) = c2;
*fun.mn(1,3) = c3;
return(fun);
}

int E_Object3::noDof()
{
return(6);
}

Mat E_Object3::GetStiffMat(PropTable* PropsT,MatTable* MatT)
{
Mat bee;   //strain displacement matrix
int nip=0;

Mat coord2;
Mat deriv;
Mat deriv2;
Mat fun;
Mat Points;
Mat jac;
int i;
double det;
Mat bT;
Mat db;
Mat bdb;
int iDof;
int iS;
int MID=-1;
double dE=210e9;
double dv=0.29;
char S1[80];
double dthk=0.001;

Property* pS=PropsT->GetItem(PID);
if (pS!=NULL)
{
  MID=pS->GetDefMatID();
}
Material* pM=MatT->GetItem(MID);
if (pS==NULL)
{
  sprintf_s(S1,"ERROR: PROPERTY NOT FOUND FOR EL %i",iLabel);
  outtext1(S1);
}
if (pM==NULL)
{  
  sprintf_s(S1,"ERROR: MATERIAL NOT FOUND FOR EL %i",iLabel);
  outtext1(S1); 
}
//Get Shell thick ness

if (((iType==91) || (iType==94)) && (pS!=NULL))
{
  PSHELL* pSh = (PSHELL*) pS;
  dthk=pSh->dT;
}

if ((pM!=NULL) && (pM->iType = 1))
{
  MAT1* pIsen = (MAT1*) pM;
  dE=pIsen->dE;
  dv=pIsen->dNU;
}
//This part calculates the 2d membraine stiffness
iDof=2;   //2 dof X,Y per node
nip=1;    //1 intergration points
iS=3;

Mat KM(iDof*iNoNodes,iDof*iNoNodes);
Mat dee=DeeMat(dE,dv,iS);
Mat coord=getCoords3d();
C3dMatrix M3=this->GetElSys();
Points=Sample(nip);
//Membraine stiffness calculation
//for dof 12
  for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun.clear();deriv.clear();jac.clear(),deriv2.clear();
    bT.clear();db.clear();bdb.clear();bee.clear();
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee=bmat(coord, deriv2,iS,iDof);
    bT = bee;
    bT.Transpose();
    db = dee*bee;
    bdb = bT*db;
    det*=*Points.mn(i,3);
    bdb*=det;
    KM+=bdb;
  }
KM*=dthk;
fun.clear();deriv.clear();jac.clear(),deriv2.clear();
bT.clear();db.clear();bdb.clear();bee.clear();
//Mindlin Formulation
//Sheer Stifness Calculation
//I think need to be reduced integration 1 sample point
double G,kk;
int k;
dee.clear();
dee.Create(2,2);
G=0.5*dE/(1+dv);
kk=5.0/6.0;
*dee.mn(1,1) = kk*G*dthk;
*dee.mn(2,2) = kk*G*dthk;
*dee.mn(1,2) = 0;
*dee.mn(2,1) = 0;
Mat KM2(12,12); 
nip=1;
Points=Sample(nip);
for(i=1;i<nip+1;i++)
  {
    det = 0;
    fun.clear();deriv.clear();jac.clear(),deriv2.clear();
    bT.clear();db.clear();bdb.clear();bee.clear();
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee.clear();
    bee.Create(2,12);
    for (k=1;k<iNoNodes+1;k++)//was m
    {
      *bee.mn(1,(k)*3-2) = *deriv2.mn(1,k);
      *bee.mn(1,(k)*3-1) = 0;
      *bee.mn(1,(k)*3-0) = *fun.mn(1,k);
      *bee.mn(2,(k)*3-2) = *deriv2.mn(2,k);
      *bee.mn(2,(k)*3-1) = -*fun.mn(1,k);
      *bee.mn(2,(k)*3-0) = 0;
    }
    bT = bee;
    bT.Transpose();
    db = dee*bee;
    bdb = bT*db;
    det*=*Points.mn(i,3);
    bdb*=det;
    KM2+=bdb;
  }
fun.clear();deriv.clear();jac.clear(),deriv2.clear();
bT.clear();db.clear();bdb.clear();bee.clear();
//The bending part
dee.clear();
dee.Create(3,3);
double Cf;
Cf=dE*dthk*dthk*dthk/(12*(1-dv*dv));
//nip should be 3 set to 1 for debugging
nip=1;
//*******************************************************************
Points=Sample(nip);
*dee.mn(1,1) = 1;
*dee.mn(1,2) = dv;
*dee.mn(1,3) = 0;
*dee.mn(2,1) = dv;
*dee.mn(2,2) = 1;
*dee.mn(2,3) = 0;
*dee.mn(3,1) = 0;
*dee.mn(3,2) = 0;
*dee.mn(3,3) = (1-dv);
dee*=Cf;
Mat KM3(12,12); 
for(i=1;i<nip+1;i++)
  {
    fun.clear();deriv.clear();jac.clear(),deriv2.clear();
    bT.clear();db.clear();bdb.clear();bee.clear();
    det = 0;
    fun=ShapeFun(Points, i);
    deriv=ShapeDer(Points, i);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee.clear();
    bee.Create(3,12);
    for (k=1;k<iNoNodes+1;k++)//was m
    {
      *bee.mn(1,(k)*3-2) = 0;
      *bee.mn(1,(k)*3-1) = 0;
      *bee.mn(1,(k)*3-0) = -*deriv2.mn(1,k);
      *bee.mn(2,(k)*3-2) = 0;
      *bee.mn(2,(k)*3-1) = *deriv2.mn(2,k);
      *bee.mn(2,(k)*3-0) = 0;
      *bee.mn(3,(k)*3-2) = 0;
      *bee.mn(3,(k)*3-1) = *deriv2.mn(1,k);
      *bee.mn(3,(k)*3-0) = -*deriv2.mn(2,k);
    }
    bT = bee;
    bT.Transpose();
    db = dee*bee;
    bdb = bT*db;
    det*=*Points.mn(i,3);
    bdb*=det;
    KM3+=bdb;
  }
KM2+=KM3;
//KM2 = KM3;
fun.clear();deriv.clear();jac.clear(),deriv2.clear();
bT.clear();db.clear();bdb.clear();bee.clear();
//Assemble all the components inplain,shear and bending in KMf
Mat KMf(6*iNoNodes,6*iNoNodes);
Vec<int> V(6);
*V.nn(1)=1;
*V.nn(2)=2;
*V.nn(3)=7;
*V.nn(4)=8;
*V.nn(5)=13;
*V.nn(6)=14;

int j;
for (i=1;i<=6;i++)
{
  for (j=1;j<=6;j++)
  {
    *KMf.mn(V[i-1],V[j-1])=*KM.mn(i,j);
  }
}
Vec<int> V1(9);
*V1.nn(1)=3;
*V1.nn(2)=4;
*V1.nn(3)=5;
*V1.nn(4)=9;
*V1.nn(5)=10;
*V1.nn(6)=11;
*V1.nn(7)=15;
*V1.nn(8)=16;
*V1.nn(9)=17;

for (i=1;i<=9;i++)
{
  for (j=1;j<=9;j++)
  {
    *KMf.mn(V1[i-1],V1[j-1])=*KM2.mn(i,j);
  }
}

for (i=6;i<=18;i+=6)
{
  *KMf.mn(i,i)=100;  //DRILLING STIFFNES
}
//Transform to global
//M3.Transpose();
Mat TMAT(18,18);

for (i=1;i<18;i+=3)
{
    *TMAT.mn(i,i)=M3.m_00;
    *TMAT.mn(i+1,i)=M3.m_10;
    *TMAT.mn(i+2,i)=M3.m_20;

    *TMAT.mn(i,i+1)=M3.m_01;
    *TMAT.mn(i+1,i+1)=M3.m_11;
    *TMAT.mn(i+2,i+1)=M3.m_21;

    *TMAT.mn(i,i+2)=M3.m_02;
    *TMAT.mn(i+1,i+2)=M3.m_12;
    *TMAT.mn(i+2,i+2)=M3.m_22;
}
//TMAT.diag();
Mat TMATT=TMAT;
TMATT.Transpose();
Mat T;
Mat TT;
T=KMf*TMAT;
TT=TMATT*T;
KM.clear();
KM2.clear();
TMATT.clear();
TMAT.clear();
T.clear();
return (TT);
}


void E_Object3::TranslateAVF(C3dVector vIn)
{
	pVertex[0]->Translate(vIn);
	pVertex[1]->Translate(vIn);
	pVertex[2]->Translate(vIn);

}
void E_Object3::TransformAVF(C3dMatrix TMat)
{
	pVertex[0]->Transform(TMat);
	pVertex[1]->Transform(TMat);
	pVertex[2]->Transform(TMat);
}

CString E_Object3::GetName()
{
	return ("Tri Shell (CTRIA3)");
}

int E_Object3::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "Mat CYS";
	iNo++;
	sVar[iNo] = "Mat Angle";
	iNo++;
	sVar[iNo] = "Z Offset";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	sVar[iNo] = "N3";
	iNo++;
	return(iNo);
}


int E_Object3::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iMCys);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", MAng);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dZOFFS);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[2]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object3::PutVarValues(PropTable* PT,int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;

	int newPID = atoi(sVar[0]);
	if (newPID != PID)
	{

		Property* pr = PT->GetItem(newPID);
		if (pr != NULL)
		{

			BOOL bC = SetProperty(pr);
			if (bC)
				outtext1("Property has Been Changed");
			else
				outtext1("Invalid Property");
		}

	}
	iMCys = atoi(sVar[1]);
	MAng = atof(sVar[2]);;
	dZOFFS = atof(sVar[3]);;;
	int N1 = atoi(sVar[4]);
	int N2 = atoi(sVar[5]);
	int N3 = atoi(sVar[6]);
	if (pVertex[0]->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex[0] = pN;

	}
	if (pVertex[1]->iLabel != N2)
	{
		pN = pMe->GetNode(N2);
		if (pN != NULL)
			pVertex[1] = pN;

	}
	if (pVertex[2]->iLabel != N3)
	{
		pN = pMe->GetNode(N3);
		if (pN != NULL)
			pVertex[2] = pN;

	}
}

//*************************************************************************************
// Pre: TRUE
// Post: returns area in Y & Y plain only for 2d analysis
//*************************************************************************************
double E_Object3::GetArea2d()
{
	C2dVector v, w;
	double dA;
	v.x = pVertex[1]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	v.y = pVertex[1]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	w.x = pVertex[2]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	w.y = pVertex[2]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	dA = 0.5*abs(v.Cross(w));
	return (dA);
}

//*********************************
IMPLEMENT_DYNAMIC( E_Object1, CObject )


E_Object1::E_Object1()
{

}

E_Object1::~E_Object1()
{
pVertex=NULL;
}

void E_Object1::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
pVertex = pInVertex[0];
iCID=0;
dM=0;
dX1=0;
dX2=0;
dX3=0;
dI11=0;
dI21=0;
dI22=0;
dI31=0;
dI32=0;
dI33=0;
}

G_Object* E_Object1::GetNode(int i)
{
return (pVertex);
}

BOOL E_Object1::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
if (pVertex==pN)
{
  brc=TRUE;
}
return (brc);
}

//needs replacing with mass property
Mat E_Object1::GetElNodalMass(PropTable* PropsT, MatTable* MatT)
{
    double M = 0;
    char S1[80];
	if (PID == -1)
	{
		M = dM;
	}
	else
	{
		Property* pS = PropsT->GetItem(PID);
		if (pS != NULL)
		{
			if (pS->iType == 161)
			{
				PMASS* pM = (PMASS*)pS;
				M = pM->dM;
			}
			else
			{
				sprintf_s(S1, "ERROR INVALID PROPERTY FOR EL %i", iLabel);
				outtext1(S1);
			}
		}
		else
		{
			sprintf_s(S1, "ERROR INVALID NO VALID PROPERTY FOR EL %i", iLabel);
			outtext1(S1);
		}
	}

  Mat FS(iNoNodes, 1);
  *FS.mn(1, 1) = M;
  return(FS);
}

Vec<int> E_Object1::GetSteerVec3d()
{
  Vec<int> V(1 * 3);
  *V.nn(1) = pVertex->dof[0];
  *V.nn(2) = pVertex->dof[1];
  *V.nn(3) = pVertex->dof[2];
  return(V);
}

Mat E_Object1::GetStiffMat(PropTable* PropsT, MatTable* MatT)
{
  Mat KM(3, 3);
  KM.MakeZero();
  return (KM);
}

void E_Object1::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

if (pVertex==pThis)
{
  pVertex=pWith;
}
}


void E_Object1::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
	  // TODO: add storing code here
      E_Object::Serialize(ar,iV,MESH);
      ar << pVertex->iLabel;
      ar << iCID;
      ar << dM;
      ar << dX1;
      ar << dX2;
      ar << dX3;
      ar << dI11;
      ar << dI21;
      ar << dI22;
      ar << dI31;
      ar << dI32;
      ar << dI33;
	}
	else
	{
      E_Object::Serialize(ar,iV,MESH);
      ar>>iNd;
      pVertex = MESH->GetNode(iNd);
      ar >> iCID;
      ar >> dM;
      ar >> dX1;
      ar >> dX2;
      ar >> dX3;
      ar >> dI11;
      ar >> dI21;
      ar >> dI22;
      ar >> dI31;
      ar >> dI32;
      ar >> dI33;
	}
}


void E_Object1::ExportUNV(FILE* pFile)
{
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
fprintf(pFile,"%10i",pVertex->iLabel);
fprintf(pFile,"\n","");
}

void E_Object1::ExportNAS(FILE* pFile)
{
fprintf(pFile,"%8s%8i%8i%8i%8s%8s%8s%8s\n","CONM2   ",iLabel,pVertex->iLabel,iCID,e8(dM),e8(dX1),e8(dX2),e8(dX3));
fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(dI11),e8(dI21),e8(dI22),e8(dI31),e8(dI32),e8(dI33));
}

void E_Object1::Transform(C3dMatrix TMat)
{
TMat.ClearTranslations();
C3dVector vOff;
vOff.Set(dX1,dX2,dX3);
vOff=TMat*vOff;
C3dMatrix mI;
mI.MakeUnit();

mI.m_00=dI11;
mI.m_10=dI21;
mI.m_20=dI31;

mI.m_01=dI21;
mI.m_11=dI22;
mI.m_21=dI32;

mI.m_02=dI31;
mI.m_12=dI32;
mI.m_22=dI33;
mI*=TMat;
dI11=mI.m_00;
dI21=mI.m_10;
dI31=mI.m_20;

dI21=mI.m_01;
dI22=mI.m_11;
dI32=mI.m_21;

dI31=mI.m_02;
dI32=mI.m_12;
dI33=mI.m_22;
}

G_Object* E_Object1::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object1* gret = new E_Object1;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex = MESH->GetNode(this->pVertex->iLabel);

gret->iCID=iCID;
gret->dM=dM;
gret->dX1=dX1;
gret->dX2=dX2;
gret->dX3=dX3;
gret->dI11=dI11;
gret->dI21=dI21;
gret->dI22=dI22;
gret->dI31=dI31;
gret->dI32=dI32;
gret->dI33=dI33;
gret->pPr=pPr;
gret->pParent=MESH;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object1::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object1* gret = new E_Object1;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex);
EInd+=iSInd;
gret->pVertex = Target->pNodes[EInd];
gret->iCID=iCID;
gret->dM=dM;
gret->dX1=dX1;
gret->dX2=dX2;
gret->dX3=dX3;
gret->dI11=dI11;
gret->dI21=dI21;
gret->dI22=dI22;
gret->dI31=dI31;
gret->dI32=dI32;
gret->dI33=dI33;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}



G_Object* E_Object1::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object1* gret = new E_Object1;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex = pInVertex[0];
gret->iCID=iCID;
gret->dM=dM;
gret->dX1=dX1;
gret->dX2=dX2;
gret->dX3=dX3;
gret->dI11=dI11;
gret->dI21=dI21;
gret->dI22=dI22;
gret->dI31=dI31;
gret->dI32=dI32;
gret->dI33=dI33;
gret->pPr=pPr;
gret->pParent=Parrent;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object1::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex->DSP_Point->x-5,(int) pVertex->DSP_Point->y-5);
pDC->LineTo((int) pVertex->DSP_Point->x+5,(int) pVertex->DSP_Point->y-5);
pDC->LineTo((int) pVertex->DSP_Point->x+5,(int) pVertex->DSP_Point->y+5);
pDC->LineTo((int) pVertex->DSP_Point->x-5,(int) pVertex->DSP_Point->y+5);
pDC->LineTo((int) pVertex->DSP_Point->x-5,(int) pVertex->DSP_Point->y-5);
}



void E_Object1::OglDraw(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector vCent;
vCent=this->Get_Centroid();
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
	Selectable=1;
	glColor3fv(cols[GetCol()]);
    glPointSize(16.0f); 
	glBegin(GL_POINTS);
    glVertex3f((float) vCent.x,(float) vCent.y,(float) vCent.z);
    glEnd();
}
else
{
   Selectable=0;
}

if (bDrawLab==TRUE)
{
	  sprintf_s(sLab,"N%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
}
}

void E_Object1::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
OglDraw(iDspFlgs,dS1,dS2);
}





C3dVector E_Object1::Get_Centroid()
{
C3dVector vT;
vT.x = pVertex->Pt_Point->x;
vT.y = pVertex->Pt_Point->y;
vT.z = pVertex->Pt_Point->z;
return (vT);
}


int E_Object1::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "CID";
	iNo++;
	sVar[iNo] = "Mass";
	iNo++;
	sVar[iNo] = "MX1";
	iNo++;
	sVar[iNo] = "MX2";
	iNo++;
	sVar[iNo] = "MX3";
	iNo++;
	sVar[iNo] = "MI11";
	iNo++;
	sVar[iNo] = "MI21";
	iNo++;
	sVar[iNo] = "MI22";
	iNo++;
	sVar[iNo] = "MI31";
	iNo++;
	sVar[iNo] = "MI32";
	iNo++;
	sVar[iNo] = "MI33";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	return(iNo);
}
//"CONM2   "
CString E_Object1::GetName()
{
	return ("Mass (CONM2)");
}

int E_Object1::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iCID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dM);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dX1);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dX2);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dX3);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI11);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI21);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI22);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI31);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI32);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI33);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object1::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*)this->pParent;
	PID = atoi(sVar[0]);
	iCID = atoi(sVar[1]);
	dM = atof(sVar[2]);;
	dX1 = atof(sVar[3]);;
	dX2 = atof(sVar[4]);;
	dX3 = atof(sVar[5]);;
	dI11 = atof(sVar[6]);;
	dI21 = atof(sVar[7]);;
	dI22 = atof(sVar[8]);;
	dI31 = atof(sVar[9]);;
	dI32 = atof(sVar[10]);;
	dI33 = atof(sVar[11]);;
	int N1 = atof(sVar[12]);;
	if (pVertex->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex = pN;
	}
}

IMPLEMENT_DYNAMIC( E_CellS , CObject )

E_CellS ::~E_CellS ()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
pVertex[4]=NULL;
}

void E_CellS ::Create(Pt_Object* pInVertex[100], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);

int i=0;
for (i=0;i<5;i++)
   {
    pVertex[i] = pInVertex[i];
   }

}

void E_CellS::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
    ar << pVertex[3]->iLabel;
    ar << pVertex[4]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[3] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[4] = MESH->GetNode(iNd);
	}
  pPr=NULL;
}

G_Object* E_CellS::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_CellS* gret = new E_CellS;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pVertex[3] = MESH->GetNode(pVertex[3]->iLabel);
gret->pVertex[4] = MESH->GetNode(pVertex[4]->iLabel);
gret->pParent=MESH;
gret->pPr=pPr;

return (gret);
}

G_Object* E_CellS::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_CellS* gret = new E_CellS;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[3]);
gret->pVertex[3] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[4]);
gret->pVertex[4] = Target->pNodes[EInd+iSInd];
gret->pParent=Target;
gret->pPr=NULL;
return (gret);
}


G_Object* E_CellS::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_CellS* gret = new E_CellS;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  =inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->pVertex[2] = pInVertex[2];
gret->pVertex[3] = pInVertex[3];
gret->pVertex[4] = pInVertex[4];
gret->pParent=Parrent;
gret->pPr=pPr;
return (gret);
}

// Draw Object line
void E_CellS::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->MoveTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[4]->DSP_Point->x,(int) pVertex[4]->DSP_Point->y);

}

BOOL E_CellS::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	  break;
  }
}
return (brc);
}

void E_CellS::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}

}

G_Object* E_CellS::GetNode(int i)
{
return (pVertex[i]);
}

C3dVector E_CellS::Get_Centroid()
{
  return (pVertex[0]->Get_Centroid());
}


void E_CellS::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  Selectable=1;
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1,0x0101);
  glColor3fv(cols[iColour]);
  glBegin(GL_LINES);
  glVertex3f((float) pVertex[1]->Pt_Point->x,(float) pVertex[1]->Pt_Point->y,(float) pVertex[1]->Pt_Point->z);
  glVertex3f((float) pVertex[3]->Pt_Point->x,(float) pVertex[3]->Pt_Point->y,(float) pVertex[3]->Pt_Point->z);
  glVertex3f((float) pVertex[2]->Pt_Point->x,(float) pVertex[2]->Pt_Point->y,(float) pVertex[2]->Pt_Point->z);
  glVertex3f((float) pVertex[0]->Pt_Point->x,(float) pVertex[0]->Pt_Point->y,(float) pVertex[0]->Pt_Point->z);
  glEnd();
  glDisable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  glVertex3f((float) pVertex[0]->Pt_Point->x,(float) pVertex[0]->Pt_Point->y,(float) pVertex[0]->Pt_Point->z);
  glVertex3f((float) pVertex[4]->Pt_Point->x,(float) pVertex[4]->Pt_Point->y,(float) pVertex[4]->Pt_Point->z);
  glEnd();
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
  {
    sprintf_s(sLab,"E%i",iLabel);
    OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
  }
}
else
{
  Selectable=0;
}
}

void E_CellS::OglDraw(int iDspFlgs,double dS1,double dS2)
{

OglDrawW(iDspFlgs,dS1,dS2);
}

//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_Object4, CObject )

E_Object4::~E_Object4()
{
pVertex[0]=NULL;
pVertex[1]=NULL;
pVertex[2]=NULL;
pVertex[3]=NULL;
}

int E_Object4::MaxBW()
{
int i;
int j;
int MaxDof;
int MinDof;
MaxDof=0;
MinDof=99999999;
for (i=0;i<iNoNodes;i++)
{
  for(j=0;j<6;j++)
  {
    if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j]>MaxDof))
	{
       MaxDof=pVertex[i]->dof[j];
    }
	if ((pVertex[i]->dof[j] > 0) && (pVertex[i]->dof[j] < MinDof))
	{
       MinDof=pVertex[i]->dof[j];
	}
  }
}
int iRC;
if (MaxDof-MinDof<0)
{
  iRC=0;
}
else
{
  iRC=MaxDof-MinDof;
}
return (iRC);
}

Mat E_Object4::Sample(int iNo)
{
Mat Pts(iNo,3);
if (iNo == 4)
{
  double r3 = 0.577350269189626;;
  *Pts.mn(1,1) = -r3;
  *Pts.mn(2,1) = r3;
  *Pts.mn(3,1) = r3;
  *Pts.mn(4,1) = -r3;

  *Pts.mn(1,2) = -r3;
  *Pts.mn(2,2) = -r3;
  *Pts.mn(3,2) = +r3;
  *Pts.mn(4,2) = +r3;

  *Pts.mn(1,3) = 1;
  *Pts.mn(2,3) = 1;
  *Pts.mn(3,3) = 1;
  *Pts.mn(4,3) = 1;
}
else if (iNo == 1)
{
  *Pts.mn(1,1) = 0;
  *Pts.mn(1,2) = 0;
  *Pts.mn(1,3) = 4;
}
return (Pts);
}

Mat E_Object4::ShapeDer(Mat Points, int i)
{
Mat der(2,4);
double xi,eta;
xi = *Points.mn(i, 1);
eta = *Points.mn(i, 2);

*der.mn(1,1) = 0.25*(-1+eta);
*der.mn(1,2) = 0.25*(1-eta);
*der.mn(1,3) = 0.25*(1+eta);
*der.mn(1,4) = 0.25*(-1-eta);

*der.mn(2,1) = 0.25*(-1+xi);
*der.mn(2,2) = 0.25*(-1-xi);
*der.mn(2,3) = 0.25*(1+xi);
*der.mn(2,4) = 0.25*(1-xi);

return(der);
}

Mat E_Object4::ShapeFun(Mat Points, int i)
{
Mat fun(1,4);
double xi,eta;
xi = *Points.mn(i, 1);
eta = *Points.mn(i, 2);

*fun.mn(1,1) = 0.25*(1-xi)*(1-eta);
*fun.mn(1,2) = 0.25*(1+xi)*(1-eta);
*fun.mn(1,3) = 0.25*(1+xi)*(1+eta);
*fun.mn(1,4) = 0.25*(1-xi)*(1+eta);
return(fun);
}

Mat E_Object4::getCoords2d()
{
int i;
Mat coord(iNoNodes,2);
for (i=0;i<iNoNodes;i++)
{
  *coord.mn(i+1, 1) = pVertex[i]->Pt_Point->x;
  *coord.mn(i+1, 2) = pVertex[i]->Pt_Point->y;
}
return (coord);
}

//*******************************************************************
//Get The coordinates after transforming the element to the XY plain
//********************************************************************
Mat E_Object4::getCoords3d()
{
int i;
C3dVector p;
Mat coord(iNoNodes,2);
C3dMatrix M3=this->GetElSys();
//M3.Transpose();
for (i=0;i<iNoNodes;i++)
{
  C3dVector p,v;
  p.x=pVertex[i]->Pt_Point->x;
  p.y=pVertex[i]->Pt_Point->y;
  p.z=pVertex[i]->Pt_Point->z;
  //M3.MakeUnit();
  v=M3.Mult(p);
  *coord.mn(i+1,1)=v.x;
  *coord.mn(i+1,2)=v.y;
}
return (coord);
}

Vec<int> E_Object4::GetSteerVec3d()
{
Vec<int> V(24);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[0]->dof[1];
*V.nn(3)=pVertex[0]->dof[2];
*V.nn(4)=pVertex[0]->dof[3];
*V.nn(5)=pVertex[0]->dof[4];
*V.nn(6)=pVertex[0]->dof[5];

*V.nn(7)=pVertex[1]->dof[0];
*V.nn(8)=pVertex[1]->dof[1];
*V.nn(9)=pVertex[1]->dof[2];
*V.nn(10)=pVertex[1]->dof[3];
*V.nn(11)=pVertex[1]->dof[4];
*V.nn(12)=pVertex[1]->dof[5];

*V.nn(13)=pVertex[2]->dof[0];
*V.nn(14)=pVertex[2]->dof[1];
*V.nn(15)=pVertex[2]->dof[2];
*V.nn(16)=pVertex[2]->dof[3];
*V.nn(17)=pVertex[2]->dof[4];
*V.nn(18)=pVertex[2]->dof[5];

*V.nn(19)=pVertex[3]->dof[0];
*V.nn(20)=pVertex[3]->dof[1];
*V.nn(21)=pVertex[3]->dof[2];
*V.nn(22)=pVertex[3]->dof[3];
*V.nn(23)=pVertex[3]->dof[4];
*V.nn(24)=pVertex[3]->dof[5];
return(V);
}

Vec<int> E_Object4::GetSteerVec3d_E()
{
	Vec<int> V(12);
	*V.nn(1) = pVertex[0]->dof[0];
	*V.nn(2) = pVertex[0]->dof[1];
	*V.nn(3) = pVertex[0]->dof[2];
	*V.nn(4) = pVertex[1]->dof[0];
	*V.nn(5) = pVertex[1]->dof[1];
	*V.nn(6) = pVertex[1]->dof[2];
	*V.nn(7) = pVertex[2]->dof[0];
	*V.nn(8) = pVertex[2]->dof[1];
	*V.nn(9) = pVertex[2]->dof[2];
	*V.nn(10) = pVertex[3]->dof[0];
	*V.nn(11) = pVertex[3]->dof[1];
	*V.nn(12) = pVertex[3]->dof[2];
	return(V);
}

Vec<int> E_Object4::GetSteerVec1d()
{
Vec<int> V(4*1);
*V.nn(1)=pVertex[0]->dof[0];
*V.nn(2)=pVertex[1]->dof[0];
*V.nn(3)=pVertex[2]->dof[0];
*V.nn(4)=pVertex[3]->dof[0];
return(V);
}

Mat E_Object4::bmatAxi(double& radius,
                       Mat& coord,
                       Mat& deriv,
                       Mat& fun)
{
int nod,i,k,l,m;
double x,y;
nod = deriv.n;
Mat bee(4,8);
radius = 0;
for (i=1;i<4+1;i++)
{
  radius = radius + *fun.mn(1,i) * *coord.mn(i, 1);
}
for (m=1;m<nod+1;m++)
{
  k = 2 * m;
  l = k - 1;
  x = *deriv.mn(1, m);
  *bee.mn(1, l) = x;
  *bee.mn(3, k) = x;
  y = *deriv.mn(2, m);
  *bee.mn(2, k) = y;
  *bee.mn(3, l) = y;
  *bee.mn(4, l) = *fun.mn(1,m) / radius;
}
return (bee);
}

//strain displacement matrix 1 pt integration for explicit
//Analysis

Mat E_Object4::GetB_1pt(double &det)
{

	Mat bee;
	Mat deriv;
	Mat deriv2;
	Mat fun;
	Mat Points;
	Mat jac;
	Mat coord;
	//This part calculates the 2d membraine stiffness
	int iDof = 2;   //2 dof X,Y per node
	int nip = 1;    //1 intergration points
	int iS = 3;
	Points = this->Sample(nip);
	det = 0;

	C3dMatrix M3 = this->GetElSys();
	coord = this->getCoords3d();
	fun = this->ShapeFun(Points, 1);
	deriv = this->ShapeDer(Points, 1);
	jac = deriv * coord;
	jac = jac.InvertJac(det);
	det *= *Points.mn(1, 3);   //*det by the weight
	deriv2 = jac * deriv;
	bee = this->bmat(coord, deriv2, iS, iDof);
	coord.clear(); 
	fun.clear(); 
	jac.clear();
	deriv.clear(); 
	deriv2.clear();
	return (bee);
}

//For explicit membraine shell analysis
Mat E_Object4::GetStiffMat_Ex(PropTable* PropsT, MatTable* MatT)
{
	Mat bee;   //strain displacement matrix
	int nip = 0;

	Mat coord2;
	Mat deriv;
	Mat deriv2;
	Mat fun;
	Mat Points;
	Mat jac;
	int i;
	double det;
	Mat bT;
	Mat db;
	Mat bdb;
	int iDof;
	int iS;
	int MID = -1;
	double dE = 210e9;
	double dv = 0.29;
	char S1[80];
	double dthk = 0.001;

	Property* pS = PropsT->GetItem(PID);
	if (pS != NULL)
	{
		MID = pS->GetDefMatID();
	}
	Material* pM = MatT->GetItem(MID);
	if (pS == NULL)
	{
		sprintf_s(S1, "ERROR: PROPERTY NOT FOUND FOR EL %i", iLabel);
		outtext1(S1);
	}
	if (pM == NULL)
	{
		sprintf_s(S1, "ERROR: MATERIAL NOT FOUND FOR EL %i", iLabel);
		outtext1(S1);
	}
	//Get Shell thickness

	if (((iType == 91) || (iType == 94)) && (pS != NULL))
	{
		PSHELL* pSh = (PSHELL*)pS;
		dthk = pSh->dT;
	}

	if ((pM != NULL) && (pM->iType = 1))
	{
		MAT1* pIsen = (MAT1*)pM;
		dE = pIsen->dE;
		dv = pIsen->dNU;
	}
	//This part calculates the 2d membraine stiffness
	iDof = 2;   //2 dof X,Y per node
	nip = 4;    //4 intergration points
	iS = 3;

	Mat KM(iDof*iNoNodes, iDof*iNoNodes);
	Mat dee = DeeMat(dE, dv, iS);         //plain stress material matrix
	Mat coord = getCoords3d();          //Nodal Coordinates
	C3dMatrix M3 = this->GetElSys();    //Element Coordinate system
	//M3.MakeUnit();
	Points = Sample(nip);               //sample points for integration
	//Membraine stiffness calculation
	//for dof 12
	for (i = 1; i < nip + 1; i++)            // for all integration points
	{
		det = 0;
		fun.clear(); deriv.clear(); jac.clear(), deriv2.clear();
		bT.clear(); db.clear(); bdb.clear(); bee.clear();
		fun = ShapeFun(Points, i);
		deriv = ShapeDer(Points, i);
		jac = deriv * coord;
		jac = jac.InvertJac(det);
		deriv2 = jac * deriv;
		bee = bmat(coord, deriv2, iS, iDof);
		bT = bee;
		bT.Transpose();
		db = dee * bee;
		bdb = bT * db;
		det *= *Points.mn(i, 3);
		bdb *= det;
		KM += bdb;
	}
	KM *= dthk;
	fun.clear(); deriv.clear(); jac.clear(), deriv2.clear();
	bT.clear(); db.clear(); bdb.clear(); bee.clear();
	return (KM);
}

Mat E_Object4::GetStiffMat(PropTable* PropsT, MatTable* MatT)
{
	Mat bee;   //strain displacement matrix
	int nip = 0;

	Mat coord2;
	Mat deriv;
	Mat deriv2;
	Mat fun;
	Mat Points;
	Mat jac;
	int i;
	double det;
	Mat bT;
	Mat db;
	Mat bdb;
	int iDof;
	int iS;
	int MID = -1;
	double dE = 210e9;
	double dv = 0.29;
	char S1[80];
	double dthk = 0.001;

	Property* pS = PropsT->GetItem(PID);
	if (pS != NULL)
	{
		MID = pS->GetDefMatID();
	}
	Material* pM = MatT->GetItem(MID);
	if (pS == NULL)
	{
		sprintf_s(S1, "ERROR: PROPERTY NOT FOUND FOR EL %i", iLabel);
		outtext1(S1);
	}
	if (pM == NULL)
	{
		sprintf_s(S1, "ERROR: MATERIAL NOT FOUND FOR EL %i", iLabel);
		outtext1(S1);
	}
	//Get Shell thickness

	if (((iType == 91) || (iType == 94)) && (pS != NULL))
	{
		PSHELL* pSh = (PSHELL*)pS;
		dthk = pSh->dT;
	}

	if ((pM != NULL) && (pM->iType = 1))
	{
		MAT1* pIsen = (MAT1*)pM;
		dE = pIsen->dE;
		dv = pIsen->dNU;
	}
	//This part calculates the 2d membraine stiffness
	iDof = 2;   //2 dof X,Y per node
	nip = 4;    //4 intergration points
	iS = 3;

	Mat KM(iDof*iNoNodes, iDof*iNoNodes);
	Mat dee = DeeMat(dE, dv, iS);         //plain stress material matrix
	Mat coord = getCoords3d();          //Nodal Coordinates
	C3dMatrix M3 = this->GetElSys();    //Element Coordinate system
	//M3.MakeUnit();
	Points = Sample(nip);               //sample points for integration
	//Membraine stiffness calculation
	//for dof 12
	for (i = 1; i < nip + 1; i++)            // for all integration points
	{
		det = 0;
		fun = ShapeFun(Points, i);
		deriv = ShapeDer(Points, i);
		jac = deriv * coord;
		jac = jac.InvertJac(det);
		deriv2 = jac * deriv;
		bee = bmat(coord, deriv2, iS, iDof);
		bT = bee;
		bT.Transpose();
		db = dee * bee;
		bdb = bT * db;
		det *= *Points.mn(i, 3);
		bdb *= det;
		KM += bdb;
		fun.clear(); deriv.clear(); jac.clear(), deriv2.clear();
		bT.clear(); db.clear(); bdb.clear(); bee.clear(); jac.clear();
	}
	KM *= dthk;
	Points.clear();
	dee.clear();
	//Mindlin Formulation
	//Sheer Stifness Calculation
	//reduced integration 1 sample point
	double G, kk;
	int k;
	dee.Create(2, 2);
	G = 0.5*dE / (1 + dv);
	kk = 5.0 / 6.0;
	*dee.mn(1, 1) = kk * G*dthk;
	*dee.mn(2, 2) = kk * G*dthk;
	*dee.mn(1, 2) = 0;
	*dee.mn(2, 1) = 0;
	Mat KM2(12, 12);
	nip = 1;                        //1 reduced integration point
	Points = Sample(nip);
	for (i = 1; i < nip + 1; i++)
	{
		det = 0;
		fun = ShapeFun(Points, i);
		deriv = ShapeDer(Points, i);
		jac = deriv * coord;
		jac = jac.InvertJac(det);
		deriv2 = jac * deriv;
		bee.clear();
		bee.Create(2, 12);
		for (k = 1; k < 4 + 1; k++)//was m
		{
			*bee.mn(1, (k) * 3 - 2) = *deriv2.mn(1, k);
			*bee.mn(1, (k) * 3 - 1) = 0;
			*bee.mn(1, (k) * 3 - 0) = *fun.mn(1, k);
			*bee.mn(2, (k) * 3 - 2) = *deriv2.mn(2, k);
			*bee.mn(2, (k) * 3 - 1) = -*fun.mn(1, k);
			*bee.mn(2, (k) * 3 - 0) = 0;
		}
		bT = bee;
		bT.Transpose();
		db = dee * bee;
		bdb = bT * db;
		det *= *Points.mn(i, 3);
		bdb *= det;
		KM2 += bdb;
		fun.clear(); deriv.clear(); jac.clear(), deriv2.clear();
		bT.clear(); db.clear(); bdb.clear(); bee.clear();
	}

	//The bending part
	Points.clear();
	dee.clear();
	dee.Create(3, 3);
	double Cf;
	Cf = dE * dthk*dthk*dthk / (12 * (1 - dv * dv));
	nip = 4;
	Points = Sample(nip);
	*dee.mn(1, 1) = 1;
	*dee.mn(1, 2) = dv;
	*dee.mn(1, 3) = 0;
	*dee.mn(2, 1) = dv;
	*dee.mn(2, 2) = 1;
	*dee.mn(2, 3) = 0;
	*dee.mn(3, 1) = 0;
	*dee.mn(3, 2) = 0;
	*dee.mn(3, 3) = (1 - dv);
	dee *= Cf;
	Mat KM3(12, 12);
	for (i = 1; i < nip + 1; i++)
	{

		det = 0;
		fun = ShapeFun(Points, i);
		deriv = ShapeDer(Points, i);
		jac = deriv * coord;
		jac = jac.InvertJac(det);
		deriv2 = jac * deriv;
		bee.clear();
		bee.Create(3, 12);
		for (k = 1; k < 4 + 1; k++)//was m
		{
			*bee.mn(1, (k) * 3 - 2) = 0;
			*bee.mn(1, (k) * 3 - 1) = 0;
			*bee.mn(1, (k) * 3 - 0) = -*deriv2.mn(1, k);
			*bee.mn(2, (k) * 3 - 2) = 0;
			*bee.mn(2, (k) * 3 - 1) = *deriv2.mn(2, k);
			*bee.mn(2, (k) * 3 - 0) = 0;
			*bee.mn(3, (k) * 3 - 2) = 0;
			*bee.mn(3, (k) * 3 - 1) = *deriv2.mn(1, k);
			*bee.mn(3, (k) * 3 - 0) = -*deriv2.mn(2, k);
		}
		bT = bee;
		bT.Transpose();
		db = dee * bee;
		bdb = bT * db;
		det *= *Points.mn(i, 3);
		bdb *= det;
		KM3 += bdb;
		fun.clear(); deriv.clear(); jac.clear(), deriv2.clear();
		bT.clear(); db.clear(); bdb.clear(); bee.clear();
	}
	KM2 += KM3;
	//Assemble all the components inplain,shear and bending in KMf
	Mat KMf(6 * iNoNodes, 6 * iNoNodes);
	Vec<int> V(8);
	*V.nn(1) = 1;
	*V.nn(2) = 2;
	*V.nn(3) = 7;
	*V.nn(4) = 8;
	*V.nn(5) = 13;
	*V.nn(6) = 14;
	*V.nn(7) = 19;
	*V.nn(8) = 20;
	int j;
	for (i = 1; i <= 8; i++)
	{
		for (j = 1; j <= 8; j++)
		{
			*KMf.mn(V[i - 1], V[j - 1]) = *KM.mn(i, j);
		}
	}
	Vec<int> V1(12);
	*V1.nn(1) = 3;
	*V1.nn(2) = 4;
	*V1.nn(3) = 5;
	*V1.nn(4) = 9;
	*V1.nn(5) = 10;
	*V1.nn(6) = 11;
	*V1.nn(7) = 15;
	*V1.nn(8) = 16;
	*V1.nn(9) = 17;
	*V1.nn(10) = 21;
	*V1.nn(11) = 22;
	*V1.nn(12) = 23;

	for (i = 1; i <= 12; i++)
	{
		for (j = 1; j <= 12; j++)
		{
			*KMf.mn(V1[i - 1], V1[j - 1]) = *KM2.mn(i, j);
		}
	}

	for (i = 6; i <= 24; i += 6)
	{   //BECAREFUL MAKING THIS VALUE LARGE MESSES THINGS UP
		*KMf.mn(i, i) = 100;       //DRILLING STIFFNESS
	}
	//Transform to global
	//M3.Transpose();
	Mat TMAT(24, 24);

	for (i = 1; i < 24; i += 3)
	{
		*TMAT.mn(i, i) = M3.m_00;
		*TMAT.mn(i + 1, i) = M3.m_10;
		*TMAT.mn(i + 2, i) = M3.m_20;

		*TMAT.mn(i, i + 1) = M3.m_01;
		*TMAT.mn(i + 1, i + 1) = M3.m_11;
		*TMAT.mn(i + 2, i + 1) = M3.m_21;

		*TMAT.mn(i, i + 2) = M3.m_02;
		*TMAT.mn(i + 1, i + 2) = M3.m_12;
		*TMAT.mn(i + 2, i + 2) = M3.m_22;
	}
	//TMAT.diag();
	Mat TMATT = TMAT;
	TMATT.Transpose();
	Mat T;
	Mat TT;
	T = KMf * TMAT;
	TT = TMATT * T;
	KM.clear();
	KM2.clear();
	KM3.clear();
	KMf.clear();
	TMATT.clear();
	TMAT.clear();
	T.clear();
	V.clear();
	V1.clear();

	return (TT);
}

int E_Object4::noDof()
{
return(6);
}


void E_Object4::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,int inMCys,double inMAng,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
iMCys= inMCys;
MAng= inMAng;
int i=0;
for (i=0;i<4;i++)
   {
    pVertex[i] = pInVertex[i];
   }
dZOFFS=0;
}

void E_Object4::Reverse()
{

Pt_Object* pT[8];
pT[0]=pVertex[0];
pT[1]=pVertex[1];
pT[2]=pVertex[2];
pT[3]=pVertex[3];
pVertex[0] = pT[0];
pVertex[1] = pT[3];
pVertex[2] = pT[2];
pVertex[3] = pT[1];
}

void E_Object4::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

BOOL E_Object4::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_Object4::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    E_Object::Serialize(ar,iV,MESH);
    ar<<iMCys;
    ar<<MAng;
    ar<<dZOFFS;
    ar << pVertex[0]->iLabel;
    ar << pVertex[1]->iLabel;
    ar << pVertex[2]->iLabel;
    ar << pVertex[3]->iLabel;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    ar>>iMCys;
    ar>>MAng;
    ar>>dZOFFS;
    ar>>iNd;
    pVertex[0] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[1] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[2] = MESH->GetNode(iNd);
    ar>>iNd;
    pVertex[3] = MESH->GetNode(iNd);
	}
  pPr=NULL;
}


void E_Object4::ExportUNV(FILE* pFile)
{
int i;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
for (i=0;i<iNoNodes;i++)
{
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
}
fprintf(pFile,"\n","");
}

void E_Object4::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s",ToString());
}


G_Object* E_Object4::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object4* gret = new E_Object4;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = MESH->GetNode(pVertex[0]->iLabel);
gret->pVertex[1] = MESH->GetNode(pVertex[1]->iLabel);
gret->pVertex[2] = MESH->GetNode(pVertex[2]->iLabel);
gret->pVertex[3] = MESH->GetNode(pVertex[3]->iLabel);
gret->pParent=MESH;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

G_Object* E_Object4::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
E_Object4* gret = new E_Object4;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
EInd=Source->GetNodeInd(pVertex[0]);
gret->pVertex[0] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[1]);
gret->pVertex[1] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[2]);
gret->pVertex[2] = Target->pNodes[EInd+iSInd];
EInd=Source->GetNodeInd(pVertex[3]);
gret->pVertex[3] = Target->pNodes[EInd+iSInd];
gret->pParent=Target;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=NULL;
gret->pResV = NULL;
return (gret);
}


G_Object* E_Object4::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
ME_Object* MESH =(ME_Object*) Parrent;
E_Object4* gret = new E_Object4;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  =inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
gret->pVertex[0] = pInVertex[0];
gret->pVertex[1] = pInVertex[1];
gret->pVertex[2] = pInVertex[2];
gret->pVertex[3] = pInVertex[3];
gret->pParent=Parrent;
gret->iMCys=iMCys;
gret->MAng=MAng;
gret->dZOFFS=dZOFFS;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

// Draw Object line
void E_Object4::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
pDC->LineTo((int) pVertex[2]->DSP_Point->x,(int) pVertex[2]->DSP_Point->y);
pDC->LineTo((int) pVertex[3]->DSP_Point->x,(int) pVertex[3]->DSP_Point->y);
pDC->LineTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);

}

G_Object* E_Object4::GetNode(int i)
{
return (pVertex[i]);
}

int E_Object4::GetLinkList(cLink* Links[200])
{
int ic;
ic=GetCol();
Links[0]=new cLink;
Links[0]->pParent=this;
Links[0]->pVertex[0]=pVertex[0];
Links[0]->pVertex[1]=pVertex[1];
Links[0]->iColour=iColour;
Links[1]=new cLink;
Links[1]->pParent=this;
Links[1]->pVertex[0]=pVertex[1];
Links[1]->pVertex[1]=pVertex[2];
Links[1]->iColour=iColour;
Links[2]=new cLink;
Links[2]->pParent=this;
Links[2]->pVertex[0]=pVertex[2];
Links[2]->pVertex[1]=pVertex[3];
Links[2]->iColour=iColour;
Links[3]=new cLink;
Links[3]->pParent=this;
Links[3]->pVertex[0]=pVertex[3];
Links[3]->pVertex[1]=pVertex[0];
Links[3]->iColour=iColour;
return (4);
}

void E_Object4::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
double dFS;
char sLab[20];
C3dVector d[4];
int i;
int ind;

for (i=0;i<4;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
dFS = ME->dResFactor;
ind=ME->iCVar;
glLineWidth(1.0);
if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<4;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S* dFS;
       }
    }
  }
  Selectable=1;
  if ((iDspFlgs & DSP_ELEMENTS)>0)
  {
    if ((iDspFlgs & DSP_LINE) > 0)
    {
      glColor3fv(cols[iColour]);
    }
    else
    {
      glColor3fv(cols[0]);
    }
    C3dVector vN;
    C3dVector vOff;
    vN=Get_Normal();
    //Need to calculate the offsets here
	double dt=0;
	double dPCompOff=0;
    if (pPr!=NULL)
    {
      dt=pPr->GetThk();
      dt*=0.5;
	  if (pPr->iType==2)
	  {
        PCOMP* pPCOMP= (PCOMP*) pPr;
		dPCompOff=pPCOMP->dZ0+dt;
	  }
    }
    if (((iDspFlgs & DSP_OFF)>0) && (dZOFFS!=DBL_MAX))
    {
       vOff=vN;
       vOff*=dZOFFS+dPCompOff;
    }
    else
    {
       vOff*=0; 
    }

    vN*=dt;
    if ((iDspFlgs & DSP_THK)>0)
    {
  	  glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));

        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));

        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
        glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
        glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
        glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	  glEnd();
    }
    else
    {
      vN*=0;
      glBegin(GL_LINES);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glEnd();
    }
	C3dVector vCent;
    vCent=Get_Centroid();
    if (bDrawLab==TRUE)
	  {
	      sprintf_s(sLab,"E%i",iLabel);
	      OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	  }
    if (((iDspFlgs & DSP_RESLAB)==0) && (pResV!=NULL))
    {
       sprintf_s(sLab,"%f",*pResV->GetAddress(ind));
       OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
    }
    
    //Results Label

	  
  }
  if ((iDspFlgs & DSP_ELSYS)==0)
  {
	glColor3fv(cols[iColour]);
    C3dMatrix mS = GetElSys();
    C3dVector vC = Get_Centroid();
    mS.Transpose();
    C3dVector vX=mS.GetColVec(1);
    C3dVector vY=mS.GetColVec(2);;
    C3dVector vZ=mS.GetColVec(3);;

    vX*=0.5*dS1; vY*=0.5*dS1; vZ*=0.5*dS1;
    vX+=vC;vY+=vC;vZ+=vC;

    glBegin(GL_LINES);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vX.x,(float) vX.y,(float) vX.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vY.x,(float) vY.y,(float) vY.z);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glEnd();
    glRasterPos3f ((float) vX.x,(float) vX.y,(float) vX.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
    glRasterPos3f ((float) vY.x,(float) vY.y,(float) vY.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
    glRasterPos3f ((float) vZ.x,(float) vZ.y,(float) vZ.z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }

  if ((iDspFlgs & DSP_MATL)==0)
  {
    C3dMatrix mS = GetElSys();
	C3dMatrix mR;

    C3dVector vD = GetFirstEdge();
	C3dVector vC = Get_Centroid();
	vD.Normalize();
	mR.Rotate(0,0,MAng);
	vD=mS*vD;
	vD=mR*vD;
	mS.Transpose();
	vD=mS*vD;
	vD*=0.5*dS1;
    vD+=vC;
    glBegin(GL_LINES);
      glVertex3f((float) vC.x,(float) vC.y,(float) vC.z);
      glVertex3f((float) vD.x,(float) vD.y,(float) vD.z);
    glEnd();
  }
}
else
{
  Selectable=0;
}
}

void E_Object4::OglDraw(int iDspFlgs,double dS1,double dS2)
{
C3dVector d[4];
int i;
for (i=0;i<4;i++)
{d[i].x=0;d[i].y=0;d[i].z=0;}
ME_Object* ME=(ME_Object*) this->pParent;
double S;
S=ME->dScale;
double dFS = 1.0;
dFS = ME->dResFactor;
float fCols[4]={0,0,0,0};
BOOL bD=FALSE;
int iVar;
iVar=ME->iCVar;
//Nodal data
glLineWidth(2.0);
if ((pVertex[0]->pResV != NULL) &&
    (pVertex[1]->pResV != NULL) &&
    (pVertex[2]->pResV != NULL) &&
    (pVertex[3]->pResV != NULL))
{
  bD=TRUE;
  fCols[0]=GetContourCol(*pVertex[0]->pResV->GetAddress(iVar)*dFS);
  fCols[1]=GetContourCol(*pVertex[1]->pResV->GetAddress(iVar)*dFS);
  fCols[2]=GetContourCol(*pVertex[2]->pResV->GetAddress(iVar)*dFS);
  fCols[3]=GetContourCol(*pVertex[3]->pResV->GetAddress(iVar)*dFS);
}
if (pResV!=NULL)
{
  bD=TRUE;
  fCols[0]=GetContourCol(*pResV->GetAddress(iVar)*dFS);
  fCols[1]=fCols[0];
  fCols[2]=fCols[0];
  fCols[3]=fCols[0];
}
C3dVector v1;
C3dVector v2;
C3dVector Vn;
C3dVector vN;
C3dVector vOff;
vN=Get_Normal();
double dt=0;
double dPCompOff=0;
if (pPr!=NULL)
{
  dt=pPr->GetThk();
  dt*=0.5;
  if (pPr->iType==2)
  {
    PCOMP* pPCOMP= (PCOMP*) pPr;
    dPCompOff=pPCOMP->dZ0+dt;
  }
}

if (((iDspFlgs & DSP_OFF)>0) && (dZOFFS!=DBL_MAX))
{
  vOff=vN;
  vOff*=dZOFFS+dPCompOff;
}
else
{
  vOff*=0; 
}


vN*=dt;
Vn =Get_Normal();


if ((iDspFlgs & DSP_ELEMENTS)>0)
{
  if ((iDspFlgs & DSP_RESDEF)==0)
  {
    for(i=0;i<4;i++)
    {
       if (pVertex[i]->pResD!=NULL)
       {
         d[i]=pVertex[i]->pResD->GetVec();
		 d[i] -= ME->vRelDispOff;
         d[i]*=S * dFS;
       }
    }
  }
	Selectable=1;
	glColor3fv(cols[iColour]);
  if (((iDspFlgs & DSP_CONT)>0) || (bD==FALSE))
  {
    if ((iDspFlgs & DSP_THK)>0)
    {
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glEnd();
      glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
	    glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
        glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
	    glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x-vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y-vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z-vN.z+d[1].z));
        glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
	    glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x-vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y-vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z-vN.z+d[2].z));
        glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
	    glEnd();

      glBegin(GL_POLYGON);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x-vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y-vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z-vN.z+d[3].z));
        glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x-vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y-vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z-vN.z+d[0].z));
	    glEnd();
    }
    else
    {
      vN*=0;
	    glBegin(GL_POLYGON);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glEnd();
    }
  }
  else
  {
      vN*=0;

      if (bD)
      {
      glColor3fv(cols[124]);
      glEnable(GL_TEXTURE_1D);
	    glBegin(GL_POLYGON);
        glTexCoord1f(fCols[0]);
	    glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[0]->Pt_Point->x+vOff.x+vN.x+d[0].x),(float) (pVertex[0]->Pt_Point->y+vOff.y+vN.y+d[0].y),(float) (pVertex[0]->Pt_Point->z+vOff.z+vN.z+d[0].z));
        glTexCoord1f(fCols[1]);
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[1]->Pt_Point->x+vOff.x+vN.x+d[1].x),(float) (pVertex[1]->Pt_Point->y+vOff.y+vN.y+d[1].y),(float) (pVertex[1]->Pt_Point->z+vOff.z+vN.z+d[1].z));
        glTexCoord1f(fCols[2]);
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[2]->Pt_Point->x+vOff.x+vN.x+d[2].x),(float) (pVertex[2]->Pt_Point->y+vOff.y+vN.y+d[2].y),(float) (pVertex[2]->Pt_Point->z+vOff.z+vN.z+d[2].z));
        glTexCoord1f(fCols[3]);
        glNormal3f((float) Vn.x,(float) Vn.y,(float) Vn.z);
	    glVertex3f((float) (pVertex[3]->Pt_Point->x+vOff.x+vN.x+d[3].x),(float) (pVertex[3]->Pt_Point->y+vOff.y+vN.y+d[3].y),(float) (pVertex[3]->Pt_Point->z+vOff.z+vN.z+d[3].z));
	    glEnd();
      glDisable(GL_TEXTURE_1D);
      }
  }
}
else
{
  Selectable=0;
}
}

int E_Object4::GetfaceList(cFace* Faces[6])
{
int ic;
ic=GetCol();
Faces[0]=new cFace();
Faces[0]->pParent=this;
Faces[0]->pVertex[0]=pVertex[0];
Faces[0]->pVertex[1]=pVertex[1];
Faces[0]->pVertex[2]=pVertex[2];
Faces[0]->pVertex[3]=pVertex[3];
Faces[0]->NoVert=4;
Faces[0]->iColour=ic;
return(1);
}



double E_Object4::GetCentriodVal(int iDof, Vec<int> &Steer, Vec<double> &Disp)
{
  double dTemp = 0;
  double T = 0;
  int iDOFID;
  int j;
  Mat fun;
  Mat FunPnt(1, 2);
  *FunPnt.mn(1, 1) = 0.0;
  *FunPnt.mn(1, 2) = 0.0;
  fun = this->ShapeFun(FunPnt, 1);
  for (j = 0; j<this->iNoNodes; j++)
  {
    T = 0;;
    iDOFID = this->pVertex[j]->dof[iDof];
    if (iDOFID>0)
    {
      T = *Disp.nn(iDOFID);
    }
    dTemp += T**fun.mn(1, j + 1);
  }
  fun.clear();
  FunPnt.clear();
  return(dTemp);
}

C3dVector E_Object4::Get_Centroid()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0;
*FunPnt.mn(1,2)=0;
fun=ShapeFun(FunPnt,1);
C3dVector vT;
vT.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vT+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}

fun.clear();
FunPnt.clear();
return (vT);
}

void E_Object4::Info()
{
  char S1[200]="";
  sprintf_s(S1,"LAB: %i COL: %i PID: %i ELTP: %i MCID: %i ANG: %f OFF: %f ",iLabel,iColour,PID,iType,iMCys,MAng,dZOFFS);
  outtext1(S1); 
  sprintf_s(S1,"NODES %i %i %i %i",pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel,pVertex[3]->iLabel);
  outtext1(S1); 

}

CString E_Object4::ToString()
{
CString sRT;
char S1[80];
CString sDir;
if (iLabel==28839)
 iLabel= iLabel;

if (iMCys==-1)
{
  sDir=e8(MAng);
}
else
{
  sprintf_s(S1,"%8i",iMCys);
  sDir=S1;
}
sprintf_s(S1,"%8s%8i%8i%8i%8i%8i%8i%8s%8s\n","CQUAD4  ",iLabel,PID,pVertex[0]->iLabel,pVertex[1]->iLabel,pVertex[2]->iLabel,pVertex[3]->iLabel,sDir,e8(dZOFFS));
sRT=S1;
return(sRT);
}

C3dVector E_Object4::Get_Normal()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0;
*FunPnt.mn(1,2)=0;
fun=ShapeDer(FunPnt,1);
C3dVector vX,vY,vT;
vX.Set(0,0,0);
vY.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
   vY+=pVertex[j]->Get_Centroid()**fun.mn(2,j+1);
}
vT=vX.Cross(vY);
vT.Normalize();
fun.clear();
FunPnt.clear();
return (vT);
}

C3dMatrix E_Object4::GetElSys()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=0;
*FunPnt.mn(1,2)=0;
fun=ShapeDer(FunPnt,1);
C3dVector vX,vY,vT;
vX.Set(0,0,0);
vY.Set(0,0,0);
int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
   vY+=pVertex[j]->Get_Centroid()**fun.mn(2,j+1);
}
vX.Normalize();
vY.Normalize();
vT=vX.Cross(vY);
vT.Normalize();
vY=vT.Cross(vX);
fun.clear();
FunPnt.clear();
C3dMatrix vR;
vR.SetColVec(1,vX);
vR.SetColVec(2,vY);
vR.SetColVec(3,vT);
vR.Transpose();
return (vR);
}

//26/06/2019
//Explicte version of get coordinate system
//adds in the displacements to git the displaced
//transformation of the element
C3dMatrix E_Object4::GetElSys_Ex(Vec<int> &Steer, Vec<double> &Disp)
{
	Mat fun;
	Mat FunPnt(1, 2);
	*FunPnt.mn(1, 1) = 0;
	*FunPnt.mn(1, 2) = 0;
	fun = ShapeDer(FunPnt, 1);
	C3dVector vX, vY, vT;
	C3dVector DicpCoord;
	vX.Set(0, 0, 0);
	vY.Set(0, 0, 0);
	DicpCoord.Set(0, 0, 0);
	int j = 0;
	for (j = 0; j < iNoNodes; j++)
	{
		DicpCoord.Set(*Disp.nn(*Steer.nn(j*3+1)), *Disp.nn(*Steer.nn(j * 3 + 2)), *Disp.nn(*Steer.nn(j * 3 + 3)));
		DicpCoord += pVertex[j]->Get_Centroid();
		vX += DicpCoord **fun.mn(1, j + 1);
		vY += DicpCoord **fun.mn(2, j + 1);
	}
	vX.Normalize();
	vY.Normalize();
	vT = vX.Cross(vY);
	vT.Normalize();
	vY = vT.Cross(vX);
	fun.clear();
	FunPnt.clear();
	C3dMatrix vR;
	vR.SetColVec(1, vX);
	vR.SetColVec(2, vY);
	vR.SetColVec(3, vT);
	vR.Transpose();
	return (vR);
}


C3dVector E_Object4::GetTestPt()
{
Mat fun;
Mat FunPnt(1,2);
*FunPnt.mn(1,1)=1;
*FunPnt.mn(1,2)=0;
fun=ShapeFun(FunPnt,1);
C3dVector vX;
vX.Set(0,0,0);

int j=0;
for (j=0;j<iNoNodes;j++)
{
   vX+=pVertex[j]->Get_Centroid()**fun.mn(1,j+1);
}
return (vX);
}

C3dVector E_Object4::GetFirstEdge()
{

C3dVector vD;
vD.x = pVertex[1]->Pt_Point->x-pVertex[0]->Pt_Point->x;
vD.y = pVertex[1]->Pt_Point->y-pVertex[0]->Pt_Point->y;
vD.z = pVertex[1]->Pt_Point->z-pVertex[0]->Pt_Point->z;

return (vD);
}


double E_Object4::QualAspect()
{
  double drc = 1;
  C3dVector v1;
  C3dVector v2;
  double de1;
  double de2;
  double de3;
  double de4;
  double deMax;
  double deMin;
  v1 = pVertex[0]->Get_Centroid();
  v2 = pVertex[1]->Get_Centroid();
  de1 = (v1 - v2).Mag();
  v1 = pVertex[1]->Get_Centroid();
  v2 = pVertex[2]->Get_Centroid();
  de2 = (v1 - v2).Mag();
  v1 = pVertex[2]->Get_Centroid();
  v2 = pVertex[3]->Get_Centroid();
  de3 = (v1 - v2).Mag();
  v1 = pVertex[3]->Get_Centroid();
  v2 = pVertex[0]->Get_Centroid();
  de4 = (v1 - v2).Mag();
  //Find Maximum value
  deMax = de1;
  if (de2 > deMax)
    deMax = de2;
  if (de3 > deMax)
    deMax = de3;
  if (de4 > deMax)
    deMax = de4;
  //Minimum
  deMin = de1;
  if (de2 < deMin)
    deMin = de2;
  if (de3 < deMin)
    deMin = de3;
  if (de4 < deMin)
    deMin = de4;
  if (deMin != 0)
    drc = deMax / deMin;
  else
    drc = 100000;
  return(drc);
}


CString E_Object4::GetName()
{
	return ("Quad Shell (CQUAD4)");
}

int E_Object4::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "PID";
	iNo++;
	sVar[iNo] = "Mat CYS";
	iNo++;
	sVar[iNo] = "Mat Angle";
	iNo++;
	sVar[iNo] = "Z Offset";
	iNo++;
	sVar[iNo] = "N1";
	iNo++;
	sVar[iNo] = "N2";
	iNo++;
	sVar[iNo] = "N3";
	iNo++;
	sVar[iNo] = "N4";
	iNo++;
	return(iNo);
}


int E_Object4::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", PID);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iMCys);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", MAng);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dZOFFS);
	sVar[iNo] = S1;
	iNo++;

	sprintf_s(S1, "%i", pVertex[0]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[1]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[2]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", pVertex[3]->iLabel);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void E_Object4::PutVarValues(PropTable* PT,int iNo, CString sVar[])
{

	Pt_Object* pN;
	ME_Object* pMe = (ME_Object*) this->pParent;
	
	int newPID = atoi(sVar[0]);
	if (newPID != PID)
	{

		Property* pr = PT->GetItem(newPID);
		if (pr != NULL)
		{
			BOOL bC = SetProperty(pr);
			if (bC)
				outtext1("Property has Been Changed");
			else
				outtext1("Invalid Property");
		}

	}
	iMCys = atoi(sVar[1]);
	MAng = atof(sVar[2]);;
	dZOFFS = atof(sVar[3]);;;
	int N1 = atoi(sVar[4]);
	int N2 = atoi(sVar[5]);
	int N3 = atoi(sVar[6]);
	int N4 = atoi(sVar[7]);
	if (pVertex[0]->iLabel != N1)
	{
		pN = pMe->GetNode(N1);
		if (pN != NULL)
			pVertex[0] = pN;

	}
	if (pVertex[1]->iLabel != N2)
	{
		pN = pMe->GetNode(N2);
		if (pN != NULL)
			pVertex[1] = pN;

	}
	if (pVertex[2]->iLabel != N3)
	{
		pN = pMe->GetNode(N3);
		if (pN != NULL)
			pVertex[2] = pN;

	}
	if (pVertex[3]->iLabel != N4)
	{
		pN = pMe->GetNode(N4);
		if (pN != NULL)
			pVertex[3] = pN;

	}
}

//*************************************************************************************
// Pre: TRUE
// Post: returns area in Y & Y plain only for 2d analysis
//*************************************************************************************
double E_Object4::GetArea2d()
{
	C2dVector v, w;
	double dA;
	v.x = pVertex[1]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	v.y = pVertex[1]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	w.x = pVertex[2]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	w.y = pVertex[2]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	dA = 0.5*abs(v.Cross(w));
	v.x = pVertex[2]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	v.y = pVertex[2]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	w.x = pVertex[3]->Pt_Point->x - pVertex[0]->Pt_Point->x;
	w.y = pVertex[3]->Pt_Point->y - pVertex[0]->Pt_Point->y;
	dA += 0.5*abs(v.Cross(w));
	return (dA);
}

//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( E_ObjectR, CObject )

E_ObjectR::E_ObjectR()
{
iDOF=DOF_ALL;
PID=999;
PIDunv=999;
dALPHA=0;
}

void E_ObjectR::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<iNoNodes;i++)
   {
    pVertex[i] = pInVertex[i];
   }
iDOF=DOF_ALL;
}

void E_ObjectR::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}

}

BOOL E_ObjectR::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	break;
  }
}
return (brc);
}

void E_ObjectR::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
  int i;
	if (ar.IsStoring())
	{
		// TODO: add storing code here

    E_Object::Serialize(ar,iV,MESH);
    for (i=0;i<iNoNodes;i++)
    {
      ar << pVertex[i]->iLabel;
    }
    ar << iDOF;
    ar << dALPHA;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    for (i=0;i<iNoNodes;i++)
    {
      ar>>iNd;;
      pVertex[i] = MESH->GetNode(iNd);
    }
    ar >> iDOF;
    ar >> dALPHA;
	}
}

G_Object* E_ObjectR::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
int i;
E_ObjectR* gret = new E_ObjectR;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  gret->pVertex[i] = MESH->GetNode(pVertex[i]->iLabel);
}
gret->iDOF=iDOF;
gret->pParent=MESH;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

G_Object* E_ObjectR::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
int i;
E_ObjectR* gret = new E_ObjectR;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  EInd=Source->GetNodeInd(pVertex[i]);
  gret->pVertex[i] = Target->pNodes[EInd+iSInd];
}
gret->iDOF=iDOF;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}

G_Object* E_ObjectR::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
int i;
ME_Object* MESH =(ME_Object*) Parrent;
E_ObjectR* gret = new E_ObjectR;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  gret->pVertex[i] = pInVertex[i];
}
gret->iDOF=iDOF;
gret->pParent=Parrent;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

void E_ObjectR::ExportUNV(FILE* pFile)
{
int i;
int iC;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
iC=0;
for (i=0;i<iNoNodes;i++)
{
  if (iC==8)
  {
    fprintf(pFile,"\n","");
	iC=0;
  }
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
  iC++;
}
fprintf(pFile,"\n","");
}

void E_ObjectR::ExportNAS(FILE* pFile)
{
int i;
int iFN;
CString sDof;
sDof=GetDOFString(iDOF);
fprintf(pFile,"%8s%8i%8i%8s","RBE2    ",iLabel,pVertex[0]->iLabel,sDof);
iFN=5;

for (i=1;i<iNoNodes;i++)
{
 fprintf(pFile,"%8i",pVertex[i]->iLabel);
 iFN++;
 if (iFN>9)
 {
   iFN=2;
   fprintf(pFile,"\n","");
   fprintf(pFile,"%8s","        ");
 }
}
fprintf(pFile,"%8s\n",e8(dALPHA));
}

// Draw Object line
void E_ObjectR::Draw(CDC* pDC,int iDrawmode)
{
int i;
for (i=0;i<iNoNodes;i++)
  {
    pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
    pDC->LineTo((int) pVertex[i]->DSP_Point->x,(int) pVertex[i]->DSP_Point->y);
  }
}



void E_ObjectR::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
OglDraw(iDspFlgs,dS1,dS2);
}

G_Object* E_ObjectR::GetNode(int i)
{
return (pVertex[i]);
}

void E_ObjectR::SetDOFString(CString sDOF)
{
  iDOF=GetDOFInt(sDOF);
}

void E_ObjectR::Info()
{
  char S1[80];
  CString sDOF;
  sDOF=GetDOFString(iDOF);
  sprintf_s(S1,"LAB: %i COL: %i PID: %i ELTP: %i",iLabel,iColour,PID,iType);
  outtext1(S1); 
  outtext1(sDOF); 
}





void E_ObjectR::OglDraw(int iDspFlgs,double dS1,double dS2)
{
int i;
char sLab[20];
BOOL bD = FALSE;
C3dVector d[200];
double S=1.0;
double dFS = 1.0;

for (i = 0; i < 200; i++)
{
	d[i].x = 0; d[i].y = 0; d[i].z = 0;
}
ME_Object* ME = (ME_Object*)this->pParent;
if (ME != NULL)
{
	S = ME->dScale;
	dFS = ME->dResFactor;
}

if ((iDspFlgs & DSP_ELEMENTS) > 0)
{
	Selectable = 1;
	C3dVector vCent;
	glColor3fv(cols[iColour]);
	if ((iDspFlgs & DSP_RESDEF) == 0)
	{
		for (i = 0; i < iNoNodes; i++)
		{
			if (pVertex[i]->pResD != NULL)
			{
				d[i] = pVertex[i]->pResD->GetVec();
				d[i] -= ME->vRelDispOff;
				d[i] *= S * dFS;
			}
		}
	}
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (i=1;i<iNoNodes;i++)
	{
	  glVertex3f((float) pVertex[0]->Pt_Point->x + d[0].x,(float) pVertex[0]->Pt_Point->y + d[0].y,(float) pVertex[0]->Pt_Point->z + d[0].z);
	  glVertex3f((float) pVertex[i]->Pt_Point->x + d[i].x,(float) pVertex[i]->Pt_Point->y + d[i].y,(float) pVertex[i]->Pt_Point->z + d[i].z);
	}
	glEnd(); 
	vCent=Get_Centroid();
	if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"E%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
}
else
{
  Selectable=0;
}
}

int E_ObjectR::GetLinkList(cLink* Links[200])
{
int i;
for (i=0;i<iNoNodes-1;i++)
{
  Links[i]=new cLink;
  Links[i]->pParent=this;
  Links[i]->pVertex[0]=pVertex[0];
  Links[i]->pVertex[1]=pVertex[i+1];
  Links[i]->iColour=iColour;
}
return (iNoNodes-1);
}



C3dVector E_ObjectR::Get_Centroid()
{
C3dVector vT;
vT.x = pVertex[0]->Pt_Point->x; 
vT.y = pVertex[0]->Pt_Point->y;
vT.z = pVertex[0]->Pt_Point->z;
return (vT);
}








IMPLEMENT_DYNAMIC( E_ObjectR2, CObject )

E_ObjectR2::E_ObjectR2()
{
iCNA=DOF_ALL;
iCNB=0;
iCMA=0;
iCMB=0;
dALPHA=0;
PIDunv=999;
}

void E_ObjectR2::Create(Pt_Object* pInVertex[200], int iLab,int iCol,int iType,int iPID,int iMat,int iNo,G_Object* Parrent,Property* inPr)
{
E_Object::Create(iLab,iCol,iType,iPID,iMat,iNo,Parrent,inPr);
int i=0;
for (i=0;i<iNoNodes;i++)
   {
    pVertex[i] = pInVertex[i];
   }

}

void E_ObjectR2::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
  int i;
	if (ar.IsStoring())
	{
		// TODO: add storing code here

    E_Object::Serialize(ar,iV,MESH);
    for (i=0;i<iNoNodes;i++)
    {
      ar << pVertex[i]->iLabel;
    }
    ar << dALPHA;
    ar << iCNA;
    ar << iCNB;
    ar << iCMA;
    ar << iCMB;
	}
	else
	{
    E_Object::Serialize(ar,iV,MESH);
    for (i=0;i<iNoNodes;i++)
    {
      ar>>iNd;;
      pVertex[i] = MESH->GetNode(iNd);
    }
    ar >> dALPHA;
    ar >> iCNA;
    ar >> iCNB;
    ar >> iCMA;
    ar >> iCMB;
	}
}

G_Object* E_ObjectR2::Copy(G_Object* Parrent)
{
ME_Object* MESH =(ME_Object*) Parrent;
int i;
E_ObjectR2* gret = new E_ObjectR2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  gret->pVertex[i] = MESH->GetNode(pVertex[i]->iLabel);
}
gret->iDOF=iDOF;
gret->pParent=MESH;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

G_Object* E_ObjectR2::CopyAppend(int iSInd,ME_Object* Target,ME_Object* Source)
{
int EInd;
int i;
E_ObjectR2* gret = new E_ObjectR2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = PID;
gret->PIDunv  = PIDunv;
gret->iType  = iType;
gret->iMatID = iMatID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  EInd=Source->GetNodeInd(pVertex[i]);
  gret->pVertex[i] = Target->pNodes[EInd+iSInd];
}
gret->iDOF=iDOF;
gret->pParent=Target;
gret->pResV = NULL;
return (gret);
}

G_Object* E_ObjectR2::Copy2(G_Object* Parrent,Pt_Object* pInVertex[200],int inPID,int inMID,int inPIDunv)
{
int i;
ME_Object* MESH =(ME_Object*) Parrent;
E_ObjectR2* gret = new E_ObjectR2;
gret->iObjType=iObjType;
gret->iLabel=iLabel;
gret->iColour=iColour;
gret->Drawn = Drawn;
gret->Selectable  = Selectable; 
gret->Visable  = Visable;
gret->PID  = inPID;
gret->PIDunv  = inPIDunv;
gret->iType  = iType;
gret->iMatID = inMID;
gret->iNoNodes = iNoNodes;
for (i=0;i<iNoNodes;i++)
{
  gret->pVertex[i] = pInVertex[i];
}
gret->iDOF=iDOF;
gret->pParent=Parrent;
gret->pPr=pPr;
gret->pResV = NULL;
return (gret);
}

C3dVector E_ObjectR2::Get_Centroid()
{
C3dVector vT;
vT.x = pVertex[0]->Pt_Point->x; 
vT.y = pVertex[0]->Pt_Point->y;
vT.z = pVertex[0]->Pt_Point->z;
return (vT);
}

void E_ObjectR2::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
  OglDraw(iDspFlgs,dS1,dS2);
}


void E_ObjectR2::OglDraw(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
C3dVector vCent;
if ((iDspFlgs & DSP_ELEMENTS) > 0)
{
	Selectable = 1;
	glColor3fv(cols[iColour]);
	glBegin(GL_LINES);
	  glVertex3f((float)pVertex[0]->Pt_Point->x, (float)pVertex[0]->Pt_Point->y, (float)pVertex[0]->Pt_Point->z);
	  glVertex3f((float)pVertex[1]->Pt_Point->x, (float)pVertex[1]->Pt_Point->y, (float)pVertex[1]->Pt_Point->z);
	glEnd();
	vCent = Get_Centroid();
	if (bDrawLab == TRUE)
	{
		sprintf_s(sLab, "E%i", iLabel);
		OglString(iDspFlgs, vCent.x, vCent.y, vCent.z, &sLab[0]);
	}
}
else
{
	Selectable = 0;
}

}

void E_ObjectR2::Draw(CDC* pDC,int iDrawmode)
{
int i;
for (i=0;i<iNoNodes;i++)
  {
    pDC->MoveTo((int) pVertex[0]->DSP_Point->x,(int) pVertex[0]->DSP_Point->y);
    pDC->LineTo((int) pVertex[1]->DSP_Point->x,(int) pVertex[1]->DSP_Point->y);
  }
}

BOOL E_ObjectR2::NodeInEl(Pt_Object* pN)
{
BOOL brc=FALSE;
int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pN)
  {
    brc=TRUE;
	  break;
  }
}
return (brc);
}


void E_ObjectR2::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{

int i=0;
for (i=0;i<iNoNodes;i++)
{
  if (pVertex[i] == pThis)
  {
    pVertex[i]=pWith;
	  break;
  }
}
}

void E_ObjectR2::ExportUNV(FILE* pFile)
{
int i;
int iC;
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",iLabel,iType,PIDunv,iMatID,iColour-150,iNoNodes);
iC=0;
for (i=0;i<iNoNodes;i++)
{
  if (iC==8)
  {
    fprintf(pFile,"\n","");
	iC=0;
  }
  fprintf(pFile,"%10i",pVertex[i]->iLabel);
  iC++;
}
fprintf(pFile,"\n","");
}

void E_ObjectR2::SetOther(CString s1,
                          CString s2,
                          CString s3,
                          CString s4,
                          double dAlp)
{

dALPHA=dAlp;
iCNA=GetDOFInt(s1);
iCNB=GetDOFInt(s2);
iCMA=GetDOFInt(s3);
iCMB=GetDOFInt(s4);
}

void E_ObjectR2::ExportNAS(FILE* pFile)
{

CString sDof1;
CString sDof2;
CString sDof3;
CString sDof4;
sDof1=GetDOFString(iCNA);
sDof2=GetDOFString(iCNB);
sDof3=GetDOFString(iCMA);
sDof4=GetDOFString(iCMB);

fprintf(pFile,"%8s%8i%8i%8i%8s%8s%8s%8s%8s\n","RBAR    ",iLabel,pVertex[0]->iLabel,pVertex[1]->iLabel,sDof1,sDof2,sDof3,sDof4,e8(dALPHA));

}

//----------------------------------------------------------------------------
//    W O R K  P L A N E    O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( WP_Object, CObject )

void WP_Object::Create(double dWPSize)
{
double ds = 0.1*dWPSize;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour =50;
iObjType = 999;
mWPTransform.MakeUnit();
//mWPTransform.Rotate(30,0,45);
//mWPTransform.Translate(3,0,2);

iWPMode = 0;  //Defualt cartesian
Pt_Point[0] = new C3dVector(-dWPSize/2,-dWPSize/2,0);
Pt_Point[1] = new C3dVector(dWPSize/2,-dWPSize/2,0);
Pt_Point[2] = new C3dVector(dWPSize/2,dWPSize/2,0);
Pt_Point[3] = new C3dVector(-dWPSize/2,dWPSize/2,0);



Pt_Point[4] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*0.5,0);
Pt_Point[5] = new C3dVector(-dWPSize/2+ds*2,-dWPSize/2+ds*0.5,0);
Pt_Point[6] = new C3dVector(-dWPSize/2+ds*2,-dWPSize/2+ds*0.25,0);
Pt_Point[7] = new C3dVector(-dWPSize/2+ds*2.4,-dWPSize/2+ds*0.75,0);
Pt_Point[8] = new C3dVector(-dWPSize/2+ds*2,-dWPSize/2+ds*1.25,0);
Pt_Point[9] = new C3dVector(-dWPSize/2+ds*2,-dWPSize/2+ds*1,0);

Pt_Point[10] = new C3dVector(-dWPSize/2+ds*1,-dWPSize/2+ds*1,0);

Pt_Point[11] = new C3dVector(-dWPSize/2+ds*1,-dWPSize/2+ds*2,0);
Pt_Point[12] = new C3dVector(-dWPSize/2+ds*1.25,-dWPSize/2+ds*2,0);
Pt_Point[13] = new C3dVector(-dWPSize/2+ds*0.75,-dWPSize/2+ds*2.4,0);
Pt_Point[14] = new C3dVector(-dWPSize/2+ds*0.25,-dWPSize/2+ds*2,0);
Pt_Point[15] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*2,0);

Pt_Point[16] = new C3dVector(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.25,0);
Pt_Point[17] = new C3dVector(-dWPSize/2+ds*3,-dWPSize/2+ds*1.25,0);
Pt_Point[18] = new C3dVector(-dWPSize/2+ds*2.5,-dWPSize/2+ds*1.25,0);
Pt_Point[19] = new C3dVector(-dWPSize/2+ds*3,-dWPSize/2+ds*0.25,0);


Pt_Point[20] = new C3dVector(-dWPSize/2+ds*0.75,-dWPSize/2+ds*2.5,0);
Pt_Point[21] = new C3dVector(-dWPSize/2+ds*0.75,-dWPSize/2+ds*3,0);
Pt_Point[22] = new C3dVector(-dWPSize/2+ds*1,-dWPSize/2+ds*3.5,0);
Pt_Point[23] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.5,0);

//The Cross Hairs
Pt_Point[24] = new C3dVector(0,-dWPSize/2,0);
Pt_Point[25] = new C3dVector(0,dWPSize/2,0);
Pt_Point[26] = new C3dVector(-dWPSize/2,0,0);
Pt_Point[27] = new C3dVector(dWPSize/2,0,0);
// The R
Pt_Point[28] = new C3dVector(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.25,0);
Pt_Point[29] = new C3dVector(-dWPSize/2+ds*3,-dWPSize/2+ds*0.25,0);
Pt_Point[30] = new C3dVector(-dWPSize/2+ds*2.5,-dWPSize/2+ds*1.25,0.0);
Pt_Point[31] = new C3dVector(-dWPSize/2+ds*3.0,-dWPSize/2+ds*1.25,0.0);
Pt_Point[32] = new C3dVector(-dWPSize/2+ds*3.0,-dWPSize/2+ds*0.75,0.0);
Pt_Point[33] = new C3dVector(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.75,0.0);
// the thete
Pt_Point[34] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*2.5,0);
Pt_Point[35] = new C3dVector(-dWPSize/2+ds*1.0,-dWPSize/2+ds*2.5,0);
Pt_Point[36] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.5,0.0);
Pt_Point[37] = new C3dVector(-dWPSize/2+ds*1.0,-dWPSize/2+ds*3.5,0.0);
Pt_Point[38] = new C3dVector(-dWPSize/2+ds*1.0,-dWPSize/2+ds*3.0,0.0);
Pt_Point[39] = new C3dVector(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.0,0.0);

DSP_Point[0] = new C3dVector();
DSP_Point[1] = new C3dVector();
DSP_Point[2] = new C3dVector();
DSP_Point[3] = new C3dVector();
DSP_Point[4] = new C3dVector();
DSP_Point[5] = new C3dVector();
DSP_Point[6] = new C3dVector();
DSP_Point[7] = new C3dVector();
DSP_Point[8] = new C3dVector();
DSP_Point[9] = new C3dVector();
DSP_Point[10] = new C3dVector();
DSP_Point[11] = new C3dVector();
DSP_Point[12] = new C3dVector();
DSP_Point[13] = new C3dVector();
DSP_Point[14] = new C3dVector();
DSP_Point[15] = new C3dVector();

DSP_Point[16] = new C3dVector();
DSP_Point[17] = new C3dVector();
DSP_Point[18] = new C3dVector();
DSP_Point[19] = new C3dVector();

DSP_Point[20] = new C3dVector();
DSP_Point[21] = new C3dVector();
DSP_Point[22] = new C3dVector();
DSP_Point[23] = new C3dVector();

DSP_Point[24] = new C3dVector();
DSP_Point[25] = new C3dVector();
DSP_Point[26] = new C3dVector();
DSP_Point[27] = new C3dVector();

DSP_Point[28] = new C3dVector();
DSP_Point[29] = new C3dVector();
DSP_Point[30] = new C3dVector();
DSP_Point[31] = new C3dVector();
DSP_Point[32] = new C3dVector();
DSP_Point[33] = new C3dVector();

DSP_Point[34] = new C3dVector();
DSP_Point[35] = new C3dVector();
DSP_Point[36] = new C3dVector();
DSP_Point[37] = new C3dVector();
DSP_Point[38] = new C3dVector();
DSP_Point[39] = new C3dVector();
}




void WP_Object::ReSize(double dWPSize)
{
double ds = 0.1*dWPSize;

if (dWPSize>0)
{

Pt_Point[0]->Set(-dWPSize/2,-dWPSize/2,0);
Pt_Point[1]->Set(dWPSize/2,-dWPSize/2,0);
Pt_Point[2]->Set(dWPSize/2,dWPSize/2,0);
Pt_Point[3]->Set(-dWPSize/2,dWPSize/2,0);

Pt_Point[24]->Set(0,-dWPSize/2,0);
Pt_Point[25]->Set(0,dWPSize/2,0);
Pt_Point[26]->Set(-dWPSize/2,0,0);
Pt_Point[27]->Set(dWPSize/2,0,0);

Pt_Point[4]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*0.5,0);
Pt_Point[5]->Set(-dWPSize/2+ds*2,-dWPSize/2+ds*0.5,0);
Pt_Point[6]->Set(-dWPSize/2+ds*2,-dWPSize/2+ds*0.25,0);
Pt_Point[7]->Set(-dWPSize/2+ds*2.4,-dWPSize/2+ds*0.75,0);
Pt_Point[8]->Set(-dWPSize/2+ds*2,-dWPSize/2+ds*1.25,0);
Pt_Point[9]->Set(-dWPSize/2+ds*2,-dWPSize/2+ds*1,0);

Pt_Point[10]->Set(-dWPSize/2+ds*1,-dWPSize/2+ds*1,0);

Pt_Point[11]->Set(-dWPSize/2+ds*1,-dWPSize/2+ds*2,0);
Pt_Point[12]->Set(-dWPSize/2+ds*1.25,-dWPSize/2+ds*2,0);
Pt_Point[13]->Set(-dWPSize/2+ds*0.75,-dWPSize/2+ds*2.4,0);
Pt_Point[14]->Set(-dWPSize/2+ds*0.25,-dWPSize/2+ds*2,0);
Pt_Point[15]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*2,0);

Pt_Point[16]->Set(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.25,0);
Pt_Point[17]->Set(-dWPSize/2+ds*3,-dWPSize/2+ds*1.25,0);
Pt_Point[18]->Set(-dWPSize/2+ds*2.5,-dWPSize/2+ds*1.25,0);
Pt_Point[19]->Set(-dWPSize/2+ds*3,-dWPSize/2+ds*0.25,0);


Pt_Point[20]->Set(-dWPSize/2+ds*0.75,-dWPSize/2+ds*2.5,0);
Pt_Point[21]->Set(-dWPSize/2+ds*0.75,-dWPSize/2+ds*3,0);
Pt_Point[22]->Set(-dWPSize/2+ds*1,-dWPSize/2+ds*3.5,0);
Pt_Point[23]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.5,0);

Pt_Point[28]->Set(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.25,0);
Pt_Point[29]->Set(-dWPSize/2+ds*3,-dWPSize/2+ds*0.25,0);
Pt_Point[30]->Set(-dWPSize/2+ds*2.5,-dWPSize/2+ds*1.25,0.0);
Pt_Point[31]->Set(-dWPSize/2+ds*3.0,-dWPSize/2+ds*1.25,0.0);
Pt_Point[32]->Set(-dWPSize/2+ds*3.0,-dWPSize/2+ds*0.75,0.0);
Pt_Point[33]->Set(-dWPSize/2+ds*2.5,-dWPSize/2+ds*0.75,0.0);

Pt_Point[34]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*2.5,0);
Pt_Point[35]->Set(-dWPSize/2+ds*1.0,-dWPSize/2+ds*2.5,0);
Pt_Point[36]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.5,0.0);
Pt_Point[37]->Set(-dWPSize/2+ds*1.0,-dWPSize/2+ds*3.5,0.0);
Pt_Point[38]->Set(-dWPSize/2+ds*1.0,-dWPSize/2+ds*3.0,0.0);
Pt_Point[39]->Set(-dWPSize/2+ds*0.5,-dWPSize/2+ds*3.0,0.0);
}
}

// Draw Object line
void WP_Object::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) DSP_Point[0]->x,(int) DSP_Point[0]->y);
pDC->LineTo((int) DSP_Point[1]->x,(int) DSP_Point[1]->y);
pDC->LineTo((int) DSP_Point[2]->x,(int) DSP_Point[2]->y);
pDC->LineTo((int) DSP_Point[3]->x,(int) DSP_Point[3]->y);
pDC->LineTo((int) DSP_Point[0]->x,(int) DSP_Point[0]->y);

pDC->MoveTo((int) DSP_Point[4]->x,(int) DSP_Point[4]->y);
pDC->LineTo((int) DSP_Point[5]->x,(int) DSP_Point[5]->y);
pDC->LineTo((int) DSP_Point[6]->x,(int) DSP_Point[6]->y);
pDC->LineTo((int) DSP_Point[7]->x,(int) DSP_Point[7]->y);
pDC->LineTo((int) DSP_Point[8]->x,(int) DSP_Point[8]->y);
pDC->LineTo((int) DSP_Point[9]->x,(int) DSP_Point[9]->y);
pDC->LineTo((int) DSP_Point[10]->x,(int) DSP_Point[10]->y);
pDC->LineTo((int) DSP_Point[11]->x,(int) DSP_Point[11]->y);
pDC->LineTo((int) DSP_Point[12]->x,(int) DSP_Point[12]->y);
pDC->LineTo((int) DSP_Point[13]->x,(int) DSP_Point[13]->y);
pDC->LineTo((int) DSP_Point[14]->x,(int) DSP_Point[14]->y);
pDC->LineTo((int) DSP_Point[15]->x,(int) DSP_Point[15]->y);
pDC->LineTo((int) DSP_Point[4]->x,(int) DSP_Point[4]->y);

pDC->MoveTo((int) DSP_Point[16]->x,(int) DSP_Point[16]->y);
pDC->LineTo((int) DSP_Point[17]->x,(int) DSP_Point[17]->y);
pDC->MoveTo((int) DSP_Point[18]->x,(int) DSP_Point[18]->y);
pDC->LineTo((int) DSP_Point[19]->x,(int) DSP_Point[19]->y);

pDC->MoveTo((int) DSP_Point[20]->x,(int) DSP_Point[20]->y);
pDC->LineTo((int) DSP_Point[21]->x,(int) DSP_Point[21]->y);
pDC->LineTo((int) DSP_Point[22]->x,(int) DSP_Point[22]->y);
pDC->MoveTo((int) DSP_Point[21]->x,(int) DSP_Point[21]->y);
pDC->LineTo((int) DSP_Point[23]->x,(int) DSP_Point[23]->y);
}

void WP_Object::OglDrawW(int iDspFlgs,double dS1,double dS2)
{

C3dVector T_Point[40];
C3dVector V;
int i;
if ((iDspFlgs & DSP_WP) > 0)
{
	Selectable = 1;
	for (i = 0; i < 40; i++)
	{
		V.x = Pt_Point[i]->x;
		V.y = Pt_Point[i]->y;
		V.z = Pt_Point[i]->z;
		T_Point[i] = mWPTransform * V;
	}
	glColor3fv(cols[GetCol()]);
	glLineWidth(12.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f((float)T_Point[0].x, (float)T_Point[0].y, (float)T_Point[0].z);
	glVertex3f((float)T_Point[1].x, (float)T_Point[1].y, (float)T_Point[1].z);
	glVertex3f((float)T_Point[2].x, (float)T_Point[2].y, (float)T_Point[2].z);
	glVertex3f((float)T_Point[3].x, (float)T_Point[3].y, (float)T_Point[3].z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f((float)T_Point[4].x, (float)T_Point[4].y, (float)T_Point[4].z);
	glVertex3f((float)T_Point[5].x, (float)T_Point[5].y, (float)T_Point[5].z);
	glVertex3f((float)T_Point[6].x, (float)T_Point[6].y, (float)T_Point[6].z);
	glVertex3f((float)T_Point[7].x, (float)T_Point[7].y, (float)T_Point[7].z);
	glVertex3f((float)T_Point[8].x, (float)T_Point[8].y, (float)T_Point[8].z);
	glVertex3f((float)T_Point[9].x, (float)T_Point[9].y, (float)T_Point[9].z);
	glVertex3f((float)T_Point[10].x, (float)T_Point[10].y, (float)T_Point[10].z);
	glVertex3f((float)T_Point[11].x, (float)T_Point[11].y, (float)T_Point[11].z);
	glVertex3f((float)T_Point[12].x, (float)T_Point[12].y, (float)T_Point[12].z);
	glVertex3f((float)T_Point[13].x, (float)T_Point[13].y, (float)T_Point[13].z);
	glVertex3f((float)T_Point[14].x, (float)T_Point[14].y, (float)T_Point[14].z);
	glVertex3f((float)T_Point[15].x, (float)T_Point[15].y, (float)T_Point[15].z);
	glEnd();

	//The R
	if (iWPMode == 1)
	{
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[28].x, (float)T_Point[28].y, (float)T_Point[28].z);
		glVertex3f((float)T_Point[30].x, (float)T_Point[30].y, (float)T_Point[30].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[30].x, (float)T_Point[30].y, (float)T_Point[30].z);
		glVertex3f((float)T_Point[31].x, (float)T_Point[31].y, (float)T_Point[31].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[31].x, (float)T_Point[31].y, (float)T_Point[31].z);
		glVertex3f((float)T_Point[32].x, (float)T_Point[32].y, (float)T_Point[32].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[32].x, (float)T_Point[32].y, (float)T_Point[32].z);
		glVertex3f((float)T_Point[33].x, (float)T_Point[33].y, (float)T_Point[33].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[33].x, (float)T_Point[33].y, (float)T_Point[33].z);
		glVertex3f((float)T_Point[29].x, (float)T_Point[29].y, (float)T_Point[29].z);
		glEnd();
		//Theta
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[38].x, (float)T_Point[38].y, (float)T_Point[38].z);
		glVertex3f((float)T_Point[39].x, (float)T_Point[39].y, (float)T_Point[39].z);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex3f((float)T_Point[34].x, (float)T_Point[34].y, (float)T_Point[34].z);
		glVertex3f((float)T_Point[35].x, (float)T_Point[35].y, (float)T_Point[35].z);
		glVertex3f((float)T_Point[37].x, (float)T_Point[37].y, (float)T_Point[37].z);
		glVertex3f((float)T_Point[36].x, (float)T_Point[36].y, (float)T_Point[36].z);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[16].x, (float)T_Point[16].y, (float)T_Point[16].z);
		glVertex3f((float)T_Point[17].x, (float)T_Point[17].y, (float)T_Point[17].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[18].x, (float)T_Point[18].y, (float)T_Point[18].z);
		glVertex3f((float)T_Point[19].x, (float)T_Point[19].y, (float)T_Point[19].z);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f((float)T_Point[20].x, (float)T_Point[20].y, (float)T_Point[20].z);
		glVertex3f((float)T_Point[21].x, (float)T_Point[21].y, (float)T_Point[21].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[21].x, (float)T_Point[21].y, (float)T_Point[21].z);
		glVertex3f((float)T_Point[22].x, (float)T_Point[22].y, (float)T_Point[22].z);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f((float)T_Point[21].x, (float)T_Point[21].y, (float)T_Point[21].z);
		glVertex3f((float)T_Point[23].x, (float)T_Point[23].y, (float)T_Point[23].z);
		glEnd();
	}
	glColor3fv(cols[154]);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0101);
	glBegin(GL_LINES);
	glVertex3f((float)T_Point[24].x, (float)T_Point[24].y, (float)T_Point[24].z);
	glVertex3f((float)T_Point[25].x, (float)T_Point[25].y, (float)T_Point[25].z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f((float)T_Point[26].x, (float)T_Point[26].y, (float)T_Point[26].z);
	glVertex3f((float)T_Point[27].x, (float)T_Point[27].y, (float)T_Point[27].z);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(2.0);
}
else
{
Selectable = 0;
}
}

void WP_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
	OglDrawW(iDspFlgs, dS1, dS2);
}

void WP_Object::Reset()
{
mWPTransform.MakeUnit();
}

C3dVector WP_Object::Get_Centroid()
{
	C3dVector vC;
	vC.Set(0, 0, 0);
	vC = mWPTransform.Mult(vC);
	return (vC);
}



void WP_Object::Align(C3dVector p1,C3dVector p2,C3dVector p3)
{
C3dVector vX;
C3dVector vZ;
C3dVector vYP;
vX=p2;
vX-=p1;
vX.Normalize();
vYP=p3;
vYP-=p1;
vYP.Normalize();
if ((vX.Mag()!=0) && (vYP.Mag()!=0))
{
  if (abs(vX.Dot(vYP))!=1)
  {
    vZ=vX.Cross(vYP);
    vYP=vZ.Cross(vX);
	  vX.Normalize();
    vYP.Normalize();
	  vZ.Normalize();
    mWPTransform.SetColVec(1,vX);
    mWPTransform.SetColVec(2,vYP);
    mWPTransform.SetColVec(3,vZ);
	  mWPTransform.Translate2(p1.x,p1.y,p1.z);
  }
}
}


void WP_Object::CentrePt(C3dVector p1)
{
  mWPTransform.Translate2(p1.x,p1.y,p1.z);
}

void WP_Object::AlignZ(C3dVector p1,C3dVector p2,C3dVector p3)
{
//  TWP->AlignZ(pO,pX,pZ);
C3dVector vX;
C3dVector vZ;
C3dVector vYP;
vX=p2;
vX-=p1;
vX.Normalize();
vZ=p3;
vZ.Normalize();
if ((vX.Mag()!=0) && (vZ.Mag()!=0))
{
  if (abs(vX.Dot(vZ))!=1)
  {
    //vZ=vX.Cross(vYP);
    vYP=vZ.Cross(vX);
    vX=vYP.Cross(vZ);
	  vX.Normalize();
    vYP.Normalize();
	  vZ.Normalize();
    mWPTransform.SetColVec(1,vX);
    mWPTransform.SetColVec(2,vYP);
    mWPTransform.SetColVec(3,vZ);
	  mWPTransform.Translate2(p1.x,p1.y,p1.z);
  }
}
}


void WP_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int j=0;
C3dVector V;
C3dVector R;
C3dMatrix mM;

for (j=0;j<28;j++)
   {
    V.x = Pt_Point[j]->x;
	V.y = Pt_Point[j]->y;
	V.z = Pt_Point[j]->z;
	R = mWPTransform * V; 
    V = pModMat->Mult(R);
    R = pScrTran->Mult(V);
    DSP_Point[j]->x = R.x;
	DSP_Point[j]->y = R.y;
	DSP_Point[j]->z = R.z;
   }

}

void WP_Object::Translate(C3dVector vIn)
{
	mWPTransform.m_30 += vIn.x;
	mWPTransform.m_31 += vIn.y;
	mWPTransform.m_32 += vIn.z;
}

void WP_Object::Transform(C3dMatrix TMat)
{
	//incorect
mWPTransform = TMat* mWPTransform;
}


void WP_Object::HighLight(CDC* pDC)
{
Draw(pDC,4);
}


G_ObjectD WP_Object::SelDist(CPoint InPT,Filter FIL)
{
double MinDist = 1e36;
double dDist;
int i;
G_ObjectD Ret;
G_Object* pO;
Ret.Dist = MinDist;
Ret.pObj = this;
Ret.Z = 0;

for (i = 0; i < 28; i++)
{
	dDist = pow((DSP_Point[i]->x - InPT.x), 2) + pow((DSP_Point[i]->y - InPT.y), 2);
	if (dDist < MinDist)
	{
		MinDist = dDist;
		Ret.Dist = dDist;
		Ret.pObj = this;
		Ret.Z = 0;
	}
}
return (Ret);
}


//****************************************************************************
//           S O L U T I O N    S E T    O B J E C T S
//****************************************************************************
IMPLEMENT_DYNAMIC(Solution, CObject)
//iType 0 = LIN STAT
//iType 1 = THERMAL

Solution::Solution()
{
  iType=0;
  sTitle="UNDEFINED";
  iNo=0;
  dTol=1.0e-9;
  iCur=-1;
}

CString Solution::GetSolutionTitleString()
{
CString src="ERROR.";
char S1[80]="";
sprintf_s(S1,"%s TP: %i TOL: %g",sTitle,iType,dTol);
src=S1;
return (src);
}

Solution::Solution(int iT, CString sTit,double dT)
{
  iType=iT;
  sTitle=sTit;
  iNo=0;
  dTol=dT;
  iCur=-1;
}

Solution::~Solution()
{
  iNo=0;
  iType=0;
  sTitle="UNDEFINED";
}

void Solution::AddStep(CString sT,int idLS,int idBS,int idTS,BOOL bRS)
{
  if (iNo<MAX_STEPS)
  {
    LS[iNo]=idLS;
    BS[iNo]=idBS;
    TS[iNo]=idTS; 
    RS[iNo]=bRS;
    sStepTitle[iNo]=sT;
    iNo++;
  }
  else
  {
    outtext1("ERROR: Max no of Steps Exceeded in Solution Set.");
  }
}

void Solution::DelStep(int ind)
{
  int i;
  if ((ind>=0) && (ind<iNo))
  {
    for (i=ind+1;i<iNo-1;i++)
	{
      LS[i-1]=LS[i];
      BS[i-1]=BS[i];
      TS[i-1]=TS[i]; 
      RS[i-1]=RS[i];
      sStepTitle[i-1]=sStepTitle[i];
	}
	iNo--;
  }
  iCur=-1;
}

void Solution::Serialize(CArchive& ar,int iV)
{
  int i;
  if (ar.IsStoring())
  {
    ar << iNo;
    ar << iCur;
    ar << sTitle;
    ar << iType;
    ar << dTol;
    for (i=0;i<iNo;i++)
    {
      ar << LS[i];
      ar << BS[i];
      ar << TS[i]; 
      ar << RS[i];
      ar << sStepTitle[i];
    }
  }
  else
  {
    ar >> iNo;
    ar >> iCur;
    ar >> sTitle;
    ar >> iType;
    ar >> dTol;
    for (i=0;i<iNo;i++)
    {
      ar >> LS[i];
      ar >> BS[i];
      ar >> TS[i]; 
      ar >> RS[i];
      ar >> sStepTitle[i];
    }
  }
}

BOOL Solution::SetCurStep(int iC)
{
  BOOL brc = FALSE;
  if ((iC>-1) && (iC < iNo))
  {
    iCur = iC;
    brc = TRUE;
  }
  else
    iCur = -1;
  return (brc);
}

int  Solution::GetCurStep()
{

return (iCur);
}


CString Solution::GetStepTitleString(int iC)
{
CString src;
char S1[80]="";
if ((iC>-1) && (iC<iNo))
{
  sprintf_s(S1,"STEP: %i LC: %i BC: %i TSET: %i RS: %i %s",iC,LS[iC],BS[iC],TS[iC],RS[iC],sStepTitle[iC]);
  src=S1;
}
else
{
  src="ERROR.";
}
return (src);
}

void Solution::Info(int iD)
{
  int i;
  char S1[200]="";
  CString sT;
  sprintf_s(S1,"ID: %i TITLE: %s",iD,sTitle);
  outtext1(S1); 

  if (iType==0)
    sT="SOLUTION TYPE: Linear Static.";
  else if (iType==1)
    sT="SOLUTION TYPE: Steady State Thermal.";
  else
    sT="SOLUTION TYPE: UNDEFINED.";
  outtext1(sT);

  for (i=0;i<iNo;i++)
  {
    sprintf_s(S1,"STEP: %i LC: %i BC: %i TSET: %i RS: %i %s",i,LS[i],BS[i],TS[i],RS[i],sStepTitle[i]);
    outtext1(S1);
  }
}


//****************************************************************************
//               A L L    S O L U T I O N    S E T S
//****************************************************************************
IMPLEMENT_DYNAMIC(SolSets , CObject)

SolSets::SolSets()
{
  int i;
  iCur=-1;
  iNo=0;
  sTitle="UNDEFINED";
  for(i=0;i<MAX_SOLS;i++)
  {
    pSols[i]=NULL;    
  }
}

SolSets::SolSets(CString sTitle)
{
  int i;
  iCur=-1;
  iNo=0;
  sTitle=sTitle;
  for(i=0;i<MAX_SOLS;i++)
  {
    pSols[i]=NULL;    
  }
}

SolSets::~SolSets()
{
  int i;
  if (iNo>0)
  { 
    for(i=0;i<iNo;i++)
    {
       if (pSols[i]!=NULL)
         delete(pSols[i]);
    }
  }
}

void SolSets::AddSolution(int iT,CString sTit,double dT)
{
  if (iNo<MAX_SOLS)
  {
    pSols[iNo]=new Solution(iT,sTit,dT);
    iCur=iNo;
    iNo++;
  }
  else
  {
    outtext1("ERROR: Max No of Solutions Exceeded.");
  }
}

//add step to current solution
void SolSets::AddStep(CString sT,int idLS,int idBS,int idTS,BOOL bRS)
{
  if (iCur!=-1)
  {
    if (pSols[iCur]!=NULL)
      pSols[iCur]->AddStep(sT,idLS,idBS,idTS,bRS);
  }
}


void SolSets::DelSolution(int ind)
{
  int i;
  if ((ind>=0) && (ind<iNo))
  {
    if (pSols[ind]!=NULL)
    {
      delete(pSols[ind]);
      for (i=ind+1;i<iNo-1;i++)
	    {
        pSols[i-1]=pSols[i];
	    }
	    iNo--;
    }
  }
}

void SolSets::Serialize(CArchive& ar,int iV)
{
  int i;
  if (ar.IsStoring())
  {
    ar << iNo;
    ar << iCur;
    ar << sTitle;
    for (i=0;i<iNo;i++)
    {
      pSols[i]->Serialize(ar,iV);
    }
  }
  else
  {
    ar >> iNo;
    ar >> iCur;
    ar >> sTitle;
    for (i=0;i<iNo;i++)
    {
      pSols[i]=new Solution();
      pSols[i]->Serialize(ar,iV);
    }
  }
}

Solution* SolSets::GetCurSolution()
{
Solution* pSol=NULL;
if (iCur>-1)
  pSol=pSols[iCur];
return (pSol);
}

BOOL SolSets::SetCurSol(int iC)
{
  BOOL brc = FALSE;
  if ((iC > -1) && (iC < iNo))
  {
    iCur = iC;
    brc = TRUE;
  }
  else
    iCur = -1;
  return (brc);
}

BOOL SolSets::SetCurStep(int iC)
{
BOOL brc = FALSE;
if (iCur>-1)
  brc=pSols[iCur]->SetCurStep(iC);
else
{
  outtext1("ERROR: No Solution Active.");
}
return (brc);
}

int  SolSets::GetCurStep()
{
int irc=-1;
if (iCur>-1)
  irc=pSols[iCur]->GetCurStep();
else
  outtext1("ERROR: No Solution Active.");
return (irc);
}


CString SolSets::GetTitleString(int iC)
{
CString src;
char S1[80]="";

if (iC>-1)
{
  sprintf_s(S1,"SOL %i ",iC);
  src=S1;
  src+=" ";
  src+=pSols[iC]->GetSolutionTitleString();
}
else
  src="ERROR.";
return (src);
}


void SolSets::Info()
{
  int i;
  char S1[200]="";
  outtext1("S O L U T I O N    S E Q E N C E S");
  for (i=0;i<iNo;i++)
  {
    pSols[i]->Info(i);
  }
  sprintf_s(S1,"CURRENT SOL SEQ: %i",iCur);
  outtext1(S1);
}

//****************************************************************************
//                 T H E    M E S H    O B J E C T
//****************************************************************************
IMPLEMENT_DYNAMIC(ME_Object, CObject)

	
ME_Object::~ME_Object()
{
   int i;
   for(i=0;i<iBCLDs;i++)
   {
     delete(pBCLDs[i]);
     pBCLDs[i]=NULL;
   }
   for(i=0;i<iElNo;i++)
   {
     delete(pElems[i]);
   }
   for(i=0;i<iNdNo;i++)
   {
     delete(pNodes[i]);
   }
   for(i=0;i<iNoGps;i++)
   {
     delete(GPs[i]);
   }
   if (FcList!=NULL)
	   delete(FcList);
   if (LkList!=NULL)
	   delete(LkList);
   if (pSOLS!=NULL)
       delete(pSOLS);
   if (pResVectors != NULL)
	   delete(pResVectors);
}

ME_Object::ME_Object()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 4;
iNdNo = 0;
iElNo = 0;
iNoGps = 0;
sName="NULL";
TransMat.MakeUnit();
pParent=NULL;
iNodeLab=0;
iElementLab=0;
bDrawN=TRUE;
bDrawCYS=TRUE;
TempList=NULL;
iCYS=0;
iCYSLab=1;
iNodeLab=1;
iElementLab=1;
dScale=10;
dScaleVec = 0.1;
dResFactor = 1.0;
vRelDispOff.Set(0, 0, 0);
iNoRes=0;
iCurResSet=-1;
iResVal=-1;
iCurResSetDef=-1;
iResValDef=-1;
iPostOpt=0;
iCurResVecSet=-1;
iResVec=-1;
iSecVecID=-1;
bUserColBar=FALSE;
//Loadsets BCs and Temperature Sets
iCurLC=-1;
iNoLCs=0;
iCurBC=-1;
iNoBCs=0;
iCurTSet=-1;
iNoTSets=0;
pResVectors = NULL;
FcList=NULL;
LkList=NULL;
pSOLS=new SolSets("UNDEFINED");
}

void ME_Object::Create(CString inName,G_Object* Parrent,int iLab)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 4;
iNdNo = 0;
iElNo = 0;
iBCLDs=0;
iNoGps = 0;
sName=inName;
TransMat.MakeUnit();
pParent=Parrent;
iNodeLab=1;
iElementLab=1;
iLabel = iLab;
TempList=NULL;
iCYS=0;
iCYSLab=1;
CResSet=NULL;
CResSetDef=NULL;
iCVar=0;
iCVarDef=0;
dScale=10;
dScaleVec = 0.1;
iPostOpt=0;
bUserColBar=FALSE;
iCurLC=-1;
iNoLCs=0;
iCurBC=-1;
iNoBCs=0;
iCurTSet=-1;
iNoTSets=0;
iCurResVecSet = -1;
iResVec = -1;
iSecVecID = -1;
}



void ME_Object::Info()
{
  char S1[200];
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("MESH OBJECT");
  outtext1(S1); 
  sprintf_s(S1,"%s%s","Name : ",sName);
  outtext1(S1);
  sprintf_s(S1, "%s%i", "Number of Nodes : ", iNdNo);
  outtext1(S1); 
  sprintf_s(S1,"%s%i","Number of Elements : ",iElNo);
  outtext1(S1); 
  sprintf_s(S1, "%s%i", "Max Node Label : ", iNodeLab);
  outtext1(S1);
  sprintf_s(S1, "%s%i", "Max Elem Label : ", iElementLab);
  outtext1(S1);
  sprintf_s(S1, "%s%i", "Max CSYS Label : ", iCYSLab);
  outtext1(S1);
  outtext1("    ****");
  int iS;
  iS = GetLCID(iCurLC);
  sprintf_s(S1, "%s%i", "Current Load Set : ", iS);
  outtext1(S1);
  iS = GetBCID(iCurBC);
  sprintf_s(S1, "%s%i", "Current BC Set   : ", iS);
  outtext1(S1);
  iS = GetTSETID(iCurTSet);
  sprintf_s(S1, "%s%i", "Current Temp Set : ", iS);
  outtext1(S1);




  outtext1("    ****");
}

void ME_Object::BuildNodeList()
{
int i;
TempList = new ObjTempList();
for (i=0;i<iNdNo;i++)
{
  if (pNodes[i]->iLabel<99999999) 
    TempList->Objs[pNodes[i]->iLabel]=pNodes[i];
}
}

void ME_Object::DeleteNodeList()
{
delete(TempList);
TempList = NULL;
}

BOOL ME_Object::isActiveSolSet()
{
BOOL brc=FALSE;
if (pSOLS!=NULL)
{
  if (pSOLS->iCur!=-1)
   brc=TRUE;
}
return(brc);
}

BOOL ME_Object::isValidLCid(int iLC)
{
int i;
BOOL brc=FALSE;
for(i=0;i<iNoLCs;i++)
{
  if (LCS[i]->iLabel==iLC)
  {
    brc=TRUE;
	  break;
  }
}
return (brc);
}

BOOL ME_Object::isValidBCid(int iLC)
{
int i;
BOOL brc=FALSE;
for(i=0;i<iNoBCs;i++)
{
  if (BCS[i]->iLabel==iLC)
  {
    brc=TRUE;
	  break;
  }
}
return (brc);
}

BOOL ME_Object::isValidTCid(int iLC)
{
int i;
BOOL brc=FALSE;
for(i=0;i<iNoTSets;i++)
{
  if (TSETS[i]->iLabel==iLC)
  {
    brc=TRUE;
	  break;
  }
}
return (brc);
}

int ME_Object::NestTSetID()
{
  int i;
  int irc = 0;
  for (i = 0; i<iNoTSets; i++)
  {
    if (TSETS[i]->iLabel > irc)
    {
      irc = TSETS[i]->iLabel;
    }
  }
  irc++;
  return (irc);
}

int ME_Object::GetLCID(int ind)
{
int irc = -1;
if (ind > -1)
  irc=LCS[ind]->iLabel;
return (irc);
}

int ME_Object::GetLCind(int ID)
{
  int iret=-1;
  int i;
  for (i = 0; i < iNoLCs; i++)
  {
    if (LCS[i]->iLabel == ID)
    {
      iret = i;
      break;
    }
  }
  return (iret);
}


int ME_Object::GetBCID(int ind)
{
int irc = -1;
if (ind > -1)
  irc=BCS[ind]->iLabel;
return (irc);
}

int ME_Object::GetBCind(int ID)
{
  int iret = -1;
  int i;
  for (i = 0; i < iNoBCs; i++)
  {
    if (BCS[i]->iLabel == ID)
    {
      iret = i;
      break;
    }
  }
  return (iret);
}

int ME_Object::GetTSETID(int ind)
{
int irc = -1;
if (ind>-1)
  irc=TSETS[ind]->iLabel;
return (irc);
}

int ME_Object::GetTSETind(int ID)
{
  int iret = -1;
  int i;
  for (i = 0; i < iNoTSets; i++)
  {
    if (TSETS[i]->iLabel == ID)
    {
      iret = i;
      break;
    }
  }
  return (iret);
}


cLinkedList* ME_Object::GetLC(int iSet)
{
int i;
cLinkedList* pRet=NULL;
for(i=0;i<iNoLCs;i++)
{
  if (LCS[i]->iLabel==iSet)
  {
    pRet=LCS[i];
	break;
  }
}
return (pRet);
}

cLinkedListB* ME_Object::GetBC(int iSet)
{
int i;
cLinkedListB* pRet=NULL;
for(i=0;i<iNoBCs;i++)
{
  if (BCS[i]->iLabel==iSet)
  {
    pRet=BCS[i];
	break;
  }
}

return (pRet);
}


cLinkedListT* ME_Object::GetTSET(int iSet)
{
int i;
cLinkedListT* pRet=NULL;
for(i=0;i<iNoTSets;i++)
{
  if (TSETS[i]->iLabel==iSet)
  {
    pRet=TSETS[i];
	break;
  }
}
return (pRet);
}

void ME_Object::SetCurLC(int iSet)
{

char buff[200];
int iSInd;
iSInd = GetLCind(iSet);
if ((iSInd>-1) && (iSInd<iNoLCs))
{
  iCurLC = iSInd;
  sprintf_s(buff, "%3i%s ID%i %s",iCurLC," : ",LCS[iCurLC]->iLabel,LCS[iCurLC]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: Invalid Set ID.");
  iCurLC = -1;
}
}

void ME_Object::SetCurBC(int iSet)
{
char buff[200];
int iSInd;
iSInd = GetBCind(iSet);
if ((iSInd>-1) && (iSInd<iNoBCs))
{
  iCurBC = iSInd;
  sprintf_s(buff, "%3i%s ID%i %s",iCurBC," : ",BCS[iCurBC]->iLabel,BCS[iCurBC]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: Invalid Set ID.");
  iCurBC = -1;
}
}

void ME_Object::SetCurTSET(int iSet)
{
char buff[200];
int iSInd;
iSInd = GetTSETind(iSet);
if ((iSInd>-1) && (iSInd<iNoTSets))
{
  iCurTSet = iSInd;
  sprintf_s(buff, "%3i%s ID%i %s",iCurTSet," : ",TSETS[iCurTSet]->iLabel,TSETS[iCurTSet]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: Invalid Set ID.");
  iCurTSet = -1;
}
}

void ME_Object::ListLC()
{
int i;
char buff[200];
CString iGp;
CString oLine;
outtext1("LOAD SET LISTING:-");

for (i=0;i<iNoLCs;i++)
{ 
	sprintf_s(buff, "%3i%s ID%i %s",i," : ",LCS[i]->iLabel,LCS[i]->sTitle);
  outtext1(buff);
}
if ((iNoLCs > 0) && (iCurLC<iNoLCs))
{
  outtext1("Active Load Set:-");
  sprintf_s(buff, "%3i%s ID%i %s",iCurLC," : ",LCS[iCurLC]->iLabel,LCS[iCurLC]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: No Active Load Set.");
}
}

void ME_Object::ListBC()
{
int i;
char buff[200];
CString iGp;
CString oLine;
outtext1("BOUNDARY SET LISTING:-");

for (i=0;i<iNoBCs;i++)
{ 
	sprintf_s(buff, "%3i%s ID%i %s",i," : ",BCS[i]->iLabel,BCS[i]->sTitle);
  outtext1(buff);
}
if ((iNoBCs > 0) && (iCurBC<iNoBCs))
{
  outtext1("Active Boundary Set:-");
  sprintf_s(buff, "%3i%s ID%i %s",iCurBC," : ",BCS[iCurBC]->iLabel,BCS[iCurBC]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: No Active Boundary Set.");
}
}

void ME_Object::ListTSET()
{
int i;
char buff[200];
CString iGp;
CString oLine;
outtext1("TEMPERATURE SET LISTING:-");

for (i=0;i<iNoTSets;i++)
{ 
	sprintf_s(buff, "%3i%s ID%i %s",i," : ",TSETS[i]->iLabel,TSETS[i]->sTitle);
  outtext1(buff);
}
if ((iNoTSets > 0) && (iCurTSet<iNoTSets))
{
  outtext1("Active Temperature Set:-");
  sprintf_s(buff, "%3i%s ID%i %s",iCurTSet," : ",TSETS[iCurTSet]->iLabel,TSETS[iCurTSet]->sTitle);
  outtext1(buff);
}
else
{
  outtext1("ERROR: No Active Temperature Set.");
}
}

void ME_Object::DeleteLC(int iSet)
{
int i;
char buff[80];

if ((iSet>-1) && (iSet<iNoLCs))
{
  sprintf_s(buff, "%s%s","Deleting Load Set:- ",LCS[iSet]->sTitle);
  LCS[iSet]->DeleteAll();
  delete(LCS[iSet]);
  outtext1(buff);
  if (iCurLC>=iSet)
    iCurLC--;
  
  for (i=iSet;i<iNoLCs-1;i++)
  {
    LCS[i]=LCS[i+1];
  }
  iNoLCs--;
  if (iNoLCs==0)
    {iCurLC=-1;}
}
}

void ME_Object::DeleteBC(int iSet)
{
int i;
char buff[80];

if ((iSet>-1) && (iSet<iNoBCs))
{
  sprintf_s(buff, "%s%s","Deleting Boundary Set:-",BCS[iSet]->sTitle);
  BCS[iSet]->DeleteAll();
  delete(BCS[iSet]);
  outtext1(buff);
  if (iCurBC>=iSet)
    iCurBC--;
  for (i=iSet;i<iNoBCs-1;i++)
  {
    BCS[i]=BCS[i+1];
  }
  iNoBCs--;
  if (iNoLCs==0)
    {iCurBC=-1;}
}
}

void ME_Object::DeleteTSET(int iSet)
{
int i;
char buff[80];

if ((iSet>-1) && (iSet<iNoTSets))
{
  sprintf_s(buff, "%s%s","Deleting Temperature Set:-",TSETS[iSet]->sTitle);
  TSETS[iSet]->DeleteAll();
  delete(TSETS[iSet]);
  outtext1(buff);
  if (iCurTSet>=iSet)
    iCurTSet--;
  for (i=iSet;i<iNoTSets-1;i++)
  {
    TSETS[i]=TSETS[i+1];
  }
  iNoTSets--;
  if (iNoLCs==0)
    {iCurTSet=-1;}
}
}

//Loadset BC as TSet Creation
int ME_Object::CreateLC(int ID, CString sTit)
{
int irc=-1;
if (iNoLCs<MAX_SETS)
{
  LCS[iNoLCs]=new cLinkedList();
  LCS[iNoLCs]->iLabel=ID;
  LCS[iNoLCs]->sTitle=sTit;
  LCS[iNoLCs]->pParent=this;
  if (iCurLC==-1)
    iCurLC=iNoLCs;
  irc=iCurLC;
  iNoLCs++;
}
return (irc);
}

int ME_Object::CreateBC(int ID, CString sTit)
{
int irc=-1;
if (iNoBCs<MAX_SETS)
{
  BCS[iNoBCs]=new cLinkedListB();
  BCS[iNoBCs]->iLabel=ID;
  BCS[iNoBCs]->sTitle=sTit;
  BCS[iNoBCs]->pParent=this;
  //if (iCurBC==-1)
    iCurBC=iNoBCs;
  irc=iCurBC;
  iNoBCs++;
}
return (irc);
}

int ME_Object::CreateTSET(int ID, CString sTit)
{
int irc=-1;
if (iNoTSets<MAX_SETS)
{
  TSETS[iNoTSets]=new cLinkedListT();
  TSETS[iNoTSets]->iLabel=ID;
  TSETS[iNoTSets]->sTitle=sTit;
  TSETS[iNoTSets]->pParent=this;
  if (iCurTSet==-1)
    iCurTSet=iNoTSets;
  irc=iCurTSet;
  iNoTSets++;
}
return (irc);
}

//iType is resulatant entity type
//pThis the object that has the association
void ME_Object::RelTo(G_Object* pThis,ObjList* pList,int iType)
{
int i;
int j;

//we are looking for Loads related to either element or node
if ((iType==321) || (iType==323) 
||  (iType==324) || (iType==326) 
||  (iType==328) || (iType==329))
{
  if (iCurLC!=-1)
  {
    BCLD* pNext;
    pNext=(BCLD*) LCS[iCurLC]->Head;
    while (pNext!=NULL)
    {
      if (pNext->pObj==pThis)
        pList->AddEx(pNext);
      pNext=(BCLD*) pNext->next;
    }
  }
}
//we are looking for Restrainrs related to node
if ((iType==322) || (iType==327))
{
  if (iCurBC!=-1)
  {
    BCLD* pNext;
    pNext=(BCLD*) BCS[iCurBC]->Head;
    while (pNext!=NULL)
    {
      if (pNext->pObj==pThis)
        pList->AddEx(pNext);
      pNext=(BCLD*) pNext->next;
    }
  }
}

if (iType==325)
{
  if (iCurTSet!=-1)
  {
    BCLD* pNext;
    pNext=(BCLD*) TSETS[iCurTSet]->Head;
    while (pNext!=NULL)
    {
      if (pNext->pObj==pThis)
        pList->AddEx(pNext);
      pNext=(BCLD*) pNext->next;
    }
  }
}


if ((iType==1) ||  (iType==3) ||  (iType==4) ||  (iType==12))
{
if ((pThis->iObjType==1) ||  
	  (pThis->iObjType==3) ||  
    (pThis->iObjType==200) ||
    (pThis->iObjType==201) ||
    (pThis->iObjType==202))
{
  if (iType==4)
  {
    if (pThis->pParent!=NULL)
    {
     pList->AddEx(pThis->pParent);
    }
  }
  else if ((iType==1) &&  (pThis->iObjType==3)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     for (i=0;i<iElNo;i++)
     {
       if (pElems[i]== pThis)
       {
          for (j=0;j<pElems[i]->iNoNodes;j++)
          {
             pList->AddEx(pElems[i]->GetNode(j));
          }
          break;
       }
     }
  }
  else if ((iType==1) &&  (pThis->iObjType==321)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     G_Object* pN;
     pN=GetNode(pThis->iLabel);
     if (pN!=NULL)
       pList->AddEx(pN);
  }
  else if ((iType==1) &&  (pThis->iObjType==322)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     G_Object* pN;
     pN=GetNode(pThis->iLabel);
     if (pN!=NULL)
       pList->AddEx(pN);
  }
  else if ((iType==1) &&  (pThis->iObjType==326)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     G_Object* pN;
     pN=GetNode(pThis->iLabel);
     if (pN!=NULL)
       pList->AddEx(pN);
  }
    else if ((iType==1) &&  (pThis->iObjType==327)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     G_Object* pN;
     pN=GetNode(pThis->iLabel);
     if (pN!=NULL)
       pList->AddEx(pN);
  }
  else if ((iType==3) &&  (pThis->iObjType==324)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     E_Object* pE;
     pE=GetElement(pThis->iLabel);
     if (pE!=NULL)
       pList->AddEx(pE);
  }
  else if ((iType==3) &&  (pThis->iObjType==325)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     E_Object* pE;
     pE=GetElement(pThis->iLabel);
     if (pE!=NULL)
       pList->AddEx(pE);
  }
  else if ((iType==3) &&  (pThis->iObjType==328)  &&  (pThis->pParent==this))
  {
     //Node related to element pThis
     E_Object* pE;
     pE=GetElement(pThis->iLabel);
     if (pE!=NULL)
       pList->AddEx(pE);
  }
  else if ((iType == 3) && (pThis->iObjType == 329) && (pThis->pParent == this))
  {
    //Node related to element pThis
    E_Object* pE;
    pE = GetElement(pThis->iLabel);
    if (pE != NULL)
      pList->AddEx(pE);
  }
  else if ((iType==3) &&  (pThis->iObjType==1)  &&  (pThis->pParent==this))
  {
     //Element related to Node pThis
     for (i=0;i<iElNo;i++)
     {
       for (j=0;j<pElems[i]->iNoNodes;j++)
       {
         if (pElems[i]->GetNode(j)==pThis)
         {
             pList->AddEx(pElems[i]);
         }
       }
     }
  }
  else if ((iType==12) &&  (pThis->iObjType==1)  &&  (pThis->pParent==this))
  {
     //Coord related to Node pThis
     G_Object* pC;
	 Pt_Object* pN;
	 pN=(Pt_Object*) pThis;
	 if (pN->DefSys>0)
	 {
       pC=GetSys(pN->DefSys);
       if (pC!=NULL)
	   {
         pList->AddEx(pC);
	   }
	 }
	 if (pN->OutSys>0)
	 {
       pC=GetSys(pN->OutSys);
       if (pC!=NULL)
	   {
         pList->AddEx(pC);
	   }
     }
  }
}
}
}

void ME_Object::Serialize(CArchive& ar,int iV)
{
  int i;
  int iE;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar<<sName;
	  ar<<bDrawN;
	  ar<<TransMat.m_00; ar<<TransMat.m_01; ar<<TransMat.m_02; ar<<TransMat.m_03;
	  ar<<TransMat.m_10; ar<<TransMat.m_11; ar<<TransMat.m_12; ar<<TransMat.m_13;
	  ar<<TransMat.m_20; ar<<TransMat.m_21; ar<<TransMat.m_22; ar<<TransMat.m_23;
	  ar<<TransMat.m_30; ar<<TransMat.m_31; ar<<TransMat.m_32; ar<<TransMat.m_33;
    ar<<iCYS;
	  for (i=0;i<iCYS;i++)
    {
      pSys[i]->Serialize(ar,iV);
    }
    ar<<iNdNo;
    for (i=0;i<iNdNo;i++)
    {
      pNodes[i]->Serialize(ar,iV);
    }
    ar << iElNo;
    for (i=0;i<iElNo;i++)
    {
      ar<<pElems[i]->iType;
      pElems[i]->Serialize(ar,iV,this);
    }
	  // LOAD AND BOUNDARY SETS
    ar<<iCurLC;
    ar<<iNoLCs;
	  for (i=0;i<iNoLCs;i++)
    {
      LCS[i]->Serialize(ar,iV,this);
	  }
    ar<<iCurBC;
    ar<<iNoBCs;
	  for (i=0;i<iNoBCs;i++)
    {
      BCS[i]->Serialize(ar,iV,this);
	  }
    ar<<iCurTSet;
    ar<<iNoTSets;
	  for (i=0;i<iNoTSets;i++)
    {
      TSETS[i]->Serialize(ar,iV,this);
	  }
    pSOLS->Serialize(ar,iV);
	}
	else
	{
    G_Object::Serialize(ar,iV);
    ar>>sName;
	  ar>>bDrawN;
    ar>>TransMat.m_00; ar>>TransMat.m_01; ar>>TransMat.m_02; ar>>TransMat.m_03;
	  ar>>TransMat.m_10; ar>>TransMat.m_11; ar>>TransMat.m_12; ar>>TransMat.m_13;
	  ar>>TransMat.m_20; ar>>TransMat.m_21; ar>>TransMat.m_22; ar>>TransMat.m_23;
	  ar>>TransMat.m_30; ar>>TransMat.m_31; ar>>TransMat.m_32; ar>>TransMat.m_33;
    ar>>iCYS;
	  for (i=0;i<iCYS;i++)
    {
      pSys[i]= new CoordSys;
      pSys[i]->Serialize(ar,iV);
      pSys[i]->pParent=this;
    }
	  ar>>iNdNo;
    if (iNdNo>5000)
      {TempList=new ObjTempList();}
    for (i=0;i<iNdNo;i++)
    {
      pNodes[i]= new Pt_Object;
      pNodes[i]->Serialize(ar,iV);
      pNodes[i]->pParent=this;
      if (TempList!=NULL)
        {TempList->Objs[pNodes[i]->iLabel]=pNodes[i];}
    }
    ar >> iElNo;
    for (i=0;i<iElNo;i++)
    {
      ar>>iE;
      switch(iE) 
      {
		  case 161:
            pElems[i] = new E_Object1;
            break;
		  case 21:
            pElems[i] = new E_Object2B;
            break;
          case 11:
            pElems[i] = new E_Object2R;
            break;
          case 22:
            pElems[i] = new E_Object2B;
            break;
          case 121:
            pElems[i] = new E_ObjectR2;
            break;
		  case 136:
            pElems[i] = new E_Object2;
            break;
		  case 137:
            pElems[i] = new E_Object2;
            break;
      case 91:
            pElems[i] = new E_Object3;
            break;
      case 94:
            pElems[i] = new E_Object4;
            break;
      case 115:
            pElems[i] = new E_Object38;
            break;
      case 111:
            pElems[i] = new E_Object34;
            break;
      case 112:
            pElems[i] = new E_Object36;
            break;
      case 122:
            pElems[i] = new E_ObjectR;
            break;
      case 1000:
            pElems[i] = new E_CellS;
            break;
      }
    pElems[i]->Serialize(ar,iV,this);
	  pElems[i]->pParent=this;
    }
	  // LOAD AND BOUNDARY SETS
    ar>>iCurLC;
    ar>>iNoLCs;
	  for (i=0;i<iNoLCs;i++)
    {
		  LCS[i]=new cLinkedList();
      LCS[i]->pParent=this;
      LCS[i]->Serialize(ar,iV,this);
	   }
     ar>>iCurBC;
     ar>>iNoBCs;
	   for (i=0;i<iNoBCs;i++)
     {
       BCS[i]=new cLinkedListB();
       BCS[i]->pParent=this;
       BCS[i]->Serialize(ar,iV,this);
	   }
     ar>>iCurTSet;
     ar>>iNoTSets;
	   for (i=0;i<iNoTSets;i++)
     {
       TSETS[i]=new cLinkedListT();
       TSETS[i]->pParent=this;
       TSETS[i]->Serialize(ar,iV,this);
	   }
    //NEW SOLUTION SEQUENCES
    pSOLS->Serialize(ar,iV);
	  }
    if (TempList!=NULL)
    {
      delete(TempList);
      TempList = NULL;
    }
    this->MaxLab();
}




G_Object* ME_Object::Copy(G_Object* Parrent)
{
int i;
int iE;
ME_Object* rME = new ME_Object;
rME->Drawn = Drawn;
rME->Selectable  = Selectable; 
rME->Visable  = Visable;
rME->iColour = iColour;
rME->iObjType = iObjType;
rME->iLabel = iLabel;
rME->sName = sName;
rME->iNdNo=iNdNo;
rME->bDrawN=bDrawN;
rME->pParent = NULL;

//rME->iNoMats=iNoMats;
//for (i=0;i<iNoMats;i++)
//{
//  rME->pMats[i]=pMats[i]->Copy();
//}
for (i=0;i<iNdNo;i++)
{
  rME->pNodes[i]= new Pt_Object;
  rME->pNodes[i]= (Pt_Object*) pNodes[i]->Copy(rME);
}

rME->iElNo=iElNo;
for (i=0;i<iElNo;i++)
{
  iE=pElems[i]->iType;
  switch(iE) 
  {
	case 161:
      rME->pElems[i] = (E_Object1*) pElems[i]->Copy(rME);
      break;
	case 21:
      rME->pElems[i] = (E_Object2*) pElems[i]->Copy(rME);
      break;
	case 22:
      rME->pElems[i] = (E_Object2*) pElems[i]->Copy(rME);
      break;
    case 121:
      rME->pElems[i] = (E_Object2*) pElems[i]->Copy(rME);
      break;
    case 136:
      rME->pElems[i] = (E_Object2*) pElems[i]->Copy(rME);
      break;
    case 137:
      rME->pElems[i] = (E_Object2*) pElems[i]->Copy(rME);
      break;
    case 91:
      rME->pElems[i] = (E_Object3*) pElems[i]->Copy(rME);
      break;
    case 94:
      rME->pElems[i] = (E_Object4*) pElems[i]->Copy(rME);
      break;
    case 115:
      rME->pElems[i] = (E_Object38*) pElems[i]->Copy(rME);
      break;
    case 111:
      rME->pElems[i] = (E_Object34*) pElems[i]->Copy(rME);
      break;
	  case 112:
      rME->pElems[i] = (E_Object36*) pElems[i]->Copy(rME);
      break;
    case 122:
      rME->pElems[i] = (E_ObjectR*) pElems[i]->Copy(rME);
      break;
  }
  rME->pElems[i]->pParent = rME;
}
rME->iCYS=iCYS;
for (i=0;i<iCYS;i++)
{
  rME->pSys[i] = (CoordSys*) pSys[i]->Copy(rME);
}


return (rME);
}

BOOL ME_Object::isFaceDeletable(cFace* inFace)
{
BOOL bRet=TRUE;
cFace* Fc[8];
int iN;
int i,j;
int iC;
iC=0;
for(i=0;i<iElNo;i++)
{
  iN = pElems[i]->GetfaceList(Fc);
  if (iN>0)
  {
    for (j=0;j<iN;j++)
    {
      if (inFace->isSame(Fc[j])==TRUE)
	  {
        iC++;
		delete(Fc[j]);
	  }
    }
	if (iC>1){break;}
  }
}
if (iC>1)
{
  bRet=FALSE;
}
return(bRet);
}

BOOL ME_Object::isLinkDeletable(cLink* inLink)
{
BOOL bRet=TRUE;
cLink* Lk[12];
int iN;
int i,j;
int iC;
iC=0;
for(i=0;i<iElNo;i++)
{
  iN = pElems[i]->GetLinkList(Lk);

  if (iN>0)
  {
    for (j=0;j<iN;j++)
    {
      if (inLink->isSame(Lk[j])==TRUE)
	  {
        iC++;
		delete(Lk[j]);
	  }
    }
	if (iC>1){break;}
  }
}
if (iC>1)
{
  bRet=FALSE;
}
return(bRet);
}

void ME_Object::BuildLinkList()
{
cFace* Fc[8];
cLink* Lk[200];
int iN;
int i,j;
for(i=0;i<iElNo;i++)
{
  iN = pElems[i]->GetfaceList(Fc);
  if (iN>0)
  {
    for (j=0;j<iN;j++)
    {
      FcList->Add(Fc[j]);
    }
  }
  iN = pElems[i]->GetLinkList(Lk);
  if (iN>0)
  {
    for (j=0;j<iN;j++)
    {
       LkList->Add(Lk[j]);
    }
  }
}
outtext1("Finished List Generation.");
}


void ME_Object::Append(ME_Object* pMexh,int iNInc,int iEInc)
{
int i;
int iSNodePos;

iSNodePos=this->iNdNo;


if (pMexh!=NULL)
{
  GPs[iNoGps] = new NEList();
  GPs[iNoGps]->Title = pMexh->sName;
  iNoGps++;
  for (i=0;i<pMexh->iNdNo;i++)
  {
    this->pNodes[iNdNo]= new Pt_Object;
    this->pNodes[iNdNo]= (Pt_Object*) pMexh->pNodes[i]->Copy(this);
    this->pNodes[iNdNo]->iLabel=iNdNo+1+iNInc; 
    GPs[iNoGps-1]->ids[GPs[iNoGps-1]->iNo] = this->pNodes[iNdNo]->iLabel;
    GPs[iNoGps-1]->iType[GPs[iNoGps-1]->iNo] = 1;
    GPs[iNoGps-1]->iNo++;
    iNdNo++;
  }
  int iE;
  for (i=0;i<pMexh->iElNo;i++)
  {
    iE=pMexh->pElems[i]->iType;
    switch(iE) 
    {
	  case 161:
      this->pElems[iElNo] = (E_Object1*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
	  case 21:
      this->pElems[iElNo] = (E_Object2*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 22:
      this->pElems[iElNo] = (E_Object2*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 121:
      this->pElems[iElNo] = (E_Object2*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 136:
      this->pElems[iElNo] = (E_Object2*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 137:
      this->pElems[iElNo] = (E_Object2*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 91:
      this->pElems[iElNo] = (E_Object3*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 94:
      this->pElems[iElNo] = (E_Object4*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 115:
      this->pElems[iElNo] = (E_Object38*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 111:
      this->pElems[iElNo] = (E_Object34*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
	  case 112:
      this->pElems[iElNo] = (E_Object36*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    case 122:
      this->pElems[iElNo] = (E_ObjectR*) pMexh->pElems[i]->CopyAppend(iSNodePos,this,pMexh);
      break;
    }
    this->pElems[iElNo]->iLabel=iElNo+1+iEInc;
    this->pElems[iElNo]->pPr=NULL;
    GPs[iNoGps-1]->ids[GPs[iNoGps-1]->iNo] = this->pElems[iElNo]->iLabel;
    GPs[iNoGps-1]->iType[GPs[iNoGps-1]->iNo] = 3;
    GPs[iNoGps-1]->iNo++;
    iElNo++;
  }
}

}

ME_Object* ME_Object::GetMesh()    //GetPtr to this
{
return (this);
}


void ME_Object::ExportSec(FILE* pFile,int id,CString Name, double w,double h,double t)
{
double Area;
double w1;
double h1;
double Ixx;
double Iyy;
w1=w-2*t;
h1=h-2*t;
Area=w*h-w1*h1;
Ixx=(w*h*h*h)/12-(w1*h1*h1*h1)/12;
Iyy=(h*w*w*w)/12-(h1*w1*w1*w1)/12;

fprintf(pFile,"%10i%10i%10i\n",id,2,0);
fprintf(pFile,"%20s\n",Name);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",w,h,t,t,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,w/2,h/2);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0,0.0,0.0);
fprintf(pFile,"%13.6E%13.6E%13.6E%13.6E\n",0.0,0.0,0.0,0.0);
fprintf(pFile,"%10i%10i%10i%10i%10i%10i\n",6,6,6,6,6,6);
fprintf(pFile,"%10i%10i%10i%10i%13.6E\n",0,45,1,11,1.0);
}

void ME_Object::ExportRes(FILE* pFile)	 
{
int i;
for (i=0;i<iNoRes;i++)
{
  ResultsSets[i]->PrintToFile(pFile);
}
}

void ME_Object::ExportNASExec(FILE* pFile, SecTable* pS)
{
	int iC = pSOLS->iCur;
	if (iC!=-1)
	{
		if (pSOLS->pSols[iC]->iType == 0)
		{
			//SOL 101
			fprintf(pFile, "%s\n", "SOL 101");
			fprintf(pFile, "%s\n", "CEND");
			ExportNASCase101(pFile, pS);
		}
		else if (pSOLS->pSols[iC]->iType == 1)
		{
			//SOL STEADY STATE HEAT
			fprintf(pFile, "%s\n", "$ERROR NO EXECUTIVE STATEMENT AVAILABLE FOR STEADY STATE HEAT TRANSFUR");
		}
		else if (pSOLS->pSols[iC]->iType == 2)
		{
			fprintf(pFile, "%s\n", "SOL 103");
			fprintf(pFile, "%s\n", "CEND");
			fprintf(pFile, "%s\n", "$ERROR CASE CONTROL NOT IMPLEMENTED YET");
		}
	}
}

void ME_Object::ExportNASCase101(FILE* pFile, SecTable* pS)
{
int i = 0;
int iNoSteps = 0;
int iC = pSOLS->iCur;
fprintf(pFile, "%s\n", "$******************* CASE CONTROL *************************");
fprintf(pFile, "%s\n", "$");
fprintf(pFile, "TITLE = %s\n", pSOLS->pSols[iC]->sTitle);
fprintf(pFile, "%s\n", "ECHO = NONE");
Solution* pSOL = pSOLS->pSols[iC];
iNoSteps = pSOL->iNo;
for (i = 0; i < iNoSteps; i++)
{
	fprintf(pFile, "SUBCASE %i\n", i + 1);
	fprintf(pFile, "    TITLE = %s\n", pSOL->sStepTitle[i]);
	if (pSOL->LS[i] > 0)
	{
		fprintf(pFile, "    LOAD = %i\n", pSOL->LS[i]);
	}
	if (pSOL->BS[i] > 0)
	{
		fprintf(pFile, "    SPC  = %i\n", pSOL->BS[i]);
	}
	if (pSOL->TS[i] > 0)
	{
		fprintf(pFile, "    TEMP = %i\n", pSOL->TS[i]);
	}
	fprintf(pFile, "    %s\n", "DISPLACEMENT(PRINT, PLOT) = ALL");
	fprintf(pFile, "    %s\n", "FORCE(PRINT, PLOT) = ALL");
	fprintf(pFile, "    %s\n", "STRESS(PRINT, PLOT, CENTER) = ALL");
	fprintf(pFile, "    %s\n", "OLOAD(PRINT) = ALL");
}
}

void ME_Object::ExportNAS(FILE* pFile,SecTable* pS)	 
{
int i;
fprintf(pFile,"%s\n","$***************COORDINATE SYSTEMS*************************");
for (i=0;i<iCYS;i++)
{
	pSys[i]->ExportNAS(pFile);
}

fprintf(pFile,"%s\n","$********************NODES*********************************");
for (i=0;i<iNdNo;i++)
{
  CoordSys* pDef = GetSys(pNodes[i]->DefSys);
	pNodes[i]->ExportNAS(pFile,pDef);
}
fprintf(pFile,"%s\n","$*******************ELEMENTS******************************");
for (i=0;i<iElNo;i++)
{
	pElems[i]->ExportNAS(pFile);
}
fprintf(pFile, "%s\n", "$********************LOADING*******************************");
for (i=0;i<iNoLCs;i++)
{
  cLinkedList* pCLC = LCS[i];
  fprintf(pFile, "$%s\n", pCLC->sTitle);
  if (pCLC!=NULL)
  {
    BCLD* pNext;
    pNext = (BCLD*) pCLC->Head;
    while (pNext != NULL)
    {
      pNext->ExportNAS(pFile);	
	  pNext = (BCLD*) pNext->next;
    }
  }
}
fprintf(pFile, "%s\n", "$********************RESTRAINTS*******************************");
for (i = 0; i < iNoBCs; i++)
{
	cLinkedList* pCBC = BCS[i];
	fprintf(pFile, "$%s\n", pCBC->sTitle);
	if (pCBC != NULL)
	{
		BCLD* pNext;
		pNext = (BCLD*)pCBC->Head;
		while (pNext != NULL)
		{
			pNext->ExportNAS(pFile);
			pNext = (BCLD*)pNext->next;
		}
	}
}

}

ResSet* ME_Object::GetResultsSet(int iSet)
{
	ResSet* pRS = NULL;
	if ((iSet < iNoRes) && (iSet > -1))
	{
		pRS = ResultsSets[iSet];
	}
	return (pRS);
}

void ME_Object::SetColourBar(float fMax,float fMin)
{
SetColBar(fMin,fMax);
}

void ME_Object::NullResults()
{
int i;
for (i = 0; i < iNdNo; i++)
{
  pNodes[i]->pResV=NULL;
}
for (i = 0; i < iElNo; i++)
{
  pElems[i]->pResV=NULL;
}
}

void ME_Object::DeleteAllResults()
{
int i;
NullResults();
for(i=0;i<iNoRes;i++)
{
  delete(ResultsSets[i]);
}

iNoRes=0;
iCurResSet=-1;
iResVal=-1;
iCurResSetDef=-1;
iResValDef=-1;
CResSet=NULL;
CResSetDef=NULL;
iCVar=-1;
iCVarDef=-1;
iPostOpt=0;
}



void ME_Object::PostElResScalar(ResSet* pRes,int iVar,int iOpt,float &fMax,float &fMin)
{
int i;
fMax=0;
fMin=0;
BOOL bFirst=TRUE;
if (iOpt==0)
  iOpt=pRes->iDefID;  
if ((pRes!=CResSet) || (iOpt!=iPostOpt))
{
  NullResults();
  iPostOpt=iOpt;
  if (iElNo > 0) 
  {
    if ((pRes->TCODE==4) || (pRes->TCODE==5) || (pRes->TCODE==25) || (pRes->TCODE == 18))
    {
    for (i = 0; i < iElNo; i++)
    {
        Res* pR=pRes->Get(pElems[i]->iLabel,iOpt);
        if (pR!=NULL)
        {
          pElems[i]->pResV=pR;
        }  
    }
	}
  }

  if (iNdNo > 0) 
  {
    if ((pRes->TCODE==1) || (pRes->TCODE==7) || (pRes->TCODE == 1011))
    {
      for (i = 0; i < iNdNo; i++)
      {
        if (pRes->iNoV==6)
        {
          Res6* pR=(Res6*) pRes->Get(pNodes[i]->iLabel,0);
          if (pR!=NULL)
          {
            pNodes[i]->pResV=pR;
          }
        }
        else if (pRes->iNoV==1)
        {
          Res1* pR=(Res1*) pRes->Get(pNodes[i]->iLabel,0);
          if (pR!=NULL)
          {
            pNodes[i]->pResV=pR;
          }
        }
		else if (pRes->iNoV == 13)
		{
			Res13* pR = (Res13*)pRes->Get(pNodes[i]->iLabel, 0);
			if (pR != NULL)
			{
				pNodes[i]->pResV = pR;
			}
		}
      }
    }
  }
}
float fResVal;
//Calculate Max Min
bFirst = TRUE;
  if (iNdNo > 0) 
  {
    if ((pRes->TCODE==1)||(pRes->TCODE==7) || (pRes->TCODE == 1011))
    {
      for (i = 0; i < iNdNo; i++)
      {
        if (pNodes[i]->pResV!=NULL)
        {
          fResVal=*pNodes[i]->pResV->GetAddress(iVar);
		      if (bFirst)
		      {
                  fMax=fResVal; 
			      fMin=fResVal;
			      bFirst=FALSE;
		      }
		      else
		      {
                if (fResVal>fMax){fMax=fResVal;}
                if (fResVal<fMin){fMin=fResVal;}
		       }
         }
       }
    }
  }
  if (iElNo > 0) 
  {
	if((pRes->TCODE==4) || (pRes->TCODE==5) || (pRes->TCODE==25) || (pRes->TCODE == 18))
    {
    for (i = 0; i < iElNo; i++)
    {
      if (pElems[i]->pResV!=NULL)
      {
         fResVal=*pElems[i]->pResV->GetAddress(iVar);
		 if (bFirst)
		 {
            fMax=fResVal; fMin=fResVal;
			bFirst=FALSE;
		 }
		 else
		 {
           if (fResVal>fMax){fMax=fResVal;}
           if (fResVal<fMin){fMin=fResVal;}
		 }
      }
    }
	}
  }
  pRes->fMaxV = fMax;
  pRes->fMinV = fMin;
  CResSet = pRes;
  iCVar = iVar;
  if (!bUserColBar)
	  SetColBar(fMin, fMax);
}

void ME_Object::PostElResDef(ResSet* pRes, int iVar, float &fMax, float &fMin)
{
	int i;
	fMax = (float)-1e+20;
	fMin = (float) 1e+20;

	if (pRes != CResSetDef)
	{
		for (i = 0; i < iNdNo; i++)
		{
			pNodes[i]->pResD = NULL;
		}
		if (iNdNo > 0)
		{
			if ((pRes->TCODE == 1) || (pRes->TCODE == 7))
			{
				for (i = 0; i < iNdNo; i++)
				{
					if (pRes->iNoV == 6)
					{
						Res6* pR = (Res6*)pRes->Get(pNodes[i]->iLabel, 0);
						if (pR != NULL)
						{
							pNodes[i]->pResD = pR;
						}
					}
				}
			}
		}
	}

	CResSetDef = pRes;
	iCVarDef = iVar;
	BuildDeromedVecs();
}


void ME_Object::DeleteResVectors()
{
	if (pResVectors != NULL)
	{
		pResVectors->DeleteAll();
		delete (pResVectors);
		pResVectors = NULL;
	}
	cBarMax = -1e+20;
	cBarMin = 1e+20;
}

// 051019
void ME_Object::GenResVectors(int iSet, int iVec, int iDf)
{
	int i;
	int iD = -1;
	int iS;
	int iEID;
	C3dMatrix mTMat;
	float fMax = (float)-1e+20;
	float fMin = (float) 1e+20;
	Res* pR;
	C3dVector vVec;
	C3dVector vC;
	BOOL isGood = FALSE;
	ResSet* pRes;
	ResDef* pDef;
	ResultsVec* pDspVec;
	pRes = this->GetResultsSet(iSet);
	if (pRes != NULL)
	{
		pDef = pRes->GetResDef(iVec);
		if (pDef != NULL)
			isGood = TRUE;
	}
	if (isGood)
	{
		if (pResVectors == NULL)
			pResVectors = new cLinkedList();
		//Vector is on node and is a vector
		if ((pDef->iLoc == 0) && (pDef->iResType == 1))
		{
			for (i = 0; i < iNdNo; i++)
			{
				pR = pRes->Get(pNodes[i]->iLabel, 0);
				if (pR != NULL)
				{
					vC = pNodes[i]->Get_Centroid();
					iD = pNodes[i]->iLabel;
					vVec.x = 0; vVec.y = 0; vVec.z = 0;
					iS = 1;
					if (iDf == 3)
					{
						vVec.x = *pR->GetAddress(pDef->iComponents[0]);
						vVec.y = *pR->GetAddress(pDef->iComponents[1]);
						vVec.z = *pR->GetAddress(pDef->iComponents[2]);
						iS = 1; //Sign is magnitude
					}
					else if (iDf == 0)
					{
						vVec.x = *pR->GetAddress(pDef->iComponents[0]);
						if (vVec.x < 0)
							iS = -1;
					}
					else if (iDf == 1)
					{
						vVec.y = *pR->GetAddress(pDef->iComponents[1]);
						if (vVec.y < 0)
							iS = -1;
					}
				    else if (iDf == 2)
				    {
					   vVec.z = *pR->GetAddress(pDef->iComponents[2]);
					   if (vVec.z < 0)
						   iS = -1;
				    }

					double dM;
					    dM = iS*vVec.Mag();
						if (dM > fMax)
							fMax = dM;
						if (dM < fMin)
							fMin = dM;
						pDspVec = new ResultsVec(iD,vC, vVec, 4, iDf,iS, pDef->iResType);
						pDspVec->pParent = this;
						pResVectors->Add(pDspVec);
				}
				
			}
			SetColBarVec(fMin, fMax);
		}
		else if ((pDef->iLoc == 1) && (pDef->iResType == 4)) //3d Tensor
		{ //Its a 3d Tensor at element centroid
			for (i = 0; i < iElNo; i++)
			{
				iS = 1;
				pR = pRes->Get(pElems[i]->iLabel, 0);
				if (pR != NULL)
				{
					iEID = pElems[i]->iLabel;
					vC = pElems[i]->Get_Centroid();
					iD = pElems[i]->iLabel;
					vVec.x = 0; vVec.y = 0; vVec.z = 0;
					//vVec.x = *pR->GetAddress(pDef->iComponents[0]);  //Sxx
					if (iDf == 0)  //X stress
					{
						vVec.x = *pR->GetAddress(pDef->iComponents[0]);
						if (vVec.x < 0)
							iS = -1;
					}
					else if (iDf == 1)  //Y stress
					{
						vVec.y = *pR->GetAddress(pDef->iComponents[1]);
						if (vVec.y < 0)
							iS = -1;
					}
					else if (iDf == 2)  //z stress
					{
						vVec.z = *pR->GetAddress(pDef->iComponents[2]);
						if (vVec.z < 0)
							iS = -1;
					}
					else if (iDf == 3)  //XY stress
					{
						vVec.y = *pR->GetAddress(pDef->iComponents[3]);
						if (vVec.y < 0)
							iS = -1;
					}
					else if (iDf == 4)  //yz stress
					{
						vVec.z = *pR->GetAddress(pDef->iComponents[4]);
						if (vVec.z < 0)
							iS = -1;
					}
					else if (iDf == 5)  //zx stress
					{
						vVec.x = *pR->GetAddress(pDef->iComponents[5]);
						if (vVec.x < 0)
							iS = -1;
					}
					else  //Its a principal 6,7,8
					{
						C3dVector vX, vY, vZ;
						vX.x = *pR->GetAddress(pDef->iComponents[0]);
						vX.y = *pR->GetAddress(pDef->iComponents[3]);
						vX.z = *pR->GetAddress(pDef->iComponents[5]);
						vY.x = *pR->GetAddress(pDef->iComponents[3]);
						vY.y = *pR->GetAddress(pDef->iComponents[1]);
						vY.z = *pR->GetAddress(pDef->iComponents[4]);
						vZ.x = *pR->GetAddress(pDef->iComponents[5]);
						vZ.y = *pR->GetAddress(pDef->iComponents[4]);
						vZ.z = *pR->GetAddress(pDef->iComponents[2]);
						double p1;
						p1 = *pR->GetAddress(pDef->iComponents[iDf]);
						vVec = EigenVector3d(iEID,vX, vY, vZ, p1);
						iS = 1;
						if (p1 < 0)
							iS = -1;
				}
				double dM;
				dM = iS*vVec.Mag();
				if (dM > fMax)
					fMax = dM;
				if (dM < fMin)
					fMin = dM;
				pDspVec = new ResultsVec(iD, vC, vVec, 4, iDf,iS, pDef->iResType);
				pDspVec->pParent = this;
				pResVectors->Add(pDspVec);
				}

			}
			SetColBarVec(fMin, fMax);
		}
		else if ((pDef->iLoc == 1) && (pDef->iResType == 3)) //2d Tensor
		{ //Its a 2d Tensor at element centroid
		for (i = 0; i < iElNo; i++)
		{
			iS = 1;
			pR = pRes->Get(pElems[i]->iLabel, 0);
			if (pR != NULL)
			{
				iEID = pElems[i]->iLabel;
				mTMat = pElems[i]->GetElSys();
				vC = pElems[i]->Get_Centroid();
				iD = pElems[i]->iLabel;
				vVec.x = 0; vVec.y = 0; vVec.z = 0;
				//vVec.x = *pR->GetAddress(pDef->iComponents[0]);  //Sxx
				if (iDf == 0)  //X stress
				{
					vVec.x = *pR->GetAddress(pDef->iComponents[0]);
					if (vVec.x < 0)
						iS = -1;
				}
				else if (iDf == 1)  //Y stress
				{
					vVec.y = *pR->GetAddress(pDef->iComponents[1]);
					if (vVec.y < 0)
						iS = -1;
				}
				else if (iDf == 2)  //XY stress
				{
					vVec.y = *pR->GetAddress(pDef->iComponents[2]);
					if (vVec.y < 0)
						iS = -1;
				}
				else  //Its a principal 3,4
				{
					C3dVector vX, vY, vZ;
					vX.x = *pR->GetAddress(pDef->iComponents[0]);
					vX.y = *pR->GetAddress(pDef->iComponents[2]);
					vX.z = 0;
					vY.x = *pR->GetAddress(pDef->iComponents[3]);
					vY.y = *pR->GetAddress(pDef->iComponents[1]);
					vY.z = 0;
					double p1;
					p1 = *pR->GetAddress(pDef->iComponents[iDf]);
					vVec = EigenVector2d(iEID,vX, vY, p1);
					iS = 1;
					if (p1 < 0)
						iS = -1;
				}
				mTMat.Transpose();
				vVec = mTMat.Mult(vVec);
				double dM;
				dM = iS * vVec.Mag();
				if (dM > fMax)
					fMax = dM;
				if (dM < fMin)
					fMin = dM;
				pDspVec = new ResultsVec(iD, vC, vVec, 4, iDf, iS, pDef->iResType);
				pDspVec->pParent = this;
				pResVectors->Add(pDspVec);
			}

		}
		SetColBarVec(fMin, fMax);
		}
		

	}

}

void ME_Object::BuildDeromedVecs()
{
int i;
if (CResSetDef!=NULL)
{
  if (iNdNo > 0) 
  {
    if ((CResSetDef->TCODE==1) || (CResSetDef->TCODE==7))
    {
      for (i = 0; i < iNdNo; i++)
      {
        Res* pR=pNodes[i]->pResD;
        if (pR!=NULL)
        {
           if (pR->pVec!=NULL)
           {
             delete(pR->pVec);
             pR->pVec=NULL;
           }
             pR->pVec=new C3dVector();

             pR->pVec->x=*pR->GetAddress(0);
             pR->pVec->y=*pR->GetAddress(1);
             pR->pVec->z=*pR->GetAddress(2);
             if (iResValDef==0)
             {
               pR->pVec->y=0;
               pR->pVec->z=0;
             }
             if (iResValDef==1)
             {
               pR->pVec->x=0;
               pR->pVec->z=0;
             }
             if (iResValDef==2)
             {
               pR->pVec->x=0;
               pR->pVec->y=0;
             }
        }
      }
    }
  }
}
}

void ME_Object::DeleteDeromedVecs()
{
    int i;
    for (i = 0; i < iNdNo; i++)
    {
      Res* pR=pNodes[i]->pResD;
      if (pR!=NULL)
      {
         if (pR->pVec!=NULL)
         {
           delete(pR->pVec);
           pR->pVec=NULL;
         }
      }
    }
}

void ME_Object::ExportGroups(FILE* pFile)	 
{
int i;
int j;
E_Object* pE;
Pt_Object* pN;
 fprintf(pFile,"%-10s%-10s%-10s%-10s%-10s\n","ELEM","ID","COL","PID","TYPE");
 fprintf(pFile,"%-10s%-10s%-10s%-10s%-10s\n","NODE","ID","COL","DEF","OUT");
for (i=0;i<iNoGps;i++)
{
  fprintf(pFile,"%-s\n","GROUP");
  fprintf(pFile,"%-s\n",GPs[i]->Title);
  fprintf(pFile,"%-i\n",GPs[i]->iNo);
  for (j=0;j<GPs[i]->iNo;j++)
  {
    if (GPs[i]->iType[j]==3)
    {
      pE=this->GetElement(GPs[i]->ids[j]);
      if (pE!=NULL)
	    fprintf(pFile,"%-10s%-10i%-10i%-10i%-10i\n","ELEM",GPs[i]->ids[j],pE->iColour,pE->PID,pE->iType);
    }
    if (GPs[i]->iType[j]==1)
    {
      pN=this->GetNode(GPs[i]->ids[j]);
      if (pN!=NULL)
	    fprintf(pFile,"%-10s%-10i%-10i%-10i%-10i\n","NODE",GPs[i]->ids[j],pN->iColour,pN->DefSys,pN->OutSys);
    }
  }
}
}



void ME_Object::ExportUNV(FILE* pFile,SecTable* pS)	 
{
int i;
int j;
COleDateTime timeStart;
timeStart = COleDateTime::GetCurrentTime();

int year=timeStart.GetYear();
int Mon=timeStart.GetMonth();
if (year<2020)
{
BOOL bNL;
//Dataset 164 Units
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","164");
fprintf(pFile,"%10i%20s%10i\n",1,"Meter (newton)               ",2);
fprintf(pFile,"%25.16E%25.16E%25.16E\n",1.0,1.0,1.0);
fprintf(pFile,"%25.16E\n",1.0);
fprintf(pFile,"%6s\n","-1");
if (pS!=NULL)
{
  pS->ExportSecs(pFile);
}
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","2411");
for (i=0;i<iNdNo;i++)
{
	pNodes[i]->ExportUNV(pFile);
}
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","2412");
for (i=0;i<iElNo;i++)
{
	pElems[i]->ExportUNV(pFile);
}
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","2438");
for (i=0;i<iElNo;i++)
{
  if (pElems[i]->iType==21)
  {
    E_Object2B* E = (E_Object2B*) pElems[i];
	  E->ExportUPVecs(pFile);
  }
}
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","-1");
fprintf(pFile,"%6s\n","2435");
for (i=0;i<iNoGps;i++)
{
  fprintf(pFile,"%10i%10i%10i%10i%10i%10i%10i%10i\n",i+1,0,0,0,0,0,0,GPs[i]->iNo);
  fprintf(pFile,"%s\n",GPs[i]->Title);
  bNL=FALSE;
  for (j=0;j<GPs[i]->iNo;j++)
  {
  if (GPs[i]->iType[j]==3)
  {
	  fprintf(pFile,"%10i%10i%10i%10i",8,GPs[i]->ids[j],0,0);
  }
  else if (GPs[i]->iType[j]==1)
  {
	  fprintf(pFile,"%10i%10i%10i%10i",7,GPs[i]->ids[j],0,0);
  }
	if (bNL==FALSE)
	{
      bNL=TRUE;
	}
    else
	{
      bNL=FALSE;
      fprintf(pFile,"\n","");
	}

  }
  if (bNL==TRUE)
  {
      fprintf(pFile,"\n","");
  }
}
fprintf(pFile,"%6s\n","-1");
}
}

int ME_Object::GetNoNode(int iType)
{
int iRC=-1;
switch(iType) 
{
case 115: iRC = 8; //115 //BRICK 8
	break;
case 112: iRC = 6; //WEDGE 6 
    break;
case 111: iRC = 4; //TET 4
	break;
case 94 : iRC = 4; //QUAD 4
    break;
case 91: iRC = 3;  //TRI 3
    break;
case 161: iRC = 1; //SCALAR
    break;
case 121: iRC = 2; //UNKNOWN 2
    break;
case 136: iRC = 2; //UNKNOWN 2
    break;
case 137: iRC = 2; //UNKNOWN 2
    break;
case 21: iRC = 2;  //BEAM 2
    break;
case 11: iRC = 2;  //ROD 2
    break;
case 22: iRC = 2;    //UNKNOWN 2
    break;
case 122: iRC = 200; //RIGID 200
    break;
case 1000: iRC = 4; //STAGGERED CELL
    break;
}
return (iRC);
}





void ME_Object::Test(PropTable* PropsT,MatTable* MatT)
{
int i,j,neq;
COleDateTime timeStart;
timeStart = COleDateTime::GetCurrentTime();
int Hour=timeStart.GetHour();
int Min=timeStart.GetMinute();
int Sec=timeStart.GetSecond();
char s1[80];
int iLC=-1;
CString sSol("");
CString sStep("");


sprintf_s(s1,"%s %i:%i:%i\n","START TIME",Hour,Min,Sec);
outtext1(s1);
for(i=0;i<iNdNo;i++)
{
  for (j=0;j<6;j++)
  {
    pNodes[i]->dof[j]=0;
  }
}



Mat KME;
//Mat dee=DeeMat(100,0.29,6);
this->ApplyRes(NULL);
neq=GenDofs();
if (neq==0)
{
  outtext1("ERROR: No Degrees of Freedoms Exist.");
  outtext1("Ensure the Correct F.E. Model is Active.");
}
else
{
  Vec<double> FVec;
  Vec<int> Steer;
  FVec = GetForceVec(NULL,neq);
  GetPressureLoads(NULL,neq,FVec);
  int iBW=this->MaxBW();

  Vec <double> KM(neq*(iBW+1));
  LocalRes(neq,Steer,KM);
  outtext1("STARTING ASSY");
  E_Object* pE;


  for (i=0;i<iElNo;i++)
  {
    pE=pElems[i];
    
    if (pE->ChkNegJac()==FALSE)
    {
      KME=pE->GetStiffMat(PropsT,MatT);
      Steer=pE->GetSteerVec3d();
    }
    else
    {

      pE=(E_Object*) pElems[i]->Copy(this);
      pE->Reverse();
      KME=pE->GetStiffMat(PropsT,MatT);
      Steer=pE->GetSteerVec3d();
      delete(pE);
      pE=NULL;
    }

    formkv(KM,KME,Steer,neq);
    //KME.diag();
    KME.clear();
  }

  //KM.diag();
  outtext1("STARTING BAND REDUCTION");
  banred(KM,neq);
  outtext1("STARTING BACK SUBSTITUTION");
  bacsub(KM,FVec);
  outtext1("FINISHED SOLUTION");
  Displacements(iLC,sSol,sStep,Steer,FVec);
  StressesBeam(iLC, sSol, sStep, PropsT,MatT,Steer,FVec);
  Stresses2d(iLC, sSol, sStep, PropsT,MatT,Steer,FVec);
  Stresses3d(iLC, sSol, sStep, PropsT,MatT,Steer,FVec);
  KM.DeleteAll();
  Steer.DeleteAll();
}
timeStart = COleDateTime::GetCurrentTime();
Hour=timeStart.GetHour();
Min=timeStart.GetMinute();
Sec=timeStart.GetSecond();
sprintf_s(s1,"%s %i:%i:%i\n","END TIME",Hour,Min,Sec);
outtext1(s1);
}

Mat GetG(Vec<double> &AA,Vec<int> &G)
{
int iS;
int i;
iS=G.n;

Mat pMult(iS,1);
for (i=1;i<=iS;i++)
{
  if (*G.nn(i)>0)
  {
    int ii;
    ii=*G.nn(i);
    double pM=*AA.nn(ii);
    *pMult.mn(i,1)=*AA.nn(ii);
  }
}        
return (pMult);
}

double IterConv(Vec<double> &X,Vec<double> &XNew)
{
int iS;
int i;
double dD=0;
double dDa=0;
double MaxDiff=0;
double MaxDiff1=0;
iS=X.n;
for (i=0;i<iS;i++)
{
  double a=XNew(i);
  double b=X(i);
  dD=abs(XNew(i)-X(i));
  dDa=abs(XNew(i));
  if (dD>MaxDiff) MaxDiff=dD;
  if (dDa>MaxDiff1) MaxDiff1=dDa;
}
return(MaxDiff/MaxDiff1);
}



double DotProd(Vec<double> &AA,Vec<double> &BB)
{
int iS;
int i;
double dP=0;
iS=AA.n;
for (i=0;i<iS;i++)
{
double f=AA(i);
double p=BB(i);
dP+=AA(i)*BB(i);
}
return(dP);
}

void PutG(Vec<double> &AA,Vec<int> &G,Mat& M)
{
int iS;
int i;
iS=G.n;
int ii;

for (i=1;i<=iS;i++)
{

  ii=*G.nn(i);
  if (ii>0)
  {
    double mmm=*M.mn(i,1);
    *AA.nn(ii)+=*M.mn(i,1);
  }
}        

}

void ME_Object::ZeroDOF()
{
  int i;
  int j;
  for(i=0;i<iNdNo;i++)
  {
    for (j=0;j<6;j++)
    {
      pNodes[i]->dof[j]=0;
    }
  }
}

BOOL ME_Object::GetStepCasesLinStat(int &iStep,CString &sSol, CString &sStep, double &dTol,cLinkedList* &pLC,cLinkedListB* &pBC,cLinkedListT* &pTC,BOOL &bRS)
{
BOOL bRet=FALSE;
Solution* pCSol;
pLC=NULL;
pBC=NULL;
pTC=NULL;
bRS=TRUE;
int iLC;
int iBC;
int iTC;
char S1[200];
sSol="NULL";
sStep="NULL";
if (pSOLS->iCur!=-1)
{
  sprintf_s(S1,"SOLUTION: %s",pSOLS->pSols[pSOLS->iCur]->sTitle);
  outtext1(S1);
  pCSol=pSOLS->pSols[pSOLS->iCur];

  dTol=pCSol->dTol;                       //solution tolerence
  if ((pCSol->iType==0) || (pCSol->iType==1))
  {
    iStep=pCSol->GetCurStep();
    if ((iStep<pCSol->iNo) && (iStep!=-1))
    {

      iLC=pCSol->LS[iStep];
      iBC=pCSol->BS[iStep];
      iTC=pCSol->TS[iStep];
      bRS=pCSol->RS[iStep];
      sSol = pCSol->GetSolutionTitleString();
      sStep = pCSol->GetStepTitleString(iStep);
      bRet=TRUE;
    }
    if (iLC!=-1)
      pLC=GetLC(iLC);
    if (iBC!=-1)
      pBC=GetBC(iBC);
    if (iTC!=-1)
      pTC=GetTSET(iTC);
  }
  else
  {
     outtext1("ERROR: Only Linear Static Available.");
  }
}


return (bRet);
}


void ME_Object::IterSol3dLin(PropTable* PropsT,MatTable* MatT)
{
CString sSol;
CString sStep;
int i,j,neq;
char s1[80];
double dErr;
int iELCnt;
int m;
double up;
double beta;
double alpha;
int aaa=0;
BOOL bGo=FALSE;
Mat KME; 
Vec<double> FVec;
Vec<int> Steer;
int iStep;
cLinkedList* pLC=NULL;
cLinkedListB* pBC=NULL;
cLinkedListT* pTC=NULL;
double dTol;
BOOL bRS;
iStep=0;

  PrintTime("START TIME: ");
  bGo=GetStepCasesLinStat(iStep, sSol, sStep,dTol,pLC,pBC,pTC,bRS);
  if (bGo)
  {
  ZeroDOF();                    //Zero the DOFS
  //THIS NEED TO BE REPERFORMED ONLY IF BC CHANGE THEN NEED TO RESTART ALL
  this->ApplyRes(pBC);             //APply Restraints (not local ones
  neq=GenDofs();                //Gen DOFS numbers
  sprintf_s(s1,"%s %i\n","NO OF EQUATIONS:-",neq);
  outtext1(s1);
  if (neq!=0)
  {
    ZeroThermalStrains();
    BuildForceVector(PropsT,MatT,pLC,pTC,neq,FVec);
    bGo=TRUE;
  }
  else
  {
    outtext1("ERROR: No Degrees of Freedom Exist.");
    outtext1("Ensure the Correct F.E. Model is Active.");
  }
  if (bGo==TRUE)
  {
    int iBW=this->MaxBW();                           //NOT USED IN THIS ITERATIVE SOLUTION
    iELCnt=0;
    int iNoRestEl=LocalResCount();
    Mat* KM = new Mat[iElNo+iNoRestEl];
    Vec<int>* G = new Vec<int>[iElNo+iNoRestEl];
    if (iNoRestEl>0)
       GenLocalResraints(KM,G,iELCnt);                //GENERATE SPRINGS FOR LOCAL RESTRAINTD
    outtext1("STARTING ASSY");
    E_Object* pE;
    Vec<double> DiagPrecon(neq);
    for (i=0;i<iElNo;i++)
    {
      pE=pElems[i];
      if (pE->ChkNegJac()==FALSE)
      {
        KME=pE->GetStiffMat(PropsT,MatT);
        Steer=pE->GetSteerVec3d();
      }
      else
      {
	      outtext1("ERROR: negative jacobian");
        pE->Reverse();
        KME=pE->GetStiffMat(PropsT,MatT);
        Steer=pE->GetSteerVec3d();
      }
      KM[iELCnt]=KME;
      G[iELCnt]=Steer;
      iELCnt++;
      KME.clear();
      Steer.clear();
    }
    //***************************************************************
    for (i=0;i<iELCnt;i++)
    {
      for (m=1;m<=KM[i].m;m++)
      {
        if (G[i](m-1)>0)
        { 
          double d1;
          d1=G[i](m-1);
          *DiagPrecon.nn(G[i](m-1))+=*KM[i].mn(m,m);
        }
      }
    }
    //Invert DiagPrecon
    Vec<double> d(neq); Vec<double> p(neq);
    for (i=0;i<neq;i++)
    {
      DiagPrecon(i)=1.0/DiagPrecon(i);
      double dT=DiagPrecon(i)*FVec(i);
      d(i)=DiagPrecon(i)*FVec(i);
      p(i)=d(i);
    }

    Mat pMult;
    Vec<double> x(neq);
    Vec<double> xnew(neq);
    outtext1("STARTING ITERATIVE SOLUTION");
    i=0;
    do
    {
       Vec<double> u(neq);
       for (j=0;j<iELCnt;j++)
       {
         pMult=GetG(p,G[j]);
         Mat uTemp=KM[j]*pMult;
         PutG(u,G[j],uTemp);
         pMult.clear();
       }
       up=DotProd(FVec,d);
       if (up==0) 
       {
         outtext1("ERROR: Forced Stop.");
         dErr=IterConv(x,xnew);
         break;
       }
       alpha=up/DotProd(p,u);
       for (m=0;m<xnew.n;m++)
       {
         xnew(m)=x(m)+p(m)*alpha;
         FVec(m)=FVec(m)-u(m)*alpha;
         d(m)=DiagPrecon(m)*FVec(m);
       }
       beta=DotProd(FVec,d)/up;
       for (m=0;m<xnew.n;m++)
       {
          p(m)=d(m)+p(m)*beta;
       }
       dErr=IterConv(x,xnew);
       x=xnew;
       if (i%100==0)
       {
          sprintf_s(s1,"%s: %i %s: %1.16f\n","ITER",i,"Err",dErr);
          outtext1(s1);
       }
       u.clear();
       i++;  
    }
    while(dErr>dTol);
    sprintf_s(s1,"%s: %i %s: %1.16f\n","FINAL ERROR ITER",i,"Err",dErr);
    outtext1(s1);
    for (i=0;i<iELCnt;i++)
    {
      KM[i].clear(); G[i].clear();
    }
    delete [] KM;
    delete [] G;
    outtext1("FINISHED SOLUTION");
    Displacements(iStep, sSol, sStep,Steer,xnew);
    TranslationalSpringForces(iStep, sSol, sStep, PropsT,MatT,Steer,xnew);
    StressesBeam(iStep, sSol, sStep, PropsT,MatT,Steer,xnew);
    Stresses2d(iStep, sSol, sStep, PropsT,MatT,Steer,xnew);
    Stresses3d(iStep, sSol, sStep, PropsT,MatT,Steer,xnew);
  }
  PrintTime("END TIME: ");
  }
  else
  {
    outtext1("FATAL ERROR");
  }
}


//*********************************************************************************
//                   E X P L I C I T    S O L
//*********************************************************************************

Mat  ME_Object::LocalDisp_E(C3dMatrix TMAT, Vec<int> &Steer, Vec<double> &Disp)
{
double dof1;
int idof;
int j;

Mat disp;
Mat disp3d;
disp.Create(8, 1);
disp3d.Create(3, 4);

for (j = 0; j < 4; j++)
{  //Get the displacements
	dof1 = 0;
	idof = *Steer.nn(j * 3 + 1);
	dof1 = *Disp.nn(idof);
	*disp3d.mn(1,j+1)= dof1;
	idof = *Steer.nn(j * 3 + 2);
	dof1 = *Disp.nn(idof);
	*disp3d.mn(2,j+1) = dof1;
	idof = *Steer.nn(j * 3 + 3);
	dof1 = *Disp.nn(idof);
	*disp3d.mn(3,j+1) = dof1;
}
C3dVector v;
C3dVector R;
int n;
//displacements in plain of element
for (j = 0; j < 4; j++)
{
	v.x = *disp3d.mn(1, j + 1);
	v.y = *disp3d.mn(2, j + 1);
	v.z = *disp3d.mn(3, j + 1);
	v = TMAT.Mult(v);
	n = 1 + j * 2;
	*disp.mn(n, 1) = v.x;
	*disp.mn(n + 1, 1) = v.y;
}
disp3d.clear();
return (disp);
}

void  ME_Object::FtoGlobal_E(Mat* LForce, C3dMatrix TMAT, Vec<int> &Steer, Vec<double> &FI)
{
	int idof;
	int j;
	Mat GForce;
	TMAT.Transpose();
	C3dVector vF;
	for (j = 0; j < 4; j++)
	{  //Get the displacements
		vF.x = *LForce->mn(j*2+1, 1);
		vF.y = *LForce->mn(j*2+2, 1);
		vF.z = 0;
		vF = TMAT.Mult(vF);
		idof = *Steer.nn(j * 3 + 1);
		*FI.nn(idof) += vF.x;
		idof = *Steer.nn(j * 3 + 2);
		*FI.nn(idof) += vF.y;
		idof = *Steer.nn(j * 3 + 3);
		*FI.nn(idof) += vF.z;
	}
}

void ME_Object::ExplicitSolTest(PropTable* PropsT, MatTable* MatT)
{ 
	char s1[80];
	double dT = 0.000001;  //Time Step
	double t = 0;
	double dMass = 0.1;
	C3dMatrix TMat;
	Vec<int> Steer;
	Mat bee;
	Mat KM;
	Mat Ldisp;         //Local displacements in xi eta
	Mat LForce;
	Mat eStrainInc;
	Mat eStrainTot(3,1);
	Mat eStress;
	int i;
	int k;
	int neq;
	double det;
	bool bGo;
	int iStep;
	CString sSol; CString sStep; double dTol;
	cLinkedList* pLC = NULL;
	cLinkedListB* pBC = NULL;
	cLinkedListT* pTC = NULL;
	BOOL bRS;
	Solution* pSol;
	pSol = pSOLS->GetCurSolution();
	if (pSol != NULL)
	{
		sSol = pSOLS->GetTitleString(pSOLS->iCur);
		iStep = pSol->GetCurStep();
		if (iStep != -1)
		{
			sStep = pSol->GetStepTitleString(iStep);
			bGo = GetStepCasesLinStat(iStep, sSol, sStep, dTol, pLC, pBC, pTC, bRS);
		}
		else
		{
			outtext1("ERROR: No Step Exists.");
		}
	}
	bGo = GetStepCasesLinStat(iStep, sSol, sStep, dTol, pLC, pBC, pTC, bRS);
	E_Object4* pE;
	Pt_Object* pN;
	//Get Current Loadcase and solution set
	ZeroDOF();                            //Zero the DOFS                                //THIS NEED TO BE REPORFED ONLY IF BC CHANGE THEN NEED TO RESTART ALL
	int iDof = 1;   
	neq = GenDofs3D(iDof);     //Generate 3 Dof at each node (membrain action only shell)
	//Initial conditions
	Vec<double> FI(neq);
	Vec<double> FE(neq);
	Vec<double> u(neq);     //last stet displacement increment
	Vec<double> x(neq);     //Total displacement
	Vec<double> xd(neq);
	Vec<double> xdd(neq);
	//Strain history
	Vec<double> eX(iElNo);
	Vec<double> eY(iElNo);
	Vec<double> eXY(iElNo);

	//*FE.nn(127) = 1;
	//*FE.nn(130) = 1;

	//*FE.nn(10) = 10;
	//******************************************************
	// START SOLUTION
	//******************************************************
	for (k = 0; k < 1000000; k++)
	{
		for (i = 0; i < iNdNo; i++)
		{
			pN = (Pt_Object*)pNodes[i];
			//Update the displacement central diff
			*u.nn(pN->dof[0]) = *xd.nn(pN->dof[0])*dT + *xdd.nn(pN->dof[0])*dT*dT;
			*u.nn(pN->dof[1]) = *xd.nn(pN->dof[1])*dT + *xdd.nn(pN->dof[1])*dT*dT;
			*u.nn(pN->dof[2]) = *xd.nn(pN->dof[2])*dT + *xdd.nn(pN->dof[2])*dT*dT;
			*x.nn(pN->dof[0]) += *xd.nn(pN->dof[0])*dT + *xdd.nn(pN->dof[0])*dT*dT;
			*x.nn(pN->dof[1]) += *xd.nn(pN->dof[1])*dT + *xdd.nn(pN->dof[1])*dT*dT;
			*x.nn(pN->dof[2]) += *xd.nn(pN->dof[2])*dT + *xdd.nn(pN->dof[2])*dT*dT;
		}
		x.diag();
		*u.nn(1) = 0;
		*u.nn(2) = 0;
		*u.nn(3) = 0;
		*u.nn(31) = 0;
		*u.nn(32) = 0;
		*u.nn(33) = 0;
		*x.nn(1) = 0;
		*x.nn(2) = 0;
		*x.nn(3) = 0;
		*x.nn(31) = 0;
		*x.nn(32) = 0;
		*x.nn(33) = 0;

		*x.nn(662) = 0;
		*x.nn(665) = 0;
		*x.nn(668) = 0;
		*x.nn(671) = 0;
		*x.nn(674) = 0;
		*x.nn(677) = 0;
		*x.nn(683) = 0;
		*x.nn(684) = 0;
		*x.nn(689) = 0;
		*x.nn(692) = 0;

		*u.nn(662) = 0;
		*u.nn(665) = 0;
		*u.nn(668) = 0;
		*u.nn(671) = 0;
		*u.nn(674) = 0;
		*u.nn(677) = 0;
		*u.nn(683) = 0;
		*u.nn(684) = 0;
		*u.nn(689) = 0;
		*u.nn(692) = 0;

		//*x.nn(693) = 1;
		//*u.nn(693) = 0;
		//*x.nn(663) = 0;
		//*u.nn(663) = 0;

		if (k < 38000)
		{
			*FE.nn(661) = 1;
			*FE.nn(664) = 1;
			*FE.nn(667) = 1;
			*FE.nn(670) = 1;
			*FE.nn(673) = 1;
			*FE.nn(676) = 1;
			*FE.nn(679) = 1;
			*FE.nn(682) = 1;
			*FE.nn(685) = 1;
			*FE.nn(688) = 1;
			*FE.nn(691) = 1;

		}


		FI.Zero();  //Zero nodal forces at each step
		//Update force vector
		if (k == 200)
			i = 0;

		for (i = 0; i < iElNo; i++)
		{
			if (pElems[i]->iType == 94)
			{
				pE = (E_Object4*)pElems[i];
				Steer = pE->GetSteerVec3d_E();  //3 dof steering vector
				TMat = pE->GetElSys_Ex(Steer,x);       //4 pt integerated membrain action (no bending) 8x8
				bee = pE->GetB_1pt(det);  //1 Pt bee mat
				Mat dee = pE->DeeMat(50e3, 0.29, 3); //Note Hard coded Properties for Now
				Ldisp = LocalDisp_E(TMat, Steer, u); //Strain Increment
				//Ldisp.diag();
				eStrainInc = bee * Ldisp;
				//Element Strain total
				*eX.nn(i + 1) += *eStrainInc.mn(1, 1);
				*eY.nn(i + 1) += *eStrainInc.mn(2, 1);
				*eXY.nn(i + 1) += *eStrainInc.mn(3, 1);
				*eStrainTot.mn(1, 1) = *eX.nn(i + 1);
				*eStrainTot.mn(2, 1) = *eY.nn(i + 1);
				*eStrainTot.mn(3, 1) = *eXY.nn(i + 1);
				eStress = dee * eStrainTot;
				bee.Transpose();
				LForce=bee*eStress;
				//LForce.diag();
				if (k % 100 == 0)
				{
					//sprintf_s(s1, "%s: %i %s: %1.16f %s: %1.16f %s: %g\n", "ITER", k, "X", *Ldisp.mn(1,1), "Y", *Ldisp.mn(2, 1),"F" , *LForce.mn(1,1));
					//outtext1(s1);
				}
				
				FtoGlobal_E(&LForce, TMat, Steer, FI);
				
				bee.clear();
				dee.clear();
				Ldisp.clear();
				Steer.clear();
				LForce.clear();
				eStrainInc.clear();
			}
		}
		//FI.diag();
		//FE.diag();

		for (i = 0; i < iNdNo; i++)
		{
			pN = (Pt_Object*)pNodes[i];
			//Calc acceleration t+dt
			double acctdt;
			acctdt = (*FE.nn(pN->dof[0]) -*FI.nn(pN->dof[0])) / dMass;
			*xd.nn(pN->dof[0]) += (*xdd.nn(pN->dof[0]) + acctdt)*dT*0.5;
			*xdd.nn(pN->dof[0]) = acctdt;

			acctdt = (*FE.nn(pN->dof[1]) -*FI.nn(pN->dof[1])) / dMass;
			*xd.nn(pN->dof[1]) += (*xdd.nn(pN->dof[1]) + acctdt)*dT*0.5;
			*xdd.nn(pN->dof[1]) = acctdt;

			acctdt = (*FE.nn(pN->dof[2]) -*FI.nn(pN->dof[2])) / dMass;
			*xd.nn(pN->dof[2]) += (*xdd.nn(pN->dof[2]) + acctdt)*dT*0.5;
			*xdd.nn(pN->dof[2]) = acctdt;
		}
		//sprintf_s(s1, "%s: %i %s: %1.16f %s: %1.16f\n", "ITER", k, "FI", *FI.nn(4), "x", *x.nn(4));
		//outtext1(s1);
		//x.diag();
		t += dT;
		if (k % 2000 == 0)
		{
			
			sprintf_s(s1, "%s: %g %s: %g %s: %g %s: %g\n", "T ", t, "X ", *x.nn(130), "Y ", *x.nn(131), "Z ", *x.nn(132));
			//sprintf_s(s1, "%s: %i %s: %g %s: %g %s: %g\n", "ITER", k, "X1 ", TMat.m_00, "X2 ", TMat.m_01, "X3 ", TMat.m_02);
			outtext1(s1);
			Displacements(k, "T", "A", Steer, x);
		}
		
	}
}

//*********************************************************************************
//                   1 D O F    T H E R M A L    S O L
//*********************************************************************************
void ME_Object::IterSol1dSS(PropTable* PropsT,MatTable* MatT)
{
int i,j,neq;
char s1[80];
double dErr;
int iELCnt;
int m;
double up;
double beta;
double alpha;
int aaa=0;
double d1;
BOOL bGo=FALSE;
Mat KME; 
Vec<double> QVec;
Vec<double> TVec;
Vec<int> Steer;
int iStep;
Solution* pSol;
cLinkedList* pLC=NULL;
cLinkedListB* pBC=NULL;
cLinkedListT* pTC=NULL;
Mat pMult;
double dTol;
BOOL bRS;
CString sSol;
CString sStep;

pSol=pSOLS->GetCurSolution();
if (pSol!=NULL)
{
  sSol=pSOLS->GetTitleString(pSOLS->iCur);
  iStep=pSol->GetCurStep();
  if (iStep!=-1)
  {
    sStep=pSol->GetStepTitleString(iStep);
    bGo=GetStepCasesLinStat(iStep, sSol,sStep,dTol,pLC,pBC,pTC,bRS);
  }
  else
  {
    outtext1("ERROR: No Step Exists.");
  }
}
else
{
  outtext1("ERROR: No Solution Exists.");
}
  if (bGo)
  {
  outtext1("STARTING SOLUTION OF:-");
  outtext1(sSol);
  outtext1(sStep);
  PrintTime("START TIME: ");
  ZeroDOF();                            //Zero the DOFS                                //THIS NEED TO BE REPORFED ONLY IF BC CHANGE THEN NEED TO RESTART ALL
  int iDof = ApplyResSS(pBC);           //APply Restraints (not local ones
  neq=GenDofs1D(iDof);                  //Gen DOFS numbers
  sprintf_s(s1,"%s %i\n","NO OF EQUATIONS:-",neq);
  outtext1(s1);
  if (neq!=0)
  {
    TVec=GetTempVec(pBC, neq);
    //TVec.diag();
    QVec=GetForceVec(pLC, neq);
    //QVec.diag();
    ReportQResultant(QVec);
    //BuildForceVector(PropsT,MatT,pLC,pTC,neq,QVec);
    bGo=TRUE;
  }
  else
  {
    outtext1("ERROR: No Degrees of Freedom Exist.");
    outtext1("Ensure the Correct F.E. Model is Active.");
  }
  if (bGo==TRUE)
  {

    int iBW=this->MaxBW();                         //NOT USED IN ITERATIVE SOL
    iELCnt=0;
    Mat* KM = new Mat[iElNo];
    Vec<int>* G = new Vec<int>[iElNo];
    
    E_Object* pE;
    Vec<double> DiagPrecon(neq);
    for (i=0;i<iElNo;i++)
    {
      pE=pElems[i];
      if (pE->ChkNegJac()==FALSE)
      {
        KME=pE->GetThermMat(PropsT,MatT);
        Steer=pE->GetSteerVec1d();
      }
      else
      {
	      outtext1("ERROR: negative jacobian reversing element.");
        pE->Reverse();
        KME=pE->GetThermMat(PropsT,MatT);
        Steer=pE->GetSteerVec1d();
      }
      KM[iELCnt]=KME;
      G[iELCnt]=Steer;
      iELCnt++;
      KME.clear();
      Steer.clear();
    }
    //***************************************************************
    for (i=0;i<iELCnt;i++)
    {
      for (m=1;m<=KM[i].m;m++)
      {
        if (G[i](m-1)>0)
        { 

          d1=G[i](m-1);
          *DiagPrecon.nn(G[i](m-1))+=*KM[i].mn(m,m);
        }
      }
    }
    //DiagPrecon.diag();
    //*********** APPLY THE TEMPS AS FLUX ADDITION TO QVEC ***********
    //TVec.diag();
    for (j=0;j<iELCnt;j++)
    {
       pMult=GetG(TVec,G[j]);
       Mat uTemp=KM[j]*pMult;
       PutG(QVec,G[j],uTemp);
       pMult.clear();
    }
    //****************** INVERT THR PRE-CODITIONER **********************
    Vec<double> d(neq); 
    Vec<double> p(neq);
    for (i=0;i<neq;i++)
    {
      if (i<iDof-1)
      {
         DiagPrecon(i)=0;
      }
      else
      {
         DiagPrecon(i)=1.0/DiagPrecon(i);
      }
      d(i)=DiagPrecon(i)*QVec(i);
      p(i)=d(i);
    }


    Vec<double> x(neq);
    Vec<double> xnew(neq);
    outtext1("STARTING ITERATIVE SOLUTION");
    i=0;
    do
    {
       Vec<double> u(neq);
       //QVec.diag();
       //p.diag();
       //d.diag();
       for (j=0;j<iELCnt;j++)
       {
         pMult=GetG(p,G[j]);
         //pMult.diag();
         //KM[j].diag();
         Mat uTemp=KM[j]*pMult;
         //uTemp.diag();
         PutG(u,G[j],uTemp);
         pMult.clear();
       }
       //u.diag();
       up=DotProd(QVec,d);
       if (up==0) 
       {
         outtext1("ERROR: Forced Stop.");
         dErr=IterConv(x,xnew);

         break;
       }
       alpha=up/DotProd(p,u);
       for (m=0;m<xnew.n;m++)
       {
         xnew(m)=x(m)+p(m)*alpha;
         QVec(m)=QVec(m)-u(m)*alpha;
         d(m)=DiagPrecon(m)*QVec(m);
       }
       beta=DotProd(QVec,d)/up;
       for (m=0;m<xnew.n;m++)
       {
          p(m)=d(m)+p(m)*beta;
       }
       dErr=IterConv(x,xnew);
       x=xnew;
       if (i%100==0)
       {
          sprintf_s(s1,"%s: %i %s: %1.16f\n","ITER",i,"Err",dErr);
          outtext1(s1);
          //inner thermal loop temporarylly here
          //RadiationLoss(Steer, xnew, QVec);
       }
       u.clear();
       i++;  
    }
    while (dErr>dTol);
    sprintf_s(s1,"%s: %i %s: %1.16f\n","FINAL ERROR ITER",i,"Err",dErr);
    outtext1(s1);
    for (i=0;i<iELCnt;i++)
    {
      KM[i].clear(); 
      G[i].clear();
    }
    delete [] KM;
    delete [] G;
    outtext1("FINISHED SOLUTION");
    for (i=0;i<iDof-1;i++)
    {
      xnew(i)=-TVec(i);
    }
    Temperatures(iStep,sSol,sStep,Steer,xnew);
    TempBCSet(iStep, sSol, sStep, Steer, xnew);
  }
  PrintTime("END TIME: ");
  }
  else
  {
    outtext1("FATAL ERROR");
  }

  TVec.clear();
  QVec.clear();
}

void ME_Object::PrintTime(CString cS)
{
int Hour;
int Min;
int Sec;
char s1[80];
COleDateTime timeStart;
timeStart = COleDateTime::GetCurrentTime();
Hour=timeStart.GetHour();
Min=timeStart.GetMinute();
Sec=timeStart.GetSecond();
sprintf_s(s1,"%s %i:%i:%i\n",cS,Hour,Min,Sec);
outtext1(s1);
}

int ME_Object::MaxBW()
{
int iMaxBW=0;
int iMB;
int i;
for (i=0;i<iElNo;i++)
{
  iMB=pElems[i]->MaxBW();
  if (iMB>iMaxBW)
  {
    iMaxBW=iMB;
  }
}
return (iMaxBW);
}

G_Object* ME_Object::AddForce(Pt_Object* pInNode,C3dVector inF,int inSetID)
{
cLinkedList* pSet=NULL;
Force* pF=NULL;
int ID;
if ((inSetID==-1) && (iCurLC==-1))
{
  outtext1("ERROR: No Load Case Active.");
}
else if ((inSetID==-1) && (iCurLC!=-1))
{
  pSet=LCS[iCurLC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetLC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pF=new Force();
  pF->Create(pInNode,
	  	     pSet,
	  	     inF,
	 	     ID);
  pSet->Add(pF);
  //pBCLDs[iBCLDs] = pF;
  //iBCLDs++;
}
return (pF);
}

G_Object* ME_Object::AddTemperature(E_Object* pInE,double inT,int inSetID)
{
cLinkedList* pSet=NULL;
Temperature* pT=NULL;
int ID;
if ((inSetID==-1) && (iCurTSet==-1))
{
  outtext1("ERROR: No Temperature Set Active.");
}
else if ((inSetID==-1) && (iCurTSet!=-1))
{
  pSet=TSETS[iCurTSet];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetTSET(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pT=new Temperature();
  pT->Create(pInE,
	  	       pSet,
	  	       inT,
	 	         ID);
  pSet->Add(pT);
}
return (pT);
}

G_Object* ME_Object::AddAccel(E_Object* pInE,C3dVector vA,int inSetID)
{
cLinkedList* pSet=NULL;
AccelLoad* pA=NULL;
int ID;
if ((inSetID==-1) && (iCurLC==-1))
{
  outtext1("ERROR: No Load Set Active.");
}
else if ((inSetID==-1) && (iCurLC!=-1))
{
  pSet=LCS[iCurLC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=LCS[iCurLC];
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pA=new AccelLoad();
  pA->Create(pInE,
	  	       pSet,
	  	       vA,
	 	         ID);
  pSet->Add(pA);
}
return (pA);
}


G_Object* ME_Object::AddRotAccel(E_Object* pInE, C3dVector vAxisD, C3dVector vAxisC, double dw, int inSetID)
{
  cLinkedList* pSet = NULL;
  RotationLoad* pA = NULL;
  int ID;
  if ((inSetID == -1) && (iCurLC == -1))
  {
    outtext1("ERROR: No Load Set Active.");
  }
  else if ((inSetID == -1) && (iCurLC != -1))
  {
    pSet = LCS[iCurLC];
    ID = pSet->iLabel;
  }
  else if (inSetID != -1)
  {
    pSet = LCS[iCurLC];
    ID = pSet->iLabel;
  }
  if (pSet != NULL)
  {
    pA = new RotationLoad();
    pA->Create(pInE,
      pSet,
      vAxisD,
      vAxisC,
      dw,
      ID);
      pSet->Add(pA);
  }
  return (pA);
}


G_Object* ME_Object::AddFluxQ(Pt_Object* pInNode,double inT,int inSetID)
{
cLinkedList* pSet=NULL;
Temperature* pT=NULL;
int ID;
if ((inSetID==-1) && (iCurLC==-1))
{
  outtext1("ERROR: No Load Set Active.");
}
else if ((inSetID==-1) && (iCurLC!=-1))
{
  pSet=LCS[iCurLC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetLC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pT=new FluxLoad();
  pT->Create(pInNode,
	  	     pSet,
	  	     inT,
	 	     ID);
  pSet->Add(pT);
}
return (pT);
}

G_Object* ME_Object::AddTemperatureBC(Pt_Object* pInNode,double inT,int inSetID)
{
cLinkedList* pSet=NULL;
Temperature* pT=NULL;
int ID;
if ((inSetID==-1) && (iCurBC==-1))
{
  outtext1("ERROR: No Boundary Set Active.");
}
else if ((inSetID==-1) && (iCurBC!=-1))
{
  pSet=BCS[iCurBC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetBC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pT=new TemperatureBC();
  pT->Create(pInNode,
	  	     pSet,
	  	     inT,
	 	     ID);
  pSet->Add(pT);
}
return (pT);
}



G_Object* ME_Object::AddMoment(Pt_Object* pInNode,C3dVector inF,int inSetID)
{
cLinkedList* pSet=NULL;
Moment* pF=NULL;
int ID;
if ((inSetID==-1) && (iCurLC==-1))
{
  outtext1("ERROR: No Load Set Active.");
}
else if ((inSetID==-1) && (iCurLC!=-1))
{
  pSet=LCS[iCurLC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetLC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pF=new Moment();
  pF->Create(pInNode,
	  	     pSet,
	  	     inF,
	 	     ID);
  pSet->Add(pF);
  //pBCLDs[iBCLDs] = pF;
  //iBCLDs++;
}
return (pF);
}

G_Object* ME_Object::AddPressure(E_Object* pInE,C3dVector inF,int inSetID)
{
cLinkedList* pSet=NULL;
Pressure* pF=NULL;
int ID;
if ((inSetID==-1) && (iCurLC==-1))
{
  outtext1("ERROR: No Load Set Active.");
}
else if ((inSetID==-1) && (iCurLC!=-1))
{
  pSet=LCS[iCurLC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetLC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pF=new Pressure();
  pF->Create(pInE,
	  	       pSet,
             inF,
             ID);
  pSet->Add(pF);
}
return (pF);
}

void ME_Object::BuildForceVector(PropTable* PropsT,MatTable* MatT,cLinkedList* pLC,cLinkedList* pTC,int neq,Vec<double> &FVec)
{
  if (pLC!=NULL)
  {
    FVec = GetForceVec(pLC,neq);                  //Get all external forces
    GetPressureLoads(pLC,neq,FVec);               //Add in all Surface pressure loads
    GetAccelLoads(PropsT,MatT,pLC,neq,FVec);      //Add In all Body loads
    GetRotAccelLoads(PropsT, MatT, pLC, neq, FVec);      //Add In all Rotational Body loads
  }
  ReportFResultant(FVec);
  if (pTC!=NULL)
    GetThermalLoads(PropsT,MatT,pTC,neq,FVec);    //Add Thermal loads
}



void ME_Object::ReportFResultant(Vec<double> &FVec)
{
  int i;
  char s1[80];
  double FXTot=0;
  double FYTot=0;
  double FZTot=0;
  double MXTot=0;
  double MYTot=0;
  double MZTot=0;
  Pt_Object* pN;
  C3dVector vC;
  C3dVector vF;
  C3dVector vM;
//sprintf_s(s1,"%s: %i %s: %1.16f\n","ITER",i,"Err",dErr);
  outtext1("GLOBAL LOADS RESULTANT:-");
  for (i=0;i<iNdNo;i++)
  {
     pN = pNodes[i];
     vC=pN->Get_Centroid();
     vF.Set(0,0,0);

     if (pN->dof[0]>0)
	   {
       vF.x=*FVec.nn(pN->dof[0]);
	   }
     if (pN->dof[1]>0)
	   {
       vF.y=*FVec.nn(pN->dof[1]);
	   }
     if (pN->dof[2]>0)
	   {
       vF.z=*FVec.nn(pN->dof[2]);
	   }
     //MOMENTS
     vM=vC.Cross(vF);
     FXTot+=vF.x;
     FYTot+=vF.y;
     FZTot+=vF.z;
     MXTot+=vM.x;
     MYTot+=vM.y;
     MZTot+=vM.z;
     if (pN->dof[3]>0)
	   {
       MXTot+=*FVec.nn(pN->dof[3]);
	   }
     if (pN->dof[4]>0)
	   {
       MYTot+=*FVec.nn(pN->dof[4]);
	   }
     if (pN->dof[5]>0)
	   {
       MZTot+=*FVec.nn(pN->dof[5]);
	   }
  }
  sprintf_s(s1,"%s: %1.16f\n","FX",FXTot);
  outtext1(_T(s1));
  sprintf_s(s1,"%s: %1.16f\n","FY",FYTot);
  outtext1(_T(s1));
  sprintf_s(s1,"%s: %1.16f\n","FZ",FZTot);
  outtext1(_T(s1));

  sprintf_s(s1,"%s: %1.16f\n","MX",MXTot);
  outtext1(_T(s1));
  sprintf_s(s1,"%s: %1.16f\n","MY",MYTot);
  outtext1(_T(s1));
  sprintf_s(s1,"%s: %1.16f\n","MZ",MZTot);
  outtext1(_T(s1));

}

void ME_Object::ReportQResultant(Vec<double> &QVec)
{
  int i;
  char s1[80];
  double Q=0;

  Pt_Object* pN;

//sprintf_s(s1,"%s: %i %s: %1.16f\n","ITER",i,"Err",dErr);
  outtext1("GLOBAL FLUX RESULTANT:-");
  for (i=0;i<iNdNo;i++)
  {
     pN = pNodes[i];
     if (pN->dof[0]>0)
	   {
       Q+=*QVec.nn(pN->dof[0]);
	   }
  }
  sprintf_s(s1,"%s: %f\n","Q",Q);
  outtext1(_T(s1));
}

void ME_Object::GetThermalLoads(PropTable* PropsT,MatTable* MatT,cLinkedList* pTC,int neq,Vec<double> &FVec)
{
  BCLD* pNext;
  Mat TF;
  Vec<int> vS;
  E_Object* pE;
  int iNS;
  int iD;

  if ((PropsT==NULL) || (MatT==NULL))
  {
    outtext1("ERROR: Property or Mat Table Missing, Body Loads Not Calculated.");
  }
  else if (pTC!=NULL)
  {
    pNext=(BCLD*) pTC->Head;
    while (pNext!=NULL)
    {
      if (pNext->iObjType==325)
      { 
        Temperature* pT=(Temperature*) pNext;
        double dT=pT->dV;
        pE=(E_Object*) pNext->pObj;
        if (pE!=NULL)
        {
          vS=pE->GetSteerVec3d();
          TF=pE->GetThermalStrainMat3d(PropsT,MatT,dT);
          iNS=pE->iNoNodes;
          iD=pE->noDof();
          int i;
          if ((pE->iType==115) || (pE->iType==111) || (pE->iType==112))
          {
            for (i=0;i<iNS;i++)
            {
              int iDD0,iDD1,iDD2;
              iDD0=*vS.nn(i*iD+1);
              iDD1=*vS.nn(i*iD+2);
              iDD2=*vS.nn(i*iD+3);
              if (iDD0!=-1)
                *FVec.nn(iDD0)+=*TF.mn(i*iD+1,1);
              if (iDD1!=-1)
                *FVec.nn(iDD1)+=*TF.mn(i*iD+2,1);
              if (iDD2!=-1)
                *FVec.nn(iDD2)+=*TF.mn(i*iD+3,1);
            }
          }
          else if ((pE->iType==91) || (pE->iType==94)) //this will be a 2DOF forcece vec
          {                                                // needs to be transfoemed to 3d
            C3dMatrix M3=pE->GetElSys();                   // The element transformation matrix
            M3.Transpose();
            C3dVector vFl;
            C3dVector vFg;
            for (i=0;i<iNS;i++)                            //For each node 
            {
              vFg.x=*TF.mn(i*2+1,1);
              vFg.y=*TF.mn(i*2+2,1);
              vFg.z=0;
              vFl=M3*vFg;
              int iDD0,iDD1,iDD2;
              iDD0=*vS.nn(i*iD+1);
              iDD1=*vS.nn(i*iD+2);
              iDD2=*vS.nn(i*iD+3);
              if (iDD0!=-1)
                *FVec.nn(iDD0)+=vFl.x;
              if (iDD1!=-1)
                *FVec.nn(iDD1)+=vFl.y;
              if (iDD2!=-1)
                *FVec.nn(iDD2)+=vFl.z;
            }
          }
          else if ((pE->iType==11) || (pE->iType==21)) //this will be a 1DOF forcece vec for RODS AND BEAMS
          {                                                // needs to be transfoemed to 3d
            C3dMatrix M3=pE->GetElSys();                   // The element transformation matrix
            M3.Transpose();
            C3dVector vFl;
            C3dVector vFg;
            for (i=0;i<iNS;i++)                            //For each node 
            {
              vFg.x=*TF.mn(i+1,1);
              vFg.y=0;
              vFg.z=0;
              vFl=M3*vFg;
              int iDD0,iDD1,iDD2;
              iDD0=*vS.nn(i*iD+1);
              iDD1=*vS.nn(i*iD+2);
              iDD2=*vS.nn(i*iD+3);
              if (iDD0!=-1)
                *FVec.nn(iDD0)+=vFl.x;
              if (iDD1!=-1)
                *FVec.nn(iDD1)+=vFl.y;
              if (iDD2!=-1)
                *FVec.nn(iDD2)+=vFl.z;
            }
          }
        }
      }
    pNext=(BCLD*) pNext->next;
    }
  }
}

void ME_Object::ZeroThermalStrains()
{
  int i;
  for (i=0;i<iElNo;i++)
  {
    pElems[i]->dTemp=0;
  }
}

void ME_Object::GetAccelLoads(PropTable* PropsT,MatTable* MatT,cLinkedList* pLC,int neq,Vec<double> &FVec)
{
  BCLD* pNext;
  Mat MM;
  Vec<int> vS;
  E_Object* pE;
  int iNS;
  int iD;

  if ((PropsT==NULL) || (MatT==NULL))
  {
    outtext1("ERROR: Property or Mat Table Missing, Body Loads Not Calculated.");
  }
  else if (pLC!=NULL)
  {
    pNext=(BCLD*) pLC->Head;
    while (pNext!=NULL)
    {
      if (pNext->iObjType==328)
      { 
        AccelLoad* pA=(AccelLoad*) pNext;
        C3dVector vAC=pA->vA;
        pE=(E_Object*) pNext->pObj;
        if (pE!=NULL)
        {
          vS=pE->GetSteerVec3d();
          MM=pE->GetElNodalMass(PropsT,MatT);
          iNS=pE->iNoNodes;
          iD=pE->noDof();
          int i;
          for (i=0;i<iNS;i++)
          {
            int iDD0,iDD1,iDD2;
            iDD0=*vS.nn(i*iD+1);
            iDD1=*vS.nn(i*iD+2);
            iDD2=*vS.nn(i*iD+3);
            double dTTT=*MM.mn(i+1,1);
            if (iDD0!=-1)
               *FVec.nn(iDD0)+=*MM.mn(i+1,1)*vAC.x;
            if (iDD1!=-1)
               *FVec.nn(iDD1)+=*MM.mn(i+1,1)*vAC.y;
            if (iDD2!=-1)
               *FVec.nn(iDD2)+=*MM.mn(i+1,1)*vAC.z;
          }
        }
      }
    pNext=(BCLD*) pNext->next;
    }
  }
}

void ME_Object::GetRotAccelLoads(PropTable* PropsT, MatTable* MatT, cLinkedList* pLC, int neq, Vec<double> &FVec)
{
  BCLD* pNext;
  Mat MM;
  Vec<int> vS;
  E_Object* pE;
  int iNS;
  int iD;

  if ((PropsT == NULL) || (MatT == NULL))
  {
    outtext1("ERROR: Property or Mat Table Missing, Body Loads Not Calculated.");
  }
  else if (pLC != NULL)
  {
    pNext = (BCLD*)pLC->Head;
    while (pNext != NULL)
    {
      if (pNext->iObjType == 329)
      {
        RotationLoad* pA = (RotationLoad*)pNext;
        C3dVector vAC = pA->vAxisC;
        C3dVector vAD = pA->vAxisD;
        double dw = pA->w;
        pE = (E_Object*) pNext->pObj;
        if (pE != NULL)
        {
          vS = pE->GetSteerVec3d();
          MM = pE->GetElNodalMass(PropsT, MatT);
          iNS = pE->iNoNodes;
          iD = pE->noDof();
          int i;
          C3dVector pN = pE->Get_Centroid();  //Node location
          C3dVector vw = pN - vAC;
          C3dVector P = vAD;
          P *= vw.Dot(vAD);
          P += vAC;
          C3dVector R = pN - P;
          double dR = R.Mag();
          R.Normalize(); //Error here
          for (i = 0; i<iNS; i++)
          {

            int iDD0, iDD1, iDD2;
            iDD0 = *vS.nn(i*iD + 1);
            iDD1 = *vS.nn(i*iD + 2);
            iDD2 = *vS.nn(i*iD + 3);
            double dTTT = *MM.mn(i + 1, 1);
            if (iDD0 != -1)
              *FVec.nn(iDD0) += *MM.mn(i + 1, 1)*R.x*dw*dw*dR;
            if (iDD1 != -1)
              *FVec.nn(iDD1) += *MM.mn(i + 1, 1)*R.y*dw*dw*dR;
            if (iDD2 != -1)
              *FVec.nn(iDD2) += *MM.mn(i + 1, 1)*R.z*dw*dw*dR;
          }
        }
      }
      pNext = (BCLD*)pNext->next;
    }
  }
}


void ME_Object::GetPressureLoads(cLinkedList* pLC,int neq,Vec<double> &FVec)
{
int j,k;
double P;
Mat coord;
Mat Press;
Mat deriv;
Mat deriv2;
Mat fun;
Mat jac;
Mat Points;
double det;
double nip;
BCLD* pNext;
pNext=(BCLD*) pLC->Head;

while (pNext!=NULL)
{
  if (pNext->iObjType==324)
  {
     E_Object* pE = (E_Object*) pNext->pObj;
     Pressure* pP=(Pressure*) pNext;
     P=pP->F.x;
     if (pE->iType==91)
     {
       nip=1;
     }
     else if (pE->iType==94)
     {
       nip=4;
     }
     coord.clear();
     Points.clear();
     Press.clear();
     Press.Create(1,pE->iNoNodes);
     coord=pE->getCoords3d();
     Points=pE->Sample((int) nip);
     for(k=1;k<nip+1;k++)
     {
       det = 0;
       fun=pE->ShapeFun(Points, k);
       deriv=pE->ShapeDer(Points, k);
       jac = deriv*coord;
       jac=jac.InvertJac(det);
       for (j=0;j<pE->iNoNodes;j++)
       {
        *Press.mn(1,j+1)+=*fun.mn(1,j+1)*P*det**Points.mn(k,3);
        double pp=*Press.mn(1,j+1);
        pp=pp;
       }
     }
     C3dVector vN;
     vN=pE->Get_Normal();
     vN.Normalize();
     Pt_Object* pN;
     for (j=0;j<pE->iNoNodes;j++)
     {
       pN=(Pt_Object*) pE->GetNode(j);
       if (pN->dof[0]>0)
	     {
         *FVec.nn(pN->dof[0])+=*Press.mn(1,j+1)*vN.x;
	     }
       if (pN->dof[1]>0)
	     {
         *FVec.nn(pN->dof[1])+=*Press.mn(1,j+1)*vN.y;
	     }
       if (pN->dof[2]>0)
	     {
         *FVec.nn(pN->dof[2])+=*Press.mn(1,j+1)*vN.z;
	     }
     }
  }
pNext=(BCLD*) pNext->next;
}

}

Vec <double> ME_Object::GetTempVec(cLinkedList* pTS,int neq)
{
Vec <double> TVec(neq);
BCLD* pNext;
pNext=(BCLD*) pTS->Head;
if (pTS!=NULL)
  {
    pNext=(BCLD*) pTS->Head;
    while (pNext!=NULL)
    {
      if (pNext->iObjType==327)
      {
        TemperatureBC* pR=(TemperatureBC*) pNext;
        Pt_Object* pNode=(Pt_Object*) pR->pObj;
        *TVec.nn(pNode->dof[0])=-pR->dV;
      }
      pNext=(BCLD*) pNext->next;
    }
  }
return (TVec);
}


Vec <double> ME_Object::GetForceVec(cLinkedList* pLC,int neq)
{
Vec <double> FVec(neq);
BCLD* pNext;
pNext=(BCLD*) pLC->Head;
while (pNext!=NULL)
{
  if (pNext->iObjType==321)
  {
     Pt_Object* pNode = (Pt_Object*) pNext->pObj;
     Force* pF=(Force*) pNext;
     if (pNode->dof[0]>0)
	   {
       *FVec.nn(pNode->dof[0])=pF->F.x;
	   }
	   if (pNode->dof[1]>0)
	   {
       *FVec.nn(pNode->dof[1])=pF->F.y;
	   }
	   if (pNode->dof[2]>0)
	   {
       *FVec.nn(pNode->dof[2])=pF->F.z;
	   }
  }
  else if (pNext->iObjType==323)
  {
     Pt_Object* pNode = (Pt_Object*) pNext->pObj;
     Moment* pF=(Moment*) pNext;
     if (pNode->dof[3]>0)
	   {
       *FVec.nn(pNode->dof[3])=pF->F.x;
	   }
	   if (pNode->dof[4]>0)
	   {
       *FVec.nn(pNode->dof[4])=pF->F.y;
	   }
	   if (pNode->dof[5]>0)
	   {
       *FVec.nn(pNode->dof[5])=pF->F.z;
	   }
  }
  else if (pNext->iObjType==326)
  {
     Pt_Object* pNode = (Pt_Object*) pNext->pObj;
     FluxLoad* pF=(FluxLoad*) pNext;
     if (pNode->dof[0]>0)
	   {
         *FVec.nn(pNode->dof[0])=pF->dV;
	   }
  }
pNext=(BCLD*) pNext->next;
}
return (FVec);
}




G_Object* ME_Object::AddRestraint(Pt_Object* pInNode,
								                  BOOL xon,
                                  BOOL yon,
                                  BOOL zon,
                                  BOOL rxon,
                                  BOOL ryon,
                                  BOOL rzon,
                                  int inSetID)
{

cLinkedList* pSet=NULL;
Restraint* pF=NULL;
int ID = -1;
if ((inSetID==-1) && (iCurBC==-1))
{
  outtext1("ERROR: No Boundary Set Active.");
}
else if ((inSetID==-1) && (iCurBC!=-1))
{
  pSet=BCS[iCurBC];  
  ID=pSet->iLabel;
}
else if (inSetID!=-1)
{
  pSet=GetBC(inSetID);
  ID=pSet->iLabel;
}
if (pSet!=NULL)
{
  pF=new Restraint();
  pF->Create(pInNode,
		     pSet,
		     xon,yon,zon,rxon,ryon,rzon,
		     inSetID);
  pSet->Add(pF);
}
return (pF);
}

void ME_Object::ApplyRes(cLinkedListB* pBC)
{
  int j;
  BCLD* pNext;
  if (pBC!=NULL)
  {
  pNext=(BCLD*) pBC->Head;
  while (pNext!=NULL)
  {
    if (pNext->iObjType==322)
    {
      Restraint* pR=(Restraint*) pNext;
      Pt_Object* pNode=(Pt_Object*) pR->pObj;
      // only apply the restrain if it a global dof restraint
      // else it local and need transformation
      if (pNode->OutSys==0)
      {
	      for (j=0;j<6;j++)
	      {
	        if (pR->REST[j]==TRUE)
	        {
            pNode->dof[j]=-1;  
	        }
	      }
      }
    }
  pNext=(BCLD*) pNext->next;
  }
  }
}



int ME_Object::ApplyResSS(cLinkedListB* pBC)
{
  int iDof=1;
  BCLD* pNext;
  if (pBC!=NULL)
  {
    pNext=(BCLD*) pBC->Head;
    while (pNext!=NULL)
    {
      if (pNext->iObjType==327)
      {
        TemperatureBC* pR=(TemperatureBC*) pNext;
        Pt_Object* pNode=(Pt_Object*) pR->pObj;
        // only apply the restrain if its ZEROR else need to use penulty method
        pNode->dof[0]=iDof;  
        iDof++;
      }
      pNext=(BCLD*) pNext->next;
    }
  }
  return (iDof);
}

void ME_Object::LocalRes(int neq,Vec<int> &Steer,Vec<double> &KMA)
{
int i,j,k;
C3dVector K;
C3dMatrix r;
CoordSys* pS;
Vec<int> V(2*3);
Mat KM(6,6);
Mat t(6,6);
Mat tt(6,6);
Mat Kmt;
Mat tKmt;

for (k=0;k<iBCLDs;k++)
{
  if (pBCLDs[k]->iObjType==322)
  {
   Restraint* pR=(Restraint*) pBCLDs[k];
   Pt_Object* pNode=(Pt_Object*) pR->pObj;
   if (pNode->OutSys!=0)
   {
     pS=this->GetSys(pNode->OutSys);
     if (pS!=NULL)
     {
       //This composes the stiffness tranformation matrix
       r=GetNodalSys(pNode);
       r.Transpose();
       Mat mr(3,3);
       *mr.mn(1,1)=r.m_00; *mr.mn(1,2)=r.m_01; *mr.mn(1,3)=r.m_02;
       *mr.mn(2,1)=r.m_10; *mr.mn(2,2)=r.m_11; *mr.mn(2,3)=r.m_12;
       *mr.mn(3,1)=r.m_20; *mr.mn(3,2)=r.m_21; *mr.mn(3,3)=r.m_22;
       for (i=1;i<4;i++)
       {
          for (j=1;j<4;j++)
          {
             *t.mn(i,j) = *mr.mn(i,j);
             *t.mn(i+3,j+3) = *mr.mn(i,j);
             *tt.mn(i,j) = *mr.mn(i,j);
             *tt.mn(i+3,j+3) = *mr.mn(i,j);
          }
       }
       tt.Transpose();
       //This composes the local stiffness matrix
       KM*=0;
	     if (pR->REST[0]==TRUE)  //X
       {
         *KM.mn(1,1)=1e15;*KM.mn(4,4)=1e15;*KM.mn(1,4)=-1e15;*KM.mn(4,1)=-1e15;
       }
       if (pR->REST[1]==TRUE)  //Y
       {
         *KM.mn(2,2)=1e15;*KM.mn(5,5)=1e15;*KM.mn(2,5)=-1e15;*KM.mn(5,2)=-1e15;
       }
       if (pR->REST[2]==TRUE)  //Z
       {
         *KM.mn(3,3)=1e15;*KM.mn(6,6)=1e15;*KM.mn(3,6)=-1e15;*KM.mn(6,3)=-1e15;
       }
       //This transforms the local stiffness to global coord sys
        Kmt.clear(); tKmt.clear();
        Kmt=KM*t;tKmt=tt*Kmt;
        //The steering vector
        *V.nn(1)=-1;*V.nn(2)=-1;*V.nn(3)=-1;
        *V.nn(4)=pNode->dof[0];*V.nn(5)=pNode->dof[1];*V.nn(6)=pNode->dof[2];
        //Put into global KM array
        formkv(KMA,tKmt,V,neq);
       // do sae for rotational restrains
       KM*=0;
	     if (pR->REST[3]==TRUE)  //X
       {
         *KM.mn(1,1)=1e15;*KM.mn(4,4)=1e15;*KM.mn(1,4)=-1e15;*KM.mn(4,1)=-1e15;
       }
       if (pR->REST[4]==TRUE)  //Y
       {
         *KM.mn(2,2)=1e15;*KM.mn(5,5)=1e15;*KM.mn(2,5)=-1e15;*KM.mn(5,2)=-1e15;
       }
       if (pR->REST[5]==TRUE)  //Z
       {
         *KM.mn(3,3)=1e15;*KM.mn(6,6)=1e15;*KM.mn(3,6)=-1e15;*KM.mn(6,3)=-1e15;
       }
       //This transforms the local stiffness to global coord sys
        Kmt.clear(); tKmt.clear();
        Kmt=KM*t;tKmt=tt*Kmt;
        //The steering vector
        *V.nn(1)=-1;*V.nn(2)=-1;*V.nn(3)=-1;
        *V.nn(4)=pNode->dof[3];*V.nn(5)=pNode->dof[4];*V.nn(6)=pNode->dof[5];
        //Put into global KM array
        formkv(KMA,tKmt,V,neq);
     }
   }
  }
}
}

//Count the number retraint element need before KM can be allocated
int ME_Object::LocalResCount()
{
int iCnt=0;
int k;
for (k=0;k<iBCLDs;k++)
{
  if (pBCLDs[k]->iObjType==322)
  {
   Restraint* pR=(Restraint*) pBCLDs[k];
   Pt_Object* pNode=(Pt_Object*) pR->pObj;
   if (pNode->OutSys!=0)
   {
     iCnt++;
     iCnt++;
   }
  }
}
return (iCnt);
}



//This version for iterative solver where no
//syatem matrix (KM) exists
void ME_Object::LocalResIter(Restraint* pR,Vec<int> &SteerT,Mat &KMAT,Vec<int> &SteerB,Mat &KMAB)
{
int i,j;
C3dVector K;
C3dMatrix r;
CoordSys* pS;
Vec<int> V(2*3);

Mat KM(6,6);
Mat t(6,6);
Mat tt(6,6);
Mat Kmt;
Mat tKmt;


Pt_Object* pNode=(Pt_Object*) pR->pObj;
if (pNode->OutSys!=0)
{
 pS=this->GetSys(pNode->OutSys);
 if (pS!=NULL)
 {
   //This composes the stiffness tranformation matrix
   r=GetNodalSys(pNode);
   r.Transpose();
   Mat mr(3,3);
   *mr.mn(1,1)=r.m_00; *mr.mn(1,2)=r.m_01; *mr.mn(1,3)=r.m_02;
   *mr.mn(2,1)=r.m_10; *mr.mn(2,2)=r.m_11; *mr.mn(2,3)=r.m_12;
   *mr.mn(3,1)=r.m_20; *mr.mn(3,2)=r.m_21; *mr.mn(3,3)=r.m_22;
   for (i=1;i<4;i++)
   {
      for (j=1;j<4;j++)
      {
         *t.mn(i,j) = *mr.mn(i,j);
         *t.mn(i+3,j+3) = *mr.mn(i,j);
         *tt.mn(i,j) = *mr.mn(i,j);
         *tt.mn(i+3,j+3) = *mr.mn(i,j);
      }
   }
   tt.Transpose();
   //This composes the local stiffness matrix
   KM*=0;
   if (pR->REST[0]==TRUE)  //X
   {
     *KM.mn(1,1)=1e13;*KM.mn(4,4)=1e13;*KM.mn(1,4)=-1e13;*KM.mn(4,1)=-1e13;
   }
   if (pR->REST[1]==TRUE)  //Y
   {
     *KM.mn(2,2)=1e13;*KM.mn(5,5)=1e13;*KM.mn(2,5)=-1e13;*KM.mn(5,2)=-1e13;
   }
   if (pR->REST[2]==TRUE)  //Z
   {
     *KM.mn(3,3)=1e13;*KM.mn(6,6)=1e13;*KM.mn(3,6)=-1e13;*KM.mn(6,3)=-1e13;
   }
   //This transforms the local stiffness to global coord sys
    Kmt.clear(); tKmt.clear();
    Kmt=KM*t;tKmt=tt*Kmt;
    //The steering vector
    *V.nn(1)=-1;*V.nn(2)=-1;*V.nn(3)=-1;
    *V.nn(4)=pNode->dof[0];*V.nn(5)=pNode->dof[1];*V.nn(6)=pNode->dof[2];
    SteerT=V;
    KMAT=tKmt;
    //Put into global KM array
    //formkv(KMA,tKmt,V,neq);
   // do same for rotational restrains

    
   KM*=0;
   if (pR->REST[3]==TRUE)  //X
   {
     *KM.mn(1,1)=1e13;*KM.mn(4,4)=1e13;*KM.mn(1,4)=-1e13;*KM.mn(4,1)=-1e13;
   }
   if (pR->REST[4]==TRUE)  //Y
   {
     *KM.mn(2,2)=1e13;*KM.mn(5,5)=1e13;*KM.mn(2,5)=-1e13;*KM.mn(5,2)=-1e13;
   }
   if (pR->REST[5]==TRUE)  //Z
   {
     *KM.mn(3,3)=1e13;*KM.mn(6,6)=1e13;*KM.mn(3,6)=-1e13;*KM.mn(6,3)=-1e13;
   }
   //This transforms the local stiffness to global coord sys
    Kmt.clear(); tKmt.clear();
    Kmt=KM*t;tKmt=tt*Kmt;
    //The steering vector
    *V.nn(1)=-1;*V.nn(2)=-1;*V.nn(3)=-1;
    *V.nn(4)=pNode->dof[3];*V.nn(5)=pNode->dof[4];*V.nn(6)=pNode->dof[5];
    SteerB=V;
    KMAB=tKmt;
 }
}
}

void ME_Object::GenLocalResraints(Mat* KM,Vec<int> *G, int &iELCnt)
{
  //THIS GENERATES THE SPRING TO HANGLE LOCAL RETRAINT DIRECTION NOT GLOBAL
  int i;
  Mat KResT(6,6);
  Vec<int> SResT(6);
  Mat KResB(6,6);
  Vec<int> SResB(6);
  for (i=0;i<iBCLDs;i++)
  {
    if (pBCLDs[i]->iObjType==322)
    {
      Restraint* pR=(Restraint*) pBCLDs[i];
      Pt_Object* pNode=(Pt_Object*) pR->pObj;
      if (pNode->OutSys!=0)
      {
        LocalResIter(pR,SResT,KResT,SResB,KResB);
        KM[iELCnt]=KResT;
        G[iELCnt]=SResT;
        iELCnt++;
        KM[iELCnt]=KResB;
        G[iELCnt]=SResB;
        iELCnt++;
      }
    }
  }
}


int ME_Object::GenDofs()
{

int i,j;
int iDof=1;
int mdof;
FILE* pFile;
pFile = fopen("GENDOF_DIAG.txt","w");

fprintf(pFile,"%s%i\n","NO of Nodes ",iNdNo);
for(i=0;i<iNdNo;i++)
{
  mdof=MaxDof(pNodes[i]);
  for (j=0;j<mdof;j++)
  {
    if (pNodes[i]->dof[j]!=-1)
    {
      fprintf(pFile,"%s %i %s %i\n","Node ",pNodes[i]->iLabel,"DOF",j);
      pNodes[i]->dof[j]=iDof;
      iDof++;
    }
  }
  for (j=mdof;j<6;j++)
  {
    pNodes[i]->dof[j]=0;
  }
}
fprintf(pFile,"%s%i\n","iDof ",iDof);
fclose(pFile);
return(iDof-1);
}

int ME_Object::GenDofs1D(int iD)
{

int i;
int iDof=iD;
FILE* pFile;
pFile = fopen("GENDOF_DIAG.txt","w");
fprintf(pFile,"%s%i\n","NO of Nodes ",iNdNo);
for(i=0;i<iNdNo;i++)
{
  if (pNodes[i]->dof[0]==0)
  {
    fprintf(pFile,"%s %i %s %i\n","Node ",pNodes[i]->iLabel,"DOF",0);
    pNodes[i]->dof[0]=iDof;
    iDof++;
  }
}
fprintf(pFile,"%s%i\n","iDof ",iDof);
fclose(pFile);
return(iDof-1);
}

int ME_Object::GenDofs3D(int iD)
{

	int i;
	int iDof = iD;
	FILE* pFile;
	pFile = fopen("GENDOF.txt", "w");
	fprintf(pFile, "%s%i\n", "NO of Nodes ", iNdNo);
	for (i = 0; i < iNdNo; i++)
	{
		if (pNodes[i]->dof[0] == 0)
		{
			fprintf(pFile, "%s %i %s %i\n", "Node ", pNodes[i]->iLabel, "DOF", iDof);
			pNodes[i]->dof[0] = iDof;
			iDof++;
		}
		if (pNodes[i]->dof[1] == 0)
		{
			fprintf(pFile, "%s %i %s %i\n", "Node ", pNodes[i]->iLabel, "DOF", iDof);
			pNodes[i]->dof[1] = iDof;
			iDof++;
		}
		if (pNodes[i]->dof[2] == 0)
		{
			fprintf(pFile, "%s %i %s %i\n", "Node ", pNodes[i]->iLabel, "DOF", iDof);
			pNodes[i]->dof[2] = iDof;
			iDof++;
		}
	}
	fprintf(pFile, "%s%i\n", "iDof ", iDof);
	fclose(pFile);
	return(iDof - 1);
}


C3dMatrix ME_Object::GetNodalSys(Pt_Object* pN)
{
C3dMatrix mRC;
C3dMatrix mInvRC;
C3dVector pt;
C3dVector pO;
CoordSys* pC;
double ang;

  pC=GetSys(pN->OutSys);
  if (pC!=NULL)
  {
    mRC=pC->GetTMat();
    if (pC->CysType==2)
    {
      pt=pN->GetCoords();
      pO=pC->Get_Centroid();
      pt-=pO;
      mRC.Transpose();
      pt=mRC*pt;
      ang=atan2(pt.y,pt.x)*180/Pi;
      mInvRC.MakeUnit();
      mInvRC.Rotate(0,0,ang);
      mRC.Transpose();
      mRC*=mInvRC;
      mInvRC=mRC;
    }
    else if (pC->CysType==1)
    {
      mInvRC=mRC;
    }
    else
    {
      outtext1("ERROR: Spherical Coordinate Sys Not Supported.");
    }
  }
  else
  {
    outtext1("ERROR: Coordinateord Sys Not Found.");
  }

return (mInvRC);
}




BOOL ME_Object::DeleteNd(Pt_Object* pN)
{
BOOL b1=FALSE;
BOOL brc=FALSE;
int i;
b1 = NodeInBCSet(pN);
if (b1==FALSE)
{
  b1=NodeInEl(pN);
}
if (b1==FALSE)
{ 
  for(i=0;i<iNdNo;i++)
  {
    if (pNodes[i]==pN)
    {
      delete(pNodes[i]);
      pNodes[i]=pNodes[iNdNo-1];
      pNodes[iNdNo-1]=NULL;
      iNdNo--;
	    brc=TRUE;
      break;
    }
  }
}
return (brc);
}

BOOL ME_Object::CanDeleteEl(E_Object* pEl)
{
  BOOL brc = FALSE;
  BOOL b1;

  b1 = ElemInBCSet(pEl);
  if (b1 == FALSE)
  {
    brc=TRUE;
  }
  return (brc);
}


BOOL ME_Object::DeleteEl(E_Object* pEl)
{
int i;
BOOL brc=FALSE;
if (CanDeleteEl(pEl)==TRUE)
{
  for (i = 0; i < iElNo; i++)
  {
    if (pElems[i] == pEl)
    {
      delete(pElems[i]);
      pElems[i] = pElems[iElNo - 1];
      pElems[iElNo - 1] = NULL;
      iElNo--;
      brc = TRUE;
      break;
    }
  }
}
return (brc);
}

BOOL ME_Object::DeleteCys(CoordSys* pS)
{
int i;
BOOL brc=FALSE;
for(i=0;i<iCYS;i++)
{
  if ((pSys[i]==pS) && (pS!=NULL))
  {
   //need to check no body loads 
   //on element
   delete(pSys[i]);
   pSys[i]=pSys[iCYS-1];
   pSys[iCYS-1]=NULL;
   iCYS--;
   brc=TRUE;
   break;
  }
}
return (brc);
}

BOOL ME_Object::DeleteBC(BCLD* pS)
{

  BOOL brc=TRUE;
  if (iCurLC!=-1)
  {
    cLinkedList* pC=LCS[iCurLC];
    pC->Remove(pS);
  }
  if (iCurBC!=-1)
  {
    cLinkedList* pC=BCS[iCurBC];
    pC->Remove(pS);
  }  
  if (iCurTSet!=-1)
  {
    cLinkedList* pC=TSETS[iCurTSet];
    pC->Remove(pS);
  } 
return (brc);
}

//Check to see if a node is used in any boundary set.
//if so we can delete if
BOOL ME_Object::NodeInBCSet(Pt_Object* pN)
{
int i;
BOOL brc=FALSE;
G_Object* pC;
BCLD* pBC;
//Check all load sets for dependancy on pN
for (i = 0; i < iNoLCs; i++)
{
  if (!brc)
  {
    pC = LCS[i]->Head;
    while (pC != NULL)
    {
      pBC = (BCLD*)pC;
      if (pN == pBC->pObj)
      {
        brc = TRUE;
        break;
      }
      pC = (cLink*)pC->next;
    }
  }
}
for (i = 0; i < iNoBCs; i++)
{
  if (!brc)
  {
    pC = BCS[i]->Head;
    while (pC != NULL)
    {
      pBC = (BCLD*) pC;
      if (pN == pBC->pObj)
      {
        brc = TRUE;
        break;
      }
      pC = (cLink*)pC->next;
    }
  }
}

return (brc);
}

//Check to see if a node is used in any boundary set.
//if so we can delete if
BOOL ME_Object::ElemInBCSet(E_Object* pE)
{
  int i;
  BOOL brc = FALSE;
  G_Object* pC;
  BCLD* pBC;
  //Check all load sets for dependancy on pN
  for (i = 0; i < iNoLCs; i++)
  {
    if (!brc)
    {
      pC = LCS[i]->Head;
      while (pC != NULL)
      {
        pBC = (BCLD*)pC;
        if (pE == pBC->pObj)
        {
          brc = TRUE;
          break;
        }
        pC = (cLink*)pC->next;
      }
    }
  }
  for (i = 0; i < iNoTSets; i++)
  {
    if (!brc)
    {
      pC = TSETS[i]->Head;
      while (pC != NULL)
      {
        pBC = (BCLD*)pC;
        if (pE == pBC->pObj)
        {
          brc = TRUE;
          break;
        }
        pC = (cLink*)pC->next;
      }
    }
  }

  return (brc);
}

void ME_Object::GetClosestNodes(C3dVector pTrg,ObjList* pRes,double dTol)
{
  int i;
  double dDist;
  C3dVector pN;
  pRes->Clear();
  for (i=0;i<iNdNo;i++)
  {
     pN=pNodes[i]->GetCoords();
     dDist=pN.Dist(pTrg);
     if (dDist<dTol)
       pRes->Add(pNodes[i]);
  }
}


Pt_Object* ME_Object::GetClosestNode(Pt_Object* pIn,double* dMinDist)
{
int i;
double dDist=1e36;
*dMinDist=1e36;

Pt_Object* pRet=NULL;
C3dVector pT;
C3dVector pS=pIn->GetCoords();
for (i=0;i<iNdNo;i++)
{
   
   pT=pNodes[i]->GetCoords();
   pT-=pS;
   dDist=pT.Mag();
   if (pNodes[i]!=pIn)
   {
     if (dDist<*dMinDist)
     {
        *dMinDist=dDist;
        pRet=pNodes[i];
     }
   }
}
return (pRet);
}

Pt_Object* ME_Object::GetClosestNode2(C3dVector pIn, double &dMinDist)
{
	int i;
	double dDist = 1e36;
	dMinDist = 1e36;

	Pt_Object* pRet = NULL;
	C3dVector pT;
	C3dVector pS = pIn;
	for (i = 0; i < iNdNo; i++)
	{

		pT = pNodes[i]->GetCoords();
		pT -= pS;
		dDist = pT.Mag();
		if (dDist < dMinDist)
		{
			dMinDist = dDist;
			pRet = pNodes[i];
		}
	}
	return (pRet);
}


void ME_Object::CNodesMerge(double dTol)
{
Pt_Object* pWith=NULL;
double dDist;
int i;
int j;

for (i=0;i<iNdNo;i++)
{
   if (pNodes[i]->iColour==160)
   {
     if ((pNodes[i]->iLabel==1059) || (pNodes[i]->iLabel==1079))
     {
       dDist=dDist;
     }

     pWith=GetClosestNode(pNodes[i], &dDist);
     if ((dDist<dTol) && (pWith!=NULL))
     {
       for (j=0;j<iElNo;j++)
       {
         pElems[j]->RepNodeInEl(pNodes[i], pWith);
       }
      // DeleteNd(pNodes[i]);
     }
   }
}
}


void ME_Object::RepNodeInEl(Pt_Object* pThis,Pt_Object* pWith)
{
  int j;
  if (pThis!=pWith)
  {
    for (j=0;j<iElNo;j++)
    {
       pElems[j]->RepNodeInEl(pThis, pWith);
    }
  }
}


ObjList* ME_Object::CNodesMerge2(double dTol,BOOL UpLab,BOOL bDel)
{
Pt_Object* pWith=NULL;
ObjList* DelNode=new ObjList();
double dDist;
int i;
int j;

for (i=0;i<iNdNo;i++)
{
     pWith=GetClosestNode(pNodes[i], &dDist);
     if ((dDist<dTol) && (pWith!=NULL))
     {
       if ((pWith->iLabel>pNodes[i]->iLabel) && (UpLab==TRUE))
       {
         for (j=0;j<iElNo;j++)
         {
           pElems[j]->RepNodeInEl(pNodes[i], pWith);
         }
         if (bDel==TRUE)
         DelNode->AddEx(pNodes[i]);
       }
       else
       {
         for (j=0;j<iElNo;j++)
         {
           pElems[j]->RepNodeInEl(pWith,pNodes[i]);
         }
         if (bDel==TRUE)
          DelNode->AddEx(pWith);
       }
         
     }
}
return (DelNode);
}

BOOL ME_Object::NodeInEl(Pt_Object* pN)
{
int i;
BOOL brc=FALSE;
for(i=0;i<iElNo;i++)
{
  if (pElems[i]->NodeInEl(pN)==TRUE)
  {
	brc=TRUE;
    break;
  }
}
return (brc);
}


int ME_Object::MaxDof(Pt_Object* pN)
{
int i;
int id=0;
int imax=0;
for(i=0;i<iElNo;i++)
{
  if (pElems[i]->NodeInEl(pN)==TRUE)
  {
    id=pElems[i]->noDof();
    if (id>imax)
	  {
	    imax=id;
    }
  }
}
return (imax);
}

void ME_Object::UpdatePropRef(PropTable* pT)
{
int i;
Property* p;
for (i=0;i<iElNo;i++)
{
if (pElems[i]->iLabel==68700267)
  i=i;
  p=pT->GetItem(pElems[i]->PID);
  if (p!=NULL)
  {
    pElems[i]->SetProperty(p);
  }
  else
  {
     //sprintf_s(S1,"%s%i","Property not found PID",pElems[i]->PID);
     //outtext1(S1); 
  }
}
}  

//Elplicitly add an element directly
void ME_Object::AddElEx(E_Object* pEl)
{
	pElems[iElNo] = pEl;
	iElNo++;
}

E_Object* ME_Object::AddEl2(int pVnode[200], int iLab,int iCol,int iType,int iPID,int iMat, int iNoNodes,int A,int B,int C,int iMatCys,double dMatAng)
{
  int iCnt;
  E_Object* cAddedEl;
  Pt_Object *pENodes[100];
  if (TempList!=NULL)
  {
    for (iCnt = 0; iCnt < iNoNodes; iCnt ++)
    {
      pENodes[iCnt] =(Pt_Object*) TempList->Objs[pVnode[iCnt]];
    }
  }
  else
  {
    for (iCnt = 0; iCnt < iNoNodes; iCnt ++)
    {
      pENodes[iCnt] = GetNode(pVnode[iCnt]);
    }
  }
  if (iLab>iElementLab)
  {
    iElementLab=iLab;
  }
  cAddedEl = AddEl(pENodes,iLab,iCol,iType,iPID,iMat,iNoNodes,A,B,C,FALSE,iMatCys,dMatAng); 
  return (cAddedEl);
}

void ME_Object::MaxLab()
{
int iCnt;
iNodeLab=0;
iElementLab=0;
iCYSLab=0;
for (iCnt = 0; iCnt < iNdNo; iCnt ++)
{
   if (pNodes[iCnt]->iLabel>iNodeLab)
   {
     iNodeLab=pNodes[iCnt]->iLabel;
   }
}
for (iCnt = 0; iCnt < iElNo; iCnt ++)
{
   if (pElems[iCnt]->iLabel>iElementLab)
   {
     iElementLab=pElems[iCnt]->iLabel;
   }
}
for (iCnt = 0; iCnt < iCYS; iCnt ++)
{
   if (pSys[iCnt]->iLabel>iCYSLab)
   {
     iCYSLab=pSys[iCnt]->iLabel;
   }
}
iNodeLab++;
iElementLab++;
iCYSLab++;
}

// pInVertex[200]	Nodes
// int iLab			Element Label
// int iCol			Element Colour
// int iType		Element Type
// int iPID			Element Property ID
// int iMat			
// int iNo			No of Nodes
// int iA
// int iB
// int iC
// BOOL AddDisp
// int iMatCys
// double dMatAng)
E_Object* ME_Object::AddEl(Pt_Object* pInVertex[200],int iLab,int iCol,int iType,int iPID,int iMat,int iNo,int iA,int iB,int iC,BOOL AddDisp,int iMatCys,double dMatAng)
{
E_Object* pERet=NULL;

if (iElNo<MAX_FESIZE)
{
if (iType == 115)
  {
  E_Object38 *pEL8 = new E_Object38();
  pEL8->Create(pInVertex, iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  if (pEL8->ChkNegJac()==TRUE)
  {
     pEL8->Reverse();
     outtext1("ERROR: reversing.");
  }
  pElems[iElNo] = pEL8;
  pERet= pEL8;
  iElNo++;
  }
else if (iType == 112)
  {
  E_Object36 *pEL6 = new E_Object36();
  pEL6->Create(pInVertex, iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  if (pEL6->ChkNegJac()==TRUE)
  {
     pEL6->Reverse();
	   outtext1("ERROR: Reversing.");
  }
  pElems[iElNo] = pEL6;
  pERet= pEL6;
  iElNo++;
  }
else if (iType == 94)
  {
  E_Object4 *pEL4 = new E_Object4();
  pEL4->Create(pInVertex, iLab,iCol,iType,iPID,iMat,iNo,iMatCys,dMatAng,this,NULL);
  pElems[iElNo] = pEL4;
  pERet= pEL4;
  iElNo++;
  }
else if (iType == 91)
  {
  E_Object3 *pEL3 = new E_Object3();
  pEL3->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,iMatCys,dMatAng,this,NULL);
  pElems[iElNo] = pEL3;
  pERet= pEL3;
  //AddElEdges(pElems[iElNo]);
  iElNo++;
  }
else if (iType == 161)
  {
  E_Object1 *pEL1 = new E_Object1();
  pEL1->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pElems[iElNo] = pEL1;
  pERet= pEL1;
  iElNo++;
  }
else if ((iType == 136) || (iType == 137))
  {
  E_Object2 *pEL2 = new E_Object2();
  pEL2->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pElems[iElNo] = pEL2;
  pERet= pEL2;
  iElNo++;
  }
else if (iType == 121) 
  {
  E_ObjectR2 *pEL2 = new E_ObjectR2();
  pEL2->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pElems[iElNo] = pEL2;
  pERet= pEL2;
  iElNo++;
  }
else if ((iType == 21) || (iType == 22))
  {
  E_Object2B *pEL2 = new E_Object2B();
  pEL2->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pEL2->SetSec(iA,iB,iC);
  pElems[iElNo] = pEL2;
  pERet= pEL2;
  iElNo++;
  }
else if (iType == 11)
  {
  E_Object2R *pEL2 = new E_Object2R();
  pEL2->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pEL2->SetSec(iA,iB,iC);
  pElems[iElNo] = pEL2;
  pERet= pEL2;
  iElNo++;
  }
else if (iType == 111)
  {
  E_Object34 *pEL34 = new E_Object34();
  pEL34->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  if (pEL34->ChkNegJac()==TRUE)
  {
     pEL34->Reverse();
	   outtext1("ERROR: Reversing.");
  }
  pElems[iElNo] = pEL34;
  pERet= pEL34;
  iElNo++;
  }
else if (iType == 122)
  {
  E_ObjectR *pELR = new E_ObjectR();
  pELR->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pElems[iElNo] = pELR;
  pERet= pELR;
  iElNo++;
  }
else if (iType == 1000)
  {
  E_CellS* pELR = new E_CellS();
  pELR->Create(pInVertex,iLab,iCol,iType,iPID,iMat,iNo,this,NULL);
  pElems[iElNo] = pELR;
  pERet= pELR;
  iElNo++;
  }
}
else
{
  outtext1("ERROR: Max ELMAX SIZE");
}



return (pERet);
}



int ME_Object::NodeToGlobal(C3dVector &vRet,int iDef)
{
int iRet;
iRet=0;
  CoordSys* Cys;
  //int CysID;
  if (iDef!=0)
  {
    Cys=GetSys(iDef);
    if (Cys == NULL)
    {
      outtext1("ERROR: Coordinate Sys Not Found.");
    }
    else
    {
      C3dMatrix A = Cys->mOrientMat;
	  iRet=Cys->RID;
      if (Cys->CysType==1)
      {
	    vRet = A*vRet;
        vRet+=Cys->Origin;
      }
      else if (Cys->CysType==2)
      {
        C3dVector pCart;
        pCart.x=vRet.x*cos(D2R*vRet.y);
        pCart.y=vRet.x*sin(D2R*vRet.y);
        pCart.z=vRet.z;
        pCart = A*pCart;
        pCart+=Cys->Origin;
        vRet=pCart;
      }
      else if (Cys->CysType==3)
      {
        C3dVector pCart;
        pCart.x=vRet.x*sin(D2R*vRet.y)*cos(D2R*vRet.z);
        pCart.y=vRet.x*sin(D2R*vRet.y)*sin(D2R*vRet.z);
        pCart.z=vRet.x*cos(D2R*vRet.y);
        pCart = A*pCart;
        pCart+=Cys->Origin;
        vRet=pCart;
      }
    }
  }

return (iRet);
}

//We Need Theta in the Cylindrical coords Sys before
//we can convert results from cylindrical output to global
C3dVector ME_Object::CartToCylCYS(CoordSys* pCy, C3dVector pP)
{
	C3dVector v1moved;
	C3dVector v2Glob;
	C3dMatrix mTran;
	C3dMatrix mInv;
	C3dVector vCyl;
	C3dVector vCent;
	mTran = pCy->GetTMat();
	vCent = pCy->Get_Centroid();
	v1moved = pP;
	v1moved.x = v1moved.x - vCent.x;
	v1moved.y = v1moved.y - vCent.y;
	v1moved.z = v1moved.z - vCent.z;
	mTran.m_30 = 0;
	mTran.m_31 = 0;
	mTran.m_32 = 0;
	mInv = mTran.Inv();
	v2Glob = mInv * v1moved;
	vCyl.x = pow(v2Glob.x * v2Glob.x + v2Glob.y * v2Glob.y, 0.5);
	vCyl.y = atan2(v2Glob.y, v2Glob.x) * 180 / Pi;
	vCyl.z = v2Glob.z;
	return (vCyl);
}

int ME_Object::VecToGlobal(Pt_Object* pN, C3dVector &vRet,int iDef)
{
int iRet;
int iLab;
iRet=0;
  CoordSys* Cys;
  //int CysID;
  if ((pN->iLabel == 162) || (pN->iLabel == 163))
	  iLab = pN->iLabel;
  if (iDef!=0)
  {
    Cys=GetSys(iDef);
    if (Cys == NULL)
    {
      outtext1("ERROR: Coordiinate Sys Not Found.");
    }
    else
    {

      C3dMatrix A = Cys->mOrientMat;
	  iRet=Cys->RID;
      if (Cys->CysType==1)
      {

	    vRet = A*vRet;
      }
      else if (Cys->CysType==2)
      {
		C3dVector vRG;
		vRG = CartToCylCYS(Cys, pN->Get_Centroid());
        C3dVector pCart;
		pCart.x = vRet.x*cos(D2R*(vRG.y)) - vRet.y*sin(D2R*(vRG.y));
		pCart.y = vRet.x*sin(D2R*(vRG.y)) + vRet.y*cos(D2R*(vRG.y));
		pCart.z = vRet.z;
        pCart = A*pCart;
        vRet=pCart;
      }
      else if (Cys->CysType==3)
      {
        C3dVector pCart;
        pCart.x=vRet.x*sin(D2R*vRet.y)*cos(D2R*vRet.z);
        pCart.y=vRet.x*sin(D2R*vRet.y)*sin(D2R*vRet.z);
        pCart.z=vRet.x*cos(D2R*vRet.y);
        pCart = A*pCart;
        vRet=pCart;
      }
    }
  }

return (iRet);
}

Pt_Object* ME_Object::AddNode(C3dVector InPt, int iLab,int i2,int i3, int iC,int iDef,int iOut)
{
int iRID;
iRID=iDef;
if (iNdNo<MAX_FESIZE)
{
  do
  {
    iRID=NodeToGlobal(InPt,iRID);
  }
  while (iRID>0);
  pNodes[iNdNo] = new Pt_Object;
  pNodes[iNdNo]->Create(InPt,iLab,i2,i3,iC,iDef,iOut,this);
  if (TempList!=NULL)
  {
    TempList->Objs[iLab]=pNodes[iNdNo];
  }
  if (iLab>iNodeLab)
  {
    iNodeLab=iLab;
  }
  iNdNo++;
}
else
{
  outtext1("ERROR: Max Node Limit Exceeded.");
}
return (pNodes[iNdNo-1]);
}

CoordSys* ME_Object::AddSys(C3dVector Orig,C3dMatrix RMat,int iRID,int iTp, int iLab, int iC)
{
if (iCYS<MAX_FESIZE)
{
  pSys[iCYS] = new CoordSys;
  pSys[iCYS]->Create(Orig,RMat,iRID,iTp,iLab,iC,this);
  if (iLab>iCYSLab)
  {
    iCYSLab=iLab;
  }
  iCYS++;
}
else
{
  outtext1("ERROR: Max No of Coordinate Sys Exceeded.");
}

return (pSys[iCYS-1]);
}




CoordSys* ME_Object::GetSys(int iLab)
{
int i;
CoordSys* oRet=NULL;
if ((iLab > 0) && (iCYS>0))
{
  for (i=0;i<iCYS;i++)
  {
    if (pSys[i]->iLabel == iLab)
	  {
      oRet = pSys[i];
	    break;
	  }
  }
}
return (oRet);
}


Pt_Object* ME_Object::GetNode(int iRLab)
{
int iCnt;
Pt_Object *pRetPt = NULL;
if ((TempList!=NULL) && (iRLab<=99999999))
{
  pRetPt = (Pt_Object*) TempList->Objs[iRLab];
}
else
{
for (iCnt = 0; iCnt < iNdNo;iCnt++)
   {
    if (pNodes[iCnt]->iLabel == iRLab)  
	   {
	   pRetPt = pNodes[iCnt];
	   break;
	   } 
   }
}
return (pRetPt);
}

int ME_Object::GetNodeInd(Pt_Object* pThisNode)
{
int iCnt;

for (iCnt = 0; iCnt < iNdNo;iCnt++)
{
  if (pNodes[iCnt] == pThisNode)  
	{
	  break;
	} 
}
return (iCnt);
}



E_Object* ME_Object::GetElement(int iRLab)
{
int iCnt;
E_Object *pRetPt = NULL;

for (iCnt = 0; iCnt < iElNo;iCnt++)
   {
    if (pElems[iCnt]->iLabel == iRLab)  
	   {
	   pRetPt = pElems[iCnt];
	   break;
	   } 
   }

return (pRetPt);
}
	

// contour map
//float Texture17[30][3] =
//{
//{0.00f, 0.00f, 1.00f},    //151 Blue
//{0.00f, 0.33f, 1.00f},    //152 Grey Blue
//{0.00f, 0.66f, 1.00f},    //153 Light Blue
//{0.00f, 1.00f, 1.00f},    //154 Cyan
//{1.00f, 0.33f, 1.00f},    //163 Light Magenta
//{1.00f, 0.00f, 1.00f},    //162 Magenta
//{0.00f, 0.33f, 0.00f},    //155 Dark Olive
//{0.00f, 0.66f, 0.00f},    //156 Dark Green
//{0.00f, 1.00f, 0.00f},    //157 Green
//{1.00f, 1.00f, 0.00f},    //158 Yellow
//{1.00f, 0.66f, 0.00f},    //159 Golden Orange
//{1.00f, 0.33f, 0.00f},    //160 Orange
//{1.00f, 0.00f, 0.00f}};   //161 Red

float Texture17[30][3] =
{
{0.00f, 0.00f, 1.00f},    //151 Blue
{0.00f, 0.33f, 1.00f},    //152 Grey Blue
{0.00f, 0.66f, 1.00f},    //153 Light Blue
{0.00f, 1.00f, 0.66f},    //154 Cyan
{0.00f, 1.00f, 0.33f},    //163 Light Magenta
{0.33f, 1.00f, 0.00f},    //162 Magenta
{0.66f, 1.00f, 0.00f},    //155 Dark Olive
{1.00f, 0.90f, 0.00f},    //156 Dark Green
{1.00f, 0.50f, 0.00f},    //157 Green
{1.00f, 0.00f, 0.00f}};   //161 Red


float Texture18[30][3] =
{
{1.00f, 0.00f, 0.00f},    //161 Red
{1.00f, 0.50f, 0.00f},    //157 Green
{1.00f, 0.90f, 0.00f},    //156 Dark Green
{0.66f, 1.00f, 0.00f},    //155 Dark Olive
{0.33f, 1.00f, 0.00f},    //162 Magenta
{0.00f, 1.00f, 0.33f},    //163 Light Magenta
{0.00f, 1.00f, 0.66f},    //154 Cyan
{0.00f, 0.66f, 1.00f},    //153 Light Blue
{0.00f, 0.33f, 1.00f},    //152 Grey Blue
{0.00f, 0.00f, 1.00f}};   //151 Blue

void CreateTexture(BOOL bRev)
{
 //This bit does the results contour mapping
  // Set pixel stora   BOOL glTexCoord1f(TOrd[i]);ge mode 
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // Generate a texture name
  GLuint m_nTexName=NULL;
  glGenTextures(1, &m_nTexName);
// Create a texture object
  glBindTexture(GL_TEXTURE_1D, m_nTexName);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
  if (bRev)
    glTexImage1D(GL_TEXTURE_1D, 0, 3,10, 0, GL_RGB,GL_FLOAT, Texture18);
  else
    glTexImage1D(GL_TEXTURE_1D, 0, 3,10, 0, GL_RGB,GL_FLOAT, Texture17);
}


void DrawColBar(int iDspFlgs,double dW,double dH)
{

int iNoCols=13;
float YOrd[30];
float TOrd[30];
float VOrd[30];
float fCw;
fCw=(float) (0.1*dW);
int i;
float dHS;
float dWInc;
float fSpan;
float fInc;
float tCol;
fSpan=cBarMax-cBarMin;
fInc=fSpan/iNoCols;

dHS=(float) (2*dH/(iNoCols+2));
dWInc=(float) (0.3*dW);
for (i=0;i<iNoCols+1;i++)
{
  YOrd[i]=(float) (-dH+dHS+dHS*i);
  TOrd[i]=(float) (fInc*i/fSpan);
  VOrd[i]=(float) (cBarMin+fInc*i);
}
glColor3fv(cols[124]);
glEnable(GL_TEXTURE_1D);
for (i=0;i<iNoCols;i++)
{
  tCol=(TOrd[i]+TOrd[i+1])/2;
  glBegin(GL_POLYGON);
    glTexCoord1f(tCol);
    glNormal3f(0,0,1);
    glVertex3f((float) (dW-fCw-dWInc),(float) YOrd[i],(float) 100);
    glTexCoord1f(tCol);
    glNormal3f(0,0,1);
    glVertex3f((float) (dW-dWInc),(float) YOrd[i],(float) 100);
    glTexCoord1f(tCol);
    glNormal3f(0,0,1);
    glVertex3f((float) (dW-dWInc),(float) YOrd[i+1],(float) 100);
    glTexCoord1f(tCol);
    glNormal3f(0,0,1);
    glVertex3f((float) dW-fCw-dWInc, (float) YOrd[i+1],100);
  glEnd();
}
glDisable(GL_TEXTURE_1D);
char sLab[80];
for (i=0;i<iNoCols+1;i++)
{

       sprintf_s(sLab,"%g",VOrd[i]);
       OglString(iDspFlgs,dW-0.95*dWInc,YOrd[i],100,&sLab[0]);
}
}


void ME_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
int i;

	if (iElNo > 0)
	{
		for (i = 0; i < iElNo; i++)
		{
			pElems[i]->OglDraw(iDspFlgs, dS1, dS2);
		}
	}
	if ((iNdNo > 0) && (bDrawN == TRUE))
	{
		for (i = 0; i < iNdNo; i++)
		{
			pNodes[i]->OglDraw(iDspFlgs, dS1, dS2);
		}
	}
	if (iCurLC != -1)
	{
		LCS[iCurLC]->OglDraw(iDspFlgs, dS1, dS2);
	}
	if (iCurBC != -1)
	{
		BCS[iCurBC]->OglDraw(iDspFlgs, dS1, dS2);
	}
	if (iCurTSet != -1)
	{
		TSETS[iCurTSet]->OglDraw(iDspFlgs, dS1, dS2);
	}

	if (iCYS > 0)
	{
		for (i = 0; i < iCYS; i++)
		{
			pSys[i]->OglDraw(iDspFlgs, dS1, dS2);
		}
	}

//if (FcList!=NULL)
//  FcList->OglDraw(iDspFlgs,dS1,dS2);
}

void ME_Object::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int i;
char sLab[80];
    if (pResVectors != NULL)
      pResVectors->OglDrawW(iDspFlgs, dS1, dS2);
	if (iElNo > 0)
	{
		for (i = 0; i < iElNo; i++)
		{
			pElems[i]->OglDrawW(iDspFlgs, dS1, dS2);
		}
	}
	if ((iNdNo > 0) && (bDrawN == TRUE))
	{
		for (i = 0; i < iNdNo; i++)
		{
			pNodes[i]->OglDrawW(iDspFlgs, dS1, dS2);
		}
	}

	if (iCurLC != -1)
	{
		LCS[iCurLC]->OglDrawW(iDspFlgs, dS1, dS2);
	}
	if (iCurBC != -1)
	{
		BCS[iCurBC]->OglDrawW(iDspFlgs, dS1, dS2);
	}
	if (iCurTSet != -1)
	{
		TSETS[iCurTSet]->OglDrawW(iDspFlgs, dS1, dS2);
	}

	if (iBCLDs > 0)
	{
		for (i = 0; i < iBCLDs; i++)
		{
			pBCLDs[i]->OglDrawW(iDspFlgs, dS1, dS2);
		}
	}
	if (iCYS > 0)
	{
		for (i = 0; i < iCYS; i++)
		{
			pSys[i]->OglDraw(iDspFlgs, dS1, dS2);
		}
	}


	// New Results Vectors

	
	C3dVector vCent;
	vCent = Get_Centroid();
	if (bDrawLab == TRUE)
	{
		sprintf_s(sLab, "ME: %s", sName);
		OglString(iDspFlgs, vCent.x, vCent.y, vCent.z, sLab);
	}
}

void ME_Object::Draw(CDC* pDC,int iDrawmode)
{
int i;
if (iDrawmode != 2)
{
if ((iNdNo>0) && (bDrawN==TRUE))
  {
    for (i = 0; i < iNdNo; i++)
    {
      pNodes[i]->Draw(pDC,iDrawmode);
    }
  }
  if (iElNo > 0)
  {
    for (i = 0; i < iElNo; i++)
    {
    pElems[i]->Draw(pDC,iDrawmode);	
    }
  }
  if (iCurLC!=-1)
  {
    LCS[iCurLC]->Draw(pDC,iDrawmode);	
  }
  if (iCurBC!=-1)
  {
    BCS[iCurBC]->Draw(pDC,iDrawmode);	
  }
  if (iCurTSet!=-1)
  {
    TSETS[iCurTSet]->Draw(pDC,iDrawmode);	
  }

  if (iBCLDs > 0)
  {
    for (i = 0; i < iBCLDs; i++)
    {
    pBCLDs[i]->Draw(pDC,iDrawmode);	
    }
  }
  if (iCYS > 0)
  {
    for (i = 0; i < iCYS; i++)
    {
      pSys[i]->Draw(pDC,iDrawmode);	
    }
  }
}
}

void ME_Object::Colour(int iCol)
{
int i;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    pNodes[i]->Colour(iCol);	
  }
}

if (iElNo > 0)
{
  for (i = 0; i < iElNo; i++)
  {
    pElems[i]->Colour(iCol);	
  }
}

if (iBCLDs > 0)
{
  for (i = 0; i < iBCLDs; i++)
  {
    pBCLDs[i]->Colour(iCol);	
  }
}
if (iCYS > 0)
{
for (i = 0; i < iCYS; i++)
  {
     pSys[i]->Colour(iCol);	
  }
}
}


void ME_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int i;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    pNodes[i]->SetToScr(pModMat, pScrTran);
  }
}

if (iElNo > 0)
{
  for (i = 0; i < iElNo; i++)
  {
    pElems[i]->SetToScr(pModMat, pScrTran);
  }
}

if (iCurLC !=-1)
{
    LCS[iCurLC]->SetToScr(pModMat, pScrTran);
}
if (iCurBC !=-1)
{
    BCS[iCurBC]->SetToScr(pModMat, pScrTran);
}
if (iCurTSet !=-1)
{
    TSETS[iCurTSet]->SetToScr(pModMat, pScrTran);
}


if (iBCLDs > 0)
{
  for (i = 0; i < iBCLDs; i++)
  {
    pBCLDs[i]->SetToScr(pModMat, pScrTran);
  }
}

if (pResVectors != NULL)
  pResVectors->SetToScr(pModMat, pScrTran);


if (iCYS > 0)
{
for (i = 0; i < iCYS; i++)
  {
     pSys[i]->SetToScr(pModMat, pScrTran);	
  }
}
}


void ME_Object::Transform(C3dMatrix TMat)
{
int i;
TransMat=TMat;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    pNodes[i]->Transform(TMat);
  }
}
if (iElNo > 0)
{
  for (i = 0; i < iElNo; i++)
  {
    pElems[i]->Transform(TMat);
  }
}
}


void ME_Object::Translate (C3dVector vIn)
{
int i;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    pNodes[i]->Translate(vIn);
  }
}

if (iCYS > 0)
{
  for (i = 0; i < iCYS; i++)
  {
    pSys[i]->Translate(vIn);
  }
}
}

void ME_Object::HighLight(CDC* pDC)
{
int i;
if (iElNo > 0)
  {
  for (i = 0; i < iElNo; i++)
    {
    pElems[i]->Draw(pDC,4);	
    }
  }
}


int ME_Object::GetMeshYExt()
{
int irc=-1;
int i;
int iYMin;
int iYMax;

if (iNdNo > 1)
{
  iYMin=(int) pNodes[0]->DSP_Point->y;
  iYMax=iYMin;
  for (i = 0; i < iNdNo; i++)
  {
    if (pNodes[i]->DSP_Point->y>iYMax)
	  iYMax=pNodes[i]->DSP_Point->y;
    if (pNodes[i]->DSP_Point->y<iYMin)
	  iYMin=pNodes[i]->DSP_Point->y;
  }
  irc=iYMax-iYMin;
}
return (irc);
}

C3dVector ME_Object::Get_Centroid()
{
C3dVector vRet;
vRet.Set(0,0,0);
int i;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    vRet+=pNodes[i]->Get_Centroid();
  }
  vRet.x=vRet.x/iNdNo;
  vRet.y=vRet.y/iNdNo;
  vRet.z=vRet.z/iNdNo;
}
return (vRet);
}

G_ObjectD ME_Object::SelDist(CPoint InPT,Filter FIL)
{
G_Object* pSel=NULL;
G_Object* pMinSel=NULL;
G_ObjectD Ret;
int i;
double dDist = 1e36;
Ret.pObj=NULL;
Ret.Dist=dDist;
//search the node regardless of
//filter so we can  get a dist
//for the mesh
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    if (pNodes[i]->isSelectable()==1)
	{
	  Ret = pNodes[i]->SelDist(InPT,FIL);
      if (Ret.Dist < dDist)
	  {
	    pMinSel=Ret.pObj;
	    dDist=Ret.Dist;
	  }
	}
  }
}


if (FIL.isFilter(3))
{
if (iElNo > 0)
{
  for (i = 0; i < iElNo; i++)
  {
	if (pElems[i]->isSelectable()==1)
	{
	  Ret = pElems[i]->SelDist(InPT,FIL);
      if (Ret.Dist < dDist)
	  {
	    pMinSel=Ret.pObj;
	    dDist=Ret.Dist;
	  }
	}
  }
}
}

//loads
if (iCurLC!=-1)
{
  if ((FIL.isFilter(200)) || 
      (FIL.isFilter(321)) ||
      (FIL.isFilter(323)) ||
	    (FIL.isFilter(324)) ||
      (FIL.isFilter(326)) ||
      (FIL.isFilter(328)) ||
      (FIL.isFilter(329)))
  {
    Ret = LCS[iCurLC]->SelDist(InPT,FIL);
    if (Ret.Dist < dDist)
	{
	  pMinSel=Ret.pObj;
	  dDist=Ret.Dist;
	}
  }
}
//Boundary condition
if (iCurBC!=-1)
{
  if ((FIL.isFilter(201)) || 
	  (FIL.isFilter(322)) || 
      (FIL.isFilter(327)))
  {
    Ret = BCS[iCurBC]->SelDist(InPT,FIL);
	if (Ret.Dist < dDist)
	{
	  pMinSel=Ret.pObj;
	  dDist=Ret.Dist;
	}
  }
}

if (iCurTSet!=-1)
{
  if ((FIL.isFilter(202)) || 
      (FIL.isFilter(325)))
  {
    Ret = TSETS[iCurTSet]->SelDist(InPT,FIL);
	if (Ret.Dist < dDist)
	{
	  pMinSel=Ret.pObj;
	  dDist=Ret.Dist;
	}
  }
}



if (FIL.isFilter(12))
{
if (iCYS > 0)
{
  for (i = 0; i < iCYS; i++)
  {
	Ret = pSys[i]->SelDist(InPT,FIL);
    if (Ret.Dist < dDist)
	{
	  pMinSel=Ret.pObj;
	  dDist=Ret.Dist;
	}
  }
}
}

if ((FIL.isFilter(3)==TRUE) 
|| (FIL.isFilter(1)==TRUE)
|| (FIL.isFilter(200)==TRUE)
|| (FIL.isFilter(201)==TRUE)
|| (FIL.isFilter(202)==TRUE)
|| (FIL.isFilter(321)==TRUE)
|| (FIL.isFilter(322)==TRUE)
|| (FIL.isFilter(323)==TRUE)
|| (FIL.isFilter(324)==TRUE)
|| (FIL.isFilter(325)==TRUE)
|| (FIL.isFilter(326)==TRUE)
|| (FIL.isFilter(327)==TRUE)
|| (FIL.isFilter(328)==TRUE)
|| (FIL.isFilter(12)==TRUE))
{
  Ret.pObj=pMinSel;
  Ret.Dist=dDist;
}
else
{
  Ret.pObj=this;
  Ret.Dist=dDist;
}
return (Ret);
}


void ME_Object::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
int i;
if (iNdNo > 0)
{
  for (i = 0; i < iNdNo; i++)
  {
    if (pNodes[i]->isSelectable()==1)
	{
	  pNodes[i]->S_Box(P1,P2,pSel);
	}
  }
}

if (iElNo > 0)
{
  for (i = 0; i < iElNo; i++)
  {
	if (pElems[i]->isSelectable()==1)
	{
	  pElems[i]->S_Box(P1,P2,pSel);
	}
  }
}

if (iCYS > 0)
{
  for (i = 0; i < iCYS; i++)
  {
	  if (pSys[i]->isSelectable() == 1)
	  {
		  pSys[i]->S_Box(P1, P2, pSel);
	  }
  }
}

if (iBCLDs > 0)
{
  for (i = 0; i < iBCLDs; i++)
  {
	  if (pBCLDs[i]->isSelectable() == 1)
	  {
		  pBCLDs[i]->S_Box(P1, P2, pSel);
	  }
  }
}
if (pResVectors!=NULL)
  pResVectors->S_Box(P1, P2, pSel);

if (iCurLC!=-1)
{
	LCS[iCurLC]->S_Box(P1, P2, pSel);
}

if (iCurBC!=-1)
{
	BCS[iCurBC]->S_Box(P1, P2, pSel);
}

if (iCurTSet!=-1)
{
   TSETS[iCurTSet]->S_Box(P1, P2, pSel);
}

}


void ME_Object::S_Sel(int iT,ObjList* pSel)
{
int i;
if ((iT==1) || (iT==-1))
{
  for (i = 0; i < iNdNo; i++)
  {
	  pSel->Add(pNodes[i]);
  }
}

if ((iT==3) || (iT==-1))
{
  for (i = 0; i < iElNo; i++)
  {
	  pSel->Add(pElems[i]);
  }
}

if ((iT==12) || (iT==-1))
{
  for (i = 0; i < iCYS; i++)
  {
	  pSel->Add(pSys[i]);
  }
}

if (((iT==321) || (iT==322) || (iT==323) || (iT==324) || (iT==328))  || (iT==-1))
{
  for (i = 0; i < iBCLDs; i++)
  {
    if (pBCLDs[i]->iObjType==iT)
	  pSel->Add(pBCLDs[i]);
  }
}


}


G_Object* ME_Object::GetObj(int iType,int iLab)
{
G_Object* pRet;
pRet = NULL;
if (iType == 1)
{
   pRet = GetNode(iLab);
}
else if (iType == 3)
{
   pRet = GetElement(iLab);
}
else if (iType == 12)
{
   pRet = GetSys(iLab);
}
else if (iType == 200)
{
  pRet = GetLC(iLab);
}
else if (iType == 201)
{
  pRet = GetBC(iLab);
}
else if (iType == 202)
{
  pRet = GetTSET(iLab);
}
return (pRet);
}
//************************************************************************
// Coeff Mat ,el stiff matrix,steering vec and no off equ
// Post : el assembled into bk
//************************************************************************
void ME_Object::formkv(Vec<double> &bk,Mat &KM,Vec<int> &g,int neq)
{
int i,j,ndof,icd,ival;
ndof = KM.m;
for (i=1;i<ndof+1;i++)
{
  if (*g.nn(i) > 0)
  {
    for (j=1;j<ndof+1;j++)
	  {
      if (*g.nn(j) > 0)
	    {
        icd = *g.nn(j) - *g.nn(i) + 1;
        if ((icd - 1) >= 0)
		    {
          ival = neq * (icd - 1) + *g.nn(i);
          *bk.nn(ival) = *bk.nn(ival) + *KM.mn(i, j);
        }
	    }
    }
  }
}
}


//************************************************************************
// Coeff Mat in upper baned vector form and no of equs
// Post : guass reduction
//************************************************************************
void ME_Object::banred(Vec<double> &bk,int neq)
{
int iw,i,j,il1,kbl,ij;
double sum;
int m,nkb,ni,nj;
sum=0;
iw = bk.n/neq-1;
for (i=2;i<neq+1;i++)
{
  il1=i-1;
  kbl=il1+iw+1;
  if (kbl-neq>0)
  {
    kbl=neq;
  }
  for(j=i;j<kbl+1;j++)
  {
    ij=(j-i)*neq+i;
    sum=bk.v[ij-1];
    nkb=j-iw;
    if(nkb<=0)
	  {
      nkb=1;
    }
    if (nkb-il1<=0)
	  {
      for (m=nkb;m<il1+1;m++)
	    {
        ni=(i-m)*neq+m;
        nj=(j-m)*neq+m;
        sum=sum-bk.v[ni-1]* bk.v[nj-1]/bk.v[m-1];
      }
    }
    bk.v[ij-1]=sum;
  }
}
}

//************************************************************************
// Reduced Coeff Matrix and load vector
// Post : load vector contain solution
//************************************************************************
void ME_Object::bacsub(Vec<double> &bk, Vec<double> &Loads)
{
double sum;
int nkb, k,i,jn,jj,i1,n,iw;
n = Loads.n;
iw = bk.n / n - 1;
*Loads.nn(1) = *Loads.nn(1) / (*bk.nn(1));
 for (i=2;i<n+1;i++)
 {
   sum = *Loads.nn(i);
   i1=i-1;
   nkb=i-iw;
   if (nkb<=0)
   {
     nkb=1;
   }
   for (k=nkb;k<i1+1;k++)
   {
     jn=(i-k)*n+k;
     sum=sum-*bk.nn(jn)**Loads.nn(k);
   }
   *Loads.nn(i)=sum/(*bk.nn(i));
 }
 for(jj=2;jj<n+1;jj++)
 {
  i=n-jj+1;
  sum=0;
  i1=i+1;
  nkb=i+iw;
  if ((nkb-n)>0)
  {
    nkb=n;
  }
  for (k=i1;k<nkb+1;k++)
  {
    jn=(k-i)*n+i;
    sum=sum+*bk.nn(jn)**Loads.nn(k);
  }
  *Loads.nn(i) = *Loads.nn(i) - sum / (*bk.nn(i));
 }
}



double ME_Object::GetDisp(int iDof,Vec<int> &Steer,Vec<double> &Disp)
{
double dret=0;
dret=*Disp.nn(iDof);
return(dret);
}


void ME_Object::Displacements(int iLC, CString sSol, CString sStep, Vec<int> &Steer,Vec<double> &Disp)
{

ResSet* Res=new ResSet();

Res->ACODE=11;
Res->TCODE=1;
Res->TYPE=0;
Res->LC=iLC;
Res->sSubTitle = sStep;
Res->sTitle = sSol;
Res->WID=8;
Res->sName="DISPLACEMENT";
Res->iNoV=6;
Res->lab[0]="TX";
Res->lab[1]="TY";
Res->lab[2]="TZ";
Res->lab[3]="RX";
Res->lab[4]="RY";
Res->lab[5]="RZ";

int i;
FILE* pFile;
pFile = fopen("DispRes.txt","w");
fprintf(pFile,"%s\n","DISPLACEMENTS");
float X,Y,Z;

for(i=0;i<iNdNo;i++)
{
    Res6* pRes=new Res6;
    pRes->ID = pNodes[i]->iLabel;
    pRes->v[0]=0;
    pRes->v[1]=0;
    pRes->v[2]=0;
    pRes->v[3]=0;
    pRes->v[4]=0;
    pRes->v[5]=0;
    

  X=0;Y=0;Z=0;
  if (pNodes[i]->dof[0]>0)
  {
	X=(float) GetDisp(pNodes[i]->dof[0],Steer,Disp);
  pRes->v[0]=X;
  }
  if (pNodes[i]->dof[1]>0)
  {
	Y=(float) GetDisp(pNodes[i]->dof[1],Steer,Disp);
  pRes->v[1]=Y;
  }
  if (pNodes[i]->dof[2]>0)
  {
	Z=(float) GetDisp(pNodes[i]->dof[2],Steer,Disp);
  pRes->v[2]=Z;
  }

  if (pNodes[i]->dof[3]>0)
  {
	Z=(float) GetDisp(pNodes[i]->dof[3],Steer,Disp);
  pRes->v[3]=Z;
  }
  if (pNodes[i]->dof[4]>0)
  {
	Z=(float) GetDisp(pNodes[i]->dof[4],Steer,Disp);
  pRes->v[4]=Z;
  }
  if (pNodes[i]->dof[5]>0)
  {
	Z=(float) GetDisp(pNodes[i]->dof[5],Steer,Disp);
  pRes->v[5]=Z;
  }
  Res->Add(pRes);
  fprintf_s(pFile,"%s%6i%s%6.4f%s%6.4f%s%6.4f\n","Node ",pNodes[i]->iLabel," X: ",X," Y: ",Y," Z: ",Z);
}
fclose(pFile);
if (Res->iCnt>0)
{
  ResultsSets[iNoRes]=Res;
  //**********Define the Vector********************
  ResDef* pVT = new ResDef();
  pVT->sResType = "DISP TRANS VEC";
  pVT->iResType = 1;   //Vector Translation
  pVT->iLoc = 0;       //Nodal
  pVT->iComponents[0] = 0;
  pVT->iComponents[1] = 1;
  pVT->iComponents[2] = 2;
  pVT->iComponents[3] = -1;
  pVT->iCompNo = 4;
  pVT->GenDefualtHeaders();
  ResDef* pVR = new ResDef();
  pVR->sResType = "DISP ROTA VEC";
  pVR->iResType = 1;   //Vector Rotation
  pVR->iLoc = 0;       //Nodal
  pVR->iComponents[0] = 3;
  pVR->iComponents[1] = 4;
  pVR->iComponents[2] = 5;
  pVR->iComponents[3] = -1;
  pVR->iCompNo = 4;
  pVT->GenDefualtHeaders();
  ResultsSets[iNoRes]->AddResDef(pVT);
  ResultsSets[iNoRes]->AddResDef(pVR);
  //***********************************************
  iNoRes++;
}
else
{
   delete(Res);
}

}

//***************************************************************
//  Radient Heat Loss Calculation
//***************************************************************
void ME_Object::RadiationLoss(Vec<int> &Steer, Vec<double> &T, Vec<double> &Q)
{
  int i;
  E_Object* pE;
  E_Object4* pE4;
  double dA=1;
  double dSBC=5.67E-8;

  //For all shell elements calculate their radient heat loss
  for (i = 0; i<iElNo; i++)
  {
    pE = pElems[i];
    if (pE->iType==94)
    {
      pE4=(E_Object4*) pE;

    }
  }
}

void ME_Object::Temperatures(int iLC,CString sSol,CString sStep,Vec<int> &Steer,Vec<double> &Disp)
{
  ResSet* Res=new ResSet();
  Res->ACODE=11;
  Res->TCODE=1;
  Res->TYPE=0;
  Res->LC=iLC;
  Res->WID=1;

  Res->sName="TEMPERATURES";
  Res->sTitle=sSol;
  Res->sSubTitle=sStep;
  Res->iNoV=1;
  Res->lab[0]="T";
  int i;
  float T;
  for(i=0;i<iNdNo;i++)
  {
    Res1* pRes=new Res1;
    pRes->ID = pNodes[i]->iLabel;
    pRes->v[0]=0;
    T=0;;
    if (pNodes[i]->dof[0]>0)
    {
	    T=(float) GetDisp(pNodes[i]->dof[0],Steer,Disp);
      pRes->v[0]=T;
    }
    Res->Add(pRes);
  }
  if (Res->iCnt>0)
  {
    ResultsSets[iNoRes]=Res;
    iNoRes++;
  }
  else
  {
   delete(Res);
  }
}

void ME_Object::TempBCSet(int iLC, CString sSol, CString sStep, Vec<int> &Steer, Vec<double> &Disp)
{
  int i;
  int iTS;
  int iTSetID;
  E_Object* pE;
  double dTemp;
  iTSetID = NestTSetID();
  iTS = CreateTSET(iTSetID, sStep);
  cLinkedListT* pTS =  GetTSET(iTSetID);
  for (i = 0; i<iElNo; i++)
  {
    pE = pElems[i];
    dTemp = pE->GetCentriodVal(0, Steer, Disp);
    Temperature* pT = new Temperature();
    pT->Create(pE, pTS, dTemp, pTS->iLabel);
    pTS->Add(pT);
  }
}

void ME_Object::TranslationalSpringForces(int iLC, CString sSol, CString sStep, PropTable* PropsT,MatTable* MatT,Vec<int> &Steer,Vec<double> &Disp)
{
int i,j,k,iNoNodes;
double dof1;
ResSet* ResS=new ResSet();
ResS->ACODE=11;
ResS->TCODE=4;
ResS->TYPE=0;
ResS->LC = iLC;
ResS->sSubTitle = sStep;
ResS->sTitle = sSol;
ResS->WID=3;

ResS->sName="SPRING FORCES/MOMENTS";
ResS->iNoV=3;
ResS->lab[0]="FX";
ResS->lab[1]="FY";
ResS->lab[2]="FZ";


Mat disp;
Mat KM;
Mat Res;
C3dVector TRA;
C3dVector RRA;
C3dVector TRB;
C3dVector RRB;
CoordSys* pCSys=NULL;
C3dMatrix TMAT;
int aa=0;
for(i=0;i<iElNo;i++)
{
  iNoNodes=0;
  if ((pElems[i]->iType==136) || (pElems[i]->iType==137))
  {
    E_Object2* pE2=(E_Object2*) pElems[i];
    pCSys=this->GetSys(pE2->iCSYS);
    if (pCSys!=NULL)
      TMAT=pE2->GetSpringSys(pCSys);
    else
      TMAT.MakeUnit();
    //TMAT=pElems[i]->GetElSys();
    iNoNodes=2;
    disp.Create(6,1);
    for (j=0;j<iNoNodes;j++)
    {
      for (k=0;k<3;k++)
      {
        dof1=0;
        Pt_Object* pN=(Pt_Object*) pElems[i]->GetNode(j);
        if (pN->dof[k]>0)
        {
          dof1=GetDisp(pN->dof[k],Steer,Disp);
        }
        *disp.mn(3*j+(k+1),1)=dof1;
      }
    }
    if (pElems[i]->iLabel==84)
      aa++;
      KM=pElems[i]->GetStiffMat(PropsT,MatT);
    //KM.diag();
    //Res=KM*disp;
    C3dVector vFG;
    C3dVector vLG;

    vFG.x=*disp.mn(4,1)-*disp.mn(1,1);
    vFG.y=*disp.mn(5,1)-*disp.mn(2,1);
    vFG.z=*disp.mn(6,1)-*disp.mn(3,1);
    double dMag;
    //if (pElems[i]->iLabel==196)
      dMag=vFG.Mag();
    //Need to transform to local
    Res3* pRes=new Res3;
    pRes->ID = pElems[i]->iLabel;
    //TRANSFORM WHEN ADDED
    TMAT.Transpose();

    vLG.x= (float) vFG.x**KM.mn(1,1)+vFG.y**KM.mn(1,2)+vFG.z**KM.mn(1,3);
    vLG.y= (float) vFG.x**KM.mn(2,1)+vFG.y**KM.mn(2,2)+vFG.z**KM.mn(2,3);
    vLG.z= (float) vFG.x**KM.mn(3,1)+vFG.y**KM.mn(3,2)+vFG.z**KM.mn(3,3);
    vLG=TMAT.Mult(vLG);
    pRes->v[0]= (float) vLG.x;
    pRes->v[1]= (float) vLG.y;
    pRes->v[2]= (float) vLG.z;
    dMag=vFG.Mag();
    ResS->Add(pRes);
    KM.clear();
    Res.clear();
    disp.clear();
  }
}
if (ResS->iCnt>0)
{
  ResultsSets[iNoRes]=ResS;
  iNoRes++;
}
else
{
   delete(ResS);
}
}

void ME_Object::StressesBeam(int iLC, CString sSol, CString sStep, PropTable* PropsT,MatTable* MatT,Vec<int> &Steer,Vec<double> &Disp)
{
int i,j,k,iNoNodes;
double dof1;
ResSet* ResS=new ResSet();
ResS->ACODE=11;
ResS->TCODE=4;
ResS->TYPE=0;
ResS->LC = iLC;
ResS->sSubTitle = sStep;
ResS->sTitle = sSol;
ResS->WID=8;

ResS->sName="BEAM GRID PT REACTIONS";
ResS->iNoV=12;
ResS->lab[0]="END A FX";
ResS->lab[1]="END A FY";
ResS->lab[2]="END A FZ";
ResS->lab[3]="END A BX";
ResS->lab[4]="END A BY";
ResS->lab[5]="END A BZ";
ResS->lab[6]="END B FX";
ResS->lab[7]="END B FY";
ResS->lab[8]="END B FZ";
ResS->lab[9]="END B BX";
ResS->lab[10]="END B BY";
ResS->lab[11]="END B BZ";

Mat disp;
Mat KM;
Mat Res;
C3dMatrix TMAT;
C3dVector TRA;
C3dVector RRA;
C3dVector TRB;
C3dVector RRB;
for(i=0;i<iElNo;i++)
{
  iNoNodes=0;
  if (pElems[i]->iType==21)
  {
    TMAT=pElems[i]->GetElSys();
    iNoNodes=2;
    disp.Create(12,1);
    for (j=0;j<iNoNodes;j++)
    {
      for (k=0;k<6;k++)
      {
        dof1=0;
        Pt_Object* pN=(Pt_Object*) pElems[i]->GetNode(j);
        if (pN->dof[k]>0)
        {
          dof1=GetDisp(pN->dof[k],Steer,Disp);
        }
        *disp.mn(6*j+(k+1),1)=dof1;
      }
    }
    KM=pElems[i]->GetStiffMat(PropsT,MatT);
    Res=KM*disp;
    TRA.x=*Res.mn(1,1);
    TRA.y=*Res.mn(2,1);
    TRA.z=*Res.mn(3,1);
    RRA.x=*Res.mn(4,1);
    RRA.y=*Res.mn(5,1);
    RRA.z=*Res.mn(6,1);

    TRB.x=*Res.mn(7,1);
    TRB.y=*Res.mn(8,1);
    TRB.z=*Res.mn(9,1);
    RRB.x=*Res.mn(10,1);
    RRB.y=*Res.mn(11,1);
    RRB.z=*Res.mn(12,1);

    TRA=TMAT*TRA;
    RRA=TMAT*RRA;
    TRB=TMAT*TRB;
    RRB=TMAT*RRB;

    KM.clear();
    Res.clear();
    disp.clear();
    //Need to transform to local
    Res12* pRes=new Res12;
    pRes->ID = pElems[i]->iLabel;
    pRes->v[0]=(float) TRA.x+pElems[i]->dTemp;
    pRes->v[1]=(float) TRA.y;
    pRes->v[2]=(float) TRA.z;
    pRes->v[3]=(float) RRA.x;
    pRes->v[4]=(float) RRA.y;
    pRes->v[5]=(float) RRA.z;

    pRes->v[6]=(float) TRB.x-pElems[i]->dTemp;
    pRes->v[7]=(float) TRB.y;
    pRes->v[8]=(float) TRB.z;
    pRes->v[9]=(float) RRB.x;
    pRes->v[10]=(float) RRB.y;
    pRes->v[11]=(float) RRB.z;

    ResS->Add(pRes);
  }
}
if (ResS->iCnt>0)
{
  ResultsSets[iNoRes]=ResS;
  iNoRes++;
}
else
{
   delete(ResS);
}
}
ResSet* ME_Object::Create2dStrainResSet(CString sTitle, int iLC, CString sStep, CString sSol)
{
	ResSet* ResStrn = new ResSet();
	ResStrn->ACODE = 11;
	ResStrn->TCODE = 4;
	ResStrn->TYPE = 0;
	ResStrn->LC = iLC;
	ResStrn->WID = 8;
	ResStrn->sName = "2d EL STRAINS";
	ResStrn->iNoV = 10;
	ResStrn->lab[0] = "Strain X";
	ResStrn->lab[1] = "Strain Y";
	ResStrn->lab[2] = "Strain XY";
	ResStrn->lab[3] = "Prin 1";
	ResStrn->lab[4] = "Prin 2";
	ResStrn->lab[5] = "Strain Curv X";
	ResStrn->lab[6] = "Strain Curv Y";
	ResStrn->lab[7] = "Strain Curv XY";
	ResStrn->lab[8] = "Prin 1";
	ResStrn->lab[9] = "Prin 2";
	ResDef* pVT;
	pVT = new ResDef();
	pVT->sResType = "2d STRAIN (Mid)";
	pVT->iResType = 3;   //2d Tenspr
	pVT->iLoc = 1;       //Element Centroid(cys global)
	pVT->iComponents[0] = 0;
	pVT->iComponents[1] = 1;
	pVT->iComponents[2] = 2;
	pVT->iComponents[3] = 3;
	pVT->iComponents[4] = 4;
	pVT->iCompNo = 5;
	pVT->GenDefualtHeaders();
	ResStrn->AddResDef(pVT);

	pVT = new ResDef();
	pVT->sResType = "2d CURVATURE (Mid)";
	pVT->iResType = 3;   //2d Tenspr
	pVT->iLoc = 1;       //Element Centroid(cys global)
	pVT->iComponents[0] = 5;
	pVT->iComponents[1] = 6;
	pVT->iComponents[2] = 7;
	pVT->iComponents[3] = 8;
	pVT->iComponents[4] = 9;
	pVT->iCompNo = 5;
	pVT->GenDefualtHeaders();
	ResStrn->AddResDef(pVT);
	return (ResStrn);
}

ResSet* ME_Object::Create2dStressResSet(CString sTitle,int iLC,CString sStep,CString sSol)
{
	ResSet* ResS = new ResSet();
	ResS->ACODE = 11;
	ResS->TCODE = 4;
	ResS->TYPE = 0;
	ResS->LC = iLC;
	ResS->sSubTitle = sStep;
	ResS->sTitle = sSol;
	ResS->WID = 8;

	ResS->sName = sTitle;
	ResS->iNoV = 21;
	ResS->lab[0] = "Stress X (Mid)";
	ResS->lab[1] = "Stress Y (Mid)";
	ResS->lab[2] = "Stress XY (Mid)";
	ResS->lab[3] = "Max Prin (Mid)";
	ResS->lab[4] = "Min Prin (Mid)";
	ResS->lab[5] = "Max Shear (Mid)";
	ResS->lab[6] = "Von Mises (Mid)";

	ResS->lab[7] = "Stress X (Z1 Bot)";
	ResS->lab[8] = "Stress Y (Z1 Bot)";
	ResS->lab[9] = "Stress XY (Z1 Bot)";
	ResS->lab[10] = "Max Prin (Z1 Bot)";
	ResS->lab[11] = "Min Prin (Z1 Bot)";
	ResS->lab[12] = "Max Shear (Z1 Bot)";
	ResS->lab[13] = "Von Mises (Z1 Bot)";

	ResS->lab[14] = "Stress X (Z2 Top)";
	ResS->lab[15] = "Stress Y (Z2 Top)";
	ResS->lab[16] = "Stress XY (Z2 Top)";
	ResS->lab[17] = "Max Prin (Z2 Top)";
	ResS->lab[18] = "Min Prin (Z2 Top)";
	ResS->lab[19] = "Max Shear (Z2 Top)";
	ResS->lab[20] = "Von Mises (Z2 Top)";
	ResSet* ResSt = new ResSet();
	ResDef* pVT;
	pVT = new ResDef();
	pVT->sResType = sTitle + "(Mid)";
	pVT->iResType = 3;   //2d Tenspr
	pVT->iLoc = 1;       //Element Centroid(cys global)
	pVT->iComponents[0] = 0;
	pVT->iComponents[1] = 1;
	pVT->iComponents[2] = 2;
	pVT->iComponents[3] = 3;
	pVT->iComponents[4] = 4;
	pVT->iCompNo = 5;
	pVT->GenDefualtHeaders();
	ResS->AddResDef(pVT);

	pVT = new ResDef();
	pVT->sResType = sTitle  + "(Z1 Bot)";
	pVT->iResType = 3;   //2d Tenspr
	pVT->iLoc = 1;       //Element Centroid(cys global)
	pVT->iComponents[0] = 7;
	pVT->iComponents[1] = 8;
	pVT->iComponents[2] = 9;
	pVT->iComponents[3] = 10;
	pVT->iComponents[4] = 11;
	pVT->iCompNo = 5;
	pVT->GenDefualtHeaders();
	ResS->AddResDef(pVT);

	pVT = new ResDef();
	pVT->sResType = sTitle + "(Z2 Top)";
	pVT->iResType = 3;   //2d Tenspr
	pVT->iLoc = 1;       //Element Centroid(cys global)
	pVT->iComponents[0] = 14;
	pVT->iComponents[1] = 15;
	pVT->iComponents[2] = 16;
	pVT->iComponents[3] = 17;
	pVT->iComponents[4] = 18;
	pVT->iCompNo = 5;
	pVT->GenDefualtHeaders();
	ResS->AddResDef(pVT);
	return (ResS);
}

void ME_Object::Add2dStressRes(ResSet* pSSet, int ID, Mat Res, Mat ResZ1, Mat ResZ2)
{
	double SX, SY, SXY;
	double cS, Mag, p1, p2;
	SX = *Res.mn(1, 1);
	SY = *Res.mn(2, 1);
	SXY = *Res.mn(3, 1);
	cS = (SX + SY)*0.5;
	Mag = pow((SX - cS)*(SX - cS) + (SXY*SXY), 0.5);
	p2 = cS + Mag;
	p1 = cS - Mag;
	Res21* pRes = new Res21;
	pRes->ID = ID;
	pRes->v[0] = (float)SX;
	pRes->v[1] = (float)SY;
	pRes->v[2] = (float)SXY;
	pRes->v[3] = (float)(cS + Mag);
	pRes->v[4] = (float)(cS - Mag);
	pRes->v[5] = (float)Mag;
	pRes->v[6] = (float)pow(p1*p1 - p1 * p2 + p2 * p2, 0.5);

	SX = *ResZ1.mn(1, 1);
	SY = *ResZ1.mn(2, 1);
	SXY = *ResZ1.mn(3, 1);
	cS = (SX + SY)*0.5;
	Mag = pow((SX - cS)*(SX - cS) + (SXY*SXY), 0.5);
	p2 = cS + Mag;
	p1 = cS - Mag;
	pRes->ID = ID;
	pRes->v[7] = (float)SX;
	pRes->v[8] = (float)SY;
	pRes->v[9] = (float)SXY;
	pRes->v[10] = (float)(cS + Mag);
	pRes->v[11] = (float)(cS - Mag);
	pRes->v[12] = (float)Mag;
	pRes->v[13] = (float)pow(p1*p1 - p1 * p2 + p2 * p2, 0.5);

	SX = *ResZ2.mn(1, 1);
	SY = *ResZ2.mn(2, 1);
	SXY = *ResZ2.mn(3, 1);
	cS = (SX + SY)*0.5;
	Mag = pow((SX - cS)*(SX - cS) + (SXY*SXY), 0.5);
	p2 = cS + Mag;
	p1 = cS - Mag;
	pRes->ID = ID;
	pRes->v[14] = (float)SX;
	pRes->v[15] = (float)SY;
	pRes->v[16] = (float)SXY;
	pRes->v[17] = (float)(cS + Mag);
	pRes->v[18] = (float)(cS - Mag);
	pRes->v[19] = (float)Mag;
	pRes->v[20] = (float)pow(p1*p1 - p1 * p2 + p2 * p2, 0.5);
	pSSet->Add(pRes);
}

void ME_Object::Stresses2d(int iLC, CString sSol, CString sStep, PropTable* PropsT,MatTable* MatT,Vec<int> &Steer,Vec<double> &Disp)
{
//ALL THESE MATS NEED TO BE CLEARED!!!
Mat dee;      //stress strain
Mat bee;      //Strain displacement
int nip = 1;  //Centroid stresses only
Mat coord;    //Nodal Coordinates
Mat deriv;    //shape function derivatives
Mat fun;      //shape function
Mat Points;   //sample points
Mat deriv2;
Mat jac;
Mat db;
Mat mStrn;
Mat Res;
Mat ResZ1;
Mat ResZ2;
Mat disp; 
Mat disp3d; 
Mat disp3dR;

C3dVector v; C3dVector R;
int n;
double det;
double dof1;
double dofR;
double dE = 210e9; double dv = 0.29; double dthk = 0.001;
double cS;
double Mag;
int MID = -1;
int i; int j; int k;
double SX, SY, SXY, RX, RY, RXY;
ResSet* ResS;                        //Mid Stress Results Set
ResSet* ResStrn;                     //Strain (dir & cur) Results Set
ResS = Create2dStressResSet("2d EL STRESSES", iLC, sStep, sSol);
ResStrn = Create2dStrainResSet("2d EL STRAINS (Mid)", iLC, sStep, sSol);

SX = 0; SY = 0, SXY = 0; RX = 0; RY = 0; RXY = 0;
int iNoNodes=0;
for(i=0;i<iElNo;i++)
{
  iNoNodes=0;
  if (pElems[i]->iType==91)
  {
    iNoNodes=3;
    disp.Create(6,1);
    disp3d.Create(3,3);
	disp3dR.Create(3, 3);
  }
  else if (pElems[i]->iType==94)
  {
    iNoNodes=4;
    disp.Create(8,1);
    disp3d.Create(3,4);
	disp3dR.Create(3, 4);
  }
  Property* pS=PropsT->GetItem(pElems[i]->PID);
  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
  }
  Material* pM=MatT->GetItem(MID);
//Get Shell thickness

 if (((pElems[i]->iType==91) || (pElems[i]->iType==94)) && (pS!=NULL))
 {
   PSHELL* pSh = (PSHELL*) pS;
   dthk=pSh->dT;
 }

 if ((pM!=NULL) && (pM->iType = 1))
 {
   MAT1* pIsen = (MAT1*) pM;
   dE=pIsen->dE;
   dv=pIsen->dNU;
 }
 //************START OF CALCULATION************
 if (iNoNodes > 0)
 {
	 for (j = 0; j < iNoNodes; j++)
	 {  //Get the displacements
		 for (k = 0; k < 3; k++)
		 {
			 dof1 = 0;
			 dofR = 0;
			 Pt_Object* pN = (Pt_Object*)pElems[i]->GetNode(j);
			 if (pN->dof[k] > 0)
			 {
				 dof1 = GetDisp(pN->dof[k], Steer, Disp);
			 }
			 *disp3d.mn(k + 1, j + 1) = dof1;  // Nodal translations
			 if (pN->dof[k + 3] > 0)
			 {
				 dofR = GetDisp(pN->dof[k + 3], Steer, Disp);
			 }
			 *disp3dR.mn(k + 1, j + 1) = dofR; // Nodal Rotations
		 }
	 }
	 C3dMatrix M = pElems[i]->GetElSys();

	 //displacement in plain of element
	 for (j = 0; j < iNoNodes; j++)
	 {
		 //Translations
		 v.x = *disp3d.mn(1, j + 1);
		 v.y = *disp3d.mn(2, j + 1);
		 v.z = *disp3d.mn(3, j + 1);
		 v = M.Mult(v);
		 n = 1 + j * 2;
		 *disp.mn(n, 1) = v.x;
		 *disp.mn(n + 1, 1) = v.y;
		 //Rotations
		 R.x = *disp3dR.mn(1, j + 1);  //Theta global X
		 R.y = *disp3dR.mn(2, j + 1);  //Theta global Y
		 R.z = *disp3dR.mn(3, j + 1);  //Theta global Z
		 R = M.Mult(R);
		 //update the plate bending / shear disp vector
		 *disp3dR.mn(1, j + 1) = v.z;   // Z displacement
		 *disp3dR.mn(2, j + 1) = R.x;   // theta X
		 *disp3dR.mn(3, j + 1) = R.y;   // theta Y (disregard theta Z)
	 }
	 disp3d.clear();
	 dee = pElems[i]->DeeMat(dE, dv, 3);
	 coord = pElems[i]->getCoords3d();       //Coords in element CSYS this case actually 2d
	 Mat NdDisp(iNoNodes, 2);
	 Points = pElems[i]->Sample(nip);         //Only 1 integration point at centre of element
	 det = 0;
	 fun = pElems[i]->ShapeFun(Points, 1);    //1x4 shape fuction values at el centre
	 deriv = pElems[i]->ShapeDer(Points, 1);  //2x4 shape fuction derivatives
	 jac = deriv * coord;
	 jac = jac.InvertJac(det);
	 deriv2 = jac * deriv;
	 //Bending Strain Components curvatures k11,k22,k12
	 //1 pt at el centre
	 C3dMatrix Bb;
	 C3dVector d;
	 C3dVector Curv;
	 Curv.Set(0, 0, 0);
	 for (k = 1; k < iNoNodes+1; k++)
	 {
	   Bb.m_00 = 0;
	   Bb.m_01 = 0;
	   Bb.m_02 = -*deriv2.mn(1, k);
	   Bb.m_10 = 0;
	   Bb.m_11 = *deriv2.mn(2, k);
	   Bb.m_12 = 0;
	   Bb.m_20 = 0;
	   Bb.m_21 = *deriv2.mn(1, k);
	   Bb.m_22 = -*deriv2.mn(2, k);
	   d.x = *disp3dR.mn(1, k);
	   d.y = *disp3dR.mn(2, k);
	   d.z = *disp3dR.mn(3, k);
	   Curv += Bb.Mult(d);
     }
	 

	//Out of Plain Shear Strain Components gamma1, gamma2

	 disp3dR.clear();
	//in plane components x,y,xy

    bee=pElems[i]->bmat(coord, deriv2,3,2);
	//bee.diag();
    db=bee*disp;
	SX=*db.mn(1,1);
    SY=*db.mn(2,1);
    SXY=*db.mn(3,1);
	//RX = *db.mn(4, 1);
	//RY = *db.mn(5, 1);
	//RXY = *db.mn(6, 1);
	SXY *= 0.5; //Not sure about
    Res10* pResStrn=new Res10;
	pResStrn->ID = pElems[i]->iLabel;
	pResStrn->v[0]=(float) SX;
	pResStrn->v[1]=(float) SY;
	pResStrn->v[2]=(float) SXY;
	cS = (SX + SY)*0.5;
	
	Mag = pow((SX - cS)*(SX - cS) + (SXY*SXY), 0.5);
	pResStrn->v[3] = (float)(cS + Mag);
	pResStrn->v[4] = (float)(cS - Mag);
	pResStrn->v[5]= (float) Curv.x;
	pResStrn->v[6]= (float) Curv.y;
	pResStrn->v[7]= (float) Curv.z;
	SX = Curv.x;
	SY = Curv.y;
	SXY = Curv.z;
	cS = (SX + SY)*0.5;
	Mag = pow((SX - cS)*(SX - cS) + (SXY*SXY), 0.5);
	pResStrn->v[8] = (float)(cS + Mag);
	pResStrn->v[9] = (float)(cS - Mag);
	//Principals 

	ResStrn->Add(pResStrn);

    *db.mn(1,1)-=pElems[i]->dTemp;   //Subtract thermal strains 
    *db.mn(2,1)-=pElems[i]->dTemp;   //Subtract thermal strains
	//Z2 bending Stress (Top surface Z2 is thick/2 z1 is -thick/2)
	mStrn = db;
	*mStrn.mn(1, 1) -= 0.5*Curv.x*dthk;   //Subtract thermal strains 
	*mStrn.mn(2, 1) -= 0.5*Curv.y*dthk;
	*mStrn.mn(3, 1) -= 0.5*Curv.z*dthk;  //Engininerring Strain to Strain????
    Res=dee* mStrn;                        //Calculare Stresses
	mStrn.clear();
	//Z1 bending Stress (Bot surface Z1 is -thick/2 z1 is -thick/2)
	mStrn = db;
	*mStrn.mn(1, 1) += 0.5*Curv.x*dthk;   //Subtract thermal strains 
	*mStrn.mn(2, 1) += 0.5*Curv.y*dthk;
	*mStrn.mn(3, 1) += 0.5*Curv.z*dthk;
	ResZ1 = dee * mStrn;                        //Calculare Stresses
	mStrn.clear();
	//Mid bending Stress (Bot surface Z1 is thick/2 z1 is -thick/2)
	mStrn = db;
	ResZ2 = dee * mStrn;
	Add2dStressRes(ResS, pElems[i]->iLabel, Res,ResZ1,ResZ2);
	Res.clear();
	ResZ1.clear();
	ResZ2.clear();
	mStrn.clear();
  }
}
if (ResStrn->iCnt>0)
{ResultsSets[iNoRes]= ResStrn; iNoRes++;}
else
{delete(ResStrn);}
if (ResS->iCnt>0)
{ResultsSets[iNoRes] = ResS; iNoRes++;}
else
{delete(ResS);}

}

C3dVector ME_Object::EigenVector3d(int iEID,C3dVector rX, C3dVector rY, C3dVector rZ, double lambda)
{
	C3dVector res;
	rX.x -= lambda;
	rY.y -= lambda;
	rZ.z -= lambda;
	char s80[80];
	double dS1;
	//Assume trail X=1 for eigen vetoy
	if (rX.z != 0)
	{
		dS1 = rY.z / rX.z;
		rX *= dS1;
		//Subtract First *dS1 form second
		rY -= rX;
		res.x = 1;
		res.y = -rY.x / rY.y;
		res.z = -(rX.x + res.y*rX.y) / rX.z;

	}
	else
	{
		sprintf_s(s80, "%s %i", "Eigenvector undetermined for EID ", iEID);
		outtext1(s80);
	}
	res.Normalize();
	res *= lambda;
	return (res);
}

C3dVector ME_Object::EigenVector2d(int iEID, C3dVector rX, C3dVector rY, double lambda)
{
	C3dVector res;
	rX.x -= lambda;
	rY.y -= lambda;
	char s80[80];
	//Assume trail X=1 for eigen vetoy
	if (rX.y != 0)
	{
		res.x = 1;
		res.y = -rX.x / rX.y;
		res.z = 0;

	}
	else
	{
		sprintf_s(s80, "%s %i", "Eigenvector undetermined for EID ", iEID);
		outtext1(s80);
	}
	res.Normalize();
	res *= lambda;
	return (res);
}


void ME_Object::CalcPrinStress(double XX,double YY,double ZZ,
                               double XY,double YZ,double XZ,
                               double &P1,double &P2,double &P3)
{
double a,b,c,d;
a=1;
b=-1*(XX+YY+ZZ);
c=XX*YY+YY*ZZ+XX*ZZ-XY*XY-YZ*YZ-XZ*XZ;
d=-1*(XX*YY*ZZ+2*XY*YZ*XZ-XY*XY*ZZ-YZ*YZ*XX-XZ*XZ*YY);
//Use the equation from above to get your cubic equation and combine all constant terms possible to
//give you a reduced equation we will use a, b, c and d to denote the coefficients of this equation.
double x,y,z;

x = ((3*c/a) - (b*b/(a*a)))/3.0;
y = ((2*b*b*b/(a*a*a)) - (9*b*c/(a*a)) + (27*d/a))/27.0;
z = y*y/4 + x*x*x/27;
// 
double i, j, k, m, n, p;
i = pow((y*y/4 - z),0.5);
j = -pow(i,(1.0/3.0));
k = acos(-(y/(2*i)));
m = cos(k/3);
n = pow(3,0.5)*sin(k/3);
p = b/(3*a);
// 
double Eig1, Eig2, Eig3;
Eig1 = -(2*j*m + p);
Eig2 = -(-j *(m + n) + p);
Eig3 = -(-j*(m - n) + p);
P1=Eig1;
if (Eig2>P1) P1=Eig2;
if (Eig3>P1) P1=Eig3;
P3=Eig1;
if (Eig2<P3) P3=Eig2;
if (Eig3<P3) P3=Eig3;
P2=Eig1;
if ((Eig1<P1) && (Eig1>P3)) P2=Eig1;
if ((Eig2<P1) && (Eig2>P3)) P2=Eig2;
if ((Eig3<P1) && (Eig3>P3)) P2=Eig3;
}


void ME_Object::Stresses3d(int iLC, CString sSol, CString sStep, PropTable* PropsT,MatTable* MatT,Vec<int> &Steer,Vec<double> &Disp)
{

ResSet* ResS=new ResSet();

ResS->ACODE=11;
ResS->TCODE=4;
ResS->TYPE=0;
ResS->LC = iLC;
ResS->sSubTitle = sStep;
ResS->sTitle = sSol;
ResS->WID=8;

ResS->sName="3d GLOBAL STRESSES";
ResS->iNoV=11;
ResS->lab[0]="Stress GX";
ResS->lab[1]="Stress GY";
ResS->lab[2]="Stress GZ";
ResS->lab[3]="Stress GXY";
ResS->lab[4]="Stress GYZ";
ResS->lab[5]="Stress GXZ";
ResS->lab[6]="Stress Max Prin";
ResS->lab[7]="Stress Mid Prin";
ResS->lab[8]="Stress Min Prin1";
ResS->lab[9]="Stress Max Shear";
ResS->lab[10]="Stress Von Mises";
ResDef* pVT = new ResDef();
pVT->sResType = "3d Stress Tensor SOLID";
pVT->iResType = 4;   //3d Tenspr
pVT->iLoc = 1;       //Element Centroid(cys global)
pVT->iComponents[0] = 0;
pVT->iComponents[1] = 1;
pVT->iComponents[2] = 2;
pVT->iComponents[3] = 3;
pVT->iComponents[4] = 4;
pVT->iComponents[5] = 5;
pVT->iComponents[6] = 6;
pVT->iComponents[7] = 7;
pVT->iComponents[8] = 8;
pVT->iCompNo = 9;
pVT->GenDefualtHeaders();
ResS->AddResDef(pVT);

ResSet* ResSt=new ResSet();
ResSt->ACODE=11;
ResSt->TCODE=4;
ResSt->TYPE=0;
ResSt->LC= iLC;
ResSt->WID=8;

ResSt->sName="3d GLOBAL STRAINS";
ResSt->iNoV=9;
ResSt->lab[0]="Strain GX";
ResSt->lab[1]="Strain GY";
ResSt->lab[2]="Strain GZ";
ResSt->lab[3]="Strain GXY";
ResSt->lab[4]="Strain GYZ";
ResSt->lab[5]="Strain GXZ";
ResSt->lab[6] = "Strain Max Prin";
ResSt->lab[7] = "Strain Mid Prin";
ResSt->lab[8] = "Strain Min Prin";

ResDef* pVS = new ResDef();
pVS->sResType = "3d Strain Tensor SOLID";
pVS->iResType = 4;   //3d Tenspr
pVS->iLoc = 1;       //Element Centroid(cys global)
pVS->iComponents[0] = 0;
pVS->iComponents[1] = 1;
pVS->iComponents[2] = 2;
pVS->iComponents[3] = 3;
pVS->iComponents[4] = 4;
pVS->iComponents[5] = 5;
pVS->iComponents[6] = 6;
pVS->iComponents[7] = 7;
pVS->iComponents[8] = 8;
pVT->GenDefualtHeaders();
pVS->iCompNo = 9;
ResSt->AddResDef(pVS);

int i;
int j;
int k;
Mat disp;

double SX,SY,SZ;
double SXY,SYZ,SXZ;
int iNoNodes=0;
for(i=0;i<iElNo;i++)
{
  //FOR SOLID ELEMENTS ONLY
if ((pElems[i]->iType==115) || (pElems[i]->iType==112) || (pElems[i]->iType==111))
{
  iNoNodes=0;
  if (pElems[i]->iType==115)
  {
    iNoNodes=8;
    disp.Create(24,1);
    
  }
  else if (pElems[i]->iType==112)
  {
    iNoNodes=6;
    disp.Create(18,1);
  }
  else if (pElems[i]->iType==111)
  {
    iNoNodes=4;
    disp.Create(12,1);
  }
  
  Mat dee;      //stress strain
  Mat bee;      //Strain displacement
  int nip=1;    //Centroid stresses only
  Mat coord;    //Nodal Coordinates
  Mat deriv;    //shape function derivatives
  Mat fun;      //shape function
  Mat Points;   //sample points
  Mat deriv2;
  Mat jac;
  Mat db;
  Mat Res;
  //Defualt values
  double det;
  double dof1;
  double dE=210e9;
  double dv=0.29;
  double dthk=0.001;
  int MID=-1;
  Property* pS=PropsT->GetItem(pElems[i]->PID);
  if (pS!=NULL)
  {
    MID=pS->GetDefMatID();
  }
  Material* pM=MatT->GetItem(MID);
  if ((pM!=NULL) && (pM->iType = 1))
  {
    MAT1* pIsen = (MAT1*) pM;
    dE=pIsen->dE;
    dv=pIsen->dNU;
  }
  if (iNoNodes>0)
  {
    for (j=0;j<iNoNodes;j++)
    {  //Get the displacements
      for (k=0;k<3;k++)
      {
        dof1=0;
        Pt_Object* pN=(Pt_Object*) pElems[i]->GetNode(j);
        if (pN->dof[k]>0)
	        dof1=GetDisp(pN->dof[k],Steer,Disp);
        int n;
        n=j*3+k+1;
        *disp.mn(n,1)=dof1;
      }
    }
    dee=pElems[i]->DeeMat(dE,dv,6);  
    coord=pElems[i]->getCoords3d();
    Points=pElems[i]->Sample(nip);
    det = 0;
    fun=pElems[i]->ShapeFun(Points, 1);
    deriv=pElems[i]->ShapeDer(Points, 1);
    jac = deriv*coord;
    jac=jac.InvertJac(det);
    deriv2 = jac*deriv;
    bee=pElems[i]->bmat(coord, deriv2,6,3);
    db=bee*disp;
    SX=*db.mn(1,1);
    SY=*db.mn(2,1);
    SZ=*db.mn(3,1);
    SXY=*db.mn(4,1);
    SYZ=*db.mn(5,1);
    SXZ=*db.mn(6,1);
    Res11* pResS=new Res11;
    pResS->ID = pElems[i]->iLabel;
    pResS->v[0]=(float) SX;
    pResS->v[1]=(float) SY;
    pResS->v[2]=(float) SZ;
    pResS->v[3]=(float) SXY;
    pResS->v[4]=(float) SYZ;
    pResS->v[5]=(float) SXZ;
	double p1, p2, p3;
	CalcPrinStress(SX, SY, SZ, SXY, SYZ, SXZ, p1, p2, p3);
	pResS->v[6] = (float)p1;
	pResS->v[7] = (float)p2;
	pResS->v[8] = (float)p3;

    ResSt->Add(pResS);
    *db.mn(1,1)-=pElems[i]->dTemp;   //Thermal Strains
    *db.mn(2,1)-=pElems[i]->dTemp;
    *db.mn(3,1)-=pElems[i]->dTemp;
    Res=dee*db;
    SX=*Res.mn(1,1);
    SY=*Res.mn(2,1);
    SZ=*Res.mn(3,1);
    SXY=*Res.mn(4,1);
    SYZ=*Res.mn(5,1);
    SXZ=*Res.mn(6,1);
    

    CalcPrinStress(SX,SY,SZ,SXY,SYZ,SXZ,p1,p2,p3);
    Res11* pRes=new Res11;
    pRes->ID = pElems[i]->iLabel;
    pRes->v[0]=(float) SX;
    pRes->v[1]=(float) SY;
    pRes->v[2]=(float) SZ;
    pRes->v[3]=(float) SXY;
    pRes->v[4]=(float) SYZ;
    pRes->v[5]=(float) SXZ;
    pRes->v[6]=(float) p1;
    pRes->v[7]=(float) p2;
    pRes->v[8]=(float) p3;
    pRes->v[9]=(float) 0.5*(p1-p3);
    pRes->v[10]=(float) pow(0.5*((p1-p2)*(p1-p2)+(p2-p3)*(p2-p3)+(p1-p3)*(p1-p3)),0.5);
    ResS->Add(pRes);
	//sEL = "STRESS CHEXA";

  }
}
}
if (ResSt->iCnt>0)
{
  ResultsSets[iNoRes]=ResSt;
  iNoRes++;
}
else
{
  delete(ResSt);
}
if (ResS->iCnt>0)
{
  ResultsSets[iNoRes]=ResS;
  iNoRes++;
}
else
{
  delete(ResS);
}


}

void ME_Object::AddOEFRes(int Vals[],int iCnt,CString sTitle,CString sSubTitle,CString inName)
{
int i;

ResultsSets[iNoRes]=new ResSet();
ResultsSets[iNoRes]->sFile=inName;
ResultsSets[iNoRes]->sTitle=sTitle;
ResultsSets[iNoRes]->sSubTitle=sSubTitle;
ResultsSets[iNoRes]->ACODE=Vals[0];
ResultsSets[iNoRes]->TCODE=Vals[1];
ResultsSets[iNoRes]->TYPE=Vals[2];
ResultsSets[iNoRes]->LC=Vals[3];
ResultsSets[iNoRes]->WID=Vals[6];
CString sEL;
BOOL isGood=FALSE;

if (Vals[2]==33)
{
  isGood=TRUE;
  sEL="FORCE CQUAD4";
}
else if (Vals[2]==74)
{
  isGood=TRUE;
  sEL="FORCE CTRIA3";
}
else if (Vals[2]==2)
{
  sEL="FORCE CBEAM (not read)";
  ResultsSets[iNoRes]->sName=sEL;
}
else if (Vals[2]==102)
{
  sEL="FORCE CBUSH";
}
else if (Vals[2]==34)
{
  sEL="FORCE CBAR";
}
else if (Vals[2]==95)
{
  sEL="FAILURE INDICES CQUAD4";
}
else if (Vals[2]==97)
{
  sEL="FAILURE INDICES CTRIA3";
}
else if (Vals[2]==1)
{
  sEL="FORCE CROD";
}

if ((iCnt>7) && (isGood))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=8;
  ResultsSets[iNoRes]->lab[0]="FX";
  ResultsSets[iNoRes]->lab[1]="FY";
  ResultsSets[iNoRes]->lab[2]="FXY";
  ResultsSets[iNoRes]->lab[3]="MX";
  ResultsSets[iNoRes]->lab[4]="MY";
  ResultsSets[iNoRes]->lab[5]="MXY";
  ResultsSets[iNoRes]->lab[6]="TX";
  ResultsSets[iNoRes]->lab[7]="TY";
  for (i=7;i<iCnt;i+=9)
  {
    Res8* pRes=new Res8;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>5) && ((ResultsSets[iNoRes]->TYPE==95) || (ResultsSets[iNoRes]->TYPE==97)))
{ //failure index
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=5;
  ResultsSets[iNoRes]->lab[0]="Lamina number";
  ResultsSets[iNoRes]->lab[1]="Failure index for direct stresses";
  ResultsSets[iNoRes]->lab[2]="Failure mode for maximum strain theory";
  ResultsSets[iNoRes]->lab[3]="Failure index for interlaminar shear stress or -1";
  ResultsSets[iNoRes]->lab[4]="Maximum of FP and FB or -1.";
  ResultsSets[iNoRes]->sOpName="Layer ";
  ResultsSets[iNoRes]->iDefID=1;
  ResultsSets[iNoRes]->iSecondaryID=0;
  int ID;
  char* ss;
  CString S;
  for (i=7;i<iCnt;i+=9)
  {
    Res5* pRes=new Res5;
	if (Vals[i]!=-1)
	  ID=Vals[i];
    pRes->ID=ID;
    pRes->v[0]=Vals[i+3];
	pRes->v[1]=*(float*) &Vals[i+4];

	ss= (char*) &Vals[i+5];
	S="";S+=ss[0];S+=ss[1];S+=ss[2];S+=ss[3];
    pRes->v[2]=atof(S); //Was this??
	if (Vals[i+6]==-1)
       pRes->v[3]=Vals[i+6];
	else
       pRes->v[3]=*(float*) &Vals[i+6];
    if (Vals[i+7]==-1)
      pRes->v[4]=Vals[i+7];
	else
      pRes->v[4]=*(float*) &Vals[i+7];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>5) && (ResultsSets[iNoRes]->TYPE==102))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=6;
  ResultsSets[iNoRes]->lab[0]="FX";
  ResultsSets[iNoRes]->lab[1]="FY";
  ResultsSets[iNoRes]->lab[2]="FZ";
  ResultsSets[iNoRes]->lab[3]="MX";
  ResultsSets[iNoRes]->lab[4]="MY";
  ResultsSets[iNoRes]->lab[5]="MZ";

  for (i=7;i<iCnt;i+=7)
  {
    Res6* pRes=new Res6;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>5) && (ResultsSets[iNoRes]->TYPE==34))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=8;
  ResultsSets[iNoRes]->lab[0]="BM1A";
  ResultsSets[iNoRes]->lab[1]="BM2A";
  ResultsSets[iNoRes]->lab[2]="BM1B";
  ResultsSets[iNoRes]->lab[3]="BM2B";
  ResultsSets[iNoRes]->lab[4]="TS1";
  ResultsSets[iNoRes]->lab[5]="TS2";
  ResultsSets[iNoRes]->lab[6]="AF";
  ResultsSets[iNoRes]->lab[7]="TRQ";
  for (i=7;i<iCnt;i+=9)
  {
    Res8* pRes=new Res8;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
	pRes->v[4]=*(float*) &Vals[i+7];
    pRes->v[5]=*(float*) &Vals[i+8];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>5) && (ResultsSets[iNoRes]->TYPE==1))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=2;
  ResultsSets[iNoRes]->lab[0]="Axial Force";
  ResultsSets[iNoRes]->lab[1]="Torque";

  for (i=7;i<iCnt;i+=9)
  {
    Res8* pRes=new Res8;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
iNoRes++;
}

void ME_Object::AddOES1Res(int Vals[],int iCnt,CString sTitle,CString sSubTitle,CString inName)
{
int i;

ResultsSets[iNoRes]=new ResSet();
ResultsSets[iNoRes]->sFile=inName;
ResultsSets[iNoRes]->sTitle=sTitle;
ResultsSets[iNoRes]->sSubTitle=sSubTitle;
ResultsSets[iNoRes]->ACODE=Vals[0];
ResultsSets[iNoRes]->TCODE=Vals[1];
ResultsSets[iNoRes]->TYPE=Vals[2];
ResultsSets[iNoRes]->LC=Vals[3];
ResultsSets[iNoRes]->WID=Vals[6];
CString sEL;
BOOL isGood=FALSE;

if (Vals[2]==33)
{
  isGood=TRUE;
  sEL="STRESS CENTRE CQUAD4";
  ResDef* pVT;
  pVT = new ResDef();
  pVT->sResType = "2d EL STRESSES (Z1 Bot Surf)";
  pVT->iResType = 3;   //2d Tenspr
  pVT->iLoc = 1;       //Element Centroid(cys global)
  pVT->iComponents[0] = 1;  //X
  pVT->iComponents[1] = 2;  //Y
  pVT->iComponents[2] = 3;  //XY
  pVT->iComponents[3] = 5;  //P1
  pVT->iComponents[4] = 6;  //P2
  pVT->iCompNo = 5;
  pVT->GenDefualtHeaders();
  ResultsSets[iNoRes]->AddResDef(pVT);
  pVT = new ResDef();
  pVT->sResType = "2d EL STRESSES (Z2 Top Surf)";
  pVT->iResType = 3;   //2d Tenspr
  pVT->iLoc = 1;       //Element Centroid(cys global)
  pVT->iComponents[0] = 10;  //X
  pVT->iComponents[1] = 11;  //Y
  pVT->iComponents[2] = 12;  //XY
  pVT->iComponents[3] = 14;  //P1
  pVT->iComponents[4] = 15;  //P2
  pVT->iCompNo = 5;
  pVT->GenDefualtHeaders();
  ResultsSets[iNoRes]->AddResDef(pVT);

}
else if (Vals[2]==74)
{
  isGood=TRUE;
  sEL="STRESS CENTRE CTRIA3";
}
else if (Vals[2]==1)
{
  sEL="STRESS CROD";
}
else if (Vals[2]==102)
{
  sEL="STRESS CBUSH";
}
else if (Vals[2]==34)
{
  sEL="STRESS CBAR";
}
else if (Vals[2]==67)
{
  sEL="STRESS CHEXA";
  ResDef* pVT = new ResDef();
  pVT->sResType = "Stress Tensor CHEXA";
  pVT->iResType = 4;   //3d Tenspr
  pVT->iLoc = 1;       //Element Centroid(cys global)
  pVT->iComponents[0] = 1;
  pVT->iComponents[1] = 9;
  pVT->iComponents[2] = 15;
  pVT->iComponents[3] = 2;
  pVT->iComponents[4] = 10;
  pVT->iComponents[5] = 16;
  pVT->iComponents[6] = 3;
  pVT->iComponents[7] = 11;
  pVT->iComponents[8] = 17;
  pVT->iCompNo = 9;
  pVT->GenDefualtHeaders();
  ResDef* pVR = new ResDef();
  ResultsSets[iNoRes]->AddResDef(pVT);
}
else if (Vals[2]==39)
{
  sEL="STRESS CTETRA";
}
else if (Vals[2]==68)
{
  sEL="STRESS CPENTA";
}
else if (Vals[2]==95)
{
  sEL="STRESS CQUAD LAYERED";
}
else if (Vals[2]==97)
{
  sEL="STRESS CTRIA LAYERED";
}
if ((iCnt>7) && (isGood))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=18;
  ResultsSets[iNoRes]->lab[0]="Z1 = Fibre distance";
  ResultsSets[iNoRes]->lab[1]="Normal in x at Z1";
  ResultsSets[iNoRes]->lab[2]="Normal in y at Z1";
  ResultsSets[iNoRes]->lab[3]="Shear in xy at Z1";
  ResultsSets[iNoRes]->lab[4]="Theta ( Shear Angle ) at Z1";
  ResultsSets[iNoRes]->lab[5]="Major Principal at Z1";
  ResultsSets[iNoRes]->lab[6]="Minor Principal at Z1";
  ResultsSets[iNoRes]->lab[7]="Maximum Shear at Z1";
  ResultsSets[iNoRes]->lab[8]="Derived VM Stress Z1";
  ResultsSets[iNoRes]->lab[9]="Z2 = Fibre distance";
  ResultsSets[iNoRes]->lab[10]="Normal in x at Z2";
  ResultsSets[iNoRes]->lab[11]="Normal in y at Z1";
  ResultsSets[iNoRes]->lab[12]="Shear in xy at Z2";
  ResultsSets[iNoRes]->lab[13]="Theta ( Shear Angle ) at Z2";
  ResultsSets[iNoRes]->lab[14]="Major Principal at Z2";
  ResultsSets[iNoRes]->lab[15]="Minor Principal at Z2";
  ResultsSets[iNoRes]->lab[16]="Maximum Shear at Z2";
  ResultsSets[iNoRes]->lab[17]="Derived VM Stress Z2";
  double p1;
  double p2;
  double vm;
  for (i=7;i<iCnt;i+=17)
  {
    Res18* pRes=new Res18;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
	//Derive VM stress from principles
	p1=*(float*) &Vals[i+6];
	p2=*(float*) &Vals[i+7];
	vm=pow(p1*p1-p1*p2+p2*p2,0.5);
	pRes->v[7] = 0.5*(p1-p2);
	pRes->v[8]=vm;
	pRes->v[9]=*(float*) &Vals[i+9];
	pRes->v[10]=*(float*) &Vals[i+10];
	pRes->v[11]=*(float*) &Vals[i+11];
	pRes->v[12]=*(float*) &Vals[i+12];
	pRes->v[13]=*(float*) &Vals[i+13];
	pRes->v[14]=*(float*) &Vals[i+14];
	pRes->v[15]=*(float*) &Vals[i+15];
	pRes->v[16]=*(float*) &Vals[i+16];
	p1=*(float*) &Vals[i+14];
	p2=*(float*) &Vals[i+15];
	vm=pow(p1*p1-p1*p2+p2*p2,0.5);
	pRes->v[17]=vm;
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==34))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=15;
  ResultsSets[iNoRes]->lab[0]="SA1";
  ResultsSets[iNoRes]->lab[1]="SA2";
  ResultsSets[iNoRes]->lab[2]="SA3";
  ResultsSets[iNoRes]->lab[3]="SA4";
  ResultsSets[iNoRes]->lab[4]="Axial";
  ResultsSets[iNoRes]->lab[5]="SA maximum";
  ResultsSets[iNoRes]->lab[6]="SA minimum";
  ResultsSets[iNoRes]->lab[7]="Safety margin in tension*";
  ResultsSets[iNoRes]->lab[8]="SB1";
  ResultsSets[iNoRes]->lab[9]="SB2";
  ResultsSets[iNoRes]->lab[10]="SB3";
  ResultsSets[iNoRes]->lab[11]="SB4";
  ResultsSets[iNoRes]->lab[12]="SB maximum";
  ResultsSets[iNoRes]->lab[13]="SB minimum";
  ResultsSets[iNoRes]->lab[14]="Safety margin in comp*";
  for (i=7;i<iCnt;i+=16)
  {
    Res15* pRes=new Res15;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
	pRes->v[8]=*(float*) &Vals[i+9];
	pRes->v[9]=*(float*) &Vals[i+10];
	pRes->v[10]=*(float*) &Vals[i+11];
	pRes->v[11]=*(float*) &Vals[i+12];
	pRes->v[12]=*(float*) &Vals[i+13];
	pRes->v[13]=*(float*) &Vals[i+14];
	pRes->v[14]=*(float*) &Vals[i+15];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && ((ResultsSets[iNoRes]->TYPE==95) || (ResultsSets[iNoRes]->TYPE==97)))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=10;
  ResultsSets[iNoRes]->sOpName="Layer ";
  ResultsSets[iNoRes]->iDefID=1;
  ResultsSets[iNoRes]->iSecondaryID=0;
  ResultsSets[iNoRes]->lab[0]="Lamina Number";
  ResultsSets[iNoRes]->lab[1]="Normal-1";
  ResultsSets[iNoRes]->lab[2]="Normal-2";
  ResultsSets[iNoRes]->lab[3]="Shear-12";
  ResultsSets[iNoRes]->lab[4]="Shear-1Z";
  ResultsSets[iNoRes]->lab[5]="Shear-2Z";
  ResultsSets[iNoRes]->lab[6]="Shear angle";
  ResultsSets[iNoRes]->lab[7]="Major principal*";
  ResultsSets[iNoRes]->lab[8]="Minor principal";
  ResultsSets[iNoRes]->lab[9]="von Mises or Maximum shear";
  for (i=7;i<iCnt;i+=11)
  {
    Res15* pRes=new Res15;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]= Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
	pRes->v[8]=*(float*) &Vals[i+9];
	pRes->v[9]=*(float*) &Vals[i+10];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==102))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=6;
  ResultsSets[iNoRes]->lab[0]="Translation x";
  ResultsSets[iNoRes]->lab[1]="Translation y";
  ResultsSets[iNoRes]->lab[2]="Translation z";
  ResultsSets[iNoRes]->lab[3]="Rotation x";
  ResultsSets[iNoRes]->lab[4]="Rotation y";
  ResultsSets[iNoRes]->lab[5]="Rotation z";
  for (i=7;i<iCnt;i+=7)
  {
    Res6* pRes=new Res6;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==1))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=4;
  ResultsSets[iNoRes]->lab[0]="Axial strain";
  ResultsSets[iNoRes]->lab[1]="Axial safety margin*";
  ResultsSets[iNoRes]->lab[2]="Torsional strain";
  ResultsSets[iNoRes]->lab[3]="Torsional safety margin*";

  for (i=7;i<iCnt;i+=5)
  {
    Res4* pRes=new Res4;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==67))    //CHEXA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=189;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[109-4] = "GRID5 ID";
  ResultsSets[iNoRes]->lab[130-4] = "GRID6 ID";
  ResultsSets[iNoRes]->lab[151-4] = "GRID7 ID";
  ResultsSets[iNoRes]->lab[172-4] = "GRID8 ID";

  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal stress";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear stress";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal stress";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal stress";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=193)
  {
    Res189* pRes=new Res189;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<193;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88) || (j==109) || (j==130) || (j==151) || (j==172))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==68))    //CPENTA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=21;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[109-4] = "GRID5 ID";
  ResultsSets[iNoRes]->lab[130-4] = "GRID6 ID";
  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal stress";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear stress";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal stress";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal stress";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=151)
  {
    Res147* pRes=new Res147;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<151;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88) || (j==109) || (j==130))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==39))    //CTETRA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=21;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal stress";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear stress";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal stress";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal stress";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=109)
  {
    Res105* pRes=new Res105;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<109;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
iNoRes++;
}


void ME_Object::AddOSTRRes(int Vals[],int iCnt,CString sTitle,CString sSubTitle,CString inName)
{
int i;

ResultsSets[iNoRes]=new ResSet();
ResultsSets[iNoRes]->sFile=inName;
ResultsSets[iNoRes]->sTitle=sTitle;
ResultsSets[iNoRes]->sSubTitle=sSubTitle;
ResultsSets[iNoRes]->ACODE=Vals[0];
ResultsSets[iNoRes]->TCODE=Vals[1];
ResultsSets[iNoRes]->TYPE=Vals[2];
ResultsSets[iNoRes]->LC=Vals[3];
ResultsSets[iNoRes]->WID=Vals[6];
CString sEL;
BOOL isGood=FALSE;

if (Vals[2]==33)
{
  isGood=TRUE;
  sEL="STRAIN CQUAD4";
}
else if (Vals[2]==74)
{
  isGood=TRUE;
  sEL="STRAIN CTRIA3";
}
else if (Vals[2]==1)
{
  sEL="STRAIN CROD";
}
else if (Vals[2]==102)
{
  sEL="STRAIN CBUSH";
}
else if (Vals[2]==34)
{
  sEL="STRAIN CBAR";
}
else if (Vals[2]==67)
{
  sEL="STRAIN CHEXA";
}
else if (Vals[2]==39)
{
  sEL="STRAIN CTETRA";
}
else if (Vals[2]==68)
{
  sEL="STRAIN CPENTA";
}
else if (Vals[2]==95)
{
  sEL="STRAIN CQUAD LAYERED";
}
else if (Vals[2]==97)
{
  sEL="STRAIN CTRIA LAYERED";
}
if ((iCnt>7) && (isGood))
{


  double dX;
  double dY;
  double dXY;
  double vm;
  bool isLayered = FALSE;
  for (i=7;i<iCnt;i+=17)
  {
    Res18* pRes=new Res18;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
	if (i == 7) //Determine if data is Layered or Centre
	{
		if (pRes->v[0] != 0)
			isLayered = TRUE;
	}
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4]*0.5;
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
	//Derive VM stress from principles
	dX = *(float*) &Vals[i + 2];
	dY = *(float*) &Vals[i + 3];
	dXY = *(float*) &Vals[i + 4];
	vm= pow(0.44444444*(dX*dX+dY*dY-dX*dY)+ 0.33333333*(dXY*dXY), 0.5);
	pRes->v[8]=vm;
	pRes->v[9]=*(float*) &Vals[i+9];
	pRes->v[10]=*(float*) &Vals[i+10];
	pRes->v[11]=*(float*) &Vals[i+11];
	pRes->v[12]=*(float*) &Vals[i+12]*0.5;
	pRes->v[13]=*(float*) &Vals[i+13];
	pRes->v[14]=*(float*) &Vals[i+14];
	pRes->v[15]=*(float*) &Vals[i+15];
	pRes->v[16]=*(float*) &Vals[i+16];
	dX = *(float*)&Vals[i + 10];
	dY = *(float*)&Vals[i + 11];
	dXY = *(float*)&Vals[i + 12];
	vm = pow(0.44444444*(dX*dX + dY * dY - dX * dY) + 0.33333333*(dXY*dXY), 0.5);
	pRes->v[17]=vm;
    ResultsSets[iNoRes]->Add(pRes);
  }
  //Headers
  if (isLayered)
  {
	  ResultsSets[iNoRes]->sName = sEL;
	  ResultsSets[iNoRes]->iNoV = 18;
	  ResultsSets[iNoRes]->lab[0] = "(Z1 Dist)";
	  ResultsSets[iNoRes]->lab[1] = "(Z1) Normal in x";
	  ResultsSets[iNoRes]->lab[2] = "(Z1) Normal in y";
	  ResultsSets[iNoRes]->lab[3] = "(Z1) Shear in xy";
	  ResultsSets[iNoRes]->lab[4] = "(Z1) Theta (Shear Angle)";
	  ResultsSets[iNoRes]->lab[5] = "(Z1) Major Principal";
	  ResultsSets[iNoRes]->lab[6] = "(Z1) Minor Principal";
	  ResultsSets[iNoRes]->lab[7] = "(Z1) Maximum Shear";
	  ResultsSets[iNoRes]->lab[8] = "(Z1) VM Strain";
	  ResultsSets[iNoRes]->lab[9] = "(Z2 Dist)";
	  ResultsSets[iNoRes]->lab[10] = "(Z2) Normal in x";
	  ResultsSets[iNoRes]->lab[11] = "(Z2) Normal in y";
	  ResultsSets[iNoRes]->lab[12] = "(Z2) Shear in xy";
	  ResultsSets[iNoRes]->lab[13] = "(Z2) Theta (Shear Angle)";
	  ResultsSets[iNoRes]->lab[14] = "(Z2) Major Principal";
	  ResultsSets[iNoRes]->lab[15] = "(Z2) Minor Principal";
	  ResultsSets[iNoRes]->lab[16] = "(Z2) Maximum Shear";
	  ResultsSets[iNoRes]->lab[17] = "(Z2) VM Strain";
  }
  else
  {
	  ResultsSets[iNoRes]->sName = sEL;
	  ResultsSets[iNoRes]->iNoV = 18;
	  ResultsSets[iNoRes]->lab[0] = "(Dist)";
	  ResultsSets[iNoRes]->lab[1] = "(Mid) Normal in x";
	  ResultsSets[iNoRes]->lab[2] = "(Mid) Normal in y";
	  ResultsSets[iNoRes]->lab[3] = "(Mid) Shear in xy";
	  ResultsSets[iNoRes]->lab[4] = "(Mid) Theta (Shear Angle)";
	  ResultsSets[iNoRes]->lab[5] = "(Mid) Major Principal";
	  ResultsSets[iNoRes]->lab[6] = "(Mid) Minor Principal";
	  ResultsSets[iNoRes]->lab[7] = "(Mid) Maximum Shear";
	  ResultsSets[iNoRes]->lab[8] = "(Mid) VM Strain";
	  ResultsSets[iNoRes]->lab[9] = "(Dist)";
	  ResultsSets[iNoRes]->lab[10] = "(Mid Curvature) Normal in x";
	  ResultsSets[iNoRes]->lab[11] = "(Mid Curvature) Normal in y";
	  ResultsSets[iNoRes]->lab[12] = "(Mid Curvature) Shear in xy";
	  ResultsSets[iNoRes]->lab[13] = "(Mid Curvature) Theta (Shear Angle)";
	  ResultsSets[iNoRes]->lab[14] = "(Mid Curvature) Major Principal";
	  ResultsSets[iNoRes]->lab[15] = "(Mid Curvature) Minor Principal";
	  ResultsSets[iNoRes]->lab[16] = "(Mid Curvature) Maximum Shear";
	  ResultsSets[iNoRes]->lab[17] = "(Mid Curvature) VM Strain";
  }
  //Results vector definition
  ResDef* pVT;
  pVT = new ResDef();
  if (isLayered)
    pVT->sResType = "(Z1) 2d STRAIN";
  else
	pVT->sResType = "(Mid) 2d STRAIN";
  pVT->iResType = 3;   //2d Tensor
  pVT->iLoc = 1;       //Element Centroid(cys global)
  pVT->iComponents[0] = 1;
  pVT->iComponents[1] = 2;
  pVT->iComponents[2] = 3;
  pVT->iComponents[3] = 5;
  pVT->iComponents[4] = 6;
  pVT->iCompNo = 5;
  pVT->GenDefualtHeaders();
  ResultsSets[iNoRes]->AddResDef(pVT);
  pVT = new ResDef();
  if (isLayered)
	  pVT->sResType = "(Z2) 2d STRAIN";
  else
	  pVT->sResType = "(Mid Curvature) 2d STRAIN";
  pVT->iResType = 3;   //2d Tensor
  pVT->iLoc = 1;       //Element Centroid(cys global)
  pVT->iComponents[0] = 10;
  pVT->iComponents[1] = 11;
  pVT->iComponents[2] = 12;
  pVT->iComponents[3] = 14;
  pVT->iComponents[4] = 15;
  pVT->iCompNo = 5;
  pVT->GenDefualtHeaders();
  ResultsSets[iNoRes]->AddResDef(pVT);
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==34))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=15;
  ResultsSets[iNoRes]->lab[0]="SA1";
  ResultsSets[iNoRes]->lab[1]="SA2";
  ResultsSets[iNoRes]->lab[2]="SA3";
  ResultsSets[iNoRes]->lab[3]="SA4";
  ResultsSets[iNoRes]->lab[4]="Axial";
  ResultsSets[iNoRes]->lab[5]="SA maximum";
  ResultsSets[iNoRes]->lab[6]="SA minimum";
  ResultsSets[iNoRes]->lab[7]="Safety margin in tension*";
  ResultsSets[iNoRes]->lab[8]="SB1";
  ResultsSets[iNoRes]->lab[9]="SB2";
  ResultsSets[iNoRes]->lab[10]="SB3";
  ResultsSets[iNoRes]->lab[11]="SB4";
  ResultsSets[iNoRes]->lab[12]="SB maximum";
  ResultsSets[iNoRes]->lab[13]="SB minimum";
  ResultsSets[iNoRes]->lab[14]="Safety margin in comp*";
  for (i=7;i<iCnt;i+=16)
  {
    Res15* pRes=new Res15;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
	pRes->v[8]=*(float*) &Vals[i+9];
	pRes->v[9]=*(float*) &Vals[i+10];
	pRes->v[10]=*(float*) &Vals[i+11];
	pRes->v[11]=*(float*) &Vals[i+12];
	pRes->v[12]=*(float*) &Vals[i+13];
	pRes->v[13]=*(float*) &Vals[i+14];
	pRes->v[14]=*(float*) &Vals[i+15];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && ((ResultsSets[iNoRes]->TYPE==95) || (ResultsSets[iNoRes]->TYPE==97)))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=10;
  ResultsSets[iNoRes]->sOpName="Layer ";
  ResultsSets[iNoRes]->iDefID=1;
ResultsSets[iNoRes]->iSecondaryID=0;
  ResultsSets[iNoRes]->lab[0]="Lamina Number";
  ResultsSets[iNoRes]->lab[1]="Normal-1";
  ResultsSets[iNoRes]->lab[2]="Normal-2";
  ResultsSets[iNoRes]->lab[3]="Shear-12";
  ResultsSets[iNoRes]->lab[4]="Shear-1Z";
  ResultsSets[iNoRes]->lab[5]="Shear-2Z";
  ResultsSets[iNoRes]->lab[6]="Shear angle";
  ResultsSets[iNoRes]->lab[7]="Major principal*";
  ResultsSets[iNoRes]->lab[8]="Minor principal";
  ResultsSets[iNoRes]->lab[9]="von Mises or Maximum shear";
  for (i=7;i<iCnt;i+=11)
  {
    Res15* pRes=new Res15;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]= Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    pRes->v[6]=*(float*) &Vals[i+7];
    pRes->v[7]=*(float*) &Vals[i+8];
	pRes->v[8]=*(float*) &Vals[i+9];
	pRes->v[9]=*(float*) &Vals[i+10];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==102))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=6;
  ResultsSets[iNoRes]->lab[0]="Translation x";
  ResultsSets[iNoRes]->lab[1]="Translation y";
  ResultsSets[iNoRes]->lab[2]="Translation z";
  ResultsSets[iNoRes]->lab[3]="Rotation x";
  ResultsSets[iNoRes]->lab[4]="Rotation y";
  ResultsSets[iNoRes]->lab[5]="Rotation z";
  for (i=7;i<iCnt;i+=7)
  {
    Res6* pRes=new Res6;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    pRes->v[4]=*(float*) &Vals[i+5];
    pRes->v[5]=*(float*) &Vals[i+6];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==1))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=4;
  ResultsSets[iNoRes]->lab[0]="Axial strain";
  ResultsSets[iNoRes]->lab[1]="Axial safety margin*";
  ResultsSets[iNoRes]->lab[2]="Torsional strain";
  ResultsSets[iNoRes]->lab[3]="Torsional safety margin*";

  for (i=7;i<iCnt;i+=5)
  {
    Res4* pRes=new Res4;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    pRes->v[3]=*(float*) &Vals[i+4];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==67))    //CHEXA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=189;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[109-4] = "GRID5 ID";
  ResultsSets[iNoRes]->lab[130-4] = "GRID6 ID";
  ResultsSets[iNoRes]->lab[151-4] = "GRID7 ID";
  ResultsSets[iNoRes]->lab[172-4] = "GRID8 ID";

  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal strain";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear strain";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal strain";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal strain";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=193)
  {
    Res189* pRes=new Res189;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<193;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88) || (j==109) || (j==130) || (j==151) || (j==172))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==68))    //CPENTA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=21;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[109-4] = "GRID5 ID";
  ResultsSets[iNoRes]->lab[130-4] = "GRID6 ID";
  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal strain";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear strain";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal strain";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal strain";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=151)
  {
    Res147* pRes=new Res147;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<151;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88) || (j==109) || (j==130))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
if ((iCnt>7) && (ResultsSets[iNoRes]->TYPE==39))    //CTETRA LIN STRESS
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=21;
  ResultsSets[iNoRes]->lab[4-4] = "GRID0 ID";
  ResultsSets[iNoRes]->lab[25-4] = "GRID1 ID";
  ResultsSets[iNoRes]->lab[46-4] = "GRID2 ID";
  ResultsSets[iNoRes]->lab[67-4] = "GRID3 ID";
  ResultsSets[iNoRes]->lab[88-4] = "GRID4 ID";
  ResultsSets[iNoRes]->lab[1] = "Normal in x";
  ResultsSets[iNoRes]->lab[2] = "Shear in xy";
  ResultsSets[iNoRes]->lab[3] = "First principal strain";
  ResultsSets[iNoRes]->lab[4] = "First principal x cosine";
  ResultsSets[iNoRes]->lab[5] = "Second principal x cosine";
  ResultsSets[iNoRes]->lab[6] = "Third principal x cosine";
  ResultsSets[iNoRes]->lab[7] = "Mean pressure";
  ResultsSets[iNoRes]->lab[8] = "Octahedral shear strain";
  ResultsSets[iNoRes]->lab[9] = "Normal in y";
  ResultsSets[iNoRes]->lab[10] = "Shear in yz";
  ResultsSets[iNoRes]->lab[11] = "Second principal strain";
  ResultsSets[iNoRes]->lab[12] = "First principal y cosine";
  ResultsSets[iNoRes]->lab[13] = "Second principal y cosine";
  ResultsSets[iNoRes]->lab[14] = "Third principal y cosine";
  ResultsSets[iNoRes]->lab[15] = "Normal in z";
  ResultsSets[iNoRes]->lab[16] = "Shear in zx";
  ResultsSets[iNoRes]->lab[17] = "Third principal strain";
  ResultsSets[iNoRes]->lab[18] = "First principal z cosine";
  ResultsSets[iNoRes]->lab[19] = "Second principal z cosine";
  ResultsSets[iNoRes]->lab[20] = "Third principal z cosine";
  int j;
  for (i=7;i<iCnt;i+=109)
  {
    Res105* pRes=new Res105;
    pRes->ID=Vals[i] / 10;
    pRes->iSID=Vals[i+1];
    //pRes->sType="null";//Vals[i+2]
    pRes->iNoActive=Vals[i+3];
	for (j=4;j<109;j++)
	{
	  if ((j==4) || (j==25) || (j==46) || (j==67) || (j==88))
	  {
		pRes->v[j-4]=Vals[i+j];
	  }
	  else
	  {
        pRes->v[j-4]=*(float*) &Vals[i+j];
	  }
	}
    ResultsSets[iNoRes]->Add(pRes);
  }
}
iNoRes++;
}

void ME_Object::AddONRGRes(int Vals[],int iCnt,CString sTitle,CString sSubTitle,CString inName)
{
int i;
if (iCnt>5)
{
ResultsSets[iNoRes]=new ResSet();
ResultsSets[iNoRes]->sFile=inName;
ResultsSets[iNoRes]->sTitle=sTitle;
ResultsSets[iNoRes]->sSubTitle=sSubTitle;
ResultsSets[iNoRes]->ACODE=Vals[0];
ResultsSets[iNoRes]->TCODE=Vals[1];
ResultsSets[iNoRes]->TYPE=Vals[2];
ResultsSets[iNoRes]->LC=Vals[3];
ResultsSets[iNoRes]->WID=Vals[6];
CString sEL;
BOOL isGood=FALSE;
if (Vals[2]==90)
{
  isGood=TRUE;
  sEL="STRAIN ENERGY CQUAD4";
}
else if (Vals[2]==88)
{
  isGood=TRUE;
  sEL="STRAIN ENERGY CTRIA3";
}

isGood = TRUE;
sEL = "STRAIN ENERGY";

if ((iCnt>7) && (isGood))
{
  ResultsSets[iNoRes]->sName=sEL;
  ResultsSets[iNoRes]->iNoV=3;
  ResultsSets[iNoRes]->lab[0]="Energy";
  ResultsSets[iNoRes]->lab[1]="Energy % Tot";
  ResultsSets[iNoRes]->lab[2]="Energy Density";
  for (i=7;i<iCnt;i+=4)
  {
    Res3* pRes=new Res3;
    pRes->ID=Vals[i] / 10;
    pRes->v[0]=*(float*) &Vals[i+1];
    pRes->v[1]=*(float*) &Vals[i+2];
    pRes->v[2]=*(float*) &Vals[i+3];
    ResultsSets[iNoRes]->Add(pRes);
  }
}
iNoRes++;
}
}

void ME_Object::AddOESNRes(int Vals[], int iCnt, CString sTitle, CString sSubTitle, CString inName)
{
  int i;
  if (iCnt>5)
  {
    ResultsSets[iNoRes] = new ResSet();
    ResultsSets[iNoRes]->sFile = inName;
    ResultsSets[iNoRes]->sTitle = sTitle;
    ResultsSets[iNoRes]->sSubTitle = sSubTitle;
    ResultsSets[iNoRes]->ACODE = Vals[0];
    ResultsSets[iNoRes]->TCODE = Vals[1];
    ResultsSets[iNoRes]->TYPE = Vals[2];
    ResultsSets[iNoRes]->LC = Vals[3];
    ResultsSets[iNoRes]->WID = Vals[6];
    CString sEL;
    BOOL isGood = FALSE;
    if (Vals[2] == 90)
    {
      isGood = TRUE;
      sEL = "STRESS NONLIN CENTRE CQUAD4";
    }
    else if (Vals[2] == 88)
    {
      isGood = TRUE;
      sEL = "STRESS NONLIN CENTRE CTRIA3";
    }

    if ((iCnt>7) && (isGood))
    {
      ResultsSets[iNoRes]->sName = sEL;
      ResultsSets[iNoRes]->iNoV = 24;
      ResultsSets[iNoRes]->lab[0] = "Z1 = Fibre distance";
      ResultsSets[iNoRes]->lab[1] = "Stress in x at Z1";
      ResultsSets[iNoRes]->lab[2] = "Stress in y at Z1";
      ResultsSets[iNoRes]->lab[3] = "UNDEF";
      ResultsSets[iNoRes]->lab[4] = "Shear stress in xy at Z1";
      ResultsSets[iNoRes]->lab[5] = "Equivalent stress at Z1";
      ResultsSets[iNoRes]->lab[6] = "Effective plastic/nlelastic strain at Z1";
      ResultsSets[iNoRes]->lab[7] = "Effective creep strain at Z1";
      ResultsSets[iNoRes]->lab[8] = "Strain in x at Z1";
      ResultsSets[iNoRes]->lab[9] = "Strain in y at Z1";
      ResultsSets[iNoRes]->lab[10] = "UNDEF";
      ResultsSets[iNoRes]->lab[11] = "Shear strain in xy at Z1";
      ResultsSets[iNoRes]->lab[12] = "Z2 = Fibre distance";
      ResultsSets[iNoRes]->lab[13] = "Stress in x at Z2";
      ResultsSets[iNoRes]->lab[14] = "Stress in y at Z2";
      ResultsSets[iNoRes]->lab[15] = "UNDEF";
      ResultsSets[iNoRes]->lab[16] = "Shear stress in xy at Z2";
      ResultsSets[iNoRes]->lab[17] = "Equivalent stress at Z2";
      ResultsSets[iNoRes]->lab[18] = "Effective plastic/nlelastic strain at Z2";
      ResultsSets[iNoRes]->lab[19] = "Effective creep strain at Z2";
      ResultsSets[iNoRes]->lab[20] = "Strain in x at Z2";
      ResultsSets[iNoRes]->lab[21] = "Strain in y at Z2";
      ResultsSets[iNoRes]->lab[22] = "UNDEF";
      ResultsSets[iNoRes]->lab[23] = "Shear strain in xy at Z2";
      for (i = 7; i<iCnt; i += 25)
      {
        Res24* pRes = new Res24;
        pRes->ID = Vals[i] / 10;
        pRes->v[0] = *(float*)&Vals[i + 1];
        pRes->v[1] = *(float*)&Vals[i + 2];
        pRes->v[2] = *(float*)&Vals[i + 3];
        pRes->v[3] = *(float*)&Vals[i + 4];
        pRes->v[4] = *(float*)&Vals[i + 5];
        pRes->v[5] = *(float*)&Vals[i + 6];
        pRes->v[6] = *(float*)&Vals[i + 7];
        pRes->v[7] = *(float*)&Vals[i + 8];
        pRes->v[8] = *(float*)&Vals[i + 9];
        pRes->v[9] = *(float*)&Vals[i + 10];
        pRes->v[10] = *(float*)&Vals[i + 11];
        pRes->v[11] = *(float*)&Vals[i + 12];
        pRes->v[12] = *(float*)&Vals[i + 13];
        pRes->v[13] = *(float*)&Vals[i + 14];
        pRes->v[14] = *(float*)&Vals[i + 15];
        pRes->v[15] = *(float*)&Vals[i + 16];
        pRes->v[16] = *(float*)&Vals[i + 17];
        pRes->v[17] = *(float*)&Vals[i + 18];
        pRes->v[18] = *(float*)&Vals[i + 19];
        pRes->v[19] = *(float*)&Vals[i + 20];
        pRes->v[20] = *(float*)&Vals[i + 21];
        pRes->v[21] = *(float*)&Vals[i + 22];
        pRes->v[22] = *(float*)&Vals[i + 23];
        pRes->v[23] = *(float*)&Vals[i + 24];
        ResultsSets[iNoRes]->Add(pRes);
      }
    }
    iNoRes++;
  }
}

void ME_Object::AddOAG1Res(int Vals[], int iCnt, CString sTitle, CString sSubTitle, CString inName)
{
	int i;
	char s30[30];
	float ModeNo;
	float ModeFreq;
	//BuildNodeList();
	C3dVector vT;
	C3dVector vR;
	ResultsSets[iNoRes] = new ResSet();
	//***********************************************
	ResultsSets[iNoRes]->sFile = inName;
	ResultsSets[iNoRes]->sTitle = sTitle;
	ResultsSets[iNoRes]->sSubTitle = sSubTitle;
	ResultsSets[iNoRes]->ACODE = Vals[0];
	ResultsSets[iNoRes]->TCODE = Vals[1];
	ResultsSets[iNoRes]->TYPE = Vals[2];
	ResultsSets[iNoRes]->LC = Vals[3];
	ModeNo = *(float*)& Vals[4];
	ModeFreq = *(float*)&Vals[5];
	ModeFreq = pow(ModeFreq, 0.5) / (3.14159265359 * 2);
	ResultsSets[iNoRes]->i1 = ModeNo;
	ResultsSets[iNoRes]->d1 = ModeNo;
	ResultsSets[iNoRes]->WID = Vals[6];

	if (iCnt > 7)
	{
		sprintf_s(s30, "%s %g %s", "ACCELERATION", ModeNo, "Hz");
		ResultsSets[iNoRes]->sName = s30;
		ResultsSets[iNoRes]->iNoV = 13;
		ResultsSets[iNoRes]->lab[0] = "Freq";
		ResultsSets[iNoRes]->lab[1] = "TX";
		ResultsSets[iNoRes]->lab[2] = "TY";
		ResultsSets[iNoRes]->lab[3] = "TZ";
		ResultsSets[iNoRes]->lab[4] = "RX";
		ResultsSets[iNoRes]->lab[5] = "RY";
		ResultsSets[iNoRes]->lab[6] = "RZ";
		ResultsSets[iNoRes]->lab[7] = "(j) X";
		ResultsSets[iNoRes]->lab[8] = "(j) Y";
		ResultsSets[iNoRes]->lab[9] = "(j) Z";
		ResultsSets[iNoRes]->lab[10] = "(j) RX";
		ResultsSets[iNoRes]->lab[11] = "(j) RY";
		ResultsSets[iNoRes]->lab[12] = "(j) RZ";
		for (i = 7; i < iCnt; i += 14)
		{
			Res13* pRes = new Res13;
			pRes->ID = Vals[i] / 10;
			pRes->v[0] = ModeNo;
			pRes->v[1] = *(float*)&Vals[i + 2];
			pRes->v[2] = *(float*)&Vals[i + 3];
			pRes->v[3] = *(float*)&Vals[i + 4];
			pRes->v[4] = *(float*)&Vals[i + 5];
			pRes->v[5] = *(float*)&Vals[i + 6];
			pRes->v[6] = *(float*)&Vals[i + 7];
			pRes->v[7] = *(float*)&Vals[i + 8];
			pRes->v[8] = *(float*)&Vals[i + 9];
			pRes->v[9] = *(float*)&Vals[i + 10];
			pRes->v[10] = *(float*)&Vals[i + 11];
			pRes->v[11] = *(float*)&Vals[i + 12];
			pRes->v[12] = *(float*)&Vals[i + 13];
			ResultsSets[iNoRes]->Add(pRes);
		}

	}
	//DeleteNodeList();
	iNoRes++;
}

void ME_Object::AddOUGRes(int Vals[], int iCnt, CString sTitle, CString sSubTitle, CString inName)
{
	int i;
	char s30[30];
	int ModeNo;
	float ModeFreq;
	BuildNodeList();
	C3dVector vT;
	C3dVector vR;
	Pt_Object* pND;
	int iRID;
	ResultsSets[iNoRes] = new ResSet();
	//**********Define the Vector********************
	ResDef* pVT = new ResDef();
	pVT->sResType = "DISP TRANS VEC";
	pVT->iResType = 1;   //Vector Translation
	pVT->iLoc = 0;       //Nodal
	pVT->iComponents[0] = 0;
	pVT->iComponents[1] = 1;
	pVT->iComponents[2] = 2;
	pVT->iComponents[3] = -1;
	pVT->iCompNo = 4;
	pVT->GenDefualtHeaders();
	ResDef* pVR = new ResDef();
	pVR->sResType = "DISP ROTA VEC";
	pVR->iResType = 1;   //Vector Rotation
	pVR->iLoc = 0;       //Nodal
	pVR->iComponents[0] = 3;
	pVR->iComponents[1] = 4;
	pVR->iComponents[2] = 5;
	pVR->iComponents[3] = -1;
	pVT->iCompNo = 4;
	pVT->GenDefualtHeaders();
	ResultsSets[iNoRes]->AddResDef(pVT);
	ResultsSets[iNoRes]->AddResDef(pVR);
	//***********************************************
	ResultsSets[iNoRes]->sFile = inName;
	ResultsSets[iNoRes]->sTitle = sTitle;
	ResultsSets[iNoRes]->sSubTitle = sSubTitle;
	ResultsSets[iNoRes]->ACODE = Vals[0];
	ResultsSets[iNoRes]->TCODE = Vals[1];
	ResultsSets[iNoRes]->TYPE = Vals[2];
	ResultsSets[iNoRes]->LC = Vals[3];
	ModeNo = Vals[4];
	ModeFreq = *(float*)&Vals[5];
	ModeFreq = pow(ModeFreq, 0.5) / (3.14159265359 * 2);
	ResultsSets[iNoRes]->i1 = ModeNo;
	ResultsSets[iNoRes]->d1 = ModeFreq;
	ResultsSets[iNoRes]->WID = Vals[6];


	if (iCnt > 7)
	{
		if (ResultsSets[iNoRes]->ACODE == 22)  //Modes
			sprintf_s(s30, "DISP MODE %g", ResultsSets[iNoRes]->d1);
		else
			sprintf_s(s30, "%s", "DISPLACEMENT");
		ResultsSets[iNoRes]->sName = s30;
		ResultsSets[iNoRes]->iNoV = 6;
		ResultsSets[iNoRes]->lab[0] = "TX";
		ResultsSets[iNoRes]->lab[1] = "TY";
		ResultsSets[iNoRes]->lab[2] = "TZ";
		ResultsSets[iNoRes]->lab[3] = "RX";
		ResultsSets[iNoRes]->lab[4] = "RY";
		ResultsSets[iNoRes]->lab[5] = "RZ";
		for (i = 7; i < iCnt; i += 8)
		{
			Res6* pRes = new Res6;
			pRes->ID = Vals[i] / 10;
			if (pRes->ID < 99999999)
				pND = (Pt_Object*)TempList->Objs[pRes->ID];
			else
				pND = GetNode(pRes->ID);
			iRID = 0;
			if (pND == NULL)
			{
				pND = GetNode(pRes->ID);
			}
			if (pND != NULL)
			{
				iRID = pND->OutSys;
			}
			vT.x = *(float*)&Vals[i + 2];
			vT.y = *(float*)&Vals[i + 3];
			vT.z = *(float*)&Vals[i + 4];
			vR.x = *(float*)&Vals[i + 5];
			vR.y = *(float*)&Vals[i + 6];
			vR.z = *(float*)&Vals[i + 7];

			if (iRID != 0)
			{
				//do
				//{
				  iRID = VecToGlobal(pND,vT, iRID);
				  iRID = VecToGlobal(pND, vR, iRID);
					//iRID=NodeToGlobal(vR,iRID);
				//} while (iRID > 0);
			}

			pRes->v[0] = vT.x;
			pRes->v[1] = vT.y;
			pRes->v[2] = vT.z;
			pRes->v[3] = vR.x;
			pRes->v[4] = vR.y;
			pRes->v[5] = vR.z;
			ResultsSets[iNoRes]->Add(pRes);
		}

	}
	DeleteNodeList();
	iNoRes++;
}


void ME_Object::ListResSets()
{
int i;
char buff[80];
outtext1("RESULTS LISTING:-");
for (i=0;i<iNoRes;i++)
{ 
  sprintf_s(buff, "%3i%s%8i%s%s",i," : LC ",ResultsSets[i]->LC," ",ResultsSets[i]->sName );
  outtext1(buff);
}
}

void ME_Object::ListVecSets()
{
	int i;
	int j;
	char buff[80];
	outtext1("AVAILABLE RESULTS VECTOR:-");
	for (i = 0; i < iNoRes; i++)
	{
		sprintf_s(buff, "%s%8i%s%8i%s%s","RES SET ",i," LC ", ResultsSets[i]->LC, " ", ResultsSets[i]->sName);
		outtext1(buff);
		if (ResultsSets[i]->iNoResDef > 0)
		{
			for (j = 0; j < ResultsSets[i]->iNoResDef; j++)
			{
			  sprintf_s(buff, "   %i,%i : %s",i,j, ResultsSets[i]->ResDefintion[j]->sResType);
			  outtext1(buff);
			}
		}
	}
}

void ME_Object::ListResSet()
{
int i;
char buff[80];
outtext1("CURRENT RESULTS LISTING:-");
if ((iCurResSet<iNoRes) && (iCurResSet>=0))
{ 
	  sprintf(buff, "%3i%s%8i%s%s",iCurResSet," : LC ",ResultsSets[iCurResSet]->LC," ",ResultsSets[iCurResSet]->sName );
    outtext1(buff);
    outtext1("Variable:-");
    for (i=0;i<ResultsSets[iCurResSet]->iNoV;i++)
    {
       sprintf_s(buff, "%3i%s%s",i," : ",ResultsSets[iCurResSet]->lab[i]);
       outtext1(buff);
    }
    sprintf_s(buff, "%s%s","Current variable : ",ResultsSets[iCurResSet]->lab[iResVal]);
    outtext1(buff);
    sprintf_s(buff, "%s%f","Max Value : ",fMaxRes);
    outtext1(buff);
    sprintf_s(buff, "%s%f","Min Value : ",fMinRes);
    outtext1(buff);
}
else
{
  outtext1("WARNING: No Results Selected.");
}
}


void ME_Object::SetCurrentResSet(int iRS,int iRV,int iOPT)
{
if ((iRS<iNoRes) && (iRV>=0))
{
  iCurResSet=iRS;
  iResVal=0;

  if ((iRV<ResultsSets[iCurResSet]->iNoV) && (iRV>=0))
  {
    iResVal=iRV;
    PostElResScalar(ResultsSets[iCurResSet],iResVal,iOPT,fMaxRes,fMinRes);

  }
  else
  {
    outtext1("WARNING: Invalid Variable ID.");
    PostElResScalar(ResultsSets[iCurResSet],iResVal,iOPT,fMaxRes,fMinRes);
  }
}
else
{
  outtext1("WARNING: Invalid LC ID.");
}

}

void ME_Object::WriteResHead(int iDspFlgs,float dW,float dH)
{
char sLab[200];
if ((iCurResSet>-1) && (CResSet!=NULL))
{
   sprintf_s(sLab,"%s",ResultsSets[iCurResSet]->sName);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.1*dH,100,&sLab[0]);
   sprintf_s(sLab,"%s %i %s","Solution :", ResultsSets[iCurResSet]->LC, ResultsSets[iCurResSet]->sTitle);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.2*dH,100,&sLab[0]);
   if (ResultsSets[iCurResSet]->ACODE==22)
	 sprintf_s(sLab, "%s %i %g Hz", "MODE :", ResultsSets[iCurResSet]->i1, ResultsSets[iCurResSet]->d1);
   else
     sprintf_s(sLab,"%s %i %s","Step :", ResultsSets[iCurResSet]->LC, ResultsSets[iCurResSet]->sSubTitle);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.3*dH,100,&sLab[0]);
   sprintf_s(sLab,"%s %s %s %i","Variable  :", ResultsSets[iCurResSet]->lab[iResVal],ResultsSets[iCurResSet]->sOpName,iPostOpt);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.4*dH,100,&sLab[0]);
   sprintf_s(sLab,"%s %f","Max Value :", ResultsSets[iCurResSet]->fMaxV);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.5*dH,100,&sLab[0]);
   sprintf_s(sLab,"%s %f","Min Value :", ResultsSets[iCurResSet]->fMinV);
   OglString(iDspFlgs,-dW+0.02*dW,dH-0.6*dH,100,&sLab[0]);
}
}

//*************************************************************
//  LIST RESPONSE DATA FOR FREQUENCY ANALYSIS
//  List repose for loadcase LC
//  and for node or element iEnt
void ME_Object::ResListRespData(int iLC, int iEnt)
{
	int i;
	int j;
	char buff[200];
	ResSet* pRS;
	Res* pR;
	CString sDL;
	BOOL bFirst = TRUE;
	outtext1("RESONSE LISTING:-");
	for (i = 0; i < iNoRes; i++)
	{
		if (ResultsSets[i]->LC == iLC)
		{
			pRS = ResultsSets[i];
			if (pRS->ACODE / 10 == 5) //Frequncy data
			{
				if (bFirst)
				{
					outtext1(pRS->sTitle);
					outtext1(pRS->sSubTitle);
					sprintf_s(buff, "%s %i", "LC:", pRS->LC);
					sDL = buff;
					outtext1(sDL);
					sDL = pRS->ToStringHead();
					outtext1(sDL);
					bFirst = FALSE;
				}

				pR = pRS->Head;
				for (j = 0; j < pRS->iCnt; j++)
				{
					if (pR->ID == iEnt)
					{
						sDL = pRS->ToStringDL(pR);
						outtext1(sDL);
					}
					pR = pR->next;
				}
			}
		}
	}
}

void ME_Object::SetDefScale(double dS)
{
  dScale=dS;
}

void ME_Object::SetDefScaleVec(double dS)
{
	dScaleVec = dS;
}

void ME_Object::SetCurrentResSetDef(int iRS,int iRV)
{
if ((iRS<iNoRes) && (iRS>-1) && (iRV>=-1))
{
  iCurResSetDef=iRS;
  iResValDef=-1;
  if ((iRV<ResultsSets[iCurResSetDef]->iNoV) && (iRV>=-1))
  {
    iResValDef=iRV;
    PostElResDef(ResultsSets[iCurResSetDef],iResValDef,fMaxRes,fMinRes);
  }
}
else
{
  outtext1("WARNING: Invalid LC ID.");
}

}
//void  ME_Object::SolveLidCav()
//{
//int i,j;
//double dt=0.001;
//double dX;
//double dY;
//dList X;
//dList Y;
//int indi;
//int indj;
//int iNJ;
//int iNI;
////Mesh building
//for (i=0;i<iNdNo;i++)
//{
//  X.AddEx(pNodes[i]->Pt_Point->x);
//  Y.AddEx(pNodes[i]->Pt_Point->y);
//}
//X.Sort();
//Y.Sort();
//
//iNJ = 2 * Y.iNo - 1      
//iNI = 2 * X.iNo - 1 
//
//Matrix <int> A(X.iNo,Y.iNo);
//for(i=0;i<A.m;i++){for(j=0;j<A.n;j++){A(i,j) = 0;}}
//
////Store the pressure point node IDs
//for (i=0;i<iNdNo;i++)
//{
//  indj=X.IsIn(pNodes[i]->Pt_Point->x);
//  indi=Y.IsIn(pNodes[i]->Pt_Point->y);
//  A(indi,indj)=pNodes[i]->iLabel;
//}
//
//Matrix <double> Sol(Y.iNo+2,X.iNo);
//Matrix <double> Diff(Y.iNo+2,X.iNo);
//
//for(i=0;i<Sol.m;i++){for(j=0;j<Sol.n;j++){Sol(i,j) = 0;}}
//for(i=0;i<Sol.m;i++){for(j=0;j<Sol.n;j++){Diff(i,j) = 0;}}
////Test Case BD
//Pt_Object* pNd;
//pNd=this->GetNode(78);
//indj=X.IsIn(pNd->Pt_Point->x);
//indi=Y.IsIn(pNd->Pt_Point->y);
//int isUCalc=-1;
////Begin of solution
//int k;
////This is the cavity lid driven test case
//
//// 
//for (k=0;k<5000;k++)
//{
//  
//  BCCavLid2(Sol);
//  BCCavLid(Sol);
//  isUCalc=-1;
//  //Sol.elem(indi+1,indj+1)=30;
//  for (i=2;i<Y.iNo;i++)
//  {
//    for (j=2;j<X.iNo;j++)
//    {
//      if (isUCalc==-1)
//      {
//        if (j%2!=0)
//        {
//          CalcU2(Sol,i,j);
//        }
//        
//      }
//      else
//      {
//        if (j%2==0)
//        {
//          CalcV2(Sol,i,j);
//        }
//      }
//    }
//    isUCalc*=-1;
//  }
// //Presures and velocity Corrections Diff
//
//  int il;
//  for(i=0;i<Diff.m;i++){for(j=0;j<Diff.n;j++){Diff(i,j) = 0;}}
//  for (il=0;il<50;il++)
//  {
//    for (i=2;i<Y.iNo-1;i++)
//    {
//      for (j=2;j<X.iNo-1;j++)
//      {
//          if ((j%2==0) && (i%2==0))
//          {
//            CalcP(Sol,Diff,i,j);
//          }
//      }
//    }
//  }
//  int isUCalc=-1;
//  //Velocity Corretions
//  /*for (i=2;i<Diff.m-2;i++)
//  {
//    for (j=2;j<Diff.n-2;j++)
//    {
//      if (isUCalc==-1)
//      {
//        if (j%2!= 0)
//        {
//          Diff.elem(i,j)=(Diff.elem(i,j+1)-Diff.elem(i,j-1))/(dX*dt);
//        }
//        
//      }
//      else
//      {
//        if (j%2==0)
//        {
//          Diff.elem(i,j)=(Diff.elem(i+1,j)-Diff.elem(i-1,j))/(dY*dt);
//        }
//      }
//    }
//    isUCalc*=-1;
//  }*/
//  for(i=0;i<A.m;i++)
//  {
//    for(j=0;j<A.n;j++)
//    {
//         Sol(i,j) += 0.1*Diff(i,j);
//    }
//  }
//}
////DiagNostics
//Sol.diag();
//Diff.diag();
//A.DeleteAll();
//Sol.DeleteAll();
//
//----------------------------------------------------------------------------
//    E L E M E N T   O B J E C T
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC( WG_Def, CObject )

WG_Def::WG_Def()
{
isMeshed=FALSE;
Name="NULL";
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
int i=0;
iObjType = 500;
iLabel = -1;
iColour = 2;
iTCnt=0;
iPartCnt=0;
vStart.Set(0,0,0);
iWG=0;
dWall=0;
dSize=0.005;
iWG2=0;
dWall2=0;
bQ1=FALSE;
bQ2=FALSE;
iTarnsitionPos=-1;
}


void WG_Def:: Create(int iLab,CString sN,G_Object* Parrent)

{
Drawn = 0;
Name=sN;
Selectable  = 1; 
Visable  = 1;
iObjType = 500;
iLabel = iLab;
iColour = 2;
iTCnt=0;
iPartCnt=0;
isMeshed=FALSE;
iWG=0;
dWall=0;
dSize=0.005;
pParent=Parrent;
iWG2=0;
dWall2=0;
bQ1=FALSE;
bQ2=FALSE;
iTarnsitionPos=-1;
}

void WG_Def::Serialize(CArchive& ar,int iV)
{
	int i;
	int iType;
	if (ar.IsStoring())
	{		// TODO: add storing code here
    G_Object::Serialize(ar,iV); 
	  ar<<Name;
	  ar<<dSize;
    ar<<iTCnt;
	  ar<<isMeshed;
	  ar<<vStart.x;
	  ar<<vStart.y;
	  ar<<vStart.z;
    ar<<iWG;
    ar<<dWall;
    ar<<iWG2;
    ar<<dWall2;
    ar<<bQ1;
    ar<<bQ2;
    ar<<iTarnsitionPos;
	  for (i=0;i<iTCnt;i++)
	  {
		  ar<<Text[i];
	  }
    ar<<iPartCnt;
	  for (i=0;i<iPartCnt;i++)
	  {
		  ar<<WGParts[i]->iObjType;
      WGParts[i]->Serialize(ar,iV); 
	  }
	}
	else
	{
    G_Object::Serialize(ar,iV);
	  ar>>Name;
	  ar>>dSize;
    ar>>iTCnt;
	  ar>>isMeshed;
	  ar>>vStart.x;
	  ar>>vStart.y;
	  ar>>vStart.z;
    ar>>iWG;
    ar>>dWall;
    ar>>iWG2;
    ar>>dWall2;
    ar>>bQ1;
    ar>>bQ2;
    ar>>iTarnsitionPos;
	  for (i=0;i<iTCnt;i++)
	  {
		  ar>>Text[i];
	  }
      ar>>iPartCnt;
	  for (i=0;i<iPartCnt;i++)
	  {
	      ar>>iType;
		  switch(iType)
		  {
		    case 600:
            WGParts[i] = new WG_Part;
			      break;
        case 601:
            WGParts[i] = new WG_Elbow;    
			      break;
       case 602:
            WGParts[i] = new WG_Brk;    
			      break;
		   case 603:
            WGParts[i] = new Sweep;    
			      break;
		   case 604:
            WGParts[i] = new SweepB;    
			      break;
       case 605:
            WGParts[i] = new WG_Brk_Int;    
			      break;
       case 606:
            WGParts[i] = new SweepF;    
			      break;
       case 607:
            WGParts[i] = new SweepFB;    
			      break;
		  }
      WGParts[i]->Serialize(ar,iV); 
		  WGParts[i]->pParent=this;
	  }
	}

	
}

void WG_Def::UpdatePropRef(PropTable* pT)
{
ME_Object* pM;
int i;
	  for (i=0;i<iPartCnt;i++)
	  {
      if ((WGParts[i]->iObjType==600) ||
          (WGParts[i]->iObjType==601) ||
          (WGParts[i]->iObjType==602) ||
          (WGParts[i]->iObjType==605))
		   {
            WG_Part* pPart =(WG_Part*)  WGParts[i];
            pM=pPart->GetMesh();
            if (pM!=NULL)
            {
              pM->UpdatePropRef(pT);
            }
       }
       else if  ((WGParts[i]->iObjType==603) ||
                 (WGParts[i]->iObjType==604))
       {
            Sweep* pSweep= (Sweep*) WGParts[i]; 
            pM=pSweep->GetMesh();
            if (pM!=NULL)
            {
              pM->UpdatePropRef(pT);
            }
		   }
    }
}

// Draw Object line
void WG_Def::Draw(CDC* pDC,int iDrawmode)
{
int i;
for (i=0;i<iPartCnt;i++)
{
  WGParts[i]->Draw(pDC,iDrawmode);
}
}



void WG_Def::OglDraw(int iDspFlgs,double dS1,double dS2)
{
int j=0;
if ((iDspFlgs & DSP_ASSEM)>0)
{
  Selectable=1;
  for (j=0;j<iPartCnt;j++)
  {
    WGParts[j]->OglDraw(iDspFlgs,dS1,dS2);
  }
}
else
{
  Selectable=0;
}
}

void WG_Def::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int j=0;
if ((iDspFlgs & DSP_ASSEM)>0)
{
  Selectable=1;
  for (j=0;j<iPartCnt;j++)
  {
    Selectable=1;
    WGParts[j]->OglDrawW(iDspFlgs, dS1, dS2);
  }
}
else
{
  Selectable=0;
  for (j=0;j<iPartCnt;j++)
  {
    WGParts[j]->Selectable=0;
  }
}
}
void WG_Def::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int i;
for (i=0;i<iPartCnt;i++)
{
  WGParts[i]->SetToScr(pModMat,pScrTran);
}

}


C3dVector WG_Def::Get_Centroid()
{
int i;
C3dVector vT;
vT.Set(0,0,0);
if (iPartCnt>0) 
{
  for (i=0;i<iPartCnt;i++)
  {
    vT+=WGParts[i]->Get_Centroid();
  }
  vT/=iPartCnt;
}
return (vT);
}

//*********************************************
//String extration procedures for build WG Proc
//*********************************************

int ExtractRad(CString inS,double* dRad)
{
int iRet=1;
int i1;
int i2;

CString sR;
CString sVec;

i1=inS.Find("Bend Rad=");
sVec=inS.Right(inS.GetLength()-i1-9);
i2=sVec.Find("(Angle");
sVec=sVec.Left(i2);
*dRad=atof(sVec)/1000;
return (iRet);
}

int ExtractTwistLen(CString inS,double* dTw)
{
int iRet=1;
int i1;
int i2;

CString sR;
CString sVec;

i1=inS.Find("MM");
if (i1<0)
{
  i1=inS.Find("mm");
}
sVec=inS.Left(i1);
i2=sVec.ReverseFind('_');
sVec=sVec.Right(sVec.GetLength()-i2-1);
*dTw=atof(sVec)/1000;
return (iRet);
}

int ExtractTwistAng(CString inS,double* dA)
{
int iRet=1;
int i1;
int i2;

CString sR;
CString sVec;

i1=inS.Find("DEG");
if (i1<0)
{
  i1=inS.Find("deg");
}
sVec=inS.Left(i1);
i2=sVec.ReverseFind('_');
sVec=sVec.Right(sVec.GetLength()-i2-1);
*dA=atof(sVec);
return (iRet);
}


int ExtractWG(CString inS,int* iW,double* dW,int* iW2,double* dW2,BOOL* bQ1,BOOL* bQ2)
{
int iRet=1;
int i1;
int i2;

CString sR;
CString sVec;
CString sWG2;
CString sWG1;

sWG1=inS;
i1=inS.Find(",");
if (i1>-1)
{
sWG1=sWG1.Left(i1);
}
  i1=sWG1.Find("_WR");
  sVec=sWG1.Right(sWG1.GetLength()-i1-3);
  i2=sVec.Find("_");
  sR=sVec.Left(i2);
  *iW=atoi(sVec);
  sVec=sVec.Right(sVec.GetLength()-i2);
  i2=sVec.Find("MM");
  sR=sVec.Left(i2);
  i1=sR.Find("_Q_");
  if (i1>-1)
  {
    *bQ1=TRUE;
    sR=sR.Right(sR.GetLength()-3);
  }
  else
  {
    sR=sR.Right(sR.GetLength()-1);
  }
  *dW=atof(sR);
i1=inS.Find(",");
if (i1>-1)
{
  sWG2=inS.Right(inS.GetLength()-i1-1);
  i1=sWG2.Find("_WR");
  sVec=sWG2.Right(sWG2.GetLength()-i1-3);
  i2=sVec.Find("_");
  sR=sVec.Left(i2);
  *iW2=atoi(sVec);
  sVec=sVec.Right(sVec.GetLength()-i2);
  i2=sVec.Find("MM");
  sR=sVec.Left(i2);
  i1=sR.Find("_Q_");
  if (i1>-1)
  {
    *bQ2=TRUE;
    sR=sR.Right(sR.GetLength()-3);
  }
  else
  {
    sR=sR.Right(sR.GetLength()-1);
  }
  *dW2=atof(sR);
}


return (iRet);
}

int ExtractPt(CString inS,C3dVector* inVec)
{
int iRet=1;
int i1;
double dx;
double dy;
double dz;
CString sX;
CString sY;
CString sVec;
i1=inS.Find(":");
sVec=inS.Right(inS.GetLength()-i1-1);
i1=sVec.Find(",");
sX=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(",");
sY=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(".");
sVec=sVec.Left(i1+3);
dx=atof(sX);
dy=atof(sY);
dz=atof(sVec);
inVec->Set(dx/1000,dy/1000,dz/1000);
return (iRet);
}



int ExtractPtS(CString inS,C3dVector* inVec)
{
int iRet=1;
int i1;
double dx;
double dy;
double dz;
CString sX;
CString sY;
CString sVec;
i1=inS.Find("(");
sVec=inS.Right(inS.GetLength()-i1-1);
i1=sVec.Find(",");
sX=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(",");
sY=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(")");
sVec=sVec.Left(i1);
dx=atof(sX);
dy=atof(sY);
dz=atof(sVec);
inVec->Set(dx/1000,dy/1000,dz/1000);
return (iRet);
}


int ExtractName(CString inS,CString* sName)
{
int iRet=1;
CString S;
int i1;

S = inS.Right(inS.GetLength()-57);
i1=S.Find(" ");
*sName=S.Left(i1);
return (iRet);
}





int ExtractOPt(CString inS,C3dVector* inVec)

{
int iRet=1;
int i1;
double dx;
double dy;
double dz;
CString sX;
CString sY;
CString sVec;

sVec=inS;
i1=sVec.Find(",");
sX=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(",");
sY=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
dx=atof(sX);
dy=atof(sY);
dz=atof(sVec);
inVec->Set(dx/1000,dy/1000,dz/1000);
return (iRet);
}

int ExtractOOPt(CString inS,C3dVector* inVec)

{
int iRet=1;
int i1;
double dx;
double dy;
double dz;
CString sX;
CString sY;
CString sVec;
i1=inS.Find(":");
sVec=inS.Right(inS.GetLength()-i1-1);
i1=sVec.Find(",");
sX=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
i1=sVec.Find(",");
sY=sVec.Left(i1);
sVec=sVec.Right(sVec.GetLength()-i1-1);
dx=atof(sX);
dy=atof(sY);
dz=atof(sVec);
inVec->Set(dx/1000,dy/1000,dz/1000);
return (iRet);
}


//*********************************************
//END String extraction procedure s
//*********************************************


void WG_Def::BuildFromDef(int* iErr)
{
outtext1("First pass build of waveguide");
outtext1("Parts Found :-");
CString s1;
CString s11;
C3dVector vPt;
C3dVector vPt2;
C3dVector vPt3;
CString sName;
double dRad;
int i;
int k=0;
int irc;
BOOL bStart=TRUE;
BOOL bFirst=TRUE;
for (i=0;i<iTCnt;i++)
{
	s1=Text[i].Left(32);
	s11=Text[i].Left(5);
	if (s11=="Type ")
	{
    irc=ExtractWG(Text[i],&iWG,&dWall,&iWG2,&dWall2,&bQ1,&bQ2);
	}
  if (s11=="Start")
	{
    irc=ExtractPtS(Text[i],&vStart);
	}
  if ((s1) == "FlangeStandard                  ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     if (bStart==FALSE)
	   {
	     irc=ExtractPt(Text[i+1],&vPt);
       vPt+=vStart;
     }
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,sName,vPt,this);
	   AddPart  (Fl);
	   bStart=FALSE;
     k++;
  }
  if ((s1) == "FlangeSpecial                   ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     if (bStart==FALSE)
	   {
	     irc=ExtractPt(Text[i+1],&vPt);
       vPt+=vStart;
     }
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,sName,vPt,this);
	   AddPart  (Fl);
	   bStart=FALSE;
     k++;
  }
  if ((s1) == "ElbowHParts                     ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Elbow* Fl = new WG_Elbow;
	   Fl->Create(k,sName,vPt,TRUE,this);
	   AddPart  (Fl);
     k++;
  }
  if ((s1) == "ThermalFin                      ") 
  {
	   outtext1(s1);
     irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,"NULL",vPt,this);
	   AddPart  (Fl);
  }
  if ((s1) == "CollarFlexible                  ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,sName,vPt,this);
     Fl->bFlipZ=bFirst;
	   AddPart  (Fl);
     k++;
     if (bFirst==TRUE)
      {bFirst=FALSE;}
     else
      {bFirst=TRUE;}
  }
  if ((s1) == "Transition                      ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,sName,vPt,this);
	   iTarnsitionPos=AddPart  (Fl);
     k++;
     //Need to chect to see if part needs zfilp
     if (iWG<iWG2)
     {Fl->bFlipZ=TRUE;}
     else if (dWall<dWall2) 
     {Fl->bFlipZ=TRUE;} 
     else if (bQ1==TRUE)
     {Fl->bFlipZ=TRUE;}
  }
  if ((s1) == "CollarTransition                ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,sName,vPt,this);
	   iTarnsitionPos=AddPart  (Fl);
     k++;
     if (iWG<iWG2)
     {Fl->bFlipZ=TRUE;}
     else if (dWall<dWall2) 
     {Fl->bFlipZ=TRUE;} 
     else if (bQ1==TRUE)
     {Fl->bFlipZ=TRUE;}
  }
  //if ((s1) == "BracketTie                      ") 
  //{
	 //  outtext1(s1);
	 //  irc=ExtractPt(Text[i],&vPt);
  //   vPt+=vStart;
	 //  irc=ExtractName(Text[i],&sName);
  //   WG_Part* Fl = new WG_Part;
	 //  Fl->Create(k,sName,vPt,this);
	 //  AddPart  (Fl);
  //   k++;
  //}  
  if ((s1 == "BracketIntegrated               ") 
   || (s1 == "BracketTie                      "))
  {
	  outtext1(s1);
	  irc=ExtractPt(Text[i],&vPt);
	  vPt+=vStart;
	  irc=ExtractName(Text[i],&sName);
	  irc=ExtractOPt(Text[i+1],&vPt2);
	  vPt2+=vStart;
	  irc=ExtractOOPt(Text[i+1],&vPt3);
	  vPt3+=vStart;
    WG_Brk_Int* Bl = new WG_Brk_Int;
	  Bl->Create(k,sName,vPt,vPt2,vPt3,this);
	  AddPart(Bl);
    k++;
  }  
  if (s1 == "BracketFlange                   ")
  {
	  outtext1(s1);
	  irc=ExtractPt(Text[i],&vPt);
	  vPt+=vStart;
	  irc=ExtractName(Text[i],&sName);
	  irc=ExtractOPt(Text[i+1],&vPt2);
	  vPt2+=vStart;
	  irc=ExtractOOPt(Text[i+1],&vPt3);
	  vPt3+=vStart;
    if (bStart==FALSE)
	   {
	     irc=ExtractPt(Text[i+2],&vPt);
       vPt+=vStart;
     }
    WG_Brk_Int* Bl = new WG_Brk_Int;
	  Bl->Create(k,sName,vPt,vPt2,vPt3,this);
	  AddPart(Bl);
    bStart=FALSE;
    k++;
  }  
  if ((s1) == "FlexibleStraight                ") 
  {
	   outtext1(s1);
     SweepF* Bl = new SweepF;
	   Bl->PreCreate(k,"FLEXI",0);
	   AddPart(Bl);
     k++;
  }
  if ((s1 == "TwistVariable                   ") || 
      (s1 == "TwistFixed                      "))
  {
	   outtext1(s1);
     double dA;
     double dT;
     C3dVector VDir;
     C3dVector vP2;
     VDir=GetLastInsPos();
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Part* Fl = new WG_Part;
	   Fl->Create(k,"NULL",vPt,this);
	   AddPart  (Fl);
     k++;
     
     irc=ExtractTwistLen(sName,&dT);
     irc=ExtractTwistAng(sName,&dA);
     Sweep* Bl = new Sweep;
	   Bl->PreCreate(k,sName,dA);
	   AddPart(Bl);
     k++;
     vP2=vPt;
     vPt-=VDir;
     vPt.Normalize();
     vPt*=dT;
     vPt+=vP2;
     Fl = new WG_Part;
	   Fl->Create(k,"NULL",vPt,this);
	   AddPart  (Fl);
     k++;
  }
  if ((s1) == "ElbowEParts                     ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
     vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
     WG_Elbow* Fl = new WG_Elbow;
	   Fl->Create(k,sName,vPt,FALSE,this);
	   AddPart  (Fl);
     k++;
  }
	else if ((s1 == "ElbowHFixed                     ") ||
		       (s1 == "ElbowHVariable                  "))
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
	   vPt+=vStart;
	   irc=ExtractRad(Text[i],&dRad);
     SweepB* El = new SweepB;
	   El->PreCreate(k,"NULL",vPt,dRad,1,0);
	   AddPart(El);
     k++;
  }
  else if (s1 == "ElbowHFlexible                  ")
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
	   vPt+=vStart;
	   irc=ExtractRad(Text[i],&dRad);
     SweepFB* El = new SweepFB;
	   El->PreCreate(k,"NULL",vPt,dRad,1,0);
	   AddPart(El);
     k++;
  }
	else if ((s1 == "ElbowEFixed                     ")  ||
		       (s1 == "ElbowEVariable                  "))
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
	   vPt+=vStart;
	   irc=ExtractRad(Text[i],&dRad);
     SweepB* El = new SweepB;
	   El->PreCreate(k,"NULL",vPt,dRad,0,0);
	   AddPart(El);
     k++;
  }
  	else if (s1 == "ElbowEFlexible                  ")
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
	   vPt+=vStart;
	   irc=ExtractRad(Text[i],&dRad);
     SweepFB* El = new SweepFB;
	   El->PreCreate(k,"NULL",vPt,dRad,0,0);
	   AddPart(El);
     k++;
  }
  else if ((s1) == "ElbowEParts                     ") 
  {
	   outtext1(s1);
	   irc=ExtractPt(Text[i],&vPt);
	   vPt+=vStart;
	   irc=ExtractName(Text[i],&sName);
	   irc=ExtractOPt(Text[i+1],&vPt2);
	   vPt2+=vStart;
     WG_Brk* Bl = new WG_Brk;
	   Bl->Create(k,sName,vPt,vPt2,this);
	   AddPart(Bl);
     k++;
  }
  else if ((s1) == "BracketClamp                    ") 
  {
	  outtext1(s1);
	  irc=ExtractPt(Text[i],&vPt);
	  vPt+=vStart;
	  irc=ExtractName(Text[i],&sName);
	  irc=ExtractOPt(Text[i+1],&vPt2);
	  vPt2+=vStart;
    WG_Brk* Bl = new WG_Brk;
	  Bl->Create(k,sName,vPt,vPt2,this);
	  AddPart(Bl);
    k++;
  }
	else if ((s1) == "Straight Rectangular Waveguide  ") 
  {
	   outtext1(s1);
	   if (WGParts[iPartCnt-1]->iObjType!=603)
	   {
        Sweep* Bl = new Sweep;
	      Bl->PreCreate(k,"TEST",0);
	      AddPart(Bl);
	   }
	   else
	   {
        Sweep* Bl = (Sweep*) WGParts[iPartCnt-1];
		    Bl->Twist=90;
	   }
     k++;
  }
}
SetFlexMarkers();
}

void WG_Def::SetFlexMarkers()
{
int i;
Sweep* pS;
bool bFirst=TRUE;
for (i=0;i<iPartCnt-1;i++)
{
  if ((WGParts[i]->iObjType == 606) || 
      (WGParts[i]->iObjType == 607) )
  {
     pS=(Sweep*) WGParts[i];
     if (bFirst==TRUE)
     {
        pS->isStart=TRUE;
        bFirst=FALSE;
        if ((WGParts[i+1]->iObjType == 606) || 
            (WGParts[i+1]->iObjType == 607) )
        {
           pS->isEnd=FALSE;
        }
        else
        {
           pS->isEnd=TRUE;
           bFirst=TRUE;
        }
     }
     else
     {
        pS->isStart=FALSE;
     }
  }
  else
  {

  }
}
}

C3dMatrix WG_Def::GetOrientMat()
{
C3dVector VN;
C3dVector VD;
C3dVector vUX;
C3dMatrix mUp;
int iS=0;
int isHB=0;
double dT=0;
int i;
//******************************************************************
// This bit find the up vector from the first bend
// Need now to acount for a twist before
// and should also condifer elbow parts
//******************************************************************

for (i=0;i<iPartCnt;i++)
{
  if ((WGParts[i]->iObjType == 604) || 
      (WGParts[i]->iObjType == 607) )
  {
    SweepB* SN = (SweepB*) WGParts[i];
    VN=SN->GetNVec();
    VD=SN->GetSVec();
    isHB=SN->isHBend;
    iS=i;
    break;
  }
  else if (WGParts[i]->iObjType == 601)
  {
    WG_Elbow* SN = (WG_Elbow*) WGParts[i];
    VN=WGParts[i+1]->GetDir(0);
    VD=WGParts[i-1]->GetDir(0);
    VN=VN.Cross(VD);
    isHB=SN->isHBend;
    iS=i;
    break;
  }
}

//Check for sweeps with twists which change the orientaion 
for (i=0;i<iS;i++)
{
  if ((WGParts[i]->iObjType == 603) || (WGParts[i]->iObjType == 606))
  {
    Sweep* SN = (Sweep*) WGParts[i];
    dT+=SN->Twist;
  }
}

vUX=VN.Cross(VD);
if (isHB)
{
  mUp.SetColVec(1,VN);
  mUp.SetColVec(2,-vUX);
  mUp.SetColVec(3,VD);
}
else
{
  mUp.SetColVec(1,vUX);
  mUp.SetColVec(2,VN);
  mUp.SetColVec(3,VD);
}
//Now corect for the twist dT
C3dMatrix mR;
mR.Rotate(0,0,-dT);
mUp*=mR;
return (mUp);
}

int WG_Def::Generate(SecTable* pSecs,int iM,int iSW,int iSH,double dS,double dSA)
{
int irc=0;
C3dMatrix mUp;
dSize=dS;
dASize=dSA;
outtext1("Second pass build of waveguide");
outtext1("Generating Sweeps");
C3dVector A;
C3dVector B;
NLine* L1;
NLine* L2;

Section* Sec = NULL;
SecProp* SecP = NULL;
int iGenWG;
double dGenWall;
double dH;
int i;
int iQM;
try
{
iGenWG=iWG;
dGenWall=dWall;  
iQM=iM;
if (bQ1)
 {iQM=iM+10;}
SecP = pSecs->GetSecP(iGenWG,dGenWall,iQM,iSW,iSH);

if (SecP!=NULL)
{
  dW=SecP->W;
  dH=SecP->H;
}

for (i=0;i<iPartCnt;i++)
{
  if (iTarnsitionPos==i)
  {
    iGenWG=iWG2;
    dGenWall=dWall2;
    iQM=iM;
    if (bQ2)
    {iQM=iM+10;}
    SecP = pSecs->GetSecP(iGenWG,dGenWall,iQM,iSW,iSH);
    if (SecP!=NULL)
    {
      dW=SecP->W;
      dH=SecP->H;
    }
  }
  else
  {

  }
  if (WGParts[i]->iObjType == 603)
  {
    Sec=pSecs->GetSec(iGenWG,dGenWall,iQM,iSW,iSH,FALSE);
	  A=WGParts[i-1]->GetInPt();
	  B=WGParts[i+1]->GetInPt();
    NLine* Ln = new NLine;
    Ln->Create(A,B,1,NULL);
	  Sweep* Sw =  (Sweep*) WGParts[i];
    Sw->Create(-1,Ln,Sec,this);
  }
  else  if (WGParts[i]->iObjType == 606)
  {
    Sec=pSecs->GetSec(iGenWG,dGenWall,iQM,iSW,iSH,TRUE);
    SecP = pSecs->GetSecPF(iGenWG);
    if (SecP!=NULL)
    {
      Sec->FilletCrns(SecP->dR/1000); 
    }
    else
    {
      Sec->FilletCrns(0.003);
    }
	  A=WGParts[i-1]->GetInPt();
	  B=WGParts[i+1]->GetInPt();
    NLine* Ln = new NLine;
    Ln->Create(A,B,1,NULL);
	  SweepF* Sw =  (SweepF*) WGParts[i];
    Sw->Create(-1,Ln,Sec,this);
  }
}
iQM=iM;
if (bQ1)
{iQM=iM+10;}
SecP = pSecs->GetSecP(iWG,dWall,iQM,iSW,iSH);
iGenWG=iWG;
dGenWall=dWall;
if (SecP!=NULL)
{
  dW=SecP->W;
  dH=SecP->H;
}
for (i=0;i<iPartCnt;i++)
{
  if (iTarnsitionPos==i)
  {
    iQM=iM;
    if (bQ2)
    {iQM=iM+10;}
    iGenWG=iWG2;
    dGenWall=dWall2;
    SecP = pSecs->GetSecP(iGenWG,dGenWall,iQM,iSW,iSH);
    if (SecP!=NULL)
    {
      dW=SecP->W;
      dH=SecP->H;
    }
  }
  if (WGParts[i]->iObjType == 604)
  {
	  if (((WGParts[i-1]->iObjType==603) || (WGParts[i-1]->iObjType==606)) && ((WGParts[i+1]->iObjType==603) || (WGParts[i+1]->iObjType==606)))
	  {
      Sec=pSecs->GetSec(iGenWG,dGenWall,iQM,iSW,iSH,FALSE);
      SweepB* SB = (SweepB*) WGParts[i];
      Sweep* S1= (Sweep*) WGParts[i-1];
      Sweep* S2= (Sweep*) WGParts[i+1];
	    L1 = (NLine*) S1->pPath;
	    L2 = (NLine*) S2->pPath;
	    NCircle* C = NFillet(L1,L2,SB->dRad);
	    SweepB* SwB =  (SweepB*) WGParts[i];
      SwB->Create(-1,C,Sec,this);
	  }
  }
  if (WGParts[i]->iObjType == 607)
  {
	  if (((WGParts[i-1]->iObjType==603) || (WGParts[i-1]->iObjType==606)) && ((WGParts[i+1]->iObjType==603) || (WGParts[i+1]->iObjType==606)))
	  {
      Sec=pSecs->GetSec(iGenWG,dGenWall,iQM,iSW,iSH,TRUE);
      SecP = pSecs->GetSecPF(iGenWG);
      if (SecP!=NULL)
      {
        Sec->FilletCrns(SecP->dR/1000); 
      }
      else
      {
        Sec->FilletCrns(0.003);
      }
      SweepB* SB = (SweepB*) WGParts[i];
      Sweep* S1= (Sweep*) WGParts[i-1];
      Sweep* S2= (Sweep*) WGParts[i+1];
	    L1 = (NLine*) S1->pPath;
	    L2 = (NLine*) S2->pPath;
	    NCircle* C = NFillet(L1,L2,SB->dRad);
	    SweepB* SwB =  (SweepB*) WGParts[i];
      SwB->Create(-1,C,Sec,this);
	  }
  }
}


//******************************************************************
mUp=GetOrientMat();
SweepB* SwB;
Sweep* SwS;
for (i=0;i<iPartCnt;i++)
{
  if (WGParts[i]->iObjType == 604)
  {
    SwB = (SweepB*) WGParts[i];
    SwB->Generate(mUp,dASize); 
    mUp =SwB->GetLastYMap();
  }
  if (WGParts[i]->iObjType == 607)
  {
    SwB = (SweepB*) WGParts[i];
    SwB->Generate(mUp,dASize); 
    mUp =SwB->GetLastYMap();
  }
  if (WGParts[i]->iObjType == 603)
  {
    SwS = (Sweep*) WGParts[i];
    SwS->Generate(mUp,dSize); 
    mUp =SwS->GetLastYMap();
  }
  if (WGParts[i]->iObjType == 606)
  {
    SweepF* SwF = (SweepF*) WGParts[i];
    SecP = pSecs->GetSecPF(iGenWG);
    if (SecP!=NULL)
    {
      SwF->Generate(mUp,dSize,SecP->dLFR/1000); 
    }
    else
    {
      SwF->Generate(mUp,dSize,0.001); 
    }
    mUp =SwS->GetLastYMap();
  }
}
}
catch(...)
{
 irc=3;
 outtext1("WG Generation FAILED");
}
return (irc);
}

//The initial orientation of the parts is taken
//for the sweep vector at on side of the part
//note this assume each part has a sweep either
//side of it
C3dMatrix WG_Def::GetTMat(int iPindex)
{
C3dMatrix mrc;
Sweep* Sw;
mrc.MakeUnit();
double dL;
if ((WGParts[iPindex]->iObjType==600) ||
    (WGParts[iPindex]->iObjType==601) ||
    (WGParts[iPindex]->iObjType==602) ||
    (WGParts[iPindex]->iObjType==605))
{

  if (iPindex<iPartCnt-1)
  {
    Sw = (Sweep*) WGParts[iPindex+1];
    dL=Sw->pPath->getLen();
    if (dL>0.000001)
    {
      mrc= Sw->GetFirstYMap();
    }
    else
    {
      Sw = (Sweep*) WGParts[iPindex-1];
      mrc= Sw->GetLastYMap();
      mrc.ReverseZ();
    }
  }
  else
  {
    Sw = (Sweep*) WGParts[iPindex-1];
    mrc= Sw->GetLastYMap();
    mrc.ReverseZ();
  }
}
return (mrc);
}

C3dVector WG_Def::GetVA(int iPindex)
{
C3dVector vrc;
C3dMatrix mrc;
vrc.Set(0,0,0);
Sweep* Sw;
if (iPindex>0)
{
  if ((WGParts[iPindex-1]->iObjType == 603) ||
     (WGParts[iPindex-1]->iObjType == 604))
  {
    Sw = (Sweep*) WGParts[iPindex-1];
    mrc= Sw->GetLastYMap();
    vrc=mrc.GetColVec(3);
  }
}
return (vrc);
}

C3dVector WG_Def::GetVB(int iPindex)
{
C3dVector vrc;
C3dMatrix mrc;
Sweep* Sw;
vrc.Set(0,0,0);
if (iPindex<iPartCnt-1)
{
  if ((WGParts[iPindex+1]->iObjType == 603) ||
     (WGParts[iPindex+1]->iObjType == 604))
  {
    Sw = (Sweep*) WGParts[iPindex+1];
    mrc= Sw->GetFirstYMap();
    vrc=mrc.GetColVec(3);
  }
}
return (vrc);
}

PBARL* WG_Def::CreatePBARL(PropTable* pT,CString sT,double W,double H, double dTHK,double dNSM,int iM,int PIDB)
{
Property* pP=NULL;
PBARL* pBar=NULL;
int iPID;
pP=pT->GetItem(PIDB);
if ((pP != NULL) && (pP->iType==5))
{
  pBar=(PBARL*) pP;
}
else
{
  pBar = new PBARL();
  if (pT->Exists(PIDB)==TRUE)
  {
    iPID=pT->NextID()+10000;
  }
  else
  {
    iPID=PIDB;
  }
  pBar->sSecType = "BOX     ";
  pBar->sTitle="Waveguide Beam";
  pBar->iMID=iM;
  pBar->iID=iPID;
  pBar->iNoDims=4;
  pBar->dDIMs[0]=W/1000;
  pBar->dDIMs[1]=H/1000;
  pBar->dDIMs[2]=dTHK/1000;
  pBar->dDIMs[3]=dTHK/1000;
  pBar->CreateSec();
  pBar->dNSM=dNSM;
  pT->AddItem(pBar);
}
return (pBar);
}


PSHELL* WG_Def::CreateShell(PropTable* pT, double dTHK,double dNSM,int iM,int PIDS)
{
Property* pP=NULL;
PSHELL* pShell=NULL;
int iPID;
pP=pT->GetItem(PIDS);
if ((pP != NULL) && (pP->iType==1))
{
  pShell=(PSHELL*) pP;
}
else
{
  pShell=new PSHELL();
  if (pT->Exists(PIDS)==TRUE)
  {
    iPID=pT->NextID()+10000;
  }
  else
  {
    iPID=PIDS;
  }
  pShell->sTitle="Waveguide Shell";
  pShell->iMID1=iM;
  pShell->iMID2=iM;
  pShell->iMID3=iM;
  pShell->iID=iPID;
  pShell->dT=dTHK/1000;
  pShell->dNSM=dNSM;
  pT->AddItem(pShell);
}
return (pShell);
}

int WG_Def::GenMesh(SecTable* pSecs,PropTable* pT,int iM,PartsCat* Cat, int iDim)
{
int irc=0;
int i;
SecProp* SecP = NULL;
C3dMatrix TMat;
C3dVector vA;
C3dVector vB;
PSHELL* pShell=NULL;
PBARL* pBar=NULL;
double ddH=0;
double ddW=0;
double ddT=0;
double iGenWG=iWG;
double dGenWall=dWall;
int iQM;
int iMAT1=1;
int iMAT2=1;
double dnsmS=0;
double dnsmB=0;
int PIDS;
int PIDB;
//******************************************************************



//******************************************************************
try
{
  if (isMeshed==FALSE)
  {
    outtext1("Generating Mesh");
    // This bit orientates the parts
    if (Cat!=NULL)
    {
      for (i=0;i<iPartCnt;i++)
      {
        if ((WGParts[i]->iObjType == 600) ||
            (WGParts[i]->iObjType == 601) ||
            (WGParts[i]->iObjType == 602) ||
            (WGParts[i]->iObjType == 605))
        {  
           TMat=GetTMat(i);
           vA=GetVA(i);
           vB=GetVB(i);
   	       WG_Part* P =  (WG_Part*) WGParts[i];
	         ME_Object* pM = Cat->Get(P->Name);
           if (P->bFlipZ==TRUE)
           {
             TMat.m_02*=-1;
             TMat.m_12*=-1;
             TMat.m_22*=-1;
           }
	         if (pM!=NULL)
	         {
             P->AttachMesh(pM,TMat,vA,vB);	
	         }
        }
      }
   }
   BOOL CutA=FALSE;
   BOOL CutB=FALSE;
   ME_Object* MeshA = NULL;
   ME_Object* MeshB = NULL;
  //NEED TO DO CUTBACKS BEFORE GENERATING SWEEPS
   for (i=0;i<iPartCnt;i++) 
   {   //Cutback straight sweeps
     if ((WGParts[i]->iObjType == 603) || (WGParts[i]->iObjType == 606))
     {  
	     CutA=FALSE;
       CutB=FALSE;
       Sweep* s = (Sweep*) WGParts[i];
       if ((WGParts[i-1]->iObjType != 604) && (WGParts[i-1]->iObjType != 607))
       {
         
	       WG_Part* WGP = (WG_Part*) WGParts[i-1];
         MeshA=WGP->GetMesh();
         if (WGP->Name!="NULL") 
           {CutA=TRUE;}
	     }
	     if ((WGParts[i+1]->iObjType != 604)  && (WGParts[i+1]->iObjType != 607))
       {
	       WG_Part* WGP = (WG_Part*) WGParts[i+1];
         MeshB=WGP->GetMesh();
         if (WGP->Name!="NULL")
           {CutB=TRUE;}
	     }
	     s->CutBack(CutA,CutB,MeshA,MeshB);
     }
   }
  iGenWG=iWG;
  dGenWall=dWall;
  iQM=iM;
  if (bQ1)
  {iQM=iM+10;}
  SecP = pSecs->GetSecP((int) iGenWG,dGenWall,iQM,0,0);
  if (SecP!=NULL)
  {
    ddW=SecP->W;
    ddH=SecP->H;
    ddT=SecP->THK;
    iMAT1=SecP->iMat1;
    iMAT2=SecP->iMat2;
    dnsmS=SecP->dNSMS;
    dnsmB=SecP->dNSMB;
    PIDS=SecP->PID;
    PIDB=SecP->PID2;
  }
  else
  {
    outtext1("Section not found");
  }
  pShell = CreateShell(pT,ddT,dnsmS,iMAT1,PIDS);
  pBar= CreatePBARL(pT,"BOX     ",ddW,ddH,ddT,dnsmB,iMAT1,PIDB);
   for (i=0;i<iPartCnt;i++)
   {
     if (iTarnsitionPos==i)
     {   iQM=iM;
         if (bQ2)
         {iQM=iM+10;}
        iGenWG=iWG2;
        dGenWall=dWall2;
        SecP = pSecs->GetSecP((int) iGenWG,dGenWall,iQM,0,0);
        if (SecP!=NULL)
        {
          ddW=SecP->W;
          ddH=SecP->H;
          ddT=SecP->THK;
          iMAT1=SecP->iMat1;
          iMAT2=SecP->iMat2;
          dnsmS=SecP->dNSMS;
          dnsmB=SecP->dNSMB;
          PIDS=SecP->PID;
          PIDB=SecP->PID2;
        }
        else
        {
           outtext1("Section not found");
        }
        pShell = CreateShell(pT,ddT,dnsmS,iMAT1,PIDS);
        pBar= CreatePBARL(pT,"BOX     ",ddW,ddH,ddT,dnsmB,iMAT2,PIDB);
     }
     if ((WGParts[i]->iObjType == 603) || (WGParts[i]->iObjType == 604))
     {
       Sweep* s = (Sweep*) WGParts[i];
	     s->GenMesh(iDim,pShell,pBar);
	     //s->DeleteSections();
     }
     if (WGParts[i]->iObjType == 606) 
     {
       SecP = pSecs->GetSecPF((int)iGenWG);
       PSHELL* pShell1 = (PSHELL*) pT->GetItem(SecP->iP1);
       PSHELL* pShell2 = (PSHELL*) pT->GetItem(SecP->iP2);
       PSHELL* pShell3 = (PSHELL*) pT->GetItem(SecP->iP3);
       PSHELL* pShell4 = (PSHELL*) pT->GetItem(SecP->iP4);
       PBARL* pBar1 = (PBARL*) pT->GetItem(SecP->iP5);
       PBARL* pBar2 = (PBARL*) pT->GetItem(SecP->iP6);
       SweepF* s = (SweepF*) WGParts[i];
	     s->GenMesh(iDim,pShell1,pShell2,pShell3,pShell4,pBar1,pBar2);
	     //s->DeleteSections();
     }
     if (WGParts[i]->iObjType == 607) 
     {
       SecP = pSecs->GetSecPF((int) iGenWG);
       PSHELL* pShell1 = (PSHELL*) pT->GetItem(SecP->iP1);
       PSHELL* pShell2 = (PSHELL*) pT->GetItem(SecP->iP2);
       PSHELL* pShell3 = (PSHELL*) pT->GetItem(SecP->iP3);
       PSHELL* pShell4 = (PSHELL*) pT->GetItem(SecP->iP4);
       PBARL* pBar1 = (PBARL*) pT->GetItem(SecP->iP5);
       PBARL* pBar2 = (PBARL*) pT->GetItem(SecP->iP6);
       SweepFB* s = (SweepFB*) WGParts[i];
	     s->GenMesh(iDim,pShell1,pShell2,pShell3,pShell4,pBar1,pBar2);
	     //s->DeleteSections();
     }
   }
 }
 isMeshed=TRUE;
}
catch(...)
{
isMeshed=FALSE;
  for (i=0;i<iPartCnt;i++)
  {
	  if ((WGParts[i]->iObjType == 603) || (WGParts[i]->iObjType == 604))
    {
      Sweep* s = (Sweep*) WGParts[i];
      if (s->Mesh!=NULL)
      {
	      delete(s->Mesh);
	      s->Mesh=NULL;
      }
	  }
  }
  irc=3;
}
return (irc);
}

ME_Object* WG_Def::GetMesh()
{
int iE=10000;
int iN=10000;
if (isMeshed)
{
ME_Object* Me = new ME_Object();
Me->sName=Name;
int i;
for (i=0;i<iPartCnt;i++)
{
  if ((WGParts[i]->iObjType == 603)  ||
	    (WGParts[i]->iObjType == 604)  ||
      (WGParts[i]->iObjType == 606)  ||
      (WGParts[i]->iObjType == 607))

  {
     Sweep* SP=(Sweep*) WGParts[i];
     Me->Append(SP->GetMesh(),iN,iE);
	   iN+=10000;
	   iE+=10000;
  }
  else
  {
     WG_Part* WP=(WG_Part*) WGParts[i];
     Me->Append(WP->GetMesh(),iN,iE);
	   iN+=10000;
	   iE+=10000;
  }
}
return (Me);
}
else
{
return (NULL);
}
}


void WG_Def::addLn(CString inT)
{

if (iTCnt<MaxT)
{
	Text[iTCnt]=inT;
	iTCnt++;
}
}

int WG_Def::AddPart(G_Object* inP)
{
if (iPartCnt<MaxP)
{
	WGParts[iPartCnt]=inP;
	iPartCnt++;
}
return (iPartCnt-1);
}

C3dVector WG_Def::GetLastInsPos()
{
  int i;

  C3dVector vRet;
  for (i=iPartCnt-1;i>=0;i--)
  {
    if ((WGParts[i]->iObjType==600) ||
        (WGParts[i]->iObjType==601) ||
        (WGParts[i]->iObjType==602) ||
        (WGParts[i]->iObjType==605) )
    {
       WG_Part* pP = (WG_Part*) WGParts[i];
	     vRet=pP->GetInPt();
       break;
    }
    else if ((WGParts[i]->iObjType==604) || 
             (WGParts[i]->iObjType==607))
    {
       SweepB* pP = (SweepB*) WGParts[i];
	     vRet=pP->GetInPt();
       break;
    }
  }
  return (vRet);
}


void WG_Def::HighLight(CDC* pDC)
{
int i;
for (i=0;i<iPartCnt;i++)
{
  WGParts[i]->HighLight(pDC);
}
}

void WG_Def::Info()
{
  char S1[80];
  int i;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  outtext1("WAVEGUIDE OBJECT");
  outtext1(Name); 
  outtext1(S1); 

  for(i=0;i<iTCnt;i++)
  {
    outtext1(Text[i]); 
  }
  sprintf_s(S1,"%s%i%s%f","Type ",iWG,"    WALL ",dWall);
  outtext1(S1); 

}

void WG_Def::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
int i;

  for (i=0;i<iPartCnt;i++)
  {
    WGParts[i]->S_Box(P1,P2,pSel);
  }
}


G_ObjectD WG_Def::SelDist(CPoint InPT,Filter FIL)
{

G_ObjectD Ret;
G_Object* pMinSel;
double ds=1e36;
double Minds=1e36;
Ret.pObj=NULL;
Ret.Dist=Minds;
int i;

  for (i=0;i<iPartCnt;i++)
  {
    Ret=WGParts[i]->SelDist(InPT,FIL);
    if (Ret.Dist<Minds)
    {
	    Minds=Ret.Dist;
      pMinSel=Ret.pObj;
    }
  }

Ret.Dist=Minds;
Ret.pObj=pMinSel;
return (Ret);
}


Circ1* WG_Def::Fillet(Line_Object* Ln,Line_Object* Ln1,double dR)
{
double R;
R=dR;
C3dVector p1;
p1.Set(Ln->pVertex1->Pt_Point->x,
	   Ln->pVertex1->Pt_Point->y,
	   Ln->pVertex1->Pt_Point->z);
C3dVector p2;
p2.Set(Ln->pVertex2->Pt_Point->x,
	   Ln->pVertex2->Pt_Point->y,
	   Ln->pVertex2->Pt_Point->z);
C3dVector p3;
p3.Set(Ln1->pVertex2->Pt_Point->x,
	   Ln1->pVertex2->Pt_Point->y,
	   Ln1->pVertex2->Pt_Point->z);

C3dVector v1;
v1=p2;
v1-=p1;
C3dVector v2;
v2=p3;
v2-=p2;

C3dVector vn;
vn=v1.Cross(v2);
vn.Normalize();
C3dVector v1o;
C3dVector v2o;

v1o=v1.Cross(vn);
v2o=v2.Cross(vn);
v1o.Normalize();
v2o.Normalize();
v1o*=R;
v2o*=R;
if (v1o.Dot(v2)<0)
  v1o*=-1;
if (v2o.Dot(v1)>0)
  v2o*=-1;
C3dVector p1o;
C3dVector p2o;
C3dVector p3o;
C3dVector p4o;

p1o=p1;
p1o+=v1o;
p2o=p2;
p2o+=v1o;
p3o=p2;
p3o+=v2o;
p4o=p3;
p4o+=v2o;
Line_Object* Ln2 = new Line_Object;
Ln2->Create(&p1o,&p2o,1,NULL);
Line_Object* Ln3 = new Line_Object;
Ln3->Create(&p3o,&p4o,1,NULL);
C3dVector IntPt;
IntPt=LnInt(Ln2,Ln3);
Circ1* cCir = new Circ1();
cCir->Create(vn,IntPt,R,-1,NULL);

C3dVector IntPt1;
IntPt1=Ln->MinPt(IntPt);
Ln->SetP2(IntPt1.x,IntPt1.y,IntPt1.z);
C3dVector IntPt2;
IntPt2=Ln1->MinPt(IntPt);
Ln1->SetP1(IntPt2.x,IntPt2.y,IntPt2.z);
double d1;
double d2;
C3dVector LVec;
C3dVector CVec;
d1=cCir->MinWPt(IntPt1);
d2=cCir->MinWPt(IntPt2);
LVec=Ln->GetDir(1);
CVec=cCir->GetDir(d1);
if (LVec.Dot(CVec)<0)
{
cCir->Reverse();
d1=cCir->MinWPt(IntPt1);
d2=cCir->MinWPt(IntPt2);
}
if (d2<d1)
{
	d2+=1;
}
if (d1<0)
{
	d1=0;
}
cCir->dS=d1;
cCir->dE=d2;

delete (Ln2);
delete (Ln3);
return(cCir);
}

//This is taken from DBase
NCircle* WG_Def::NFillet(NLine* Ln,NLine* Ln1,double dR)
{
C3dVector PNear1;
C3dVector PNear2;
double R;
R=dR;
C3dVector p1;
C3dVector p2;
C3dVector p3;
C3dVector pT;
C3dVector vL1Dir;
C3dVector* pLT1;
C3dVector* pLT2;
C3dVector v1;
C3dVector v2;
C3dVector v3;
C3dVector vRef;
C3dVector vDir;
// The intersection of the lines



v1=Ln->GetDir(0.5);

v2=Ln1->GetDir(0.5);;
p1=Ln->GetPt(0);
pLT1=Ln->cPts[1]->Pt_Point;
p2=NLnInt(Ln,Ln1);
//p2=pLT1;
p3=Ln1->GetPt(1);
pLT2=Ln1->cPts[0]->Pt_Point;
C3dVector vn;
vn=v1.Cross(v2);
vn.Normalize();
C3dVector v1o;
C3dVector v2o;

v1o=v1.Cross(vn);
v2o=v2.Cross(vn);
v1o.Normalize();
v2o.Normalize();
v1o*=R;
v2o*=R;
if (v1o.Dot(v2)<0)
  v1o*=-1;
if (v2o.Dot(v1)>0)
  v2o*=-1;
C3dVector p1o;
C3dVector p2o;
C3dVector p3o;
C3dVector p4o;

p1o=p1;
p1o+=v1o;
p2o=p2;
p2o+=v1o;
p3o=p2;
p3o+=v2o;
p4o=p3;
p4o+=v2o;
NLine* Ln2 = new NLine;
Ln2->Create(p1o,p2o,1,NULL);
NLine* Ln3 = new NLine;
Ln3->Create(p3o,p4o,1,NULL);
C3dVector IntPt;
IntPt=NLnInt(Ln2,Ln3);


C3dVector IntPt1;
IntPt1=Ln->MinPt(IntPt);
vRef=IntPt1;
vRef-=IntPt;
vRef.Normalize();

NCircle* cCir = new NCircle();
cCir->Create2(vn,IntPt,vRef,R,-1,NULL);
cCir->vNorm=vn; // Note the transform method in Create2 changes the normal
                //so set it back 
pLT1->Set(IntPt1.x,IntPt1.y,IntPt1.z);  //trim the end point
C3dVector IntPt2;
IntPt2=Ln1->MinPt(IntPt);
pLT2->Set(IntPt2.x,IntPt2.y,IntPt2.z);  //trim the end point

//d1q=cCir->MinWPt(IntPt1);

//vRef=IntPt1;
//vDir=cCir->GetDir(d1q);
//vDir.Normalize();
//vRef-=cCir->vCent;
//vRef.Normalize();
  //if (vDir.Dot(vL1Dir)<0)
  //{
  //  vn*=-1;
  //}
  //delete (cCir);
  //cCir = new NCircle();
  //cCir->Create2(vn,IntPt,vRef,R,-1,NULL);
  //d1q=cCir->MinWPt(IntPt1);
double d2q;
d2q=cCir->MinWPt(IntPt2);
cCir->we=d2q;

delete (Ln2);
delete (Ln3);

return(cCir);
}

C3dVector WG_Def::NLnInt(NCurve* L1,NCurve* L2)
{
int i=0;
double MinDist = 10000000;
double dDist = 0;
double dDistB = 0;
const double dTol = 0.0000001;
C3dVector P1;
C3dVector P2;
int iMaxIt=0;
P1=L1->GetPt(0);

do
{
  P2=L2->MinPt(P1);
  P1=L1->MinPt(P2);
  dDist=P2.Dist(P1);
  iMaxIt++;
}
while ((dDist>dTol) && (iMaxIt<100000));
return (P2);
}

C3dVector WG_Def::LnInt(Line_Object* L1,G_Object* L2)
{
int i=0;
double MinDist = 10000000;
double dDist = 0;
double dDistB = 0;
const double dTol = 0.0000001;
C3dVector P1;
C3dVector P2;

P1.Set(L1->pVertex1->Pt_Point->x,
       L1->pVertex1->Pt_Point->y,
       L1->pVertex1->Pt_Point->z);

do
{
  P2=L2->MinPt(P1);
  P1=L1->MinPt(P2);
  dDist=P2.Dist(P1);
}
while (dDist>dTol);
return (P2);
}

IMPLEMENT_DYNAMIC( WG_Part, CObject )

WG_Part::WG_Part()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 600;
iLabel = -1;
iColour = 2;
inPt=NULL;
Mesh=NULL;
bFlipZ=FALSE;
}

void WG_Part::Create(int iLab,CString inName,
					 C3dVector inP,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 600;
iLabel = iLab;
iColour = 2;
Mesh=NULL;
Name=inName;
pParent=Parrent;
if (inPt!=NULL)
{ delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,1,this);
bFlipZ=FALSE;
}


C3dVector WG_Part:: GetInPt()
{
C3dVector vR;
vR.x=inPt->Pt_Point->x;
vR.y=inPt->Pt_Point->y;
vR.z=inPt->Pt_Point->z;
return (vR);
}


// Draw Object line
void WG_Part::Draw(CDC* pDC,int iDrawmode)
{
if (inPt!=NULL)
  {inPt->Draw(pDC,iDrawmode);
   pDC->MoveTo((int) inPt->DSP_Point->x+20,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y+20);
   pDC->LineTo((int) inPt->DSP_Point->x-20,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y-20);
   pDC->LineTo((int) inPt->DSP_Point->x+20,(int) inPt->DSP_Point->y);
}
if (Mesh!=NULL)
  {Mesh->Draw(pDC,iDrawmode);}
}

void WG_Part::HighLight(CDC* pDC)
{
if (inPt!=NULL)
  {inPt->Draw(pDC,4);
   pDC->MoveTo((int) inPt->DSP_Point->x+20,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y+20);
   pDC->LineTo((int) inPt->DSP_Point->x-20,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y-20);
   pDC->LineTo((int) inPt->DSP_Point->x+20,(int) inPt->DSP_Point->y);
   }
}

void WG_Part::OglDraw(int iDspFlgs,double dS1,double dS2)
{
if (inPt!=NULL)
{
  glColor3fv(cols[inPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) inPt->Pt_Point->x,(float) inPt->Pt_Point->y,(float) inPt->Pt_Point->z);
  glEnd();
}
if (Mesh!=NULL)
  {Mesh->OglDraw(iDspFlgs,dS1,dS2);}
}

void WG_Part::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if (inPt!=NULL)
{
  glColor3fv(cols[inPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) inPt->Pt_Point->x,(float) inPt->Pt_Point->y,(float) inPt->Pt_Point->z);
  glEnd();
}
if (Mesh!=NULL)
  {Mesh->OglDrawW(iDspFlgs,dS1,dS2);}
}

void WG_Part::Info()
{
outtext1("Wave Guide Part");
G_Object::Info();
outtext1(Name);
}


C3dVector WG_Part::MinPt(C3dVector inPt)
{
C3dVector vRet;
vRet.x=this->inPt->Pt_Point->x;
vRet.y=this->inPt->Pt_Point->y;
vRet.z=this->inPt->Pt_Point->z;

return (vRet);
}

ME_Object* WG_Part::GetMesh()
{
return (Mesh);
}

void WG_Part::AttachMesh(ME_Object* pM,C3dMatrix TMat,C3dVector vA,C3dVector vB)
{
Mesh=pM;
TMat.Translate2(inPt->Pt_Point->x,inPt->Pt_Point->y,inPt->Pt_Point->z);
Mesh->Transform(TMat);
AttachTform(TMat);
Mesh->pParent=this;
}

void WG_Part::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
if (inPt!=NULL)
  {inPt->SetToScr(pModMat,pScrTran);}
if (Mesh!=NULL)
  {Mesh->SetToScr(pModMat,pScrTran);}
}

G_ObjectD WG_Part::SelDist(CPoint InPT,Filter FIL)
{
double d;
G_ObjectD Ret;
d=1000;

if (Mesh==NULL)
{
  Ret=inPt->SelDist(InPT,FIL);
  Ret.pObj=this;
}
else
{
  Ret=Mesh->SelDist(InPT,FIL);
}
return (Ret);
}

void WG_Part::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
if (Mesh!=NULL)
{
  Mesh->S_Box(P1,P2,pSel);
}
}




C3dVector WG_Part::Get_Centroid()
{
return (inPt->Get_Centroid());
}


void WG_Part::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{  		// TODO: add storing code here
      G_Object::Serialize(ar,iV);
	  ar<<OMat.m_00;ar<<OMat.m_01;ar<<OMat.m_02;ar<<OMat.m_03;
      ar<<OMat.m_10;ar<<OMat.m_11;ar<<OMat.m_12;ar<<OMat.m_13;
      ar<<OMat.m_20;ar<<OMat.m_21;ar<<OMat.m_22;ar<<OMat.m_23;
      ar<<OMat.m_30;ar<<OMat.m_31;ar<<OMat.m_32;ar<<OMat.m_33;
      ar<<Name;
      ar<<bFlipZ;
	  inPt->Serialize(ar,iV);
	  int iISMesh=0;
	  if (Mesh!=NULL)
	  {
       iISMesh=1;
       ar<<iISMesh;
       Mesh->Serialize(ar,iV);
	  }
	  else
      {
	   iISMesh=0;
       ar<<iISMesh;
	  }
	}
	else
	{
      G_Object::Serialize(ar,iV);
	    ar>>OMat.m_00;ar>>OMat.m_01;ar>>OMat.m_02;ar>>OMat.m_03;
      ar>>OMat.m_10;ar>>OMat.m_11;ar>>OMat.m_12;ar>>OMat.m_13;
      ar>>OMat.m_20;ar>>OMat.m_21;ar>>OMat.m_22;ar>>OMat.m_23;
      ar>>OMat.m_30;ar>>OMat.m_31;ar>>OMat.m_32;ar>>OMat.m_33;
      ar>>Name;
      ar>>bFlipZ;
	    inPt=new CvPt_Object;
	    inPt->Serialize(ar,iV);
	    int iISMesh=0;
      ar>>iISMesh;
	    if (iISMesh==1)
	    {
	      Mesh = new ME_Object();
	      Mesh->pParent=this;
        Mesh->Serialize(ar,iV);
	    }
    }
}


void WG_Part::AttachTform(C3dMatrix inMat)
{
  OMat=inMat;
}

void WG_Part::FlipZ()
{
C3dMatrix invOMat;
C3dMatrix ZRot;

if (Mesh!=NULL)
{
  ZRot.Rotate(0,0,180);
  invOMat.MakeUnit();
  invOMat.m_30=-inPt->Pt_Point->x;
  invOMat.m_31=-inPt->Pt_Point->y;
  invOMat.m_32=-inPt->Pt_Point->z;
  Mesh->Transform(invOMat);
  invOMat.MakeUnit();
  invOMat=OMat;
  invOMat.Transpose();
  invOMat.m_30=0;
  invOMat.m_31=0;
  invOMat.m_32=0;
  Mesh->Transform(invOMat);

  invOMat=OMat*ZRot;
  invOMat.m_30=inPt->Pt_Point->x;
  invOMat.m_31=inPt->Pt_Point->y;
  invOMat.m_32=inPt->Pt_Point->z;
  Mesh->Transform(invOMat);
  AttachTform(invOMat);
}
}


void WG_Part::FlipY()
{
C3dMatrix invOMat;
C3dMatrix ZRot;

if (Mesh!=NULL)
{
  ZRot.Rotate(0,90,0);
  invOMat.MakeUnit();
  invOMat.m_30=-inPt->Pt_Point->x;
  invOMat.m_31=-inPt->Pt_Point->y;
  invOMat.m_32=-inPt->Pt_Point->z;
  Mesh->Transform(invOMat);
  invOMat.MakeUnit();
  invOMat=OMat;
  invOMat.Transpose();
  invOMat.m_30=0;
  invOMat.m_31=0;
  invOMat.m_32=0;
  Mesh->Transform(invOMat);

  invOMat=OMat*ZRot;
  invOMat.m_30=inPt->Pt_Point->x;
  invOMat.m_31=inPt->Pt_Point->y;
  invOMat.m_32=inPt->Pt_Point->z;
  Mesh->Transform(invOMat);
  AttachTform(invOMat);
}
}

IMPLEMENT_DYNAMIC( WG_Elbow, CObject )

WG_Elbow::WG_Elbow()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 601;
iLabel = -1;
iColour = 2;
inPt=NULL;
isHBend=0;
}

void WG_Elbow::Create(int iLab,CString inName,
					 C3dVector inP,int HB,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 601;
iLabel = iLab;
iColour = 2;
pParent=Parrent;
Name=inName;
if (inPt!=NULL)
{ delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,11,this);
isHBend=HB;
}

void WG_Elbow::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{  		// TODO: add storing code here
      WG_Part::Serialize(ar,iV);
      ar<<isHBend;
	}
	else
	{
      WG_Part::Serialize(ar,iV);
      ar>>isHBend;
    }
}


void WG_Elbow::AttachMesh(ME_Object* pM,C3dMatrix TMat,C3dVector vA,C3dVector vB)
{

C3dMatrix Forward;
C3dMatrix Back;

C3dVector vZ;
C3dVector vNA;
C3dVector vNB;
TMat.MakeUnit();
vA*=-1;
vZ=vA.Cross(vB);
vB=vZ.Cross(vA);
Forward.SetColVec(1,vA);
Forward.SetColVec(2,vB);
Forward.SetColVec(3,vZ);

Mesh=pM;
Pt_Object* pANd =Mesh->GetNode(1);
Pt_Object* pBNd =Mesh->GetNode(2);
if ((pANd!=NULL) && (pBNd!=NULL))
{
  vNA = pANd->GetCoords();
  vNB = pBNd->GetCoords();
  vNA.Normalize();
  vNB.Normalize();
  vZ=vNA.Cross(vNB);
  vNB=vZ.Cross(vNA);
  Back.SetColVec(1,vNA);
  Back.SetColVec(2,vNB);
  Back.SetColVec(3,vZ);
  Back.Transpose();
  TMat*=Forward;
  TMat*=Back;
}
TMat.Translate2(inPt->Pt_Point->x,inPt->Pt_Point->y,inPt->Pt_Point->z);
Mesh->Transform(TMat);
AttachTform(TMat);
Mesh->pParent=this;
}

IMPLEMENT_DYNAMIC( WG_Brk, CObject )

WG_Brk::WG_Brk()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 602;
iLabel = -1;
iColour = 2;
inPt=NULL;
OPt=NULL;
}

void WG_Brk::Create(int iLab,CString inName,
					 C3dVector inP,C3dVector inOP,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 602;
iLabel = iLab;
iColour = 2;
pParent=Parrent;
Name=inName;
if (inPt!=NULL)
{ delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,11,this);
if (OPt!=NULL)
{ delete(OPt);}
OPt=new CvPt_Object;
OPt->Create(inOP,0,-1,0,0,11,this);
}

void WG_Brk::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{  		// TODO: add storing code here
      WG_Part::Serialize(ar,iV);
      OPt->Serialize(ar,iV);
	}
	else
	{
      WG_Part::Serialize(ar,iV);
	    OPt=new CvPt_Object;
      OPt->Serialize(ar,iV);
    }
}


G_ObjectD WG_Brk::SelDist(CPoint InPT,Filter FIL)
{
double Minds;
G_ObjectD Ret;
G_Object* pMinSel;
Minds=1e36;
Ret.pObj=NULL;
Ret.Dist=Minds;

if (Mesh!=NULL)
{
  Ret=Mesh->SelDist(InPT,FIL);
  pMinSel=Ret.pObj;
  Minds=Ret.Dist;
}

if (inPt!=NULL)
{
  Ret=inPt->SelDist(InPT,FIL);
  if (Ret.Dist<Minds)
  {
    pMinSel=Ret.pObj;
    Minds=Ret.Dist;
  }
}
if (OPt!=NULL)
{
  Ret=OPt->SelDist(InPT,FIL);
  if (Ret.Dist<Minds)
  {
    pMinSel=Ret.pObj;
    Minds=Ret.Dist;
  }
}
Ret.Dist=Minds;
Ret.pObj=pMinSel;

return (Ret);
}

void WG_Brk::AttachMesh(ME_Object* pM,C3dMatrix TMat,C3dVector vA,C3dVector vB)
{
Mesh=pM;
TMat.Translate2(inPt->Pt_Point->x,inPt->Pt_Point->y,inPt->Pt_Point->z);
Mesh->Transform(TMat);
AttachTform(TMat);
Pt_Object* pXNd =Mesh->GetNode(3);
if (pXNd!=NULL)
{
   C3dVector XPt;
	 C3dVector XAcctPt;
   XPt.x=pXNd->Pt_Point->x-inPt->Pt_Point->x;
   XPt.y=pXNd->Pt_Point->y-inPt->Pt_Point->y;
   XPt.z=pXNd->Pt_Point->z-inPt->Pt_Point->z;
	 XPt.Normalize();
   XAcctPt.x=OPt->Pt_Point->x-inPt->Pt_Point->x;
	 XAcctPt.y=OPt->Pt_Point->y-inPt->Pt_Point->y;
	 XAcctPt.z=OPt->Pt_Point->z-inPt->Pt_Point->z;
	 XAcctPt.Normalize();
	 double ddot;
   ddot=XPt.Dot(XAcctPt);
	 if (ddot<-0.5)
	 {
			FlipZ();
	 }	      }

Mesh->pParent=this;
}

void WG_Brk::Draw(CDC* pDC,int iDrawmode)
{
WG_Part::Draw(pDC,iDrawmode);
if ((inPt!=NULL) && (OPt!=NULL))
  {
   inPt->Draw(pDC,iDrawmode);
   pDC->MoveTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) OPt->DSP_Point->x,(int) OPt->DSP_Point->y);

  }

}

void WG_Brk::OglDraw(int iDspFlgs,double dS1,double dS2)
{
if (OPt!=NULL)
  {OPt->OglDraw(iDspFlgs,dS1,dS2);}
if (Mesh!=NULL)
  {Mesh->OglDraw(iDspFlgs,dS1,dS2);}
}

void WG_Brk::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if (inPt!=NULL)
{
  glColor3fv(cols[inPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) inPt->Pt_Point->x,(float) inPt->Pt_Point->y,(float) inPt->Pt_Point->z);
  glEnd();
}
if (OPt!=NULL)
{
  glColor3fv(cols[OPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) OPt->Pt_Point->x,(float) OPt->Pt_Point->y,(float) OPt->Pt_Point->z);
  glEnd();
}
if (Mesh!=NULL)
  {Mesh->OglDrawW(iDspFlgs,dS1,dS2);}
}

void WG_Brk::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
if ((inPt!=NULL) && (OPt!=NULL))
  {
	inPt->SetToScr(pModMat,pScrTran);
	OPt->SetToScr(pModMat,pScrTran);
  }
if (Mesh!=NULL)
{Mesh->SetToScr(pModMat,pScrTran);}
}
//******************************************************
IMPLEMENT_DYNAMIC(WG_Brk_Int, CObject )

WG_Brk_Int::WG_Brk_Int()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 605;
iLabel = -1;
iColour = 2;
inPt=NULL;
OPt=NULL;
OPt2=NULL;
}

void WG_Brk_Int::Create(int iLab,CString inName,
					  C3dVector inP,C3dVector inOP,C3dVector inOP2,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 605;
iLabel = iLab;
iColour = 2;
pParent=Parrent;
Name=inName;
if (inPt!=NULL)
{ delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,11,this);
if (OPt!=NULL)
{ delete(OPt);}
OPt=new CvPt_Object;
OPt->Create(inOP,0,-1,0,0,11,this);
if (OPt2!=NULL)
{ delete(OPt2);}
OPt2=new CvPt_Object;
OPt2->Create(inOP2,0,-1,0,0,11,this);
}

void WG_Brk_Int::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{  		// TODO: add storing code here
      WG_Part::Serialize(ar,iV);
      OPt->Serialize(ar,iV);
      OPt2->Serialize(ar,iV);
	}
	else
	{
      WG_Part::Serialize(ar,iV);
	    OPt=new CvPt_Object;
      OPt->Serialize(ar,iV);
      OPt2=new CvPt_Object;
      OPt2->Serialize(ar,iV);
    }
}

G_ObjectD WG_Brk_Int::SelDist(CPoint InPT,Filter FIL)
{
double Minds;
G_ObjectD Ret;
G_Object* pMinSel;

Minds=1e36;
Ret.pObj=NULL;
Ret.Dist=Minds;
Ret=WG_Brk::SelDist(InPT,FIL);
Minds=Ret.Dist;
pMinSel=Ret.pObj;

if (OPt2!=NULL)
{
  Ret=OPt2->SelDist(InPT,FIL);
  if (Ret.Dist<Minds)
  {
    pMinSel=Ret.pObj;
    Minds=Ret.Dist;
  }
}
Ret.Dist=Minds;
Ret.pObj=pMinSel;
return(Ret);
}


void WG_Brk_Int::AttachMesh(ME_Object* pM,C3dMatrix TMat,C3dVector vA,C3dVector vB)
{
C3dVector vD;
double dd=0;
double dMin=100000;
C3dVector vX;
C3dVector vY;
C3dVector vZ;
C3dVector vDown;
C3dVector vFEM;
C3dVector vREP;
TMat.ClearTranslations();

double dAng=0;
int i;

if (vA.Mag()==0)
{
  vA=vB;
}

Mesh=pM;
Pt_Object* pXNd1 =Mesh->GetNode(3);
//Pt_Object* pXNd2 =Mesh->GetNode(4);
C3dMatrix SaveTMat;
SaveTMat=TMat;
C3dMatrix ZRot;
vREP=OPt->GetCoords();
C3dMatrix MinTmat;
if (pXNd1!=NULL)
  {
  for (i=0;i<8;i++)
  {
    ZRot.Rotate(0,0,dAng);
    TMat=SaveTMat*ZRot;
    vFEM=pXNd1->Get_Centroid();
    vFEM=TMat.Mult(vFEM);
    vFEM+=inPt->GetCoords();
    vD=vFEM;
    vD-=vREP;
    dd=vD.Mag();
    if (dd<dMin)
    {
      dMin=dd;
      MinTmat=TMat;
    }
    if (i==3)
    {
       vX=SaveTMat.GetColVec(1);
       vZ=SaveTMat.GetColVec(3);
       vX*=-1;
       vZ*=-1;
       SaveTMat.SetColVec(1,vX);
       SaveTMat.SetColVec(3,vZ);
    }
    dAng+=90.0;
  }  
}
else
{
  outtext1("Mesh cannot be orientated chexk nodes 3 & 4");
}
MinTmat.Translate2(inPt->Pt_Point->x,inPt->Pt_Point->y,inPt->Pt_Point->z);
Mesh->Transform(MinTmat);
AttachTform(MinTmat);
Mesh->pParent=this;
}

void WG_Brk_Int::Draw(CDC* pDC,int iDrawmode)
{
WG_Part::Draw(pDC,iDrawmode);
if ((inPt!=NULL) && (OPt!=NULL) && (OPt2!=NULL))
  {
   inPt->Draw(pDC,iDrawmode);
   pDC->MoveTo((int) inPt->DSP_Point->x,(int) inPt->DSP_Point->y);
   pDC->LineTo((int) OPt->DSP_Point->x,(int) OPt->DSP_Point->y);
   pDC->LineTo((int) OPt2->DSP_Point->x,(int) OPt2->DSP_Point->y);
  }

}

void WG_Brk_Int::OglDraw(int iDspFlgs,double dS1,double dS2)
{
if (OPt!=NULL)
  {OPt->OglDraw(iDspFlgs,dS1,dS2);}
if (OPt2!=NULL)
  {OPt2->OglDraw(iDspFlgs,dS1,dS2);}
if (Mesh!=NULL)
  {Mesh->OglDraw(iDspFlgs,dS1,dS2);}
}

void WG_Brk_Int::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if (inPt!=NULL)
{
  glColor3fv(cols[inPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) inPt->Pt_Point->x,(float) inPt->Pt_Point->y,(float) inPt->Pt_Point->z);
  glEnd();
}
if (OPt!=NULL)
{
  glColor3fv(cols[OPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) OPt->Pt_Point->x,(float) OPt->Pt_Point->y,(float) OPt->Pt_Point->z);
  glEnd();
}
if (OPt2!=NULL)
{
  glColor3fv(cols[OPt->GetCol()]);
  glPointSize(16.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) OPt2->Pt_Point->x,(float) OPt2->Pt_Point->y,(float) OPt2->Pt_Point->z);
  glEnd();
}
if (Mesh!=NULL)
  {Mesh->OglDrawW(iDspFlgs,dS1,dS2);}
}

void WG_Brk_Int::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
if ((inPt!=NULL) && (OPt!=NULL) && (OPt2!=NULL))
  {
	inPt->SetToScr(pModMat,pScrTran);
	OPt->SetToScr(pModMat,pScrTran);
  OPt2->SetToScr(pModMat,pScrTran);
  }
if (Mesh!=NULL)
{Mesh->SetToScr(pModMat,pScrTran);}
}
//*****************************************************

IMPLEMENT_DYNAMIC( Section, CObject )

Section::~Section()
{
int i;
for (i=0;i<iLnCnt;i++)
{
  delete (pLn[i]);
  pLn[i]=NULL;
}
}

void Section::Create(int iLab,int inPID,int inPID2,int inSec,int inCol,G_Object* Parrent)
{
TMat.MakeUnit();
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 200;
iLabel = iLab;
iColour = inCol;
iLnCnt=0;
iSec=inSec;
iPID=inPID;
iPID2=inPID2;
pParent=Parrent;
}



void Section::Serialize(CArchive& ar,int iV)
{
  int i;
  int iT;
	if (ar.IsStoring())
	{
		// TODO: add storing code here

	G_Object::Serialize(ar,iV);
	ar<<iSec;
    ar<<iPID;
	ar<<iPID2;
    ar<<TMat.m_00;ar<<TMat.m_01;ar<<TMat.m_02;ar<<TMat.m_03;
    ar<<TMat.m_10;ar<<TMat.m_11;ar<<TMat.m_12;ar<<TMat.m_13;
    ar<<TMat.m_20;ar<<TMat.m_21;ar<<TMat.m_22;ar<<TMat.m_23;
    ar<<TMat.m_30;ar<<TMat.m_31;ar<<TMat.m_32;ar<<TMat.m_33;
	  inPt.Serialize(ar,iV);
    ar<<iLnCnt;
    for(i=0;i<iLnCnt;i++)
    {
      ar << NoSegs[i];
      ar << pLn[i]->iObjType;
      pLn[i]->Serialize(ar,iV);;
    }
	}
	else
	{
    G_Object::Serialize(ar,iV);
	  ar>>iSec;
    ar>>iPID;
	  ar>>iPID2;
    ar>>TMat.m_00;ar>>TMat.m_01;ar>>TMat.m_02;ar>>TMat.m_03;
    ar>>TMat.m_10;ar>>TMat.m_11;ar>>TMat.m_12;ar>>TMat.m_13;
    ar>>TMat.m_20;ar>>TMat.m_21;ar>>TMat.m_22;ar>>TMat.m_23;
    ar>>TMat.m_30;ar>>TMat.m_31;ar>>TMat.m_32;ar>>TMat.m_33;
	  inPt.Serialize(ar,iV);
    ar>>iLnCnt;
    for(i=0;i<iLnCnt;i++)
    {
      ar >> NoSegs[i];
      ar >> iT;
      if (iT==8)
      {
        pLn[i]=new NCircle;
        pLn[i]->Serialize(ar,iV);
      }
      else
      {
        pLn[i]=new NLine;
        pLn[i]->Serialize(ar,iV);
      }
    }
	}	
}

G_Object* Section::Copy(G_Object* Parrent)
{
int i;
Section* NewSec = new Section;
NewSec->Create(iLabel,iPID,iPID2,iSec,iColour,NULL);
for (i=0;i<iLnCnt;i++)
{
  NewSec->pLn[i]=(NCurve*) pLn[i]->Copy(this);
  NewSec->NoSegs[i]=NoSegs[i];

}
NewSec->iLnCnt=iLnCnt;
NewSec->pParent=Parrent;
return (NewSec);
}

// Draw Object line
void Section::Draw(CDC* pDC,int iDrawmode)
{
int j=0;
if (iLnCnt != 0)
{
for (j=0;j<iLnCnt;j++)
   {
   pLn[j]->HighLight(pDC);
   }
}
}

void Section::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
  OglDraw(iDspFlgs,dS1,dS2);
}

void Section::OglDraw(int iDspFlgs,double dS1,double dS2)
{
int j=0;
if (iLnCnt != 0)
{
for (j=0;j<iLnCnt;j++)
   {
   pLn[j]->OglDraw(iDspFlgs,dS1,dS2);
   }
}

}


void Section::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int j;
if (iLnCnt != 0)
{
  for (j=0;j<iLnCnt;j++)
  {
    pLn[j]->SetToScr(pModMat,pScrTran);
  }
}
}

void Section::Transform(C3dMatrix inTMat)
{
TMat=inTMat;
int j=0;
if (iLnCnt != 0)
  {
  for (j=0;j<iLnCnt;j++)
   {
    pLn[j]->Transform(inTMat);
   }
  }
}



C3dVector Section::Get_Centroid()
{
C3dVector vT;
vT.x = 0; //to be done
vT.y = 0;
vT.z = 0;
return (vT);
}


void Section::HighLight(CDC* pDC)
{
Draw(pDC,4);
}


G_ObjectD Section::SelDist(CPoint InPT,Filter FIL)
{
G_ObjectD Ret;
Ret.Dist=1000;
Ret.pObj=this;
return (Ret);
}

void Section::ChangeCol(int iCOl)
{
int i;
for (i=0;i<iLnCnt;i++)
  {pLn[i]->iColour= iCOl;}
}

void Section::AddLine(double X1,double Y1,
                      double X2,double Y2,
                      int NoDivs)
{
C3dVector pt1;
pt1.Set(X1,Y1,0);
C3dVector pt2;
pt2.Set(X2,Y2,0);
if (iLnCnt < 50) 
{
  NLine* pL = new NLine();

  pL->Create(pt1,pt2,iLnCnt,this);
  pL->pParent=this;
  pLn[iLnCnt] = pL;
  NoSegs[iLnCnt] =NoDivs;
  iLnCnt++;
}
}

void Section::MakeSpace(int iPos)
{
int i;
if (iLnCnt<50)
{
  for (i=iLnCnt;i>iPos;i--)
  {
    NoSegs[i] =NoSegs[i-1];
    pLn[i] =pLn[i-1];
  }
  pLn[iPos]=NULL;
  iLnCnt++;
}

}

void Section::FilletCrns(double dR)
{
NCircle* pC;
NoSegs[0]-=2;
NoSegs[1]-=2;
NoSegs[2]-=2;
NoSegs[3]-=2;

pC=NFillet((NLine*) pLn[0],(NLine*) pLn[1],dR);
MakeSpace(1);
pLn[1]=(NCircle*) pC;
pLn[1]->pParent=this;
NoSegs[1]=2;
pC=NFillet((NLine*) pLn[2],(NLine*) pLn[3],dR);
MakeSpace(3);
pLn[3]=(NCircle*) pC;
pLn[3]->pParent=this;
NoSegs[3]=2;
pC=NFillet((NLine*) pLn[4],(NLine*) pLn[5],dR);
MakeSpace(5);
pLn[5]=(NCircle*) pC;
pLn[5]->pParent=this;
NoSegs[5]=2;
  pC=NFillet((NLine*) pLn[6],(NLine*) pLn[0],dR);
  MakeSpace(7);
  pLn[7]=(NCircle*) pC;
  pLn[7]->pParent=this;
  NoSegs[7]=2;
}


int Section::GetNoLines()
{
return(iLnCnt);
}

int Section::GetDivs(int iL)
{
return (NoSegs[iL]);
}


C3dVector Section::GetPt(int iSeg,int iPos)
{
C3dVector vRet;


double dw;
double dPos;
dPos=iPos;
dw=dPos/NoSegs[iSeg];
if ((pLn[iSeg]->iObjType==7) && (pLn[iSeg]->iType == 3))
{
  dw*=0.25;
}

vRet=pLn[iSeg]->GetPt(dw);
return (vRet);
}

BOOL Section::isClosedSec()
{
BOOL brc = FALSE;
double dDist;
double dw=1;
C3dVector S;
C3dVector E;
S=pLn[0]->GetPt(0);
if ((pLn[iLnCnt-1]->iObjType==7) && (pLn[iLnCnt - 1]->iType == 3))
{
  dw*=0.25;
}
E=pLn[iLnCnt-1]->GetPt(dw);
dDist=S.Dist(E);
if (dDist<0.00001)
{
  brc=TRUE;
}
return (brc);
}

C3dVector Section::NLnInt(NCurve* L1,NCurve* L2)
{
int i=0;
double MinDist = 10000000;
double dDist = 0;
double dDistB = 0;
const double dTol = 0.00001;
C3dVector P1;
C3dVector P2;
int iMaxIt=0;
P1=L1->GetPt(0);

do
{
  P2=L2->MinPt(P1);
  P1=L1->MinPt(P2);
  dDist=P2.Dist(P1);
  iMaxIt++;
}
while ((dDist>dTol) && (iMaxIt<100000));
return (P2);
}

//This is taken from DBase
NCircle* Section::NFillet(NLine* Ln,NLine* Ln1,double dR)
{
C3dVector PNear1;
C3dVector PNear2;
double R;
R=dR;
C3dVector p1;
C3dVector p2;
C3dVector p3;
C3dVector pT;
C3dVector vL1Dir;
C3dVector* pLT1;
C3dVector* pLT2;
C3dVector v1;
C3dVector v2;
C3dVector v3;
C3dVector vRef;
C3dVector vDir;
// The intersection of the lines



v1=Ln->GetDir(0.5);

v2=Ln1->GetDir(0.5);;
p1=Ln->GetPt(0);
pLT1=Ln->cPts[1]->Pt_Point;
p2=NLnInt(Ln,Ln1);
//p2=pLT1;
p3=Ln1->GetPt(1);
pLT2=Ln1->cPts[0]->Pt_Point;
C3dVector vn;
vn=v1.Cross(v2);
vn.Normalize();
C3dVector v1o;
C3dVector v2o;

v1o=v1.Cross(vn);
v2o=v2.Cross(vn);
v1o.Normalize();
v2o.Normalize();
v1o*=R;
v2o*=R;
if (v1o.Dot(v2)<0)
  v1o*=-1;
if (v2o.Dot(v1)>0)
  v2o*=-1;
C3dVector p1o;
C3dVector p2o;
C3dVector p3o;
C3dVector p4o;

p1o=p1;
p1o+=v1o;
p2o=p2;
p2o+=v1o;
p3o=p2;
p3o+=v2o;
p4o=p3;
p4o+=v2o;
NLine* Ln2 = new NLine;
Ln2->Create(p1o,p2o,1,NULL);
NLine* Ln3 = new NLine;
Ln3->Create(p3o,p4o,1,NULL);
C3dVector IntPt;
IntPt=NLnInt(Ln2,Ln3);


C3dVector IntPt1;
IntPt1=Ln->MinPt(IntPt);
vRef=IntPt1;
vRef-=IntPt;
vRef.Normalize();

NCircle* cCir = new NCircle();
cCir->Create2(vn,IntPt,vRef,R,-1,NULL);
cCir->vNorm=vn; // Note the transform method in Create2 changes the normal
                //so set it back 
pLT1->Set(IntPt1.x,IntPt1.y,IntPt1.z);  //trim the end point
C3dVector IntPt2;
IntPt2=Ln1->MinPt(IntPt);
pLT2->Set(IntPt2.x,IntPt2.y,IntPt2.z);  //trim the end point

//d1q=cCir->MinWPt(IntPt1);

//vRef=IntPt1;
//vDir=cCir->GetDir(d1q);
//vDir.Normalize();
//vRef-=cCir->vCent;
//vRef.Normalize();
  //if (vDir.Dot(vL1Dir)<0)
  //{
  //  vn*=-1;
  //}
  //delete (cCir);
  //cCir = new NCircle();
  //cCir->Create2(vn,IntPt,vRef,R,-1,NULL);
  //d1q=cCir->MinWPt(IntPt1);
double d2q;
d2q=cCir->MinWPt(IntPt2);
cCir->we=d2q;

delete (Ln2);
delete (Ln3);

return(cCir);
}

void Section::SetInPt(C3dVector Pt)
{
inPt = Pt;
}




IMPLEMENT_DYNAMIC( Sweep, CObject )

Sweep::Sweep()
{
pPath=NULL; 
pBaseSec=NULL;
Mesh=NULL;
iSecCnt=0;
Twist=0;
isVoid=FALSE;
pParent=NULL;
iMeshCnt = 0;
isStart=FALSE;
isEnd=FALSE;
iObjType = 603;
}

void Sweep::PreCreate(int iLab,CString inName,double dT)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
Name=inName;
Twist=dT;
}

void Sweep::Create(int iLab,G_Object* inPath,
                   Section* inSec,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
Mesh=NULL;
isVoid=FALSE;
pPath= inPath->Copy(this); 
pBaseSec=(Section*) inSec->Copy(this);
pPath->pParent=this;
pBaseSec->pParent=this;
pParent=Parrent;
}

void Sweep::Serialize(CArchive& ar,int iV)
{
  int i;
  int iTp;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
	  ar<<iMeshCnt;
    ar<<iSecCnt;
    ar<<Twist;
	  ar<<isVoid;
    ar << isStart;
    ar << isEnd;
    ar << dElLength;
    ar << pPath->iObjType;
    pPath->Serialize(ar,iV); 
    pBaseSec->Serialize(ar,iV); 
    for (i=0;i<iSecCnt;i++)
    {
      pAllSecs[i]->Serialize(ar,iV); 
    }
	  int iISMesh=0;
	  if (Mesh!=NULL)
	  {
       iISMesh=1;
       ar<<iISMesh;
       Mesh->Serialize(ar,iV);
	  }
	  else
    {
	     iISMesh=0;
       ar<<iISMesh;
	  }
	}
	else
	{
    G_Object::Serialize(ar,iV);
	  ar>>iMeshCnt;
    ar>>iSecCnt;
    ar>>Twist;
    ar>>isVoid;
    ar >> isStart;
    ar >> isEnd;
    ar >> dElLength;
    ar >>iTp;
    if (iTp==9)
    {
      pPath=new NLine;
    }
    else if (iTp==7)
    {
      pPath= new NCurve;
    }
    else if (iTp==8)
    {
      pPath= new NCircle;
    }
    pPath->Serialize(ar,iV);
	  pPath->pParent=this;
    pBaseSec=new Section;
    pBaseSec->Serialize(ar,iV); 
    pBaseSec->pParent=this;
    for (i=0;i<iSecCnt;i++)
    {
      pAllSecs[i]=new Section;
      pAllSecs[i]->Serialize(ar,iV); 
	    pAllSecs[i]->pParent=this;
    }
	  int iISMesh=0;
    ar>>iISMesh;
	  if (iISMesh==1)
	  {
	   Mesh = new ME_Object();
     Mesh->Serialize(ar,iV);
	   Mesh->pParent=this;
	  }
	}
}

C3dVector Sweep::GetInPt()
{
C3dVector vRet;
return (vRet);
}

C3dVector Sweep::GetDir(double w)
{

C3dVector vN;
vN=pPath->GetDir(w);
return(vN);
}

C3dVector Sweep::MinPt(C3dVector inPt)
{
C3dVector vRet(0,0,0);
if (pPath!=NULL)
{
  vRet=pPath->GetPt(0.5);
}
return (vRet);
}

// Draw Object line
void Sweep::Draw(CDC* pDC,int iDrawmode)
{
int j=0;
if (iSecCnt != 0)
{
  for (j=0;j<iSecCnt;j++)
  {
    pAllSecs[j]->Draw(pDC,iDrawmode);
  }
}
if (pPath != NULL)
{
  pPath->Draw(pDC,iDrawmode);
}
if (Mesh!=NULL)
{
  Mesh->Draw(pDC,iDrawmode);
}
}

void Sweep::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int j=0;
if (iSecCnt != 0)
{
for (j=0;j<iSecCnt;j++)
   {
   pAllSecs[j]->OglDrawW(iDspFlgs,dS1,dS2);
   }
}
if (pPath != NULL)
{
   pPath->OglDrawW(iDspFlgs,dS1,dS2);
}
if (Mesh!=NULL)
{
  Mesh->OglDrawW(iDspFlgs,dS1,dS2);
}
}

void Sweep::OglDraw(int iDspFlgs,double dS1,double dS2)
{

int j=0;
if (iSecCnt != 0)
{
for (j=0;j<iSecCnt;j++)
   {
   pAllSecs[j]->OglDraw(iDspFlgs,dS1,dS2);
   }
}
if (pPath != NULL)
{
   pPath->OglDraw(iDspFlgs,dS1,dS2);
}
if (Mesh!=NULL)
{
  Mesh->OglDraw(iDspFlgs,dS1,dS2);
}
}


void Sweep::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int j=0;
if (iSecCnt != 0)
{
  for (j=0;j<iSecCnt;j++)
  {
    pAllSecs[j]->SetToScr(pModMat,pScrTran);
  }
}
if (pPath != NULL)
{
  pPath->SetToScr(pModMat,pScrTran);
}
if (Mesh!=NULL)
 {Mesh->SetToScr(pModMat,pScrTran);}

}

void Sweep::Transform(C3dMatrix inTMat)
{
int j=0;
if (iSecCnt != 0)
{
  for (j=0;j<iSecCnt;j++)
  {
    pAllSecs[j]->Transform(inTMat);
  }
}
if (pPath != NULL)
{
  pPath->Transform(inTMat);
}
}



C3dVector Sweep::Get_Centroid()
{
C3dVector vT;
if (pPath!=NULL)
{
  vT=pPath->Get_Centroid();
}
return (vT);
}


void Sweep::HighLight(CDC* pDC)
{
int j=0;
if (iSecCnt != 0)
{
  for (j=0;j<iSecCnt;j++)
  {
    pAllSecs[j]->Draw(pDC,4);
  }
}
if (pPath != NULL)
{
  pPath->Draw(pDC,4);
}
}


G_ObjectD Sweep::SelDist(CPoint InPT,Filter FIL)
{
G_ObjectD Ret;
double dS=10000;
if ((pPath!=NULL) && (Mesh==NULL))
{
  Ret=pPath->SelDist(InPT,FIL);
  Ret.pObj=this;
}
else if (Mesh!=NULL)
{
  Ret=Mesh->SelDist(InPT,FIL);
}
else
{
  Ret.pObj=this;
}
return (Ret);
}

void Sweep::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
if (Mesh!=NULL)
{
  Mesh->S_Box(P1,P2,pSel);
}
} 

void Sweep::DeleteSections()
{
int i; 
for (i=0;i<iSecCnt;i++)
{	
  //pAllSecs[i]->~Section();
  delete(pAllSecs[i]);
}
iSecCnt=0;
}

double Sweep::GetSecSpacing()
{
double dRet=0;
C3dVector p1,p2;
if (iSecCnt>1)
{
  p1=pAllSecs[0]->GetPt(0,0);
  p2=pAllSecs[1]->GetPt(0,0);
  dRet=p1.Dist(p2);
}


return(dRet);
}


void Sweep::CutBack(BOOL bEA,BOOL bEB,ME_Object* MeshA,ME_Object* MeshB)
{
double cBW;
double dDist;
C3dVector pCPt;
C3dVector p1;
C3dVector vDirO;
C3dVector vDirN;
Pt_Object* pCN1;
Pt_Object* pCN2;
C3dMatrix mUp = GetFirstYMap();
NLine* pL;
pL = (NLine*) pPath;
vDirO=pL->GetDir(0);
double dD = GetSecSpacing();
DeleteSections();
double dLen=pL->getLen();
cBW=0.75*dD/dLen;
if ((MeshA!=NULL) && (bEA==TRUE))
{
  //Cutback nodes in standaard parts definition
  pCN1=MeshA->GetNode(1);
  pCN2=MeshA->GetNode(2);
  if (pCN1!=NULL)
  {
    pCPt=pCN1->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[0]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  } 
  if (pCN2!=NULL)
  {
    pCPt=pCN2->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[0]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  }
}
else if ((MeshA==NULL) && (bEA==TRUE))
{
  p1=pL->GetPt(cBW);
  pL->cPts[0]->Pt_Point->Set(p1.x,p1.y,p1.z);
}


if ((MeshB!=NULL) && (bEB==TRUE))
{
  //Cutback nodes in standaard parts definition
  pCN1=MeshB->GetNode(1);
  pCN2=MeshB->GetNode(2);
  if (pCN1!=NULL)
  {
    pCPt=pCN1->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[1]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  } 
  if (pCN2!=NULL)
  {
    pCPt=pCN2->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[1]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  }
}
else if ((MeshB==NULL) && (bEB==TRUE))
{
  p1=pL->GetPt(1-cBW);
  pL->cPts[1]->Pt_Point->Set(p1.x,p1.y,p1.z);
}
vDirN=pL->GetDir(0);
double dDot;
dDot=vDirO.Dot(vDirN);
if (dDot<0)
{
  isVoid=TRUE;
}
Generate(mUp,dElLength);
}



void Sweep::Generate(C3dMatrix mUp,double dElLen)
{
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
#endif
dElLength=dElLen;
C3dVector Loc;
C3dVector Dir;
C3dMatrix TMat;
C3dMatrix OMat;
double dAng;
double dA=0;
int i;
int k=0;
double w;
double dInc;
double dRem;
int iSteps;
double dSteps;
double dLen;
#ifdef _DEBUG
   oldMemState.Checkpoint();
#endif 
if (isVoid==FALSE)
{
if (dElLen<=0)
{
  dElLen=0.005;
}
if (pPath->iObjType == 7)
{  
  dLen=pPath->getLen();
  dSteps=dLen/dElLen;
  iSteps=(int) dSteps;
  
  dRem=dSteps-iSteps;

  if (iSteps==0)
  {
	iSteps=1;    
  }
  if (dRem>0.5)
  {
	iSteps+=1;
  }

  dSteps=(double) iSteps;
  dInc=1/dSteps;
  
  w=0;
  for (i=0;i<iSteps+1;i++)
  {
    Dir = pPath->GetDir(w);
    TMat=TMat.CalcTran(Dir);
    Loc = pPath->GetPt(w);
    //Calculate the sections
    //orientation
    if (i>0)
    {
      //Get last sections orientation
      mUp = GetLastYMap();
	    dA=Twist/iSteps;
    }
    dAng=TMat.GetZRot(mUp);
	  dAng+=dA;
    OMat.MakeUnit();
    OMat.Rotate(0,0,dAng);
    TMat=TMat*OMat;
    TMat.Translate2(Loc.x,Loc.y,Loc.z);
    pAllSecs[iSecCnt] = (Section*) pBaseSec->Copy(this);
    pAllSecs[iSecCnt]->SetInPt(Loc);
    pAllSecs[iSecCnt]->Transform(TMat);

    iSecCnt++;
    w+=dInc;
	if (w>1)
	{w=1;}
  }
}
//DeleteSections();

#ifdef _DEBUG
   newMemState.Checkpoint();
   if( diffMemState.Difference( oldMemState, newMemState ) )
      {
        TRACE( "Memory leaked!\n" );
        diffMemState.DumpStatistics();
      }

#endif 
}
}

void Sweep::GenMesh(int iDim,PSHELL* pS,PBARL* pB)
{
int i;
int j;
int k;
Pt_Object* S1[500][100];
Pt_Object* E1[2000];
Pt_Object* pNd;
C3dVector Nd;
int iCnt1=0;
int iNlab=1;
int iPID;
int iSecT;
int iCol;
BOOL isClosed=FALSE;
Property* Prop;
E_Object* pE;
if (isVoid==FALSE)
{
if (pAllSecs[0]!= NULL)
{
  isClosed=pAllSecs[0]->isClosedSec();
  if (iDim==1) 
  {
	  Prop=pB;
    iPID=pBaseSec->iPID2;
  }
  else
  {
    Prop=pS;
    iPID=pBaseSec->iPID;
  }
  iSecT=pBaseSec->iSec;
  iCol=pBaseSec->iColour;
}
if (this->pPath->getLen()>0.000001)
{
if (iDim==2)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);
  for (i=0;i<iSecCnt;i++)
  {
    iCnt1=0;
	for (j=0;j<pAllSecs[i]->GetNoLines();j++)
	{
    for(k=0;k<pAllSecs[i]->GetDivs(j);k++)
	  {
      Nd=pAllSecs[i]->GetPt(j,k);
      pNd=Mesh->AddNode(Nd,iNlab,0,0,11,0,0);
      iNlab++;
      S1[i][iCnt1]=pNd;
      iCnt1++;
      if ((j==pAllSecs[i]->GetNoLines()-1) &&
	       (k==pAllSecs[i]->GetDivs(j)-1))
      {
        if (isClosed==FALSE)
	      {
          Nd=pAllSecs[i]->GetPt(j,pAllSecs[i]->GetDivs(j));
          pNd=Mesh->AddNode(Nd,iNlab,0,0,11,0,0);
	        iNlab++;
          S1[i][iCnt1]=pNd;
          iCnt1++;
	      }
	      else
	      {
          S1[i][iCnt1]=S1[i][0];
          iCnt1++;
	      }
      }
	  }
	}
  }
  int iECnt=1;
  for(i=0;i<iSecCnt-1;i++)
  {
    for (j=0;j<iCnt1-1;j++)
    {
      E1[0]=S1[i+1][j];
      E1[1]=S1[i][j];
      E1[2]=S1[i][j+1];
      E1[3]=S1[i+1][j+1];
      pE= Mesh->AddEl(E1,iECnt,152,94,Prop->iID,1,4,0,0,1,FALSE,-1,0);
      pE->PIDunv=iPID;
      pE->pPr=Prop;
      iECnt++;
    }
  }
}
else if (iDim==1)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);
  for (i=0;i<iSecCnt;i++)
  {
    Nd=pAllSecs[i]->inPt;
	  pNd=Mesh->AddNode(Nd,i+1,0,0,11,0,0);
	  iNlab++;
	  S1[i][0]=pNd;
  }
  for(i=0;i<iSecCnt-1;i++)
  {
    E1[0]=S1[i][0];
	E1[1]=S1[i+1][0];
    pE= Mesh->AddEl(E1,i+1,152,21,Prop->iID,1,2,0,1,1,FALSE,-1,0);
    pE->pPr=Prop;
    pE->PIDunv=iPID;
	  E_Object2* pE1 =(E_Object2*) pE;
    C3dVector vv;
    vv=pAllSecs[i]->TMat.GetUpVec();
	  pE1->SetUpVec(vv);
    pE1->B=iSecT;
    pE1->C=iSecT;
  }
}
}
if (iDim==1)
{
  S1[0][0]->iColour=160;
  S1[iSecCnt-1][0]->iColour=160;
}
else if (iDim==2)
{
 for (j=0;j<iCnt1-1;j++)
 {
   S1[0][j]->iColour=160;
 }
 for (j=0;j<iCnt1-1;j++)
 {
   S1[iSecCnt-1][j]->iColour=160;
 }
}
}
}

ME_Object*  Sweep::GetMesh()
{
return (Mesh);
}

 C3dMatrix Sweep::GetLastYMap()
{
C3dMatrix mRet;
if (iSecCnt>0)
{
  mRet=pAllSecs[iSecCnt-1]->TMat;
}
return (mRet);
}

C3dMatrix Sweep::GetFirstYMap()
{
C3dMatrix mRet;
if (iSecCnt>0)
{
  mRet=pAllSecs[0]->TMat;
}
return (mRet);
}


IMPLEMENT_DYNAMIC(SweepF, CObject )

SweepF::SweepF()
{
Sweep::Sweep();
iObjType = 606;
pParent=NULL;
isStart=TRUE;
isEnd=TRUE;
}

void SweepF::PreCreate(int iLab,CString inName,double dT)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
Name=inName;
Twist=dT;
}

void SweepF::Generate(C3dMatrix mUp,double dElLen,double dLenFR)
{
C3dVector Loc;
C3dVector Dir;
C3dMatrix TMat;
C3dMatrix OMat;
dElLength=dElLen;
double dAng;
double dA=0;
dFR=dLenFR;
int i;
int k=0;
double w;
double dInc;
double dRem;
int iSteps;
double dSteps;
double dLen;
double dS=0;
double dF=1;
if (isVoid==FALSE)
{
if (dElLen<=0)
{
  dElLen=0.005;
}
if (dLenFR<=0)
{
  dLenFR=0.001;
}
if (pPath->iObjType == 7)
{  
  dLen=pPath->getLen();
  if (isStart==TRUE)
  {
    //dLen=dLen-dLenFR;
    dS=dLenFR/dLen;
  }
  if (isEnd==TRUE)
  {
    //dLen=dLen-dLenFR;
    dF=1-dLenFR/dLen;
  }
  dSteps=dLen/dElLen;
  iSteps=(int) dSteps;
  
  dRem=dSteps-iSteps;

  if (iSteps==0)
  {
	iSteps=1;    
  }
  if (dRem>0.5)
  {
	iSteps+=1;
  }

  dSteps=(double) iSteps;
  dInc=(dF-dS)/dSteps;
  
  if (isStart==TRUE)
  {
    Dir = pPath->GetDir(0);
    TMat=TMat.CalcTran(Dir);
    Loc = pPath->GetPt(0);
    dAng=TMat.GetZRot(mUp);
	  dAng+=dA;
    OMat.MakeUnit();
    OMat.Rotate(0,0,dAng);
    TMat=TMat*OMat;
    TMat.Translate2(Loc.x,Loc.y,Loc.z);
    pAllSecs[iSecCnt] = (Section*) pBaseSec->Copy(this);
    pAllSecs[iSecCnt]->ChangeCol(4);
    pAllSecs[iSecCnt]->SetInPt(Loc);
    pAllSecs[iSecCnt]->Transform(TMat);
    iSecCnt++;
  }

  w=dS;
  for (i=0;i<iSteps+1;i++)
  {
    Dir = pPath->GetDir(w);
    TMat=TMat.CalcTran(Dir);
    Loc = pPath->GetPt(w);
    //Calculate the sections
    //orientation
    if (i>0)
    {
      //Get last sections orientation
      mUp = GetLastYMap();
	    dA=Twist/iSteps;
    }
    dAng=TMat.GetZRot(mUp);
	  dAng+=dA;
    OMat.MakeUnit();
    OMat.Rotate(0,0,dAng);
    TMat=TMat*OMat;
    TMat.Translate2(Loc.x,Loc.y,Loc.z);
    pAllSecs[iSecCnt] = (Section*) pBaseSec->Copy(this);
    pAllSecs[iSecCnt]->ChangeCol(55);
    pAllSecs[iSecCnt]->SetInPt(Loc);
    pAllSecs[iSecCnt]->Transform(TMat);

    iSecCnt++;
    w+=dInc;
	  if (w>1)
	  {w=1;}
  }
}
if (isEnd==TRUE)
  {
    Dir = pPath->GetDir(1);
    TMat=TMat.CalcTran(Dir);
    Loc = pPath->GetPt(1);
    //Calculate the sections
    //orientation
    if (i>0)
    {
      //Get last sections orientation
      mUp = GetLastYMap();
	    dA=Twist/iSteps;
    }
    dAng=TMat.GetZRot(mUp);
	  dAng+=dA;
    OMat.MakeUnit();
    OMat.Rotate(0,0,dAng);
    TMat=TMat*OMat;
    TMat.Translate2(Loc.x,Loc.y,Loc.z);
    pAllSecs[iSecCnt] = (Section*) pBaseSec->Copy(this);
    pAllSecs[iSecCnt]->ChangeCol(4);
    pAllSecs[iSecCnt]->SetInPt(Loc);
    pAllSecs[iSecCnt]->Transform(TMat);
    iSecCnt++;
  }
}
}

void SweepF::CutBack(BOOL bEA,BOOL bEB,ME_Object* MeshA,ME_Object* MeshB)
{
double cBW;
double dDist;
C3dVector pCPt;
C3dVector p1;
C3dVector vDirO;
C3dVector vDirN;
Pt_Object* pCN1;
Pt_Object* pCN2;
C3dMatrix mUp = GetFirstYMap();
NLine* pL;
pL = (NLine*) pPath;
vDirO=pL->GetDir(0);
double dD = GetSecSpacing();
DeleteSections();
double dLen=pL->getLen();
cBW=0.75*dD/dLen;
if ((MeshA!=NULL) && (bEA==TRUE))
{
  //Cutback nodes in standaard parts definition
  pCN1=MeshA->GetNode(1);
  pCN2=MeshA->GetNode(2);
  if (pCN1!=NULL)
  {
    pCPt=pCN1->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[0]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  } 
  if (pCN2!=NULL)
  {
    pCPt=pCN2->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[0]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  }
}
else if ((MeshA==NULL) && (bEA==TRUE))
{
  p1=pL->GetPt(cBW);
  pL->cPts[0]->Pt_Point->Set(p1.x,p1.y,p1.z);
}


if ((MeshB!=NULL) && (bEB==TRUE))
{
  //Cutback nodes in standaard parts definition
  pCN1=MeshB->GetNode(1);
  pCN2=MeshB->GetNode(2);
  if (pCN1!=NULL)
  {
    pCPt=pCN1->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[1]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  } 
  if (pCN2!=NULL)
  {
    pCPt=pCN2->GetCoords();
	  dDist=pL->MinDist(pCPt);
	  if (dDist<0.001)
	  {
      pL->cPts[1]->Pt_Point->Set(pCPt.x,pCPt.y,pCPt.z);
	  }
  }
}
else if ((MeshB==NULL) && (bEB==TRUE))
{
  p1=pL->GetPt(1-cBW);
  pL->cPts[1]->Pt_Point->Set(p1.x,p1.y,p1.z);
}
vDirN=pL->GetDir(0);
double dDot;
dDot=vDirO.Dot(vDirN);
if (dDot<0)
{
  isVoid=TRUE;
}
Generate(mUp,dElLength,dFR);
}

BOOL isCorner(Pt_Object* P1,Pt_Object* P2,Pt_Object* P3,Pt_Object* P4)
{
BOOL brc = FALSE;
if ((P1->iColour==3) &&
    (P2->iColour==3) &&
    (P3->iColour==3) &&
    (P4->iColour==3))
{
  brc=TRUE;
}

return(brc);
}


void SweepF::GenMesh(int iDim,PSHELL* pS1,PSHELL* pS2
                             ,PSHELL* pS3,PSHELL* pS4
                             ,PBARL* pB1,PBARL* pB2)
{
int i;
int j;
int k;
Pt_Object* S1[500][100];
Pt_Object* E1[2000];
Pt_Object* pNd;
C3dVector Nd;
int iCnt1=0;
int iNlab=1;
int iPID;


BOOL isClosed=FALSE;
Property* Prop;
E_Object* pE;
if (isVoid==FALSE)
{
if (pAllSecs[0]!= NULL)
{
  isClosed=pAllSecs[0]->isClosedSec();
}

if (iDim==2)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);

  for (i=0;i<iSecCnt;i++)
  {
  iCnt1=0;
	for (j=0;j<pAllSecs[i]->GetNoLines();j++)
	{
    int iNoD;
    iNoD=pAllSecs[i]->GetDivs(j);
    for(k=0;k<iNoD;k++)
	  {
      Nd=pAllSecs[i]->GetPt(j,k);
      if ((pAllSecs[i]->pLn[j]->iObjType==8) || (k==0))
      {
        pNd=Mesh->AddNode(Nd,iNlab,0,0,3,0,0);
      }
      else
      {
        pNd=Mesh->AddNode(Nd,iNlab,0,0,55,0,0);
      }
      iNlab++;
      S1[i][iCnt1]=pNd;
      iCnt1++;
      if ((j==pAllSecs[i]->GetNoLines()-1) &&
	       (k==pAllSecs[i]->GetDivs(j)-1))
      {
        if (isClosed==FALSE)
	      {
          Nd=pAllSecs[i]->GetPt(j,pAllSecs[i]->GetDivs(j));
          pNd=Mesh->AddNode(Nd,iNlab,0,0,11,0,0);
	        iNlab++;
          S1[i][iCnt1]=pNd;
          iCnt1++;
	      }
	      else
	      {
          S1[i][iCnt1]=S1[i][0];
          iCnt1++;
	      }
      }
	  }
	}
  }
  int iECnt=1;
  int iELCol;
  for(i=0;i<iSecCnt-1;i++)
  {

    for (j=0;j<iCnt1-1;j++)
    {
      E1[0]=S1[i+1][j];
      E1[1]=S1[i][j];
      E1[2]=S1[i][j+1];
      E1[3]=S1[i+1][j+1];
      
      if (((i==0) && (isStart==TRUE)) &&
          (isCorner(E1[0],E1[1],E1[2],E1[3])==FALSE))
      {
        iELCol=153;
        Prop=pS1;
        iPID=pS1->iID;
      }
      else if (((i==iSecCnt-2) && (isEnd==TRUE)) &&
              (isCorner(E1[0],E1[1],E1[2],E1[3])==FALSE))
      {
        iELCol=153;
        Prop=pS1;
        iPID=pS1->iID;
      }
      else if (((i==0) && (isStart==TRUE)) &&
          (isCorner(E1[0],E1[1],E1[2],E1[3])==TRUE))
      {
        iELCol=162;
        Prop=pS2;
        iPID=pS2->iID;
      }
      else if (((i==iSecCnt-2) && (isEnd==TRUE)) &&
          (isCorner(E1[0],E1[1],E1[2],E1[3])==TRUE))
      {
        iELCol=162;
        Prop=pS2;
        iPID=pS2->iID;
      }
      else if (isCorner(E1[0],E1[1],E1[2],E1[3])==TRUE)
      {
         iELCol=3;
         Prop=pS4;
         iPID=pS4->iID;
      }
      else
      {
         iELCol=55;
         Prop=pS3;
         iPID=pS3->iID;
      }
      pE= Mesh->AddEl(E1,iECnt,iELCol,94,iPID,1,4,0,0,1,FALSE,-1,0);
      pE->PIDunv=iPID;
      pE->pPr=Prop;
      iECnt++;
    }
  }
}
else if (iDim==1)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);
  for (i=0;i<iSecCnt;i++)
  {
    Nd=pAllSecs[i]->inPt;
	  pNd=Mesh->AddNode(Nd,i+1,0,0,11,0,0);
	  iNlab++;
	  S1[i][0]=pNd;
  }
    int iELCol;
  for(i=0;i<iSecCnt-1;i++)
  {
      E1[0]=S1[i][0];
	  E1[1]=S1[i+1][0];
      if ((i==0) && (isStart==TRUE))
      {
        iELCol=153;
        Prop=pB1;
        iPID=pB1->iID;
      }
      else if ((i==iSecCnt-2) && (isEnd==TRUE)) 
      {
        iELCol=153;
        Prop=pB1;
        iPID=pB1->iID;
      }
      else
      {
        iELCol=55;
        Prop=pB2;
        iPID=pB2->iID;
      }
    pE= Mesh->AddEl(E1,i+1,iELCol,21,iPID,1,2,0,1,1,FALSE,-1,0);
    pE->pPr=Prop;
    pE->PIDunv=iPID;
	E_Object2* pE1 =(E_Object2*) pE;
    C3dVector vv;
    vv=pAllSecs[i]->TMat.GetUpVec();
    if (vv.Mag()==0)
    {
      int ff=0;
      ff++;
    }
	  pE1->SetUpVec(vv);
  }
}
}
  for (i=0;i<iSecCnt;i++)
  {
    Nd=pAllSecs[i]->inPt;
	  pNd=Mesh->AddNode(Nd,i+1,0,0,11,0,0);
	  iNlab++;
	  S1[i][0]=pNd;
  }
if (iDim==1)
{
  S1[0][0]->iColour=160;
  S1[iSecCnt-1][0]->iColour=160;
}
else if (iDim==2)
{
 for (j=0;j<iCnt1-1;j++)
 {
   S1[0][j]->iColour=160;
 }
 for (j=0;j<iCnt1-1;j++)
 {
   S1[iSecCnt-1][j]->iColour=160;
 }
}
}


IMPLEMENT_DYNAMIC(SweepFB, CObject )

SweepFB::SweepFB()
{
SweepB::SweepB();
iObjType = 607;
pParent=NULL;
}

void SweepFB::PreCreate(int iLab,CString inName,
					   C3dVector inP,double dR,int EH,double dT)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
Name=inName;
dRad=dR;
Twist=dT;
if (inPt!=NULL)
  {delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,11,this);
isHBend=EH;
}

void SweepFB::GenMesh(int iDim,PSHELL* pS1,PSHELL* pS2
                             ,PSHELL* pS3,PSHELL* pS4
                             ,PBARL* pB1,PBARL* pB2)
{
int i;
int j;
int k;
Pt_Object* S1[500][100];
Pt_Object* E1[2000];
Pt_Object* pNd;
C3dVector Nd;
int iCnt1=0;
int iNlab=1;
int iPID;


BOOL isClosed=FALSE;
Property* Prop;
E_Object* pE;
if (isVoid==FALSE)
{
if (pAllSecs[0]!= NULL)
{
  isClosed=pAllSecs[0]->isClosedSec();
}

if (iDim==2)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);

  for (i=0;i<iSecCnt;i++)
  {
  iCnt1=0;
	for (j=0;j<pAllSecs[i]->GetNoLines();j++)
	{
    int iNoD;
    iNoD=pAllSecs[i]->GetDivs(j);
    for(k=0;k<iNoD;k++)
	  {
      Nd=pAllSecs[i]->GetPt(j,k);
      if ((pAllSecs[i]->pLn[j]->iObjType==8) || (k==0))
      {
        pNd=Mesh->AddNode(Nd,iNlab,0,0,3,0,0);
      }
      else
      {
        pNd=Mesh->AddNode(Nd,iNlab,0,0,55,0,0);
      }
      iNlab++;
      S1[i][iCnt1]=pNd;
      iCnt1++;
      if ((j==pAllSecs[i]->GetNoLines()-1) &&
	       (k==pAllSecs[i]->GetDivs(j)-1))
      {
        if (isClosed==FALSE)
	      {
          Nd=pAllSecs[i]->GetPt(j,pAllSecs[i]->GetDivs(j));
          pNd=Mesh->AddNode(Nd,iNlab,0,0,11,0,0);
	        iNlab++;
          S1[i][iCnt1]=pNd;
          iCnt1++;
	      }
	      else
	      {
          S1[i][iCnt1]=S1[i][0];
          iCnt1++;
	      }
      }
	  }
	}
  }
  int iECnt=1;
  int iELCol;
  for(i=0;i<iSecCnt-1;i++)
  {

    for (j=0;j<iCnt1-1;j++)
    {
      
      E1[0]=S1[i+1][j];
      E1[1]=S1[i][j];
      E1[2]=S1[i][j+1];
      E1[3]=S1[i+1][j+1];
      if (isCorner(E1[0],E1[1],E1[2],E1[3])==TRUE)
      {
         iELCol=3;
         Prop=pS4;
         iPID=pS4->iID;
      }
      else
      {
         iELCol=55;
         Prop=pS3;
         iPID=pS3->iID;
      }
      pE= Mesh->AddEl(E1,iECnt,iELCol,94,iPID,1,4,0,0,1,FALSE,-1,0);
      pE->PIDunv=iPID;
      pE->pPr=Prop;
      iECnt++;
    }
  }
}
else if (iDim==1)
{
  Mesh=new ME_Object();
  Mesh->Create("",this,iMeshCnt);
  for (i=0;i<iSecCnt;i++)
  {
    Nd=pAllSecs[i]->inPt;
	  pNd=Mesh->AddNode(Nd,i+1,0,0,11,0,0);
	  iNlab++;
	  S1[i][0]=pNd;
  }
    int iELCol;
  for(i=0;i<iSecCnt-1;i++)
  {
    E1[0]=S1[i][0];
	  E1[1]=S1[i+1][0];

        iELCol=55;
        Prop=pB2;
        iPID=pB2->iID;

    pE= Mesh->AddEl(E1,i+1,iELCol,21,iPID,1,2,0,1,1,FALSE,-1,0);
    pE->pPr=Prop;
    pE->PIDunv=iPID;
	  E_Object2* pE1 =(E_Object2*) pE;
    C3dVector vv;
    vv=pAllSecs[i]->TMat.GetUpVec();
	  pE1->SetUpVec(vv);
  }
}
}
if (iDim==1)
{
  S1[0][0]->iColour=160;
  S1[iSecCnt-1][0]->iColour=160;
}
else if (iDim==2)
{
 for (j=0;j<iCnt1-1;j++)
 {
   S1[0][j]->iColour=160;
 }
 for (j=0;j<iCnt1-1;j++)
 {
   S1[iSecCnt-1][j]->iColour=160;
 }
}
}


IMPLEMENT_DYNAMIC(SweepB, CObject )

SweepB::SweepB()
{
Sweep::Sweep();
iObjType = 604;
dRad=0;
isHBend=0;
inPt=NULL;
pParent=NULL;
}

void SweepB::Create(int iLab,NCurve* inPath,
                   Section* inSec,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
pPath=(NCurve*) inPath->Copy(this); 
pBaseSec=(Section*) inSec->Copy(this);
pPath->pParent =this;
pBaseSec->pParent =this;
pParent=Parrent;
}

void SweepB::PreCreate(int iLab,CString inName,
					   C3dVector inP,double dR,int EH,double dT)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 604;
iLabel = iLab;
iColour = 2;
iSecCnt=0;
Name=inName;
dRad=dR;
Twist=dT;
if (inPt!=NULL)
  {delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,11,this);
isHBend=EH;
}

void SweepB::Serialize(CArchive& ar,int iV)
{
Sweep::Serialize(ar,iV);
if (ar.IsStoring())
  {
	ar<<dRad;
	ar<<isHBend;
	inPt->Serialize(ar,iV);
  } 
  else
  {
	ar>>dRad;
	ar>>isHBend;
	inPt=new CvPt_Object;
	inPt->Serialize(ar,iV);
  }
}
// Draw Object line
void SweepB::Draw(CDC* pDC,int iDrawmode)
{
Sweep::Draw(pDC,iDrawmode);
inPt->Draw(pDC,iDrawmode);
}


void SweepB::OglDraw(int iDspFlgs,double dS1,double dS2)
{
Sweep::OglDraw(iDspFlgs,dS1,dS2);
inPt->OglDraw(iDspFlgs,dS1,dS2);
}


void SweepB::Generate(C3dMatrix mUp,double dElLen)
{
C3dVector Loc;
C3dVector Dir;
C3dMatrix TMat;
C3dMatrix TMat2;
C3dMatrix OMat;
dElLength=dElLen;
double dAng;
double dS;
double dE;
double dInc;
NCurve* C=(NCurve*) pPath;
dS=C->ws;
dE=C->we;

int i;
double w;
int iSteps;
double dDegStep=dElLen; //10degrees
double dT;
double dRem;
dT=((dE-dS))*360/dDegStep;
iSteps=(int) dT;
dRem=dT-iSteps;
if (dRem>0)
{
  iSteps+=1;
}
if (iSteps==0)
{
  iSteps=1;
}
dInc = (dE-dS)/iSteps;
if (pPath->iObjType == 7) 
{  
  w=dS;
  for (i=0;i<iSteps+1;i++)
  {
    //Twist
    //TMat2.Rotate(0,0,5);
    Dir = pPath->GetDir(w);
    TMat=TMat.CalcTran(Dir);
    Loc = pPath->GetPt(w);
    //Calculate the sections
    //orientation
    if (i>0)
    {
      //Get last sections orientation
      mUp = GetLastYMap();
    }
    dAng=TMat.GetZRot(mUp);
    OMat.MakeUnit();
    OMat.Rotate(0,0,dAng);
    TMat=TMat*OMat;
    TMat.Translate2(Loc.x,Loc.y,Loc.z);
    pAllSecs[iSecCnt] = (Section*) pBaseSec->Copy(this);
    //pAllSecs[iSecCnt]->Transform(TMat2);
    //pAllSecs[iSecCnt]->Transform(OMat);
	  pAllSecs[iSecCnt]->SetInPt(Loc);
    pAllSecs[iSecCnt]->Transform(TMat);
    iSecCnt++;
    w+=dInc;
  }
}
}


void SweepB::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
Sweep::SetToScr(pModMat, pScrTran);
inPt->SetToScr(pModMat, pScrTran);
}

void SweepB::Transform(C3dMatrix inTMat)
{
Sweep::Transform(inTMat);
inPt->Transform(inTMat);
}


C3dVector SweepB::GetNVec()
{
C3dVector vN;
NCircle* C =(NCircle*) pPath;
vN.x=C->vNorm.x;
vN.y=C->vNorm.y;
vN.z=C->vNorm.z;
return(vN);
}

C3dVector SweepB::GetSVec()
{
C3dVector vN;
NCircle* C =(NCircle*) pPath;
vN=C->GetDir(0);
vN.Normalize();
return(vN);
}

C3dVector SweepB:: GetInPt()
{
C3dVector vR;
vR.x=inPt->Pt_Point->x;
vR.y=inPt->Pt_Point->y;
vR.z=inPt->Pt_Point->z;
return (vR);
}

 IMPLEMENT_DYNAMIC( PartsCat, CObject )




 //int iNo;
 //int iCurDsp;
 //ME_Object* P_Obj[100];
 
PartsCat::PartsCat(int iLab)
{
iLabel = iLab;
iNo = 0;
iCurDsp = -1;
}



PartsCat::~PartsCat()
{
int i;
for(i=0;i<iNo;i++)
{
  delete(P_Obj[i]);
}
}

void PartsCat::Create(int iLab)
{
iLabel = iLab;
iNo = 0;
iCurDsp = -1;
}




void PartsCat::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}


void PartsCat::Info()
{
  char S1[80];
  CString OutT;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",iLabel," Col; ",iColour);
  OutT+=S1;
  outtext1("PARTS CAT");
  outtext1(OutT); 
  outtext1("    ****");
}

void PartsCat::Draw(CDC* pDC,int iDrawmode)
{
if ((iCurDsp >= 0) && (iCurDsp<iNo))
{
  if (P_Obj[iCurDsp]!= NULL)
  {
    P_Obj[iCurDsp]->Draw(pDC,iDrawmode);
	pDC->TextOut(20,20,P_Obj[iCurDsp]->sName);
  }
}
}

void PartsCat::OglDraw(int iDspFlgs,double dS1,double dS2)
{
char sLab[200];
C3dVector pCent;
if ((iCurDsp >= 0) && (iCurDsp<iNo))
{
  if (P_Obj[iCurDsp]!= NULL)
  {
    sprintf_s(sLab,"%s",P_Obj[iCurDsp]->sName);
    pCent=P_Obj[iCurDsp]->Get_Centroid();
	  OglString(iDspFlgs,pCent.x,pCent.y,pCent.z,&sLab[0]);
    P_Obj[iCurDsp]->OglDraw(iDspFlgs,dS1,dS2);
  }
}
}

void PartsCat::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[200];
C3dVector pCent;
if ((iCurDsp >= 0) && (iCurDsp<iNo))
{
  if (P_Obj[iCurDsp]!= NULL)
  {
    sprintf_s(sLab,"%s",P_Obj[iCurDsp]->sName);
    pCent=P_Obj[iCurDsp]->Get_Centroid();
	  OglString(iDspFlgs,pCent.x,pCent.y,pCent.z,&sLab[0]);
    P_Obj[iCurDsp]->OglDrawW(iDspFlgs,dS1,dS2);
  }
}
}

void PartsCat::HighLight(CDC* pDC)
{
if ((iCurDsp >= 0) && (iCurDsp<iNo))
{
  if (P_Obj[iCurDsp]!= NULL)
  {
    P_Obj[iCurDsp]->HighLight(pDC);
  }
}
}

void PartsCat::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
if ((iCurDsp >= 0) && (iCurDsp<iNo))
{
  if (P_Obj[iCurDsp]!= NULL)
  {
    P_Obj[iCurDsp]->SetToScr(pModMat,pScrTran);
  }
}
}

void PartsCat::Next()
{
if (iCurDsp<iNo-1) 
{
  iCurDsp++;
}
}

void PartsCat::Previous()
{
if (iCurDsp>0) 
{
  iCurDsp--;
}
}


void PartsCat::Transform(C3dMatrix TMat)
{

}

G_ObjectD PartsCat::SelDist(CPoint InPT,Filter FIL)
{
double d;
G_Object* pO;
G_ObjectD Ret;
d=1e36;
pO=this;
pO->pParent=NULL;
Ret.Dist=d;
Ret.pObj=pO;
return (Ret);
}


void PartsCat::Add(ME_Object* inMesh)
{
if (iNo < MaxPrtNo)
{
  P_Obj[iNo]=inMesh;
  iNo++;
  iCurDsp=0;
}
}

ME_Object* PartsCat::Get(CString inName)
{
ME_Object* pRet;
pRet=NULL;
int i;
for (i=0;i<iNo;i++)
{
  if (P_Obj[i]->sName==inName)
  {
    pRet=(ME_Object*) P_Obj[i]->Copy(this);
    break;
  }
}

return (pRet);
}

ME_Object* PartsCat::GetCur()
{
ME_Object* pRet;
pRet=NULL;
if (iCurDsp!=-1)
{
  pRet=(ME_Object*) P_Obj[iCurDsp]->Copy(this);
  pRet->pParent=NULL;
}
return (pRet);
}


IMPLEMENT_DYNAMIC( pRecord, CObject )

 
pRecord::pRecord(int iIndex,int iType)
{
iT=iType;
iID=iIndex;
iCnt=0;
}

pRecord::~pRecord()
{
int i;
for(i=0;i<iCnt;i++)
{
  delete(propVals[i]);
}
}

void pRecord::Serialize(CArchive& ar,int iV)
{
int i;
if (ar.IsStoring())
{
  ar<<iID;
  ar<<iT;
  ar<<iCnt;
  for (i=0;i<iCnt;i++)
  {
   propVals[i]->Serialize(ar,iV);
  }    
}
else
{
  ar>>iID;
  ar>>iT;
  ar>>iCnt;
  for (i=0;i<iCnt;i++)
  {
	if (iT==1)
	{
      propVals[i]= new piVal;
	}
	else if (iT==2)
	{
      propVals[i]= new pdVal;
	}
	else if (iT==3)
	{
      propVals[i]= new psVal;
	}
    propVals[i]->Serialize(ar,iV);
  }  
}
}


void pRecord::Add(pVal* inP)
{
if (iCnt<50)
{
  propVals[iCnt]= inP;
  iCnt++;
}
}




IMPLEMENT_DYNAMIC( pVal, CObject )

void pVal::Serialize(CArchive& ar,int iV)
{
if (ar.IsStoring())
{
     
}
else
{
 
}
}


IMPLEMENT_DYNAMIC( piVal, CObject )

void piVal::Serialize(CArchive& ar,int iV)
{
if (ar.IsStoring())
{
    ar<<iVal; 
}
else
{
    ar>>iVal; 
}
}

IMPLEMENT_DYNAMIC( pdVal, CObject )

void pdVal::Serialize(CArchive& ar,int iV)
{
if (ar.IsStoring())
{
    ar<<dVal; 
}
else
{
     ar>>dVal; 
}
}


IMPLEMENT_DYNAMIC( psVal, CObject )

void psVal::Serialize(CArchive& ar,int iV)
{
if (ar.IsStoring())
{
    ar<<sVal; 
}
else
{
     ar>>sVal; 
}
}



IMPLEMENT_DYNAMIC( Prop, CObject )



Prop::Prop(int inD,int inT,
		int inN,int inP,CString inName)
{
iCnt=0;
iD=inD;
iT=inT;
iN=inN;
iP=inP;
Name=inName;
}

Prop::Prop()
{
iCnt=0;
iD=-1;
iT=-1;
iN=-1;
iP=-1;
Name="ROYBLOWS";
}

Prop::~Prop()
{
int i;
for (i=0;i<iCnt;i++)
{
  delete(pRecs[i]);
}
}


void Prop::Serialize(CArchive& ar,int iV)
{
int i;
if (ar.IsStoring())
{
	ar<<iCnt;
	ar<<iD;
	ar<<iT;
	ar<<iN;
	ar<<iP;
	ar<<Name;
}
else
{
	ar>>iCnt;
	ar>>iD;
	ar>>iT;
	ar>>iN;
	ar>>iP;
	ar>>Name;
}
for (i=0;i<iCnt;i++)
{
  pRecs[i]->Serialize(ar,iV);
}
}


void Prop::Add(pRecord* inRec)
{
if (iCnt<MaxRec)
{
  pRecs[iCnt]=inRec;
  iCnt++;
}
}



IMPLEMENT_DYNAMIC(Entity , CObject )

Entity::Entity()
{
sTitle="";
iID = -1;
iType= -1;
}

void Entity::Create(CString inTitle,
                      int inPid,
                      int inType)
{
sTitle=inTitle;
iID = inPid;
iType= inType;
}

void Entity::List()
{
  char S1[200];
  CString OutT;
  sprintf_s(S1,"%s %i %s %i","ID",iID,"TYPE",iType);
  OutT=S1;
  outtext1(OutT); 
}

void Entity::ListShort()
{
  char S1[200];
  sprintf_s(S1,"%s %i %s %i  %s","ID",iID,"TYPE",iType,this->sTitle);
  outtext1(_T(S1)); 
}

void Entity::ExportNAS(FILE* pFile)
{
}

Entity* Entity::Copy()
{
Entity* pREt=new Entity();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
return (pREt);
}


void Entity::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar<<iType;
    ar<<sTitle;
    ar<<iID;
  }
  else
  {
    ar>>iType;
    ar>>sTitle;
    ar>>iID;
  }
}

int Entity::GetVarHeaders(CString sVar[])
{
  sVar[0]="Hello";
  sVar[1] = "Hello2";
  return(2);
}


int Entity::GetVarValues(CString sVar[])
{

  sVar[0] = "Hello";
  sVar[1] = "Hello2";
  return(2);
}


void Entity::PutVarValues(int iNo, CString sVar[])
{

}




IMPLEMENT_DYNAMIC(Property , CObject )

BOOL Property::HasMat(int inMID)
{
return (FALSE);
}

int Property::GetMat()
{
return (-1);
}

double Property::GetThk()
{
return(0);
}

BSec* Property::GetSec()
{
return(NULL);
}

void Property::UpdateMats(NEList* newMats)
{
}

int Property::GetDefMatID()
{
  return (-1);
}

void Property::ChangeMat(int thisMat,int inMID)
{
}

// MAT
IMPLEMENT_DYNAMIC(Material , CObject )

void Material::Info()
{
}

double Material::GetDensity()
{
  return(1.0);
}

double Material::GetCTE()
{
  return(0.0);
}

double Material::GetE()
{
  return(0.0);
}

double Material::GetV()
{
  return(0.0);
}

IMPLEMENT_DYNAMIC(PMASS, CObject)

PMASS::PMASS()
{
  sTitle = "";
  iID = -1;
  iType = 161;
  dM = 0;
}

PMASS* PMASS::Copy()
{
  PMASS* pREt = new PMASS();
  pREt->iID = iID;
  pREt->sTitle = sTitle;
  pREt->iType = iType;
  pREt->dM = dM;
  return (pREt);
}

void PMASS::Serialize(CArchive& ar, int iV)
{
  if (ar.IsStoring())
  {
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << dM;
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> dM;
  }
}


void PMASS::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1, "%s %i %s %s", "PID", iID, "TYPE ", "LUMPED MASS");
  OutT = S1;
  outtext1(OutT);
  sprintf_s(S1, "%s %s", "TITLE : ", sTitle);
  OutT = S1;
  outtext1(OutT);
  sprintf_s(S1, "%s %f", "M    : ", dM);
  OutT = S1;
  outtext1(OutT);
}

int PMASS::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Point Mass (M)";
  return(1);
}


int PMASS::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%g", dM);
  sVar[iNo] = S1;
  iNo++;
  return (iNo);
}

void PMASS::PutVarValues(int iNo, CString sVar[])
{
  dM = atof(sVar[0]);
}

IMPLEMENT_DYNAMIC(PSPRINGT , CObject)

PSPRINGT::PSPRINGT()
{
  sTitle="";
  iID = -1;
  iType= 136;
  dkx=1000;
  dky=1000;
  dkz=1000;
  dkcoeff=0;
}

PSPRINGT* PSPRINGT::Copy()
{
  PSPRINGT* pREt=new PSPRINGT();
  pREt->iID=iID;
  pREt->sTitle=sTitle;
  pREt->iType=iType;
  pREt->dkx=dkx;
  pREt->dky=dky;
  pREt->dkz=dkz;
  pREt->dkcoeff=dkcoeff;
return (pREt);
}

void PSPRINGT::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << dkx;
    ar << dky;
    ar << dkz;
    ar << dkcoeff;
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> dkx;
    ar >> dky;
    ar >> dkz;
    ar >> dkcoeff;
  }
}


void PSPRINGT::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","TRANSLATION SPRING");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kx    : ",dkx);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","ky    : ",dky);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kz    : ",dkz);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kCoeff: ",dkcoeff);
  OutT=S1;
  outtext1(OutT); 
}

int PSPRINGT::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Translational Stiffness in X (Kx)";
  sVar[1] = "Translational Stiffness in Y (Ky)";
  sVar[2] = "Translational Stiffness in Z (Kz)";
  sVar[3] = "Conduction Coefficeint (kcoeff)";
  return(4);
}


int PSPRINGT::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%g", dkx);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dky);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dkz);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dkcoeff);
  sVar[iNo] = S1;
  iNo++;
  return (iNo);
}

void PSPRINGT::PutVarValues(int iNo, CString sVar[])
{
  dkx = atof(sVar[0]);
  dky = atof(sVar[1]);
  dkz = atof(sVar[2]);
  dkcoeff = atof(sVar[3]);
}

void PSPRINGT::ExportNAS(FILE* pFile)
{
	fprintf(pFile, "$%s\n", sTitle);
	fprintf(pFile, "%8s%8i%8s%8s%8s%8s\n", "PBUSH   ", iID, "       K", e8(dkx), e8(dky), e8(dkz));
}

IMPLEMENT_DYNAMIC(PSPRINGR , CObject)

PSPRINGR::PSPRINGR()
{
  sTitle="";
  iID = -1;
  iType= 137;
  dkx=1000;
  dky=1000;
  dkz=1000;
  dkcoeff=0;
}

void PSPRINGR::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","ROTATION SPRING");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kx    : ",dkx);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","ky    : ",dky);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kz    : ",dkz);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","kCoeff: ",dkcoeff);
  OutT=S1;
  outtext1(OutT); 
}

int PSPRINGR::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Rotational Stiffness in X (Rx)";
  sVar[1] = "Rotational Stiffness in Y (Ry)";
  sVar[2] = "Rotational Stiffness in Z (Rz)";
  sVar[3] = "Conduction Coefficeint (kcoeff)";
  return(4);
}

void PSPRINGR::ExportNAS(FILE* pFile)
{
	fprintf(pFile, "$%s\n", sTitle);
	fprintf(pFile, "%8s%8i%8s%8s%8s%8s%8s%8s%8s\n", "PBUSH   ", iID, "       K","        ","       K","       K", e8(dkx), e8(dky), e8(dkz));
}

IMPLEMENT_DYNAMIC(PSOLID , CObject)

PSOLID::PSOLID()
{
sTitle="";
iID = -1;
iType= 3;
iMID=-1;
iCORDM=-1;
sIN="TWO     ";
sSTRESS="GRID    ";
sISOP="REDUCED ";
sFCTN="        ";
}

void PSOLID::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << iMID;
    ar << iCORDM;
    ar << sIN;
    ar << sSTRESS;
    ar << sISOP;
    ar << sFCTN;
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> iMID;
    ar >> iCORDM;
    ar >> sIN;
    ar >> sSTRESS;
    ar >> sISOP;
    ar >> sFCTN;
  }
}


void PSOLID::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","SOLID");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %i","MAT1  : ",iMID);
  outtext1(_T(S1));
  sprintf_s(S1,"%s %i","CSYS  : ",iCORDM);
  outtext1(_T(S1));
}


int PSOLID::GetMat()
{
return (iMID);
}

BOOL PSOLID::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}

void PSOLID::UpdateMats(NEList* newMats)
{
 iMID=newMats->Get(iMID);
}

void PSOLID::ChangeMat(int thisMat,int inMID)
{
if (iMID==thisMat)
 iMID=inMID;
}

int PSOLID::GetDefMatID()
{
return (iMID);
}

void PSOLID::ExportNAS(FILE* pFile)
{
fprintf(pFile,"$%s\n",sTitle);
fprintf(pFile,"%8s%8i%8i%8i%8s%8s%8s%8s\n","PSOLID  ",iID,iMID,iCORDM,sIN,sSTRESS,sISOP,sFCTN);
}

PSOLID* PSOLID::Copy()
{
PSOLID* pREt=new PSOLID();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID=iMID;
pREt->iCORDM=iCORDM;
pREt->sIN=sIN;
pREt->sSTRESS=sSTRESS;
pREt->sISOP=sISOP;
pREt->sFCTN=sFCTN;
return (pREt);
}

int PSOLID::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Material ID (MID)";
  sVar[1] = "Material coord system (MCID) ";
  return(3);
}


int PSOLID::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%i", iMID);
  sVar[0] = S1;
  iNo++;
  sprintf_s(S1, "%i", iCORDM);
  sVar[1] = S1;
  iNo++;
  return (iNo);
}


void PSOLID::PutVarValues(int iNo, CString sVar[])
{
  iMID = atoi(sVar[0]);
  iCORDM = atoi(sVar[1]);
}

IMPLEMENT_DYNAMIC(PBAR , CObject)

PBAR::PBAR()
{
sTitle="";
iID = -1;
iType= 4;
iMID=-1;
dA=0;
dI1=0;
dI2=0;
dJ=0;
dNSM=0;
dC1=0;
dC2=0;
dD1=0;
dD2=0;
dE1=0;
dE2=0;
dF1=0;
dF2=0;
dK1=0;
dK2=0;
dI12=0;
DspSec.Clear();
}

void PBAR::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << iMID;

    ar << dA;
    ar << dI1;
    ar << dI2;
    ar << dJ;
    ar << dNSM;
    ar << dC1;
    ar << dC2;
    ar << dD1;
    ar << dD2;
    ar << dE1;
    ar << dE2;
    ar << dF1;
    ar << dF2;
    ar << dK1;
    ar << dK2;
    ar << dI12;
    DspSec.Serialize(ar,iV);
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> iMID;

    ar >> dA;
    ar >> dI1;
    ar >> dI2;
    ar >> dJ;
    ar >> dNSM;
    ar >> dC1;
    ar >> dC2;
    ar >> dD1;
    ar >> dD2;
    ar >> dE1;
    ar >> dE2;
    ar >> dF1;
    ar >> dF2;
    ar >> dK1;
    ar >> dK2;
    ar >> dI12;
    DspSec.Serialize(ar,iV);
  }
}

void PBAR::UpdateMats(NEList* newMats)
{
iMID=newMats->Get(iMID);
}

void PBAR::ChangeMat(int thisMat,int inMID)
{
if (iMID==thisMat)
  iMID=inMID;
}

int PBAR::GetVarHeaders(CString sVar[])
{
	sVar[0] = "Material ID (MID)";
	sVar[1] = "Area (A)";
	sVar[2] = "Second Moment of Inertia (I1 (Izz))";
	sVar[3] = "Second Moment of Inertia (I2 (Iyy))";
	sVar[4] = "Torsional Constant (J)";
	sVar[5] = "Non Strut Mass";
	return(6);
}


int PBAR::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%i", iMID);
	sVar[0] = S1;
	iNo++;
	sprintf_s(S1, "%g", dA);
	sVar[1] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI1);
	sVar[2] = S1;
	iNo++;
	sprintf_s(S1, "%g", dI2);
	sVar[3] = S1;
	iNo++;
	sprintf_s(S1, "%g", dJ);
	sVar[4] = S1;
	iNo++;
	sprintf_s(S1, "%g", dNSM);
	sVar[5] = S1;
	iNo++;
	return (iNo);
}

void PBAR::PutVarValues(int iNo, CString sVar[])
{
	iMID = atoi(sVar[0]);
	dA = atof(sVar[1]);
	dI1 = atof(sVar[2]);
	dI2 = atof(sVar[3]);
	dJ = atof(sVar[4]);
	dNSM = atof(sVar[5]);
	CreateSec();
}

void PBAR::CreateSec()
{
double dSize=0.01;
double dH;
double dW;
if ((dA > 0) && (dI1>0) && (dI2>0))
{
	if (dI1 == dI2) //Assume circular for display
	{
		dSize = sqrt(dA);
		DspSec.CreateDefSec(dSize / 2, 55);
	}
	else  //Assume rectangle for display
	{
		//dH = sqrt(12 * dI1 / dA);
		//dW = dA / dH;
		dW = pow((144 * dI1*dI1*dI1 / dI2), 0.125);
		dH = pow((144 * dI2*dI2*dI2 / dI1), 0.125);
		DspSec.CreateBar(dW, dH);
	}
}
else
{
	DspSec.CreateDefSec(dSize / 2, 55);
}
}


int PBAR::GetMat()
{
  return (iMID);
}

int PBAR::GetDefMatID()
{
  return (iMID);
}


BOOL PBAR::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}

void PBAR::ExportNAS(FILE* pFile)
{
fprintf(pFile,"$%s\n",sTitle);
fprintf(pFile,"%-8s%8i%8i%8s%8s%8s%8s%8s\n","PBAR    ",iID,iMID,e8(dA),e8(dI1),e8(dI2),e8(dJ),e8(dNSM));
fprintf(pFile,"%-8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(dC1),e8(dC2),e8(dD1),e8(dD2),e8(dE1),e8(dE2),e8(dF1),e8(dF2));
fprintf(pFile,"%-8s%8s%8s%8s\n","        ",e8(dK1),e8(dK2),e8(dI12));

}

PBAR* PBAR::Copy()
{
int  i;
PBAR* pREt=new PBAR();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID=iMID;

pREt->dA=dA;
pREt->dI1=dI1;
pREt->dI2=dI2;
pREt->dJ=dJ;
pREt->dNSM=dNSM;
pREt->dC1=dC1;
pREt->dC2=dC2;
pREt->dD1=dD1;
pREt->dD2=dD2;
pREt->dE1=dE1;
pREt->dE2=dE2;
pREt->dF1=dF1;
pREt->dF2=dF2;
pREt->dK1=dK1;
pREt->dK2=dK2;
pREt->dI12=dI12;
pREt->DspSec.iLnCnt1=DspSec.iLnCnt1;
pREt->DspSec.iLnCnt2=DspSec.iLnCnt2;
for (i=0;i<DspSec.iLnCnt1;i++)
{
  pREt->DspSec.pLnLoop1[i] = DspSec.pLnLoop1[i];
}
for (i=0;i<DspSec.iLnCnt2;i++)
{
  pREt->DspSec.pLnLoop2[i] = DspSec.pLnLoop2[i];
}
return (pREt);
}

BSec* PBAR::GetSec()
{
return (&DspSec);
}


IMPLEMENT_DYNAMIC(PBEAM , CObject)

PBEAM::PBEAM()
{
sTitle="";
iID = -1;
iType= 6;
iMID=-1;

iNo=1;
A[0]=0.0001;

K1=DBL_MAX;
K2=DBL_MAX;
S1=DBL_MAX;
S2=DBL_MAX;
NSIA=DBL_MAX;
NSIB=DBL_MAX;
CWA=DBL_MAX;
CWB=DBL_MAX;
M1A=DBL_MAX;
M2A=DBL_MAX;
M1B=DBL_MAX;
M2B=DBL_MAX;
N1A=DBL_MAX;
N2A=DBL_MAX;
N1B=DBL_MAX;
N2B=DBL_MAX;
DspSec.Clear();
}

void PBEAM::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << iMID;
    ar << iNo;
    for (i=0;i<iNo;i++)
    {
      ar << A[i];
      ar << I1[i];
      ar << I2[i];
      ar << I12[i];
      ar << J[i];
      ar << NSM[i];
      ar << C1[i];
      ar << C2[i];
      ar << D1[i];
      ar << D2[i];
      ar << E1[i];
      ar << E2[i];
      ar << F1[i];
      ar << F2[i];
      ar << SO[i];
      ar << XXB[i];
    }
    ar << K1;
    ar << K2; 
    ar << S1; 
    ar << S2; 
    ar << NSIA;
    ar << NSIB;
    ar << CWA;
    ar << CWB;
    ar << M1A;
    ar << M2A;
    ar << M1B;
    ar << M2B;
    ar << N1A;
    ar << N2A;
    ar << N1B;
    ar << N2B;
    DspSec.Serialize(ar,iV);
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> iMID;
    ar >> iNo;
    for (i=0;i<iNo;i++)
    {
      ar >> A[i];
      ar >> I1[i];
      ar >> I2[i];
      ar >> I12[i];
      ar >> J[i];
      ar >> NSM[i];
      ar >> C1[i];
      ar >> C2[i];
      ar >> D1[i];
      ar >> D2[i];
      ar >> E1[i];
      ar >> E2[i];
      ar >> F1[i];
      ar >> F2[i];
      ar >> SO[i];
      ar >> XXB[i];
    }
    ar >> K1;
    ar >> K2; 
    ar >> S1; 
    ar >> S2; 
    ar >> NSIA;
    ar >> NSIB;
    ar >> CWA;
    ar >> CWB;
    ar >> M1A;
    ar >> M2A;
    ar >> M1B;
    ar >> M2B;
    ar >> N1A;
    ar >> N2A;
    ar >> N1B;
    ar >> N2B;
    DspSec.Serialize(ar,iV);
  }
}

void PBEAM::UpdateMats(NEList* newMats)
{
iMID=newMats->Get(iMID);
}

void PBEAM::ChangeMat(int thisMat,int inMID)
{
if (iMID==thisMat)
  iMID=inMID;
}

int PBEAM::GetMat()
{
return (iMID);
}

void PBEAM::CreateSec()
{
double dSize;
if (A[0]>0)
{
  dSize=sqrt(A[0]);
}
else
{
  dSize=0.01;
}
DspSec.CreateDefSec(dSize/2,55);
}

BOOL PBEAM::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}

void PBEAM::ExportNAS(FILE* pFile)
{
int i;
fprintf(pFile,"$%s\n",sTitle);
fprintf(pFile,"%8s%8i%8i%8s%8s%8s%8s%8s%8s\n","PBEAM   ",iID,iMID,e8(A[0]),e8(I1[0]),e8(I2[0]),e8(I12[0]),e8(J[0]),e8(NSM[0]));
fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(C1[0]),e8(C2[0]),e8(D1[0]),e8(D2[0]),e8(E1[0]),e8(E2[0]),e8(F1[0]),e8(F2[0]));
for (i=1;i<iNo;i++)
{
  fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",SO[i],e8(XXB[i]),e8(A[i]),e8(I1[i]),e8(I2[i]),e8(I12[i]),e8(J[i]),e8(NSM[i]));
  if ((SO[i].Find("YESA")>-1) || (SO[i].Find("NO")>-1))
  {
  }
  else
  {
    fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(C1[i]),e8(C2[i]),e8(D1[i]),e8(D2[i]),e8(E1[i]),e8(E2[i]),e8(F1[i]),e8(F2[i]));
  }
}

fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(K1),e8(K2),e8(S1),e8(S2),e8(NSIA),e8(NSIB),e8(CWA),e8(CWB));
fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(M1A),e8(M2A),e8(M1B),e8(M2B),e8(N1A),e8(N2A),e8(N1B),e8(N2B));


//fprintf(pFile,"%-8s%-8s%-8s%-8s\n","        ",e8(dK1),e8(dK2),e8(dI12));

}

PBEAM* PBEAM::Copy()
{
int  i;
PBEAM* pREt=new PBEAM();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID=iMID;

for (i=0;i<9;i++)
{
  pREt->A[i]=A[i];
  pREt->I1[i]=I1[i];
  pREt->I2[i]=I2[i];
  pREt->I12[i]=I12[i];
  pREt->J[i]=J[i];
  pREt->NSM[i]=NSM[i];
  pREt->C1[i]=C1[i];
  pREt->C2[i]=C2[i];
  pREt->D1[i]=D1[i];
  pREt->D2[i]=D2[i];
  pREt->E1[i]=E1[i];
  pREt->E2[i]=E2[i];
  pREt->F1[i]=F1[i];
  pREt->F2[i]=F2[i];
  pREt->SO[i]=SO[i];
  pREt->XXB[i]=XXB[i];
}
 pREt->K1=K1;
 pREt->K2=K2; 
 pREt->S1=S1; 
 pREt->S2=S2; 
 pREt->NSIA=NSIA;
 pREt->NSIB=NSIB;
 pREt->CWA=CWA;
 pREt->CWB=CWB;
 pREt->M1A=M1A;
 pREt->M2A=M2A;
 pREt->M1B=M1B;
 pREt->M2B=M2B;
 pREt->N1A=N1A;
 pREt->N2A=N2A;
 pREt->N1B=N1B;
 pREt->N2B=N2B;
pREt->DspSec.iLnCnt1=DspSec.iLnCnt1;
pREt->DspSec.iLnCnt2=DspSec.iLnCnt2;
for (i=0;i<DspSec.iLnCnt1;i++)
{
  pREt->DspSec.pLnLoop1[i] = DspSec.pLnLoop1[i];
}
for (i=0;i<DspSec.iLnCnt2;i++)
{
  pREt->DspSec.pLnLoop2[i] = DspSec.pLnLoop2[i];
}
return (pREt);
}

BSec* PBEAM::GetSec()
{
return (&DspSec);
}



IMPLEMENT_DYNAMIC(PBARL , CObject)

PBARL::PBARL()
{
sTitle="";
iID = -1;
iType= 5;
iMID=-1;
iNoDims=0;
sSecType="";
DspSec.Clear();
sGROUP="MSCBML0";
dNSM=0;
A=0;
Izz=0;
Iyy=0;
J=0;
}

int PBARL::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	int i;
	char S1[8] = "";
	sVar[iNo] = "Material ID (MID)";
	iNo++;
	sVar[iNo] = "Section";
	iNo++;
	for (i = 0; i < iNoDims; i++)
	{
		sprintf_s(S1, "DIM %i", i+1);
		sVar[iNo] = S1;
		iNo++;
	}
	sVar[iNo] = "Non Strut Mass";
	iNo++;

	return(iNo);
}


int PBARL::GetVarValues(CString sVar[])
{
	int iNo = 0;
	int i;
	char S1[80] = "";
	sprintf_s(S1, "%i", iMID);
	sVar[iNo] = S1;
	iNo++;
	sVar[iNo] = sSecType;
	iNo++;
	for (i = 0; i < iNoDims; i++)
	{
		sprintf_s(S1, "%g", dDIMs[i]);
		sVar[iNo] = S1;
		iNo++;
	}
	sprintf_s(S1, "%g", dNSM);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}


void PBARL::PutVarValues(int iNo, CString sVar[])
{
	int iC = 0;
	int i;
	iMID = atoi(sVar[iC]);
	iC++;
	iC++;
	for (i = 0; i < iNoDims; i++)
	{
		dDIMs[i] = atof(sVar[iC]);
		iC++;
	}
	iC++;
	dNSM = atof(sVar[iC]);
	CreateSec();
}



void PBARL::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << iMID;
    ar << sGROUP;
    ar << sSecType;
    ar << iNoDims;
    ar << dNSM;
    for (i=0;i<iNoDims;i++)
    {
      ar << dDIMs[i];
    }
    DspSec.Serialize(ar,iV);
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> iMID;
    ar >> sGROUP;
    ar >> sSecType;
    ar >> iNoDims;
    ar >> dNSM;
    for (i=0;i<iNoDims;i++)
    {
      ar >> dDIMs[i];
    }
    DspSec.Serialize(ar,iV);
    CalcProps();
  }
}


void PBARL::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","BEAM");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %s","TYPE  : ",sSecType);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %s","GRP   : ",sGROUP);
  outtext1(_T(S1));
  sprintf_s(S1,"%s %i","MAT1  : ",iMID);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","A     : ",A);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","Izz   : ",Izz);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","Iyy   : ",Iyy);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","J     : ",J);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","NSM   : ",dNSM);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %i","IDIMS : ",iNoDims);
  outtext1(_T(S1)); 
  int i;
  for (i=0;i<iNoDims;i++)
  {
    sprintf_s(S1,"DIM: %i : %f""IDIMS : ",i,dDIMs[i]);
    outtext1(_T(S1)); 
  }
}

void PBARL::CreateSec()
{

if (sSecType.Find("BOX")>-1)
{
  DspSec.CreateBox(dDIMs[0],dDIMs[1],dDIMs[2],dDIMs[3]);
}
else if (sSecType.Find("BAR")>-1)
{
  DspSec.CreateBar(dDIMs[0],dDIMs[1]);
}
else if (sSecType.Find("ROD")>-1)
{
  DspSec.CreateRod(dDIMs[0]);
}
else if (sSecType.Find("TUBE")>-1)
{
  DspSec.CreateTube(dDIMs[0],dDIMs[1]);
}
else if (sSecType.Find("I  ")>-1)
{
  DspSec.CreateI(dDIMs[0],dDIMs[1],dDIMs[2],dDIMs[3],dDIMs[4],dDIMs[5]);
}
this->CalcProps();
}

int PBARL::GetMat()
{
return (iMID);
}


BOOL PBARL::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}

int PBARL::GetNoDims()
{
int iRet=-1;
if (sSecType.Find("BOX")>-1)
{
   iRet=4;
}
else if (sSecType.Find("BAR")>-1)
{
   iRet=2;
}
else if (sSecType.Find("ROD")>-1)
{
   iRet=1;
}
else if (sSecType.Find("TUBE")>-1)
{
   iRet=2;
}
else if (sSecType.Find("I   ")>-1)
{
   iRet=6;
}
return iRet;
}

void PBARL::ExportNAS(FILE* pFile)
{
int i;
fprintf(pFile,"$%s\n",sTitle);
fprintf(pFile,"%8s%8i%8i%8s%8s%8s\n","PBARL   ",iID,iMID,sGROUP,ncr(sSecType)," ");
fprintf(pFile,"%8s"," ");
for (i=0;i<iNoDims;i++)
{
  fprintf(pFile,"%8s",e8(dDIMs[i]));
}
fprintf(pFile,"%8s\n",e8(dNSM));
}

PBARL* PBARL::Copy()
{
int  i;
PBARL* pREt=new PBARL();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID=iMID;
pREt->sGROUP=sGROUP;
pREt->sSecType=sSecType;
pREt->iNoDims=iNoDims;
pREt->dNSM=dNSM;
    for (i=0;i<iNoDims;i++)
    {
      pREt->dDIMs[i]=dDIMs[i];
    }
pREt->DspSec.iLnCnt1=DspSec.iLnCnt1;
pREt->DspSec.iLnCnt2=DspSec.iLnCnt2;
for (i=0;i<DspSec.iLnCnt1;i++)
{
  pREt->DspSec.pLnLoop1[i] = DspSec.pLnLoop1[i];
}
for (i=0;i<DspSec.iLnCnt2;i++)
{
  pREt->DspSec.pLnLoop2[i] = DspSec.pLnLoop2[i];
}
return (pREt);
}

BSec* PBARL::GetSec()
{
return (&DspSec);
}

void PBARL::UpdateMats(NEList* newMats)
{
iMID=newMats->Get(iMID);
}

void PBARL::ChangeMat(int thisMat,int inMID)
{
if (iMID==thisMat)
  iMID=inMID;
}

int PBARL::GetDefMatID()
{
  return (iMID);
}

void PBARL::CalcProps()
{
if (sSecType.Find("ROD")!=-1)
{
  double R=dDIMs[0];
  A=Pi*R*R;
  Izz=Pi*R*R*R*R/4;
  Iyy=Izz;
  J=Pi*R*R*R*R/2;
}
else if (sSecType.Find("TUBE")!=-1)
{
  double Do=2*dDIMs[0];
  double Di=2*dDIMs[1];
  A=Pi*(Do*Do-Di*Di)/4;
  Izz=Pi*(Do*Do*Do*Do-Di*Di*Di*Di)/64;
  Iyy=Izz;
  J=Pi*(Do*Do*Do*Do-Di*Di*Di*Di)/32;
}
else if (sSecType.Find("BAR")!=-1)
{
  double W=dDIMs[0];
  double H=dDIMs[1];
  double a,b;
  a=W;
  b=H;
  if (a<b)
  {
    a=H;
    b=W;
  }
  A=W*H;
  Iyy=H*W*W*W/12;
  Izz=W*H*H*H/12;
  J=a*b*b*b*(0.333333333-0.21*b/a*(1-b*b*b*b/(12*a*a*a*a)));

}
else if (sSecType.Find("BOX")!=-1)
{
  double Wo=dDIMs[0];
  double Ho=dDIMs[1];
  double Wi=Wo-2*dDIMs[2];
  double Hi=Ho-2*dDIMs[3];
  A=Wo*Ho-Wi*Hi;
  Iyy=Ho*Wo*Wo*Wo/12-Hi*Wi*Wi*Wi/12;
  Izz=Wo*Ho*Ho*Ho/12-Wi*Hi*Hi*Hi/12;
  J=Izz+Iyy;
}
else
{
  outtext1("ERROR: Unable to Calculate Beam Properties.");
}
}

//************************************************
IMPLEMENT_DYNAMIC(PROD , CObject)

PROD::PROD()
{
sTitle="PROD";
iID = -1;
iType= 11;
iMID=-1;
sSecType="VIEW";
DspSec.Clear();
A=0;
J=0;
}

void PROD::Serialize(CArchive& ar,int iV)
{

	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << iMID;
    ar << sGROUP;
    ar << sSecType;
    ar << A;
    ar << J;
    DspSec.Serialize(ar,iV);
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> iMID;
    ar >> sGROUP;
    ar >> sSecType;
    ar >> A;
    ar >> J;
    DspSec.Serialize(ar,iV);
  }
}

void PROD::CreateSec()
{
double W;
W=pow(A,0.5);
W*=1/Pi;
DspSec.CreateDefSec(W,4);
}

BOOL PROD::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}





PROD* PROD::Copy()
{
int  i;
PROD* pREt=new PROD();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID=iMID;
pREt->sGROUP=sGROUP;
pREt->sSecType=sSecType;

pREt->DspSec.iLnCnt1=DspSec.iLnCnt1;
pREt->DspSec.iLnCnt2=DspSec.iLnCnt2;
for (i=0;i<DspSec.iLnCnt1;i++)
{
  pREt->DspSec.pLnLoop1[i] = DspSec.pLnLoop1[i];
}
for (i=0;i<DspSec.iLnCnt2;i++)
{
  pREt->DspSec.pLnLoop2[i] = DspSec.pLnLoop2[i];
}
return (pREt);
}


void PROD::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","ROD");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %s","TYPE  : ",sSecType);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %s","GRP   : ",sGROUP);
  outtext1(_T(S1));
  sprintf_s(S1,"%s %i","MAT1  : ",iMID);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","A     : ",A);
  outtext1(_T(S1)); 
  sprintf_s(S1,"%s %f","J     : ",J);
  outtext1(_T(S1)); 
}

BSec* PROD::GetSec()
{
return (&DspSec);
}

int PROD::GetMat()
{
return (iMID);
}

void PROD::UpdateMats(NEList* newMats)
{
iMID=newMats->Get(iMID);
}

void PROD::ChangeMat(int thisMat,int inMID)
{
if (iMID==thisMat)
  iMID=inMID;
}

int PROD::GetDefMatID()
{
  return (iMID);
}

void PROD::CalcProps()
{

}

void PROD::ExportNAS(FILE* pFile)
{
fprintf(pFile,"%8s%8i%8i%8s%8s\n","PROD    ",iID,iMID,e8(A),e8(J));
}
//***************************************************

int PROD::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Material ID (MID)";
  sVar[1] = "Secton area (A)";
  sVar[2] = "Torsional constant (J)";
  return(3);
}


int PROD::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%i", iMID);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", A);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", J);
  sVar[iNo] = S1;
  iNo++;
  return (iNo);
}

void PROD::PutVarValues(int iNo, CString sVar[])
{
  iMID = atoi(sVar[0]);
  A = atof(sVar[1]);
  J = atof(sVar[2]);
  CreateSec();
}



IMPLEMENT_DYNAMIC(PSHELL , CObject)

void PSHELL::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar<< iType;
    ar<< sTitle;
    ar<< iID;
    ar<< iMID1;
    ar<< dT;
    ar<< iMID2;
    ar<< d12IT3;
    ar<< iMID3;
    ar<< dTST;
    ar<< dNSM;
    ar<< dZ1;
    ar<< dZ2;
    ar<<iMID4;
  }
  else
  {
    ar>>iType;
    ar>>sTitle;
    ar>>iID;
    ar>> iMID1;
    ar>> dT;
    ar>> iMID2;
    ar>> d12IT3;
    ar>> iMID3;
    ar>> dTST;
    ar>> dNSM;
    ar>> dZ1;
    ar>> dZ2;
    ar>>iMID4;
  }
}

int PSHELL::GetMat()
{
return (iMID1);
}

BOOL PSHELL::HasMat(int inMID)
{
  BOOL brc=FALSE;
  if (iMID1==inMID)
  {
    brc=TRUE;
  }
  return (brc);
}

void PSHELL::List()
{
  char S1[200];
  CString OutT;
  outtext1("PROPERTY LISTING:-");
  sprintf_s(S1,"%s %i %s %s","PID",iID,"TYPE ","SHELL");
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %s","TITLE : ",sTitle);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %i","MAT1  : ",iMID1);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","THK   : ",dT);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %i","MAT2  : ",iMID2);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","12IT3 : ",d12IT3);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %i","MAT3  : ",iMID3);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","TST   : ",dTST);
  OutT=S1;
  outtext1(OutT); 
  sprintf_s(S1,"%s %f","NSM   : ",dNSM);
  OutT=S1;
  outtext1(OutT); 
  if ((dZ1!=DBL_MAX) || (dZ2!=DBL_MAX) || (iMID4>0))
  {
    sprintf_s(S1,"%s %f","Z1    : ",dZ1);
    OutT=S1;
    outtext1(OutT); 
    sprintf_s(S1,"%s %f","Z2    : ",dZ2);
    OutT=S1;
    outtext1(OutT); 
  }
}

double PSHELL::GetThk()
{
return(dT);
}

void PSHELL::UpdateMats(NEList* newMats)
{
 iMID1=newMats->Get(iMID1);
 iMID2=newMats->Get(iMID2);
 iMID3=newMats->Get(iMID3);
 iMID4=newMats->Get(iMID4);
}

void PSHELL::ChangeMat(int thisMat,int inMID)
{
if (iMID1==thisMat)
{iMID1=inMID;}
if (iMID2==thisMat)
{iMID2=inMID;}
if (iMID3==thisMat)
{iMID3=inMID;}

}

int PSHELL::GetDefMatID()
{
return(iMID1);
}

PSHELL* PSHELL::Copy()
{
PSHELL* pREt=new PSHELL();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->iMID1=iMID1;
pREt->dT=dT;
pREt->iMID2=iMID2;
pREt->d12IT3=d12IT3;
pREt->iMID3=iMID3;
pREt->dTST=dTST;
pREt->dNSM=dNSM;
pREt->dZ1=dZ1;
pREt->dZ2=dZ2;
return (pREt);
}


void PSHELL::ExportNAS(FILE* pFile)
{
fprintf(pFile, "$%s\n", sTitle);
fprintf(pFile,"%s",ToString());
}

CString PSHELL::ToString()
{
char S[200]="";
CString src="";
sprintf_s(S,"%8s%8i%8i%8s%8i","PSHELL  ",iID,iMID1,e8(dT),iMID2);
src=S;
if (d12IT3==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(d12IT3));
src+=S;
if (iMID3==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8i",iMID3);
src+=S;
if (dTST==0)
   sprintf_s(S,"%8s","        ");
else
sprintf_s(S,"%8s",e8(dTST));
src+=S;
sprintf_s(S,"%8s\n",e8(dNSM));
src+=S;
if ((dZ1!=0) || (dZ2!=0) || (iMID4!=0))
{
  src+="        ";
  if (dZ1==0)
   sprintf_s(S,"%8s","        ");
  else
   sprintf_s(S,"%8s",e8(dZ1));
  src+=S;
  if (dZ2==0)
   sprintf_s(S,"%8s","        ");
  else
   sprintf_s(S,"%8s",e8(dZ2));
  src+=S;
  if (iMID4==0)
   sprintf_s(S,"%8s\n","        ");
  else
   sprintf_s(S,"%8i\n",iMID4);
  src+=S;
}
return (src);
}

PSHELL::PSHELL()
{
  sTitle="";
  iID = -1;
  iType= 1;
  iMID1=-1;
  dT=0;
  iMID2=-1;
  d12IT3=1.0;
  iMID3=-1;
  dTST=.833333;
  dNSM=0;
  dZ1=0;
  dZ2=0;
  iMID4=0;
}


int PSHELL::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Material ID 1 (MID)";
  sVar[1] = "Thickness (T)";
  sVar[2] = "Material ID 2 (MID2)";
  sVar[3] = "Bending stiffness parameter (12IT3)";
  sVar[4] = "Material transverse shear (MID3)";
  sVar[5] = "Transverse shear ratio  (TS/T)";
  sVar[6] = "Non structural mass  (NSM)";
  sVar[7] = "Fibre dist for stresses (Z1)";
  sVar[8] = "Fibre dist for stresses (Z2)";
  sVar[9] = "Material embrane-bending coupling (MID4)";
  return(10);
}


int PSHELL::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%i", iMID1);
  sVar[0] = S1;
  iNo++;
  sprintf_s(S1, "%g", dT);
  sVar[1] = S1;
  iNo++;
  sprintf_s(S1, "%i", iMID2);
  sVar[2] = S1;
  iNo++;
  sprintf_s(S1, "%g", d12IT3);
  sVar[3] = S1;
  iNo++;
  sprintf_s(S1, "%i", iMID3);
  sVar[4] = S1;
  iNo++;
  sprintf_s(S1, "%g", dTST);
  sVar[5] = S1;
  iNo++;
  sprintf_s(S1, "%g", dNSM);
  sVar[6] = S1;
  iNo++;
  sprintf_s(S1, "%g", dZ1);
  sVar[7] = S1;
  iNo++;
  sprintf_s(S1, "%g", dZ2);
  sVar[8] = S1;
  iNo++;
  sprintf_s(S1, "%i", iMID4);
  sVar[9] = S1;
  iNo++;
  return (iNo);
}


void PSHELL::PutVarValues(int iNo, CString sVar[])
{
iMID1  = atoi(sVar[0]);
dT     = atof(sVar[1]);
iMID2  = atoi(sVar[2]);
d12IT3 = atof(sVar[3]);
iMID3  = atoi(sVar[4]);
dTST   = atof(sVar[5]);
dNSM   = atof(sVar[6]);
dZ1    = atof(sVar[7]);
dZ2    = atof(sVar[8]);
iMID4  = atoi(sVar[9]);
}

//MAt1
IMPLEMENT_DYNAMIC(MAT1 , CObject)

MAT1* MAT1::Copy()
{
MAT1* pREt=new MAT1();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;
pREt->dE=dE;
pREt->dG=dG;
pREt->dNU=dNU;
pREt->dRHO=dRHO;
pREt->dA=dA;
pREt->dTREF=dTREF;
pREt->dGE=dGE;
pREt->dST=dST;
pREt->dSC=dSC;
pREt->dSS=dSS;
pREt->iMCSID=iMCSID;
return (pREt);
}

double MAT1::GetDensity()
{
  return(dRHO);
}

double MAT1::GetCTE()
{
  return(dA);
}

double MAT1::GetE()
{
  return(dE);
}

double MAT1::GetV()
{
  return(dNU);
}

void MAT1::Info()
{
  char S1[200]="";
  outtext1("MATERIAL LISTING:-");
  sprintf_s(S1,"LAB: %i TITLE: %s",iID,sTitle);
  outtext1(S1);
  sprintf_s(S1,"E    : %f",dE);
  outtext1(S1);
  if (dG!=DBL_MAX)
     sprintf_s(S1,"G    : %s",float8NAS(dG));
  else
     sprintf_s(S1,"G    : %s","");
  outtext1(S1);
  sprintf_s(S1,"NU   : %s",float8NAS(dNU));
  outtext1(S1);
  sprintf_s(S1,"RHO  : %s",float8NAS(dRHO));
  outtext1(S1);
  sprintf_s(S1,"CTE  : %s",float8NAS(dA));
  outtext1(S1);
  sprintf_s(S1,"MCID : %i",iMCSID);
  outtext1(S1);
  sprintf_s(S1,"k    : %s",float8NAS(dk));
  outtext1(S1);
}

int MAT1::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Young Modulus (E)";
  sVar[1] = "Shear Modulus (G)";
  sVar[2] = "Poisions Ratio )NU)";
  sVar[3] = "Density (RHO)";
  sVar[4] = "Coeff Thermal Expansion (CTE)";
  sVar[5] = "Reference Temperatue (TREF)";
  sVar[6] = "Material Coordinate Sys (MCID)";
  sVar[7] = "Thermal Conductivity (k)";
  return(8);
}


int MAT1::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%g", dE);
  sVar[0] = S1;
  iNo++;
  sprintf_s(S1, "%g", dG);
  sVar[1] = S1;
  iNo++;
  sprintf_s(S1, "%g", dNU);
  sVar[2] = S1;
  iNo++;
  sprintf_s(S1, "%g", dRHO);
  sVar[3] = S1;
  iNo++; 
  sprintf_s(S1, "%g", dA);
  sVar[4] = S1;
  iNo++;
  sprintf_s(S1, "%g", dTREF);
  sVar[5] = S1;
  iNo++;
  sprintf_s(S1, "%i", iMCSID);
  sVar[6] = S1;
  iNo++;
  sprintf_s(S1, "%g", dk);
  sVar[7] = S1;
  iNo++;
  return (iNo);
}


void MAT1::PutVarValues(int iNo, CString sVar[])
{
  dE = atof(sVar[0]);
  dG = atof(sVar[1]);
  dNU = atof(sVar[2]);
  dRHO = atof(sVar[3]);
  dA = atof(sVar[4]);
  dTREF = atof(sVar[5]);
  iMCSID = atoi(sVar[6]);
  dk = atof(sVar[7]);
}


void MAT1::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << dE;
    ar << dG;
    ar << dNU;
    ar << dRHO;
    ar << dA;
    ar << dk;
    ar << dTREF;
    ar << dGE;
    ar << dST;
    ar << dSC;
    ar << dSS;
    ar << iMCSID;
  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> dE;
    ar >> dG;
    ar >> dNU;
    ar >> dRHO;
    ar >> dA;
    ar >> dk;
    ar >> dTREF;
    ar >> dGE;
    ar >> dST;
    ar >> dSC;
    ar >> dSS;
    ar >> iMCSID;
  }
}


void MAT1::ExportNAS(FILE* pFile)
{
fprintf(pFile,"$%s\n",sTitle);
CString sG = "        ";
CString sV = "        ";
CString CTE;
if (dG>0)
{
	sG=e8(dG);
}
else
{
	sV = e8(dNU);
}
CTE = e8(dA);
fprintf(pFile,"%8s%8i%8s%8s%8s%8s%8s%8s%8s\n","MAT1    ",iID,e8(dE),sG,sV,e8(dRHO),e8(dA),e8(dTREF),e8(dGE));
//fprintf(pFile,"%8s%8s%8s%8s%8i\n","        ",e8(dST),e8(dSC),e8(dSS),iMCSID);
}

MAT1::MAT1()
{
sTitle="";
iID = -1;
iType = 1;
dE=210e9;
dG=0;
dNU=0.29;
dRHO=0;
dA=0;
dk=0;
dTREF=0;
dGE=0;
dST=0;
dSC=0;
dSS=0;
iMCSID=-1;
}


//MAt8
IMPLEMENT_DYNAMIC(MAT8 , CObject)

MAT8* MAT8::Copy()
{
MAT8* pREt=new MAT8();
pREt->iType=iType;
pREt->sTitle=sTitle;
pREt->iID=iID;
pREt->dE1=dE1;
pREt->dE2=dE2;
pREt->dNU12=dNU12;
pREt->dG12=dG12;
pREt->dG1Z=dG1Z;
pREt->dG2Z=dG2Z;
pREt->dRHO=dRHO;

pREt->dA1=dA1;
pREt->dA2=dA2;
pREt->dTREF=dTREF;
pREt->dXt=dXt;
pREt->dXc=dXc;
pREt->dYt=dYt;
pREt->dYc=dYc;

pREt->dS=dS;
pREt->dGE=dGE;
pREt->F12=F12;
pREt->STRN=STRN;
return (pREt);
}

double MAT8::GetDensity()
{
  return(dRHO);
}

void MAT8::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
    ar << iType;
    ar << sTitle;
    ar << iID;
    ar << dE1;
    ar << dE2;
    ar << dNU12;
    ar << dG12;
    ar << dG1Z;
    ar << dG2Z;
    ar << dRHO;

    ar << dA1;
    ar << dA2;
    ar << dTREF;
    ar << dXt;
    ar << dXc;
    ar << dYt;
    ar << dYc;

    ar << dS;
    ar << dGE;
    ar << F12;
    ar << STRN;

  }
  else
  {
    ar >> iType;
    ar >> sTitle;
    ar >> iID;
    ar >> dE1;
    ar >> dE2;
    ar >> dNU12;
    ar >> dG12;
    ar >> dG1Z;
    ar >> dG2Z;
    ar >> dRHO;

    ar >> dA1;
    ar >> dA2;
    ar >> dTREF;
    ar >> dXt;
    ar >> dXc;
    ar >> dYt;
    ar >> dYc;

    ar >> dS;
    ar >> dGE;
    ar >> F12;
    ar >> STRN;
  }
}


void MAT8::ExportNAS(FILE* pFile)
{
fprintf(pFile,"%s",ToString());
//fprintf(pFile,"%8s%8i%8s%8s%8s%8s%8s%8s%8s\n","MAT8    ",iID,e8(dE1),e8(dE2),e8(dNU12),e8(dG12),e8(dG1Z),e8(dG2Z),e8(dRHO));
//fprintf(pFile,"%8s%8s%8s%8s%8s%8s%8s%8s%8s\n","        ",e8(dA1),e8(dA2),e8(dTREF),e8(dXt),e8(dXc),e8(dYt),e8(dYc),e8(dS));
//fprintf(pFile,"%8s%8s%8s\n","        ",e8(dGE),e8(F12),e8(STRN));
}

CString MAT8::ToString()
{
char S[200]="";
CString src="";
sprintf_s(S,"%8s%8i%8s%8s%8s%8s%8s%8s%8s\n","MAT8    ",iID,e8(dE1),e8(dE2),e8(dNU12),e8(dG12),e8(dG1Z),e8(dG2Z),e8(dRHO));
src=S;
sprintf_s(S,"        %8s%8s",e8(dA1),e8(dA2));
src+=S;
if (dTREF==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(dTREF));
src+=S;
if (dXt==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(dXt));
src+=S;
if (dXc==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(dXc));
src+=S;
if (dYt==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(dYt));
src+=S;
if (dYc==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(dYc));
src+=S;
if (dS==0)
   sprintf_s(S,"%8s\n","        ");
else
   sprintf_s(S,"%8s\n",e8(dS));
src+=S;
if (dGE==0)
   sprintf_s(S,"        %8s","        ");
else
   sprintf_s(S,"        %8s",e8(dGE));
src+=S;
if (F12==0)
   sprintf_s(S,"%8s","        ");
else
   sprintf_s(S,"%8s",e8(F12));
src+=S;
if (STRN==0)
   sprintf_s(S,"%8s\n","        ");
else
   sprintf_s(S,"%8s\n",e8(STRN));
src+=S;
return (src);
}


MAT8::MAT8()
{

sTitle="";
iID = -1;
iType = 8;
dE1 = 0;
dE2 = 0;
dNU12 = 0;
dG12 = 0;
dG1Z = 0;
dG2Z = 0;
dRHO = 0;

dA1 = 0;
dA2 = 0;
dTREF = 0;
dXt = 0;
dXc = 0;
dYt = 0;
dYc = 0;

dS = 0;
dGE = 0;
F12 = 0;
STRN = 0;
}

int MAT8::GetVarHeaders(CString sVar[])
{
  sVar[0] = "Young Modulus longitudinal (E1)";
  sVar[1] = "Young Modulus lateral (E2)";
  sVar[2] = "Poisions ratio (NU12)";
  sVar[3] = "In-plane shear modulus(G12)";
  sVar[4] = "Transverse shear modulus 1-Z plane.(G1Z)";
  sVar[5] = "Transverse shear modulus 2-Z plane.(G2Z)";
  sVar[6] = "Density (RHO)";
  sVar[7] = "Coeff thermal expansion 1 Dir (A1)";
  sVar[8] = "Coeff thermal expansion 2 Dir (A2)";
  sVar[9] = "Reference Temperatue (TREF)";
  sVar[10] = "Allow tensile stress 1 Dir (dXt)";
  sVar[11] = "Allow compressive stress 1 Dir (dXc)";
  sVar[12] = "Allow tensile stress 2 Dir (dYt)";
  sVar[13] = "Allow compressive stress 2 Dir (dYc)";
  sVar[14] = "Allow stress/strain in-plane shear (S)";
  sVar[15] = "Structural damping coefficient (GE)";
  sVar[16] = "Tsai-Wu interactive term (F12)";
  sVar[17] = "Xt,Xc,Yt,Yc are STRN if 1 (STRN)";
  //sVar[7] = "Thermal Conductivity (k)";
  return(18);
}


int MAT8::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%g", dE1);
  sVar[0] = S1;
  iNo++;
  sprintf_s(S1, "%g", dE2);
  sVar[1] = S1;
  iNo++;
  sprintf_s(S1, "%g", dNU12);
  sVar[2] = S1;
  iNo++;
  sprintf_s(S1, "%g", dG12);
  sVar[3] = S1;
  iNo++;
  sprintf_s(S1, "%g", dG1Z);
  sVar[4] = S1;
  iNo++;
  sprintf_s(S1, "%g", dG2Z);
  sVar[5] = S1;
  iNo++;
  sprintf_s(S1, "%g", dRHO);
  sVar[6] = S1;
  iNo++;
  sprintf_s(S1, "%g", dA1);
  sVar[7] = S1;
  iNo++;
  sprintf_s(S1, "%g", dA2);
  sVar[8] = S1;
  iNo++;
  sprintf_s(S1, "%g", dTREF);
  sVar[9] = S1;
  iNo++;
  sprintf_s(S1, "%g", dXt);
  sVar[10] = S1;
  iNo++;
  sprintf_s(S1, "%g", dXc);
  sVar[11] = S1;
  iNo++;
  sprintf_s(S1, "%g", dYt);
  sVar[12] = S1;
  iNo++;
  sprintf_s(S1, "%g", dYc);
  sVar[13] = S1;
  iNo++;
  sprintf_s(S1, "%g", dS);
  sVar[14] = S1;
  iNo++;
  sprintf_s(S1, "%g", dGE);
  sVar[15] = S1;
  iNo++;
  sprintf_s(S1, "%g", F12);
  sVar[16] = S1;
  iNo++;
  sprintf_s(S1, "%i", STRN);
  sVar[17] = S1;
  iNo++;
  return (iNo);
}


void MAT8::PutVarValues(int iNo, CString sVar[])
{
  dE1 = atof(sVar[0]);
  dE2 = atof(sVar[1]);
  dNU12 = atof(sVar[2]);
  dG12 = atof(sVar[3]);
  dG1Z = atof(sVar[4]);
  dG2Z = atof(sVar[5]);
  dRHO = atof(sVar[6]);

  dA1 = atof(sVar[7]);
  dA2 = atof(sVar[8]);
  dTREF = atof(sVar[9]);
  dXt = atof(sVar[10]);
  dXc = atof(sVar[11]);
  dYt = atof(sVar[12]);
  dYc = atof(sVar[13]);

  dS = atof(sVar[14]);
  dGE = atof(sVar[15]);
  F12 = atof(sVar[16]);
  STRN = atoi(sVar[17]);
}

//*************************************************************************
//                               P C O M P G
//*************************************************************************
IMPLEMENT_DYNAMIC(PCOMPG, CObject)

PCOMPG::PCOMPG()
{
	sTitle = "";
	iID = -1;
	iType = 222;
	dZ0 = -0.5;
	dNSM = 0;
	dSB = 0;
	FT = 4;
	dRefT = 0;
	dGE = 0;
	bLAM = FALSE;
	iNoLays = 0;
}

void PCOMPG::Serialize(CArchive& ar, int iV)
{
	int i;
	if (ar.IsStoring())
	{
		ar << sTitle;
		ar << iID;
		ar << iType;
		ar << dZ0;
		ar << dNSM;
		ar << dSB;
		ar << FT;
		ar << dRefT;
		ar << dGE;
		ar << bLAM;
		ar << iNoLays;
		for (i = 0; i < iNoLays; i++)
		{
			ar << GPLYID[i];
			ar << MID[i];
			ar << T[i];
			ar << Theta[i];
			ar << sOut[i];
		}
	}
	else
	{
		ar >> sTitle;
		ar >> iID;
		ar >> iType;
		ar >> dZ0;
		ar >> dNSM;
		ar >> dSB;
		ar >> FT;
		ar >> dRefT;
		ar >> dGE;
		ar >> bLAM;
		ar >> iNoLays;
		for (i = 0; i < iNoLays; i++)
		{
			ar >> GPLYID[i];
			ar >> MID[i];
			ar >> T[i];
			ar >> Theta[i];
			ar >> sOut[i];
		}
	}
}


void PCOMPG::AddLayer(int inPLYID, int inMID, double inT, double inThe, BOOL inSo)
{
	if (iNoLays < MAX_LAYERS)
	{
		GPLYID[iNoLays] = inPLYID;
		MID[iNoLays] = inMID;
		T[iNoLays] = inT;
		Theta[iNoLays] = inThe;
		sOut[iNoLays] = inSo;
		iNoLays++;
	}
	else
	{
		outtext1("ERROR: Max No of Layers Exceeded.");
	}
}

int PCOMPG::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	int i;
	char S1[80] = "";
	sVar[iNo] = "Dist to bottom surf (Z0)";
	iNo++;
	sVar[iNo] = "Non structural mass (NSM)";
	iNo++;
	sVar[iNo] = "Allow shear stress (SB)";
	iNo++;
	sVar[iNo] = "Failure theory (FT)";
	iNo++;
	sVar[iNo] = "Reference temperature (TREF)";
	iNo++;
	sVar[iNo] = "Damping coefficient  (GE)";
	iNo++;
	sVar[iNo] = "Laminate options.  (LAM)";
	iNo++;
	sVar[iNo] = "No Off Layers ";
	iNo++;
	for (i = 0; i < iNoLays; i++)
	{
		sprintf_s(S1, "Layer %i", i + 1);
		sVar[iNo] = S1;
		iNo++;
	}
	return(iNo);
}


int PCOMPG::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%g", dZ0);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dNSM);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dSB);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", FT);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dRefT);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", dGE);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", bLAM);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iNoLays);
	sVar[iNo] = S1;
	iNo++;
	int i;
	for (i = 0; i < iNoLays; i++)
	{
		sprintf_s(S1, "%i %i,%g,%g,%i", GPLYID[i], MID[i], T[i], Theta[i], sOut[i]);
		sVar[iNo] = S1;
		iNo++;
	}
	return (iNo);
}


void PCOMPG::PutVarValues(int iNo, CString sVar[])
{

}




IMPLEMENT_DYNAMIC(PCOMP , CObject)

void PCOMP::Serialize(CArchive& ar,int iV)
{
int i;
	if (ar.IsStoring())
	{
    ar<<sTitle;
    ar<<iID;
    ar<<iType;
    ar<<dZ0;
    ar<<dNSM;
    ar<<dSB;
    ar<<FT;
    ar<<dRefT;
    ar<<dGE;
    ar<<bLAM;
    ar<<iNoLays;
    for (i=0;i<iNoLays;i++)
    {
      ar<< MID[i];
      ar<< T[i];
      ar<< Theta[i];
      ar<< sOut[i];
    }
  }
  else
  {
    ar>>sTitle;
    ar>>iID;
    ar>>iType;
    ar>>dZ0;
    ar>>dNSM;
    ar>>dSB;
    ar>>FT;
    ar>>dRefT;
    ar>>dGE;
    ar>>bLAM;
    ar>>iNoLays;
    for (i=0;i<iNoLays;i++)
    {
      ar>> MID[i];
      ar>> T[i];
      ar>> Theta[i];
      ar>> sOut[i];
    }
  }
}

void PCOMP::List()
{
  int i;
  char S1[200];
  CString OutT;
  outtext1("PCOMP LISTING:-"); 
  Property::List();
  outtext1("Mat,Thk,Theta"); 
  for (i=0;i<iNoLays;i++)
  {
    sprintf_s(S1,"%i,%f,%f",MID[i],T[i],Theta[i]);
    OutT=S1;
    outtext1(OutT); 
  }
  if (bLAM==TRUE)
   outtext1("- - - - - - - - -S Y M E T R I C- - - - - - - - -"); 
  double dthk=GetThk();
    sprintf_s(S1,"%s,%f","Total thickness= ",dthk);
    OutT=S1;
    outtext1(OutT); 
	sprintf_s(S1,"%s,%f","Z0= ",dZ0);
    OutT=S1;
    outtext1(OutT); 
}

double PCOMP::GetThk()
{
int i;
double dRet=0;
for (i=0;i<iNoLays;i++)
{
  dRet+=T[i];
}
if (bLAM==TRUE)
   dRet*=2;
return (dRet);
}

void PCOMP::UpdateMats(NEList* newMats)
{
  int i;
  for (i=0;i<iNoLays;i++)
  {
    MID[i]=newMats->Get(MID[i]);
  }
}

void PCOMP::ChangeMat(int thisMat,int inMID)
{
  int i;
  for (i=0;i<iNoLays;i++)
  {
    if (MID[i]==inMID)
    {
      MID[i]=inMID;
    }
  }
}

BOOL PCOMP::HasMat(int inMID)
{
  int i;
  BOOL brc=FALSE;
  for (i=0;i<iNoLays;i++)
  {
    if (MID[i]==inMID)
    {
      brc=TRUE;
      break;
    }
  }
  return (brc);
}

CString PCOMP::ToString()
{
	int i;
	char S[200] = "";
	CString src = "";
	CString sSB;
	if (dSB != 0)
		sSB = e8(dSB);
	else
		sSB = "        ";
	CString sFT = "        ";
	if (FT == 1)
		sFT = "    HILL";
	else if (FT == 2)
		sFT = "    HOFF";
	else if (FT == 3)
		sFT = "    TSAI";
	else if (FT == 4)
		sFT = "  STRESS";
	else if (FT == 5)
		sFT = "    STRN";
	else if (FT == 6)
		sFT = "  LARCO2";
	else if (FT == 7)
		sFT = "   PUCK ";
	else if (FT == 8)
		sFT = "     MCT";
	CString sLAM = "        ";
	if (bLAM)
		sLAM = "SYM     ";
	sprintf_s(S, "%8s%8i%8s%8s%8s%8s%8s%8s%8s\n", "PCOMP   ", iID, e8(dZ0), e8(dNSM), sSB, sFT,e8(dRefT),e8(dGE),sLAM);
	src = S;
	int iLcnt = 0;
	for (i = 0; i < iNoLays; i++)
	{
		if (iLcnt == 0)
			src += "        ";
		CString sO="      NO";
		if (sOut[i] == TRUE)
			sO = "     YES";
		if (iLcnt == 0)
		{
			sprintf_s(S, "%8i%8s%8s%8s", MID[i], e8(T[i]), e8(Theta[i]), sO);
			src += S;
			iLcnt = 1;
		}
		else
		{
			sprintf_s(S, "%8i%8s%8s%8s\n", MID[i], e8(T[i]), e8(Theta[i]), sO);
			src += S;
			iLcnt = 0;
		}
	}
	if (iLcnt == 1)
		src+="\n";
	return(src);
}

void PCOMP::ExportNAS(FILE* pFile)
{
	fprintf(pFile, "$%s\n", sTitle);
	fprintf(pFile, "%s", ToString());
}

PCOMP* PCOMP::Copy()
{
int i;
PCOMP* pREt=new PCOMP();
pREt->iID=iID;
pREt->sTitle=sTitle;
pREt->iType=iType;

pREt->dZ0=dZ0;
pREt->dNSM=dNSM;
pREt->dSB=dSB;
pREt->FT=FT;
pREt->dRefT=dRefT;
pREt->dGE=dGE;
pREt->bLAM=bLAM;
pREt->iNoLays=iNoLays;
for (i=0;i<iNoLays;i++)
{
  pREt->MID[i]=MID[i];
  pREt->T[i]=T[i];
  pREt->Theta[i]=Theta[i];
  pREt->sOut[i]=sOut[i];
}
return (pREt);
}

//Add a layer to the pcomp stack
void PCOMP::AddLayer(int inMID,double inT,double inThe,BOOL inSo)
{
if (iNoLays<MAX_LAYERS)
{
   MID[iNoLays]=inMID;
   T[iNoLays]=inT;
   Theta[iNoLays]=inThe;
   sOut[iNoLays]=inSo;
   iNoLays++;
}
else
{
  outtext1("ERROR: Max No of Layers Exceeded.");
}


}

PCOMP::PCOMP()
{
sTitle="";
iID = -1;
iType= 2;
dZ0=-0.5;
dNSM=0;
dSB=0;
FT=4;
dRefT=0;
dGE=0;
bLAM=FALSE;
iNoLays=0;
}

int PCOMP::GetVarHeaders(CString sVar[])
{
  int iNo=0;
  int i;
  char S1[80] = "";
  sVar[iNo] = "Dist to bottom surf (Z0)";
  iNo++;
  sVar[iNo] = "Non structural mass (NSM)";
  iNo++;
  sVar[iNo] = "Allow shear stress (SB)";
  iNo++;
  sVar[iNo] = "Failure theory (FT)";
  iNo++;
  sVar[iNo] = "Reference temperature (TREF)";
  iNo++;
  sVar[iNo] = "Damping coefficient  (GE)";
  iNo++;
  sVar[iNo] = "Laminate options.  (LAM)";
  iNo++;
  sVar[iNo] = "No Off Layers ";
  iNo++;
  for (i = 0; i<iNoLays; i++)
  {
    sprintf_s(S1, "Layer %i",i+1);
    sVar[iNo] = S1;
    iNo++;
  }
  return(iNo);
}


int PCOMP::GetVarValues(CString sVar[])
{
  int iNo = 0;
  char S1[80] = "";
  sprintf_s(S1, "%g", dZ0);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dNSM);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dSB);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%i", FT);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dRefT);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%g", dGE);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%i", bLAM);
  sVar[iNo] = S1;
  iNo++;
  sprintf_s(S1, "%i", iNoLays);
  sVar[iNo] = S1;
  iNo++;
  int i;
  for (i=0;i<iNoLays;i++)
  {
    sprintf_s(S1, "%i,%g,%g,%i", MID[i],T[i], Theta[i],sOut[i]);
    sVar[iNo] = S1;
    iNo++;
  }
  return (iNo);
}


void PCOMP::PutVarValues(int iNo, CString sVar[])
{

}


IMPLEMENT_DYNAMIC(Moment, CObject)

void Moment::Create(G_Object* pInNode,
				            G_Object* Parrent,
				            C3dVector inF,
				            int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=323;
iLabel=pInNode->iLabel;
iColour=4;
pParent=Parrent;
pObj= pInNode;
Pt_Object* pNode=(Pt_Object*) pInNode;
SetID=inSetID;
F=inF;
Point[0].x=pNode->Pt_Point->x;
Point[0].y=pNode->Pt_Point->y;
Point[0].z=pNode->Pt_Point->z;
Point[1]=F;
Point[1].Normalize();
Point[1]+=Point[0];
}

void Moment::OglDraw(int iDspFlgs, double dS1, double dS2)
{
	OglDrawW(iDspFlgs, dS1, dS2);
}

void Moment::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	int i;
	double X, Y, Z;
	C3dVector Pts[7];  //Arrow Head
	C3dMatrix mT;
	C3dVector vOff;
	C3dVector vOffA2;
	if ((iDspFlgs & DSP_BC) > 0)
	{
		Selectable = 1;
		Point[1] = F;
		Point[1].Normalize();
		mT = Point[1].GetTMat();
		vOffA2 = Point[1];
		Point[1] *= dS1;
		vOffA2*=0.75*dS1;
		Point[1] += Point[0];
		vOffA2 += Point[0];
		vOff.Set(AHead[0][0], AHead[0][1], AHead[0][2]);
		for (i = 0; i < 7; i++)
		{
			Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
			Pts[i] -= vOff;
			Pts[i] = mT.Mult(Pts[i]);
			Pts[i] *= 0.2*dS1;
		}
		X = Point[1].x; Y = Point[1].y; Z = Point[1].z;

		glLineWidth(2);
		glColor3fv(cols[iColour]);
		glBegin(GL_LINES);
		glVertex3f((float)Point[0].x, (float)Point[0].y, (float)Point[0].z);
		glVertex3f((float)Point[1].x, (float)Point[1].y, (float)Point[1].z);
		glEnd();
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glVertex3f((float) 0.5*(Point[0].x + Point[1].x), (float)0.5*(Point[0].y + Point[1].y), (float)0.5*(Point[0].z + Point[1].z));
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
		glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
		glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
		glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
		glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
		glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
		glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
		glEnd();
		X = vOffA2.x; Y = vOffA2.y; Z = vOffA2.z;

		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
		glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
		glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
		glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
		glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
		glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
		glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
		glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
		glEnd();
		char sLab[20];
		sprintf_s(sLab, "%3.0f", F.Mag());
		OglString(iDspFlgs, Point[1].x, Point[1].y, Point[1].z, &sLab[0]);
	}
	else
	{
		Selectable = 0;
	}
}

void Moment::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s%8i%8i%8s%8s%8s%8s%8s\n","MOMENT  ",SetID,pObj->iLabel,"       0","     1.0",e8(F.x),e8(F.y),e8(F.z));
}

CString Moment::GetName()
{
	return ("Moment (MOMENT)");
}

IMPLEMENT_DYNAMIC(Pressure, CObject)

void Pressure::Create(E_Object* pInEl,
				              G_Object* Parrent,
				              C3dVector inF,
				              int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=324;
iLabel=pInEl->iLabel;
iColour=4;
pParent=Parrent;
pObj=pInEl;
SetID=inSetID;
F=inF;
Point[0]=pInEl->Get_Centroid();
Point[1]=pInEl->Get_Normal();
Point[1].Normalize();
Point[1]+=Point[0];
}

void Pressure::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int i;
double X, Y, Z;
C3dVector Pts[7];  //Arrow Head
C3dMatrix mT;
C3dVector vOff;

if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  E_Object* pE=(E_Object*) pObj;
  Point[0] = pE->Get_Centroid();
  Point[1]=pE->Get_Normal();
  if (F.x < 0)
	  Point[1] *= -1;

  Point[1].Normalize();
  mT = Point[1].GetTMat();
  Point[1]*=-dS1;
  Point[1]+=Point[0];
  vOff.Set(AHead[0][0], AHead[0][1], AHead[0][2]);
  for (i = 0; i < 7; i++)
  {
	  Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
	  Pts[i] -= vOff;
	  Pts[i] = mT.Mult(Pts[i]);
	  Pts[i] *= 0.2*dS1;
  }
  X = Point[0].x; Y = Point[0].y; Z = Point[0].z;
  glLineWidth(2);
  glColor3fv(cols[iColour]);
  glBegin(GL_LINES);
  glVertex3f((float) Point[0].x,(float) Point[0].y,(float) Point[0].z);
  glVertex3f((float) Point[1].x,(float) Point[1].y,(float) Point[1].z);
  glEnd();
  glPointSize(10.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) 0.5*(Point[0].x + Point[1].x), (float)0.5*(Point[0].y + Point[1].y), (float)0.5*(Point[0].z + Point[1].z));
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
  glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
  glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
  glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
  glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
  glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
  glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
  glEnd();
  char sLab[20];
  sprintf_s(sLab,"%3.0f",F.Mag());
  OglString(iDspFlgs,Point[1].x,Point[1].y,Point[1].z,&sLab[0]);
}
else
{
  Selectable=0;
}
}

void Pressure::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
G_Object::SetToScr(pModMat, pScrTran);
E_Object* pE = (E_Object*) pObj;
Point[0]=pE->Get_Centroid();

C3dVector V;
C3dVector R;
R.x = Point[0].x;
R.y = Point[0].y;
R.z = Point[0].z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
DSP_Point[0].x = R.x;
DSP_Point[0].y = R.y;
DSP_Point[0].z = 0;

R.x = Point[1].x;
R.y = Point[1].y;
R.z = Point[1].z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
DSP_Point[1].x = R.x;
DSP_Point[1].y = R.y;
DSP_Point[1].z = 0;
}

void Pressure::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iEL;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
    F.Serialize(ar,iV);
    ar << pObj->iLabel;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
    ar>>SetID;
    F.Serialize(ar,iV);
    ar>>iEL;
    pObj = MESH->GetElement(iEL);
	  pParent=MESH;
	}
}

void Pressure::ExportNAS(FILE* pFile)
{
int i;
E_Object* pE;
if (pObj != NULL)
{
	pE = (E_Object*) pObj;
	if (pE->iNoNodes == 3)
	{
		fprintf(pFile, "%8s%8i%8s%8i%8i%8i\n", "PLOAD   ", SetID,e8(F.x), pE->GetNode(0)->iLabel, pE->GetNode(1)->iLabel, pE->GetNode(2)->iLabel);
	}
	else if (pE->iNoNodes == 4)
	{
		fprintf(pFile, "%8s%8i%8s%8i%8i%8i%8i\n", "PLOAD   ", SetID, e8(F.x), pE->GetNode(0)->iLabel, pE->GetNode(1)->iLabel, pE->GetNode(2)->iLabel, pE->GetNode(3)->iLabel);
	}
}

}

CString Pressure::GetName()
{
	return ("Presure (PLOAD)");
}

int Pressure::GetVarHeaders(CString sVar[])
{
	int iNo = 0;

	sVar[iNo] = "P";
	iNo++;

	return(iNo);
}


int Pressure::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";

	sprintf_s(S1, "%g", F.x);
	sVar[iNo] = S1;
	iNo++;

	return (iNo);
}

void Pressure::PutVarValues(PropTable* PT,int iNo, CString sVar[])
{

	ME_Object* pMe = (ME_Object*)this->pParent;
	F.x = atof(sVar[0]);
}

//*********************************************************************************
//***************         START OF TEMPERATUE           ***************************
//*********************************************************************************

IMPLEMENT_DYNAMIC(Temperature, CObject)


Temperature::Temperature()
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=325;
iLabel=-1;
iColour=55;
pParent=NULL;
pObj= NULL;
SetID=-1;
dV=0;
}


Temperature::~Temperature()
{
SetID=-1;
pObj=NULL;
}

void Temperature::Create(G_Object* pInE,
				                 G_Object* Parrent,
				                 double inV,
				                 int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=325;
iLabel=pInE->iLabel;
iColour=4;
pParent=Parrent;
pObj= pInE;
SetID=inSetID;
dV=inV;
if (pObj!=NULL)
   Point=pObj->Get_Centroid();
}


void Temperature::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
  C3dVector V;
  C3dVector R;
  G_Object::SetToScr(pModMat, pScrTran);
  if (pObj != NULL)
  {
    Point = pObj->Get_Centroid();
    R.x = Point.x;
    R.y = Point.y;
    R.z = Point.z;
    V.x = pModMat->m_00 * R.x + pModMat->m_01 * R.y + pModMat->m_02 * R.z + pModMat->m_30;
    V.y = pModMat->m_10 * R.x + pModMat->m_11 * R.y + pModMat->m_12 * R.z + pModMat->m_31;
    V.z = pModMat->m_20 * R.x + pModMat->m_21 * R.y + pModMat->m_22 * R.z + pModMat->m_32;
    R.x = pScrTran->m_00 * V.x + pScrTran->m_01 * V.y + pScrTran->m_02 * V.z + pScrTran->m_30;
    R.y = pScrTran->m_10 * V.x + pScrTran->m_11 * V.y + pScrTran->m_12 * V.z + pScrTran->m_31;
    R.z = pScrTran->m_20 * V.x + pScrTran->m_21 * V.y + pScrTran->m_22 * V.z + pScrTran->m_32;
    DSP_Point.x = R.x;
    DSP_Point.y = R.y;
    DSP_Point.z = 0;
  }
}

void Temperature::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iE;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
      ar<<dV;
	  DSP_Point.Serialize(ar,iV);
      Point.Serialize(ar,iV);
      ar << pObj->iLabel;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
    ar>>SetID;
    ar>>dV;
	  DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar>>iE;
    pObj = MESH->GetElement(iE);
	  pParent=MESH;
	}
}


void Temperature::ExportUNV(FILE* pFile)
{

}

void Temperature::ExportNAS(FILE* pFile)
{

//fprintf(pFile,"%8s%8s%8i%8s%8s%8s%8s%8s\n","FORCE   ","       1",pObj->iLabel,"       0","     1.0",e8(F.x),e8(F.y),e8(F.z));
}



C3dVector Temperature::Get_Centroid()
{
C3dVector vT;
if (pObj!=NULL)
  vT = pObj->Get_Centroid();
return (vT);
}





void Temperature::OglDraw(int iDspFlgs,double dS1,double dS2)
{
  OglDrawW(iDspFlgs,dS1,dS2);
}

void Temperature::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  glColor3fv(cols[iColour]);
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glEnd();
  glLineWidth(2.0);
  char sLab[20];
  sprintf_s(sLab,"%3.0f",dV);
  OglString(iDspFlgs,Point.x,Point.y,Point.z,&sLab[0]);
}
else
{
  Selectable=0;
}
}



void Temperature::Info()
{
  char S1[80];
  CString OutT;
  G_Object::Info();
  sprintf_s(S1,"%s%i%s%i%s%f","SETID ",iObjType,"; NODE ",pObj->iLabel," Val; ",dV);
  OutT+=S1;
}

//*********************************************************************************
//***************         END OF TEMPERATUE           *****************************
//*********************************************************************************


//*********************************************************************************
//***************         START OF ACELERATION LOADING           ******************
//*********************************************************************************

IMPLEMENT_DYNAMIC(AccelLoad, CObject)


AccelLoad::AccelLoad()
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=328;
iLabel=-1;
iColour=28;
pParent=NULL;
pObj= NULL;
SetID=-1;
vA.Set(0,0,0);
}


AccelLoad::~AccelLoad()
{
SetID=-1;
pObj=NULL;
}

void AccelLoad::Create(G_Object* pInE,
				               G_Object* Parrent,
				               C3dVector inV,
				               int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=328;
iLabel=pInE->iLabel;
iColour=23;
pParent=Parrent;
pObj= pInE;
SetID=inSetID;
vA=inV;
if (pObj!=NULL)
   Point=pObj->Get_Centroid();
}


void AccelLoad::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
  C3dVector V;
  C3dVector R;
  G_Object::SetToScr(pModMat, pScrTran);
  if (pObj != NULL)
  {
    Point = pObj->Get_Centroid();
    R.x = Point.x;
    R.y = Point.y;
    R.z = Point.z;
    V.x = pModMat->m_00 * R.x + pModMat->m_01 * R.y + pModMat->m_02 * R.z + pModMat->m_30;
    V.y = pModMat->m_10 * R.x + pModMat->m_11 * R.y + pModMat->m_12 * R.z + pModMat->m_31;
    V.z = pModMat->m_20 * R.x + pModMat->m_21 * R.y + pModMat->m_22 * R.z + pModMat->m_32;
    R.x = pScrTran->m_00 * V.x + pScrTran->m_01 * V.y + pScrTran->m_02 * V.z + pScrTran->m_30;
    R.y = pScrTran->m_10 * V.x + pScrTran->m_11 * V.y + pScrTran->m_12 * V.z + pScrTran->m_31;
    R.z = pScrTran->m_20 * V.x + pScrTran->m_21 * V.y + pScrTran->m_22 * V.z + pScrTran->m_32;
    DSP_Point.x = R.x;
    DSP_Point.y = R.y;
    DSP_Point.z = 0;
  }
}


void AccelLoad::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iE;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
    vA.Serialize(ar,iV);
	  DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar << pObj->iLabel;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
    ar>>SetID;
    vA.Serialize(ar,iV);
	  DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar>>iE;
    pObj = MESH->GetElement(iE);
	  pParent=MESH;
	}
}


void AccelLoad::ExportUNV(FILE* pFile)
{

}

void AccelLoad::ExportNAS(FILE* pFile)
{

//fprintf(pFile,"%8s%8s%8i%8s%8s%8s%8s%8s\n","FORCE   ","       1",pObj->iLabel,"       0","     1.0",e8(F.x),e8(F.y),e8(F.z));
}



C3dVector AccelLoad::Get_Centroid()
{
C3dVector vT;
if (pObj!=NULL)
  vT = pObj->Get_Centroid();
return (vT);
}





void AccelLoad::OglDraw(int iDspFlgs,double dS1,double dS2)
{
  OglDrawW(iDspFlgs,dS1,dS2);
}

void AccelLoad::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  glColor3fv(cols[iColour]);
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glVertex3f((float) Point.x-0.15*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.15*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.15*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y-0.5*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y-0.5*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.15*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glEnd();
  glLineWidth(2.0);
  //sprintf_s(sLab,"%3.0f",dV);
  //OglString(iDspFlgs,Point.x,Point.y,Point.z,&sLab[0]);
}
else
{
  Selectable=0;
}
}



void AccelLoad::Info()
{
  char S1[80];
  CString OutT;
  G_Object::Info();
  sprintf_s(S1,"ACCEL VECTOR %8i X %s Y %s Z %s",iLabel,float8NAS(vA.x),float8NAS(vA.y),float8NAS(vA.z));
  outtext1(S1); 

}

//*********************************************************************************
//***************         START OF ACELERATION LOADING           ******************
//*********************************************************************************

IMPLEMENT_DYNAMIC(RotationLoad, CObject)


RotationLoad::RotationLoad()
{
  Drawn = 0;
  Selectable = 1;
  Visable = 1;
  iObjType = 329;
  iLabel = -1;
  iColour = 60;
  pParent = NULL;
  pObj = NULL;
  SetID = -1;
  vAxisD.Set(0, 0, 0);
  vAxisC.Set(0, 0, 0);
  w = 0;
}


RotationLoad ::~RotationLoad()
{
  SetID = -1;
  pObj = NULL;
}

void RotationLoad::Create(G_Object* pInE,
                          G_Object* Parrent,
                          C3dVector inD,
                          C3dVector inC,
                          double dW,
                          int inSetID)
{
  Drawn = 0;
  Selectable = 1;
  Visable = 1;
  iObjType = 329;
  iLabel = pInE->iLabel;
  iColour = 60;
  pParent = Parrent;
  pObj = pInE;
  SetID = inSetID;
  vAxisD = inD;
  vAxisC = inD;
  w = dW;
  if (pObj != NULL)
    Point = pObj->Get_Centroid();
}


void RotationLoad::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
  C3dVector V;
  C3dVector R;
  G_Object::SetToScr(pModMat, pScrTran);
  if (pObj != NULL)
  {
    Point = pObj->Get_Centroid();
    R.x = Point.x;
    R.y = Point.y;
    R.z = Point.z;
    V.x = pModMat->m_00 * R.x + pModMat->m_01 * R.y + pModMat->m_02 * R.z + pModMat->m_30;
    V.y = pModMat->m_10 * R.x + pModMat->m_11 * R.y + pModMat->m_12 * R.z + pModMat->m_31;
    V.z = pModMat->m_20 * R.x + pModMat->m_21 * R.y + pModMat->m_22 * R.z + pModMat->m_32;
    R.x = pScrTran->m_00 * V.x + pScrTran->m_01 * V.y + pScrTran->m_02 * V.z + pScrTran->m_30;
    R.y = pScrTran->m_10 * V.x + pScrTran->m_11 * V.y + pScrTran->m_12 * V.z + pScrTran->m_31;
    R.z = pScrTran->m_20 * V.x + pScrTran->m_21 * V.y + pScrTran->m_22 * V.z + pScrTran->m_32;
    DSP_Point.x = R.x;
    DSP_Point.y = R.y;
    DSP_Point.z = 0;
  }
}


void RotationLoad::Serialize(CArchive& ar, int iV, ME_Object* MESH)

{
  int iE;
  if (ar.IsStoring())
  {
    G_Object::Serialize(ar, iV);
    ar << SetID;
    vAxisD.Serialize(ar, iV);
    vAxisC.Serialize(ar, iV);
    ar << w;
    DSP_Point.Serialize(ar, iV);
    Point.Serialize(ar, iV);
    ar << pObj->iLabel;
  }
  else
  {
    G_Object::Serialize(ar, iV);
    ar >> SetID;
    vAxisD.Serialize(ar, iV);
    vAxisC.Serialize(ar, iV);
    ar >> w;
    DSP_Point.Serialize(ar, iV);
    Point.Serialize(ar, iV);
    ar >> iE;
    pObj = MESH->GetElement(iE);
    pParent = MESH;
  }
}


C3dVector RotationLoad::Get_Centroid()
{
  C3dVector vT;
  if (pObj != NULL)
    vT = pObj->Get_Centroid();
  return (vT);
}


void RotationLoad::OglDraw(int iDspFlgs, double dS1, double dS2)
{
  OglDrawW(iDspFlgs, dS1, dS2);
}

void RotationLoad::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
  if ((iDspFlgs & DSP_BC)>0)
  {
    Selectable = 1;
    glColor3fv(cols[iColour]);
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glVertex3f((float)Point.x - 0.15*dS1, (float)Point.y + 0.2*dS1, (float)Point.z);
    glVertex3f((float)Point.x + 0.15*dS1, (float)Point.y + 0.2*dS1, (float)Point.z);
    glVertex3f((float)Point.x + 0.15*dS1, (float)Point.y + 0.2*dS1, (float)Point.z);
    glVertex3f((float)Point.x, (float)Point.y - 0.5*dS1, (float)Point.z);
    //glVertex3f((float)Point.x, (float)Point.y - 0.5*dS1, (float)Point.z);
    //glVertex3f((float)Point.x - 0.15*dS1, (float)Point.y + 0.2*dS1, (float)Point.z);
    glEnd();
    glLineWidth(2.0);
    //sprintf_s(sLab,"%3.0f",dV);
    //OglString(iDspFlgs,Point.x,Point.y,Point.z,&sLab[0]);
  }
  else
  {
    Selectable = 0;
  }
}

void RotationLoad::Info()
{
  char S1[80];
  CString OutT;
  G_Object::Info();
  sprintf_s(S1, "AXIS CENTRE %8i X %s Y %s Z %s", iLabel, float8NAS(vAxisC.x), float8NAS(vAxisC.y), float8NAS(vAxisC.z));
  outtext1(S1);
  sprintf_s(S1, "AXIS VECTOR %8i X %s Y %s Z %s w %f", iLabel, float8NAS(vAxisD.x), float8NAS(vAxisD.y), float8NAS(vAxisD.z), w);
  outtext1(S1);
}


//*********************************************************************************
//***************         END OF ACCELERATION LOADING           *******************
//*********************************************************************************


//****************************************************************************
//****************************************************************************
//                       For thermal analysis
//****************************************************************************
//****************************************************************************
IMPLEMENT_DYNAMIC(TemperatureBC, CObject)

TemperatureBC::TemperatureBC()
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=327;
iLabel=-1;
iColour=4;
pParent=NULL;
pObj= NULL;
SetID=-1;
dV=0;
}


TemperatureBC::~TemperatureBC()
{
SetID=-1;
pObj=NULL;
}

void TemperatureBC::Create(G_Object* pInNode,
				           G_Object* Parrent,
				           double inV,
				           int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=327;
iLabel=pInNode->iLabel;
iColour=4;
pParent=Parrent;
pObj= pInNode;
SetID=inSetID;
dV=inV;
if (pObj!=NULL)
   Point=pObj->Get_Centroid();
}

void TemperatureBC::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  glColor3fv(cols[iColour]);
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glEnd();
  glLineWidth(2.0);
  char sLab[20];
  sprintf_s(sLab,"%g",dV);
  OglString(iDspFlgs,Point.x,Point.y,Point.z,&sLab[0]);
}
else
{
  Selectable=0;
}
}

void TemperatureBC::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iN;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
    ar<<dV;
	  DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar << pObj->iLabel;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
    ar>>SetID;
    ar>>dV;
	  DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar>>iN;
    pObj = MESH->GetNode(iN);
	  pParent=MESH;
	}
}

//***************           NET FLUX             *****************************
IMPLEMENT_DYNAMIC(FluxLoad, CObject)

FluxLoad::FluxLoad()
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=326;
iLabel=-1;
iColour=54;
pParent=NULL;
pObj= NULL;
SetID=-1;
dV=0;
}


FluxLoad::~FluxLoad()
{
SetID=-1;
pObj=NULL;
}

void FluxLoad::Create(G_Object* pInNode,
				      G_Object* Parrent,
				      double inV,
				      int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=326;
iLabel=pInNode->iLabel;
iColour=54;
pParent=Parrent;
pObj= pInNode;
SetID=inSetID;
dV=inV;
if (pObj!=NULL)
   Point=pObj->Get_Centroid();
}

void FluxLoad::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  glColor3fv(cols[iColour]);
  glLineWidth(4.0);
  glBegin(GL_LINES);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x+0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y-0.2*dS1,(float) Point.z);
  glVertex3f((float) Point.x-0.2*dS1,(float) Point.y+0.2*dS1,(float) Point.z);
  glEnd();
  glLineWidth(2.0);
  char sLab[20];
  sprintf_s(sLab,"%3.0f",dV);
  OglString(iDspFlgs,Point.x,Point.y,Point.z,&sLab[0]);
}
else
{
  Selectable=0;
}
}

void FluxLoad::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iN;
  if (ar.IsStoring())
  {
    G_Object::Serialize(ar,iV);
    ar<<SetID;
    ar<<dV;
    DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar << pObj->iLabel;
  }
  else
  {
    G_Object::Serialize(ar,iV);
    ar>>SetID;
    ar>>dV;
    DSP_Point.Serialize(ar,iV);
    Point.Serialize(ar,iV);
    ar>>iN;
    pObj = MESH->GetNode(iN);
    pParent=MESH;
  }
}
//*********************************************************************************
//***************         END OF THERMAL              *****************************
//*********************************************************************************

IMPLEMENT_DYNAMIC( Force, CObject )

void Force::Create(G_Object* pInNode,
				   G_Object* Parrent,
				   C3dVector inF,
				   int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=321;
iLabel=pInNode->iLabel;
iColour=55;
pParent=Parrent;
pObj= pInNode;
SetID=inSetID;
F=inF;
//Pt_Object* pNode = (Pt_Object*) pInNode;
Point[0]=pObj->Get_Centroid();
Point[1]=F;
Point[1].Normalize();
Point[1]+=Point[0];

}

void Force::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
      F.Serialize(ar,iV);
      ar << pObj->iLabel;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
      ar>>SetID;
      F.Serialize(ar,iV);
      ar>>iNd;
      pObj = MESH->GetNode(iNd);
	  pParent=MESH;

	}
}



Force::~Force()
{
SetID=-1;
pObj=NULL;
}

void Force::ExportUNV(FILE* pFile)
{

}

void Force::ExportNAS(FILE* pFile)
{

fprintf(pFile,"%8s%8i%8i%8s%8s%8s%8s%8s\n","FORCE   ", SetID,pObj->iLabel,"       0","     1.0",e8(F.x),e8(F.y),e8(F.z));
}


C3dVector Force::Get_Centroid()
{
C3dVector vT;
vT =Point[0];
vT+=Point[1];
vT*=0.5;
return (vT);
}


void Force::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
G_Object::SetToScr(pModMat, pScrTran);
Pt_Object* pNode = (Pt_Object*) pObj;
Point[0].x=pNode->Pt_Point->x;
Point[0].y=pNode->Pt_Point->y;
Point[0].z=pNode->Pt_Point->z;
//Point[1]=F;
//Point[1].Normalize();
//Point[1]*=0.05;
//Point[1]+=Point[0];
C3dVector V;
C3dVector R;
R.x = Point[0].x;
R.y = Point[0].y;
R.z = Point[0].z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
DSP_Point[0].x = R.x;
DSP_Point[0].y = R.y;
DSP_Point[0].z = 0;

R.x = Point[1].x;
R.y = Point[1].y;
R.z = Point[1].z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
DSP_Point[1].x = R.x;
DSP_Point[1].y = R.y;
DSP_Point[1].z = 0;
}

void Force::Draw(CDC* pDC,int iDrawmode)
{
pDC->MoveTo((int) DSP_Point[0].x,(int) DSP_Point[0].y);
pDC->LineTo((int) DSP_Point[1].x,(int) DSP_Point[1].y);
}

void Force::HighLight(CDC* pDC)
{
this->Draw(pDC,4);
}

void Force::OglDraw(int iDspFlgs,double dS1,double dS2)
{
  OglDrawW(iDspFlgs,dS1,dS2);
}

void Force::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int i;
double X, Y, Z;
C3dVector Pts[7];  //Arrow Head
C3dMatrix mT;
C3dVector vOff;
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  Point[1]=F;
  Point[1].Normalize();
  mT = Point[1].GetTMat();
  Point[1]*=dS1;
  Point[1]+=Point[0];
  vOff.Set(AHead[0][0], AHead[0][1], AHead[0][2]);
  for (i = 0; i < 7; i++)
  {
	  Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
	  Pts[i] -= vOff;
	  Pts[i] = mT.Mult(Pts[i]);
	  Pts[i] *= 0.2*dS1;
  }
  X = Point[1].x; Y = Point[1].y; Z = Point[1].z;

  glLineWidth(2);
  glColor3fv(cols[iColour]);
  glBegin(GL_LINES);
  glVertex3f((float) Point[0].x,(float) Point[0].y,(float) Point[0].z);
  glVertex3f((float) Point[1].x,(float) Point[1].y,(float) Point[1].z);
  glEnd();
  glPointSize(10.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) 0.5*(Point[0].x + Point[1].x), (float)0.5*(Point[0].y + Point[1].y), (float)0.5*(Point[0].z + Point[1].z));
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
  glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
  glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
  glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
  glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
  glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
  glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
  glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
  glEnd();
  char sLab[20];
  sprintf_s(sLab,"%3.0f",F.Mag());
  OglString(iDspFlgs,Point[1].x,Point[1].y,Point[1].z,&sLab[0]);
}
else
{
  Selectable=0;
}
}

G_ObjectD Force::SelDist(CPoint InPT,Filter FIL)
{

CPoint cPt2;
double dt;
double d;
G_ObjectD Ret;


dt =  0.5*(DSP_Point[0].x+DSP_Point[1].x)-InPT.x;
cPt2.x = long (dt);
dt =  0.5*(DSP_Point[0].y+DSP_Point[1].y)-InPT.y;
cPt2.y = long (dt);
d=(cPt2.x*cPt2.x+cPt2.y*cPt2.y);
Ret.Dist=d;
Ret.pObj=this;
return (Ret);
}

void Force::Info()
{
  char S1[80];
  CString OutT;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",pObj->iLabel," Col; ",iColour);
  OutT+=S1;
  outtext1("FORCE VECTOR");
  outtext1(OutT); 
  sprintf_s(S1,"%s%8.5f,%8.5f,%8.5f","FORCE: ",F.x,F.y,F.z);
  OutT=S1;
  outtext1(OutT); 
}

CString Force::GetName()
{
	return ("Force (FORCE)");
} 

int Force::GetVarHeaders(CString sVar[])
{
	int iNo = 0;

	sVar[iNo] = "FX";
	iNo++;
	sVar[iNo] = "FY";
	iNo++;
	sVar[iNo] = "FZ";
	iNo++;
	return(iNo);
}


int Force::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";

	sprintf_s(S1, "%g", F.x);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", F.y);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", F.z);
	sVar[iNo] = S1;
	iNo++;

	return (iNo);
}

void Force::PutVarValues(PropTable* PT,int iNo, CString sVar[])
{

	ME_Object* pMe = (ME_Object*)this->pParent;

	F.x = atof(sVar[0]);
	F.y = atof(sVar[1]);
	F.z = atof(sVar[2]);
}

IMPLEMENT_DYNAMIC( Restraint, CObject )

void Restraint::Create(G_Object* pInNode,
					             G_Object* Parrent,
					             BOOL xon,
                       BOOL yon,
                       BOOL zon,
                       BOOL rxon,
                       BOOL ryon,
                       BOOL rzon,		   
				               int inSetID)
{
Drawn=0;
Selectable=1; 
Visable=1;
iObjType=322;
iLabel=pInNode->iLabel;
iColour=2;
pParent=Parrent;
pObj=pInNode;
SetID=inSetID;
REST[0]=xon;
REST[1]=yon;
REST[2]=zon;
REST[3]=rxon;
REST[4]=ryon;
REST[5]=rzon;
}

void Restraint::Serialize(CArchive& ar,int iV,ME_Object* MESH)

{
  int iNd;
	if (ar.IsStoring())
	{
	  G_Object::Serialize(ar,iV);
	  ar<<SetID;
    ar << pObj->iLabel;
	  ar<<REST[0];
	  ar<<REST[1];
	  ar<<REST[2];
	  ar<<REST[3];
	  ar<<REST[4];
	  ar<<REST[5];
	}
	else
	{
	  G_Object::Serialize(ar,iV);
      ar>>SetID;
      ar>>iNd;
      pObj = MESH->GetNode(iNd);
      Pt_Object* pN = (Pt_Object*) pObj;
      Point=pN->GetCoords();
	  ar>>REST[0];
	  ar>>REST[1];
	  ar>>REST[2];
	  ar>>REST[3];
	  ar>>REST[4];
	  ar>>REST[5];
	  pParent=MESH;
	}
}



Restraint::~Restraint()
{
SetID=-1;
pObj=NULL;
}

void Restraint::ExportUNV(FILE* pFile)
{

}

void Restraint::ExportNAS(FILE* pFile)
{

C3dVector vCent;
vCent=Get_Centroid();
char S1[7];
sprintf_s(S1,"%s",GetDofStr());

fprintf(pFile,"%8s%8i%8i%8s%8s\n","SPC     ",SetID,pObj->iLabel,GetDofStr(),"     0.0");

}

C3dVector Restraint::Get_Centroid()
{
return (Point);
}


void Restraint::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
G_Object::SetToScr(pModMat, pScrTran);
Pt_Object* pNode = (Pt_Object*) pObj;
Point.x=pNode->Pt_Point->x;
Point.y=pNode->Pt_Point->y;
Point.z=pNode->Pt_Point->z;
//Point[1]=F;
//Point[1].Normalize();
//Point[1]*=0.05;
//Point[1]+=Point[0];
C3dVector V;
C3dVector R;
R.x = Point.x;
R.y = Point.y;
R.z = Point.z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
DSP_Point.x = R.x;
DSP_Point.y = R.y;
DSP_Point.z = 0;

}

void Restraint::Draw(CDC* pDC,int iDrawmode)
{
pDC->MoveTo((int) DSP_Point.x,(int) DSP_Point.y);
pDC->LineTo((int) DSP_Point.x-10,(int) DSP_Point.y-10);
pDC->LineTo((int) DSP_Point.x-10,(int) DSP_Point.y+10);
pDC->LineTo((int) DSP_Point.x,(int) DSP_Point.y);
}

void Restraint::HighLight(CDC* pDC)
{
this->Draw(pDC,4);
}

CString Restraint::GetDofStr()
{
CString sRet;
char sLab[7];
int i=0;
if (REST[0]) 
{
   sLab[i]='1'; 
   i++;
}
if (REST[1]) 
{
   sLab[i]='2'; 
   i++;
}
if (REST[2]) 
{
   sLab[i]='3'; 
   i++;
}
if (REST[3]) 
{
   sLab[i]='4'; 
   i++;
}
if (REST[4]) 
{
   sLab[i]='5'; 
   i++;
}
if (REST[5]) 
{
   sLab[i]='6'; 
   i++;
}
sLab[i]=NULL;
sRet=sLab;
return (sRet);
}

void Restraint::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
if ((iDspFlgs & DSP_BC)>0)
{
  Selectable=1;
  glColor3fv(cols[iColour]);
  C3dVector vCent;
  vCent=pObj->Get_Centroid();
  glRasterPos3f ((float) vCent.x,(float) vCent.y,(float) vCent.z);
  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPR);
  char S1[7];
  sprintf_s(S1,"%s",GetDofStr());
  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&S1[0]);
}
else
{
  Selectable=0;
}
}

void Restraint::OglDraw(int iDspFlgs,double dS1,double dS2)
{

OglDrawW(iDspFlgs,dS1,dS2);
}

//G_ObjectD Restraint::SelDist(CPoint InPT,Filter FIL)
//{
//CPoint cPt2;
//G_ObjectD Ret;
//
//cPt2.x = DSP_Point.x-InPT.x;
//cPt2.y = DSP_Point.y-InPT.y;
//Ret.Dist=(cPt2.x*cPt2.x+cPt2.y*cPt2.y);
//Ret.pObj=this;
//return (Ret);
//}

void Restraint::Info()
{
  char S1[80];
  CString OutT;
  sprintf_s(S1,"%s%i%s%i%s%i","Type ",iObjType,"; Label ",pObj->iLabel," Col; ",iColour);
  OutT+=S1;
  outtext1("RESTRAINT");
  outtext1(OutT); 
  sprintf_s(S1,"%s%s,","DOF STR: ",GetDofStr());
  OutT=S1;
  outtext1(OutT); 
}

CString Restraint::GetName()
{
	return ("Restraint (SPC)");
}

int Restraint::GetVarHeaders(CString sVar[])
{
	int iNo = 0;
	sVar[iNo] = "DOF 1 (0/1)";
	iNo++;
	sVar[iNo] = "DOF 2 (0/1)";
	iNo++;
	sVar[iNo] = "DOF 3 (0/1)";
	iNo++;
	sVar[iNo] = "DOF 4 (0/1)";
	iNo++;
	sVar[iNo] = "DOF 5 (0/1)";
	iNo++;
	sVar[iNo] = "DOF 6 (0/1)";
	iNo++;
	return(iNo);
}


int Restraint::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sVar[iNo] = "0";
	if (REST[0])
	  sVar[iNo] = "1";
	iNo++;
	sVar[iNo] = "0";
	if (REST[1])
		sVar[iNo] = "1";
	iNo++;
	sVar[iNo] = "0";
	if (REST[2])
		sVar[iNo] = "1";
	iNo++;
	sVar[iNo] = "0";
	if (REST[3])
		sVar[iNo] = "1";
	iNo++;
	sVar[iNo] = "0";
	if (REST[4])
		sVar[iNo] = "1";
	iNo++;
	sVar[iNo] = "0";
	if (REST[5])
		sVar[iNo] = "1";
	iNo++;
	return (iNo);
}

void Restraint::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{
	int iDF = atoi(sVar[0]);
	if (iDF != 1)
		REST[0] = FALSE;
	else
		REST[0] = TRUE;
	iDF = atoi(sVar[1]);
	if (iDF != 1)
		REST[1] = FALSE;
	else
		REST[1] = TRUE;
	iDF = atoi(sVar[2]);
	if (iDF != 1)
		REST[2] = FALSE;
	else
		REST[2] = TRUE;
	iDF = atoi(sVar[3]);
	if (iDF != 1)
		REST[3] = FALSE;
	else
		REST[3] = TRUE;
	iDF = atoi(sVar[4]);
	if (iDF != 1)
		REST[4] = FALSE;
	else
		REST[4] = TRUE;
	iDF = atoi(sVar[5]);
	if (iDF != 1)
		REST[5] = FALSE;
	else
		REST[5] = TRUE;
}

IMPLEMENT_DYNAMIC(ResultsVec, CObject)

ResultsVec::ResultsVec(int iD,C3dVector Pt, C3dVector Vec, int iC,int iDf, int iS, int iTyp)
{
	Point = Pt;
	Vector = Vec;
	iColour = iC;
	iObjType = 330;
	Selectable = 1;
	iLabel = iD;
	iDof = iD;
	iSign = iS;  //SIgn -/+ of the vector
	//Compute the a transform
	iType = iTyp;

}

void ResultsVec::Create(int iD, C3dVector Pt, C3dVector Vec, int iC)
{
	Point=Pt;
	Vector=Vec;
	iColour = iC;
	iObjType = 330;
	iLabel = iD;
	//Create Arrow Head

}

C3dVector ResultsVec::Get_Centroid()
{
	return(Point);
}

void ResultsVec::OglDraw(int iDspFlgs, double dS1, double dS2)
{

	OglDrawW(iDspFlgs, dS1, dS2);
}


void ResultsVec::DrawVector(int iDspFlgs,double dS1, double dS2,double dS, double dRF)
{
	int i;
	C3dVector p2;
	C3dVector vZ;
	vZ = Vector;
	vZ.Normalize();
	C3dMatrix mT;
	mT = vZ.GetTMat();
	C3dVector Pts[7];
	//Create Arrow Head
	for (i = 0; i < 7; i++)
	{
		Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
		Pts[i] = mT.Mult(Pts[i]);
		Pts[i] *= 0.2;
	}
	p2 = Vector;
	p2 *= dS * dRF / VecMaxMag();   //Normalise vector length
	p2 += Point;
	float fCol;
	double X, Y, Z;
	X = Pts[0].x;
	Y = Pts[0].y;
	Z = Pts[0].z;
	if (dRF < 0) //Need to reverse the arrow
	{
		X *= -1;
		Y *= -1;
		Z *= -1;
	}
	glColor3fv(cols[124]);  //Texture colours don't work without this
	fCol = GetContourColVec((float)iSign*Vector.Mag()*abs(dRF));
	glEnable(GL_TEXTURE_1D);
	glBegin(GL_LINES);
	glTexCoord1f(fCol);
	glVertex3f((float)Point.x, (float)Point.y, (float)Point.z);
	glTexCoord1f(fCol);
	glVertex3f((float)p2.x, (float)p2.y, (float)p2.z);
	glEnd();
	glLineWidth(1);
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
	glEnd();
	glDisable(GL_TEXTURE_1D);
	glLineWidth(2);
	if (((iDspFlgs & DSP_RESLAB) == 0))
	{
		char sLab[20];
		sprintf_s(sLab, "%g", iSign*Vector.Mag());
		OglString(iDspFlgs, Point.x, Point.y, Point.z, &sLab[0]);
	}
}

void ResultsVec::DrawTenVector(int iDspFlgs,double dS1, double dS2, double dS, double dRF)
{
	int i;
	C3dVector p2;  //Positive direction
	C3dVector p3;  //Negative direction
	C3dVector vZ;
	vZ = Vector;
	vZ.Normalize();
	C3dMatrix mT;
	mT = vZ.GetTMat();
	C3dVector Pts[7];
	//Create Arrow Head
	for (i = 0; i < 7; i++)
	{
		Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
		Pts[i] = mT.Mult(Pts[i]);
		Pts[i] *= 0.2;
	}
	p2 = Vector;
	p2 *= 0.5*dS * dRF / VecMaxMag();   //Normalise vector length
	p3 = p2;
	p3 *= -1; //Negative direction
	p2 += Point;
	p3 += Point;
	float fCol;
	double X, Y, Z;
	X = Pts[0].x * iSign;
	Y = Pts[0].y * iSign;
	Z = Pts[0].z * iSign;

	if (dRF < 0) //Need to reverse the arrow
	{
		X *= -1;
		Y *= -1;
		Z *= -1;
	}
	glColor3fv(cols[124]);  //Texture colours don't work without this
	fCol = GetContourColVec((float)iSign*Vector.Mag()*abs(dRF));
	glEnable(GL_TEXTURE_1D);
	glBegin(GL_LINES);
	glTexCoord1f(fCol);
	glVertex3f((float)Point.x, (float)Point.y, (float)Point.z);
	glTexCoord1f(fCol);
	glVertex3f((float)p2.x, (float)p2.y, (float)p2.z);

	glTexCoord1f(fCol);
	glVertex3f((float)Point.x, (float)Point.y, (float)Point.z);
	glTexCoord1f(fCol);
	glVertex3f((float)p3.x, (float)p3.y, (float)p3.z);
	glEnd();
	glLineWidth(1);
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p2.x, (float)Pts[2].y*dS1 + p2.y, (float)Pts[2].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p2.x, (float)Pts[3].y*dS1 + p2.y, (float)Pts[3].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p2.x, (float)Pts[4].y*dS1 + p2.y, (float)Pts[4].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p2.x, (float)Pts[5].y*dS1 + p2.y, (float)Pts[5].z*dS1 + p2.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p2.x, (float)Y*dS1 + p2.y, (float)Z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p2.x, (float)Pts[1].y*dS1 + p2.y, (float)Pts[1].z*dS1 + p2.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p2.x, (float)Pts[6].y*dS1 + p2.y, (float)Pts[6].z*dS1 + p2.z);
	glEnd();

	//SECOND ARROW HEAD
	//**************************************************************************************

	X *= -1;
	Y *= -1;
	Z *= -1;
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p3.x, (float)Pts[2].y*dS1 + p3.y, (float)Pts[2].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p3.x, (float)Pts[1].y*dS1 + p3.y, (float)Pts[1].z*dS1 + p3.z);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p3.x, (float)Pts[3].y*dS1 + p3.y, (float)Pts[3].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[2].x*dS1 + p3.x, (float)Pts[2].y*dS1 + p3.y, (float)Pts[2].z*dS1 + p3.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p3.x, (float)Pts[4].y*dS1 + p3.y, (float)Pts[4].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[3].x*dS1 + p3.x, (float)Pts[3].y*dS1 + p3.y, (float)Pts[3].z*dS1 + p3.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p3.x, (float)Pts[5].y*dS1 + p3.y, (float)Pts[5].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[4].x*dS1 + p3.x, (float)Pts[4].y*dS1 + p3.y, (float)Pts[4].z*dS1 + p3.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p3.x, (float)Pts[6].y*dS1 + p3.y, (float)Pts[6].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[5].x*dS1 + p3.x, (float)Pts[5].y*dS1 + p3.y, (float)Pts[5].z*dS1 + p3.z);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord1f(fCol);
	glVertex3f((float)X*dS1 + p3.x, (float)Y*dS1 + p3.y, (float)Z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[1].x*dS1 + p3.x, (float)Pts[1].y*dS1 + p3.y, (float)Pts[1].z*dS1 + p3.z);
	glTexCoord1f(fCol);
	glVertex3f((float)Pts[6].x*dS1 + p3.x, (float)Pts[6].y*dS1 + p3.y, (float)Pts[6].z*dS1 + p3.z);
	glEnd();


	glDisable(GL_TEXTURE_1D);
	glLineWidth(2);
	if (((iDspFlgs & DSP_RESLAB) == 0))
	{
		char sLab[20];
		sprintf_s(sLab, "%g", iSign*Vector.Mag());
		OglString(iDspFlgs, Point.x, Point.y, Point.z, &sLab[0]);
	}
}

void ResultsVec::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	ME_Object* ME = (ME_Object*) this->pParent;
	double dS = 1.0;
	double dRF = 1.0;
	if (ME != NULL)
	{
		dS = ME->dScaleVec;
		dRF = ME->dResFactor;
	}
	if ((iDspFlgs & DSP_VEC) == 0)
	{
		Selectable = 1;
		if (iType==1)
		  DrawVector(iDspFlgs,dS1, dS2, dS, dRF);
		else
		  DrawTenVector(iDspFlgs,dS1, dS2, dS, dRF);
	}
	else
	{
		Selectable = 0;
	}
}


void ResultsVec::Info()
{
	char S1[80];
	sprintf_s(S1, "LABEL %8i MAG %g",iLabel,Vector.Mag());
	outtext1(S1);
}






IMPLEMENT_DYNAMIC( CoordSys, CObject )
void CoordSys::Create(C3dVector Orig,C3dMatrix RMat,int inRID,int inTp, int iLab, int iC,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 12;
iColour = iC;
iLabel = iLab;
mOrientMat=RMat;
Origin = Orig;
dScl=1;
pParent=Parrent;
RID = inRID;
CysType = inTp;
}

C3dMatrix CoordSys::GetTMat()
{
return (mOrientMat);
}

G_Object* CoordSys::Copy(G_Object* Parrent)
{
CoordSys* cSys = new CoordSys;
cSys->Drawn = Drawn;
cSys->Selectable  = Selectable; 
cSys->Visable  = Visable;
cSys->iColour = iColour;
cSys->iObjType = iObjType;
cSys->iLabel = iLabel;
cSys->mOrientMat=mOrientMat;
cSys->Origin=Origin;
cSys->dScl=dScl;
cSys->RID=RID;
cSys->CysType=CysType;
cSys->pParent = Parrent;
return (cSys);
}

void CoordSys::ExportNAS(FILE* pFile)
{
C3dVector pB;
C3dVector pC;
CString sType;
CString sRID; 
if (CysType==3)
{
  sType="CORD2S  ";
}
else if (CysType==2)
{
  sType="CORD2C  ";
}
else
{
  sType="CORD2R  ";
}
pB.x	= mOrientMat.m_02;
pB.y  = mOrientMat.m_12;
pB.z  = mOrientMat.m_22;
pC.x	= mOrientMat.m_00;
pC.y  = mOrientMat.m_10;
pC.z  = mOrientMat.m_20;
pB+=Origin;
pC+=Origin;
if (RID = -1)
{
	RID = 0;
}
fprintf(pFile,"%8s%8i%8i%8s%8s%8s%8s%8s%8s\n",sType,iLabel, RID,e8(Origin.x),e8(Origin.y),e8(Origin.z),e8(pB.x),e8(pB.y),e8(pB.z));
fprintf(pFile,"%8s%8s%8s%8s\n","        ",e8(pC.x),e8(pC.y),e8(pC.z));
}

CString CoordSys::ToString()
{
C3dVector pB;
C3dVector pC;
CString sType;
if (CysType==3)
{
  sType="CORD2S  ";
}
else if (CysType==2)
{
  sType="CORD2C  ";
}
else
{
  sType="CORD2R  ";
}
pB.x  = mOrientMat.m_02;
pB.y  = mOrientMat.m_12;
pB.z  = mOrientMat.m_22;
pC.x  = mOrientMat.m_00;
pC.y  = mOrientMat.m_10;
pC.z  = mOrientMat.m_20;
pB+=Origin;
pC+=Origin;
int iRID=RID;
if (RID==-1)
{
  iRID=0;
}

char S1[200];
CString OutT;
sprintf_s(S1,"%8s%8i%8i%8s%8s%8s%8s%8s%8s\n%8s%8s%8s%8s\n",sType,iLabel,iRID,e8(Origin.x),e8(Origin.y),e8(Origin.z),e8(pB.x),e8(pB.y),e8(pB.z),"        ",e8(pC.x),e8(pC.y),e8(pC.z));
//sprintf_s(S1,"%8s%8s%8s%8s\n","        ",e8(pC.x),e8(pC.y),e8(pC.z));
OutT=S1;
return (OutT);
}

void CoordSys::Info()
{
  char S1[200];
  CString OutT;
  //G_Object::Info();
  sprintf_s(S1,"%i,%8.5f,%8.5f,%8.5f,,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f,%8.5f",
            iLabel,Origin.x,Origin.y,Origin.z,
            mOrientMat.m_00, mOrientMat.m_10, mOrientMat.m_20,
            mOrientMat.m_01, mOrientMat.m_11, mOrientMat.m_21,
            mOrientMat.m_02, mOrientMat.m_12, mOrientMat.m_22);
  OutT=S1;
  outtext1(OutT); 
}



C3dVector CoordSys::Get_Centroid() 
{

return (Origin);
}

void CoordSys::Translate (C3dVector vIn)
{
Origin.Translate(vIn);
}



void CoordSys::Serialize(CArchive& ar,int iV)
{

	if (ar.IsStoring())
	{
      G_Object::Serialize(ar,iV);
      mOrientMat.Serialize(ar,iV);
	  Origin.Serialize(ar,iV);
	  ar<<dScl;
	  ar<<RID;
	  ar<<CysType;
	}
	else
	{
	  G_Object::Serialize(ar,iV);
      mOrientMat.Serialize(ar,iV);
	  Origin.Serialize(ar,iV);
	  ar>>dScl;
	  ar>>RID;
	  ar>>CysType;
	}
}




void CoordSys::OglDrawW(int iDspFlgs,double dS1,double dS2)
{

C3dVector X;
C3dVector Y;
C3dVector Z;


if ((iDspFlgs & DSP_COORD)>0)
{
	Selectable=1;

	X.x = mOrientMat.m_00;
	X.y = mOrientMat.m_10;
	X.z = mOrientMat.m_20;

	Y.x = mOrientMat.m_01;
	Y.y = mOrientMat.m_11;
	Y.z = mOrientMat.m_21;

	Z.x = mOrientMat.m_02;
	Z.y = mOrientMat.m_12;
	Z.z = mOrientMat.m_22;

	X*=dS1;
	Y*=dS1;
	Z*=dS1;

	X+=Origin;
	Y+=Origin;
	Z+=Origin;

	glColor3fv(cols[GetCol()]);
	glBegin(GL_LINES);


	glVertex3f((float) Origin.x,(float) Origin.y,(float) Origin.z);
	glVertex3f((float) X.x,(float) X.y,(float) X.z);

	glVertex3f((float) Origin.x,(float) Origin.y,(float) Origin.z);
	glVertex3f((float) Y.x,(float) Y.y,(float) Y.z);

	glVertex3f((float) Origin.x,(float) Origin.y,(float) Origin.z);
	glVertex3f((float) Z.x,(float) Z.y,(float) Z.z);
	glEnd();

  if (iDspFlgs & DSP_BLACK)
  {
    glColor3fv(cols[124]);
  }
  else
  {
    glColor3fv(cols[0]);
  }
	if (CysType==2)
	{
	  glRasterPos3f ((float) X.x,(float) X.y,(float) X.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPR);
	  glRasterPos3f ((float) Y.x,(float) Y.y,(float) Y.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPT);
	  glRasterPos3f ((float) Z.x,(float) Z.y,(float) Z.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
	}
	else if (CysType==3)
	{
	  glRasterPos3f ((float) X.x,(float) X.y,(float) X.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPR);
	  glRasterPos3f ((float) Y.x,(float) Y.y,(float) Y.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPT);
	  glRasterPos3f ((float) Z.x,(float) Z.y,(float) Z.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPP);
	}
	else
	{
	  glRasterPos3f ((float) X.x,(float) X.y,(float) X.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPX);
	  glRasterPos3f ((float) Y.x,(float) Y.y,(float) Y.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPY);
	  glRasterPos3f ((float) Z.x,(float) Z.y,(float) Z.z);
	  glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
	}
	char sLab[20];
	C3dVector vCent;
	vCent=Get_Centroid();
	if (bDrawLab==TRUE)
	{
		sprintf_s(sLab,"Cys%i",iLabel);
		OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
}
else
{
   Selectable=0;
}
}


void CoordSys::OglDraw(int iDspFlgs,double dS1,double dS2)
{
OglDrawW(iDspFlgs,dS1,dS2);
}


//*****************************************************************
// BlowsR 14/07/2020
// Text Object added
// linked list of symbols
//*****************************************************************
IMPLEMENT_DYNAMIC(Text, CObject)

Text::Text()
{
	C3dVector inP;
	inP.Set(0, 0, 0);
	pParent = NULL;
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iObjType = 6;
	iLabel = -1;
	iColour = 100;
	pSyms = new cLinkedList();
	sText = "";
	if (inPt != NULL)
	{
		delete(inPt);
	}
	inPt = new CvPt_Object;
	inPt->Create(inP, 0, -1, 0, 0, 1, this);
	vNorm.Set(0,0,1);
}

Text::Text(int iLab,CString sT, double dH)
{
	C3dVector inP;
	inP.Set(0, 0, 0);
	pParent = NULL;
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iObjType = 6;
	iLabel = iLab;
	iColour = 100;
	pSyms = new cLinkedList();
	sText = "";
	dTextHeight=dH;
	sText=sT;
	if (inPt != NULL)
	{
		delete(inPt);
	}
	inPt = new CvPt_Object;
	inPt->Create(inP, 0, -1, 0, 0, 20, this);
	vNorm.Set(0, 0, 1);
}

Text::~Text()
{
	if (inPt != NULL)
		delete (inPt);
	if (pSyms != NULL)
		pSyms->DeleteAll();
}

void Text::OglDraw(int iDspFlgs, double dS1, double dS2)
{
	OglDrawW(iDspFlgs, dS1, dS2);
}

void Text::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	char sLab[20];
	C3dVector vC;
	Symbol* pS = NULL;
	if ((iDspFlgs & DSP_CURVES) > 0)
	{
		Selectable = 1;
		pS = (Symbol*)pSyms->Head;
		while (pS != NULL)
		{
			pS->Selectable = 1;
			pS = (Symbol*)pS->next;
		}
		glColor3fv(cols[GetCol()]);
		inPt->OglDrawW(iDspFlgs, dS1, dS2);
		pSyms->OglDrawW(iDspFlgs, dS1, dS2);

		if (bDrawLab == TRUE)
		{
			vC = this->Get_Centroid();
			sprintf_s(sLab, "Txt%i", iLabel);
			OglString(iDspFlgs, (float)vC.x, (float)vC.y, (float)vC.z, &sLab[0]);
		}
	}
	else
	{
		Selectable = 0;
		pS = (Symbol*)pSyms->Head;
		while (pS != NULL)
		{
			pS->Selectable = 0;
			pS = (Symbol*)pS->next;
		}
	}
}

G_ObjectD Text::SelDist(CPoint InPT, Filter FIL)
{
	G_ObjectD Ret;
	G_ObjectD dist;
	Ret.pObj = NULL;
	Ret.Dist = 1e36;;
	Symbol* pS = NULL;
	pS = (Symbol*) pSyms->Head;
	//  IF POINTS ARE SELECTABLE
	if (FIL.isFilter(0))
	{
			dist = inPt->SelDist(InPT, FIL);
			if (dist.Dist <= Ret.Dist)
			{
					Ret.Dist = dist.Dist;
					Ret.pObj = inPt;
			}
	}
	if (FIL.isFilter(6))
	{
		while (pS != NULL)
		{
			dist = pS->SelDist(InPT, FIL);
			if (dist.Dist < Ret.Dist)
			{
				Ret.Dist = dist.Dist;
				Ret.pObj = this;
			}
			pS = (Symbol*)pS->next;
		}
	}
	return(Ret);
}

void Text::S_Box(CPoint P1, CPoint P2, ObjList* pSel)
{
	G_Object::S_Box(P1, P2, pSel);
}


void Text::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
	Symbol* pS = NULL;
	pS = (Symbol*)pSyms->Head;
	while (pS != NULL)
	{
		pS->SetToScr(pModMat, pScrTran);
		pS = (Symbol*) pS->next;
	}
	inPt->SetToScr(pModMat, pScrTran);

	C3dVector vC;
	vC = Get_Centroid();
	vC.SetToScr(pModMat, pScrTran);
	SelPt = vC;
}

void Text::HighLight(CDC* pDC)
{

	Symbol* pS = NULL;
	pS = (Symbol*) pSyms->Head;
	while (pS != NULL)
	{
		pS->HighLight(pDC);
		pS = (Symbol*) pS->next;
	}
}

void Text::Transform(C3dMatrix TMat)
{

	inPt->Transform(TMat);
	vNorm = TMat * vNorm;
	Symbol* pS = NULL;
	pS = (Symbol*)pSyms->Head;
	while (pS != NULL)
	{
		pS->Transform(TMat);
		pS = (Symbol*)pS->next;
	}
}

void Text::Translate(C3dVector vIn)
{
	Symbol* pS = NULL;
	pS = (Symbol*)pSyms->Head;
	inPt->Translate(vIn);
	while (pS != NULL)
	{
		pS->Translate(vIn);
		pS = (Symbol*)pS->next;
	}
}

void Text::Move(C3dVector vM)
{
	Symbol* pS = NULL;
	pS = (Symbol*)pSyms->Head;
	while (pS != NULL)
	{
		pS->Move(vM);
		pS = (Symbol*)pS->next;
	}
}

void Text::Serialize(CArchive& ar, int iV)
{
	C3dVector v1;
	C3dVector v2;
	int i;
	int iNo;
	Link* pCL;
	Symbol* pSym = NULL;
	if (ar.IsStoring())
	{
		G_Object::Serialize(ar, iV);
		inPt->Serialize(ar, iV);				 //Insertion Point
		vNorm.Serialize(ar, iV); 			//Normal
		ar<<dTextHeight;					//Text Height
		ar<<sText;
		ar<<pSyms->iCnt;	 
		pSym = (Symbol*) pSyms->Head;
		while (pSym!=NULL)
		{
			pSym->Serialize(ar, iV);
			pSym = (Symbol*) pSym->next;
		}
	}
	else
	{
		G_Object::Serialize(ar, iV);
		inPt->Serialize(ar, iV);				 //Insertion Point
		vNorm.Serialize(ar, iV); 			//Normal
		ar >> dTextHeight;					//Text Height
		ar >> sText;
		ar >> iNo;
		pSym = (Symbol*)pSyms->Head;
		for (i=0;i<iNo;i++)
		{
			pSym = new Symbol();
			pSym->pParent = this;
			pSym->Serialize(ar, iV);
			pSyms->Add(pSym);
		}
	}
}

C3dVector Text::Get_Centroid()
{
	C3dVector vC;
	vC.Set(inPt->Pt_Point->x, inPt->Pt_Point->y, inPt->Pt_Point->z);
	return(vC);
}


G_Object* Text::Copy(G_Object* Parrent)
{
	Symbol* pS = NULL;
	Text* cText = new Text();
	cText->Drawn = Drawn;
	cText->Selectable = Selectable;
	cText->Visable = Visable;
	cText->iColour = iColour;
	cText->iObjType = iObjType;
	cText->iLabel = iLabel;
	cText->pParent = Parrent;
	//Specifics
	cText->inPt->Pt_Point->x = inPt->Pt_Point->x; 
	cText->inPt->Pt_Point->y = inPt->Pt_Point->y;
	cText->inPt->Pt_Point->z = inPt->Pt_Point->z;
	cText->inPt->iColour = inPt->iColour;
	cText->vNorm = vNorm;			
	cText->dTextHeight = dTextHeight;
	cText->sText;
			

	pS = (Symbol*) pSyms->Head;
	while (pS != NULL)
	{
		cText->pSyms->Add(pS->Copy(cText));
		pS = (Symbol*) pS->next;
	}
	
	return(cText);
}



//26/09/2016
//symbol class used for compounds of lines
// fonts, hatches etc

IMPLEMENT_DYNAMIC(Symbol , CObject )


Symbol::Symbol()
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 5;
iLabel = -1;
iColour = 2;
pParent = NULL;
pL=NULL;

inPt=NULL;
iSegs=0;
}

Symbol::~Symbol()
{
  if (inPt!=NULL)
    delete(inPt);
  if (vCent!=NULL)
    delete(vCent);

  Link* pNext;
  while (pL!=NULL) 
  {  
	pNext=pL->pNext;
    delete (pL);
    pL=pNext;
  } 
}

void Symbol::Create(int iLab,C3dVector inP,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 5;
iLabel = iLab;
iColour = 2;
pParent=Parrent;
if (inPt!=NULL)
  { delete(inPt);}
inPt=new CvPt_Object;
inPt->Create(inP,0,-1,0,0,1,this);
pL=NULL;
iSegs=0;
}

void Symbol::addSeg(C3dVector pt1,C3dVector pt2)
{

Link* pSeg = new Link(pt1.x,pt1.y,pt1.z,
					  pt2.x,pt2.y,pt2.z);

pSeg->pNext=pL;
pL=pSeg;
iSegs++;
}

void Symbol::OglDraw(int iDspFlgs,double dS1,double dS2)
{
  OglDrawW(iDspFlgs,dS1,dS2);
}

void Symbol::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[80];
if ((iDspFlgs & DSP_CURVES)>0)
{
  Selectable=1;
  Link* pCL;
  if (this->pParent!=NULL)
	  glColor3fv(cols[pParent->iColour]);
  else
      glColor3fv(cols[iColour]);
  C3dVector vPt;
  C3dVector vPt2;
  glLineWidth(4);
  glBegin(GL_LINES);
  pCL=pL;
    while (pCL!=NULL)
    {
      glVertex3f((float) pCL->p1->Pt_Point->x,(float) pCL->p1->Pt_Point->y,(float) pCL->p1->Pt_Point->z);
      glVertex3f((float) pCL->p2->Pt_Point->x,(float) pCL->p2->Pt_Point->y,(float) pCL->p2->Pt_Point->z);
	  pCL=pCL->pNext;
    }
  glEnd();
  glLineWidth(2.0);
  glDisable(GL_LINE_STIPPLE);
  C3dVector vCent=Get_Centroid();
  if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"C%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
}
else
{
  Selectable=0;
}
}

void Symbol::Draw(CDC* pDC,int iDrawmode)
{
  Link* pCL;
  pCL=pL;
  C3dVector p1;
  C3dVector p2;
  if (inPt!=NULL)
    inPt->Draw(pDC,iDrawmode);
  if (vCent!=NULL)
    vCent->Draw(pDC,iDrawmode);
  while (pCL!=NULL)
  {
    //Points at end of segement
    pDC->MoveTo((int) pCL->p1->DSP_Point->x,(int) pCL->p1->DSP_Point->y);
    pDC->LineTo((int) pCL->p2->DSP_Point->x,(int) pCL->p2->DSP_Point->y);
	pCL=pCL->pNext;
  }
}


void Symbol::CalculateMetrics()
{
//Calculate dimensions of symbol
Link* pCL=pL;
double dMinX = 0;double dMinY = 0;
double dMaxX = 0;double dMaxY = 0;
vCent=new CvPt_Object();

w=0;
h=0;
if (pCL!=NULL)
{
  dMinX=pCL->p1->Pt_Point->x;
  dMaxX=dMinX;
  dMinY=pCL->p1->Pt_Point->y;
  dMaxY=dMinY;
  while (pCL!=NULL)
  {
    if (pCL->p1->Pt_Point->x<dMinX)
	 dMinX=pCL->p1->Pt_Point->x;
    if (pCL->p2->Pt_Point->x<dMinX)
	 dMinX=pCL->p2->Pt_Point->x;
    if (pCL->p1->Pt_Point->x>dMaxX)
	 dMaxX=pCL->p1->Pt_Point->x;
    if (pCL->p2->Pt_Point->x>dMaxX)
	 dMaxX=pCL->p2->Pt_Point->x;

    if (pCL->p1->Pt_Point->y<dMinY)
	 dMinY=pCL->p1->Pt_Point->y;
    if (pCL->p2->Pt_Point->y<dMinY)
	 dMinY=pCL->p2->Pt_Point->y;
    if (pCL->p1->Pt_Point->y>dMaxY)
	 dMaxY=pCL->p1->Pt_Point->y;
    if (pCL->p2->Pt_Point->y>dMaxY)
	 dMaxY=pCL->p2->Pt_Point->y;
    pCL=pCL->pNext;
  }
}
w=dMaxX-dMinX;
h=dMaxY-dMinY;
vCent->Pt_Point->x=(w)/2;
vCent->Pt_Point->y=(h)/2;
vCent->Pt_Point->z=0;
//Bring to Origin
inPt->Pt_Point->Set(0, 0, 0);
pCL = pL;
  while (pCL != NULL)
  {
	  pCL->p1->Pt_Point->x -= dMinX;
	  pCL->p2->Pt_Point->x -= dMinX;
	  //pCL->p1->Pt_Point->y -= dMinY;
	  //pCL->p2->Pt_Point->y -= dMinY;
	  pCL = pCL->pNext;
  }

}


C3dVector Symbol::MinPt(C3dVector inPt)
{
  return (GetCoords());
}

void Symbol::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
  Link* pCL=pL;
  G_Object::SetToScr(pModMat,pScrTran);
  vCent->SetToScr(pModMat,pScrTran);
  inPt->SetToScr(pModMat,pScrTran);
  while (pCL!=NULL)  //Evaluate  each link coord to screen pixel coords
  {
    pCL->p1->SetToScr(pModMat,pScrTran);
    pCL->p2->SetToScr(pModMat,pScrTran);
    pCL=pCL->pNext;
  }
}

G_Object* Symbol::Copy(G_Object* Parrent)
{
Symbol* nSym =  new Symbol();
C3dVector p1;
C3dVector p2;
Link* pCL=pL;
nSym->Drawn = Drawn;
nSym->Selectable  = Selectable; 
nSym->Visable  = Visable;
nSym->iColour = iColour;
nSym->iObjType = iObjType;
nSym->iLabel = iLabel;

nSym->w=w;                  
nSym->h=h;                  

nSym->pL=NULL;
nSym->inPt=(CvPt_Object*) inPt->Copy(NULL);          
nSym->vCent=(CvPt_Object*) vCent->Copy(NULL);        

  while (pCL!=NULL)  //Evaluate  each link coord to screen pixel coords
  {
    p1.x=pCL->p1->Pt_Point->x;
    p1.y=pCL->p1->Pt_Point->y;
    p1.z=pCL->p1->Pt_Point->z;

	p2.x=pCL->p2->Pt_Point->x;
    p2.y=pCL->p2->Pt_Point->y;
    p2.z=pCL->p2->Pt_Point->z;
    nSym->addSeg(p1,p2);
    pCL=pCL->pNext;
  }
return(nSym);
}

void Symbol::HighLight(CDC* pDC)
{
this->Draw(pDC,4);
}

G_ObjectD Symbol::SelDist(CPoint InPT,Filter FIL)
{
	return(G_Object::SelDist(InPT,FIL));
}

void Symbol::Info()
{
  Link* pCL=pL;
  char S1[80];
  sprintf_s(S1,"SYM %8i",iLabel);
  outtext1(S1);
  sprintf_s(S1,"NSEGS %8i W %s H %s",iSegs,float8NAS(w),float8NAS(h));
  outtext1(S1);
  while (pCL!=NULL)  //Evaluate  each link coord to screen pixel coords
  {
    sprintf_s(S1,"%s %s %s",float8NAS(pCL->p1->Pt_Point->x),
	                        float8NAS(pCL->p1->Pt_Point->y),
			  			    float8NAS(pCL->p1->Pt_Point->z));
    outtext1(S1);
    sprintf_s(S1,"%s %s %s",float8NAS(pCL->p2->Pt_Point->x),
	                        float8NAS(pCL->p2->Pt_Point->y),
			  			    float8NAS(pCL->p2->Pt_Point->z));
    outtext1(S1);
    pCL=pCL->pNext;
  }

}

C3dVector Symbol::GetCoords() 
{
  C3dVector vRet;
  vRet.Set(vCent->Pt_Point->x,vCent->Pt_Point->y,vCent->Pt_Point->z);
  return (vRet);
}

C3dVector Symbol::Get_Centroid()
{
  return (GetCoords());
}


//*****************************************************************************************
// SYMBOLS FUNCTIONS
// 14/07/2020
//*****************************************************************************************
void Symbol::Translate(C3dVector vIn)
{
	Link* pCL = pL;
	vCent->Move(vIn);
	inPt->Move(vIn);
	pCL = pL;
	while (pCL != NULL)
	{
		pCL->p1->Move(vIn);
		pCL->p2->Move(vIn);
		pCL = pCL->pNext;
	}
}

void Symbol::Transform(C3dMatrix TMat)
{
	Link* pCL = pL;
	vCent->Transform(TMat);
	inPt->Transform(TMat);
	pCL = pL;
	while (pCL != NULL)
	{
		pCL->p1->Transform(TMat);
		pCL->p2->Transform(TMat);
		pCL = pCL->pNext;
	}
}


void Symbol::Move(C3dVector vM)
{

	Link* pCL = pL;
	vCent->Move(vM);
	inPt->Move(vM);
	pCL = pL;
	while (pCL != NULL)
	{
		pCL->p1->Move(vM);
		pCL->p2->Move(vM);
		pCL = pCL->pNext;
	}
}

void Symbol::Serialize(CArchive& ar, int iV)
{
	C3dVector v1;
	C3dVector v2;
	int i;
	int iNo;
	Link* pCL;
	if (ar.IsStoring())
	{
		G_Object::Serialize(ar, iV);
		inPt->Serialize(ar, iV);
		vCent->Serialize(ar, iV);;
		ar<<w;                   
		ar<<h;                   
		ar<<iSegs;
		pCL = pL;
		for (i = 0; i < iSegs; i++)
		{
			pCL->p1->Pt_Point->Serialize(ar, iV);
			pCL->p2->Pt_Point->Serialize(ar, iV);
			pCL = pCL->pNext;
		}
	}
	else
	{
		G_Object::Serialize(ar, iV);
		inPt = new CvPt_Object();
		vCent = new CvPt_Object();
		inPt->Serialize(ar, iV);
		vCent->Serialize(ar, iV);;
		ar >> w;
		ar >> h;
		ar >> iNo;
		for (i = 0; i < iNo; i++)
		{
			v1.Serialize(ar, iV);
			v2.Serialize(ar, iV);
			addSeg(v1, v2);
		}
	}
}



//*****************************************************************************************
// START OF BREP OBJECT DATA STRUCTURE USES
// 10/01/2020
//*****************************************************************************************
IMPLEMENT_DYNAMIC(USE, CObject)
USE::USE()
{
	iNo = 0;
	pObj = NULL;
}

BOOL USE::IsIn(G_Object* pUse)
{
	int i;
	for (i = 0; i < iNo; i++)
	{
		if (Use[i] == pUse)
		{
			return (TRUE);
		}
	}
	return (FALSE);
}

void USE::Add(G_Object* pUse)
{
	if (iNo < MAX_USES)
	{
		Use[iNo] = pUse;
		iNo++;
	}
	else
	{
		outtext1("ERROR: Max uses exceeded in class USE. ");
	}
}

void USE::AddEx(G_Object* pUse)
{
	if (iNo < MAX_USES)
	{
		if (!IsIn(pUse))
		{
			Use[iNo] = pUse;
			iNo++;
		}
	}
	else
	{
		outtext1("ERROR: Max uses exceeded in class USE. ");
	}
}


void USE::Remove(G_Object* pUse)
{
	int i;
	for (i = 0; i < iNo; i++)
	{
		if (Use[i] == pUse)
		{
			Use[i] = Use[iNo - 1];
			iNo--;
			break;
		}
	}
}

//*****************************************************************************************
// START OF BREP OBJECT DATA STRUCTURE FACE/SHELL/SOLID PART
// 10/01/2020
//*****************************************************************************************
IMPLEMENT_DYNAMIC(Face, CObject)
// Create Object

Face::Face()
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iObjType = 18;
	pParent = NULL;
	pSurf = NULL;		//pParent Surface
	bOrient = 1;		//Agrees with parent surface
}

Face::Face(NSurf* pS, BOOL bO)
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iObjType = 18;
	pParent = NULL;
	pSurf = pS;
	bOrient = bO;
}

Face::~Face()
{
	pSurf = NULL;
	//May have to delete pSurf if it was create as bOrient was = 0
	//need to come back to this
	pParent = NULL;
}

void Face::Serialize(CArchive& ar, int iV)
{
	G_Object::Serialize(ar, iV);
}

void Face::Info()
{
	char S1[80];
	sprintf_s(S1, "FACE ID: %i ORIENT: %i", iLabel, bOrient);
	outtext1(S1);
}

void Face::OglDraw(int iDspFlgs, double dS1, double dS2)
{
		if (pSurf != NULL)
			pSurf->OglDraw(iDspFlgs, dS1, dS2);
}

void Face::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
		if (pSurf != NULL)
			pSurf->OglDrawW(iDspFlgs, dS1, dS2);
}

void Face::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
	//if (pSurf != NULL)
	//	pSurf->SetToScr(pModMat, pScrTran);
}

void Face::HighLight(CDC* pDC)
{
	if (pSurf != NULL)
		pSurf->HighLight(pDC);
}

G_ObjectD Face::SelDist(CPoint InPT, Filter FIL)
{
	//Not sure about the behaviout of this yest
	G_ObjectD Ret;
	double dDist= 1e36;
	Ret.Dist = 1e36;
	Ret.pObj = NULL;
	if ((FIL.isFilter(18)) || (FIL.isFilter(19)) || (FIL.isFilter(20)))
	{
		if (pSurf != NULL)
		{
			Ret = pSurf->SelDistFace(InPT, FIL);
			Ret.pObj = this;
		}
			
	}
	return (Ret);
}

void Face::S_Box(CPoint P1, CPoint P2, ObjList* pSel)
{
	if (pSurf != NULL)
		pSurf->S_Box(P1, P2, pSel);
}

void Face::Translate(C3dVector vTVect)
{
	if (pSurf != NULL)
		pSurf->Translate(vTVect);
}

void Face::Transform(C3dMatrix TMat)
{
	if (pSurf != NULL)
		pSurf->Transform(TMat);
}

C3dVector Face::GetPt(double dU, double dV)
{
	C3dVector vRet;
	vRet.Set(0, 0, 0);
	if (pSurf != NULL)
		vRet = pSurf->GetPt(dU, dV);
	return (vRet);
}

C3dVector Face::Get_Normal(double dU, double dV)
{
	C3dVector vRet;
	vRet.Set(0, 0, 0);
	if (pSurf != NULL)
		vRet = pSurf->Get_Normal(dU, dV);
	return (vRet);
}

C3dVector Face::Get_Centroid()
{
	C3dVector vRet;
	vRet.Set(0, 0, 0);
	if (pSurf != NULL)
		vRet = pSurf->Get_Centroid();
	return (vRet);
}

void Face::RelTo(G_Object* pThis, ObjList* pList, int iType)
{

}

//*****************************************************************************************
// START OF BREP OBJECT DATA STRUCTURE THE S H E L L
// 10/01/2020
//*****************************************************************************************
IMPLEMENT_DYNAMIC(Shell, CObject)
Shell::Shell()
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iObjType = 19;
	pParent = NULL;		//This will be the parent Part object
	bOrient = 1;		//Agrees with parent surface
	bOrient = TRUE;		//True = Outer shell False = void		
	isOuter = TRUE;		//If TRUE is the outer shell
}

Shell::Shell(BOOL isOut, int iLab)
{
	Shell();
	isOuter = isOut;
	iLabel = iLab;
}


G_ObjectD Shell::SelDist(CPoint InPT, Filter FIL)
{
	G_ObjectD Ret;
	G_ObjectD D;
	double MinDist = 1e36;
	Ret.Dist = 1e36;
	Ret.pObj = NULL;
	Ret.Z = 1e36;
	Face* pNext;
	pNext = (Face*) pFaces.Head;
	while (pNext != NULL)  //Search All Faces in Shell
	{
		D = pNext->SelDist(InPT, FIL);
		if (D.Dist < MinDist)
		{
			MinDist= D.Dist;
			Ret.Dist = D.Dist;
			Ret.Z = D.Z;
			Ret.pObj = (Face*) pNext;
		}
		pNext = (Face*) pNext->next;
	}
	if (!FIL.isFilter(18)) //Faces is not required
		Ret.pObj = this;
	return (Ret);
}

void Shell::HighLight(CDC* pDC)
{
	G_Object* pNext;
	pNext = pFaces.Head;
	while (pNext != NULL)
	{
		pNext->HighLight(pDC);
		pNext = (G_Object*)pNext->next;
	}
}

void Shell::RelTo(G_Object* pThis, ObjList* pList, int iType)
{
	//Surface related to face
	if (pThis->iObjType == 18)
	{
		Face* pF = (Face*)pThis;
		if ((iType == 15) || (iType == 16) || (iType == 17))
		{
			if (pF->pSurf != NULL)
				pList->AddEx(pF->pSurf);
		}
		else if (iType == 7)
		{
			if (pF->pSurf != NULL)
				pF->pSurf->RelTo(pThis, pList, iType);
		}
		else if (iType == 19)  //Shell
		{
			if (pThis->iObjType == 18)
			{
				pList->AddEx(this);
			}
		}

	}
	else if (pThis->iObjType == 19) //Faces Related To Shell
	{
		Face* pNext = (Face*) pFaces.Head;
		while (pNext != NULL)
		{
			pList->AddEx(pNext);
			pNext = (Face*)pNext->next;
		}
	}
	else if (pThis->iObjType == 7) //
	{
		pThis->iObjType == 7;
	}

}

void Shell::Info()
{

	char S1[80];
	USE* pU;
	Face* pF;
	pF = (Face*) pFaces.Head;
	outtext1("************ FACES IN SHELL **************");
	while (pF != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i", pF->iObjType, pF->iLabel);
		outtext1(S1);
		pF = (Face*) pF->next;
	}
	outtext1("************* POINT USES ****************");
	pU = (USE*) PtUSES.Head;
	while (pU != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i USES %i", pU->pObj->iObjType, pU->pObj->iLabel, pU->iNo);
		outtext1(S1);
		pU = (USE*) pU->next;
	}
	outtext1("************* CURVE USES ****************");
	pU = (USE*)CurveUSES.Head;
	while (pU != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i USES %i", pU->pObj->iObjType, pU->pObj->iLabel, pU->iNo);
		outtext1(S1);
		pU = (USE*) pU->next;
	}
}

void Shell::OglDraw(int iDspFlgs, double dS1, double dS2)
{

	G_Object* pNext;
	pNext = pFaces.Head;
	while (pNext != NULL)
	{
		pNext->OglDraw(iDspFlgs, dS1, dS2);
		pNext = (G_Object*)pNext->next;
	}
}

void Shell::OglDrawW(int iDspFlgs, double dS1, double dS2)
{

}

Shell::~Shell()
{
	pFaces.DeleteAll();		//Faces used in shell
	cLinkedList CurveUSES;  // Curves uses in shells
	USE* pNext;
	pNext = (USE*) CurveUSES.Head;
	while (pNext != NULL)
	{
		pNext->iNo = 0;;
		pNext->pObj = NULL;
		pNext = (USE*) pNext->next;
	}
	//cLinkedList PtUSES;		// Points uses in shells
}

void Shell::Serialize(CArchive& ar, int iV, Part* pP)
{
	int i;
	int j;
	int iCnt;
	int iFLab;
	Face* pNext;
	USE* pU;
	if (ar.IsStoring())
	{
		G_Object::Serialize(ar, iV);
		ar << bOrient;			//Orientatopn of shell wet underlying surfaces TRUE=Agree
		ar << isOuter;
		//All Faces
		ar << pFaces.iCnt;
		ar << pFaces.sTitle;
		pNext = (Face*)pFaces.Head;
		while (pNext != NULL)
		{
			pNext->Serialize(ar, iV);
			ar << pNext->bOrient;
			ar << pNext->pSurf->iLabel;
			pNext = (Face*)pNext->next;
		}
		//Point Uses
		ar << PtUSES.iCnt;
		ar << PtUSES.sTitle;
		pU = (USE*) PtUSES.Head;
		while (pU != NULL)
		{
			pU->Serialize(ar, iV);
			ar << pU->iNo;
			ar << pU->pObj->iLabel;					//The Space Curve
			for (i = 0; i < pU->iNo; i++)
			{
				ar << pU->Use[i]->iLabel;			//The Face IDs its usd in
			}
			pU = (USE*) pU->next;
		}
		//Curve Uses
		ar << CurveUSES.iCnt;
		ar << CurveUSES.sTitle;
		pU = (USE*) CurveUSES.Head;
		while (pU != NULL)
		{
			pU->Serialize(ar, iV);
			ar << pU->iNo;
			ar << pU->pObj->iLabel;					//The Space Curve
			for (i = 0; i < pU->iNo; i++)
			{
				ar << pU->Use[i]->iLabel;			//The Face IDs its usd in
			}
			pU = (USE*) pU->next;
		}
	}
	else
	{
		G_Object::Serialize(ar, iV);
		ar >> bOrient;			//Orientatopn of shell wet underlying surfaces TRUE=Agree
		ar >> isOuter;
		//All Faces
		ar >> iCnt;
		ar >> pFaces.sTitle;
		//All Faces
		for (i = 0; i < iCnt; i++)
		{
			Face* pF = new Face();
			pF->Serialize(ar, iV);
			pF->pParent = this;
			ar >> pF->bOrient;
			ar >> iFLab;
			pF->pSurf = (NSurf*) pP->pPartS.Get(iFLab);
			pFaces.Add(pF);
		}
		//Point Uses
		ar >> iCnt;
		ar >> PtUSES.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			int iPLab;
			int iC;
			pU = new USE();
			pU->Serialize(ar, iV);
			ar >> pU->iNo;
			ar >> iPLab;
			pU->pObj = (CvPt_Object* ) pP->pPartV.Get(iPLab);			//The Space Curve
			for (j = 0; j < pU->iNo; j++)
			{
				ar >> iC;
				pU->Use[j] = (NCurve*) pP->pPartC.Get(iC); 			//The Face IDs its usd in
			}
			PtUSES.Add(pU);
		}
		//Curve Uses
		ar >> iCnt;
		ar >> CurveUSES.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			int iCLab;
			int iF;
			pU = new USE();
			pU->Serialize(ar, iV);
			ar >> pU->iNo;
			ar >> iCLab;
			pU->pObj = (NCurve*) pP->pPartC.Get(iCLab);			//The Space Curve
			for (j = 0; j < pU->iNo; j++)
			{
				ar >> iF;
				pU->Use[j]= (Face*) pFaces.Get(iF); ;			//The Face IDs its usd in
			}
			CurveUSES.Add(pU);
		}
	}
}


Face* Shell::GetFace(G_Object* pF)
{
	Face* pRet = NULL;
	Face* pFNext;
	pFNext = (Face*) this->pFaces.Head;
	while (pFNext != NULL)
	{
		if (pFNext == pF)
		{
			pRet = pFNext;
			break;
		}
		pFNext = (Face*)pFNext->next;
	}
	return (pRet);
}

void Shell::DeleteFace(G_Object* pF)
{
	pFaces.Remove2(pF);
}

void Shell::AddFace(Face* pF, BOOL bO, int iLab)
{
	CvPt_Object* pS = NULL;					//Start Vertex		
	CvPt_Object* pE = NULL;					//End Vertex
	pF->bOrient = bO;
	pF->pParent = this;
	pF->iLabel = iLab;
	pFaces.Add(pF);
	int i;
	int j;
	USE* pU;
	NSurf* pSurface;
	NCurve* pSC;
	pSurface = (NSurf *)pF->pSurf;
	//Now need to add related space curves
	for (i = 0; i < pSurface->iNoExtCvs; i++)
	{
		pSC = pSurface->pExtLoop[i]->pSC;
		if (pSC != NULL)
		{
			pS = pSC->pS;
			if (pS->iLabel == 1)
				pS->iLabel = 1;
			if (pS != NULL)
			{
				pU = (USE*) PtUSES.Get(pS->iLabel);
				if (pU == NULL)
				{
					pU = new USE();
					PtUSES.Add(pU);
					pU->iLabel = pS->iLabel;
					pU->pObj = pS;							//The underlying space curve	
					pU->AddEx(pSC);
				}
				else
				{
					pU->AddEx(pSC);
				}
			}
			pE = pSC->pE;
			if (pE->iLabel == 1)
				pE->iLabel = 1;
			if (pE != NULL)
			{
				pU = (USE*)PtUSES.Get(pE->iLabel);
				if (pU == NULL)
				{
					pU = new USE();
					PtUSES.Add(pU);
					pU->iLabel = pE->iLabel;
					pU->pObj = pE;							//The underlying space curve	
					pU->AddEx(pSC);
				}
				else
				{
					pU->AddEx(pSC);
				}
			}
			pU = (USE*)CurveUSES.Get(pSC->iLabel);
			if (pU == NULL)
			{
				pU = new USE();
				CurveUSES.Add(pU);
				pU->iLabel = pSC->iLabel;
				pU->pObj = pSC;							//The underlying space curve	
				pU->Add(pF);		                    //Curve on surface usage
			}
			else
			{
				pU->Add(pF);
			}
		}
	}
	for (i = 0; i < pSurface->iNoIntLoops; i++)
	{
		for (j = 0; j < pSurface->iNoIntCvs[i]; j++)
		{
			pSC = pSurface->pIntLoop[i][j]->pSC;
			if (pSC != NULL)
			{
				pS = pSC->pS;
				if (pS->iLabel == 1)
					pS->iLabel = 1;
				if (pS != NULL)
				{
					pU = (USE*)PtUSES.Get(pS->iLabel);
					if (pU == NULL)
					{
						pU = new USE();
						PtUSES.Add(pU);
						pU->iLabel = pS->iLabel;
						pU->pObj = pS;							//The underlying space curve	
						pU->AddEx(pSC);
					}
					else
					{
						pU->AddEx(pSC);
					}
				}
				pE = pSC->pE;
				if (pE->iLabel == 1)
					pE->iLabel = 1;
				if (pE != NULL)
				{
					pU = (USE*)PtUSES.Get(pE->iLabel);
					if (pU == NULL)
					{
						pU = new USE();
						PtUSES.Add(pU);
						pU->iLabel = pE->iLabel;
						pU->pObj = pE;							//The underlying space curve	
						pU->AddEx(pSC);
					}
					else
					{
						pU->AddEx(pSC);
					}
				}
				pU = (USE*)CurveUSES.Get(pSC->iLabel);
				if (pU == NULL)
				{
					pU = new USE();
					CurveUSES.Add(pU);
					pU->iLabel = pSC->iLabel;
					pU->pObj = pSC;							//The underlying space curve	
					pU->Add(pF);		//Curve on surface usage
				}
				else
				{
					pU->Add(pF);
				}
			}
		}
	}

}

//Remove Curve Face uses and Pt Curve Uses
void  Shell::RemoveFaceUses(Face* pF)
{
	int i;
	int j;
	NSurf* pSurface;
	NCurve* pSC;
	pSurface = (NSurf *) pF->pSurf;
	USE* pU;
	if (pF == NULL)
		return;
	for (i = 0; i < pSurface->iNoExtCvs; i++)
	{
		pSC = pSurface->pExtLoop[i]->pSC;
		//Remove Curves From Pts Uses
		if (pSC != NULL)
		{
			pU = (USE*) CurveUSES.Get(pSC->iLabel);
			if (pU != NULL)
				pU->Remove(pF);
			if (pU->iNo == 0)
				CurveUSES.Remove2(pU);
		}
	}
	for (i = 0; i < pSurface->iNoIntLoops; i++)
	{
		for (j = 0; j < pSurface->iNoIntCvs[i]; j++)
		{
			pSC = pSurface->pIntLoop[i][j]->pSC;
			//Remove Curves From Pts Uses
			if (pSC != NULL)
			{
				pU = (USE*) CurveUSES.Get(pSC->iLabel);
				if (pU != NULL)
					pU->Remove(pF);
				if (pU->iNo == 0)
					CurveUSES.Remove2(pU);
			}
		}
	}
}

//Remove Curve uses from PtUSES
void  Shell::RemoveCurveUses(NCurve* pSC)
{
	CvPt_Object* pS = NULL;					//Start Vertex		
	CvPt_Object* pE = NULL;					//End Vertex
	USE* pU;
	if (pSC != NULL)
	{
		pS = pSC->pS;
		if (pS != NULL)
		{
			pU = (USE*) PtUSES.Get(pS->iLabel);
			if (pU != NULL)
			{
				pU->Remove(pSC);
				if (pU->iNo == 0)
					PtUSES.Remove2(pU);
			}
		}
		pE = pSC->pE;
		if (pE != NULL)
		{
			pU = (USE*)PtUSES.Get(pE->iLabel);
			if (pU != NULL)
				pU->Remove(pSC);
			if (pU->iNo == 0)
				PtUSES.Remove2(pU);
		}
	}
}

//*****************************************************************************************
// START OF BREP OBJECT DATA STRUCTURE THE P A R T  O B J E C T
// 10/01/2020
//*****************************************************************************************
IMPLEMENT_DYNAMIC(Part, CObject)

Part::Part()
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iColour = 13;
	iLabel = -1;
	iObjType = 20;
	iShellLab=1;
	iVertexLab = 1;
	iCurveLab=1;
	iFaceLab=1;
	iSurfLab=1;
}

Part::~Part()
{
	pShells.DeleteAll();	//Pointer to all Shells forming part
	FaceUSES.DeleteAll();	//Face USES in shells
	pPartS.DeleteAll();		//Space Surfaces used in part
	pPartC.DeleteAll();		//Space curves used in part
	pPartV.DeleteAll();		//Vertices used in Part
		
}

Part::Part(int iLab)
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iColour = 13;
	iObjType = 20;
	iShellLab = 1;
	iVertexLab = 1;
	iCurveLab = 1;
	iFaceLab = 1;
	iSurfLab = 1;
	iLabel = iLab;
}

void Part::Serialize(CArchive& ar, int iV)
{
	int iT;
	int iST;
	int i;
	int iCnt;
	G_Object* pNext;
	Shell* pS;
	NCurve* pCv;
	NSurf* pSf;
	if (ar.IsStoring())
	{
		G_Object::Serialize(ar, iV);
		ar << Alpha;
		ar << sName;
		ar << iShellLab;
		ar << iVertexLab;
		ar << iCurveLab;
		ar << iFaceLab;
		ar << iSurfLab;
		//All verticese
		ar << pPartV.iCnt;
		ar << pPartV.sTitle;
		pNext = pPartV.Head;
		while (pNext != NULL)
		{
			pNext->Serialize(ar, iV);
			pNext = pNext->next;
		}
		//All Curves
		ar << pPartC.iCnt;
		ar << pPartC.sTitle;
		pNext = pPartC.Head;
		while (pNext != NULL)
		{
			ar << pNext->iObjType;
			ar << pNext->iType;
			pNext->Serialize(ar, iV);
			pNext = pNext->next;
		}
		//All Surfaces
		ar << pPartS.iCnt;
		ar << pPartS.sTitle;
		pNext = pPartS.Head;
		while (pNext != NULL)
		{
			ar << pNext->iObjType;
			ar << pNext->iType;
			pNext->Serialize(ar, iV);
			pNext = pNext->next;
		}
		
		//All Shells
		ar << pShells.iCnt;
		ar << pShells.sTitle;
		pS = (Shell*) pShells.Head;
		while (pS != NULL)
		{
			pS->Serialize(ar, iV,this);
			pS = (Shell*) pS->next;
		}
	}
	else
	{
		G_Object::Serialize(ar, iV);
		ar >> Alpha;
		ar >> sName;
		ar >> iShellLab;
		ar >> iVertexLab;
		ar >> iCurveLab;
		ar >> iFaceLab;
		ar >> iSurfLab;
		//All verticese
		ar >> iCnt;
		ar >> pPartV.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			CvPt_Object* pCv = new CvPt_Object();
			pCv->pParent = this;
			pCv->Serialize(ar, iV);
			pPartV.Add(pCv);
		}
		//All Curves
		ar >> iCnt;
		ar >> pPartC.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			ar >> iT;
			ar >> iST;
			if (iST == 3)
				pCv = new NCircle;
			else if (iST == 2)
				pCv = new NLine;
			else
				pCv = new NCurve;
			pCv->pParent = this;
			pCv->Serialize(ar, iV);
			pPartC.Add(pCv);
		}
		//All Surfaces
		ar >> iCnt;
		ar >> pPartS.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			ar >> iT;
			ar >> iST;
			if (iST == 3)
				pSf = new NSurfR;
			else if (iST == 2)
				pSf = new NSurfE;
			else
				pSf = new NSurf;
			pSf->pParent = this;
			pSf->Serialize(ar, iV);
			pPartS.Add(pSf);
		}
		ReConectCvEndPts(0.000001);
		//All Shells
		ar >> iCnt;
		ar >> pShells.sTitle;
		for (i = 0; i < iCnt; i++)
		{
			Shell* pSh = new Shell();
			pSh->Serialize(ar, iV,this);
			pSh->pParent = this;
			pShells.Add(pSh);
		}
	}
}


void Part::AddShell(Shell* pShell)
{
	pShell->pParent = this;		//The shell belongs to this part
	pShell->iLabel = iShellLab;
	iShellLab++;
	pShells.Add(pShell);


}

void Part::ReConectCvEndPts(double dTol)
{
	NCurve* pNext;
	pNext = (NCurve*) pPartC.Head;
	while (pNext != NULL)
	{
		pNext->pS = GetVert(pNext->vS, dTol);
		pNext->pE = GetVert(pNext->vE, dTol);
		pNext = (NCurve*)pNext->next;
	}
}

CvPt_Object* Part::GetVert(C3dVector pPt, double dTol)
{
	CvPt_Object* pRet = NULL;
	double dDist;
	CvPt_Object* pNext;
	pNext = (CvPt_Object*)pPartV.Head;
	while (pNext != NULL)
	{
		dDist = pNext->Pt_Point->Dist(pPt);
		if (dDist < dTol)
		{
			pRet = pNext;
			break;
		}
		pNext = (CvPt_Object*)pNext->next;
	}
	if (pRet == NULL)
	{
		CvPt_Object* pNewP = new CvPt_Object();
		pNewP->Create(pPt, 1.0, iVertexLab, -1, -1, 4, this);
		iVertexLab++;
		pPartV.Add(pNewP);
		pRet = pNewP;
	}

	return (pRet);
}

void Part::AddSurf(Face* pF)
{
	//Need to exclusively add Face, trim cureve and points
	int i;
	int j;
	G_Object* pS;
	NSurf* pSurface;
	NCurve* pSC;
	pS = pPartS.GetByPtr(pF->pSurf); //Add the underlying trimmed surface
	if (pS == NULL)
	{
		pSurface = (NSurf *)pF->pSurf;
		pSurface->iLabel = iSurfLab; //we will rely on ID so need to be careful to preserve integrity
		pSurface->pParent = this;
		iSurfLab++;
		pPartS.Add(pSurface);
		//Now need to add related space curves
		
		for (i = 0; i < pSurface->iNoExtCvs; i++)
		{
			pSC = pSurface->pExtLoop[i]->pSC;
			if (pSC != NULL)
			{
				if (pPartC.GetByPtr(pSC) == NULL)
				{
					pSC->iLabel = iCurveLab;
					pSC->pParent = this;
					iCurveLab++;
					pPartC.Add(pSC);
					//Add The End Points
					pSC->pS = GetVert(pSC->vS, 0.000001);
					pSC->pE = GetVert(pSC->vE, 0.000001);
				}
			}
		}
		for (i = 0; i < pSurface->iNoIntLoops; i++)
		{
			for (j=0;j< pSurface->iNoIntCvs[i];j++)
			{
				pSC = pSurface->pIntLoop[i][j]->pSC;
				if (pSC != NULL)
				{
					if (pPartC.GetByPtr(pSC) == NULL)
					{
						pSC->pParent = this;
						pSC->iLabel = iCurveLab;
						iCurveLab++;
						pPartC.Add(pSC);
						//Add The End Points
						pSC->pS = GetVert(pSC->vS, 0.000001);
						pSC->pE = GetVert(pSC->vE, 0.000001);
					}
				}
			}
		}

	}
}

void Part::CurveColour(int iCol)
{
	G_Object* pNext;
	pNext = pPartC.Head;
	while (pNext != NULL)
	{
		pNext->iColour = iCol;
		pNext = (G_Object*)pNext->next;
	}
}

void Part::SurfColour(int iCol)
{
	G_Object* pNext;
	pNext = pPartS.Head;
	while (pNext != NULL)
	{
		pNext->iColour = iCol;
		pNext = (G_Object*)pNext->next;
	}
}

void Part::OglDraw(int iDspFlgs, double dS1, double dS2)
{
	Shell* pShell;
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	iShellLab;
	pShell = (Shell*) pShells.Head;
	while (pShell != NULL)
	{
		pShell->OglDraw(iDspFlgs, dS1, dS2);
		pShell = (Shell*) pShell->next;
	}
	glDisable(GL_BLEND);
}

void Part::OglDrawW(int iDspFlgs, double dS1, double dS2)
{
	G_Object* pNext;
	pNext = pPartC.Head;
	while (pNext != NULL)
	{
		pNext->OglDrawW(iDspFlgs, dS1, dS2);
		pNext = (G_Object*)pNext->next;
	}
	pNext = pPartV.Head;
	while (pNext != NULL)
	{
		pNext->OglDrawW(iDspFlgs, dS1, dS2);
		pNext = (G_Object*)pNext->next;
	}
}

void Part::GetBoundingBox(C3dVector& vll, C3dVector& vur)
{
	int i;
	NCurve* pSC;
	C3dVector vMinXYZ;
	C3dVector vMaxXYZ;
	G_Object* pNext;
	pNext = pPartC.Head;
	BOOL bFirst = TRUE;
	C3dVector vPt;
	while (pNext != NULL)
	{
		pSC =(NCurve*) pNext;
		for (i = 0; i < pSC->iNoCPts; i++)
		{
			vPt = pSC->cPts[i]->Pt_Point;
			if (bFirst)
			{
				vMinXYZ = vPt;
				vMaxXYZ = vPt;
				bFirst = FALSE;
			}
			else
			{
				if (vPt.x < vMinXYZ.x)
					vMinXYZ.x = vPt.x;
				if (vPt.y < vMinXYZ.y)
					vMinXYZ.y = vPt.y;
				if (vPt.z < vMinXYZ.z)
					vMinXYZ.z = vPt.z;

				if (vPt.x > vMaxXYZ.x)
					vMaxXYZ.x = vPt.x;
				if (vPt.y > vMaxXYZ.y)
					vMaxXYZ.y = vPt.y;
				if (vPt.z > vMaxXYZ.z)
					vMaxXYZ.z = vPt.z;
			}
		}
		pNext = (G_Object*)pNext->next;
	}
	vll = vMinXYZ;
	vur = vMaxXYZ;
	BBox[0].Pt_Point->Set(vMinXYZ.x, vMinXYZ.y, vMinXYZ.z);
	BBox[1].Pt_Point->Set(vMinXYZ.x, vMaxXYZ.y, vMinXYZ.z);
	BBox[2].Pt_Point->Set(vMaxXYZ.x, vMaxXYZ.y, vMinXYZ.z);
	BBox[3].Pt_Point->Set(vMaxXYZ.x, vMinXYZ.y, vMinXYZ.z);

	BBox[4].Pt_Point->Set(vMinXYZ.x, vMinXYZ.y, vMaxXYZ.z);
	BBox[5].Pt_Point->Set(vMinXYZ.x, vMaxXYZ.y, vMaxXYZ.z);
	BBox[6].Pt_Point->Set(vMaxXYZ.x, vMaxXYZ.y, vMaxXYZ.z);
	BBox[7].Pt_Point->Set(vMaxXYZ.x, vMinXYZ.y, vMaxXYZ.z);

}

void Part::SetToScr(C3dMatrix* pModMat, C3dMatrix* pScrTran)
{
	GetBoundingBox(vBoxMin, vBoxMax);
	C3dVector V;
	C3dVector R;
	G_Object::SetToScr(pModMat, pScrTran);
	int i;
	for (i=0;i<8;i++)
		BBox[i].SetToScr(pModMat, pScrTran);

	CvPt_Object* pNext;
	pNext = (CvPt_Object*)pPartV.Head;
	while (pNext != NULL)
	{
		pNext->SetToScr(pModMat, pScrTran);
		pNext = (CvPt_Object*)pNext->next;
	}
	NCurve* pNextC;
	pNextC = (NCurve*) pPartC.Head;
	while (pNextC != NULL)
	{
		pNextC->SetToScr(pModMat, pScrTran);
		pNextC = (NCurve*)pNextC->next;
	}
	NSurf* pNextS;
	pNextS = (NSurf*)pPartS.Head;
	while (pNextS != NULL)
	{
		pNextS->SetToScr(pModMat, pScrTran);
		pNextS = (NSurf*)pNextS->next;
	}

}

void Part::HighLight(CDC* pDC)
{
	int i=0;
	pDC->MoveTo((int) BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	for (i=1;i<4;i++)
		pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 0;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);

	i = 4;
	pDC->MoveTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	for (i = 5; i < 8; i++)
		pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 4;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);

	i = 0;
	pDC->MoveTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 4;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 1;
	pDC->MoveTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 5;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 2;
	pDC->MoveTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 6;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 3;
	pDC->MoveTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
	i = 7;
	pDC->LineTo((int)BBox[i].DSP_Point->x, (int)BBox[i].DSP_Point->y);
}

G_ObjectD Part::SelDist(CPoint InPT, Filter FIL)
{
	char S1[80] = "";
	
	G_ObjectD Ret;
	G_ObjectD D;
	Ret.Dist = 1e36;
	Ret.pObj = NULL;
	Ret.Z = 1e36;
	Shell* pShell;
	iShellLab;
	CvPt_Object* pV;
	NCurve* pC;
	NSurf* pS;
	pShell = (Shell*)pShells.Head;
	while (pShell != NULL)
	{
		D = pShell->SelDist(InPT, FIL);
		if (D.Dist < Ret.Dist)
		{
			Ret.Dist = D.Dist;
			Ret.Z = D.Z;
			Ret.pObj = D.pObj;
		}
		pShell = (Shell*) pShell->next;
	}
	//sprintf_s(S1, "%s %i", "Z:", Ret.Z);
	//outtext1(S1);

	if (FIL.isFilter(0))
	{
		pV = (CvPt_Object*) pPartV.Head;
		while (pV != NULL)
		{
			D = pV->SelDist(InPT, FIL);
			if (D.Dist < Ret.Dist)
			{
				Ret.Dist = D.Dist;
				Ret.Z = D.Z;
				Ret.pObj = D.pObj;
			}
			pV = (CvPt_Object*) pV->next;
		}
	}
	if ((FIL.isFilter(7)) || (FIL.isFilter(8)) || (FIL.isFilter(9)))
	{
		pC = (NCurve*) pPartC.Head;
		while (pC != NULL)
		{
			D = pC->SelDist(InPT, FIL);
			if (D.Dist < Ret.Dist)
			{
				Ret.Dist = D.Dist;
				Ret.Z = D.Z;
				Ret.pObj = D.pObj;
			}
			pC = (NCurve*) pC->next;
		}
	}
	if ((FIL.isFilter(15)) || (FIL.isFilter(16)) || (FIL.isFilter(17)))
	{
		pS = (NSurf*) pPartS.Head;
		while (pS != NULL)
		{
			D = pS->SelDist(InPT, FIL);
			if (D.Dist < Ret.Dist)
			{
				Ret.Dist = D.Dist;
				Ret.Z = D.Z;
				Ret.pObj = D.pObj;
			}
			pS = (NSurf*) pS->next;
		}
	}
	if ((!FIL.isFilter(18)) && (!FIL.isFilter(19)) &&
		(!FIL.isFilter(0)) && (!FIL.isFilter(7)) &&
		(!FIL.isFilter(15)) && (!FIL.isFilter(16)) && (!FIL.isFilter(17)))
	{
		Ret.pObj = this;
	}

	return (Ret);
}

int Part::GetVarHeaders(CString sVar[])
{
	sVar[0] = "Name";
	sVar[1] = "Colour ID";
	sVar[2] = "Translucency";
	return(3);
}


int Part::GetVarValues(CString sVar[])
{
	int iNo = 0;
	char S1[80] = "";
	sprintf_s(S1, "%s", sName);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%i", iColour);
	sVar[iNo] = S1;
	iNo++;
	sprintf_s(S1, "%g", Alpha);
	sVar[iNo] = S1;
	iNo++;
	return (iNo);
}

void Part::PutVarValues(PropTable* PT, int iNo, CString sVar[])
{
	sName = sVar[0];
	iColour = atoi(sVar[1]);
	Alpha = atof(sVar[2]);
}

void Part::RelTo(G_Object* pThis, ObjList* pList, int iType)
{
	int i;
	USE* pU;

	if (pThis->iObjType == 7) //Its a space curve
	{
		Shell* pShell;
		if (iType == 18)  //Face
		{
			pShell = (Shell*)pShells.Head;
			while (pShell != NULL)
			{
				pU = (USE*)pShell->CurveUSES.Get(pThis->iLabel);
				if (pU != NULL)
				{
					for (i = 0; i < pU->iNo; i++)
					{
						if (pU->Use[i] != NULL)
						{
							pList->AddEx(pU->Use[i]);
						}
					}
				}
				pShell = (Shell*)pShells.next;
			}
		}
		else if (iType == 0)
		{
			NCurve* pC = (NCurve*) pThis;
			if (pC->pS!=NULL)
			  pList->AddEx(pC->pS);
			if (pC->pE != NULL)
				pList->AddEx(pC->pE);
		}
	}
	else if (pThis->iObjType == 19) //Shell Related
	{
		pThis->RelTo(pThis, pList, iType);
	}
	else if (pThis->iObjType == 20) //Part Related
	{
		if (iType == 19) //Shells related to Part
		{
			Shell* pShell;
			pShell = (Shell*)pShells.Head;
			while (pShell != NULL)
			{
				pList->AddEx(pShell);
				pShell = (Shell*) pShell->next;
			}
		}
		if (iType == 7) //Curves related to Part
		{
			NCurve* pC;
			pC = (NCurve*)pPartC.Head;
			while (pC != NULL)
			{
				pList->AddEx(pC);
				pC = (NCurve*) pC->next;
			}
		}
		if (iType == 15) //Surfaces related to Part
		{
			NSurf* pS;
			pS = (NSurf*)pPartS.Head;
			while (pS != NULL)
			{
				pList->AddEx(pS);
				pS = (NSurf*)pS->next;
			}
		}
		if (iType == 0) //Vertex related to Part
		{
			CvPt_Object* pP;
			pP = (CvPt_Object*) pPartV.Head;
			while (pP != NULL)
			{
				pList->AddEx(pP);
				pP = (CvPt_Object*) pP->next;
			}
		}
	}

}

void Part::Info()
{

	char S1[80];
	outtext1("******* Vertices *******");
	CvPt_Object* pNext;
	pNext = (CvPt_Object*) pPartV.Head;
	while (pNext != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i %g %g %g", pNext->iObjType, pNext->iLabel,pNext->Pt_Point->x, pNext->Pt_Point->y,pNext->Pt_Point->z);
		outtext1(S1);
		pNext = (CvPt_Object*) pNext->next;
	}

	outtext1("******* Curves *******");
	NCurve* pC;
	pC = (NCurve*) pPartC.Head;
	while (pC != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i PT1: %i PT2: %i", pC->iObjType, pC->iLabel, pC->pS->iLabel, pC->pE->iLabel);
		outtext1(S1);
		pC = (NCurve*) pC->next;
	}
	outtext1("******* Surfaces *******");
	G_Object* pN = pPartS.Head;
	while (pN != NULL)
	{
		sprintf_s(S1, "TYPE: %i LAB: %i", pN->iObjType, pN->iLabel);
		outtext1(S1);
		pN = pN->next;
	}
	
	outtext1("**************");
}



G_Object* Part::GetObj(int iType, int iLab)
{
	G_Object* pRet;
	pRet = NULL;
	if (iType == 0)  
	{	//Vertex
		pRet = pPartV.Get(iLab);
	}
	else if ((iType == 7) || (iType == 8) || (iType == 9))
	{	//Curve
		pRet = pPartC.Get(iLab);
	}
	else if ((iType == 15) || (iType == 16) || (iType == 17))
	{	//Surface
		pRet = pPartS.Get(iLab);
	}
	else if (iType == 19)
	{	//Shell
		pRet = pShells.Get(iLab);
	}
	//Also need faces
	return (pRet);
}

void Part::Translate(C3dVector vIn)
{
	CvPt_Object* pNext;
	pNext = (CvPt_Object*)pPartV.Head;
	while (pNext != NULL)
	{
		pNext->Translate(vIn);
		pNext = (CvPt_Object*)pNext->next;
	}

	NCurve* pC;
	pC = (NCurve*)pPartC.Head;
	while (pC != NULL)
	{
		pC->Translate(vIn);
		pC = (NCurve*)pC->next;
	}

	G_Object* pN = pPartS.Head;
	while (pN != NULL)
	{
		pN->Translate(vIn);
		pN = pN->next;
	}
}

void Part::Transform(C3dMatrix TMat)
{
	CvPt_Object* pNext;
	pNext = (CvPt_Object*)pPartV.Head;
	while (pNext != NULL)
	{
		pNext->Transform(TMat);
		pNext = (CvPt_Object*)pNext->next;
	}

	NCurve* pC;
	pC = (NCurve*)pPartC.Head;
	while (pC != NULL)
	{
		pC->Transform(TMat);
		pC = (NCurve*)pC->next;
	}

	G_Object* pN = pPartS.Head;
	while (pN != NULL)
	{
		pN->Transform(TMat);
		pN = pN->next;
	}
}

CString Part::GetName()
{
	return("Solid Part");
}

void Part::S_Box(CPoint P1, CPoint P2, ObjList* pSel)
{
	//Surfaces in part
	G_Object* pN = pPartS.Head;
	while (pN != NULL)
	{
		if (pN->isSelectable())
		{
			pN->S_Box(P1, P2, pSel);
		}
		pN =  pN->next;
	}
	//Vertices
	pN = pPartV.Head;
	while (pN != NULL)
	{
		if (pN->isSelectable())
		{
			pN->S_Box(P1, P2, pSel);
		}
		pN = pN->next;
	}
	//Space curves
	pN = pPartC.Head;
	while (pN != NULL)
	{
		if (pN->isSelectable())
		{
			pN->S_Box(P1, P2, pSel);
		}
		pN = pN->next;
	}
}

void Part::DeleteSurface(G_Object* pS)
{
	//find the surface in part to delete
	pPartS.Remove2(pS);
}

void Part::DeletePoint(G_Object* pS)
{
	//find the surface in part to delete
	pPartV.Remove2(pS);
}

void Part::DeleteCurve(G_Object* pS)
{
	//find the surface in part to delete
	pPartC.Remove2(pS);
}

void Part::DeleteFace(G_Object* pS)
{
	BOOL break2 = FALSE;
	Shell* pShell;
	pShell = (Shell*)pShells.Head;
	while (pShell != NULL)
	{
		Face* pFNext;
		pFNext = (Face*) pShell->pFaces.Head;
		while (pFNext != NULL)
		{
			if (pFNext == pS)
			{
				pShell->pFaces.Remove2(pFNext);
				return;
			}
			pFNext = (Face*) pFNext->next;
		}
		pShell = (Shell*) pShell->next;
	}
}

void Part::GetFaceRef(G_Object* pS, ObjList* pFaces)
{
	Shell* pShell;
	pShell = (Shell*) pShells.Head;
	while (pShell != NULL)
	{
		Face* pFNext;
		pFNext = (Face*) pShell->pFaces.Head;
		while (pFNext != NULL)
		{
			if (pFNext->pSurf == pS)
			{
				pFaces->AddEx(pFNext);
			}
			pFNext = (Face*)pFNext->next;
		}
		pShell = (Shell*) pShell->next;
	}
}

void Part::GetFace(G_Object* pF, ObjList* pFaces)
{
	Shell* pShell;
	pShell = (Shell*)pShells.Head;
	while (pShell != NULL)
	{
		Face* pFNext;
		pFNext = (Face*)pShell->pFaces.Head;
		while (pFNext != NULL)
		{
			if (pFNext == pF)
			{
				pFaces->AddEx(pFNext);
				return;
			}
			pFNext = (Face*)pFNext->next;
		}
		pShell = (Shell*)pShell->next;
	}
}

int Part::GetPointUseCnt(CvPt_Object* pPT)
{
	int iRC = 1;
	Shell* pShell;
	pShell = (Shell*) pShells.Head;
	USE* pU;
	while (pShell != NULL)
	{
		//pShell->CurveUSES.
		pU = (USE*) pShell->PtUSES.Get(pPT->iLabel);
		if (pU != NULL)
		{
			iRC = pU->iNo;
			if (iRC > 0)
				break;
		}
		else
		{
			iRC = 0;
		}
		pShell = (Shell*)pShell->next;
	}
	return (iRC);
}

int Part::GetCurveUseCnt(NCurve* pSC)
{
	int iRC = 1;
	Shell* pShell;
	pShell = (Shell*) pShells.Head;
	USE* pU;
	while (pShell != NULL)
	{
		//pShell->CurveUSES.
	    pU = (USE*) pShell->CurveUSES.Get(pSC->iLabel);
		if (pU != NULL)
		{
			iRC = pU->iNo;
			if (iRC > 0)
				break;
		}
		else
		{
			iRC = 0;
		}
		pShell = (Shell*) pShell->next;
	}
	return (iRC);
}


//Remove curve uses on pt for all shells
void Part::RemoveCurveUses(NCurve* pSC)
{
	Shell* pShell;
	pShell = (Shell*) pShells.Head;
	while (pShell != NULL)
	{
		pShell->RemoveCurveUses(pSC);
		pShell = (Shell*)pShell->next;
	}
}

//*****************************************************************************************

IMPLEMENT_DYNAMIC( CvPt_Object , CObject )

CvPt_Object ::~CvPt_Object ()
{
 if (Pt_Point!=NULL)
 {
   delete(Pt_Point);
   Pt_Point=NULL;
 }

 if (DSP_Point!=NULL)
 {
   delete(DSP_Point);
   DSP_Point=NULL;
 }

}


// Create Object
CvPt_Object::CvPt_Object ()
{
Pt_Point = new C3dVector();
DSP_Point = new C3dVector();
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 0;
iColour = 75;
iLabel = -1;
Pt_Point->x=0;
Pt_Point->y=0;
Pt_Point->z=0;
w=1;
}

void CvPt_Object::Create(C3dVector InPt,double Inw, int iLab,int i2,int i3, int iC,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iObjType = 0;
iColour = iC;
iLabel = iLab;
Pt_Point->x=InPt.x;
Pt_Point->y=InPt.y;
Pt_Point->z=InPt.z;
//Pt_Point = new C3dVector(InPt.x,InPt.y,InPt.z);
//DSP_Point = new C3dVector();
pParent=Parrent;
w=Inw;
}

void CvPt_Object::Info()
{
  char S1[80];
  G_Object::Info();
  sprintf_s(S1,"LAB: %i X: %f Y: %f Z: %f W: %f",iLabel,Pt_Point->x,Pt_Point->y,Pt_Point->z,w);
  outtext1(_T(S1));
  
}

C3dVector CvPt_Object::MinPt(C3dVector inPt)
{
C3dVector vRet;
vRet.Set(Pt_Point->x,Pt_Point->y,Pt_Point->z);
return (vRet);
}




G_Object* CvPt_Object::Copy(G_Object* Parrent)
{
CvPt_Object* PtRet = new CvPt_Object;
PtRet->Drawn = Drawn;
PtRet->Selectable  = Selectable; 
PtRet->Visable  = Visable;
PtRet->iColour = iColour;
PtRet->iObjType = iObjType;
PtRet->iLabel = iLabel;
PtRet->Pt_Point = new C3dVector(Pt_Point->x,Pt_Point->y,Pt_Point->z);
PtRet->DSP_Point = new C3dVector();
PtRet->pParent=Parrent;
PtRet->w=w;
return (PtRet);
}

void CvPt_Object::Serialize(CArchive& ar,int iV)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar<<Pt_Point->x;
    ar<<Pt_Point->y;
    ar<<Pt_Point->z;  
	ar<<w;
	}
	else
	{
    G_Object::Serialize(ar,iV);
    Pt_Point = new C3dVector();
    DSP_Point = new C3dVector();
    ar>>Pt_Point->x;
    ar>>Pt_Point->y;
    ar>>Pt_Point->z; 
    ar>>w;
	}
}





void CvPt_Object::Clear()
{

}

C3dVector CvPt_Object::GetCoords() 
{
C3dVector vRet;
vRet.Set(Pt_Point->x,Pt_Point->y,Pt_Point->z);
return (vRet);
}

C3dVector CvPt_Object::Get_Centroid()
{
return (GetCoords());
}



// Draw Object line
void CvPt_Object::Draw(CDC* pDC,int iDrawmode)
{

pDC->MoveTo((int) DSP_Point->x-4,(int) DSP_Point->y+4);
pDC->LineTo((int) DSP_Point->x+4,(int) DSP_Point->y-4);
pDC->MoveTo((int) DSP_Point->x+4,(int) DSP_Point->y+4);
pDC->LineTo((int) DSP_Point->x-4,(int) DSP_Point->y-4);
pDC->MoveTo((int) DSP_Point->x+4,(int) DSP_Point->y);
pDC->LineTo((int) DSP_Point->x-5,(int) DSP_Point->y);
pDC->MoveTo((int) DSP_Point->x,(int) DSP_Point->y-4);
pDC->LineTo((int) DSP_Point->x,(int) DSP_Point->y+4);
//pDC->SetPixel(DSP_Point->x, DSP_Point->y+4, 255 );

//pDC->Ellipse(DSP_Point->x+3,DSP_Point->y+3,DSP_Point->x-3,DSP_Point->y-3);
}


void CvPt_Object::OglDraw(int iDspFlgs,double dS1,double dS2)
{
OglDrawW(iDspFlgs,dS1,dS2);
}

void CvPt_Object::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
if ((iDspFlgs & DSP_POINTS) > 0)
{
  Selectable=1;
  glColor3fv(cols[GetCol()]);
  glPointSize(6.0f);
  glBegin(GL_POINTS);
  glVertex3f((float) Pt_Point->x,(float) Pt_Point->y,(float) Pt_Point->z);
  glEnd();
  	if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"Pt%i",iLabel);
	  OglString(iDspFlgs,(float) Pt_Point->x,(float) Pt_Point->y,(float) Pt_Point->z,&sLab[0]);
    }
}
else
{
  Selectable=0;
}

}

void CvPt_Object::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{

C3dVector V;
C3dVector R;
G_Object::SetToScr(pModMat,pScrTran);
  
R.x = Pt_Point->x;
R.y = Pt_Point->y;
R.z = Pt_Point->z;
V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;

R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;


DSP_Point->x = R.x;
DSP_Point->y = R.y;
DSP_Point->z = 0;
}

void CvPt_Object::Move(C3dVector vM)
{
Pt_Point->x +=vM.x;
Pt_Point->y +=vM.y;
Pt_Point->z +=vM.z;
}

void CvPt_Object::Transform(C3dMatrix TMAt)
{
C3dVector R;
R.x =  TMAt.m_00 * Pt_Point->x +  TMAt.m_01 * Pt_Point->y +  TMAt.m_02 * Pt_Point->z +  TMAt.m_30;
R.y =  TMAt.m_10 * Pt_Point->x +  TMAt.m_11 * Pt_Point->y +  TMAt.m_12 * Pt_Point->z +  TMAt.m_31;
R.z =  TMAt.m_20 * Pt_Point->x +  TMAt.m_21 * Pt_Point->y +  TMAt.m_22 * Pt_Point->z +  TMAt.m_32;
Pt_Point->x =R.x;
Pt_Point->y =R.y;
Pt_Point->z =R.z;

}

void CvPt_Object::Translate(C3dVector vIn)
{
Pt_Point->x +=vIn.x;
Pt_Point->y +=vIn.y;
Pt_Point->z +=vIn.z;
}

void CvPt_Object::ScaleX(double d)
{
Pt_Point->x *=d;
}


void CvPt_Object::HighLight(CDC* pDC)
{
pDC->Ellipse((int) DSP_Point->x+5,(int) DSP_Point->y+5,(int) DSP_Point->x-5,(int) DSP_Point->y-5);

}





void CvPt_Object::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
if ((DSP_Point->x > P1.x) && 
	  (DSP_Point->x < P2.x) &&
    (DSP_Point->y > P1.y) &&
    (DSP_Point->y < P2.y))
{
	pSel->Add(this);
}
}


void CvPt_Object::SetTo(C3dVector cInVect)
{
Pt_Point->x = cInVect.x;
Pt_Point->y = cInVect.y;
Pt_Point->z = cInVect.z;
}


IMPLEMENT_DYNAMIC( CvPt_ObjectW , CObject )
void CvPt_ObjectW::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
G_Object::SetToScr(pModMat,pScrTran);
C3dVector V;
C3dVector R;
double x,y;
NSurf* pS;
pS=(NSurf*) this->pParent->pParent;
if (pS!=NULL)
{
  x=Pt_Point->x;
  y=Pt_Point->y;
  w=w;
  R=pS->GetPt(x,y);
  V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
  V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
  V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
  R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
  R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
  R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
  DSP_Point->x = R.x;
  DSP_Point->y = R.y;
  DSP_Point->z = 0;
}
}

C3dVector CvPt_ObjectW::Get_Centroid()
{
//FOR POINTS ON A SURFACE THAT NEED u,v COORDS CONVERTING TO XYZ
C3dVector V;
C3dVector R;
double x,y;
NSurf* pS;
pS=(NSurf*) this->pParent->pParent;
if (pS!=NULL)
{
  x=Pt_Point->x;
  y=Pt_Point->y;
  w=w;
  R=pS->GetPt(x,y);
}
return (R);
}

IMPLEMENT_DYNAMIC(NCurve, CObject)

NCurve ::~NCurve ()
{
int i;
for (i=0;i<iNoCPts;i++)
{
  if (cPts [i]!=NULL)
  {
    delete(cPts[i]);
    cPts[i]=NULL;
  }
}

}

void NCurve::Clean()
{
int i;
for (i=0;i<iNoCPts;i++)
{
  if (cPts [i]!=NULL)
  {
    delete(cPts[i]);
    cPts[i]=NULL;
  }
}
iNoCPts=0;

}

NCurve::NCurve()
{
iType = 1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = -1;
iObjType = 7;
iNoCPts=0;
p=3;
ws=0;
we=1;
iLnThk=4;
iLnType=1;
DrawCPts=FALSE;
DrawNoCvs=FALSE;
pS=NULL;
pE=NULL;
iInc = -1;
dLSize = 1;
}


    // CvPt_Object* pVertex[20];
void NCurve::Create(int iLab,G_Object* Parrent)
{
Drawn = 0;
iType = 1;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iObjType = 7;
iLnThk = 4;
iNoCPts=0;
p=3;
ws=0.0;
we=1.0;
pParent=Parrent;
DrawCPts=FALSE;
DrawNoCvs=FALSE;
pS=NULL;
pE=NULL;
iInc = -1;
dLSize = 1;

}

CString NCurve::GetKnotString()
{
  CString sK;
  int i;
  char S1[80];

  for (i = 0; i<iNoCPts + p + 1; i++)
  {
    sprintf_s(S1, "%g,", knots[i]);
    sK += S1;
  }
  return (sK);
}

void NCurve::Info()
{
int i;
char S1[80];
sprintf_s(S1, "%s", "CURVE OBJECT");
outtext1(S1);
G_Object::Info();
sprintf_s(S1, "%s%i", "Curve Type : ", iType);
outtext1(S1);
sprintf_s(S1, "%s%i", "Curve Mesh Increment : ", iInc);
outtext1(S1);
sprintf_s(S1, "Order: %i", p);
outtext1(_T(S1));
CString sO;
for (i=0;i<iNoCPts;i++)
{
  sprintf_s(S1, "LAB: %i X: %f Y: %f Z: %f W: %f", iLabel, cPts[i]->Pt_Point->x, cPts[i]->Pt_Point->y, cPts[i]->Pt_Point->z, cPts[i]->w);
  outtext1(_T(S1));
}
outtext1("Knot Sequence:-");
for (i = 0; i<iNoCPts+p+1; i++)
{
  sprintf_s(S1, "%g,", knots[i]);
  sO += S1;
}
outtext1(sO);
}



C3dVector NCurve::Get_Centroid()
{
return (GetPt(0.5));
}

void NCurve::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		G_Object::Serialize(ar,iV);
		ar << dLSize;
		ar << iInc;
		ar << iLnThk;
		ar << iLnType;
		ar << iNoCPts;
		ar << p;
		ar << ws;
		ar << we;
		ar << DrawCPts;
		ar << vS.x; ar << vS.y; ar << vS.z;
		ar << vE.x; ar << vE.y; ar << vE.z;
		for (i=0;i<iNoCPts;i++)
		{
			cPts[i]->Serialize(ar,iV);
		}
		for (i=0;i<iNoCPts+p+1;i++)
		{
			ar << knots[i];
		}
	}
	else
	{
		G_Object::Serialize(ar,iV);
		if (iV < -51)
		{
			ar >> dLSize;
			ar >> iInc;
		}
		else
		{
			iInc = -1;
		}
		ar >> iLnThk;
		ar >> iLnType;
		ar >> iNoCPts;
		ar >> p;
		ar >> ws;
		ar >> we;
		ar >> DrawCPts;
		if (iV <= -50)
		{
			ar >> vS.x; ar >> vS.y; ar >> vS.z;
			ar >> vE.x; ar >> vE.y; ar >> vE.z;
		}
		for (i=0;i<iNoCPts;i++)
		{
			cPts[i]=new CvPt_Object();
			cPts[i]->Serialize(ar,iV);
			cPts[i]->pParent=this;
		}
		for (i=0;i<iNoCPts+p+1;i++)
		{
			ar >> knots[i];
		}
	}
}

G_Object* NCurve::Copy(G_Object* Parrent)
{
int i;
NCurve* cPoly = new NCurve();
cPoly->iType = iType;
cPoly->Drawn = Drawn;
cPoly->Selectable  = Selectable; 
cPoly->Visable  = Visable;
cPoly->iColour = iColour;
cPoly->iObjType = iObjType;
cPoly->iLabel = iLabel;
cPoly->iNoCPts = iNoCPts;
cPoly->p = p;
cPoly->ws = ws;
cPoly->we = we;
cPoly->iInc = iInc;
cPoly->dLSize = dLSize;
cPoly->pParent=Parrent;

for (i=0;i<iNoCPts;i++)
{
  cPoly->cPts[i]=new CvPt_Object();
  cPoly->cPts[i]=(CvPt_Object*) cPts[i]->Copy(cPoly);
}
for (i=0;i<iNoCPts+p+1;i++)
{
  cPoly->knots[i]=knots[i];
}
return (cPoly);
}

void NCurve::GetContolPts(Vec <C4dVector> & Cpts)
{
int i;
double w;
Cpts.Size(iNoCPts);

for (i=0;i<iNoCPts;i++)
{
  w=cPts[i]->w;
  Cpts(i).xw=cPts[i]->Pt_Point->x*w;
  Cpts(i).yw=cPts[i]->Pt_Point->y*w;
  Cpts(i).zw=cPts[i]->Pt_Point->z*w;
  Cpts(i).w;
}
}

void NCurve::GetcPts(Vec <C3dVector> & Cpts)
{
int i;

Cpts.Size(iNoCPts);

for (i=0;i<iNoCPts;i++)
{
  Cpts(i).x=cPts[i]->Pt_Point->x;
  Cpts(i).y=cPts[i]->Pt_Point->y;
  Cpts(i).z=cPts[i]->Pt_Point->z;
}
}


void NCurve::GetKnotVec(Vec<double>& U)
{
int i;

int iNoK = (iNoCPts)+p+1;
U.Size(iNoK);
for (i=0;i<iNoK;i++)
{
  U(i)=knots[i];
}
}

int NCurve::knotInsertion(double u, int r, Vec <C4dVector> & ncP,Vec <double> & ncU)
{
  // Compute k and s      u = [ u_k , u_k+1)  with u_k having multiplicity s
  int k=0,s=0 ;
  int i,j ;
  int deg_ = p;
  Vec <C4dVector> P;
  GetContolPts(P);
  Vec<double> U;
  GetKnotVec(U);
  if(u<U[deg_] || u>U[P.n])
  {
     //ERROR: out of range
  }
  for(i=0;i<U.n;i++)
  {
    if(U[i]>u) 
    {
      k = i-1 ;
      break ;
    }
  }
  if(u<=U[k])
  {
    s = 1 ;
    for(i=k;i>deg_;i--)
    {
      if(U[i]<=U[i-1])
	       s++ ;
      else
	      break ;
    }
  }
  else
  {
    s=0 ;
  }

  if((r+s)>p+1)
    r = p+1-s ;

  if(r<=0)
    return 0 ; 


  ncU.Size(U.n+r);

  // Load new knot vector
  for(i=0;i<=k;i++)
    ncU[i] = U[i] ;
  for(i=1;i<=r;i++)
    ncU[k+i] = u ;
  for(i=k+1;i<U.n; i++)
    ncU[i+r] = U[i] ;

  // Save unaltered control points
  ncP.Size(P.n+r);
  Vec <C4dVector> R(p+1);

  for(i=0; i<=k-p ; i++)
    ncP[i] = P[i] ;
  for(i=k-s ; i< P.n ; i++)
    ncP[i+r] = P[i] ;
  for(i=0; i<=p-s; i++)
    R[i] = P[k-p+i] ;

  // Insert the knot r times
  int L=0;
  double alpha;
  for(j=1; j<=r ; j++){
    L = k-p+j ;
    for(i=0;i<=p-j-s;i++){
      alpha = (u-U[L+i])/(U[i+k+1]-U[L+i]) ;
      R[i] = R[i+1]*alpha + R[i]*(1.0-alpha) ;
    }
    ncP[L] = R[0] ;
    if(p-j-s > 0)
      ncP[k+r-j-s] = R[p-j-s] ;
  }

  // Load remaining control points
  for(i=L+1; i<k-s ; i++){
    ncP[i] = R[i-L] ;
  }
  P.DeleteAll();
  U.DeleteAll();
  R.DeleteAll();

  return r; 
}


void NCurve::refineKnotVector(Vec<double>& XVec,Vec <C4dVector> & P,Vec <double> & U)
{
  int deg_ = p;
  Vec <C4dVector> cP;
  GetContolPts(cP);
  Vec<double> cU;
  GetKnotVec(cU);

  int n = cP.n-1 ;
  int m = n+p+1 ;
  int a,b ;
  int r = XVec.n-1 ;


  P.Size(r+1+n+1);
  U.Size(r+1+n+1+p+1);

  a = FindSpan(XVec[0]) ;
  b = FindSpan(XVec[r]) ;
  ++b ;
  int j ;
  for(j=0; j<=a-p ; j++)
    P[j] = cP[j] ;
  for(j = b-1 ; j<=n ; j++)
    P[j+r+1] = cP[j] ;
  for(j=0; j<=a ; j++)
    U[j] = cU[j] ;
  for(j=b+p ; j<=m ; j++)
    U[j+r+1] = cU[j] ;
  int i = b+p-1 ; 
  int k = b+p+r ;
  for(j=r; j>=0 ; j--){
    while(XVec[j] <= cU[i] && i>a){
      P[k-p-1] = cP[i-p-1] ;
      U[k] = cU[i] ;
      --k ;
      --i ;
    }
    P[k-p-1] = P[k-p] ;
    for(int l=1; l<=p ; l++){
      int ind = k-p+l ;
      double alpha = U[k+l] - XVec[j] ;
      if(alpha==0.0)
	     P[ind-1] = P[ind] ;
      else
	     alpha /= U[k+l]-cU[i-p+l] ;
       P[ind-1] = P[ind-1]*alpha + P[ind]*(1.0-alpha) ;
    }
    U[k] = XVec[j] ;
    --k ;
  }
}

double NCurve::chordLengthParam(const Vec<C3dVector> & Q, Vec<double> &ub)
 {
  int i ;
  double d=0;
  C3dVector T;
  ub.Size(Q.n) ;
  ub[0] = 0 ; 
  for(i=1;i<ub.n;i++){
    T=Q[i];
    T-=Q[i-1];
    d += T.Mag();
  }
  if(d>0){
    for(i=1;i<ub.n-1;++i){
      T=Q[i];
      T-=Q[i-1];
      ub[i] = ub[i-1] + T.Mag()/d ;
    }
    ub[ub.n-1] = 1.0 ; // In case there is some addition round-off
  }
  return d ;
}

void NCurve::knotAveraging(const Vec<double>& uk, int deg, Vec<double>& U)
{
  U.Size(uk.n+deg+1);

  int j ;
  for(j=1;j<uk.n-deg;++j){
    U[j+deg] = 0.0 ;
    for(int i=j;i<j+deg;++i)
      U[j+deg] += uk[i] ;
    U[j+deg] /= deg ;
  }
  for(j=0;j<=deg;++j)
    U[j] = 0.0 ;
  for(j=U.n-deg-1;j<U.n;++j)
    U[j] = 1.0 ;
}

void NCurve::globalInterp(int d,Vec <C4dVector> &nP,Vec<double> &nU)
{
  int i,j ;
  Vec <C3dVector> Q;
  Vec <double> ub;
  p=d;
  GetcPts(Q);
  chordLengthParam(Q,ub);

  nP.Size(Q.n) ;
  nU.Size(Q.n+d+1);

  Matrix <double> A(Q.n,Q.n) ;
  knotAveraging(ub,d,nU);

  // Initialize the basis matrix A
  double N[10];
   for(i=0;i<A.m;i++){
    for(j=0;j<A.n;j++) 
	    A(i,j) = 0;
  } 


  for(i=1;i<Q.n-1;i++){
    int span = FindSpan2(d,nU,ub[i]);
    BasisFun2(d,span,ub[i],nU,N) ;
    for(j=0;j<=d;j++) 
	    A(i,span-d+j) = N[j];
  }
  A(0,0)  = 1.0 ;
  A(Q.n-1,Q.n-1) = 1.0 ;

  //// Init matrix for LSE
  Matrix<double> qq(Q.n,3) ;
  Matrix<double> xx;
  for(i=0;i<Q.n;i++)
  {
      qq(i,0) = Q[i].x;
      qq(i,1) = Q[i].y;
      qq(i,2) = Q[i].z;
  }
  double dd;
  for (i=0;i<A.m;i++)
  {
     dd=A(i,0);
  }
  A.Invert();
  for (i=0;i<A.m;i++)
  {
     dd=A(i,0);
  }

  A.Mult(qq,xx);
  for(i=0;i<Q.n;i++)
  {
      nP[i].xw = xx(i,0);
      nP[i].yw = xx(i,1);
      nP[i].zw = xx(i,2);
      nP[i].w = 1;
  }
  //clean up
  A.DeleteAll();
  qq.DeleteAll();
  ub.DeleteAll();
  Q.DeleteAll();
  xx.DeleteAll();

  //solve(A,qq,xx) ;

  //// Store the data
  //for(i=0;i<xx.rows();i++){
  //  for(j=0;j<D;j++)
  //    P[i].data[j] = (T)xx(i,j) ;
  //  P[i].w() = 1.0 ;
  //}

}


void NCurve::AddVert(C3dVector pInVertex1, double dWght)
{


if (iNoCPts<MAX_CVPTS)
{
  cPts [iNoCPts] = new CvPt_Object();
  cPts [iNoCPts]->Create(pInVertex1,dWght,0,0,0,12,this);
  iNoCPts++;
}
else
{
  outtext1("ERROR: Max No of Points Exceeded.");
}

}



void NCurve::Generate(int inp)
{
p = inp;
int i;
int ind=0;
double span;
span = 1.0/(iNoCPts-p);

for(i=0;i<p;i++)
{
  knots[ind]=0;
  ind++;
}
for(i=0;i<iNoCPts-p+1;i++)
{
  knots[ind]=span*i;
  ind++;
}
for(i=0;i<p;i++)
{
  knots[ind]=1;
  ind++;
}
}




void NCurve::GenerateExp(double Deg,Vec <C4dVector> & P,Vec<double> & U)
{
int i;
C3dVector V;
double W;
Clean();
p=(int) Deg;
for(i=0;i<P.n;i++)
{
  if (iNoCPts<MAX_CVPTS)
  {
    W=P[i].w;
    V.x=P[i].xw;
    V.y=P[i].yw;
    V.z=P[i].zw;
    cPts [iNoCPts] = new CvPt_Object();
    cPts [iNoCPts]->Create(V,W,0,0,0,12,this);
    iNoCPts++;
  }
  else
  {
    outtext1("ERROR: Max No of Points Exceeded.");
  }
}
for(i=0;i<U.n;i++)
{
  knots[i] = U[i];
}
}


int NCurve::FindSpan(double u)
{
int iRet;
int low,mid,high;

if (u==knots[iNoCPts])
{
  iRet=iNoCPts-1;
}
else
{
  low=p;
  high=iNoCPts;
  mid=(low+high)/2;
  while((u<knots[mid]) || (u>=knots[mid+1]))
  {
    if (u<knots[mid])
	{
	  high=mid;
	}
	else
	{
      low=mid;
	}
    mid=(low+high)/2;
  }
iRet=mid;
}
return (iRet);
}

int NCurve::FindSpan2(double deg,const Vec <double> & U, double u)
{
int iRet;
int low,mid,high;

if (u==U[iNoCPts])
{
  iRet=iNoCPts-1;
}
else
{
  low=(int) deg;
  high=iNoCPts;
  mid=(low+high)/2;
  while((u<U[mid]) || (u>=U[mid+1]))
  {
    if (u<U[mid])
	{
	  high=mid;
	}
	else
	{
      low=mid;
	}
    mid=(low+high)/2;
  }
iRet=mid;
}
return (iRet);
}

void NCurve::BasisFun(double deg,int i,double u,double N[10])
{
int j;
int r;
double left[20];
double right[20];
double saved;
double temp;
N[0]=1.0;
for (j=1;j<=deg;j++)
{
  left[j]=u-knots[i+1-j];
  right[j]=knots[i+j]-u;
  saved=0.0;
  for (r=0;r<j;r++)
  {
    temp=N[r]/(right[r+1]+left[j-r]);
	  N[r]=saved+right[r+1]*temp;
    saved=left[j-r]*temp;
  }
  N[j]=saved;
}
}

void NCurve::BasisFun2(double deg,int i,double u,const Vec <double> & U,double N[10])
{
int j;
int r;
double left[20];
double right[20];
double saved;
double temp;
N[0]=1.0;
for (j=1;j<=deg;j++)
{
  left[j]=u-U[i+1-j];
  right[j]=U[i+j]-u;
  saved=0.0;
  for (r=0;r<j;r++)
  {
    temp=N[r]/(right[r+1]+left[j-r]);
	  N[r]=saved+right[r+1]*temp;
    saved=left[j-r]*temp;
  }
  N[j]=saved;
}
}

void NCurve::binomialCoef(Mat* Bin)
{
  int n,k ;
  // Setup the first line
  *Bin->mn2(0,0) = 1.0 ;
  for(k=Bin->n-1;k>0;--k)
  {
    *Bin->mn2(0,k) = 0.0 ;
  }
  // Setup the other lines
  for(n=0;n<Bin->m-1;n++)
  {
    *Bin->mn2(n+1,0) = 1.0 ;
    for(k=1;k<Bin->n;k++)
    {
      if(n+1<k)
	      *Bin->mn2(n,k) = 0.0 ;
      else
	      *Bin->mn2(n+1,k) = *Bin->mn2(n,k) + *Bin->mn2(n,k-1) ;
    }
  }
}

void NCurve::deriveAt(double u,int d,C3dVector CK[10])
{
  C4dVector ders[10];
  int deg_=p;
  int du;
  du = d;
  if (deg_ < d)
  {
    du = deg_;
  }
  int iSpan ;
  Mat* derF ;

  iSpan = FindSpan(u) ;
  derF = dersBasisFuns(du,u,iSpan) ;

  int k;
  for(k=du;k>=0;--k)
  {
    ders[k].Clear();
    for(int j=deg_;j>=0;--j)
    {
      ders[k].xw += *derF->mn2(k,j)*(cPts[iSpan-deg_+j]->Pt_Point->x*cPts[iSpan-deg_+j]->w) ;
      ders[k].yw += *derF->mn2(k,j)*(cPts[iSpan-deg_+j]->Pt_Point->y*cPts[iSpan-deg_+j]->w) ;
      ders[k].zw += *derF->mn2(k,j)*(cPts[iSpan-deg_+j]->Pt_Point->z*cPts[iSpan-deg_+j]->w) ;
      ders[k].w += *derF->mn2(k,j)*(cPts[iSpan-deg_+j]->w) ;
    }
  }
    Mat* Bin = new Mat(d+1,d+1);
    binomialCoef(Bin);
    C3dVector v;
    int i;
    for (k=0;k<=du;k++)
    {
     v.x=ders[k].xw;
     v.y=ders[k].yw;
     v.z=ders[k].zw;
     for (i=1;i<=k;i++)
     {
       v.x=v.x-(*Bin->mn2(k,i))*ders[i].w*CK[k-i].x;
       v.y=v.y-(*Bin->mn2(k,i))*ders[i].w*CK[k-i].y;
       v.z=v.z-(*Bin->mn2(k,i))*ders[i].w*CK[k-i].z;
     }
     CK[k].x=v.x/ders[0].w;
     CK[k].y=v.y/ders[0].w;
     CK[k].z=v.z/ders[0].w;
    }
  delete(derF);
  delete(Bin);
}


Mat* NCurve::dersBasisFuns(int n,double u, int span)  
{
int deg_ =p;
double* left = new double[2*(deg_+1)]; 
double* right = &left[deg_+1] ;
  
Mat* ndu= new Mat(deg_+1,deg_+1);
double saved,temp ;
int j,r ;

Mat*  ders= new Mat(n+1,deg_+1) ;

  *ndu->mn2(0,0) = 1.0 ;
  for(j=1; j<= deg_ ;j++)
  {
    left[j] = u-knots[span+1-j] ;
    right[j] = knots[span+j]-u ;
    saved = 0.0 ;
    for(r=0;r<j ; r++)
    {
      // Lower triangle
      *ndu->mn2(j,r) = right[r+1]+left[j-r] ;
      temp = *ndu->mn2(r,j-1)/(*ndu->mn2(j,r)) ;
      // Upper triangle
      *ndu->mn2(r,j) = saved+right[r+1] * temp ;
      saved = left[j-r] * temp ;
    }

    *ndu->mn2(j,j) = saved ;
  }

  for(j=deg_;j>=0;--j)
    *ders->mn2(0,j) = *ndu->mn2(j,deg_) ;

  // Compute the derivatives
  Mat* a = new Mat(deg_+1,deg_+1) ;
  for(r=0;r<=deg_;r++){
    int s1,s2 ;
    s1 = 0 ; s2 = 1 ; // alternate rows in array a
    *a->mn2(0,0) = 1.0 ;
    // Compute the kth derivative
    for(int k=1;k<=n;k++){
      double d ;
      int rk,pk,j1,j2 ;
      d = 0.0 ;
      rk = r-k ; pk = deg_-k ;

      if(r>=k)
      {
	      *a->mn2(s2,0) = (*a->mn2(s1,0))/(*ndu->mn2(pk+1,rk)) ;
	      d = *a->mn2(s2,0)*(*ndu->mn2(rk,pk));
      }

      if(rk>=-1){
	      j1 = 1 ;
      }
      else{
	      j1 = -rk ;
      }

      if(r-1 <= pk){
	       j2 = k-1 ;
      }
      else{
	       j2 = deg_-r ;
      }

      for(j=j1;j<=j2;j++)
      {
	      *a->mn2(s2,j) = (*a->mn2(s1,j)-*a->mn2(s1,j-1))/(*ndu->mn2(pk+1,rk+j)) ;
	      d += *a->mn2(s2,j)**ndu->mn2(rk+j,pk) ;
      }
      
      if(r<=pk)
      {
	     *a->mn2(s2,k) = -(*a->mn2(s1,k-1))/(*ndu->mn2(pk+1,r)) ;
	     d += *a->mn2(s2,k)*(*ndu->mn2(r,pk)) ;
      }
      *ders->mn2(k,r) = d ;
      j = s1 ; s1 = s2 ; s2 = j ; // Switch rows
    }
  }

  // Multiply through by the correct factors
  r = deg_ ;
  for(int k=1;k<=n;k++)
  {
    for(j=deg_;j>=0;--j)
      *ders->mn2(k,j) *= r ;
    r *= deg_-k ;
  }
delete (a);
delete (left);
delete (ndu);
return (ders);
}

void NCurve::CPolyRot(double dAng)
{
int i;
C3dVector vT1,a,b;
for (i = 0; i < iNoCPts; i++)
{
   vT1.x = cPts [i]->Pt_Point->x;
   vT1.y = cPts [i]->Pt_Point->y;
   vT1.z = cPts [i]->Pt_Point->z;
   vT1.Rotate(a,b,dAng);
   cPts [i]->Pt_Point->x = vT1.x;
   cPts [i]->Pt_Point->y = vT1.y;
   cPts [i]->Pt_Point->z = vT1.z;
}
}


void NCurve::Translate (C3dVector vIn)
{
int i;

for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->Pt_Point->x+=vIn.x;  
  cPts[i]->Pt_Point->y+=vIn.y;
  cPts[i]->Pt_Point->z+=vIn.z;
}
}

void NCurve::ScaleX(double d)
{
int i;

for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->Pt_Point->x*=d;  
}
}

void NCurve::ScaleW(double dIn)
{
int i;
for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->w*=dIn;  
}
}

int NCurve::GetOrder()
{

return (p);
}

C3dVector NCurve::MinPt(C3dVector inPt)
{
C3dVector vRet;
const double dIncs = 36.0;
int i;
double dStp = 1/dIncs;
double dW=0;
double dMinW=0;
C3dVector cPt;
C3dVector tPt;
C3dVector cNextPt;
double dWScl;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;


cPt=GetPt(dW);
tPt=GetPt(dW+dStp);
dWScl = cPt.Dist(tPt);
for(i=0;i<dIncs;i++)
{
  dW=i*dStp;
  cPt=GetPt(dW);
  Dist=cPt.Dist(inPt);
  if (Dist<minDist)
  {
    minDist=Dist;
    dMinW=dW;
  }
}
//start of iteration
dW=dMinW;
double dWStp=dStp; 
double sw = 0.0000001;   //delta w small increment for test direction or reducing dist
C3dVector PtPlusW;
C3dVector PtMinW;
double dC;
double dM;
double dP;
double dWp;
double dWm;
i=0;
C3dVector vT;
do
{
  cPt=GetPt(dW);   
  dWp=dW+sw; 
  dWm=dW-sw;
  //Out of parametric bounds conditions
  if (dWp>1)
  {
    if (iType==3)
      dWp-=1;
	else
      dWp=1;
  }
  if (dWm<0)
  {
    if (iObjType==3)
      dWm+=1;
	else
      dWm=0;
  }
  PtPlusW = GetPt(dWp);
  PtMinW = GetPt(dWm);
  vT=inPt-cPt;      dC=vT.Mag();
  vT=inPt-PtPlusW;  dP=vT.Mag();
  vT=inPt-PtMinW;   dM=vT.Mag();
  dWStp*=0.5;
  if (dP<dC)
    dW+=dWStp;
  if (dM<dC)
    dW-=dWStp;
  // Out of parametric bounds conditions
  if (iObjType==3)
  {
	if (dW<0)
      dW=dW+1;
    if (dW>1)
      dW=dW-1;
  }
  else
  {
    if (dW<0)
      dW=0;
    if (dW>1)
      dW=1;
  }
  i++;
}
while  ((dWStp>0.00001)&&(i<100));
vRet=GetPt(dW);

return (vRet);
}

double NCurve::MinWPt(C3dVector inPt)
{
C3dVector vRet;
const double dIncs = 36;
int i;
double dStp = 1/dIncs;
double dlStp;
double dW=0;
double dMinW=0;
C3dVector cPt;
C3dVector tPt;
C3dVector cNextPt;
double dWScl;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;
double dM;
double dDot;
cPt=GetPt(dW);
tPt=GetPt(dW+dStp);
dWScl = cPt.Dist(tPt);
for(i=0;i<dIncs;i++)
{
  dW=i*dStp;
  cPt=GetPt(dW);
  Dist=cPt.Dist(inPt);
  if (Dist<minDist)
  {
    minDist=Dist;
    dMinW=dW;
  }
}
//start of iteration
dW=dMinW;
dlStp=dStp;
i=0;
do
{
  cPt=GetPt(dW);
  //cNextPt=GetPt(dW+0.0000001);
  //v1=cNextPt;
  v1=GetDir(dW);
  v2=inPt;
  v2-=cPt;
  dM=v2.Mag();
  v1.Normalize();
  v2.Normalize();
  dDot=v2.Dot(v1);
  dlStp=(dM*dDot/dWScl)*1*dStp;
  dW=dW+dlStp;
  if (dW<0)
  {
    dW=0;
  }
  if (dW>1)
  {
    dW=1;
  }
  i++;
}
while ((pow((dlStp*dlStp),0.5)>dTol) & (i<10000));
vRet=GetPt(dW);
return (dW);
}

//***********************************************************************
// This check for the ambiguity of projecting 0 & 1 paremetric 
// values to a closed surface
//***********************************************************************
void NCurve::EndPtChk01(NSurf* pSurf,
                        double dS,
	                    C3dVector* pSfS,
	                    double dE,
	                    C3dVector* pSfE)
{
	C3dVector p3dC;
	C3dVector pTST;
	BOOL isClosedU = pSurf->isClosedinU();
	BOOL isClosedV = pSurf->isClosedinV();
	//Round of numerical errors
	if (abs(pSfS->x) < PARA_NEAR_ZERO)
		pSfS->x = 0;
	if (abs(pSfS->y) < PARA_NEAR_ZERO)
		pSfS->y = 0;
	if (abs(pSfS->x - 1) < PARA_NEAR_ZERO)
		pSfS->x = 1;
	if (abs(pSfS->y - 1) < PARA_NEAR_ZERO)
		pSfS->y = 1;
	if (abs(pSfE->x) < PARA_NEAR_ZERO)
		pSfE->x = 0;
	if (abs(pSfE->y) < PARA_NEAR_ZERO)
		pSfE->y = 0;
	if (abs(pSfE->x - 1) < PARA_NEAR_ZERO)
		pSfE->x = 1;
	if (abs(pSfE->y - 1) < PARA_NEAR_ZERO)
		pSfE->y = 1;
	//********************************************************
	if (isClosedU)
	{
		if ((pSfS->x == 0) || (pSfS->x == 1))
		{
			p3dC = this->GetPt(0.1*(ws+we));
			pTST = pSurf->MinPtW(p3dC);
			if (pTST.x > 0.5)
				pSfS->x = 1;
			else
				pSfS->x = 0;
		}
		if ((pSfE->x == 0) || (pSfE->x == 1))
		{
			p3dC = this->GetPt(1 - 0.1*(ws + we));
			pTST = pSurf->MinPtW(p3dC);
			if (pTST.x > 0.5)
				pSfE->x = 1;
			else
				pSfE->x = 0;
		}
	}
	if (isClosedV)
	{
		//Check Both End for 0-1 confussion
		if ((pSfS->y == 0) || (pSfS->y == 1))
		{
			p3dC = this->GetPt(0.1*(ws + we));
			pTST = pSurf->MinPtW(p3dC);
			if (pTST.y > 0.5)
				pSfS->y = 1;
			else
				pSfS->y = 0;
		}
		if ((pSfE->y == 0) || (pSfE->y == 1))
		{
			p3dC = this->GetPt(1 - 0.1*(ws + we));
			pTST = pSurf->MinPtW(p3dC);
			if (pTST.y > 0.5)
				pSfE->y = 1;
			else
				pSfE->y = 0;
		}
	}
}


NCurveOnSurf* NCurve::GetSurfaceCV(NSurf* pSurf)
{
int i;
BOOL bPl;
C3dVector pt;
NCurveOnSurf* pS = new NCurveOnSurf();
pS->Drawn = Drawn;
pS->Selectable  = Selectable; 
pS->Visable  = Visable;
pS->iColour = iColour;
pS->iObjType = 13;
pS->iLabel = iLabel;
pS->iNoCPts = iNoCPts;
pS->p = p;
pS->ws = ws;
pS->we = we;
pS->pParent=pSurf;
bPl = pSurf->isPlanar();
for (i=0;i<iNoCPts+p+1;i++)
{
  pS->knots[i]=knots[i];
}

for (i=0;i<iNoCPts;i++)
{
  pt=cPts[i]->Pt_Point;
  pt=pSurf->MinPtW(pt);
  pS->cPts[i]=new CvPt_ObjectW();
  pS->cPts[i]->Create(pt,cPts[i]->w,-1,55,1,1,pS);
}
return(pS);
}

NCurveOnSurf* NCurve::GetSurfaceCV2(NSurf* pSurf)
{
int i;
C3dVector pt;
NCurveOnSurf* pS = new NCurveOnSurf();
pS->Drawn = Drawn;
pS->Selectable  = Selectable; 
pS->Visable  = Visable;
pS->iColour = iColour;
pS->iObjType = 13;
pS->iLabel = iLabel;
pS->iNoCPts = iNoCPts;
pS->p = p;
pS->ws = ws;
pS->we = we;
pS->pParent=pSurf;

for (i=0;i<iNoCPts+p+1;i++)
{
  pS->knots[i]=knots[i];
}

for (i=0;i<iNoCPts;i++)
{
  pt=cPts[i]->Pt_Point;
  pS->cPts[i]=new CvPt_ObjectW();
  pS->cPts[i]->Create(pt,cPts[i]->w,-1,55,1,1,pS);
}
return(pS);
}

//***********************************************************************
// THIS PROJECTION CHECKS THE FOLLOWING
// CASE 1) If Its a planar curve in plane of a planar surface
// CASE 2) IF Its not a planar survce are the the curve to be pojected
//         actually isoline - then the segement on the surface is a
//         line 2 points (like cylinder trims)
// CASE 3) Must poject multiple point and fit curve 2nd order.
// ELSE Fail 
//***********************************************************************

NCurveOnSurf* NCurve::GetSurfaceCV3(NSurf* pSurf)
{
	const double dIncs = 10;
	double dStp;
	dStp = (we - ws) / dIncs;
	int j;
	double dCu;
	C3dVector pt;
	NCurveOnSurf* pS = NULL;
	BOOL bIsCv = FALSE;
	BOOL bPlainar = pSurf->isPlanar();
	C3dVector p3dC;
	C3dVector pSf;
	C3dVector pSfS;
	C3dVector pSfE;
	C3dVector pTST;
	double dCnstU;
	double dCnstV;
	BOOL bCnstU;
	BOOL bCnstV;

	if (bPlainar)
	{
		pS = GetSurfaceCV(pSurf);
	}
	else
	{
		//Test for isocurve
		//pick 10 points on curves if either u or v remain constant 
		//it an iso curve 
		p3dC = this->GetPt(ws);
		pSf = pSurf->MinPtW(p3dC); 
		dCnstU = pSf.x;
		dCnstV = pSf.y;
		bCnstU = TRUE;
		bCnstV = TRUE;
		for (j = 1; j < dIncs; j++)
		{
			dCu = ws + j * dStp;
			p3dC = this->GetPt(dCu);
			pSf = pSurf->MinPtW(p3dC);
			if ((pSf.x < dCnstU - dTol) || (pSf.x > dCnstU + dTol))
				bCnstU = FALSE;
			if ((pSf.y < dCnstV - dTol) || (pSf.y > dCnstV + dTol))
				bCnstV = FALSE;
		}
		if (pSurf->iLabel == 25)
			pSurf->iLabel = 25;
		if ((bCnstU) || (bCnstV))
		{
			//outtext1("Is Iso CUrve");
			pS = new NCurveOnSurf();
			pS->iLabel = iLabel;
			pS->p = 1;
			pS->ws = ws;
			pS->we = we;
			pS->pParent = pSurf;

			p3dC = this->GetPt(0);
			pSfS = pSurf->MinPtW(p3dC);
			p3dC = this->GetPt(1);
			pSfE = pSurf->MinPtW(p3dC);
			if (bCnstU)
			{
				//check v end pts
				if (pSfS.y == pSfE.y) //We have a problem
				{
					p3dC = this->GetPt(0 + 0.1);
					pTST = pSurf->MinPtW(p3dC);
					if (pTST.y > 0.5)
						pSfS.y = 1;
					p3dC = this->GetPt(1 - 0.1);
					pTST = pSurf->MinPtW(p3dC);
					if (pTST.y > 0.5)
						pSfE.y = 1;
				}
			}
			if (bCnstV)
			{
				if (pSfS.x ==pSfE.x) //We have a problem
				{
					p3dC = this->GetPt(0 + 0.1);
					pTST = pSurf->MinPtW(p3dC);
					if (pTST.x > 0.5)
						pSfS.x = 1;
					p3dC = this->GetPt(1 - 0.1);
					pTST = pSurf->MinPtW(p3dC);
					if (pTST.x > 0.5)
						pSfE.x = 1;
				}
			}
			pS->AddVert(pSfS,1);
			pS->AddVert(pSfE, 1);
			pS->knots[0] = 0.0;
			pS->knots[1] = 0.0;
			pS->knots[2] = 1.0;
			pS->knots[3] = 1.0;
		}
	}
	return(pS);
}

//******************************************************************
//      NON PLANAR SURF NOT ISOCURVE  ***GENERAL CASE***
//		CASE 3
//******************************************************************
NCurveOnSurf* NCurve::GetSurfaceCV4(NSurf* pSurf)
{
	NCurveOnSurf* pS = NULL;
	C3dVector p3dC;
	C3dVector pSfS;
	C3dVector pSfE;
	C3dVector pTST;
	C3dVector pSfMid;
	p3dC = this->GetPt(this->ws);
	pSfS = pSurf->MinPtW(p3dC);
	p3dC = this->GetPt(0.5*(this->ws+ this->we));
	pSfMid = pSurf->MinPtW(p3dC);
	p3dC = this->GetPt(this->we);
	pSfE = pSurf->MinPtW(p3dC);
	this->EndPtChk01(pSurf, this->ws, &pSfS, this->we, &pSfE);


	pS = new NCurveOnSurf();
	pS->iLabel = iLabel;
	pS->p = 1;
	pS->ws = 0;
	pS->we = 1;
	pS->pParent = pSurf;

	pS->AddVert(pSfS, 1);
pS->AddVert(pSfMid, 1);
	pS->AddVert(pSfE, 1);
	pS->knots[0] = 0.0;
	pS->knots[1] = 0.0;
	pS->knots[2] = 0.5;
	pS->knots[3] = 1.0;
	pS->knots[4] = 1.0;
	return (pS);
}

//************************************************************************************
// Test to see if this curve is an isocurve on pSurf
// retutns -1 not an iso-curve
// returns 1 for isocurve in u
// returns 2 for isocurve in v
//************************************************************************************
int NCurve::isIsoCurve(NSurf* pSurf)
{
	int iret = -1;
	double dStp;
	dStp = (we - ws) / ISO_TEST_INC;
	int j;
	C3dVector p3dC;
	C3dVector pSf;
	C3dVector pSfS;
	C3dVector pSfE;
	C3dVector pTST;
	double dCnstU;
	double dCnstV;
	BOOL bCnstU;
	BOOL bCnstV;
	double dCu;

	//Test for isocurve
	//pick 10 points on curves if either u or v remain constant 
	//it an iso curve 
	p3dC = GetPt(ws);
	pSf = pSurf->MinPtW(p3dC);
	if ((abs(pSf.x) < PARA_NEAR_ZERO) || (abs(pSf.x - 1) < PARA_NEAR_ZERO))
		pSf.x = 0;
	dCnstU = pSf.x;
	if ((abs(pSf.y) < PARA_NEAR_ZERO) || (abs(pSf.y - 1) < PARA_NEAR_ZERO))
		pSf.y = 0;
	dCnstV = pSf.y;
	bCnstU = TRUE;
	bCnstV = TRUE;
	for (j = 1; j < ISO_TEST_INC; j++)
	{
		dCu = ws + j * dStp;
		p3dC = GetPt(dCu);
		pSf = pSurf->MinPtW(p3dC);
		if ((abs(pSf.x) < PARA_NEAR_ZERO) || (abs(pSf.x - 1) < PARA_NEAR_ZERO))
			pSf.x = 0;
		if ((abs(pSf.y) < PARA_NEAR_ZERO) || (abs(pSf.y - 1) < PARA_NEAR_ZERO))
			pSf.y = 0;
		if (abs(pSf.x - dCnstU) > PARA_NEAR_ZERO)
			bCnstU = FALSE;
		if (abs(pSf.y - dCnstV) > PARA_NEAR_ZERO)
			bCnstV = FALSE;
	}
	if (bCnstU)
		iret = 1;
	if (bCnstV)
		iret = 2;
	return(iret);
}

void NCurve::DrawCtrlPtsTog()
{
if (DrawCPts==TRUE)
{
  DrawCPts=FALSE;
}
else
{
  DrawCPts=TRUE;
}
}


void NCurve::OglDraw(int iDspFlgs,double dS1,double dS2)
{
OglDrawW(iDspFlgs,dS1,dS2);
}

void NCurve::OglDrawCtrlPts()
{
int i;
glColor3fv(cols[iColour]);
glPointSize(15.0f);
glBegin(GL_POINTS);
for (i = 0; i < iNoCPts; i++)
{
  glVertex3f((float) cPts[i]->Pt_Point->x,(float) cPts[i]->Pt_Point->y,(float) cPts[i]->Pt_Point->z);
}
glEnd();
}

void NCurve::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[80];
if ((iDspFlgs & DSP_CURVES)>0)
{
Selectable=1;
int i;
glColor3fv(cols[iColour]);
if (DrawCPts==TRUE)
{
  OglDrawCtrlPts();
}
C3dVector vPt;
C3dVector vPt2;
double dw=0;
double dSpan;
double dInc=0.02;
int iNo;

dSpan = we-ws;
if (p>0)
{
  double dt;
  dt= dSpan/dInc;
  iNo = (int) dt;
}
//else
//{
//  iNo = 2;
//  dInc = dSpan;
//}
  
vPt=GetPt(ws);
dw=ws;
glLineWidth(iLnThk);
// for dotted
if (iLnType==2)
{
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1,0x0101);
}
else if (iLnType==3)
{
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1,0x00FF);
}
//
glBegin(GL_LINES);
for (i = 0; i < iNo; i++)
{
  dw=dw+dInc;
  if (dw>1.0){dw=1;}
  vPt2=GetPt(dw);
  glVertex3f((float) vPt.x,(float) vPt.y,(float) vPt.z);
  glVertex3f((float) vPt2.x,(float) vPt2.y,(float) vPt2.z);
  vPt=vPt2;
}
glEnd();
glLineWidth(2.0);
glDisable(GL_LINE_STIPPLE);
  C3dVector vCent;
  vCent=Get_Centroid();
  if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"C%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
}
else
{
  Selectable=0;
}
}

double NCurve::CorrectW(double w)
{
return(w);
}

double NCurve::getLen()
{
int i;
C3dVector p1;
C3dVector p2;
double dw=0;
double dLen=0;
for (i=0;i<99;i++)
{
  p1=GetPt(dw);
  p2=GetPt(dw+0.01);
  p1-=p2;
  dLen+=p1.Mag();
  dw+=0.01;
}
return (dLen);
}

void NCurve::Reverse()
{
int i;
CvPt_Object* cPtsCp[MAX_CTPTS];
//Note IMPORTANT for a non uniform knot seq
//Need to reverse that to
for (i=0;i<iNoCPts;i++)
{
  cPtsCp[i] = cPts[iNoCPts-i-1];
}
for (i=0;i<iNoCPts;i++)
{
  cPts[i] = cPtsCp[i];
}
}


void NCurve::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int i;
G_Object::SetToScr(pModMat,pScrTran);
pModZ = pModMat;
pScrZ = pScrTran;
for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->SetToScr(pModMat,pScrTran);
}
//END POINTS
C3dVector vT;
if (pS==NULL)
{
  pS=new CvPt_Object();
  pS->pParent=this;
}
vT=this->GetPt(ws);
pS->Pt_Point->Set(vT.x,vT.y,vT.z);
pS->SetToScr(pModMat,pScrTran);
if (pE==NULL)
{
  pE=new CvPt_Object();
  pE->pParent=this;
}
vT=this->GetPt(we);
pE->Pt_Point->Set(vT.x,vT.y,vT.z);
pE->SetToScr(pModMat,pScrTran);
}

void NCurve::Transform(C3dMatrix TMat)
{
int i;
for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->Transform(TMat);
}
}


void NCurve::HighLight(CDC* pDC)
{
double dw=0;
double dSpan;
double dInc=0.02;
C3dVector vPt;
int iNo;
int i;

dSpan = we-ws;
double dt;
dt=dSpan/dInc;
iNo = (int) dt;
dw=ws;
vPt=GetPt(dw);
Pt_Object* ThePoint = new Pt_Object;
ThePoint->Create(vPt,1,0,0,11,0,0,NULL);
ThePoint->SetToScr(pModZ,pScrZ);
pDC->MoveTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);
for (i=0;i<iNo-1;i++)
{
  dw=dw+dInc;
  vPt=GetPt(dw);
  ThePoint->Pt_Point->x = vPt.x; 
  ThePoint->Pt_Point->y = vPt.y;
  ThePoint->Pt_Point->z = vPt.z;
  ThePoint->SetToScr(pModZ,pScrZ);
  pDC->LineTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);  
}
delete(ThePoint);
}

G_ObjectD NCurve::SelDist(CPoint InPT,Filter FIL)
{
G_ObjectD Ret;
double MinDist = 1e36;
double dDist = 0;
double dw=0;
double dSpan;
C3dVector vPt;
C3dVector vPt2;
int i;
double dInc=0.1;
double dMinW=0;
double dMinPixDist;
double dPixSpan;
const int NO_PTS = 10;
const int NO_PTS2 = 100;
BOOL bIsPt = FALSE;
Ret.pObj = NULL;
//  IF POINTS ARE SELECTABLE
if (FIL.isFilter(0))
{
	for (i = 0; i < iNoCPts; i++)
	{
		dDist = pow((cPts[i]->DSP_Point->x - InPT.x), 2) + pow((cPts[i]->DSP_Point->y - InPT.y), 2);
		if (dDist <= MinDist)
		{
			MinDist = dDist;
			if (dDist < 400)
			{
				bIsPt = TRUE;
				Ret.Dist = MinDist;
				Ret.pObj = cPts[i];
			}
		}
	}
	if (ws != 0)
	{
		dDist = pow((pS->DSP_Point->x - InPT.x), 2) + pow((pS->DSP_Point->y - InPT.y), 2);
		if (dDist <= MinDist)
		{
			MinDist = dDist;
			if (dDist < 400)
			{
				bIsPt = TRUE;
				Ret.Dist = MinDist;
				Ret.pObj = pS;
			}
		}
	}
	if (we != 1)
	{
		dDist = pow((pE->DSP_Point->x - InPT.x), 2) + pow((pE->DSP_Point->y - InPT.y), 2);
		if (dDist <= MinDist)
		{
			MinDist = dDist;
			if (dDist < 400)
			{
				bIsPt = TRUE;
				Ret.Dist = MinDist;
				Ret.pObj = pE;
			}
		}
	}
}
dMinPixDist = 10000;
//IF CURVES ARE SELECTABLE
if (!bIsPt)
{
  if (FIL.isFilter(13) ||
      FIL.isFilter(7))
   {
      Pt_Object* ThePoint = new (Pt_Object);
      ThePoint->Create(vPt, 0, 0, 0, 11, 0, 0, NULL);
      //First pass selection
      dSpan = we - ws;
      dInc = dSpan / NO_PTS;
      //calculate pixels between w increments;
      vPt = GetPt(0);
      ThePoint->Pt_Point->Set(vPt.x, vPt.y, vPt.z);
      ThePoint->SetToScr(pModZ, pScrZ);
      vPt.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
      vPt2 = GetPt(dInc);
      ThePoint->Pt_Point->Set(vPt2.x, vPt2.y, vPt2.z);
      ThePoint->SetToScr(pModZ, pScrZ);
      vPt2.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
      vPt2 -= vPt;
      dPixSpan = vPt2.Mag();
      dw = ws + dInc;
      for (i = 0; i < NO_PTS - 1; i++)
      {
        vPt = GetPt(dw);
        ThePoint->Pt_Point->Set(vPt.x, vPt.y, vPt.z);
        ThePoint->SetToScr(pModZ, pScrZ);
        dDist = pow((ThePoint->DSP_Point->x - InPT.x), 2) + pow((ThePoint->DSP_Point->y - InPT.y), 2);
        double dTTTT;
        dTTTT = pow(dDist, 0.5);
        if (dTTTT < dMinPixDist)
        {
          dMinPixDist = dTTTT;
          dMinW = dw;
        }
        if (dDist < MinDist)
        {
          MinDist = dDist;
        }
        dw = dw + dInc;
      }
      //Refine if dMinPixDist<100
      if (dMinPixDist < dPixSpan)
      {
        double dds=0;
        double dde=0;
        dds = dMinW - dInc;
        dde = dMinW + dInc;
        if (dds < ws)
          dds = ws;
        if (dde > we)
          dde = we;
        dSpan = dde - dds;
        dInc = dSpan / NO_PTS2;
        dw = dds + dInc;
        for (i = 0; i < NO_PTS2 - 1; i++)
        {
          vPt = GetPt(dw);
          ThePoint->Pt_Point->Set(vPt.x, vPt.y, vPt.z);
          ThePoint->SetToScr(pModZ, pScrZ);
          dDist = pow((ThePoint->DSP_Point->x - InPT.x), 2) + pow((ThePoint->DSP_Point->y - InPT.y), 2);
            if (dDist < MinDist)
          {
            MinDist = dDist;
            dMinW = dw;
            dMinPixDist = pow(MinDist, 0.5);
          }
          dw = dw + dInc;
        }
      }
      delete(ThePoint);
  }  
}

if (Ret.pObj==NULL)
{
  Ret.Dist=MinDist;
  Ret.pObj=this;
}
return (Ret);
}

void NCurve::S_Box(CPoint P1, CPoint P2, ObjList* pSel)
{
	int i;
	
	if (DrawCPts == TRUE)
	{
		for (i = 0; i < iNoCPts; i++)
		{
			cPts[i]->S_Box(P1, P2, pSel);
		}
	}
	else
	{
		G_Object::S_Box(P1, P2, pSel);
	}
}

C3dVector NCurve::GetPt(double w)
{
C4dVector Pt;
C4dVector PtSum;
C3dVector vR;
int iSpan;
int i;
double N[20];
if (w>1)
  w=1;
//if (w<0)
//  w=0;
iSpan = FindSpan(w);
BasisFun(p,iSpan,w,N);
PtSum.Clear();
for (i=0;i<=p;i++)
{

  Pt.xw = cPts[iSpan-p+i]->Pt_Point->x*cPts[iSpan-p+i]->w;
  Pt.yw = cPts[iSpan-p+i]->Pt_Point->y*cPts[iSpan-p+i]->w;
  Pt.zw = cPts[iSpan-p+i]->Pt_Point->z*cPts[iSpan-p+i]->w;
  Pt.w = cPts[iSpan-p+i]->w;
  PtSum=PtSum+Pt*N[i];
}
vR.x=PtSum.xw/PtSum.w;
vR.y=PtSum.yw/PtSum.w;
vR.z=PtSum.zw/PtSum.w;
return (vR);
}


C3dVector NCurve::GetDir(double w)
{
C3dVector DirA[10];
deriveAt(w,1,DirA);
return (DirA[1]);
}

void NCurve::NullPointRef()
{
	pS = NULL;
	pE = NULL;
}


IMPLEMENT_DYNAMIC(NCurveOnSurf, CObject)

//BlowsR
NCurveOnSurf::NCurveOnSurf()
{
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iColour = 75;
	iObjType = 13;
	iNoCPts = 0;
	p = 3;
	ws = 0.0;
	we = 1.0;
	DrawCPts = FALSE;
	DrawNoCvs = FALSE;
	iLnThk = 4;
	iLnType = 1;
	pSC = NULL;
	bOrient = TRUE; //Defualt agrees with space curve
}

void NCurveOnSurf::Create(int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 75;
iLabel = iLab;
iObjType = 13;
iNoCPts=0;
p=3;
ws=0.0;
we=1.0;
pParent=Parrent;
DrawCPts=FALSE;
DrawNoCvs=FALSE;
iLnThk=4;
iLnType=1;
pSC = NULL;
bOrient = TRUE; //Defualt agrees with space curve
}


NCurve* NCurveOnSurf::GetSurfaceCVG(G_Object* pSurf)
{
int i;
NSurf* pSS = (NSurf*) pSurf;

C3dVector pt;
NCurve* pS = new NCurve();
pS->Drawn = Drawn;
pS->Selectable  = Selectable; 
pS->Visable  = Visable;
pS->iColour = iColour;
pS->iObjType = 7;
pS->iLabel = iLabel;
pS->iNoCPts = iNoCPts;
pS->p = p;
pS->ws = ws;
pS->we = we;
pS->pParent=pSurf;

for (i=0;i<iNoCPts+p+1;i++)
{
  pS->knots[i]=knots[i];
}
C3dVector gPt;
for (i=0;i<iNoCPts;i++)
{
  pt=cPts[i]->Pt_Point;
  gPt=pSS->GetPt(pt.x,pt.y);
  pS->cPts[i]=new CvPt_Object();
  pS->cPts[i]->Create(gPt,cPts[i]->w,-1,55,1,1,pS);
}
return(pS);
}

C3dVector NCurveOnSurf::GetLastPt()
{
	C3dVector vLast;
	vLast.x = cPts[iNoCPts-1]->Pt_Point->x;
	vLast.y = cPts[iNoCPts-1]->Pt_Point->y;
	vLast.z = cPts[iNoCPts-1]->Pt_Point->z;
	return(vLast);
}

C3dVector NCurveOnSurf::Get_Centroid()
{
BOOL isCir;
C3dVector v1;
C3dVector v2;
C3dVector vRet;
isCir=isCircle();
if (isCir==TRUE)
{
   v1=GetPt(0.0);
   v2=GetPt(0.5);
   vRet=(v2-v1);
   vRet*=0.5;
   vRet+=v1;
}
else
{
  vRet=GetPt(0.5);
}
return (vRet);
}

BOOL NCurveOnSurf::isCircle()
{
// this need to be more of a verbose check
// doing it quickly
C3dVector v1;
C3dVector v2;
BOOL bRet;
bRet=FALSE;
double dMag;

if (iNoCPts==9)
{
  v1=cPts[0]->Pt_Point;
  v2=cPts[8]->Pt_Point;
  v1-=v2;
  dMag=v1.Mag();
  if (dMag<0.000000001)
  {
    bRet=TRUE;
  }
}
return (bRet);
}

void NCurveOnSurf::AddVert(C3dVector pInVertex1, double dWght)
{
if (iNoCPts<MAX_CVPTS)
{
  cPts [iNoCPts] = new CvPt_ObjectW();
  cPts [iNoCPts]->Create(pInVertex1,dWght,0,0,0,12,this);
  iNoCPts++;
}
else
{
  outtext1("ERROR: Max No of Points Exceeded.");
}
}

void NCurveOnSurf::OglDrawW(int iDspFlgs)
{
int i;
double dw=0;
double dSpan;
double dInc=0.02;
C3dVector vPt;
C3dVector vPt2;
double dt;
int iNo;
if (pParent!=NULL)
{
if (DrawCPts==TRUE)
{
  
  OglDrawCtrlPts();
}
  NSurf* pS= (NSurf*) pParent;
  glColor3fv(cols[iColour]);
  dSpan = we-ws;
  dt= dSpan/dInc;
  iNo = (int) dt;
  vPt=NCurve::GetPt(ws);
  vPt=pS->GetPt(vPt.x,vPt.y);
  dw=ws;
  glLineWidth(iLnThk);
  if (iLnType==2)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x0101);
  }
  else if (iLnType==3)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x00FF);
  }
  else if (iLnType==4)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5,0x0C0F);
  }
  glBegin(GL_LINES);
  for (i = 0; i < iNo; i++)
  {
    dw=dw+dInc;
    if (dw>1.0){dw=1;}
    vPt2=NCurve::GetPt(dw);
    vPt2=pS->GetPt(vPt2.x,vPt2.y);
    glVertex3f((float) vPt.x,(float) vPt.y,(float) vPt.z);
    glVertex3f((float) vPt2.x,(float) vPt2.y,(float) vPt2.z);
    vPt=vPt2;
  }
  glEnd();
  glDisable(GL_LINE_STIPPLE);
}
}


void NCurveOnSurf::OglDrawCtrlPts()
{
int i;
C3dVector vP;
glColor3fv(cols[iColour]);
glPointSize(15.0f);
glBegin(GL_POINTS);
if (pParent!=NULL)
{
  for (i = 0; i < iNoCPts; i++)
  {
    vP=cPts[i]->Get_Centroid();
    glVertex3f((float) vP.x,(float) vP.y,(float) vP.z);
  }
}
glEnd();
}
//void NCurveOnSurf::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
//{
//int i;
//pModZ = pModMat;
//pScrZ = pScrTran;
//C3dVector V;
//C3dVector R;
//NSurf* pS;
//double x,y,w;
//pS=(NSurf*) this->pParent;
//
//for (i = 0; i < iNoCPts; i++)
//{
//  x=cPts[i]->Pt_Point->x;
//  y=cPts[i]->Pt_Point->y;
//  w=cPts[i]->w;
//  R=pS->GetPt(x/w,y/w);
//
//  V.x =  pModMat->m_00 * R.x +  pModMat->m_01 * R.y +  pModMat->m_02 * R.z +  pModMat->m_30;
//  V.y =  pModMat->m_10 * R.x +  pModMat->m_11 * R.y +  pModMat->m_12 * R.z +  pModMat->m_31;
//  V.z =  pModMat->m_20 * R.x +  pModMat->m_21 * R.y +  pModMat->m_22 * R.z +  pModMat->m_32;
//  R.x =  pScrTran->m_00 * V.x +  pScrTran->m_01 * V.y +  pScrTran->m_02 * V.z +  pScrTran->m_30;
//  R.y =  pScrTran->m_10 * V.x +  pScrTran->m_11 * V.y +  pScrTran->m_12 * V.z +  pScrTran->m_31;
//  R.z =  pScrTran->m_20 * V.x +  pScrTran->m_21 * V.y +  pScrTran->m_22 * V.z +  pScrTran->m_32;
//  cPts[i]->DSP_Point->x = R.x;
//  cPts[i]->DSP_Point->y = R.y;
//  cPts[i]->DSP_Point->z = 0;
//}
//}


void NCurveOnSurf::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
int i;
G_Object::SetToScr(pModMat,pScrTran);
pModZ = pModMat;
pScrZ = pScrTran;
for (i = 0; i < iNoCPts; i++)
{
  cPts[i]->SetToScr(pModMat,pScrTran);
}
//END POINTS NOT CVPR W FOR PARAMETRIC CURVES ON SURFACE
C3dVector vT;
if (this->pParent!=NULL)
{
  if (pS==NULL)
  {
    pS=new CvPt_ObjectW();
    pS->pParent=this;
  }
  vT=this->GetParaPt(ws);
  pS->Pt_Point->Set(vT.x,vT.y,0);
  pS->SetToScr(pModMat,pScrTran);
}
  if (pE==NULL)
  {
    pE=new CvPt_ObjectW();
    pE->pParent=this;
  }
  vT=this->GetParaPt(we);
  pE->Pt_Point->Set(vT.x,vT.y,0);
  pE->SetToScr(pModMat,pScrTran);
}

C3dVector NCurveOnSurf::GetPt(double w)
{
C3dVector Pt;
if (pParent!=NULL)
{
  NSurf* pS= (NSurf*) pParent;
  Pt=NCurve::GetPt(w);
  Pt = pS->GetPt(Pt.x,Pt.y);
}
return (Pt);
}

double NCurveOnSurf::getLen()
{
	int i;
	int iInc = 50;
	double dS;
	double dL = 0;
	double dInc;
	C3dVector v1;
	C3dVector v2;
	//if (iNoCPts == 2) //Straight Line
	//	iInc = 50;
	//else
	iInc = 50;
	dS = we - ws; //ws, we start and end w values
	dInc = dS / iInc;
	dS = ws;
	for (i = 0; i < iInc; i++)
	{
		v1 = GetPt(dS);
		dS += dInc;
		v2 = GetPt(dS);
		v2 -= v1;
		dL += v2.Mag();
	}

	return (dL);
}

C3dVector NCurveOnSurf::GetParaPt(double w)
{
C3dVector Pt;
Pt=NCurve::GetPt(w);
return (Pt);
}

void NCurveOnSurf::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
    G_Object::Serialize(ar,iV);
    ar << iLnThk;
    ar << iLnType;
	ar << iNoCPts;
    ar << p;
    ar << ws;
    ar << we;
	  ar << DrawCPts;
	  for (i=0;i<iNoCPts;i++)
    {
      cPts[i]->Serialize(ar,iV);
    }
    for (i=0;i<iNoCPts+p+1;i++)
    {
      ar << knots[i];
    }
	}
	else
	{
    G_Object::Serialize(ar,iV);
	  ar >> iLnThk;
    ar >> iLnType;
	  ar >> iNoCPts;
    ar >> p;
    ar >> ws;
    ar >> we;
	  ar >> DrawCPts;
	  for (i=0;i<iNoCPts;i++)
    {
      cPts[i]=new CvPt_ObjectW();
      cPts[i]->Serialize(ar,iV);
      cPts[i]->pParent=this;
    }
    for (i=0;i<iNoCPts+p+1;i++)
    {
      ar >> knots[i];
    }
	}
}

void NCurveOnSurf::HighLight(CDC* pDC)
{
double dw=0;
double dSpan;
double dInc=0.05;
C3dVector vPt;
int iNo;
int i;
double dt;
Pt_Object* ThePoint = new Pt_Object;
if (pParent!=NULL)
{
  NSurf* pS= (NSurf*) pParent;
  dSpan = we-ws;
  dt=dSpan/dInc;
  iNo = (int) dt;
  dw=ws;
  vPt=NCurve::GetPt(dw);
  vPt=pS->GetPt(vPt.x,vPt.y);
  ThePoint->Create(vPt,1,0,0,11,0,0,NULL);
  ThePoint->SetToScr(pModZ,pScrZ);
  pDC->MoveTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);
  for (i=0;i<iNo-1;i++)
  {
    dw=dw+dInc;
    vPt=NCurve::GetPt(dw);
    vPt=pS->GetPt(vPt.x,vPt.y);
    ThePoint->Pt_Point->x = vPt.x; 
    ThePoint->Pt_Point->y = vPt.y;
    ThePoint->Pt_Point->z = vPt.z;
    ThePoint->SetToScr(pModZ,pScrZ);
    pDC->LineTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);  
  }
}
delete(ThePoint);
}

void NCurveOnSurf::Translate (C3dVector vIn)
{
}

void NCurveOnSurf::Transform(C3dMatrix TMat)
{
}

C3dVector NCurveOnSurf::MinPt(C3dVector inPt)
{
C3dVector vRet;
const double dIncs = 36;
int i;
double dStp = 1/dIncs;
double dlStp;
double dW=0;
double dMinW=0;
C3dVector cPt;
C3dVector tPt;
C3dVector cNextPt;
double dWScl;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;
double dM;
double dDot;
if (pParent!=NULL)
{
  NSurf* pS= (NSurf*) pParent;
  cPt=NCurve::GetPt(dW);
  cPt=pS->GetPt(cPt.x,cPt.y);
  tPt=NCurve::GetPt(dW+dStp);
  tPt=pS->GetPt(tPt.x,tPt.y);
  dWScl = cPt.Dist(tPt);
  for(i=0;i<dIncs;i++)
  {
    dW=i*dStp;
    cPt=NCurve::GetPt(dW);
    cPt=pS->GetPt(cPt.x,cPt.y);
    Dist=cPt.Dist(inPt);
    if (Dist<minDist)
    {
      minDist=Dist;
      dMinW=dW;
    }
  }
//start of iteration
  dW=dMinW;
  dlStp=dStp;
  i=0;
  do
  {
    cPt=NCurve::GetPt(dW);
    cPt=pS->GetPt(cPt.x,cPt.y);
    cNextPt=NCurve::GetPt(dW+0.000001);
    cNextPt=pS->GetPt(cNextPt.x,cNextPt.y);
    v1=cNextPt;
    v1-=cPt;
    v2=inPt;
    v2-=cPt;
    dM=v2.Mag();
    v1.Normalize();
    v2.Normalize();
    dDot=v2.Dot(v1);
    dlStp=(dM*dDot/dWScl)*1*dStp;
    dW=dW+dlStp;
    i++;
  }
  while ((pow((dlStp*dlStp),0.5)>dTol) && (i<100));
  vRet=NCurve::GetPt(dW);
  vRet=pS->GetPt(vRet.x,vRet.y);
}
return (vRet);
}

G_Object* NCurveOnSurf::Copy(G_Object* Parrent)
{
int i;
C3dVector pt;
NCurveOnSurf* pS = new NCurveOnSurf();
pS->Drawn = Drawn;
pS->Selectable  = Selectable; 
pS->Visable  = Visable;
pS->iColour = iColour;
pS->iObjType = 13;
pS->iLabel = iLabel;
pS->iNoCPts = iNoCPts;
pS->p = p;
pS->ws = ws;
pS->we = we;
pS->pParent=Parrent;

for (i=0;i<iNoCPts+p+1;i++)
{
  pS->knots[i]=knots[i];
}

for (i=0;i<iNoCPts;i++)
{
  pS->cPts[i]=new CvPt_Object();
  pS->cPts[i]=(CvPt_Object*) cPts[i]->Copy(Parrent);
}
return(pS);
}

void NCurveOnSurf::TrimOGL(GLUnurbsObj* pSurf)
{
int ku;
int i;
int iInd;
GLfloat* ctrlpts;
GLfloat* KnotsU;
ku=iNoCPts+p+1;
KnotsU = (GLfloat*) malloc(ku*sizeof(GLfloat));
for (i = 0;i < ku;i++)
{
 KnotsU[i] = (GLfloat) knots[i];
}
ctrlpts = (GLfloat*) malloc(3*iNoCPts*sizeof(GLfloat));
iInd=0;
double w;
for (i=0;i<iNoCPts;i++)
  {
    w=cPts[i]->w;
    ctrlpts[iInd]   = (GLfloat) (cPts[i]->Pt_Point->x*w);
    ctrlpts[iInd+1] = (GLfloat) (cPts[i]->Pt_Point->y*w);
    ctrlpts[iInd+2] = (GLfloat) w;
    iInd = iInd+3;
  }
gluNurbsCurve (pSurf, ku, KnotsU, 3, ctrlpts, p+1, GLU_MAP1_TRIM_3);
free(ctrlpts);
free(KnotsU);
}

IMPLEMENT_DYNAMIC(NCircle, CObject)

NCircle::NCircle()
{
	iType = 3;
	ws = 0;
	we = 1;
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iColour = 3;
	iLabel = -1;
	iLnThk = 4;
	iLnType = 1;
	iObjType = 7;
	dRadius = 0;
	pParent = NULL;
	iNoCPts = 0;
}

void NCircle::Create(C3dVector vN,C3dVector vC,double dRad,int iLab,G_Object* Parrent)
{
iType = 3;
ws=0;
we=1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iLnThk=4;
iLnType=1;
iObjType = 7;
dRadius = dRad;
pParent=Parrent;
iNoCPts=0;
p=2;
ws=0.0;
we=1.0;
vNorm=vN;
//Note vCent gets set after the followin transformations
//don't set it explicitly here
vCent.Set(0,0,0);
C3dVector vPt;

double r2 = 0.70710678118654752440084436210485;

vPt.x = dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = dRadius; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = 0; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = -dRadius; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = -dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = -dRadius; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = 0; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = dRadius; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);

knots[0]=0;
knots[1]=0;
knots[2]=0;

knots[3]=0.25;
knots[4]=0.25;

knots[5]=0.5;
knots[6]=0.5;

knots[7]=0.75;
knots[8]=0.75;

knots[9]=1.0;
knots[10]=1.0;
knots[11]=1.0;

C3dMatrix RMat;
RMat=RMat.CalcTran(vN);
C3dMatrix TMat;
this->Transform(RMat);
TMat.Translate(vC.x,vC.y,vC.z);
this->Transform(TMat);
vCent.Set(vC.x,vC.y,vC.z);
}

void NCircle::Create2(C3dVector vN,C3dVector vC,C3dVector vR,double dRad,int iLab,G_Object* Parrent)
{
iType = 3;
ws=0;
we=1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iLnThk=4;
iLnType=1;
iObjType = 7;
dRadius = dRad;
pParent=Parrent;
iNoCPts=0;
p=2;
ws=0.0;
we=1.0;
vNorm=vN;
vCent=vC;


C3dVector vPt;

double r2 = 0.70710678118654752440084436210485;

vPt.x = dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = dRadius; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = 0; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = -dRadius; vPt.y = dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = -dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = -dRadius; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = 0; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, 1);
vPt.x = dRadius; vPt.y = -dRadius; vPt.z = 0;
AddVert(vPt, r2);
vPt.x = dRadius; vPt.y = 0; vPt.z = 0;
AddVert(vPt, 1);

knots[0]=0;
knots[1]=0;
knots[2]=0;

knots[3]=0.25;
knots[4]=0.25;

knots[5]=0.5;
knots[6]=0.5;

knots[7]=0.75;
knots[8]=0.75;

knots[9]=1.0;
knots[10]=1.0;
knots[11]=1.0;

C3dMatrix RMat;
RMat.MakeUnit();
C3dVector vX;
C3dVector vY;
C3dVector vZ;
vZ=vN;
vX=vR;
vZ.Normalize();
vX.Normalize();
vY=vZ.Cross(vX);
RMat.SetColVec(1,vX);
RMat.SetColVec(2,vY);
RMat.SetColVec(3,vZ);
C3dMatrix TMat;
this->Transform(RMat);
TMat.Translate(vC.x,vC.y,vC.z);
this->Transform(TMat);
vNorm=vN;
vCent=vC;
}

void NCircle::Serialize(CArchive& ar,int iV)
{
NCurve::Serialize(ar,iV);
if (ar.IsStoring())
{
		// TODO: add storing code here
  vNorm.Serialize(ar,iV);
  vCent.Serialize(ar,iV);
  ar<<dRadius;
}
else
{
  vNorm.Serialize(ar,iV);
  vCent.Serialize(ar,iV);
  ar>>dRadius;
}
}

C3dVector NCircle::Get_Centroid()
{
return(vCent);
}

G_Object* NCircle::OffSet(C3dVector vN,C3dVector vDir,double Dist)
{

C3dVector p1;
C3dVector vR;
p1=this->GetPt(0);
vR=p1-vCent;
double dRad;
dRad=dRadius;
NCircle* Cir2=NULL;
vDir-=vCent;
if (vDir.Mag()>dRadius)
{
  dRad+=Dist;
  Cir2 = new NCircle;
  Cir2->Create2(vNorm,vCent,vR,dRad,1,NULL);
  Cir2->we=we;
  Cir2->ws=ws;
}
else if (vDir.Mag()<dRadius)
{
  dRad-=Dist;
  Cir2 = new NCircle;
  Cir2->Create2(vNorm,vCent,vR,dRad,1,NULL);
  Cir2->we=we;
  Cir2->ws=ws;
}

return (Cir2);
}


double NCircle::CorrectW(double w)
{
C3dVector vX,vY,vA;
double dX;
vX=this->GetPt(0);
vY=this->GetPt(0.25);
vA=this->GetPt(w);
dX=vA.Dot(vX);
return(w);
}


void NCircle::Reverse()
{
int i;
C3dVector* p[9];
double w[9];
vNorm = -vNorm;

for (i=0;i<9;i++)
{
  p[i] = cPts[i]->Pt_Point;
  w[i] =cPts[i]->w;
}
for (i=0;i<9;i++)
{
  cPts[i]->Pt_Point = p[8-i];
  cPts[i]->w = w[8-i];
}

ws=1-we;
we=1;

}

G_Object* NCircle::Copy(G_Object* Parrent)
{

NCircle* cPoly =  new NCircle();
int i;

cPoly->Drawn = Drawn;
cPoly->Selectable  = Selectable; 
cPoly->Visable  = Visable;
cPoly->iColour = iColour;
cPoly->iObjType = iObjType;
cPoly->iLabel = iLabel;
cPoly->iNoCPts = iNoCPts;
cPoly->p = p;
cPoly->ws = ws;
cPoly->we = we;
cPoly->pParent=Parrent;
cPoly->dRadius=dRadius;
for (i=0;i<iNoCPts;i++)
{
  cPoly->cPts[i]=new CvPt_Object();
  cPoly->cPts[i]=(CvPt_Object*) cPts[i]->Copy(cPoly);
}
for (i=0;i<iNoCPts+p+1;i++)
{
  cPoly->knots[i]=knots[i];
}

cPoly->vCent=vCent;
cPoly->vNorm=vNorm;
return(cPoly);
}

void NCircle::Translate(C3dVector vIn)
{
NCurve::Translate(vIn);
vCent+=vIn;
}

void NCircle::Transform(C3dMatrix TMat)
{
  NCurve::Transform(TMat);
  vCent=TMat.Mult(vCent);
  TMat.m_30=0;
  TMat.m_31=0;
  TMat.m_32=0;
  vNorm=TMat.Mult(vNorm);
}


void NCircle::Info()
{
  char S1[80];
  sprintf_s(S1, "%s", "CIRCLE OBJECT");
  outtext1(S1);
  G_Object::Info();
  sprintf_s(S1, "%s%i", "Curve Type : ", iType);
  outtext1(S1);
  sprintf_s(S1,"%s%f","Radius : ",dRadius);
  outtext1(S1); 
  sprintf_s(S1,"%s%f%s%f%s%f","Centre : ",vCent.x,",",vCent.y,",",vCent.z);
  outtext1(S1); 
  sprintf_s(S1,"%s%f%s%f%s%f","Normal : ",vNorm.x,",",vNorm.y,",",vNorm.z);
  outtext1(S1);
  outtext1("    ****");
}


IMPLEMENT_DYNAMIC(NLine, CObject)

NLine::NLine()
{
	iType = 2;
	Drawn = 0;
	Selectable = 1;
	Visable = 1;
	iColour = 3;
	iLnThk = 4;
	iLnType = 1;
	iObjType = 7;
	pParent = NULL;
	iNoCPts = 0;
	p = 1;
	ws = 0.0;
	we = 1.0;
}

void NLine::Create(C3dVector vP1,C3dVector vP2,int iLab,G_Object* Parrent)
{
iType = 2;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 3;
iLabel = iLab;
iLnThk=4;
iLnType=1;
iObjType = 7;
pParent=Parrent;
iNoCPts=0;
p=1;
ws=0.0;
we=1.0;


AddVert(vP1, 1);
AddVert(vP2, 1);

knots[0]=0.0;
knots[1]=0.0;
knots[2]=1.0;
knots[3]=1.0;
}




void NLine::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
int i;
char sLab[20];
C3dVector vC;

if ((iDspFlgs & DSP_CURVES)>0)
{
	if (DrawCPts == TRUE)
	{
		OglDrawCtrlPts();
	}
  Selectable=1;
  glColor3fv(cols[iColour]);
  C3dVector vPt1;
  C3dVector vPt2;
  vPt1=GetPt(ws);
  vPt2=GetPt(we);
  glLineWidth((float) iLnThk);
  if (iLnType==2)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x0101);
  }
  else if (iLnType==3)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x00FF);
  }
  else if (iLnType==4)
  {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5,0x0C0F);
  }
  glBegin(GL_LINES);
  for (i = 0; i < iNoCPts-1; i++)
  {
    glVertex3f((float) vPt1.x,(float) vPt1.y,(float) vPt1.z);
    glVertex3f((float) vPt2.x,(float) vPt2.y,(float) vPt2.z);
  }
  glEnd();
  glLineWidth(2.0);
  glDisable(GL_LINE_STIPPLE);
  vC=this->Get_Centroid();
  if (bDrawLab==TRUE)
  {
    sprintf_s(sLab,"Ln%i",iLabel);
    OglString(iDspFlgs,(float) vC.x,(float) vC.y,(float) vC.z,&sLab[0]);
  }
}
else
{
  Selectable=0;
}
}



void NLine::HighLight(CDC* pDC)
{
double dw=0;
double dSpan;
double dInc=0.02;
C3dVector vPt;
int iNo;
dSpan = we-ws;
double dt;
dt=dSpan/dInc;
iNo = (int) dt;
dw=ws;
vPt=GetPt(ws);
Pt_Object* ThePoint = new Pt_Object;
ThePoint->Create(vPt,1,0,0,11,0,0,NULL);
ThePoint->SetToScr(pModZ,pScrZ);
pDC->MoveTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);
vPt=GetPt(we);
ThePoint->Pt_Point->x = vPt.x; 
ThePoint->Pt_Point->y = vPt.y;
ThePoint->Pt_Point->z = vPt.z;
ThePoint->SetToScr(pModZ,pScrZ);
pDC->LineTo((int) ThePoint->DSP_Point->x,(int)ThePoint->DSP_Point->y);  

delete(ThePoint);
}


C3dVector NLine::MinPt(C3dVector inPt)
{
C3dVector vRet;
C3dVector v1;
C3dVector v2;
C3dVector P1;
C3dVector P2;
double dm;
double dDot;
P1.Set(cPts[0]->Pt_Point->x,
       cPts[0]->Pt_Point->y,
       cPts[0]->Pt_Point->z);
P2.Set(cPts[1]->Pt_Point->x,
       cPts[1]->Pt_Point->y,
       cPts[1]->Pt_Point->z);
v1=P2;
v1-=P1;
v2=inPt;
v2-=P1;
dm=v2.Mag();
v1.Normalize();
v2.Normalize();
dDot=v2.Dot(v1);
dm=dm*dDot;
vRet=P1+v1*dm;
return vRet;
}

void NLine::Info()
{
char S1[80];
sprintf_s(S1, "%s", "LINE OBJECT");
outtext1(S1);
G_Object::Info();
sprintf_s(S1, "%s%i", "Curve Type : ", iType);
outtext1(S1);
//Set like this temporaly for generating symbols table
sprintf_s(S1,"Pt1: %f %f %f",cPts[0]->Pt_Point->x,cPts[0]->Pt_Point->y,cPts[0]->Pt_Point->z);
outtext1(S1); 
sprintf_s(S1,"Pt2: %f %f %f",cPts[1]->Pt_Point->x,cPts[1]->Pt_Point->y,cPts[1]->Pt_Point->z);
outtext1(S1); 

}

G_Object* NLine::Copy(G_Object* Parrent)
{
int i;
NLine* cPoly = new NLine();
cPoly->iType = iType;
cPoly->Drawn = Drawn;
cPoly->Selectable  = Selectable; 
cPoly->Visable  = Visable;
cPoly->iColour = iColour;
cPoly->iObjType = iObjType;
cPoly->iLabel = iLabel;
cPoly->iNoCPts = iNoCPts;
cPoly->p = p;
cPoly->ws = ws;
cPoly->we = we;
cPoly->pParent=Parrent;

for (i=0;i<iNoCPts;i++)
{
  cPoly->cPts[i]=new CvPt_Object();
  cPoly->cPts[i]=(CvPt_Object*) cPts[i]->Copy(cPoly);
}
for (i=0;i<iNoCPts+p+1;i++)
{
  cPoly->knots[i]=knots[i];
}
return (cPoly);
}

double NLine::MinDist(C3dVector inPt)
{
C3dVector vRet;
double dX,dY,dZ,dU;
int iCnt1;
double MinDist = 100000000000000.0;
double dDist = 0;

for (iCnt1 = 1; iCnt1 < 1000; iCnt1++)
{
dU = iCnt1*0.001;
dX = (cPts[1]->Pt_Point->x-cPts[0]->Pt_Point->x)*dU;
dY = (cPts[1]->Pt_Point->y-cPts[0]->Pt_Point->y)*dU;
dZ = (cPts[1]->Pt_Point->z-cPts[0]->Pt_Point->z)*dU;
vRet.x = cPts[0]->Pt_Point->x + dX;
vRet.y = cPts[0]->Pt_Point->y + dY;
vRet.z = cPts[0]->Pt_Point->z + dZ;
dDist = pow((pow((vRet.x - inPt.x),2)+pow((vRet.y - inPt.y),2)+pow((vRet.z - inPt.z),2)),0.5);
if (dDist < MinDist)
  {
  MinDist = dDist;
  }
}
return (MinDist);
}


double NLine::getLen()
{
C3dVector p1;
C3dVector p2;
p1=GetPt(0);
p2=GetPt(1);
p1-=p2;
return (p1.Mag());
}

G_Object* NLine::OffSet(C3dVector vN,C3dVector vDir,double Dist)
{

C3dVector p1;
C3dVector p2;

p1.Set(cPts[0]->Pt_Point->x,
	     cPts[0]->Pt_Point->y,
	     cPts[0]->Pt_Point->z);

p2.Set(cPts[1]->Pt_Point->x,
	     cPts[1]->Pt_Point->y,
	     cPts[1]->Pt_Point->z);


C3dVector v1;
C3dVector v2;
C3dVector v1o;
v1=p2; v1-=p1;
v2=vDir; v2-=p1;
v2.Normalize();
v1o=v1.Cross(vN);
v1o.Normalize();
v1o*=Dist;

if (v1o.Dot(v2)<0)
  v1o*=-1;

NLine* Ln2 = new NLine;
Ln2->Create(p1+v1o,p2+v1o,1,NULL);
return (Ln2);
}

IMPLEMENT_DYNAMIC(NSurf, CObject)

NSurf ::~NSurf ()
{
int i;
for (i=0;i<iNoCvs;i++)
{
  delete(pCVsU [i]);
  pCVsU [i]=NULL;
}

if (pCVsV[0]!=NULL)
{
  //Note the control point have already been deleted above
  //dont delete again
  pCVsV[0]->iNoCPts=0;
  delete(pCVsV[0]);
  pCVsV[0]=NULL;
}
if (pCVsV[1]!=NULL)
{
  //Note the control point have already been deleted above
  //dont delete again
  pCVsV[1]->iNoCPts=0;
  delete(pCVsV[1]);
  pCVsV[1]=NULL;
}
for (i=0;i<iNoTrimCvs;i++)
{
  delete(pSurfCvs [i]);
  pSurfCvs [i]=NULL;
}
iNoTrimCvs=0;
iNoExtCvs=0;
}


NSurf::NSurf()
{
iType = 1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = -1;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
dSSize = -1;
}


    // CvPt_Object* pVertex[20];
void NSurf::Create(int iLab,G_Object* Parrent)
{
iType = 1;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = iLab;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pParent=Parrent;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
iNoIntLoops=0;
dSSize = -1;

}


G_Object* NSurf::Copy(G_Object* Parrent)
{
int i,j;
NSurf* pSurf = new NSurf();
pSurf->iType = iType;
pSurf->Drawn = Drawn;
pSurf->Selectable  = Selectable; 
pSurf->Visable  = Visable;
pSurf->iColour = iColour;
pSurf->iObjType = iObjType;
pSurf->iLabel = iLabel;
pSurf->iNoCvs = iNoCvs;
pSurf->pU = pU;
pSurf->pV = pV;
pSurf->pParent=Parrent;
pSurf->iNoTrimCvs=iNoTrimCvs;
pSurf->dSSize = dSSize;

for (i=0;i<iNoCvs;i++)
{
  pSurf->pCVsU [i] = (NCurve*) pCVsU[i]->Copy(pSurf);
}
for (i=0;i<2;i++)
{
  pSurf->pCVsV [i] = (NCurve*) pCVsV[i]->Copy(pSurf);
}
for (i=0;i<iNoTrimCvs;i++)
{
  pSurf->pSurfCvs [i] = (NCurveOnSurf*) pSurfCvs[i]->Copy(pSurf);
}
pSurf->iNoTrimCvs=iNoTrimCvs;
for (i=0;i<iNoExtCvs;i++)
{
  pSurf->pExtLoop [i] = (NCurveOnSurf*) pExtLoop[i]->Copy(pSurf);
}
pSurf->iNoExtCvs=iNoExtCvs;
for (i=0;i<iNoIntLoops;i++)
{
  pSurf->iNoIntCvs [i] = iNoIntCvs[i];
}
pSurf->iNoIntLoops=iNoIntLoops;
for (i=0;i<iNoIntLoops;i++)
{
  for(j=0;j<iNoIntCvs[i];j++)
  {
    pSurf->pIntLoop[i][j]=(NCurveOnSurf*) pIntLoop[i][j]->Copy(pSurf);
  }
}
return (pSurf);
}

void NSurf::Info()
{
	char S1[80];
	sprintf_s(S1, "%s", "SURFACE OBJECT");
	outtext1(S1);
	G_Object::Info();
	sprintf_s(S1, "%s%i", "Surface Type : ", iType);
	outtext1(S1);
	sprintf_s(S1, "%s%f", "Surface Mesh Size : ", dSSize);
	outtext1(S1);
	int i;
	int j;
	for (i=0;i<iNoExtCvs;i++)
		pExtLoop[i]->Info();
	outtext1("********** INTERNAL LOOPS**********");
	for (i = 0; i < iNoIntLoops; i++)
	{
		sprintf_s(S1, "%s%i", "LOOP : ", i);
		outtext1(S1);
		for (j = 0; j < iNoIntCvs[i]; j++)
		{
			pIntLoop[i][j]->Info();
		}
	}

}

void NSurf::Copy2(NSurf* pSurf)
{
int i,j;
pSurf->iType = iType;
pSurf->Drawn = Drawn;
pSurf->Selectable  = Selectable; 
pSurf->Visable  = Visable;
pSurf->iColour = iColour;
pSurf->iObjType = iObjType;
pSurf->iLabel = iLabel;
pSurf->iNoCvs = iNoCvs;
pSurf->pU = pU;
pSurf->pV = pV;
pSurf->pParent=NULL;
pSurf->iNoTrimCvs=iNoTrimCvs;
pSurf->dSSize = dSSize;
for (i=0;i<iNoCvs;i++)
{
  pSurf->pCVsU [i] = (NCurve*) pCVsU[i]->Copy(pSurf);
}
for (i=0;i<2;i++)
{
  pSurf->pCVsV [i] = (NCurve*) pCVsV[i]->Copy(pSurf);
}
for (i=0;i<iNoTrimCvs;i++)
{
  pSurf->pSurfCvs [i] = (NCurveOnSurf*) pSurfCvs[i]->Copy(pSurf);
}
pSurf->iNoTrimCvs=iNoTrimCvs;
for (i=0;i<iNoExtCvs;i++)
{
  pSurf->pExtLoop [i] = pExtLoop[i];
}
pSurf->iNoExtCvs=iNoExtCvs;
for (i=0;i<iNoIntLoops;i++)
{
  pSurf->iNoIntCvs [i] = iNoIntCvs[i];
}
pSurf->iNoIntLoops=iNoIntLoops;
for (i=0;i<iNoIntLoops;i++)
{
  for(j=0;j<iNoIntCvs[i];j++)
  {
    pSurf->pIntLoop[i][j]=pIntLoop[i][j];
  }
}
}

C3dVector NSurf::Get_Centroid()
{
return (GetPt(0.5,0.5));
}


void NSurf::Serialize(CArchive& ar,int iV)
{
	int i,j;
	int iObj;
	int iSObj;
	if (ar.IsStoring())
	{
	// TODO: add storing code here
	  G_Object::Serialize(ar,iV);
	  ar << dSSize;
	  ar << iNoCvs;
	  ar << pU;
	  ar << pV;
	  ar << DrawCPts;
	  ar << iNoTrimCvs;
	  ar << iNoExtCvs;
	  ar << iNoIntLoops;
  
	  for (i=0;i<iNoCvs;i++)
	  {
		ar << pCVsU[i]->iObjType;
		ar << pCVsU[i]->iType;
		pCVsU[i]->Serialize(ar,iV);
	  }
	  for (i=0;i<2;i++)
	  {
		ar << pCVsV[i]->iObjType;
		ar << pCVsV[i]->iType;
		pCVsV[i]->Serialize(ar,iV);
	  }
	  for (i=0;i<iNoTrimCvs;i++)
	  {
		ar << pSurfCvs[i]->iObjType;
		ar << pSurfCvs[i]->iType;
		pSurfCvs[i]->Serialize(ar,iV);
	  }
	  for (i=0;i<iNoExtCvs;i++)
	  {
		ar << pExtLoop[i]->iObjType;
		ar << pExtLoop[i]->iType;
		//Reattach space curves
		if (pExtLoop[i]->pSC != NULL)
			ar << pExtLoop[i]->pSC->iLabel;
		else
			ar << -1;

		pExtLoop[i]->Serialize(ar,iV);;
	  }
	  for (i=0;i<iNoIntLoops;i++)
	  {
		ar<<iNoIntCvs[i];
	  }
	  for (i=0;i<iNoIntLoops;i++)
	  {
		for(j=0;j<iNoIntCvs[i];j++)
		{
			ar << pIntLoop[i][j]->iObjType;
			ar << pIntLoop[i][j]->iType;
			if (pIntLoop[i][j]->pSC != NULL)
				ar << pIntLoop[i][j]->pSC->iLabel;
			else
				ar << -1;
		  pIntLoop[i][j]->Serialize(ar,iV);;
		}
	  }
	}
	else
	{
		int iSC;
		G_Object::Serialize(ar,iV);
		ar >> dSSize;
		ar >> iNoCvs;
		ar >> pU;
		ar >> pV;
		ar >> DrawCPts;
		ar>>iNoTrimCvs;
		ar>>iNoExtCvs;
		ar>>iNoIntLoops;
		for (i=0;i<iNoCvs;i++)
		{
			ar >> iObj;
			ar >> iSObj;
			if (iSObj==1)
			{
				pCVsU[i]= new NCurve();
			}
			else if (iSObj==3)
			{
				pCVsU[i]= new NCircle();
			}
			else if (iSObj==2)
			{
				pCVsU[i]= new NLine();
			}
			pCVsU[i]->Serialize(ar,iV);
			pCVsU[i]->pParent=this;
		}
		for (i=0;i<2;i++)
		{
			ar >> iObj;
			ar >> iSObj;
			if (iSObj==1)
			{
				pCVsV[i]= new NCurve();
			}
			else if (iSObj==3)
			{
				pCVsV[i]= new NCircle();
			}
			else if (iSObj==2)
			{
				pCVsV[i]= new NLine();
			}
			pCVsV[i]->Serialize(ar,iV);
			pCVsV[i]->pParent=this;
		}
		for (i=0;i<iNoTrimCvs;i++)
		{
			ar >> iObj;
			ar >> iSObj;
			pSurfCvs[i]= new NCurveOnSurf();
			pSurfCvs[i]->Serialize(ar,iV);
			pSurfCvs[i]->pParent=this;
		}
		for (i=0;i<iNoExtCvs;i++)
		{
			ar >> iObj;
			ar >> iSObj;
			ar >> iSC;
			pExtLoop[i]= new NCurveOnSurf();
			pExtLoop[i]->Serialize(ar,iV);
			pExtLoop[i]->pParent=this;
			pExtLoop[i]->pSC = NULL;
			if (iSC != -1)
			{
				if (this->pParent != NULL)
				{
					if (this->pParent->iObjType == 20)
					{
						Part* pPt = (Part*)this->pParent;
						pExtLoop[i]->pSC = (NCurve*) pPt->pPartC.Get(iSC);
					}
				}
			}
		}
		for (i=0;i<iNoIntLoops;i++)
		{
			ar>>iNoIntCvs[i];
		}
		for (i=0;i<iNoIntLoops;i++)
		{
			for(j=0;j<iNoIntCvs[i];j++)
			{
				ar >> iObj;
				ar >> iSObj;
				ar >> iSC;
				pIntLoop[i][j]= new NCurveOnSurf();
				pIntLoop[i][j]->Serialize(ar,iV);
				pIntLoop[i][j]->pParent=this;
				if (iSC != -1)
				{
					if (this->pParent != NULL)
					{
						if (this->pParent->iObjType == 20)
						{
							Part* pPt = (Part*)this->pParent;
							pIntLoop[i][j]->pSC = (NCurve*)pPt->pPartC.Get(iSC);
						}
					}
				}
			}
		}
	}
}

void NSurf::GetBoundingUV(double& MinU,
                          double& MinV,
                          double& SpanU,
                          double& SpanV)
{
  int i;
  double MaxU;
  double MaxV;
  //initiatal values
  if (iNoExtCvs<=1)  //No user trim
  {
    MinU=0;
    MinV=0;
    SpanU=1.0;
    SpanV=1.0;
  }
  else
  {
    NCurveOnSurf* pC = (NCurveOnSurf*) pExtLoop[0];
    MinU=1.0;
    MinV=1.0;
    MaxU=0;
    MaxV=0;
    for (i=0;i<iNoExtCvs;i++)
    {
       pC = (NCurveOnSurf*) pExtLoop[i];
       C3dVector pI=pC->GetParaPt(pC->ws);
       if (pI.x<MinU)
         MinU=pI.x;
       if (pI.y<MinV)
         MinV=pI.y;
       if (pI.x>MaxU)
         MaxU=pI.x;
       if (pI.y>MaxV)
         MaxV=pI.y;
    }
    SpanU=MaxU-MinU;
    SpanV=MaxV-MinV;
  }
}


void NSurf::AddCV(NCurve* inCV)
{
if (iNoCvs<MAX_CVS)
{
  pCVsU[iNoCvs] = (NCurve*) inCV->Copy(this);
  iNoCvs++;
}
else
{
  outtext1("ERROR: Max No of Curves in Surface Exceeded.");
}
}

void NSurf::RelTo(G_Object* pThis, ObjList* pList, int iType)
{
	int i;
	if (iType == 7) //Apace Curves
	{
		for (i = 0; i < iNoExtCvs; i++)
		{
			if (pExtLoop[i]->pSC != NULL)
				pList->AddEx(pExtLoop[i]->pSC);
		}
	}
}

void NSurf::OglDrawW(int iDspFlgs,double dS1,double dS2)
{
char sLab[20];
double dUi;
double dVi;
double dSpanU;
double dSpanV;
C3dVector vCent;
//UMarker direction Postition
C3dVector vU;
C3dVector vUDir;
C3dVector vVDir;
C3dVector vZDir;
C3dVector vO;
C3dVector vO1;

if ((iDspFlgs & DSP_SURFACES)>0)
{
  int i;
  int j;
  Selectable=1;

  vUDir=this->GetPt(0.25,0.0);
  vCent=Get_Centroid();
  vO.Set(0,0,0);

  this->GetBoundingUV(dUi, dVi, dSpanU, dSpanV);
  vO = this->GetPt(dUi + dSpanU / 2, dVi + dSpanV/2);
  vUDir = this->GetPt(dUi + dSpanU / 2+0.025, dVi + dSpanV / 2);
  vVDir = this->GetPt(dUi + dSpanU / 2 , dVi + dSpanV / 2 + 0.025);
  //vUDir = this->GetPt(dUi + dSpanU / 2 + 0.25, dVi + dSpanV / 2);
  //vVDir = this->GetPt(dUi + dSpanU / 2, dVi + dSpanV / 2 + 0.25);
  vUDir -= vO;
  vVDir -= vO;
  vUDir.Normalize();
  vVDir.Normalize();
  vZDir = vUDir.Cross(vVDir);
  for (i=0;i<iNoTrimCvs;i++)
  {
    if ((iDspFlgs & DSP_SURC) > 0)
    {
      pSurfCvs[i]->Selectable=1;
      pSurfCvs[i]->OglDrawW(DSP_CURVES);
    }
    else
    {
      pSurfCvs[i]->Selectable=0;
    }
  }
  //DRAW EXTERNAL TRIM LOOP
  if ((iDspFlgs & DSP_SURC) > 0)
  {
	  for (i = 0; i < iNoExtCvs; i++)
	  {
		  pExtLoop[i]->OglDrawW(DSP_CURVES);
		  pExtLoop[i]->Selectable = 1;
	  }
	  //DRAW INTERNAL TRIM LOOP
	  if (this->iNoIntCvs > 0)
	  {
		  for (i = 0; i < iNoIntLoops; i++)
		  {
			  for (j = 0; j < iNoIntCvs[i]; j++)
			  {
				  pIntLoop[i][j]->OglDrawW(DSP_CURVES);
				  pIntLoop[i][j]->Selectable = 1;
			  }
		  }
	  }
  }


  //*********************************************************************************
  //Remove to just render lines
  //glLineWidth(2.0);
  //nu = pCVsU[0]->iNoCPts;
  //ku = nu + pU + 1;
  //nv = iNoCvs;
  //kv = nv + pV + 1;

  //KnotsU = (GLfloat*)malloc(ku*sizeof(GLfloat));
  //for (icu = 0; icu < ku; icu++)
  //{
  //  KnotsU[icu] = (float)pCVsU[0]->knots[icu];
  //}
  //KnotsV = (GLfloat*)malloc(kv*sizeof(GLfloat));
  //for (icu = 0; icu < kv; icu++)
  //{
  //  KnotsV[icu] = (float)pCVsV[0]->knots[icu];
  //}

  //int iInd = 0;

  //double dW;
  //ctrlpts = (GLfloat*)malloc(4 * nv*nu*sizeof(GLfloat));
  //for (i = 0; i<nv; i++)
  //{
  //  for (j = 0; j<nu; j++)
  //  {
  //    dW = pCVsU[i]->cPts[j]->w;
  //    ctrlpts[iInd] = (float)pCVsU[i]->cPts[j]->Pt_Point->x*(float)dW;
  //    ctrlpts[iInd + 1] = (float)pCVsU[i]->cPts[j]->Pt_Point->y*(float)dW;
  //    ctrlpts[iInd + 2] = (float)pCVsU[i]->cPts[j]->Pt_Point->z*(float)dW;
  //    ctrlpts[iInd + 3] = (float)dW;
  //    iInd = iInd + 4;
  //  }
  //}


  //GLUnurbsObj* pSurf;
  //pSurf = gluNewNurbsRenderer();
  //gluNurbsProperty(pSurf, GLU_SAMPLING_METHOD, GLU_PARAMETRIC_ERROR);
  //gluNurbsProperty(pSurf, GLU_PARAMETRIC_TOLERANCE, 0.05f);
  //gluNurbsProperty(pSurf, GLU_DISPLAY_MODE, GLU_OUTLINE_PATCH);
  //gluBeginSurface(pSurf);
  //gluNurbsSurface(pSurf,
  //  ku,
  //  KnotsU,
  //  kv,
  //  KnotsV,
  //  4,
  //  4 * nu,
  //  ctrlpts,
  //  pU + 1,
  //  pV + 1,
  //  GL_MAP2_VERTEX_4);


  //gluBeginTrim(pSurf);
  //  for (i = 0; i<iNoExtCvs; i++)
  //  {
  //    pExtLoop[i]->TrimOGL(pSurf);
  //  }
  //gluEndTrim(pSurf);
  //gluEndSurface(pSurf);
  //gluDeleteNurbsRenderer(pSurf);
  //free(ctrlpts);
  //free(KnotsU);
  //free(KnotsV);

  //*********************************************************************************



  //Draw the U marker
  if ((iDspFlgs & DSP_SURFU) == 0)
  {
    C3dVector p2;
	C3dVector p3;
	C3dVector p4;
	double X, Y, Z;
    p2=vO;
    p2 += vUDir*dS1;
	p3 = vO;
	p3 += vVDir * dS1;
	p4 = vO;
	p4 += vZDir * dS1;
    glColor3fv(cols[iColour]);
	//FIRST ARROW
	C3dVector Pts[7];
	C3dMatrix mT;
	X = p2.x; Y = p2.y; Z = p2.z;
	mT = vUDir.GetTMat();
	for (i = 0; i < 7; i++)
	{
		Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
		//Pts[i] -= vOff;
		Pts[i] = mT.Mult(Pts[i]);
		Pts[i] *= 0.2*dS1;
	}
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glEnd();
    glBegin(GL_LINES);
      glVertex3f((float) vO.x,(float) vO.y,(float) vO.z);
      glVertex3f((float) p2.x,(float) p2.y,(float) p2.z);
    glEnd();
    glRasterPos3f ((float)Pts[0].x+X,(float)Pts[0].y+Y,(float)Pts[0].z+Z);
    glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPU);
	//END FIRST ARROW
	//SECOND ARROW
	X = p3.x; Y = p3.y; Z = p3.z;
	mT = vVDir.GetTMat();
	for (i = 0; i < 7; i++)
	{
		Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
		//Pts[i] -= vOff;
		Pts[i] = mT.Mult(Pts[i]);
		Pts[i] *= 0.2*dS1;
	}
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f((float)vO.x, (float)vO.y, (float)vO.z);
	glVertex3f((float)p3.x, (float)p3.y, (float)p3.z);
	glEnd();
	glRasterPos3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPV);
	//END SECOND ARROW
	//THIRD Z ARROW
	X = p4.x; Y = p4.y; Z = p4.z;
	mT = vZDir.GetTMat();
	for (i = 0; i < 7; i++)
	{
		Pts[i].Set(AHead[i][0], AHead[i][1], AHead[i][2]);
		//Pts[i] -= vOff;
		Pts[i] = mT.Mult(Pts[i]);
		Pts[i] *= 0.2*dS1;
	}
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glVertex3f((float)Pts[2].x + X, (float)Pts[2].y + Y, (float)Pts[2].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glVertex3f((float)Pts[3].x + X, (float)Pts[3].y + Y, (float)Pts[3].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glVertex3f((float)Pts[4].x + X, (float)Pts[4].y + Y, (float)Pts[4].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glVertex3f((float)Pts[5].x + X, (float)Pts[5].y + Y, (float)Pts[5].z + Z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glVertex3f((float)Pts[1].x + X, (float)Pts[1].y + Y, (float)Pts[1].z + Z);
	glVertex3f((float)Pts[6].x + X, (float)Pts[6].y + Y, (float)Pts[6].z + Z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f((float)vO.x, (float)vO.y, (float)vO.z);
	glVertex3f((float)p4.x, (float)p4.y, (float)p4.z);
	glEnd();
	glRasterPos3f((float)Pts[0].x + X, (float)Pts[0].y + Y, (float)Pts[0].z + Z);
	glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, BMPZ);
  }

  if (DrawCPts==TRUE)
  {
    for (i = 0; i < iNoCvs; i++)
    {
      pCVsU[i]->OglDrawCtrlPts();
    }
  }
  if (bDrawLab==TRUE)
	{
	  sprintf_s(sLab,"S%i",iLabel);
	  OglString(iDspFlgs,vCent.x,vCent.y,vCent.z,&sLab[0]);
	}
}
else
{
  Selectable=0;
}
}




void NSurf::OglDraw(int iDspFlgs,double dS1,double dS2)
{
//Below draw the curve using the open gl nurbs render


//remove the statement and put some where where it only gets called once
//*******************************************************************
//CleanExternalTrim();

if ((iDspFlgs & DSP_SURFACES)>0)
{
  Selectable=1;
  GLfloat* ctrlpts;
  GLfloat* KnotsU;
  GLfloat* KnotsV;
  GLfloat knots[6] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0};
  GLfloat edgePt[5][2] = /* counter clockwise */
      {{0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, 
       {0.0, 0.0}};


int nu;
int nv;
int ku;
int kv;
int icu;

glColor3fv(cols[iColour]); //Solid Colour
if (pParent != NULL)
{
	if (pParent->iObjType == 20)
	{
		Part* pPart = (Part*)pParent;
		float R = cols[pPart->iColour][0];
		float G = cols[pPart->iColour][1];
		float B = cols[pPart->iColour][2];
		float A = pPart->Alpha;
		glColor4f(R, G, B, A);
	}
}

nu = pCVsU[0]->iNoCPts;
ku=nu+pU+1;
nv = iNoCvs;
kv=nv+pV+1;

KnotsU = (GLfloat*) malloc(ku*sizeof(GLfloat));
for (icu = 0;icu < ku;icu++)
{
  KnotsU[icu] = (GLfloat) pCVsU[0]->knots[icu];
}
KnotsV = (GLfloat*) malloc(kv*sizeof(GLfloat));
for (icu = 0;icu < kv;icu++)
{
  KnotsV[icu] = (GLfloat) pCVsV[0]->knots[icu];
}

int iInd = 0;
int j;
int i;
double dW;
ctrlpts = (GLfloat*) malloc(4*nv*nu*sizeof(GLfloat));
for (i=0;i<nv;i++)
{
  for (j=0;j<nu;j++)
  {
    dW = pCVsU[i]->cPts[j]->w;
    ctrlpts[iInd]   = (GLfloat) pCVsU[i]->cPts[j]->Pt_Point->x*(GLfloat) dW;
    ctrlpts[iInd+1] = (GLfloat) pCVsU[i]->cPts[j]->Pt_Point->y*(GLfloat) dW;
    ctrlpts[iInd+2] = (GLfloat) pCVsU[i]->cPts[j]->Pt_Point->z*(GLfloat) dW;
    ctrlpts[iInd+3] = (GLfloat) dW;
    iInd = iInd+4;
  }
}


GLUnurbsObj* pSurf;
pSurf=gluNewNurbsRenderer();
//gluNurbsProperty(pSurf, GLU_SAMPLING_METHOD,GLU_PARAMETRIC_ERROR);
gluNurbsProperty(pSurf, GLU_SAMPLING_METHOD,GLU_DOMAIN_DISTANCE);
if (pU==1)
gluNurbsProperty(pSurf, GLU_U_STEP, 18);
else
gluNurbsProperty(pSurf,GLU_U_STEP,18);
if (pV == 1)
gluNurbsProperty(pSurf, GLU_V_STEP, 18);
else
gluNurbsProperty(pSurf,GLU_V_STEP,18);
//gluNurbsProperty(pSurf, GLU_SAMPLING_METHOD, GLU_PATH_LENGTH);
//gluNurbsProperty(pSurf, GLU_PARAMETRIC_TOLERANCE,0.01f);
//gluNurbsProperty(pSurf, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON); //GLU_FILL  GLU_OUTLINE_POLYGON

gluNurbsProperty(pSurf, GLU_CULLING, GLU_FALSE);
gluBeginSurface(pSurf);       
      gluNurbsSurface(pSurf,  
                      ku,  
                      KnotsU,  
                      kv,  
                      KnotsV,  
                      4,  
                      4*nu,  
                      ctrlpts,  
                      pU+1,  
                      pV+1,  
                      GL_MAP2_VERTEX_4); 
  //gluBeginTrim (pSurf);
  //    gluPwlCurve (pSurf, 5, &edgePt[0][0], 2,
  //                 GLU_MAP1_TRIM_2);
  // gluEndTrim (pSurf);

gluBeginTrim (pSurf);
for (i=0;i<iNoExtCvs;i++)
{
  ////gluPwlCurve (pSurf, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
   pExtLoop[i]->TrimOGL(pSurf);
}
gluEndTrim (pSurf);
//internal loop
if (iNoIntLoops>0)
{
  for (i=0;i<iNoIntLoops;i++)
  {
    gluBeginTrim (pSurf);
    for (j=0;j<iNoIntCvs[i];j++)
    {
      pIntLoop[i][j]->TrimOGL(pSurf);
    }
    gluEndTrim (pSurf);
  }
}
gluEndSurface(pSurf); 
gluDeleteNurbsRenderer(pSurf);
free(ctrlpts);
free(KnotsU);
free(KnotsV);
}
else
{
  Selectable=0;
}
}


void NSurf::SetToScr(C3dMatrix* pModMat,C3dMatrix* pScrTran)
{
pModZ = pModMat;
pScrZ = pScrTran;
G_Object::SetToScr( pModMat,pScrTran);
int i;
int j;
for (i = 0; i < iNoCvs; i++)
{
  pCVsU[i]->SetToScr(pModMat,pScrTran);
}
pCVsV[0]->SetToScr(pModMat,pScrTran);
pCVsV[1]->SetToScr(pModMat,pScrTran);

  for (i = 0; i < iNoTrimCvs; i++)
  {
    pSurfCvs[i]->SetToScr(pModMat,pScrTran);
  }
  for (i=0;i<iNoExtCvs;i++)
  {
    //Here
    pExtLoop[i]->SetToScr(pModMat,pScrTran);
  }
  for (i=0;i<iNoIntLoops;i++)
  {
    for(j=0;j<iNoIntCvs[i];j++)
    {
	  //Here
      pIntLoop[i][j]->SetToScr(pModMat,pScrTran);
    }
  }


}

void NSurf::NullCurveRef()
{
	int i;
	int j;
	for (i = 0; i < iNoExtCvs; i++)
	{
		pExtLoop[i]->pSC = NULL;
	}
	for (i = 0; i < iNoIntLoops; i++)
	{
		for (j = 0; j < iNoIntCvs[i]; j++)
		{
			pIntLoop[i][j]->pSC = NULL;
		}
	}
}


void NSurf::DrawCtrlPtsTog()
{
if (DrawCPts==FALSE)
{
  DrawCPts=TRUE;
}
else
{
  DrawCPts=FALSE;
}
}



void NSurf::HighLight(CDC* pDC)
{
int i;
if (iNoExtCvs>0)
{
  for (i=0;i<iNoExtCvs;i++)
  {
     pExtLoop[i]->HighLight(pDC);
  }
}
}


void NSurf::S_Box(CPoint P1,CPoint P2,ObjList* pSel)
{
int i;
int j;
//CALL SURFACE S_BOX METHOD
G_Object::S_Box(P1,P2,pSel);

if (DrawCPts == TRUE)
{
	for (i = 0; i < iNoCvs; i++)
	{
		pCVsU[i]->S_Box(P1, P2, pSel);
		for (j = 0; j < pCVsU[i]->iNoCPts; j++)
		{
			pCVsU[i]->cPts[j]->S_Box(P1, P2, pSel);
		}
	}
}

//CALL EXT TRIM CURVE S_BOX METHODS
for (i=0;i<this->iNoExtCvs;i++)
{
  pExtLoop[i]->S_Box(P1,P2,pSel);
}
//CALL INTERNAL TRIM CURVE S_BOX METHODS
for (i=0;i<iNoIntLoops;i++)
{
  for (j=0;j<iNoIntCvs[i];j++)
  {  
	pIntLoop[i][j]->S_Box(P1,P2,pSel);
  }
}
}

G_ObjectD NSurf::SelDist(CPoint InPT,Filter FIL)
{
G_ObjectD Ret;
G_ObjectD vT;
C3dVector vPt;
C3dVector vPt2;
int i,j;
double dV;
double dU;
double dVi;
double dUi;
double dIncV;
double dIncU;
double dMinU;
double dMinV;
double dSpanV;
double dSpanU;
double dDist;
double dMinPixDist;
double dPixSpan;
double ddsU;
double ddeU;
double ddsV;
double ddeV;
double MinDist=1e36;
double Z = 0;
const int NO_PTS=5;
const int NO_PTS_RF = 50;
Pt_Object* ThePoint = new (Pt_Object);
ThePoint->Create(vPt,0,0,0,11,0,0,NULL);
//check the surface
this->GetBoundingUV(dUi,dVi,dSpanU,dSpanV);
dIncU=dSpanU/NO_PTS;
dIncV=dSpanV/NO_PTS;
//calculate pixels between w increments;
vPt = GetPt(0,0);
ThePoint->Pt_Point->Set(vPt.x, vPt.y, vPt.z);
ThePoint->SetToScr(pModZ, pScrZ);
vPt.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
vPt2 = GetPt(dIncU, dIncV);
ThePoint->Pt_Point->Set(vPt2.x, vPt2.y, vPt2.z);
ThePoint->SetToScr(pModZ, pScrZ);
vPt2.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
vPt2 -= vPt;
dPixSpan = vPt2.Mag();
dU = dUi + dIncU;
for (j=0;j<NO_PTS-1;j++)
{
  dV = dVi + dIncV;
  for (i=0;i<NO_PTS-1;i++)
  {
    vPt=GetPt(dU,dV);
    ThePoint->Pt_Point->x = vPt.x;
    ThePoint->Pt_Point->y = vPt.y;
    ThePoint->Pt_Point->z = vPt.z;
	ThePoint->SetToScr(pModZ,pScrZ);
    dDist = pow((ThePoint->DSP_Point->x-InPT.x),2)+pow((ThePoint->DSP_Point->y-InPT.y),2);
	if (dDist < MinDist)
	{
      MinDist = dDist;
	  Z = ThePoint->Pt_Point->z;
      dMinU = dU;
      dMinV = dV;
      dMinPixDist = pow(MinDist, 0.5);
	}
	dV+=dIncV;
  }
  dU+=dIncU;
}
//Refine if dMinPixDist<100
if (dMinPixDist < dPixSpan)
{

  ddsU = dMinU - dIncU;
  ddeU = dMinU + dIncU;
  if (ddsU < dUi)
    ddsU = dUi;
  if (ddeU > dUi + dSpanU)
    ddeU = dUi + dSpanU;
  ddsV = dMinV - dIncV;
  ddeV = dMinV + dIncV;
  if (ddsV < dVi)
    ddsV = dVi;
  if (ddeV > dVi + dSpanV)
    ddeV = dVi + dSpanV;
  dSpanV = ddeV - ddsV;
  dSpanU = ddeU - ddsU;
  //iNo = 100;
  //dw = dds;
  dIncU = dSpanU / NO_PTS_RF;
  dIncV = dSpanV / NO_PTS_RF;
  dU = ddsU + dIncU;
  for (j = 0; j<NO_PTS_RF - 1; j++)
  {
    dV = ddsV + dIncV;
    for (i = 0; i<NO_PTS_RF - 1; i++)
    {
      vPt = GetPt(dU, dV);
      ThePoint->Pt_Point->x = vPt.x;
      ThePoint->Pt_Point->y = vPt.y;
      ThePoint->Pt_Point->z = vPt.z;
      ThePoint->SetToScr(pModZ, pScrZ);
      dDist = pow((ThePoint->DSP_Point->x - InPT.x), 2) + pow((ThePoint->DSP_Point->y - InPT.y), 2);
      if (dDist < MinDist)
      {
        MinDist = dDist;
		Z = ThePoint->Pt_Point->z;
      }
      dV += dIncV;
    }
    dU += dIncU;
  }
}
delete(ThePoint);
Ret.pObj=NULL;
//Check Surface definition Curves
if (iNoExtCvs==0)
{
  if ((FIL.isFilter(7)) ||
     (FIL.isFilter(8)) ||
     (FIL.isFilter(9)))
  {
    for (i = 0; i < iNoCvs; i++)
    {
      vT=pCVsU[i]->SelDist(InPT,FIL);
	  if (vT.Dist<MinDist)
	  {
        MinDist=vT.Dist;
        Ret.Dist=MinDist;
        Ret.pObj=vT.pObj;
	  }
    }
    for (i = 0; i < 2; i++)
    {
      vT=pCVsV[i]->SelDist(InPT,FIL);
	  if (vT.Dist<MinDist)
	  {
        MinDist=vT.Dist;
        Ret.Dist=MinDist;
        Ret.pObj=vT.pObj;
	  }
    }
  }
}

// Check the control points
if (FIL.isFilter(0))
{
  if (DrawCPts==TRUE)
  {
    for (i = 0; i < iNoCvs; i++)
    {
      for (j=0;j<pCVsU[i]->iNoCPts;j++)
	    {
         vT=pCVsU[i]->cPts[j]->SelDist(InPT,FIL);
		 if (vT.Dist<MinDist)
	       {
           MinDist=vT.Dist;
           Ret.Dist=MinDist;
           Ret.pObj=vT.pObj;
	       }
	  }
    }
  }
}

//******************************************************
//Change to curve selection
//Check active trim curves only not all trim curves
//EXTERNAL TRIM LOOP FIRST
//******************************************************
if (this->iNoExtCvs>0)
{
  if (FIL.isFilter(13) || FIL.isFilter(0))
  {
    for (i=0;i<this->iNoExtCvs;i++)
    {
      vT=this->pExtLoop[i]->SelDist(InPT,FIL);
	  if (vT.Dist<MinDist)
	  {
        MinDist=vT.Dist;
        Ret.Dist=MinDist;
        Ret.pObj=vT.pObj;
	  }
	}
  }
}
  //INTERNAL TRIM LOOPS
  //******************************************************
if (this->iNoIntCvs>0)
{
  if (FIL.isFilter(13) || FIL.isFilter(0))
  {
    for (i=0;i<iNoIntLoops;i++)
    {
      for (j=0;j<iNoIntCvs[i];j++)
      {  
        //pIL[j]=pIntLoop[i][iNoIntCvs[i]-j];
	    vT=this->pIntLoop[i][j]->SelDist(InPT,FIL);
	    if (vT.Dist<MinDist)
	    {
          MinDist=vT.Dist;
          Ret.Dist=MinDist;
          Ret.pObj=vT.pObj;
	    }
	  }
	}
  }
}

//******************************************************
// Check all curves on surface if in trim curve or not??
// Not sure best way to handle this

if (FIL.isFilter(13))
{
  for (i=0;i<iNoTrimCvs;i++)
  {
    vT=pSurfCvs[i]->SelDist(InPT,FIL);
	if (vT.Dist<MinDist)
	{
      MinDist=vT.Dist;
      Ret.Dist=MinDist;
      Ret.pObj=vT.pObj;
	}
  }
}


if (Ret.pObj==NULL)
{
  Ret.Dist=MinDist;
  Ret.pObj=this;
  Ret.Z = Z;
}

return (Ret);
}

G_ObjectD NSurf::SelDistFace(CPoint InPT, Filter FIL)
{
	G_ObjectD Ret;
	C3dVector vPt;
	C3dVector vPt2;
	int i, j;
	double dV;
	double dU;
	double dVi;
	double dUi;
	double dIncV;
	double dIncU;
	double dMinU;
	double dMinV;
	double dSpanV;
	double dSpanU;
	double dDist;
	double dMinPixDist;
	double dPixSpan;
	double ddsU;
	double ddeU;
	double ddsV;
	double ddeV;
	double MinDist = 1e36;
	double Z = 0;
	const int NO_PTS = 5;
	const int NO_PTS_RF = 50;
	Pt_Object* ThePoint = new (Pt_Object);
	ThePoint->Create(vPt, 0, 0, 0, 11, 0, 0, NULL);
	//check the surface
	this->GetBoundingUV(dUi, dVi, dSpanU, dSpanV);
	dIncU = dSpanU / NO_PTS;
	dIncV = dSpanV / NO_PTS;
	//calculate pixels between w increments;
	vPt = GetPt(0, 0);
	ThePoint->Pt_Point->Set(vPt.x, vPt.y, vPt.z);
	ThePoint->SetToScr(pModZ, pScrZ);
	vPt.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
	vPt2 = GetPt(dIncU, dIncV);
	ThePoint->Pt_Point->Set(vPt2.x, vPt2.y, vPt2.z);
	ThePoint->SetToScr(pModZ, pScrZ);
	vPt2.Set(ThePoint->DSP_Point->x, ThePoint->DSP_Point->y, 0);
	vPt2 -= vPt;
	dPixSpan = vPt2.Mag();
	dU = dUi + dIncU;
	for (j = 0; j < NO_PTS - 1; j++)
	{
		dV = dVi + dIncV;
		for (i = 0; i < NO_PTS - 1; i++)
		{
			vPt = GetPt(dU, dV);
			ThePoint->Pt_Point->x = vPt.x;
			ThePoint->Pt_Point->y = vPt.y;
			ThePoint->Pt_Point->z = vPt.z;
			ThePoint->SetToScr(pModZ, pScrZ);
			dDist = pow((ThePoint->DSP_Point->x - InPT.x), 2) + pow((ThePoint->DSP_Point->y - InPT.y), 2);
			if (dDist < MinDist)
			{
				MinDist = dDist;
				Z = ThePoint->DSP_Point->z;
				dMinU = dU;
				dMinV = dV;
				dMinPixDist = pow(MinDist, 0.5);
			}
			dV += dIncV;
		}
		dU += dIncU;
	}
	//Refine if dMinPixDist<100
	if (dMinPixDist < dPixSpan)
	{

		ddsU = dMinU - dIncU;
		ddeU = dMinU + dIncU;
		if (ddsU < dUi)
			ddsU = dUi;
		if (ddeU > dUi + dSpanU)
			ddeU = dUi + dSpanU;
		ddsV = dMinV - dIncV;
		ddeV = dMinV + dIncV;
		if (ddsV < dVi)
			ddsV = dVi;
		if (ddeV > dVi + dSpanV)
			ddeV = dVi + dSpanV;
		dSpanV = ddeV - ddsV;
		dSpanU = ddeU - ddsU;
		//iNo = 100;
		//dw = dds;
		dIncU = dSpanU / NO_PTS_RF;
		dIncV = dSpanV / NO_PTS_RF;
		dU = ddsU + dIncU;
		for (j = 0; j < NO_PTS_RF - 1; j++)
		{
			dV = ddsV + dIncV;
			for (i = 0; i < NO_PTS_RF - 1; i++)
			{
				vPt = GetPt(dU, dV);
				ThePoint->Pt_Point->x = vPt.x;
				ThePoint->Pt_Point->y = vPt.y;
				ThePoint->Pt_Point->z = vPt.z;
				ThePoint->SetToScr(pModZ, pScrZ);
				dDist = pow((ThePoint->DSP_Point->x - InPT.x), 2) + pow((ThePoint->DSP_Point->y - InPT.y), 2);
				if (dDist < MinDist)
				{
					MinDist = dDist;
					Z = ThePoint->DSP_Point->z;
				}
				dV += dIncV;
			}
			dU += dIncU;
		}
	}
	delete(ThePoint);
	Ret.pObj = NULL;

	if (Ret.pObj == NULL)
	{
		Ret.Dist = MinDist;
		Ret.pObj = this;
		Ret.Z = Z;
	}
	return (Ret);
}

void NSurf::Translate (C3dVector vIn)
{
int i;
for (i = 0; i < iNoCvs; i++)
{
  pCVsU[i]->Translate(vIn);
}
}

void NSurf::Transform(C3dMatrix TMat)
{
int i;
for (i = 0; i < iNoCvs; i++)
{
  pCVsU[i]->Transform(TMat);
}
}

C3dVector NSurf::GetPt(double dU,double dV)
{
C3dVector vRet;
int l,k;
int uspan;
int vspan;
double W;
double NU[20];
double NV[20];
C4dVector temp[20];
C4dVector pt;
C4dVector SW;
if (dU < 0)
	dU = 0;
else if (dU>1)
    dU = 1;
if (dV < 0)
	dV = 0;
else if (dV > 1)
    dV = 1;

uspan = pCVsU[0]->FindSpan(dU);
pCVsU[0]->BasisFun(pCVsU[0]->p,uspan,dU,NU);
vspan = pCVsV[0]->FindSpan(dV);
pCVsV[0]->BasisFun(pCVsV[0]->p,vspan,dV,NV);
for (l=0;l<=pV;l++)
{
  temp[l].Clear();
  for(k=0;k<=pU;k++)
  {
    W=pCVsU[vspan-pV+l]->cPts[uspan-pU+k]->w;
    pt.xw = pCVsU[vspan-pV+l]->cPts[uspan-pU+k]->Pt_Point->x*W;
    pt.yw = pCVsU[vspan-pV+l]->cPts[uspan-pU+k]->Pt_Point->y*W;
    pt.zw = pCVsU[vspan-pV+l]->cPts[uspan-pU+k]->Pt_Point->z*W;
    pt.w = W;
    temp[l] = temp[l]+pt*NU[k];
  }
}
SW.Clear();
for (l=0;l<=pV;l++)
{
  SW=SW+temp[l]*NV[l];
}
vRet.x=SW.xw/SW.w;
vRet.y=SW.yw/SW.w;
vRet.z=SW.zw/SW.w;
return (vRet);
}

void NSurf::binomialCoef(Mat* Bin)
{
  int n,k ;
  // Setup the first line
  *Bin->mn2(0,0) = 1.0 ;
  for(k=Bin->n-1;k>0;--k)
  {
    *Bin->mn2(0,k) = 0.0 ;
  }
  // Setup the other lines
  for(n=0;n<Bin->m-1;n++)
  {
    *Bin->mn2(n+1,0) = 1.0 ;
    for(k=1;k<Bin->n;k++)
    {
      if(n+1<k)
	      *Bin->mn2(n,k) = 0.0 ;
      else
	      *Bin->mn2(n+1,k) = *Bin->mn2(n,k) + *Bin->mn2(n,k-1) ;
    }
  }
}

void NSurf::AddTrimCurveExp(NCurveOnSurf* pCurve)
{

	if (pCurve != NULL)
	{
		if (iNoTrimCvs < MAX_LOOPCVS)
		{
			pSurfCvs[iNoTrimCvs] = pCurve;
			pSurfCvs[iNoTrimCvs]->pParent = this;
			pCurve->iLnType = 1;
			pCurve->iLnThk = 4;
			pCurve->iColour = 75;
			pCurve->SetToScr(pModZ, pScrZ);
			iNoTrimCvs++;
		}
	}
}

NCurveOnSurf* NSurf::AddTrimCurve(NCurve* pCurve)
{
NCurveOnSurf* pSCv=NULL;
if (pCurve!=NULL)
{
	if (iNoTrimCvs < MAX_LOOPCVS)
	{
		pSCv = pCurve->GetSurfaceCV3(this);
		if (pSCv != NULL)
		{
			pSCv->pParent = this;
			pSCv->iLnType = 1;
			pSCv->iLnThk = 4;
			pSCv->iColour = 75;
			pSurfCvs[iNoTrimCvs] = pSCv;
			//pSurfCvs[iNoTrimCvs]->iColour=this->iColour;
			pSCv->SetToScr(pModZ, pScrZ);
			iNoTrimCvs++;
		}
	}
}
return (pSCv);
}

//This does onlt need calling once so remover from Opedgl draw shaded
void NSurf::CleanExternalTrim()
{
int i;
NCurveOnSurf* c1;
NCurveOnSurf* c2;
if (iNoExtCvs>0)
{
  for (i=0;i<iNoExtCvs-1;i++)
  {
    c1=pExtLoop[i];
    c2=pExtLoop[i+1];
    c1->cPts[c1->iNoCPts-1]->Pt_Point->x=c2->cPts[0]->Pt_Point->x;
    c1->cPts[c1->iNoCPts-1]->Pt_Point->y=c2->cPts[0]->Pt_Point->y;
  }
  c1=pExtLoop[iNoExtCvs-1];
  c2=pExtLoop[0];
  c1->cPts[c1->iNoCPts-1]->Pt_Point->x=c2->cPts[0]->Pt_Point->x;
  c1->cPts[c1->iNoCPts-1]->Pt_Point->y=c2->cPts[0]->Pt_Point->y;
}
}

C3dVector NSurf::Get_Normal(double dU,double dV)
{
C3dVector v1;
C3dVector v2;
C3dVector vN;
Matrix<C3dVector> der;
deriveAt(dU,dV,1,der);
v1=der(1,0);
v2=der(0,1);
der.DeleteAll();
vN=v1.Cross(v2);
return (vN);
}

BOOL NSurf::isPlanar()
{
	bool brc=TRUE;
	C3dVector v1;
	C3dVector vt;
	double dDot;
	if ((pU == 1) && (pV == 1))
	{
		v1 = Get_Normal(0.5, 0.5);
		v1.Normalize();
		vt = Get_Normal(0.1, 0.1);
		vt.Normalize();
		dDot = v1.Dot(vt);
		if ((dDot < 0.999999) || (dDot > 1.000001))
			return (FALSE);
		vt = Get_Normal(0.9, 0.1);
		vt.Normalize();
		dDot = v1.Dot(vt);
		if ((dDot < 0.999999) || (dDot > 1.000001))
			return (FALSE);
		vt = Get_Normal(0.9, 0.9);
		vt.Normalize();
		dDot = v1.Dot(vt);
		if ((dDot < 0.999999) || (dDot > 1.000001))
			return (FALSE);

	}
	else
	{
		brc=FALSE;
	}
 
	return (brc);
}


 BOOL NSurf::isClosedinU()
{
	 BOOL brc;
	 C3dVector s = this->GetPt(0, 1);
	 C3dVector e = this->GetPt(1, 1);
	 double dD;
	 dD = s.Dist(e);
	 if (dD < 0.0000001)
		 brc = TRUE;
	 else
		 brc = FALSE;
	 return (brc);
}

 BOOL NSurf::isClosedinV()
{
	 BOOL brc;
	 C3dVector s = this->GetPt(0, 0);
	 C3dVector e = this->GetPt(0, 1);
	 double dD;
	 dD = s.Dist(e);
	 if (dD < 0.0000001)
		 brc = TRUE;
	 else
		 brc = FALSE;
	 return (brc);
}

C3dVector NSurf::MinPt(C3dVector inPt)
{
C3dVector vRet;
const double dIncs = 36;
int i;
int j;
double dStp = 1/dIncs;

double dWu=0;
double dWv=0;
double dMinWu=0;
double dMinWv=0;
C3dVector cPt;
C3dVector cNextPt;
double Dist;
double minDist = 1e36;
C3dVector v1;
C3dVector v2;

double dDotU;
double dDotV;
cPt=GetPt(dWu,dWv);
for(i=0;i<dIncs;i++)
{
  for(j=0;j<dIncs;j++)
  {
    dWu=i*dStp;
    dWv=j*dStp;
    cPt=GetPt(dWu,dWv);
    Dist=cPt.Dist(inPt);
    if (Dist<minDist)
    {
      minDist=Dist;
      dMinWu=dWu;
      dMinWv=dWv;
    }
  }
}

//start of iteration
dWu=dMinWu;
dWv=dMinWv;

i=0;
Matrix<C3dVector> der;
C3dVector Vec;
double dUscl;
double dVscl;
double dErr1=1e36;
double dErr=1e36;
double absErr;
do
{
  der.DeleteAll();
  cPt=GetPt(dWu,dWv);
  Vec=inPt-cPt;
  dErr1=dErr;
  dErr=Vec.Mag();
  absErr= abs(dErr-dErr1);
  deriveAt(dWu,dWv,1,der);
  v1=der(1,0);
  v2=der(0,1);
  dUscl=v1.Mag();
  dVscl=v2.Mag();
  v1.Normalize();
  v2.Normalize();
//  Vec.Normalize();
  dDotU=v1.Dot(Vec)/dUscl;
  dDotV=v2.Dot(Vec)/dVscl;
  dWu+=dDotU;
  dWv+=dDotV;
  if (dWu>1){dWu=1;}
  if (dWv>1){dWv=1;}
  if (dWu<0){dWu=0;}
  if (dWv<0){dWv=0;}
  i++;
}
while ((absErr>dTol) && (i<100));
vRet=GetPt(dWu,dWv);
der.DeleteAll();
return (vRet);
}

C3dVector NSurf::MinPtW(C3dVector inPt)
{
C3dVector vRet;
const double dTol = 0.00000001;
const double dIncs = 64;
int i;
int j;
double dStp = 1/dIncs;

double dWu=0;
double dWv=0;
double dMinWu=0;
double dMinWv=0;
C3dVector cPt;
C3dVector cPtN;
C3dVector cNextPt;
double Dist;
double minDist = 1e36;
C3dVector vD;
C3dVector v1;
C3dVector v2;

double dDotU;
double dDotV;
cPt=GetPt(dWu,dWv);
for(i=0;i<dIncs;i++)
{
  for(j=0;j<dIncs;j++)
  {
    dWu=i*dStp;
    dWv=j*dStp;
    cPt=GetPt(dWu,dWv);
	cPtN = GetPt(dWu+0.00001, dWv+0.00001);
	cPtN -= cPt; cPtN.Normalize();
	vD = inPt; vD -= cPt; vD.Normalize();
	double dD;
	dD = cPtN.Dot(vD);

    Dist=cPt.Dist(inPt);
    if ((Dist<minDist) && (dD>=0))
    {
      minDist=Dist;
      dMinWu=dWu;
      dMinWv=dWv;
    }
  }
}

//start of iteration
dWu=dMinWu;
dWv=dMinWv;

i=0;
Matrix<C3dVector> der;
C3dVector Vec;
double dUscl;
double dVscl;
double dErr1=1e36;
double dErr=1e36;
double absErr;
do
{
  der.DeleteAll();
  cPt=GetPt(dWu,dWv);
  Vec=inPt-cPt;
  dErr1=dErr;
  dErr=Vec.Mag();
  absErr=abs(dErr-dErr1);
  deriveAt(dWu,dWv,1,der);
  v1=der(1,0);
  v2=der(0,1);
  dUscl=v1.Mag();
  dVscl=v2.Mag();
  v1.Normalize();
  v2.Normalize();
//  Vec.Normalize();
  dDotU=v1.Dot(Vec)/dUscl;
  dDotV=v2.Dot(Vec)/dVscl;
  dWu+=dDotU;
  dWv+=dDotV;
  if (dWu>1){dWu=1;}
  if (dWv>1){dWv=1;}
  if (dWu<0){dWu=0;}
  if (dWv<0){dWv=0;}
  i++;
}
while ((absErr>dTol) && (i<100));
vRet.Set(dWu,dWv,0);
der.DeleteAll();
return (vRet);
}
Mat* NSurf::Surfacederive(double u, double v, int d,Matrix <C4dVector> &skl)
{
  int k,l,du,dv;
  //Mat* skl = new Mat(d+1,d+1);
  skl.Size(d+1,d+1);
  du = minimum(d,pU) ;
  for(k=pU+1;k<=d;++k)
  {
    for(l=0;l<=d-k;++l)
    {
      skl(k,l).Clear();
    }
  }
  dv=minimum(d,pV) ;
  for(l=pV+1;l<=d;++l)
  {
    for(k=0;k<=d-l;++k)
    {
      skl(k,l).Clear();
      C4dVector A=skl(k,l);
    }
  }
  int uspan = pCVsU[0]->FindSpan(u) ;
  int vspan = pCVsV[0]->FindSpan(v) ;
  Mat* Nu;
  Mat* Nv;
  Nu=pCVsU[0]->dersBasisFuns(du,u,uspan); 
  Nv=pCVsV[0]->dersBasisFuns(dv,v,vspan); 
  double W;
  C4dVector temp[10];
  int dd,r,s ;
  C4dVector pt;
  for(k=0;k<=du;++k)
  {
    for(s=0;s<=pV;++s)
    {
      temp[s].Clear();
      for(r=0;r<=pU;++r)
      {
	      //temp[s] += Nu(k,r)*pCVsU[uspan-degU+r]->cPts[vspan-degV+s] ;
            W=pCVsU[vspan-pV+s]->cPts[uspan-pU+r]->w;
            pt.xw = pCVsU[vspan-pV+s]->cPts[uspan-pU+r]->Pt_Point->x*W;
            pt.yw = pCVsU[vspan-pV+s]->cPts[uspan-pU+r]->Pt_Point->y*W;
            pt.zw = pCVsU[vspan-pV+s]->cPts[uspan-pU+r]->Pt_Point->z*W;
            pt.w = W;
            temp[s] = temp[s]+pt**Nu->mn2(k,r);
      }
    }
    dd = minimum(d-k,dv) ;
    for(l=0;l<=dd;++l)
    {
      skl(k,l).Clear();
      for(s=0;s<=pV;++s)
      {
	      skl(k,l).xw += temp[s].xw**Nv->mn2(l,s);
        skl(k,l).yw += temp[s].yw**Nv->mn2(l,s);
        skl(k,l).zw += temp[s].zw**Nv->mn2(l,s);
        skl(k,l).w += temp[s].w**Nv->mn2(l,s);
      }
    }
  }

delete (Nu);
delete (Nv);
return(NULL);
}


void NSurf::deriveAt(double u, double v,int d,Matrix <C3dVector> &skl)
{
int i,j,k,l; 
C3dVector pv;
C3dVector pv2;
C3dVector a;
Matrix <C4dVector> ders;
Surfacederive(u,v,d,ders);
skl.Size(d+1,d+1);
Mat* Bin = new Mat(d+1,d+1);
binomialCoef(Bin);

for(k=0;k<=d;++k)
{
  for(l=0;l<=d-k;++l)
  {
      pv.x = ders(k,l).xw;
      pv.y = ders(k,l).yw;
      pv.z = ders(k,l).zw;
      for(j=1;j<=l;j++)
      {
	     pv -=skl(k,l-j)**Bin->mn2(l,j)*ders(0,j).w;
      }
      for(i=1;i<=k;i++)
      {
	      pv -= skl(k-i,l)**Bin->mn2(k,i)*ders(i,0).w;
	      pv2.Set(0,0,0) ;
	      for(j=1;j<=l;j++)
        {
	        pv2 += skl(k-i,l-j)**Bin->mn2(l,j)*ders(i,j).w ;
        }
	      pv -= pv2**Bin->mn2(k,i) ;
      }
      skl(k,l) = pv;
      skl(k,l) /=ders(0,0).w;
  }
 }

delete (Bin);
ders.DeleteAll();
}

BOOL NSurf::Generate(int pInV,double dvs,double dve)
{
BOOL bRet=FALSE;
int i;

//Check no pts and order in each U curve
if (iNoCvs>1)
{
  for (i=1;i<iNoCvs;i++)
  {
    if ((pCVsU[0]->p!=pCVsU[i]->p) || (pCVsU[0]->iNoCPts!=pCVsU[i]->iNoCPts)) 
    {
      bRet=TRUE;
    }
  }
}
else
{
bRet=TRUE;
}
if (bRet==FALSE)
{

  pU = pCVsU[0]->p;
  if (iNoCvs>=pInV+1)
  {
    pV = pInV;
  }
  else
  {
    pV = iNoCvs-1;
  }
  pCVsV[0]=new NCurve();
  pCVsV[1]=new NCurve();

  pCVsV[0]->Create(-1,this);
  pCVsV[1]->Create(-1,this);
  pCVsV[0]->iNoCPts = iNoCvs;
  pCVsV[1]->iNoCPts = iNoCvs;

  for (i=0;i<iNoCvs;i++)
  {
    pCVsV[0]->cPts[i]=pCVsU[i]->cPts[0];
    pCVsV[1]->cPts[i]=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1];
  }
  pCVsV[0]->Generate(pV);
  pCVsV[1]->Generate(pV);
}
  pCVsV[0]->ws=dvs;
  pCVsV[1]->ws=dvs;
  pCVsV[0]->we=dve;
  pCVsV[1]->we=dve;
DefualtTrim();
return (bRet);
}

BOOL NSurf::GenerateFit(int pInV,double dvs,double dve)
{
BOOL bRet=FALSE;
int i;
int j;
//Check no pts and order in each U curve
if (iNoCvs>1)
{
  for (i=1;i<iNoCvs;i++)
  {
    if ((pCVsU[0]->p!=pCVsU[i]->p) || (pCVsU[0]->iNoCPts!=pCVsU[i]->iNoCPts)) 
    {
      bRet=TRUE;
    }
  }
}
else
{
bRet=TRUE;
}
Vec<C4dVector> P;
Vec<double> U;
if (bRet==FALSE)
{

// Interpolate the points in the V direction
  for (j=0;j<pCVsU[0]->iNoCPts;j++)
  {
    NCurve* cPolyW = new NCurve();
    cPolyW->Create(0,NULL);
    for (i=0;i<iNoCvs;i++)
    {
      C3dVector a=pCVsU[i]->cPts[j]->GetCoords();
      cPolyW->AddVert(pCVsU[i]->cPts[j]->GetCoords(),pCVsU[i]->cPts[j]->w);
    }
    cPolyW->globalInterp(pInV,P,U);
    cPolyW->GenerateExp(pInV,P,U); 
    for (i=0;i<iNoCvs;i++)
    {
      //pCVsU[i]->cPts[j]->w=cPolyW->cPts[i]->w;
      pCVsU[i]->cPts[j]->Pt_Point->x=cPolyW->cPts[i]->Pt_Point->x;
      pCVsU[i]->cPts[j]->Pt_Point->y=cPolyW->cPts[i]->Pt_Point->y;
      pCVsU[i]->cPts[j]->Pt_Point->z=cPolyW->cPts[i]->Pt_Point->z;
    }
    P.DeleteAll();
    U.DeleteAll();
    delete(cPolyW);
  }

//
  pU = pCVsU[0]->p;
  if (iNoCvs>=pInV+1)
  {
    pV = pInV;
  }
  else
  {
    pV = iNoCvs-1;
  }
  pCVsV[0]=new NCurve();
  pCVsV[1]=new NCurve();

  pCVsV[0]->Create(-1,this);
  pCVsV[1]->Create(-1,this);
  pCVsV[0]->iNoCPts = iNoCvs;
  pCVsV[1]->iNoCPts = iNoCvs;

  for (i=0;i<iNoCvs;i++)
  {
    pCVsV[0]->cPts[i]=pCVsU[i]->cPts[0];
    pCVsV[1]->cPts[i]=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1];
  }
  pCVsV[0]->Generate(pV);
  pCVsV[1]->Generate(pV);
}
  pCVsV[0]->ws=dvs;
  pCVsV[1]->ws=dvs;
  pCVsV[0]->we=dve;
  pCVsV[1]->we=dve;
DefualtTrim();
return (bRet);
}
BOOL NSurf::GenerateExp(C3dVector cPts[1000],
                      double wghts[1000],
                      double KnotsU[100],
                      double KnotsV[100],
                      int noU,
                      int noV,
                      int pInU,
                      int pInV)
{
BOOL bRet=FALSE;
int i;
int j;
Create(-1,NULL);
pU=pInU;
pV=pInV;
Vec<C4dVector> P;
Vec<double> U;
P.Size(noU);
U.Size(noU+pInU+1);
NCurve* cPolyW;
for (j=0;j<noU+pInU+1;j++)
{
  U[j]=KnotsU[j];
}
for (i=0;i<noV;i++)
{ 
  cPolyW = new NCurve();
  for (j=0;j<noU;j++)
  {
    P[j].xw=cPts[j+(i*noU)].x;
    P[j].yw=cPts[j+(i*noU)].y;
    P[j].zw=cPts[j+(i*noU)].z;
    P[j].w=wghts[j+(i*noU)];
  }

  cPolyW->GenerateExp(pU,P,U);
  this->AddCV(cPolyW);

}
P.clear();
P.Size(iNoCvs);
U.clear();
U.Size(noV+pInV+1);

for (i=0;i<iNoCvs;i++)
{
  P[i].xw=pCVsU[i]->cPts[0]->Pt_Point->x;
  P[i].yw=pCVsU[i]->cPts[0]->Pt_Point->y;
  P[i].zw=pCVsU[i]->cPts[0]->Pt_Point->z;
  P[i].w=pCVsU[i]->cPts[0]->w;
}
for (j=0;j<noV+pInV+1;j++)
{
  U[j]=KnotsV[j];
}
cPolyW = new NCurve();
cPolyW->GenerateExp(pV,P,U);
pCVsV[0]=cPolyW;

for (i=0;i<iNoCvs;i++)
{
  P[i].xw=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1]->Pt_Point->x;
  P[i].yw=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1]->Pt_Point->y;
  P[i].zw=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1]->Pt_Point->z;
  P[i].w=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1]->w;
}
cPolyW = new NCurve();
cPolyW->GenerateExp(pV,P,U);
pCVsV[1]=cPolyW;

//DefualtTrim();

P.DeleteAll();

////U.DeleteAll;
//Check no pts and order in each U curve
//if (iNoCvs>1)
//{
//  for (i=1;i<iNoCvs;i++)
//  {
//    if ((pCVsU[0]->p!=pCVsU[i]->p) || (pCVsU[0]->iNoCPts!=pCVsU[i]->iNoCPts)) 
//    {
//      bRet=TRUE;
//    }
//  }
//}
//else
//{
//bRet=TRUE;
//}
//if (bRet==FALSE)
//{
//  pU = pCVsU[0]->p;
//  if (iNoCvs>=pInV+1)
//  {
//    pV = pInV;
//  }
//  else
//  {
//    pV = iNoCvs-1;
//  }
//  pCVsV[0]=new NCurve();
//  pCVsV[1]=new NCurve();
//  pCVsV[0]->Create(-1,this);
//  pCVsV[1]->Create(-1,this);
//  pCVsV[0]->iNoCPts = iNoCvs;
//  pCVsV[1]->iNoCPts = iNoCvs;
//
//  for (i=0;i<iNoCvs;i++)
//  {
//    pCVsV[0]->cPts[i]=pCVsU[i]->cPts[0];
//    pCVsV[1]->cPts[i]=pCVsU[i]->cPts[pCVsU[0]->iNoCPts-1];
//  }
//  pCVsV[0]->Generate(pV);
//  pCVsV[1]->Generate(pV);
//}
//DefualtTrim();
return (bRet);
}

//Remove the curve from list do not delete as may have been 
//moved to a rtim loop
void NSurf::RemoveTrimCv(int ind)
{

int i;
for (i=ind;i<iNoTrimCvs-1;i++)
{
  pSurfCvs[i]=pSurfCvs[i+1];
}
iNoTrimCvs--;
}


int NSurf::GetTrimCvInd(G_Object* pCV)
{
int irc=-1;
int i;
for (i=0;i<iNoTrimCvs;i++)
{
  if (pSurfCvs[i]==pCV)
  {
    irc=i;
    break;
  }
}
return (irc);
}


void NSurf::InternalTrim(ObjList* pCvs)
{
int i;
int irc;
BOOL berr=FALSE;

for (i=0;i<pCvs->iNo;i++)
{
  irc=GetTrimCvInd(pCvs->Objs[i]);
  pIntLoop[iNoIntLoops][i]=pSurfCvs[irc]; 
  RemoveTrimCv(irc);
}
iNoIntCvs[iNoIntLoops]=pCvs->iNo;
iNoIntLoops++;
}

void NSurf::UserTrim(ObjList* pCvs)
{
int i;
int irc;
BOOL berr=FALSE;
for (i = 0; i < iNoExtCvs; i++)
{
	delete(pExtLoop[i]);
}
iNoExtCvs = 0;
for (i=0;i<pCvs->iNo;i++)
{
  irc=GetTrimCvInd(pCvs->Objs[i]);
  pExtLoop[i]=pSurfCvs[irc];
  RemoveTrimCv(irc);
}
iNoExtCvs=pCvs->iNo;
}

void NSurf::DeleteExtTrimLoop()
{
//for debuging only at moment
int i;

for (i=0;i<iNoExtCvs;i++)
{
   pSurfCvs[iNoTrimCvs]=pExtLoop[i];
   iNoTrimCvs++;
}
iNoExtCvs=0;
this->DefualtTrim();
}

void NSurf::DeleteIntTrimLoop()
{
	//for debuging only at moment
	int i;
	int j;

	for (i = 0; i < iNoIntLoops; i++)
	{
		for (j = 0; j < iNoIntCvs[i]; j++)
		{
			pSurfCvs[iNoTrimCvs] = pIntLoop[i][j];
			iNoTrimCvs++;
		}
	}
	iNoIntLoops = 0;
}




void NSurf::DefualtTrim()
{
double ut = pCVsU[0]->we;
double vt = pCVsV[0]->we;;
double us = pCVsU[0]->ws;
double vs = pCVsV[0]->ws;;
NCurveOnSurf* pTRM[4];
pTRM[0]=new NCurveOnSurf();
pTRM[0]->Create(-1,this);
C3dVector Pt;
Pt.Set(us,vs,0);
pTRM[0]->AddVert(Pt,1);
Pt.Set(ut,vs,0);
pTRM[0]->AddVert(Pt,1);
pTRM[0]->Generate(1);

pTRM[1]=new NCurveOnSurf();
pTRM[1]->Create(-1,this);
Pt.Set(ut,vs,0);
pTRM[1]->AddVert(Pt,1);
Pt.Set(ut,vt,0);
pTRM[1]->AddVert(Pt,1);
pTRM[1]->Generate(1);

pTRM[2]=new NCurveOnSurf();
pTRM[2]->Create(-1,this);
Pt.Set(ut,vt,0);
pTRM[2]->AddVert(Pt,1);
Pt.Set(us,vt,0);
pTRM[2]->AddVert(Pt,1);
pTRM[2]->Generate(1);

pTRM[3]=new NCurveOnSurf();
pTRM[3]->Create(-1,this);
Pt.Set(us,vt,0);
pTRM[3]->AddVert(Pt,1);
Pt.Set(us,vs,0);
pTRM[3]->AddVert(Pt,1);
pTRM[3]->Generate(1);

pExtLoop[0]=pTRM[0];
iNoExtCvs++;
pExtLoop[1]=pTRM[1];
iNoExtCvs++;
pExtLoop[2]=pTRM[2];
iNoExtCvs++;
pExtLoop[3]=pTRM[3];
iNoExtCvs++;

}

//check the direction of the external trim loop if
//counterclockwise then ok for opengl render
BOOL NSurf::ChkExtLoopDir()
{
BOOL brc=FALSE;
C3dVector vN;
C3dVector v1;
C3dVector v2;
C3dVector vC;
C3dVector vN2;
int i;
//vN= this->Get_Normal(0.5,0.5);
//vN.Normalize();
//get two non parrell vector from te trim loop
//v1=pSurfCvs[pExtLoop[0]]->GetPt(1);
//v1-=pSurfCvs[pExtLoop[0]]->GetPt(0);

//v1.Normalize();
for (i=0;i<iNoExtCvs-2;i++)
{
vC=pExtLoop[i]->GetPt(pExtLoop[i]->we);
vN=vC;
v1=pExtLoop[i]->GetPt(pExtLoop[i]->we-0.01);
v2=pExtLoop[i+1]->GetPt(pExtLoop[i+1]->ws+0.01);
v1-=vC;
v2-=vC;
v1.Normalize();
v2.Normalize();
vC=v2.Cross(v1);
NCurveOnSurf* pC;

if (vC.Mag()>0.1)
{
	vC.Normalize();
	pC=pExtLoop[i];
    vN=this->Get_Normal(pC->cPts[pC->iNoCPts-1]->Pt_Point->x,pC->cPts[pC->iNoCPts-1]->Pt_Point->y);
    vN.Normalize();
	if (vC.Dot(vN)>0)
	{
	  brc=TRUE;
	  break;
	}
}
}

return (brc);
}



void NSurf::ReverseExtTrimLoop()
{

int i;
NCurveOnSurf* pEL[500];
//change the order of the curve and reverse teir definition
pEL[0]=pExtLoop[0];
for (i=1;i<iNoExtCvs;i++)
{
      //gluPwlCurve (pSurf, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
   pEL[i]=pExtLoop[iNoExtCvs-i];
}
pExtLoop[0]->Reverse();
for (i=1;i<iNoExtCvs;i++)
{
      //gluPwlCurve (pSurf, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
   pExtLoop[i]=pEL[i];
   pExtLoop[i]->Reverse();
}

}

void NSurf::ReverseIntTrimLoops()
{
int i;
int j;

NCurveOnSurf* pIL[500];
for (i=0;i<iNoIntLoops;i++)
{
//change the order of the curve and reverse teir definition
  pIL[0]=pIntLoop[i][0];
  for (j=1;j<iNoIntCvs[i];j++)
  {  
      //gluPwlCurve (pSurf, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
     pIL[j]=pIntLoop[i][iNoIntCvs[i]-j];
  }
  for (j=0;j<iNoIntCvs[i];j++)
  {
      //gluPwlCurve (pSurf, 5, &edgePt[0][0], 2, GLU_MAP1_TRIM_2);
     pIntLoop[i][j]=pIL[j];
     pIntLoop[i][j]->Reverse();
  }
}
}


void NSurf::TrimLoop(double us,double ue,double vs, double ve)
{

pSurfCvs[0]=new NCurveOnSurf();
pSurfCvs[0]->Create(-1,this);
C3dVector Pt;
Pt.Set(us,vs,0);
pSurfCvs[0]->AddVert(Pt,1);
Pt.Set(ue,vs,0);
pSurfCvs[0]->AddVert(Pt,1);
pSurfCvs[0]->Generate(1);
iNoTrimCvs++;
pSurfCvs[1]=new NCurveOnSurf();
pSurfCvs[1]->Create(-1,this);
Pt.Set(ue,vs,0);
pSurfCvs[1]->AddVert(Pt,1);
Pt.Set(ue,ve,0);
pSurfCvs[1]->AddVert(Pt,1);
pSurfCvs[1]->Generate(1);
iNoTrimCvs++;
pSurfCvs[2]=new NCurveOnSurf();
pSurfCvs[2]->Create(-1,this);
Pt.Set(ue,ve,0);
pSurfCvs[2]->AddVert(Pt,1);
Pt.Set(us,ve,0);
pSurfCvs[2]->AddVert(Pt,1);
pSurfCvs[2]->Generate(1);
iNoTrimCvs++;
pSurfCvs[3]=new NCurveOnSurf();
pSurfCvs[3]->Create(-1,this);
Pt.Set(us,ve,0);
pSurfCvs[3]->AddVert(Pt,1);
Pt.Set(us,vs,0);
pSurfCvs[3]->AddVert(Pt,1);
pSurfCvs[3]->Generate(1);
iNoTrimCvs++;
pExtLoop[0]=pSurfCvs[0];
iNoExtCvs++;
pExtLoop[1]=pSurfCvs[1];;
iNoExtCvs++;
pExtLoop[2]=pSurfCvs[2];;
iNoExtCvs++;
pExtLoop[3]=pSurfCvs[3];;
iNoExtCvs++;
RemoveTrimCv(0);
RemoveTrimCv(1);
RemoveTrimCv(2);
RemoveTrimCv(3);
}

IMPLEMENT_DYNAMIC(NSurfR, CObject)


NSurfR::NSurfR()
{
iType = 3;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = -1;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
}


    // CvPt_Object* pVertex[20];
void NSurfR::Create(NCurve* pCIn,C3dVector vN,C3dVector vC,int iLab,G_Object* Parrent, double dS)
{
iType = 3;
C3dMatrix Glob;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = iLab;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pParent=Parrent;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
iNoIntLoops=0;
vNorm=vN;
vCent=vC;
C3dVector vX,vY,vZ;
vY=vN;
vY.Normalize();
vX=pCIn->Get_Centroid();
vX-=vC;
vX.Normalize();
vZ=vX.Cross(vY);
vZ.Normalize();
vX=vY.Cross(vZ);

Glob.SetColVec(1,vX);
Glob.SetColVec(2,vY);
Glob.SetColVec(3,vZ);
Glob.m_30=vC.x;
Glob.m_31=vC.y;
Glob.m_32=vC.z;

C3dMatrix mRot;
mRot.Rotate(0,R2D*dS,0);
double r2;
r2=1.4142135623730950488016887242097;

AddCV(pCIn);
C3dVector v;
C3dMatrix Glob2;
Glob2=Glob;
Glob.m_30=0;
Glob.m_31=0;
Glob.m_32=0;
v=-vC;

Glob.Transpose();
pCVsU[0]->Translate(v);
pCVsU[0]->Transform(Glob);

AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(45);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(90);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(135);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(180);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(225);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(270);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(315);
AddCV(pCVsU[0]);



int i;
for (i=0;i<iNoCvs;i++)
{
  pCVsU[i]->Transform(mRot);
  pCVsU[i]->Transform(Glob2);
}

}

void NSurfR::CreateRev(NCurve* pCIn,C3dVector vN,C3dVector vC,C3dVector vRef,int iLab,G_Object* Parrent)
{
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 16;
iLabel = iLab;
iObjType = 16;
iNoCvs = 0;
DrawCPts=FALSE;
pParent=Parrent;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
iNoIntLoops=0;
vNorm=vN;
vCent=vC;
C3dMatrix Glob;
C3dVector vX,vY,vZ;
double r2;
r2=1.4142135623730950488016887242097;

AddCV(pCIn);
C3dVector v;
C3dMatrix Glob2;

v=-vC;
vX=vRef;
vY=vN;
vX.Normalize();
vY.Normalize();
vZ=vX.Cross(vY);
Glob.SetColVec(1,vX);
Glob.SetColVec(2,vY);
Glob.SetColVec(3,vZ);
Glob2=Glob;
Glob2.Translate(vC.x,vC.y,vC.z);
Glob.Transpose();
pCVsU[0]->Translate(v);
pCVsU[0]->Transform(Glob);

AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(45);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(90);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(135);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(180);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(225);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->CPolyRot(270);
AddCV(pCVsU[0]);
pCVsU[iNoCvs-1]->ScaleX(r2);
pCVsU[iNoCvs-1]->ScaleW(r2/2);
pCVsU[iNoCvs-1]->CPolyRot(315);
AddCV(pCVsU[0]);



int i;
for (i=0;i<iNoCvs;i++)
{
  pCVsU[i]->Transform(Glob2);
}
}


BOOL NSurfR::Generate(int pInV,double dvs,double dve)
{
BOOL brc;
brc = NSurf::Generate(2,dvs,dve);
if (brc==FALSE)
{
  pCVsV[0]->knots[0]=0;
  pCVsV[0]->knots[1]=0;
  pCVsV[0]->knots[2]=0;
  pCVsV[0]->knots[3]=0.25;
  pCVsV[0]->knots[4]=0.25;
  pCVsV[0]->knots[5]=0.5;
  pCVsV[0]->knots[6]=0.5;
  pCVsV[0]->knots[7]=0.75;
  pCVsV[0]->knots[8]=0.75;
  pCVsV[0]->knots[9]=1.0;
  pCVsV[0]->knots[10]=1.0;
  pCVsV[0]->knots[11]=1.0;

  pCVsV[1]->knots[0]=0;
  pCVsV[1]->knots[1]=0;
  pCVsV[1]->knots[2]=0;
  pCVsV[1]->knots[3]=0.25;
  pCVsV[1]->knots[4]=0.25;
  pCVsV[1]->knots[5]=0.5;
  pCVsV[1]->knots[6]=0.5;
  pCVsV[1]->knots[7]=0.75;
  pCVsV[1]->knots[8]=0.75;
  pCVsV[1]->knots[9]=1.0;
  pCVsV[1]->knots[10]=1.0;
  pCVsV[1]->knots[11]=1.0;
  this->dUs=pCVsU[0]->knots[0];
  this->dVs=pCVsV[0]->knots[0];
}

return(brc);
}

void NSurfR::Serialize(CArchive& ar,int iV)
{
NSurf::Serialize(ar,iV);
vNorm.Serialize(ar,iV);
vCent.Serialize(ar,iV);
}

G_Object* NSurfR::Copy(G_Object* Parrent)
{

NSurfR* pSurf = new NSurfR();
NSurf::Copy2(pSurf);
pSurf->pParent=Parrent;
pSurf->vNorm=vNorm;
pSurf->vCent=vCent;
return(pSurf);
}

IMPLEMENT_DYNAMIC(NSurfE, CObject)


NSurfE::NSurfE()
{
iType = 2;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = -1;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
iNoTrimCvs=0;
iNoExtCvs=0;
}


    // CvPt_Object* pVertex[20];
void NSurfE::Create(NCurve* pCIn1,NCurve* pCIn2,C3dVector vTr,int iLab,G_Object* Parrent)
{
iType = 2;
Drawn = 0;
Selectable  = 1; 
Visable  = 1;
iColour = 151;
iLabel = iLab;
iObjType = 15;
iNoCvs = 0;
DrawCPts=FALSE;
pParent=Parrent;
pCVsV[0]=NULL;
pCVsV[1]=NULL;
vTran=vTr;
iNoTrimCvs=0;
iNoExtCvs=0;
AddCV(pCIn1);
AddCV(pCIn2);
iNoIntLoops=0;
}

BOOL NSurfE::Generate(int pInV,double dvs,double dve)
{
BOOL brc;
brc = NSurf::Generate(1,dvs,dve);
return(brc);
}

G_Object* NSurfE::Copy(G_Object* Parrent)
{

NSurfE* pSurf = new NSurfE();
NSurf::Copy2(pSurf);
pSurf->pParent=Parrent;
pSurf->vTran=vTran;
return(pSurf);
}

void NSurfE::Serialize(CArchive& ar,int iV)
{
NSurf::Serialize(ar,iV);
vTran.Serialize(ar,iV);
}


IgesP::IgesP()
{

iNo=0;
}

void IgesP::Add(CString inSt)
{
P[iNo]=inSt;
iNo++;
}

CString IgesP::getPLine(int pInd,int iNo)
{
int i;
int iC;
CString sL="";
for (i=0;i<iNo;i++)
{
  sL+=P[pInd-1+i];
	if (i==iNo-1)
	{
    iC=sL.ReverseFind(';');
	  sL=sL.Left(iC);
	}
	else
	{
    iC=sL.ReverseFind(',');
	  sL=sL.Left(iC+1);
	}
    
}
sL+=',';
return (sL);





}BEGIN_MESSAGE_MAP(CFilterDialog, CDialog)
ON_LBN_SELCHANGE(IDC_LIST1, &CFilterDialog::OnLbnSelchangeList1)
ON_LBN_DBLCLK(IDC_LIST1, &CFilterDialog::OnLbnDblclkList1)
ON_LBN_DBLCLK(IDC_LIST2, &CFilterDialog::OnLbnDblclkList2)
ON_BN_CLICKED(IDC_BUTTON1, &CFilterDialog::OnBnClickedButton1)
ON_BN_CLICKED(IDC_CLEAR_FIL, &CFilterDialog::OnBnClickedClearFil)
ON_BN_CLICKED(IDC_BUTTON2, &CFilterDialog::OnBnClickedButton2)
END_MESSAGE_MAP()

//*****************************************************************
//The filter dialog box
//*****************************************************************
CFilterDialog::CFilterDialog()
	: CDialog(CFilterDialog::IDD, NULL)
{
}

void CFilterDialog::OnLbnSelchangeList1()
{
  // TODO: Add your control notification handler code here
}


void CFilterDialog::ReSet()
{
  int i;
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  CListBox* pItemsNonSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  char OutT[80];

  for (i=0;i<pFilt->iNoOfType;i++)
  {
    sprintf_s(OutT,"%i : %s",pFilt->iType[i],pFilt->sType[i]);
    if (pFilt->isFilter(pFilt->iType[i])==TRUE)
    {
      pItemsNonSelectable->AddString(OutT);
    }
    else
    {
      pItemsSelectable->AddString(OutT);
    }
  }
}

BOOL CFilterDialog::OnInitDialog()
{

  CDialog::OnInitDialog();
  
  // TODO:  Add extra initialization here
  ReSet();
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterDialog::OnLbnDblclkList1()
{
  // TODO: Add your control notification handler code here
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  int ind = pItemsSelectable->GetCurSel();
  CString cT;
  pItemsSelectable->GetText(ind,cT);
  int i1;
  CString sR;
  i1=cT.Find(":");
  sR=cT.Left(i1-1);
  ind=atoi(sR);
  pFilt->SetFilter(ind);
  pItemsSelectable->ResetContent();
  pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  pItemsSelectable->ResetContent();
  ReSet();
}

void CFilterDialog::OnLbnDblclkList2()
{
  // TODO: Add your control notification handler code here
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  int ind = pItemsSelectable->GetCurSel();
  CString cT;
  pItemsSelectable->GetText(ind,cT);
  int i1;
  CString sR;
  i1=cT.Find(":");
  sR=cT.Left(i1-1);
  ind=atoi(sR);
  pFilt->RemFilter(ind);
  pItemsSelectable->ResetContent();
  pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  pItemsSelectable->ResetContent();
  ReSet();
  
}

void CFilterDialog::OnBnClickedButton1()
{
  pFilt->SetAll();
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  pItemsSelectable->ResetContent();
  pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  pItemsSelectable->ResetContent();
  ReSet();
}

void CFilterDialog::OnBnClickedClearFil()
{
  // TODO: Add your control notification handler code here
  pFilt->Clear();
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  pItemsSelectable->ResetContent();
  pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  pItemsSelectable->ResetContent();
  ReSet();
}

void CFilterDialog::OnBnClickedButton2()
{
  // TODO: Add your control notification handler code here
  int ind;
  int ind2;
  int i1;
  CString cT;
  CString sR;
  CListBox* pItemsSelectable=(CListBox*) GetDlgItem(IDC_LIST2);
  CListBox* pItemsNonSelectable=(CListBox*) GetDlgItem(IDC_LIST1);
  ind=pItemsSelectable->GetCurSel();
  ind2=pItemsNonSelectable->GetCurSel();
  if ((ind>=0) || (ind2>=0))
  {

    pFilt->Clear();
  }
  if (ind>=0)
  {
    pItemsSelectable->GetText(ind,cT);
    i1=cT.Find(":");
    sR=cT.Left(i1-1);
    ind=atoi(sR);
    pFilt->SetFilter(ind);
  }
  if (ind2>=0)
  {
    pItemsNonSelectable->GetText(ind2,cT);
    i1=cT.Find(":");
    sR=cT.Left(i1-1);
    ind2=atoi(sR);
    pFilt->SetFilter(ind2);
  }
  if ((ind>=0) || (ind2>=0))
  {
    pItemsSelectable->ResetContent();
    pItemsNonSelectable->ResetContent();
    ReSet();
  }
  OnOK();
}







CGroupDialog::CGroupDialog()
	: CDialog(CGroupDialog::IDD, NULL)
{
iNo=0;
}


void CGroupDialog::AddGroup(CString sName)
{
Groups[iNo]=sName;
iNo++;
}

void CGroupDialog::RefreshGPS()
{
int i;
char OutT[80];
CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
pGroups->ResetContent();
  for (i=0;i<iNo;i++)
  {
    sprintf_s(OutT,"%i : %s",i,Groups[i]);
    pGroups->AddString(OutT);
  }
  if (iGp!=-1)
  {
    pGroups->SetCurSel(iGp);
  }
}

void CGroupDialog::DeleteGP(int iThisGp)
{
int i;
if (iThisGp!=-1)
{
 for (i=iThisGp;i<iNo-1;i++)
 {
   Groups[i]=Groups[i+1];
 }
 iNo--;
 char sStr[10];
 itoa (iGp,sStr,10);
 outtextMSG2("GPDEL");
 outtextMSG2(sStr);
   if ((iThisGp>-1) && (iThisGp<iNo))
  {iGp=iThisGp;}
  else
  {iGp=-1;}
 RefreshGPS();
}
}


BOOL CGroupDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  RefreshGPS();
  return(TRUE);
}
  

BEGIN_MESSAGE_MAP(CGroupDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CGroupDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CGroupDialog::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_GROUPADDGP, &CGroupDialog::OnBnClickedGroupaddgp)
  ON_BN_CLICKED(IDC_GROUPREMGRP, &CGroupDialog::OnBnClickedGroupremgrp)
  ON_BN_CLICKED(IDC_GROUPDSPCURRENT, &CGroupDialog::OnBnClickedGroupdspcurrent)
  ON_BN_CLICKED(IDC_GROUPDSPALL, &CGroupDialog::OnBnClickedGroupdspall)
  ON_BN_CLICKED(IDC_GROUPDELS, &CGroupDialog::OnBnClickedGroupdels)
END_MESSAGE_MAP()

void CGroupDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
  iGp=pGroups->GetCurSel();
  if (iGp!=-1)
  {
    char sStr[10];
    itoa (iGp,sStr,10);
	outtextMSG2("GPSET");
	outtextMSG2(sStr);
  }
  //OnOK();
}

void CGroupDialog::OnBnClickedCancel()
{
  // TODO: Add your control notification handler code here
  iGp=-1;
  OnCancel();
}


CRelToDialog::CRelToDialog()
	: CDialog(CRelToDialog::IDD, NULL)
{
	iType=-1;
}


BOOL CRelToDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

int i;
  CListBox* pTypes=(CListBox*) GetDlgItem(IDC_LIST_RELTO);
  char OutT[80];
  for (i=0;i<pFilt->iNoOfType;i++)
  {
    sprintf_s(OutT,"%i : %s",pFilt->iType[i],pFilt->sType[i]);
    pTypes->AddString(OutT);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CRelToDialog, CDialog)
  ON_LBN_DBLCLK(IDC_LIST_RELTO, &CRelToDialog::OnLbnDblclkListRelto)
END_MESSAGE_MAP()

void CRelToDialog::OnLbnDblclkListRelto()
{
  int ind;
  int i1;
  CString sR;
  // TODO: Add your control notification handler code here
  CListBox* pTypes=(CListBox*) GetDlgItem(IDC_LIST_RELTO);
  iType=pTypes->GetCurSel();
  CString cT;
  pTypes->GetText(iType,cT);
  i1=cT.Find(":");
  sR=cT.Left(i1-1);
  ind=atoi(sR);
  iType=ind;
  OnOK();
}


IMPLEMENT_DYNAMIC( BSec, CObject )

BSec::~BSec()
{
iLnCnt1=0;
iLnCnt2=0;
}

void BSec::Clear()
{
iLnCnt1=0;
iLnCnt2=0;
}

BSec::BSec()
{
int iLnCnt1=0;
int iLnCnt2=0;
}

void BSec::Create()
{
int iLnCnt1=0;
int iLnCnt2=0;
}

void BSec::CreateDefSec(double W,int iC)
{
Clear();
int i;
double Ang=0;
double AngInc;
AngInc=(360.0/(MAX_SECPTS-1))*D2R;
for (i=0;i<MAX_SECPTS-1;i++)
{
  AddOutPt(W*sin(Ang),W*cos(Ang));
  Ang+=AngInc;
}
AddOutPt(W*sin(0.0),W*cos(0.0));
}

void BSec::CreateBox(double W,double H,double Wthk,double Hthk)
{
Clear();
W/=2;
H/=2;
AddOutPt(-W,-H);
AddOutPt(-W,+H);
AddOutPt(+W,+H);
AddOutPt(+W,-H);
AddOutPt(-W,-H);
W-=Wthk;
H-=Hthk;
AddInPt(-W,-H);
AddInPt(-W,+H);
AddInPt(+W,+H);
AddInPt(+W,-H);
AddInPt(-W,-H);
}

void BSec::CreateBar(double W,double H)
{
Clear();
W/=2;
H/=2;
AddOutPt(-W,-H);
AddOutPt(-W,+H);
AddOutPt(+W,+H);
AddOutPt(+W,-H);
AddOutPt(-W,-H);
}

void BSec::CreateI(double WH,double BW,double TW,double WT,double BWT,double TWT)
{
Clear();
WH/=2;
BW/=2;
TW/=2;
AddOutPt(-BW,-WH);
AddOutPt(+BW,-WH);
AddOutPt(+BW,-WH+BWT);
AddOutPt(+WT/2,-WH+BWT);
AddOutPt(+WT/2, WH-TWT);
AddOutPt(TW, WH-TWT);
AddOutPt(TW, WH);
AddOutPt(-TW, WH);
AddOutPt(-TW, WH-TWT);
AddOutPt(-WT/2, WH-TWT);
AddOutPt(-WT/2, -WH+BWT);
AddOutPt(-BW,-WH+BWT);
AddOutPt(-BW,-WH);
}


void BSec::CreateRod(double R)
{
Clear();
int i;
double Ang=0;
double AngInc;
AngInc=(360.0/(MAX_SECPTS-1))*D2R;
for (i=0;i<MAX_SECPTS-1;i++)
{
  AddOutPt(R*sin(Ang),R*cos(Ang));
  Ang+=AngInc;
}
AddOutPt(R*sin(0.0),R*cos(0.0));
}

void BSec::CreateTube(double R,double r)
{
Clear();
int i;
double Ang=0;
double AngInc;
AngInc=(360.0/(MAX_SECPTS-1))*D2R;
for (i=0;i<MAX_SECPTS-1;i++)
{
  AddOutPt(R*sin(Ang),R*cos(Ang));
  Ang+=AngInc;
}
AddOutPt(R*sin(0.0),R*cos(0.0));
Ang=0;
for (i=0;i<MAX_SECPTS-1;i++)
{
  AddInPt(r*sin(Ang),r*cos(Ang));
  Ang+=AngInc;
}
AddInPt(r*sin(0.0),r*cos(0.0));
}


void BSec::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
	  ar<<iLnCnt1;
    ar<<iLnCnt2;
	  ar<<inPt.x;
    ar<<inPt.y;
    ar<<inPt.z;
    ar<<iLnCnt1;
    for(i=0;i<iLnCnt1;i++)
    {
      ar<<pLnLoop1[i].x;
      ar<<pLnLoop1[i].y;
      ar<<pLnLoop1[i].z;
    }
    ar<<iLnCnt2;
    for(i=0;i<iLnCnt2;i++)
    {
      ar<<pLnLoop2[i].x;
      ar<<pLnLoop2[i].y;
      ar<<pLnLoop2[i].z;
    }
	}
	else
	{
	  ar>>iLnCnt1;
    ar>>iLnCnt2;
	  ar>>inPt.x;
    ar>>inPt.y;
    ar>>inPt.z;
    ar>>iLnCnt1;
    for(i=0;i<iLnCnt1;i++)
    {
      ar>>pLnLoop1[i].x;
      ar>>pLnLoop1[i].y;
      ar>>pLnLoop1[i].z;
    }
    ar>>iLnCnt2;
    for(i=0;i<iLnCnt2;i++)
    {
      ar>>pLnLoop2[i].x;
      ar>>pLnLoop2[i].y;
      ar>>pLnLoop2[i].z;
    }
	}	
}

void BSec::OglDraw(int iDspFlgs,C3dMatrix TA,C3dMatrix TB,C3dVector d0,C3dVector d1,float C1,float C2,BOOL bD)
{

C3dVector p1;
C3dVector p2;
C3dVector p3;
C3dVector p4;


int j=0;
if (((iDspFlgs & DSP_CONT)>0) || (bD=FALSE))
{
  if (iLnCnt1 > 1)
  {
    for (j=0;j<iLnCnt1-1;j++)
    {
      p1=TA*pLnLoop1[j+1];
      p2=TA*pLnLoop1[j];
      p3=TB*pLnLoop1[j];
      p4=TB*pLnLoop1[j+1];
      glBegin(GL_POLYGON);
	    glVertex3f((float) (p1.x+d0.x),(float) (p1.y+d0.y),(float) (p1.z+d0.z));
        glVertex3f((float) (p4.x+d1.x),(float) (p4.y+d1.y),(float) (p4.z+d1.z));
        glVertex3f((float) (p3.x+d1.x),(float) (p3.y+d1.y),(float) (p3.z+d1.z));
        glVertex3f((float) (p2.x+d0.x),(float) (p2.y+d0.y),(float) (p2.z+d0.z));     
      glEnd();
    }
  }
  if (iLnCnt2 > 1)
  {
    for (j=0;j<iLnCnt2-1;j++)
    {
      p1=TA*pLnLoop2[j];
      p2=TA*pLnLoop2[j+1];
      p3=TB*pLnLoop2[j+1];
      p4=TB*pLnLoop2[j];
      glBegin(GL_POLYGON);
	    glVertex3f((float) (p1.x+d0.x),(float) (p1.y+d0.y),(float) (p1.z+d0.z));
        glVertex3f((float) (p4.x+d1.x),(float) (p4.y+d1.y),(float) (p4.z+d1.z));
        glVertex3f((float) (p3.x+d1.x),(float) (p3.y+d1.y),(float) (p3.z+d1.z));
        glVertex3f((float) (p2.x+d0.x),(float) (p2.y+d0.y),(float) (p2.z+d0.z));
      glEnd();
    }
  }
}
else
{
  glColor3fv(cols[124]);
  glEnable(GL_TEXTURE_1D);
  if (iLnCnt1 > 1)
  {
    for (j=0;j<iLnCnt1-1;j++)
    {
      p1=TA*pLnLoop1[j+1];
      p2=TA*pLnLoop1[j];
      p3=TB*pLnLoop1[j];
      p4=TB*pLnLoop1[j+1];
      glBegin(GL_POLYGON);
        glTexCoord1f(C1);
	    glVertex3f((float) (p1.x+d0.x),(float) (p1.y+d0.y),(float) (p1.z+d0.z));
        glTexCoord1f(C2);
        glVertex3f((float) (p4.x+d1.x),(float) (p4.y+d1.y),(float) (p4.z+d1.z));
        glTexCoord1f(C2);
        glVertex3f((float) (p3.x+d1.x),(float) (p3.y+d1.y),(float) (p3.z+d1.z));
        glTexCoord1f(C1);
        glVertex3f((float) (p2.x+d0.x),(float) (p2.y+d0.y),(float) (p2.z+d0.z));


      glEnd();
    }
  }
  if (iLnCnt2 > 1)
  {
    for (j=0;j<iLnCnt2-1;j++)
    {
      p1=TA*pLnLoop2[j];
      p2=TA*pLnLoop2[j+1];
      p3=TB*pLnLoop2[j+1];
      p4=TB*pLnLoop2[j];
      glBegin(GL_POLYGON);
	    glVertex3f((float) (p1.x+d0.x),(float) (p1.y+d0.y),(float) (p1.z+d0.z));
        glVertex3f((float) (p4.x+d1.x),(float) (p4.y+d1.y),(float) (p4.z+d1.z));
        glVertex3f((float) (p3.x+d1.x),(float) (p3.y+d1.y),(float) (p3.z+d1.z));
        glVertex3f((float) (p2.x+d0.x),(float) (p2.y+d0.y),(float) (p2.z+d0.z));
      glEnd();
    }
  }
  glDisable(GL_TEXTURE_1D);
}
}

void BSec::OglDrawW(int iDspFlgs,C3dMatrix TMat,C3dVector d0,C3dVector d1)
{
C3dVector p1;
C3dVector p2;
C3dVector vOf;
vOf=d0;
vOf+=d1;
vOf*=0.5;
int j=0;
if (iLnCnt1 > 1)
{
  for (j=0;j<iLnCnt1-1;j++)
  {
    p1=TMat*pLnLoop1[j];
    p2=TMat*pLnLoop1[j+1];
    glBegin(GL_LINES);
	  glVertex3f((float) (p1.x+vOf.x),(float) (p1.y+vOf.y),(float) (p1.z+vOf.z));
      glVertex3f((float) (p2.x+vOf.x),(float) (p2.y+vOf.y),(float) (p2.z+vOf.z));
    glEnd();
  }
}
if (iLnCnt2>1)
{
  for (j=0;j<iLnCnt2-1;j++)
  {
    p1=TMat*pLnLoop2[j];
    p2=TMat*pLnLoop2[j+1];
    glBegin(GL_LINES);
	  glVertex3f((float) (p1.x+vOf.x),(float) (p1.y+vOf.y),(float) (p1.z+vOf.z));
      glVertex3f((float) (p2.x+vOf.x),(float) (p2.y+vOf.y),(float) (p2.z+vOf.z));
    glEnd();
  }
}
}


void BSec::AddOutPt(double X1,double Y1)
{
if (iLnCnt1<MAX_SECPTS)
{
  pLnLoop1[iLnCnt1].Set(X1,Y1,0);
  iLnCnt1++;
}
}


void BSec::AddInPt(double X1,double Y1)
{
if (iLnCnt2<MAX_SECPTS)
{
  pLnLoop2[iLnCnt2].Set(X1,Y1,0);
  iLnCnt2++;
}
}

IMPLEMENT_DYNAMIC(MatTable, CObject )

Material* MatTable::GetItem(int iID)
{
Material* pRet;
pRet=(Material*) Table::GetItem(iID);
return (pRet);
}



void MatTable::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
    ar << iNo;
	  for (i=0;i<iNo;i++)
    {
      ar<<pEnts[i]->iType;
      pEnts[i]->Serialize(ar,iV);
    }
  }
  else
  {
    int iE;
    ar >> iNo;
	  for (i=0;i<iNo;i++)
    {
      ar>>iE;
      switch(iE) 
      {
		    case 1:
          pEnts[i] = new MAT1;
          break;
		    case 8:
          pEnts[i] = new MAT8;
          break;
      }
      pEnts[i]->Serialize(ar,iV);
    }
  }
}


IMPLEMENT_DYNAMIC(PropTable, CObject )

Property* PropTable::GetItem(int iID)
{
Property* pRet;
pRet=(Property*) Table::GetItem(iID);
return (pRet);
}

void PropTable::UpdateMats(NEList* newMats)
{
int i;
Property* pRet;
for (i=0;i<iNo;i++)
{
  pRet=(Property*) pEnts[i];
  pRet->UpdateMats(newMats);
}

}

PBARL* PropTable::GetPBARL(CString sT,double W,double H, double dTHK,double dNSM,int iM)
{
int i;
PBARL* pRet=NULL;
PBARL* pB=NULL;
for (i=0;i<iNo;i++)
{
  if (pEnts[i]->iType==5)
  {
    pB=(PBARL*) pEnts[i];
    if ((pB->sSecType==sT) &&
        (pB->dNSM==dNSM) &&
        (pB->iMID==iM) &&
        (pB->dDIMs[0]==W) &&
        (pB->dDIMs[1]==H) &&
        (pB->dDIMs[2]==dTHK))
    {
      pRet=pB;
    }
  }
}
return(pRet);
}

PSHELL* PropTable::GetShell(double dTHK,double dNSM,int iM)
{
int i;
PSHELL* pRet=NULL;
PSHELL* pS=NULL;
for (i=0;i<iNo;i++)
{
  if (pEnts[i]->iType==1)
  {
    pS=(PSHELL*) pEnts[i];
    if ((pS->dT==dTHK) &&
        (pS->iMID1==iM) &&
        (pS->dNSM==dNSM))
    {pRet=pS;}
  }
}

return(pRet);
}


void PropTable::Serialize(CArchive& ar,int iV)
{
  int i;
	if (ar.IsStoring())
	{
    ar << iNo;
	  for (i=0;i<iNo;i++)
    {
      ar<<pEnts[i]->iType;
      pEnts[i]->Serialize(ar,iV);
    }
  }
  else
  {
    int iE;
    ar >> iNo;
	  for (i=0;i<iNo;i++)
    {
      ar>>iE;
      switch(iE) 
      {
		case 1:
          pEnts[i] = new PSHELL;
          break;
		case 2:
          pEnts[i] = new PCOMP;
          break;
		case 222:
			pEnts[i] = new PCOMPG;
			break;
        case 3:
          pEnts[i] = new PSOLID;
          break;
        case 4:
          pEnts[i] = new PBAR;
          break;
        case 5:
          pEnts[i] = new PBARL;
          break;
        case 6:
          pEnts[i] = new PBEAM;
          break;
        case 11:
          pEnts[i] = new PROD;
          break;
        case 136:
          pEnts[i] = new PSPRINGT;
          break;
        case 137:
          pEnts[i] = new PSPRINGR;
          break;
        case 161:
          pEnts[i] = new PMASS;
          break;
      }
      pEnts[i]->Serialize(ar,iV);
      if (iE==4)
      {
        PBAR* pbar = (PBAR*) pEnts[i];
        pbar->CreateSec();
      }
      else if (iE==6)
      {
        PBEAM* pbar = (PBEAM*) pEnts[i];
        pbar->CreateSec();
      }
    }
  }
}
//****************************************************************
//PROPERTY TABLE
//****************************************************************
IMPLEMENT_DYNAMIC(Table, CObject )

Table::Table()
{
  iNo=0;
}

Table::~Table()
{
	DeleteAll();
}

void Table::DeleteAll()
{
	int i;
	for (i = 0; i < iNo; i++)
		delete(pEnts[i]);
	iNo = 0;
}



void Table::AddItem(Entity* pIn)
{
BOOL bisIn =  Exists(pIn->iID);
if (bisIn==FALSE)
{
  if (iNo<MAX_ENTS)
  {
     pEnts[iNo]=pIn;
     iNo++;
  }
  else
  {

    outtext1("ERROR: Max No in Table Exceeded.");
  }
}
else
{
    char S1[80];
    sprintf_s(S1,"%s%i","WARNING: Property Conflict. ",pIn->iID);
    outtext1(S1); 
}
}

 BOOL Table::Exists(int iPID)
{
int i;
BOOL bRet=FALSE;
for (i=0;i<iNo;i++)
{
  if (pEnts[i]->iID==iPID)
  {
     bRet=TRUE;
     break;
  }
}
return (bRet);

}



Entity* Table::GetItem(int iID)
{
Entity* pRet=NULL;
int i;
for (i=0;i<iNo;i++)
{
  if (pEnts[i]->iID==iID)
  {
     pRet=pEnts[i];
     break;
  }
}
return (pRet);
}

void Table::ExportNAS(FILE* pF)
{
int i;
for (i=0;i<iNo;i++)
{
   pEnts[i]->ExportNAS(pF);
}
}

void Table::ListAll()
{
int i;
if (iNo>0)
{
  for (i=0;i<iNo;i++)
  {
   pEnts[i]->ListShort();
  }
}
else
{
  outtext1("None Defined.");
}
}

int Table::NextID()
{
int i;
int iRet=0;
if (iNo>0)
{
  for (i=0;i<iNo;i++)
  {
   if (pEnts[i]->iID>iRet)
   {
     iRet=pEnts[i]->iID;
   }
  }
}
iRet++;
return (iRet);
}

void Table::Serialize(CArchive& ar,int iV)
{

}



//****************************************************************************
//****************************************************************************
BEGIN_MESSAGE_MAP(CResSelDialog, CDialog)
  ON_BN_CLICKED(IDOK, &CResSelDialog::OnBnClickedOk)
  ON_LBN_SELCANCEL(IDC_LIST_RES, &CResSelDialog::OnLbnSelcancelListRes)
  ON_LBN_SELCHANGE(IDC_LIST_RES, &CResSelDialog::OnLbnSelchangeListRes)
//  ON_STN_CLICKED(IDC_LCTEXT, &CResSelDialog::OnStnClickedLctext)
ON_LBN_SELCHANGE(IDC_LIST_VAL, &CResSelDialog::OnSelchangeListVal)
END_MESSAGE_MAP()

//*****************************************************************
//The Res dialog box
//*****************************************************************
CResSelDialog::CResSelDialog()
	: CDialog(CResSelDialog::IDD, NULL)
{
  iNoRes=0;
  iCurResSet=-1;
  iResVal=-1;
  iSecResID=-1;
}

void CResSelDialog::SetData(BOOL isVec, ResSet* pInRes[], int iInNoRes, int iInCurResSet, int iInResVal, int inSecResID)
{
bIsVec = isVec;
int i;
iNoRes=iInNoRes;
for (i=0;i<iInNoRes;i++)
{
  pRes[i]=pInRes[i];
}
iCurResSet=iInCurResSet;
iResVal=iInResVal;
iSecResID=inSecResID;
}

void CResSelDialog::Init()
{
int i;
CListBox* pResList=(CListBox*) GetDlgItem(IDC_LIST_RES);
char OutT[80];
if (iNoRes>0)
{
  for (i=0;i<iNoRes;i++)
  {
    sprintf_s(OutT,"%i: LC %i : %s",i,pRes[i]->LC,pRes[i]->sName);
    pResList->AddString(OutT);
  }
}
}

void CResSelDialog::OnBnClickedOk()
{
CListBox* pResList=(CListBox*) GetDlgItem(IDC_LIST_RES);
CListBox* pValList=(CListBox*) GetDlgItem(IDC_LIST_VAL);
CListBox* pSecList=(CListBox*) GetDlgItem(IDC_LIST_VAL2);
  // TODO: Add your control notification handler code here
  OnOK();
iCurResSet=pResList->GetCurSel();
iResVal=pValList->GetCurSel();
iSecResID=pSecList->GetCurSel();
if (!bIsVec)
   iSecResID+=1;
}

BOOL CResSelDialog::OnInitDialog()
{
  int i;
  int iMaxSec;
  CDialog::OnInitDialog();
  CListBox* pResList=(CListBox*) GetDlgItem(IDC_LIST_RES);
  CListBox* pValList=(CListBox*) GetDlgItem(IDC_LIST_VAL);
  CListBox* pSecList=(CListBox*) GetDlgItem(IDC_LIST_VAL2);
  Init();
  if (iCurResSet!=-1)
    pResList->SetCurSel(iCurResSet);
  if (iResVal!=-1)
  {
    char OutT[80];
	if (pRes[iCurResSet] != NULL)
	{
		if (bIsVec) //Is a vector plot find all valid vectors
		{
			for (i = 0; i < pRes[iCurResSet]->iNoResDef; i++)
			{
				sprintf_s(OutT, "%i : %s", i, pRes[iCurResSet]->ResDefintion[i]->sResType);
				pValList->AddString(OutT);
			}
		}
		else
		{
			for (i = 0; i < pRes[iCurResSet]->iNoV; i++)
			{
				sprintf_s(OutT, "%i : %s", i, pRes[iCurResSet]->lab[i]);
				pValList->AddString(OutT);
			}
		}
	}
    pValList->SetCurSel(iResVal);
	if (!bIsVec)
	{
		if (pRes[iCurResSet]->iSecondaryID != -1)
		{
			iMaxSec = pRes[iCurResSet]->getMaxSecondaryID();
			for (i = 0; i < iMaxSec; i++)
			{
				sprintf_s(OutT, "%s : %i", pRes[iCurResSet]->sOpName, i + 1);
				pSecList->AddString(OutT);
			}
			pSecList->SetCurSel(iSecResID-1);
		}
	}
	else
	{
		if ((iResVal > -1) && (iCurResSet>-1))
		{
			for (i = 0; i < pRes[iCurResSet]->ResDefintion[iResVal]->iCompNo; i++)
			{
				sprintf_s(OutT, "%i : %s", i, pRes[iCurResSet]->ResDefintion[iResVal]->Lab9[i]);
				pSecList->AddString(OutT);
			}
			pSecList->SetCurSel(iSecResID);
		}
	}
  }
 
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CResSelDialog::OnLbnSelcancelListRes()
{
  // TODO: Add your control notification handler code here

}

void CResSelDialog::OnLbnSelchangeListRes()
{
  // TODO: Add your control notification handler code here
  int i;
CListBox* pResList=(CListBox*) GetDlgItem(IDC_LIST_RES);
CListBox* pValList=(CListBox*) GetDlgItem(IDC_LIST_VAL);
CListBox* pSecList=(CListBox*) GetDlgItem(IDC_LIST_VAL2);
CStatic* pLCTitle=(CStatic*) GetDlgItem(IDC_LCTEXT);
CStatic* pLCSubTitle=(CStatic*) GetDlgItem(IDC_SUBTITLE);
int ind = pResList->GetCurSel();
pValList->ResetContent();
pSecList->ResetContent();
pLCTitle->SetWindowText("TITLE:- "+pRes[ind]->sTitle);
pLCSubTitle->SetWindowText("SUBTITLE:- "+pRes[ind]->sSubTitle);
char OutT[80];
if (bIsVec)
{
	for (i = 0; i < pRes[ind]->iNoResDef; i++)
	{
		sprintf_s(OutT, "%i : %s", i, pRes[ind]->ResDefintion[i]->sResType);
		pValList->AddString(OutT);
	}
}
else
{
	for (i = 0; i < pRes[ind]->iNoV; i++)
	{
		sprintf_s(OutT, "%i : %s", i, pRes[ind]->lab[i]);
		pValList->AddString(OutT);
	}
}
int iMaxSec;

if (pRes[ind]->iSecondaryID != -1)
{
	{
		iMaxSec = pRes[ind]->getMaxSecondaryID();
		for (i = 0; i < iMaxSec; i++)
		{
			sprintf_s(OutT, "%s : %i", pRes[ind]->sOpName, i + 1);
			pSecList->AddString(OutT);
		}
	}
}

}

void CResSelDialog::OnSelchangeListVal()
{
	char OutT[80];
	CListBox* pResList = (CListBox*)GetDlgItem(IDC_LIST_RES);
	CListBox* pValList = (CListBox*)GetDlgItem(IDC_LIST_VAL);
	CListBox* pSecList = (CListBox*)GetDlgItem(IDC_LIST_VAL2);
	CStatic* pLCTitle = (CStatic*)GetDlgItem(IDC_LCTEXT);
	CStatic* pLCSubTitle = (CStatic*)GetDlgItem(IDC_SUBTITLE);
	
	int i;
	int ind = pResList->GetCurSel();

	// TODO: Add your control notification handler code here
	if (bIsVec)
	{
		pSecList->ResetContent();
		int iV = pValList->GetCurSel();
		if (iV > -1)
		{
			for (i = 0; i < pRes[ind]->ResDefintion[iV]->iCompNo; i++)
			{
				sprintf_s(OutT, "%i : %s", i, pRes[ind]->ResDefintion[iV]->Lab9[i]);
				pSecList->AddString(OutT);
			}
		}
	}

}

//******************************************************************
// NASTRAN FIELD CLASS
//****************************************************************** 

//CString[MAX_FIELDS];

NasCard::NasCard()
{
iNo=0;
iLnCnt=0;
}

void NasCard::Clear()
{
iNo=0;
iLnCnt=0;
}

//Checks to see if string is empty or conatins
//only spaces (a blank nastran field)
//if its blank TRUE is returned else FALSE
BOOL isClear(CString S)
{
BOOL brc=TRUE;
int i;
int iL;

iL = S.GetLength();
for (i=0;i<iL;i++)
{
  if (S[i] != ' ')
  {
    brc = FALSE;
    break;
  }
}
return (brc);
};

BOOL NasCard::isVoid(int iField)
{
BOOL brc=TRUE;
if (iField < iNo)
  brc = isClear(Fields[iField]);
return(brc);
}


CString NasCard::GetField(int iField)
{
CString sRet="NULL";
if ((iField>=0) && (iField<iNo))
  sRet=Fields[iField];
return (sRet);
}

BOOL NasCard::AddLn(CString sStr)
{
CString sF1;
CString sF10;

BOOL brc=FALSE;
int iField;
int iNum;
int i;
int iStart;
iStart=sStr.Find(",");
if (iStart != -1) 
{
	iStart=iStart;
}
if (iStart == -1) 
{
	if (sStr.Find("*") > -1) 
	{
	  brc = TRUE;
	  iField = 16;
	  iNum = 4;
	}
	else
	{
	  brc = FALSE;
	  iField = 8;
	  iNum = 8;
	}
	for (i=0;i<iNum;i++)
	{
	  Fields[iNo] = sStr.Mid(8 + i * iField, iField);
	  iNo++;
	}
}
else
{
  sStr=sStr.Right(sStr.GetLength()-iStart-1);
  while (iStart!=-1)
  {
    iStart=sStr.Find(",");
    sF1=sStr.Left(iStart);
	sStr=sStr.Right(sStr.GetLength()-iStart-1);
	if (iStart==-1)
	  Fields[iNo] = sStr;
	else
	  Fields[iNo] = sF1;

	iNo++;
  }
}
iLnCnt++;
return(brc);
}

//Check for a continuation card
BOOL isCont(CString S1, CString S2)
{
BOOL brc;
CString f10;
CString F1;
CString C1;

brc = FALSE;
  //Check for a contination entre
  f10 = S1.Mid(72, 8);
  F1 = S2.Mid(0, 8);
  C1 = S2.Mid(0, 1);
  F1.Trim();
  //Not a comment or blank line
  if ((isClear(S2)==FALSE) && (S2.Find("$") == -1))
  {
    if (f10.Find(F1)!=-1)
      brc = TRUE;
    else if ((C1 == " ") || (C1 == "*") || (C1 == "+"))
      brc = TRUE;
  }

return(brc);
}

void NasCard::Read(FILE* pFile,CString& sLine,CString& sLineN)
{
AddLn(sLine);
BOOL bExit=FALSE;
char s1[200];
do 
{
  if (isCont(sLine, sLineN)==TRUE)
  {
    sLine = sLineN;
    AddLn (sLine);
    if (feof(pFile)==FALSE)
    {
      fgets(s1,200,pFile);
      sLineN=s1;
    }
    else
    {
      sLine = "";
      sLineN = "";
      bExit=TRUE;
    }
  }
  else
  {
     bExit=TRUE;
  }
}
while (bExit==FALSE);
}

//******************************************************************
// DELAUNAY MESH GENERATION 
//****************************************************************** 

Facest3::Facest3(C3dVector* v1,C3dVector* v2,C3dVector* v3)
{
pV[0]=v1;
pV[1]=v2;
pV[2]=v3;
 double x1=pV[0]->x;
 double y1=pV[0]->y;
 double x2=pV[1]->x;
 double y2=pV[1]->y;
 double x3=pV[2]->x;
 double y3=pV[2]->y;
 double a=(y2-y3)*(x2-x1)-(y2-y1)*(x2-x3);
 double a1=(x1+x2)*(x2-x1)+(y2-y1)*(y1+y2);
 double a2=(x2+x3)*(x2-x3)+(y2-y3)*(y2+y3);
 vC.x=(a1*(y2-y3)-a2*(y2-y1))/a/2;
 vC.y=(a2*(x2-x1)-a1*(x2-x3))/a/2;
 vC.z=0;
 CirR=vC.Dist(*pV[0]);

}

Trianguation::Trianguation()
{
  iNo=0;
}

void Trianguation::AddTriangle(C3dVector* v1,C3dVector* v2,C3dVector* v3)
{
Tri[iNo]=new Facest3(v1,v2,v3);
}



Delaunay2d::Delaunay2d()
{
  iNo=0;
  AddPt(0,0,0);
  AddPt(0,1,0);
  AddPt(1,1,0);
  AddPt(1,0,0);
  //Initial triangluation covering whole
  //parametric domain.
  Tris.AddTriangle(&Nodes[0],&Nodes[1],&Nodes[2]);
  Tris.AddTriangle(&Nodes[2],&Nodes[3],&Nodes[0]);
}

void Delaunay2d::AddPt(double x,double y,double z)
{
  Nodes[iNo].x=x;
  Nodes[iNo].y=y;
  Nodes[iNo].z=z;
  iNo++;
}




void CGroupDialog::OnBnClickedGroupaddgp()
{
  CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
  iGp=pGroups->GetCurSel();
  if (iGp!=-1)
  {
    char sStr[10];
    itoa (iGp,sStr,10);
	outtextMSG2("GPADDGP");
	outtextMSG2(sStr);
  }
}


void CGroupDialog::OnBnClickedGroupremgrp()
{
  CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
  iGp=pGroups->GetCurSel();
  if (iGp!=-1)
  {
    char sStr[10];
    itoa (iGp,sStr,10);
	outtextMSG2("GPREMGP");
	outtextMSG2(sStr);
  }
}


void CGroupDialog::OnBnClickedGroupdspcurrent()
{
	// TODO: Add your control notification handler code here
  CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
  iGp=pGroups->GetCurSel();
  if (iGp!=-1)
  {
    char sStr[10];
    itoa (iGp,sStr,10);
	outtextMSG2("GPSET");
	outtextMSG2(sStr);
	outtextMSG2("DSPGP");
  }
  //OnOK();
}


void CGroupDialog::OnBnClickedGroupdspall()
{
	// TODO: Add your control notification handler code here
	outtextMSG2("DSPALL");
}


void CGroupDialog::OnBnClickedGroupdels()
{
	int iThisGp=-1;
	CListBox* pGroups=(CListBox*) GetDlgItem(IDC_GROUP_SEL);
	iThisGp=pGroups->GetCurSel();
	DeleteGP(iThisGp);
}

//Colour pick dialog
CColourPickDialog::CColourPickDialog()
	: CDialog(CColourPickDialog::IDD, NULL)
{

iNoCols=0;
iSel=-1;

}

void CColourPickDialog::AddCol(int inR,int inG,int inB)
{
  if (iNoCols<MAX_COL)
  {
    R[iNoCols]=inR;
    G[iNoCols]=inG;
    B[iNoCols]=inB;
    iNoCols++;
  }
}

BEGIN_MESSAGE_MAP(CColourPickDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDCANCEL, &CColourPickDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

void CColourPickDialog::OnPaint()
{
COLORREF col;;
int iWH = 40;             //Width and height of block
int iX = 10;
int iY = 0;
int iNo = 0;
//SIZE DIALOG BOX TO FIT COLOURS
CRect oSize;
this->GetWindowRect(&oSize);
oSize.right = oSize.left+21* iWH;
oSize.bottom = oSize.top + 12 * iWH;
this->MoveWindow(oSize, 0);
oSize.top =  9 * iWH+15;
oSize.left = 10;// 20 * iWH - 50;
oSize.bottom = 9 * iWH+ iWH+15;// oSize.top + 25;
oSize.right = 60;// oSize.left + 50;
CButton* btnOK = (CButton*) this->GetDlgItem(IDOK);
if (btnOK!=NULL)
  btnOK->MoveWindow(oSize, 0);
oSize.top = 9 * iWH+15;
oSize.left = 65;// 20 * iWH - 50;
oSize.bottom = 9 * iWH + iWH+15;// oSize.top + 25;
oSize.right = 165;// oSize.left + 50;
CButton* btnC = (CButton*)this->GetDlgItem(IDCANCEL);
if (btnC != NULL)
  btnC->MoveWindow(oSize, 0);
oSize.top = 9 * iWH+7+15;
oSize.left = 200;// 20 * iWH - 50;
oSize.bottom = 9 * iWH + 45;// oSize.top + 25;
oSize.right = 10* iWH;// oSize.left + 50;
CStatic* lbl = (CStatic*)this->GetDlgItem(IDC_STATIC_COL);
if (lbl != NULL)
  lbl->MoveWindow(oSize, 0);
//END SIZE DIALOG BOX TO FIT COLOURS
int i;
CPaintDC dc(this); // device context for painting
for (i=0;i<iNoCols;i++)
{
	Rects[i].top=iY;
    Rects[i].left=iX;
	Rects[i].right=iX+iWH;
    Rects[i].bottom=iY+iWH;
	col=RGB(R[i],G[i],B[i]);
	CBrush Brush(col);
	if (iSel==i)
	  Rects[i].InflateRect(-5,-5);
	dc.FillRect(&Rects[i],&Brush);
    iNo++;
	iX+=iWH;
	if (iNo==20)
	{
      iX=10;
	  iY=iY+iWH;
	  iNo=0;
	}   
}
}
void CColourPickDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

CDialog::OnLButtonDblClk(nFlags, point);

}


void CColourPickDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

CDialog::OnLButtonUp(nFlags, point);
int i;

for (i=0;i<iNoCols;i++)
{
	if ((point.x>Rects[i].left) && (point.x<Rects[i].right))
	{
	  if ((point.y>Rects[i].top) && (point.y<Rects[i].bottom))
	  {
		  iSel=i;
		  break;
	  }
	}
}
// get the system to repaint dialog
char S1[22];
sprintf_s(S1,"%s%i","Colour Index: ",iSel);
CWnd *label = GetDlgItem(IDC_STATIC_COL);
label->SetWindowText(S1);


this->Invalidate();
this->UpdateWindow();
}

void CColourPickDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
	iSel=-1;
}
BEGIN_MESSAGE_MAP(CSETSDialog, CDialog)
	ON_BN_CLICKED(IDCREATE, &CSETSDialog::OnBnClickedCreate)
	ON_STN_CLICKED(IDC_SETLABEL, &CSETSDialog::OnStnClickedSetlabel)
	ON_BN_CLICKED(IDCDELSET, &CSETSDialog::OnBnClickedCdelset)
	ON_BN_CLICKED(IDCSETCUR, &CSETSDialog::OnBnClickedCsetcur)
	ON_BN_CLICKED(IDCSETLIST, &CSETSDialog::OnBnClickedCsetlist)
  ON_EN_CHANGE(IDC_SETID, &CSETSDialog::OnEnChangeSetid)
  ON_BN_CLICKED(IDC_NONEACT, &CSETSDialog::OnBnClickedNoneact)
END_MESSAGE_MAP()
//****************************************************************************
//             BOUNDARY CONDITIONS GENERIC DIALOG BOX
//****************************************************************************
CSETSDialog::CSETSDialog()
	       : CDialog(CSETSDialog::IDD, NULL)
{



}


void CSETSDialog::AttachSets(int* iNo, int *iCur)
{
iNoS=iNo;
iCurS=iCur;
}

void CSETSDialog::AddSet(int ind,CString sTit)
{
  SETS[ind]=sTit;
}

void CSETSDialog::RemoveSet(int ind)
{
int i;
if (ind<*iNoS-1)
{
   for (i=ind+1;i<*iNoS;i++)
   {
     SETS[i-1] = SETS[i];
   }
}
}


void CSETSDialog::Refresh()
{
int i;

CListBox* pSets=(CListBox*) GetDlgItem(IDC_LIST1);
pSets->ResetContent();
for (i=0;i<*iNoS;i++)
{
   pSets->AddString(SETS[i]);
}
CStatic* pSt= (CStatic*) GetDlgItem(IDC_CURSET);
if (*iNoS>0)
  pSt->SetWindowTextA(SETS[*iCurS]);
else
pSt->SetWindowTextA("ERROR: No Sets Defined.");
}

//****************************************************************************
void CSETSDialog::OnBnClickedCreate()
{
	// TODO: Add your control notification handler code here
  CString cT;
  CString cI;
  int iID;
  CEdit* pTitle=(CEdit*) GetDlgItem(IDC_TITLE);
  CEdit* pSID=(CEdit*) GetDlgItem(IDC_SETID);

  pTitle->GetWindowText(cT);
  pSID->GetWindowText(cI);

  //CListBox* pSets=(CListBox*) GetDlgItem(IDC_LIST1);
  //pSets->AddString(cT);
  iID=atoi(cI);
  if (iID>0)
  {
    int iNow=*iNoS-1;
    outtextMSG2(sSET);
    outtextMSG2(cI);
    outtextMSG2(cT);
    char sNew[80];
    CListBox* pSets=(CListBox*) GetDlgItem(IDC_LIST1);
    sprintf_s(sNew,"%i : %s",iID,cT);
    if (iNow==*iNoS-2)
    {
      AddSet(*iNoS-1,sNew);
      pSets->AddString(sNew);
    }
  }
  else
  {
    outtext1("ERROR: Set ID Must be Greater than 0.");
  }
}

BOOL CSETSDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowTextA(sTitle);
    Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSETSDialog::OnStnClickedSetlabel()
{
	// TODO: Add your control notification handler code here
}


void CSETSDialog::OnBnClickedCdelset()
{
	// TODO: Add your control notification handler code here
CListBox* pSets=(CListBox*) GetDlgItem(IDC_LIST1);
int iSet=pSets->GetCurSel();
char sStr[10];
itoa (iSet,sStr,10);
RemoveSet(iSet);        //Must do this before its removed
outtextMSG2(sDEL);
outtextMSG2(sStr);
Refresh();
}

void CSETSDialog::OnBnClickedCsetcur()
{
	// TODO: Add your control notification handler code here
CListBox* pSets=(CListBox*) GetDlgItem(IDC_LIST1);
int iSet=pSets->GetCurSel();
CString sL;
CString sID;
pSets->GetText(iSet,sL);
int ipos = sL.Find(":");
sID = sL.Left(ipos);
sID.Trim(" ");
//char sStr[10];
//itoa (iSet,sStr,10);
outtextMSG2(sACT);
outtextMSG2(sID);
Refresh();
}

void CSETSDialog::OnBnClickedNoneact()
{
  // TODO: Add your control notification handler code here
  char sStr[10];
  itoa(-1, sStr, 10);
  outtextMSG2(sACT);
  outtextMSG2(sStr);
  Refresh();
}

void CSETSDialog::OnBnClickedCsetlist()
{
	// TODO: Add your control notification handler code here
outtextMSG2(sLIST);
}


//****************************************************************************
//                        SOLUTION SEQUENCE DIALOG BOX
//****************************************************************************
CSOLDialog::CSOLDialog() : CDialog(CSOLDialog::IDD, NULL)
{

}

void CSOLDialog::Refresh()
{
  char OutT[80];
  CString ss;
  int i;
  CListBox* pSol=(CListBox*) GetDlgItem(IDC_SOL_LST);
  pSol->ResetContent();
  for (i=0;i<pSOL->iNo;i++)
  {
     sprintf_s(OutT,"%i : %s",i,pSOL->pSols[i]->sTitle);
     pSol->AddString(OutT);
  }
  if (pSOL->iCur!=-1)
  {
    sprintf_s(OutT, "%g", pSOL->pSols[pSOL->iCur]->dTol);
    pSol->SetCurSel(pSOL->iCur);
    CEdit* pTol=(CEdit*) GetDlgItem(IDC_TOL_TXT);
    pTol->SetWindowTextA (OutT);
    CComboBox* pType=(CComboBox*) GetDlgItem(IDC_TYPE_CBO);
    pType->SetCurSel(pSOL->pSols[pSOL->iCur]->iType);
    CEdit* pTit=(CEdit*) GetDlgItem(IDC_TITLE_TXT);
    pTit->SetWindowTextA(pSOL->pSols[pSOL->iCur]->sTitle);
    CStatic* pSt= (CStatic*) GetDlgItem(IDC_STATIC_SOL_ACT);
    sprintf_s(OutT,"%i : %s",pSOL->iCur,pSOL->pSols[pSOL->iCur]->sTitle);
 
    ss=pSOL->pSols[pSOL->iCur]->GetSolutionTitleString();
    pSt->SetWindowTextA(ss);

  }
  else
  {
    CEdit* pTit=(CEdit*) GetDlgItem(IDC_TITLE_TXT);
    pTit->SetWindowTextA("");
    CStatic* pSt = (CStatic*)GetDlgItem(IDC_STATIC_SOL_ACT);
    pSt->SetWindowTextA("No Solution Active.");
  }
}

BOOL CSOLDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowTextA(sTitle);
  CComboBox* pT=(CComboBox*) GetDlgItem(IDC_TYPE_CBO);
  pT->ResetContent();
  pT->AddString("0: Lin Static");
  pT->AddString("1: SS Heat");
  pT->AddString("2: Lanczos N/A");
  CEdit* pTol=(CEdit*) GetDlgItem(IDC_TOL_TXT);
  pTol->SetWindowTextA("0.000000001");
  Refresh();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CSOLDialog, CDialog)
  ON_BN_CLICKED(ID_CR, &CSOLDialog::OnBnClickedCr)
  ON_BN_CLICKED(ID_ACT, &CSOLDialog::OnBnClickedAct)
  ON_BN_CLICKED(ID_DEL, &CSOLDialog::OnBnClickedDel)
  ON_BN_CLICKED(IDOK, &CSOLDialog::OnBnClickedOk)
END_MESSAGE_MAP()

void CSOLDialog::OnBnClickedCr()
{
  CString sTitle;
  CString sTol;
  char S1[20]="";
  // TODO: Add your control notification handler code here
  CEdit* pTitle=(CEdit*) GetDlgItem(IDC_TITLE_TXT);
  //IDC_TYPE_CBO
  CComboBox* pCT=(CComboBox*) GetDlgItem(IDC_TYPE_CBO);
  int iT=pCT->GetCurSel();
  CEdit* pTol=(CEdit*) GetDlgItem(IDC_TOL_TXT);
  pTitle->GetWindowText(sTitle);
  pTol->GetWindowText(sTol);
  outtextMSG2("SOLCR");
  outtextMSG2(sTitle);
  sprintf_s(S1,"%i",iT);
  outtextMSG2(S1);
  outtextMSG2(sTol);
  Refresh();
}

void CSOLDialog::OnBnClickedAct()
{
  // TODO: Add your control notification handler code here
  CListBox* pSol=(CListBox*) GetDlgItem(IDC_SOL_LST);
  int iSol = pSol->GetCurSel();
  char sStr[10];
  itoa(iSol, sStr, 10);
  outtextMSG2("SOLACT");
  outtextMSG2(sStr);
  Refresh();
}

void CSOLDialog::OnBnClickedDel()
{
  // TODO: Add your control notification handler code here
  int iDel;
  int i;
  CListBox* pSol=(CListBox*) GetDlgItem(IDC_SOL_LST);
  iDel=pSol->GetCurSel();
  if (pSOL->iNo>-1)
  {
    if (pSOL->iCur==iDel)
      pSOL->iCur=-1;
    else if (pSOL->iCur>iDel)
      pSOL->iCur--;
    delete(pSOL->pSols[iDel]);
    for (i=iDel;i<pSOL->iNo-1;i++)
    {
       pSOL->pSols[i]=pSOL->pSols[i+1];
    }
    pSOL->iNo--;
  }
  Refresh();
}

//****************************************************************************
//                        SOLUTION SETS DIALOG BOX
//****************************************************************************
CSTEPSDialog::CSTEPSDialog() : CDialog(CSTEPSDialog::IDD, NULL)
{
iNoLC=0;
iNoBC=0;
iNoTC=0;
int i;
for (i=0;i<100;i++)
{
   sTC[i]="N/A";
   iTC[i]=-1;
   sBC[i]="N/A";
   iBC[i]=-1;
   sLC[i]="N/A";
   iLC[i]=-1;
}

}




int CSTEPSDialog::GetLCInd(int ID)
{
  int iRC=-1;
  int i;
  for(i=0;i<iNoLC;i++)
  {
    if (iLC[i]==ID)
    {
      iRC=i;
      break;
    }
  }
  return(iRC);
}

int CSTEPSDialog::GetBCInd(int ID)
{
  int iRC=-1;
  int i;
  for(i=0;i<iNoBC;i++)
  {
    if (iBC[i]==ID)
    {
      iRC=i;
      break;
    }
  }
  return(iRC);
}

int CSTEPSDialog::GetTCInd(int ID)
{
  int iRC=-1;
  int i;
  for(i=0;i<iNoTC;i++)
  {
    if (iTC[i]==ID)
    {
      iRC=i;
      break;
    }
  }
  return(iRC);
}

void CSTEPSDialog::AddSet(int iID,CString sTXT,int iT)
{
  if (iT==0)
  {
    iLC[iNoLC]=iID;
    sLC[iNoLC]=sTXT;
    iNoLC++;
  }
  else if (iT==1)
  {
    iBC[iNoBC]=iID;
    sBC[iNoBC]=sTXT;
    iNoBC++;
  }
  else if (iT==2)
  {
    iTC[iNoTC]=iID;
    sTC[iNoTC]=sTXT;
    iNoTC++;
  }
}

void CSTEPSDialog::Refresh()
{
int i;

CStatic* pSolTit=(CStatic*) GetDlgItem(IDC_CURSOL_TXT );
CStatic* pSt = (CStatic*)GetDlgItem(IDC_STATIC_STEP_ACT);
if (pSOL->iCur == -1)
{
  pSolTit->SetWindowTextA("ERROR: No Active Solution Sequence!");
  pSt->SetWindowTextA("ERROR.");
}
else
  pSolTit->SetWindowTextA(pSOL->pSols[pSOL->iCur]->sTitle);
CComboBox* pLC=(CComboBox*) GetDlgItem(IDC_LC_DD);
pLC->ResetContent();
for (i=0;i<iNoLC;i++)
{
   pLC->AddString(sLC[i]);
}
CComboBox* pBC=(CComboBox*) GetDlgItem(IDC_BC_DD);
pBC->ResetContent();
for (i=0;i<iNoBC;i++)
{
   pBC->AddString(sBC[i]);
}
CComboBox* pTC=(CComboBox*) GetDlgItem(IDC_TC_DD);
pTC->ResetContent();
for (i=0;i<iNoTC;i++)
{
   pTC->AddString(sTC[i]);
}
CComboBox* pRS=(CComboBox*) GetDlgItem(IDC_RS_DD);
pRS->ResetContent();
pRS->AddString("NO");
pRS->AddString("YES");
//Sol seq
CListBox* pSol=(CListBox*) GetDlgItem(IDC_STEP_LBX);
pSol->ResetContent();
Solution* pS;
char S1[200]="";
if (pSOL!=NULL)
{
  if (pSOL->iCur!=-1)
  {
    pS=pSOL->pSols[pSOL->iCur];
    for (i=0;i<pS->iNo;i++)
    {
      //sprintf_s(S1,"STEP: %i LC: %i BC: %i TSET: %i RS: %i %s",i,pS->LS[i],pS->BS[i],pS->TS[i],pS->RS[i],pS->sStepTitle[i]);
      pSol->AddString(pSOL->GetCurSolution()->GetStepTitleString(i));
    }
  }
}

if ((pSOL->iCur>-1) && (pSOL->iCur<pSOL->iNo))
{
  int iC;
  iC=pSOL->GetCurStep();
  if (iC>-1)
  {
    pSol->SetCurSel(iC);
    pSt->SetWindowTextA(pSOL->GetCurSolution()->GetStepTitleString(iC));
  }
  else
  {
    pSt->SetWindowTextA("ERROR: No Step Active.");
  }
}
}


BOOL CSTEPSDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowTextA(sTitle);
    Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CSTEPSDialog, CDialog)
  ON_WM_CREATE()
  ON_LBN_SELCHANGE(IDC_STEP_LBX, &CSTEPSDialog::OnLbnSelchangeStepLbx)
  ON_BN_CLICKED(IDC_CREATE, &CSTEPSDialog::OnBnClickedCreate)
  ON_BN_CLICKED(IDC_ACT_STEP, &CSTEPSDialog::OnBnClickedActStep)
  ON_BN_CLICKED(IDOK, &CSTEPSDialog::OnBnClickedOk)
  ON_BN_CLICKED(IDC_STEP_DEL, &CSTEPSDialog::OnBnClickedStepDel)
END_MESSAGE_MAP()

int CSTEPSDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  Add your specialized creation code here

  return 0;
}

void CSTEPSDialog::OnLbnSelchangeStepLbx()
{
  // TODO: Add your control notification handler code here
  //int iStep;
  //int iI;
  //Solution* pS;
  //CListBox* pSets=(CListBox*) GetDlgItem(IDC_STEP_LBX);
  //iStep=pSets->GetCurSel();
  //if (pSOL!=NULL)
  //{
  //  pS=pSOL->pSols[iStep];
  //  iI=GetLCInd(pS->LS[iStep]);
  //  if (iI!=-1)
  //  {
  //     CComboBox* pLC=(CComboBox*) GetDlgItem(IDC_LC_DD);
  //     pLC->SetCurSel(iI);
  //  }
  //  iI=GetBCInd(pS->BS[iStep]);
  //  if (iI!=-1)
  //  {
  //     CComboBox* pBC=(CComboBox*) GetDlgItem(IDC_BC_DD);
  //     pBC->SetCurSel(iI);
  //  }
  //  iI=GetTCInd(pS->TS[iStep]);
  //  if (iI!=-1)
  //  {
  //     CComboBox* pTC=(CComboBox*) GetDlgItem(IDC_TC_DD);
  //     pTC->SetCurSel(iI);
  //  }
  //  CEdit* pST=(CEdit*) GetDlgItem(IDC_STPTIT_TB);
  //  pST->SetWindowTextA(pS->sStepTitle[iStep]);
  //}
  //CComboBox* pRS=(CComboBox*) GetDlgItem(IDC_RS_DD);
  //if(pS->RS[iStep]==0)
  //  pRS->SetCurSel(0);
  //else
  //  pRS->SetCurSel(1);
}



void CSTEPSDialog::OnBnClickedCreate()
{
  // TODO: Add your control notification handler code here
  CString sTitle;
  int iRS;
  int iLSet;
  int iBSet;
  int iTSet;
  char S1[20]="";

  CComboBox* pCB;
  // TODO: Add your control notification handler code here
  CEdit* pTitle=(CEdit*) GetDlgItem(IDC_STPTIT_TB);
  pTitle->GetWindowText(sTitle);
  pCB=(CComboBox*) GetDlgItem(IDC_LC_DD);
  iLSet=pCB->GetCurSel();
  pCB=(CComboBox*) GetDlgItem(IDC_BC_DD);
  iBSet=pCB->GetCurSel();
  pCB=(CComboBox*) GetDlgItem(IDC_TC_DD);
  iTSet=pCB->GetCurSel();
  pCB=(CComboBox*) GetDlgItem(IDC_RS_DD);
  iRS=pCB->GetCurSel();


  outtextMSG2("STEPCR");
  outtextMSG2(sTitle);
  sprintf_s(S1,"%i",iLC[iLSet]);
  outtextMSG2(S1);
  sprintf_s(S1,"%i",iBC[iBSet]);
  outtextMSG2(S1);
  if (iTSet!=-1)
  {
    sprintf_s(S1,"%i",iTC[iTSet]);
    outtextMSG2(S1);
  }
  else
  {
    outtextMSG2("");
  }
  sprintf_s(S1,"%i",iRS);
  outtextMSG2(S1);
  Refresh();
}

void CSTEPSDialog::OnBnClickedActStep()
{
  // TODO: Add your control notification handler code here
  CListBox* pStep=(CListBox*) GetDlgItem(IDC_STEP_LBX);
  int iStep = pStep->GetCurSel();
  char sStr[10];
  itoa(iStep, sStr, 10);
  outtextMSG2("STEPACT");
  outtextMSG2(sStr);
  Refresh();
}

void CSTEPSDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnOK();
}

void CSOLDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnOK();
}

void CSTEPSDialog::OnBnClickedStepDel()
{

  // TODO: Add your control notification handler code here
  int iDel;
  Solution* pS=NULL;

  CListBox* pSol=(CListBox*) GetDlgItem(IDC_STEP_LBX);
  iDel=pSol->GetCurSel();
  pS=pSOL->GetCurSolution();
  if (pS!=NULL)
    pS->DelStep(iDel);

  Refresh();
}



void CSETSDialog::OnEnChangeSetid()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}



//*****************************************************************
//The Entity Editor dialog box
//*****************************************************************
const int MAX_VALS = 500;

CEntEditDialog::CEntEditDialog()
  : CDialog(CEntEditDialog::IDD, NULL)
{
  pEnt=NULL;
  pO = NULL;
  PT = NULL;
  m_iItemBeingEdited==1;
}

BOOL CEntEditDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  eEdit = (CEdit*) GetDlgItem(IDC_EDIT_FLOAT);
  if (pEnt!=NULL)
    Populate2();
  if (pO != NULL)
	Populate1();
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntEditDialog::Populate1()
{
	CString sVName[MAX_VALS];
	CString sVVals[MAX_VALS];
	int nItem;
	int i;

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT
		| LVS_EX_GRIDLINES
		| LVS_EX_HEADERDRAGDROP
		| LVS_OWNERDRAWFIXED);
	CRect oSize;
	m_List.GetWindowRect(oSize);
	int iW = oSize.Width();

	m_List.InsertColumn(0, "Variable Name", LVCFMT_LEFT, iW - 250);
	m_List.InsertColumn(1, "Value", LVCFMT_LEFT, 250);

	Ed_Title.SetWindowTextA(pO->GetName());
	char S1[80] = "";
	sprintf_s(S1, "%i", pO->iLabel);
	CString ss;
	ss = S1;
	Ed_ID.SetWindowTextA(ss);;

	iNo = pO->GetVarHeaders(sVName);
	iNo2 = pO->GetVarValues(sVVals);
	for (i = 0; i < iNo; i++)
	{
		nItem = m_List.InsertItem(i, sVName[i]);
		m_List.SetItemText(nItem, 1, sVVals[i]);
	}

}

void CEntEditDialog::Populate2()
{
CString sVName[MAX_VALS];
CString sVVals[MAX_VALS];
int nItem;
int i;

m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT
                       |LVS_EX_GRIDLINES
                       | LVS_EX_HEADERDRAGDROP
                       | LVS_OWNERDRAWFIXED);
CRect oSize;
m_List.GetWindowRect(oSize);
int iW= oSize.Width();

m_List.InsertColumn(0, "Variable Name", LVCFMT_LEFT, iW-250);
m_List.InsertColumn(1, "Value", LVCFMT_LEFT, 250);

Ed_Title.SetWindowTextA(pEnt->sTitle);
char S1[80] = "";
sprintf_s(S1, "%i", pEnt->iID);
CString ss;
ss=S1;
Ed_ID.SetWindowTextA(ss);;

iNo = pEnt->GetVarHeaders(sVName);
iNo2 = pEnt->GetVarValues(sVVals);
for (i = 0; i<iNo; i++)
{
  nItem = m_List.InsertItem(i, sVName[i]);
  m_List.SetItemText(nItem, 1, sVVals[i]);
}

}



BEGIN_MESSAGE_MAP(CEntEditDialog, CDialog)
ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDOK, &CEntEditDialog::OnBnClickedOk)
ON_BN_CLICKED(IDC_ENTLIST, &CEntEditDialog::OnBnClickedEntlist)
ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CEntEditDialog::OnDblclkList1)
END_MESSAGE_MAP()


void CEntEditDialog::OnEnChangeEditFloat()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}


BOOL CEntEditDialog::PreTranslateMessage(MSG* pMsg)
{
  // TODO: Add your specialized code here and/or call the base class
  if (pMsg->wParam == VK_ESCAPE)
  {
    eEdit->ShowWindow(SW_HIDE);
    m_iItemBeingEdited = -1;
    return TRUE;
  }
  else if((pMsg->wParam == VK_RETURN) && (m_iItemBeingEdited!=-1))
  {
    CString sNew;
    eEdit->GetWindowText(sNew);    
    eEdit->ShowWindow(SW_HIDE);
    m_List.SetItemText(m_iItemBeingEdited, 1, sNew);
    m_iItemBeingEdited=-1;
    return TRUE;
  }
  else
  {
    return CDialog::PreTranslateMessage(pMsg);
  }
}


void CEntEditDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  
  CDialog::OnLButtonDown(nFlags, point);
}


void CEntEditDialog::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  int i;
  CString sVVals[50];
  CEdit* pT = (CEdit*) GetDlgItem(IDC_PTITLE);
  if ((pEnt!=NULL) || (pO!=NULL))
  {
	if (pEnt != NULL)
		pT->GetWindowTextA(pEnt->sTitle);
    for (i=0;i<iNo;i++)
    {
      sVVals[i] = m_List.GetItemText(i,1);
      CString sS;
      sS = m_List.GetItemText(i, 1);
    }
	if (pEnt!=NULL)
      pEnt->PutVarValues(iNo, sVVals);
	if (pO != NULL)
	  pO->PutVarValues(PT,iNo, sVVals);
  }
  CDialog::OnOK();
}


void CEntEditDialog::OnBnClickedEntlist()
{
  // TODO: Add your control notification handler code here
  int i;
  CString sTemp;
  outtext1("Entity Data Listing:-");
  CEdit* pT = (CEdit*)GetDlgItem(IDC_PTITLE);
  CEdit* pID = (CEdit*)GetDlgItem(IDC_ENTID);
  pT->GetWindowTextA(sTemp);
  outtext1(sTemp);
  pID->GetWindowTextA(sTemp);
  outtext1(sTemp);
  for (i=0;i<iNo;i++)
  {
    sTemp = m_List.GetItemText(i, 0);
    outtext1(sTemp);
    sTemp = m_List.GetItemText(i, 1);
    outtext1(sTemp);
  }
}


void CEntEditDialog::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  // TODO: Add your control notification handler code here
  POSITION iSeA = m_List.GetFirstSelectedItemPosition();
  int iRow= m_List.GetSelectionMark();
  m_iItemBeingEdited=iRow;
  *pResult = 0;
  CRect r;
  int iR= m_List.GetColumnWidth(0);
  int iL = m_List.GetColumnWidth(1);
  m_List.GetItemRect(iRow, &r, LVIR_BOUNDS);
  eEdit->SetParent(&m_List);
  eEdit->MoveWindow(iR, r.top-2, iL, (r.bottom - r.top)+4, 1);
  CString sS;
  sS= m_List.GetItemText(iRow,1);
  
  eEdit->SetWindowText(sS);
  eEdit->SetSel(0, -1, TRUE);
  eEdit->ShowWindow(SW_SHOW);
  eEdit->SetFocus();
  
}
void CEntEditDialog::DoDataExchange(CDataExchange* pDX)
{
  // TODO: Add your specialized code here and/or call the base class

  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PTITLE, Ed_Title);
  DDX_Control(pDX, IDC_ENTID, Ed_ID);
  DDX_Control(pDX, IDC_LIST1, m_List);
}
// C:\Users\Roy\Documents\Visual Studio 2017\Projects\M3da\M3da\G_Object.cpp : implementation file
//




void CEntEditDialog::OnBnClickedMfclink2()
{
  // TODO: Add your control notification handler code here
}

//***************************************************************
// Basic class for visulising lamina in latimate stack
//***************************************************************
Lamina::Lamina()
{
  dZOFFS=0;
  pVertex[0].Set(-0.7,-0.5,0);
  pVertex[1].Set(0.7, -0.5, 0);
  pVertex[2].Set(0.7, 0.5, 0);
  pVertex[3].Set(-0.7, 0.5, 0);
}

Lamina::~Lamina()
{
}

void Lamina::SetZ(double dZ)
{
  dZOFFS =dZ;
  pVertex[0].z = dZ;
  pVertex[1].z = dZ;
  pVertex[2].z = dZ;
  pVertex[3].z = dZ;
}


void Lamina::SetThk(double dT)
{
  dThk = dT;
}

void Lamina::SetAng(double dA)
{
  dMAng=dA;
}

void Lamina::OglDraw()
{
  C3dMatrix R;
  R.MakeUnit();
  R.Rotate(0, 0, dMAng);
  C3dVector vTmp;
  vTmp=R*pVertex[0];
  glColor3fv(cols[144]);
  glBegin(GL_POLYGON);
    vTmp = R*pVertex[0];
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
    vTmp = R*pVertex[1];
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
    vTmp = R*pVertex[2];
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
    vTmp = R*pVertex[3];
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
  glEnd();

  //Draw fibres
  double dWid;
  double dWidInc;
  int iInc=2;
  C3dVector vS;
  C3dVector vE;
  int iC=0;
  dWid= pVertex[3].y- pVertex[0].y;
  dWidInc=dWid/iInc;
  vS=pVertex[0];
  vE=pVertex[1];
  glColor3fv(cols[0]);
  
  glBegin(GL_LINES);
  for (iC =0; iC<iInc+1; iC++)
  {
    vTmp = R*vS;
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
    vTmp = R*vE;   
    glVertex3f((float)(vTmp.x), (float)(vTmp.y), (float)(vTmp.z + dZOFFS));
    vS.y += dWidInc;
    vE.y += dWidInc;
  }
  glEnd();

}

//**************************************************
//  T H E    P C O M P    E D I T O R
//**************************************************
CPcompEditor::CPcompEditor()
  : CDialog(IDD_PCOMPEDIT, NULL)
{
iNoLayers=0;
pEnt=NULL;
hdcOld = wglGetCurrentDC();
hrcOld = wglGetCurrentContext();
hrc = NULL;
hdc = NULL;
vMat.MakeUnit();
}


void CPcompEditor::AddVisLayer(double dA, double dZ)
{
  Laminate[iNoLayers].SetAng(dA);
  Laminate[iNoLayers].SetZ(dZ);
  iNoLayers++;
}

CPcompEditor::~CPcompEditor()
{
  if (hrc != NULL)
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hrc);
  }

  if (pDrg != NULL)
    delete(pDrg);
  wglMakeCurrent(hdcOld, hrcOld);
}

void CPcompEditor::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPcompEditor, CDialog)
  ON_BN_CLICKED(IDOK, &CPcompEditor::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTTON1, &CPcompEditor::OnBnClickedButton1)
//  ON_WM_PAINT()
ON_WM_PAINT()
END_MESSAGE_MAP()

void CPcompEditor::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  CDialog::OnOK();
}


BOOL CPcompEditor::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Add extra initialization here
  pDrg = new CWnd;
  pDrg->Create(_T("STATIC"), _T("Hi"), WS_CHILD | WS_VISIBLE | WS_THICKFRAME,
        CRect(20, 20, 300, 300), this, 1234);
  InitOGL();
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


void CPcompEditor::InitOGL()
{
  static PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
    PFD_SUPPORT_OPENGL |          // support OpenGL
    PFD_DOUBLEBUFFER,             // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    24,                             // 24-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    32,                             // 32-bit z-buffer
    0,                              // no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
  };

  // Get device context only once.
  hdc = pDrg->GetDC()->m_hDC;
  // Pixel format.
  m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
  SetPixelFormat(hdc, m_nPixelFormat, &pfd);

  // Create the OpenGL Rendering Context.
  hrc = wglCreateContext(hdc);
  wglMakeCurrent(hdc, hrc);



  // Send draw request
  //OnDraw(NULL);
}

void CPcompEditor::OglDraw()
{

  glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0f);
  CRect cR;
  pDrg->GetWindowRect(&cR);
  int iW = cR.Width();
  int iH = cR.Height();
  glViewport(0, 0, iW, iH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  OglMat mOGLmat= vMat.GetOglMat();
  glMultMatrixf(mOGLmat.fMat);
  //gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);
  glMatrixMode(GL_MODELVIEW);
  glPointSize(10.0f);
  glColor3f((float) 0.0, (float) 0.0, (float)0.9);
  glBegin(GL_POINTS);
  glVertex3f(20.0, 30.0, 10.0);
  glVertex3f(140.0, 30.0, 20.0);
  glVertex3f(50.0, 50.0, 30.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.5, 0.0, 0.0);
  glVertex3f(0.0, 0.5, 0.0);
  glEnd();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  char sLab[20];

  int i;
  for (i = 0; i<iNoLayers; i++)
    Laminate[i].OglDraw();
  sprintf_s(sLab, "%s", "Z");
  OglString(1, 0.0, 0.0, 0.7, &sLab[0]);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.7);
  glEnd();
  sprintf_s(sLab, "%s", "1");
  OglString(1, 0.7, 0.0, 0.0, &sLab[0]);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.7, 0.0, 0.0);
  glEnd();

  glFinish();
  SwapBuffers(wglGetCurrentDC());
}

void CPcompEditor::OnBnClickedButton1()
{
  // TODO: Add your control notification handler code here
  //OglDraw();
  vMat.Rotate(-85,0,7.5);

  AddVisLayer(0.0, -0.5);
  AddVisLayer(45.0,-0.4);
  AddVisLayer(90.0, -0.3);
  AddVisLayer(45.0, -0.2);
  AddVisLayer(0.0,  -0.1);
  AddVisLayer(90.0, 0);
  AddVisLayer(0.0, 0.1);
  AddVisLayer(45.0, 0.2);
  AddVisLayer(90.0, 0.3);
  AddVisLayer(45.0, 0.4);
  AddVisLayer(0.0, 0.5);
}

void CPcompEditor::OnPaint()
{
  //CPaintDC dc(this); // device context for painting
  OglDraw();
}

//***************************************************
// 2d Material Point Method State Variable
// 06/03/2019
//***************************************************
MPMVar2d::MPMVar2d()
{
  Pid=-1;            //Particle ID
  pNode = NULL;
  BCellid=-1;        //Euler Cell ID
  dMp = 1.0;         //Particle Mass
  dVol = 1.0;        //Particle Volume
  dvFp.Create(2, 2);
  *dvFp.mn(1, 1) = 1.0;  //Deformation gradient
  *dvFp.mn(1, 2) = 0.0;
  *dvFp.mn(2, 1) = 0.0;
  *dvFp.mn(2, 2) = 1.0;
  ds.Create(3, 1);
  *ds.mn(1, 1) = 0.0;   //Stress
  *ds.mn(2, 1) = 0.0;   //Stress
  *ds.mn(3, 1) = 0.0;   //Stress
  deps.Create(3, 1);
  *deps.mn(1, 1) = 0.0;   //Strain
  *deps.mn(2, 1) = 0.0;   //Strain
  *deps.mn(3, 1) = 0.0;   //Strain
  dVp.Create(2, 1);
  *dVp.mn(1, 1) = 0.0;   //Velocity
  *dVp.mn(2, 1) = 0.0;   //Velocity
}

MPMVar2d::~MPMVar2d()
{
  dvFp.clear();       //Deformation gradient  
  ds.clear();         //Stress
  deps.clear();;       //Strain
  dVp.clear();;        //Velocity
  dXp.clear();;        //Position
}



//***************************************************
// 2d Material Point Euler Nodal Data
// 06/03/2019
//***************************************************
Ndata::Ndata()
{
  nMass=0;
  nMomentum.Create(2, 1);
  nMomentum.MakeZero();
  nIForce.Create(2, 1);
  nIForce.MakeZero();
  nEforce.Create(2, 1);
  nEforce.MakeZero();
}

void Ndata::Reset()
{
  nMass = 0;
  nMomentum.MakeZero();
  nIForce.MakeZero();
  nEforce.MakeZero();
}


Ndata::~Ndata()
{
  nMomentum.clear();
  nIForce.clear();
  nEforce.clear();
}




