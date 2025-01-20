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
	std::cout << "<<<<<<建立网络连接成功<<<<<<" << std::endl;
};

void CustomziedMdSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "<<<<<<网络连接断开<<<<<<" << std::endl;
	std::cerr << "错误码：" << nReason << std::endl;
};

void CustomziedMdSpi::OnHeartBeatWarning(int nTimeLapse) {
	std::cerr << "<<<<<<网络心跳超时<<<<<<" << std::endl;
	std::cerr << "距上次连接时间：" << nTimeLapse << std::endl;
};

void CustomziedMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult){
		std::cout << "<<<<<<账户登录成功<<<<<<" << std::endl;
		std::cout << "交易日：" << pRspUserLogin->TradingDay << std::endl;
		std::cout << "登录时间：" << pRspUserLogin->LoginTime << std::endl;
		std::cout << "经纪商：" << pRspUserLogin->BrokerID << std::endl;
		std::cout << "帐户名：" << pRspUserLogin->UserID << std::endl;
	}
	else {
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<账户登出成功<<<<<<" << std::endl;
		std::cout << "经纪商：" << pUserLogout->BrokerID << std::endl;
		std::cout << "帐户名：" << pUserLogout->UserID << std::endl;
	}
	else { 
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl; 
	}
};

void CustomziedMdSpi::OnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField* pMulticastInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

void CustomziedMdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult)
	{
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<订阅行情成功<<<<<<" << std::endl;
		std::cout << "合约代码：" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else { 
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<取消订阅行情成功<<<<<<" << std::endl;
		std::cout << "合约代码：" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<订阅询价成功<<<<<<" << std::endl;
		std::cout << "合约代码：" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<取消订阅询价成功<<<<<<" << std::endl;
		std::cout << "合约代码：" << pSpecificInstrument->InstrumentID << std::endl;
	}
	else
	{
		std::cerr << "返回错误--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
};

void CustomziedMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData) {
	//std::cout << "<<<<<<获得深度行情<<<<<<" << std::endl;
	//std::cout << "交易日： " << pDepthMarketData->TradingDay << std::endl;
	//std::cout << "交易所代码： " << pDepthMarketData->ExchangeID << std::endl;
	//std::cout << "合约代码： " << pDepthMarketData->InstrumentID << std::endl;
	//std::cout << "合约在交易所的代码： " << pDepthMarketData->ExchangeInstID << std::endl;
	//std::cout << "最新价： " << pDepthMarketData->LastPrice << std::endl;
	//std::cout << "数量： " << pDepthMarketData->Volume << std::endl;

	string fname = "au2502.csv";
	ofstream outFile(fname, ios::app);				  //打开文件进行写入,ios::app表示在文件末尾追加写入
	// ********写入数据*********
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
	//数字需转为字符串进行写入,csv文件结束一行写入需要"\n"或者endl进行换行
	outFile.close();								            //关闭文件
};

void CustomziedMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) {
	std::cout << "<<<<<<获得询价结果<<<<<<" << std::endl;
	std::cout << "交易日：" << pForQuoteRsp->TradingDay << std::endl;
	std::cout << "交易所代码：" << pForQuoteRsp->ExchangeID << std::endl;
	std::cout << "合约代码：" << pForQuoteRsp->InstrumentID << std::endl;
	std::cout << "询价编号：" << pForQuoteRsp->ForQuoteSysID << std::endl;
};