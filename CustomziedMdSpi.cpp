#include <iostream>
#include <fstream>
#include <string>
#include "CustomziedMdSpi.h"
using namespace std;

extern CThostFtdcMdApi* g_pMdUserApi;
extern TThostFtdcBrokerIDType gBrokerID;
extern TThostFtdcInvestorIDType gInvesterID;
extern TThostFtdcPasswordType gInvesterPassword;
extern char* g_pInstrumentID[];
extern int instrumentNum;
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;

void CustomziedMdSpi::OnFrontConnected() {
	std::cout << "<<<<<<�����������ӳɹ�<<<<<<" << std::endl;
};

void CustomziedMdSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "<<<<<<�������ӶϿ�<<<<<<" << std::endl;
	std::cerr << "�����룺" << nReason << std::endl;
};

void CustomziedMdSpi::OnHeartBeatWarning(int nTimeLapse) {
	std::cerr << "<<<<<<����������ʱ<<<<<<" << std::endl;
	std::cerr << "���ϴ�����ʱ�䣺" << nTimeLapse << std::endl;
};

void CustomziedMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult){
		std::cout << "<<<<<<�˻���¼�ɹ�<<<<<<" << std::endl;
		std::cout << "�����գ�" << pRspUserLogin->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺" << pRspUserLogin->LoginTime << std::endl;
		std::cout << "�����̣�" << pRspUserLogin->BrokerID << std::endl;
		std::cout << "�ʻ�����" << pRspUserLogin->UserID << std::endl;
	}
	else {
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<�˻��ǳ��ɹ�<<<<<<" << std::endl;
		std::cout << "�����̣�" << pUserLogout->BrokerID << std::endl;
		std::cout << "�ʻ�����" << pUserLogout->UserID << std::endl;
	}
	else { 
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl; 
	}
};

void CustomziedMdSpi::OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

void CustomziedMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
	{
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<��������ɹ�<<<<<<" << std::endl;
		std::cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else { 
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<ȡ����������ɹ�<<<<<<" << std::endl;
		std::cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<����ѯ�۳ɹ�<<<<<<" << std::endl;
		std::cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<ȡ������ѯ�۳ɹ�<<<<<<" << std::endl;
		std::cout << "��Լ���룺" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) {
	//std::cout << "<<<<<<����������<<<<<<" << std::endl;
	//std::cout << "�����գ� " << pDepthMarketData->TradingDay << std::endl;
	//std::cout << "���������룺 " << pDepthMarketData->ExchangeID << std::endl;
	//std::cout << "��Լ���룺 " << pDepthMarketData->InstrumentID << std::endl;
	//std::cout << "��Լ�ڽ������Ĵ��룺 " << pDepthMarketData->ExchangeInstID << std::endl;
	//std::cout << "���¼ۣ� " << pDepthMarketData->LastPrice << std::endl;
	//std::cout << "������ " << pDepthMarketData->Volume << std::endl;

	string fname = "au2502.csv";
	ofstream outFile(fname, ios::app);				  //���ļ�����д��,ios::app��ʾ���ļ�ĩβ׷��д��
	// ********д������*********
	outFile << pDepthMarketData->TradingDay << ','
			<< pDepthMarketData->InstrumentID << ','
			<< pDepthMarketData->ExchangeID << ','
			<< pDepthMarketData->ExchangeInstID << ','
			<< to_string(pDepthMarketData->LastPrice) << ','
			<< to_string(pDepthMarketData->PreSettlementPrice) << ','
			<< to_string(pDepthMarketData->PreClosePrice) << ','
			<< to_string(pDepthMarketData->PreOpenInterest) << ','
			<< to_string(pDepthMarketData->OpenPrice) << ','
			<< to_string(pDepthMarketData->HighestPrice) << ','
			<< to_string(pDepthMarketData->LowestPrice) << ','
			<< to_string(pDepthMarketData->Volume) << ','
			<< to_string(pDepthMarketData->Turnover) << ','
			<< to_string(pDepthMarketData->OpenInterest) << ','
			<< to_string(pDepthMarketData->ClosePrice) << ','
			<< to_string(pDepthMarketData->SettlementPrice) << ','
			<< to_string(pDepthMarketData->UpperLimitPrice) << ','
			<< to_string(pDepthMarketData->LowerLimitPrice) << ','
			<< to_string(pDepthMarketData->PreDelta) << ','
			<< to_string(pDepthMarketData->CurrDelta) << ','
			<< pDepthMarketData->UpdateTime << ','
			<< to_string(pDepthMarketData->UpdateMillisec) << ','
			<< to_string(pDepthMarketData->BidPrice1) << ','
			<< to_string(pDepthMarketData->BidVolume1) << ','
			<< to_string(pDepthMarketData->AskPrice1) << ','
			<< to_string(pDepthMarketData->AskVolume1) << ','
			<< to_string(pDepthMarketData->BidPrice2) << ','
			<< to_string(pDepthMarketData->BidVolume2) << ','
			<< to_string(pDepthMarketData->AskPrice2) << ','
			<< to_string(pDepthMarketData->AskVolume2) << ','
			<< to_string(pDepthMarketData->BidPrice3) << ','
			<< to_string(pDepthMarketData->BidVolume3) << ','
			<< to_string(pDepthMarketData->AskPrice3) << ','
			<< to_string(pDepthMarketData->AskVolume3) << ','
			<< to_string(pDepthMarketData->BidPrice4) << ','
			<< to_string(pDepthMarketData->BidVolume4) << ','
			<< to_string(pDepthMarketData->AskPrice4) << ','
			<< to_string(pDepthMarketData->AskVolume4) << ','
			<< to_string(pDepthMarketData->BidPrice5) << ','
			<< to_string(pDepthMarketData->BidVolume5) << ','
			<< to_string(pDepthMarketData->AskPrice5) << ','
			<< to_string(pDepthMarketData->AskVolume5) << ','
			<< to_string(pDepthMarketData->AveragePrice) << ','
			<< pDepthMarketData->ActionDay << endl;
	//������תΪ�ַ�������д��,csv�ļ�����һ��д����Ҫ"\n"����endl���л���
	outFile.close();								            //�ر��ļ�
};

void CustomziedMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) {
	std::cout << "<<<<<<���ѯ�۽��<<<<<<" << std::endl;
	std::cout << "�����գ�" << pForQuoteRsp->TradingDay << std::endl;
	std::cout << "���������룺" << pForQuoteRsp->ExchangeID << std::endl;
	std::cout << "��Լ���룺" << pForQuoteRsp->InstrumentID << std::endl;
	std::cout << "ѯ�۱�ţ�" << pForQuoteRsp->ForQuoteSysID << std::endl;
};