#include <iostream>
#include "CustomziedMdSpi.h"
#include "CustomziedTraderSpi.h"

using namespace std;

// ���ӿ�
#pragma comment (lib, "thostmduserapi_se.lib")
#pragma comment (lib, "thosttraderapi_se.lib")

// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                         // �����̴���
TThostFtdcInvestorIDType gInvesterID = "";                         // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "";                     // Ͷ��������
bool loginFlag = false;									    		// ��¼��־

// �������
CThostFtdcMdApi* g_pMdUserApi = nullptr;                           // ����ָ��
char gMdFrontAddr[] = "tcp://180.168.146.187:10211";               // ����ǰ�õ�ַ
char* g_pInstrumentID[] = {"au2502"};			                   // �����Լ�����б��н���������������������֣��������������ѡһ��
int instrumentNum = 1;                                             // �����Լ��������

// ���ײ���
CThostFtdcTraderApi* g_pTradeUserApi = nullptr;                    // ����ָ��
char gTradeFrontAddr[] = "tcp://180.168.146.187:10201";            // ����ǰ�õ�ַ
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "au2502";        // �����׵ĺ�Լ����
TThostFtdcDirectionType gTradeDirection;                           // ��������
TThostFtdcPriceType	Price;                                         // ���׼۸�
TThostFtdcVolumeType gVolumeTotalOriginal = 1;                     // ��������
TThostFtdcCombOffsetFlagType gCombOffsetFlag;                      // ��Ͽ�ƽ��־
TThostFtdcFrontIDType	trade_front_id;                            // ǰ�ñ��
TThostFtdcSessionIDType	session_id;                                // �Ự���
TThostFtdcOrderRefType	order_ref;                                 // ��������

int main() {
	// �˺�����
	std::cout << "�������˺ţ� " << std::endl;
	std::cin >> gInvesterID;
	std::cout << "���������룺 " << std::endl;
	std::cin >> gInvesterPassword;

	// ��ʼ�����鹤���̣߳���CTP��������
	cout << "��ʼ������..." << endl;
	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();             // ��������ʵ��
	CThostFtdcMdSpi* pMdUserSpi = new CustomziedMdSpi;             // ��������ص�ʵ��
	g_pMdUserApi->RegisterSpi(pMdUserSpi);                         // ע���¼���
	// ���ӽ���֪ͨ
	g_pMdUserApi->RegisterFront(gMdFrontAddr);                     // ��������ǰ�õ�ַ
	g_pMdUserApi->Init();                                          // ��������

	// ��ʼ�����׹����̣߳���CTP��������
	cout << "��ʼ������..." << endl;
	g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(); // ��������ʵ��
	CThostFtdcTraderSpi* pTradeSpi = new CustomziedTraderSpi;     // �������׻ص�ʵ��
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                      // ע���¼���
	// ���ӽ���֪ͨ
	g_pTradeUserApi->RegisterFront(gTradeFrontAddr);              // ���ý���ǰ�õ�ַ
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_QUICK);      // ���Ĺ�����
	g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);     // ����˽����
	g_pTradeUserApi->Init();                                      // ��������
	system("pause");

	// ��CTP���������¼
	CThostFtdcReqUserLoginField loginReq;
	memset(&loginReq, 0, sizeof(loginReq));
	strcpy_s(loginReq.BrokerID, gBrokerID);
	strcpy_s(loginReq.UserID, gInvesterID);
	strcpy_s(loginReq.Password, gInvesterPassword);
	static int requestID1 = 0;
	int rt1 = g_pMdUserApi->ReqUserLogin(&loginReq, ++requestID1);	   // ��¼Ӧ��
	if (!rt1) {
		std::cout << "<<<<<<���������¼����ɹ�<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<���������¼����ʧ��<<<<<<" << std::endl;
	}	

	// ��CTP�����׵�¼
	static int requestID2 = 0;
	int rt2 = g_pTradeUserApi->ReqUserLogin(&loginReq, ++requestID2);	   // ��¼Ӧ��
	if (!rt2){
		std::cout << "<<<<<<���ͽ��׵�¼����ɹ�<<<<<<" << std::endl; 
	}
	else{ 
		std::cerr << "<<<<<<���ͽ��׵�¼����ʧ��<<<<<<" << std::endl; 
	}
	system("pause");

	// Ͷ���߽�����ȷ��
	CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
	memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
	strcpy_s(settlementConfirmReq.BrokerID, gBrokerID);
	strcpy_s(settlementConfirmReq.InvestorID, gInvesterID);
	static int requestID3 = 0;
	int rt3 = g_pTradeUserApi->ReqSettlementInfoConfirm(&settlementConfirmReq, ++requestID3);
	if (!rt3) { 
		std::cout << "<<<<<<����Ͷ���߽�����ȷ������ɹ�<<<<<<" << std::endl; 
	}
	else { 
		std::cerr << "<<<<<<����Ͷ���߽�����ȷ������ʧ��<<<<<<" << std::endl; 
	}
	system("pause");

	// �����ѯ��Լ
	CThostFtdcQryInstrumentField instrumentReq;
	memset(&instrumentReq, 0, sizeof(instrumentReq));
	strcpy_s(instrumentReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID4 = 0; // ������
	int rt4 = g_pTradeUserApi->ReqQryInstrument(&instrumentReq, ++requestID4);
	if (!rt4)
		std::cout << "<<<<<<���ͺ�Լ��ѯ����ɹ�<<<<<<" << std::endl;
	else
		std::cerr << "<<<<<<���ͺ�Լ��ѯ����ʧ��<<<<<<" << std::endl;
	system("pause");

	// �����ѯͶ���ֲ߳�
	CThostFtdcQryInvestorPositionField postionReq;
	memset(&postionReq, 0, sizeof(postionReq));
	strcpy_s(postionReq.BrokerID, gBrokerID);
	strcpy_s(postionReq.InvestorID, gInvesterID);
	strcpy_s(postionReq.InstrumentID, g_pTradeInstrumentID);
	static int requestID5 = 0; // ������
	int rt5 = g_pTradeUserApi->ReqQryInvestorPosition(&postionReq, ++requestID5);
	if (!rt5) { 
		std::cout << "<<<<<<����Ͷ���ֲֲ߳�ѯ����ɹ�<<<<<<" << std::endl; 
	}
	else {
		std::cerr << "<<<<<<����Ͷ���ֲֲ߳�ѯ����ʧ��<<<<<<" << std::endl;
	}
	system("pause");

	// �����ѯͶ�����ʽ��˻�
	CThostFtdcQryTradingAccountField tradingAccountReq;
	memset(&tradingAccountReq, 0, sizeof(tradingAccountReq));
	strcpy_s(tradingAccountReq.BrokerID, gBrokerID);
	strcpy_s(tradingAccountReq.InvestorID, gInvesterID);
	static int requestID6 = 0; // ������
	int rt6 = g_pTradeUserApi->ReqQryTradingAccount(&tradingAccountReq, ++requestID6);
	if (!rt6) { 
		std::cout << "<<<<<<����Ͷ�����ʽ��˻���ѯ����ɹ�<<<<<<" << std::endl; 
	}
	else { 
		std::cerr << "<<<<<<����Ͷ�����ʽ��˻���ѯ����ʧ��<<<<<<" << std::endl; 
	}
	system("pause");
	
	/////////////////////////////////////////////////////////////////////////
	// ��CTP��������
	int rt7 = g_pMdUserApi->SubscribeMarketData(g_pInstrumentID, instrumentNum);	   // ���鶩��Ӧ��ʧ�� / �ɹ�����������������
	if (!rt7) {
		std::cout << "<<<<<<���Ͷ�������ɹ�<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<���Ͷ�������ʧ��<<<<<<" << std::endl;
	}
	std::cout << "��һ�����뽻�׻��ڣ��ɲ���������... " << std::endl;
	system("pause");

	// ���׼۸񡢽��׷��򡢿�ƽ����
	char temp1[] = "";
	char temp2;
	char temp3;
	std::cout << "�����뽻�׼۸� " << std::endl;
	std::cin >> temp1;
	Price = atof(temp1);
	std::cout << "�����뽻�׷���0�������룬1��������" << std::endl;
	std::cin >> temp2;
	if (temp2 == '0') {
		gTradeDirection = THOST_FTDC_D_Buy;
	}
	else {
		gTradeDirection = THOST_FTDC_D_Sell;
	}
	std::cout << "�����뿪ƽ����0�����֣�1����ƽ�֣�2����ǿƽ��3����ƽ��4����ƽ��5����ǿ����6������ǿƽ" << std::endl;
	std::cin >> temp3;
	if (temp3 == '0') {
		*gCombOffsetFlag = THOST_FTDC_OF_Open;
	}
	else if (temp3 == '1'){
		*gCombOffsetFlag = THOST_FTDC_OF_Close;
	}
	else if (temp3 == '2'){
		*gCombOffsetFlag = THOST_FTDC_OF_ForceClose;
	}
	else if (temp3 == '3') {
		*gCombOffsetFlag = THOST_FTDC_OF_CloseToday;
	}
	else if (temp3 == '4') {
		*gCombOffsetFlag = THOST_FTDC_OF_CloseYesterday;
	}
	else if (temp3 == '5') {
		*gCombOffsetFlag = THOST_FTDC_OF_ForceOff;
	}
	else if (temp3 == '6') {
		*gCombOffsetFlag = THOST_FTDC_OF_LocalForceClose;
	}
	else {
		std::cerr << "�������" << std::endl;
	}

	// ������������������
	CThostFtdcInputOrderField orderInsertReq;
	memset(&orderInsertReq, 0, sizeof(orderInsertReq));
	///���͹�˾����
	strcpy_s(orderInsertReq.BrokerID, gBrokerID);
	///Ͷ���ߴ���
	strcpy_s(orderInsertReq.InvestorID, gInvesterID);
	///��Լ����
	strcpy_s(orderInsertReq.InstrumentID, g_pTradeInstrumentID);
	///�������ã�δ���õ���ģʽ
	strcpy_s(orderInsertReq.OrderRef, order_ref);
	///�����۸�����: �޼�
	orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	orderInsertReq.Direction = gTradeDirection;
	///��Ͽ�ƽ��־: ����
	orderInsertReq.CombOffsetFlag[0] = *gCombOffsetFlag;
	///���Ͷ���ױ���־
	orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	orderInsertReq.LimitPrice = Price;
	///������1
	orderInsertReq.VolumeTotalOriginal = gVolumeTotalOriginal;
	///��Ч������: ������Ч
	orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
	///�ɽ�������: �κ�����
	orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	orderInsertReq.MinVolume = 1;
	///��������: ����
	orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ǿƽԭ��: ��ǿƽ
	orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	orderInsertReq.IsAutoSuspend = 0;
	///�û�ǿ����־: ��
	orderInsertReq.UserForceClose = 0;

	static int requestID8 = 0; // ������
	int rt8 = g_pTradeUserApi->ReqOrderInsert(&orderInsertReq, ++requestID8);
	if (!rt8)
		std::cout << ">>>>>>���ͱ���¼������ɹ�" << std::endl;
	else
		std::cerr << "--->>>���ͱ���¼������ʧ��" << std::endl;
	system("pause");
	
	/////////////////////////////////////////////////////////////////////////
	// ��CTPȡ����������
	int rt9 = g_pMdUserApi->UnSubscribeMarketData(g_pInstrumentID, instrumentNum);
	if (!rt9)
	{
		std::cout << "<<<<<<����ȡ��������������ɹ�<<<<<<" << std::endl;
	}
	else
	{
		std::cerr << "<<<<<<����ȡ��������������ʧ��<<<<<<" << std::endl;
	}
	system("pause");
	
	// ��CTP��������ǳ�
	CThostFtdcUserLogoutField logoutReq;
	memset(&logoutReq, 0, sizeof(logoutReq));
	strcpy_s(logoutReq.BrokerID, gBrokerID);
	strcpy_s(logoutReq.UserID, gInvesterID);
	static int requestID10 = 0;
	int rt10 = g_pMdUserApi->ReqUserLogout(&logoutReq, ++requestID10);	   // �ǳ�Ӧ��
	if (!rt10) {
		std::cout << "<<<<<<��������ǳ�����ɹ�<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<��������ǳ�����ʧ��<<<<<<" << std::endl;
	}
	system("pause");

	// ��CTP�����׵ǳ�
	static int requestID11 = 0;
	int rt11 = g_pTradeUserApi->ReqUserLogout(&logoutReq, ++requestID11);	   // �ǳ�Ӧ��
	if (!rt11) {
		std::cout << "<<<<<<���ͽ��׵ǳ�����ɹ�<<<<<<" << std::endl;
	}
	else {
		std::cerr << "<<<<<<���ͽ��׵ǳ�����ʧ��<<<<<<" << std::endl;
	}
	system("pause");

	// �߳��˳�
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();
	system("pause");

	return 0;
}