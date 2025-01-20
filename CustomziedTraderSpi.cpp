#include <iostream>
#include "CustomziedTraderSpi.h"

extern TThostFtdcBrokerIDType gBrokerID;
extern TThostFtdcInvestorIDType gInvesterID;
extern TThostFtdcPasswordType gInvesterPassword;
extern CThostFtdcTraderApi* g_pTradeUserApi;
extern TThostFtdcInstrumentIDType g_pTradeInstrumentID;
extern TThostFtdcDirectionType gTradeDirection;
extern TThostFtdcPriceType gLimitPrice;
extern TThostFtdcVolumeType gVolumeTotalOriginal;
extern bool loginFlag;

extern TThostFtdcFrontIDType	trade_front_id;	//ǰ�ñ��
extern TThostFtdcSessionIDType	session_id;	    //�Ự���
extern TThostFtdcOrderRefType	order_ref;	    //��������

void CustomziedTraderSpi::OnFrontConnected() {
	std::cout << "<<<<<<�����������ӳɹ�<<<<<<" << std::endl;
};

void CustomziedTraderSpi::OnFrontDisconnected(int nReason) {
	std::cerr << "<<<<<<�������ӶϿ�<<<<<<" << std::endl;
	std::cerr << "�����룺" << nReason << std::endl;
};

void CustomziedTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (!bResult)
	{
		std::cout << "<<<<<<�˻���¼�ɹ�<<<<<<" << std::endl;
		loginFlag = true;
		std::cout << "�����գ�" << pRspUserLogin->TradingDay << std::endl;
		std::cout << "��¼ʱ�䣺" << pRspUserLogin->LoginTime << std::endl;
		std::cout << "�����̣�" << pRspUserLogin->BrokerID << std::endl;
		std::cout << "�ʻ�����" << pRspUserLogin->UserID << std::endl;
		// ����Ự����
		trade_front_id = pRspUserLogin->FrontID;
		session_id = pRspUserLogin->SessionID;
		strcpy_s(order_ref, pRspUserLogin->MaxOrderRef);
	}
};

void CustomziedTraderSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	isErrorRspInfo(pRspInfo);
};

bool CustomziedTraderSpi::isErrorRspInfo(CThostFtdcRspInfoField* pRspInfo) {
	bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
	if (bResult) {
		std::cerr << "���ش���--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
	}
	return bResult;
};

void CustomziedTraderSpi::OnHeartBeatWarning(int nTimeLapse) {
	std::cerr << "<<<<<<����������ʱ<<<<<<" << std::endl;
	std::cerr << "���ϴ�����ʱ�䣺" << nTimeLapse << std::endl;
};

void CustomziedTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		loginFlag = false; // �ǳ��Ͳ����ٽ�����
		std::cout << "<<<<<<�˻��ǳ��ɹ�<<<<<<" << std::endl;
		std::cout << "�����̣� " << pUserLogout->BrokerID << std::endl;
		std::cout << "�ʻ����� " << pUserLogout->UserID << std::endl;
	}
};

void CustomziedTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<Ͷ���߽�����ȷ�ϳɹ�<<<<<<" << std::endl;
		std::cout << "ȷ�����ڣ�" << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "ȷ��ʱ�䣺" << pSettlementInfoConfirm->ConfirmTime << std::endl;
	};
};

void CustomziedTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<��ѯ��Լ����ɹ�<<<<<<" << std::endl;
		std::cout << "���������룺" << pInstrument->ExchangeID << std::endl;
		std::cout << "��Լ���룺" << pInstrument->InstrumentID << std::endl;
		std::cout << "��Լ�ڽ������Ĵ��룺" << pInstrument->ExchangeInstID << std::endl;
		std::cout << "ִ�мۣ�" << pInstrument->StrikePrice << std::endl;
		std::cout << "�����գ�" << pInstrument->EndDelivDate << std::endl;
		std::cout << "��ǰ����״̬��" << pInstrument->IsTrading << std::endl;
	};
};

void CustomziedTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<��ѯͶ�����ʽ��˻��ɹ�<<<<<<" << std::endl;
		std::cout << "Ͷ�����˺ţ�" << pTradingAccount->AccountID << std::endl;
		std::cout << "�����ʽ�" << pTradingAccount->Available << std::endl;
		std::cout << "��ȡ�ʽ�" << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "��ǰ��֤��:" << pTradingAccount->CurrMargin << std::endl;
		std::cout << "ƽ��ӯ����" << pTradingAccount->CloseProfit << std::endl;
	}
};

void CustomziedTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<��ѯͶ���ֲֳ߳ɹ�<<<<<<" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "��Լ���룺 " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "���ּ۸� " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "�������� " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "���ַ��� " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << std::endl;
		}
		else
		{
			std::cout << "----->�ú�Լδ�ֲ�" << std::endl;
		}
	}
};

void CustomziedTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "<<<<<<����¼��ɹ�<<<<<<" << std::endl;
		std::cout << "��Լ���룺 " << pInputOrder->InstrumentID << std::endl;
		std::cout << "�۸� " << pInputOrder->LimitPrice << std::endl;
		std::cout << "������ " << pInputOrder->VolumeTotalOriginal << std::endl;
		std::cout << "���ַ��� " << pInputOrder->Direction << std::endl;
	}
};

void CustomziedTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	if (!isErrorRspInfo(pRspInfo))
	{
		std::cout << "=====���������ɹ�=====" << std::endl;
		std::cout << "��Լ���룺 " << pInputOrderAction->InstrumentID << std::endl;
		std::cout << "������־�� " << pInputOrderAction->ActionFlag;
	}
};

void CustomziedTraderSpi::OnRtnOrder(CThostFtdcOrderField* pOrder) {
	char str[10];
	sprintf_s(str, "%d", pOrder->OrderSubmitStatus);
	int orderState = atoi(str) - 48;	//����״̬0=�Ѿ��ύ��3=�Ѿ�����

	std::cout << "=====�յ�����Ӧ��=====" << std::endl;

	if (isMyOrder(pOrder))
	{
		if (isTradingOrder(pOrder))
		{
			std::cout << "--->>> �ȴ��ɽ��У�" << std::endl;
			//reqOrderAction(pOrder); // ������Գ���
			//reqUserLogout(); // �ǳ�����
		}
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			std::cout << "--->>> �����ɹ���" << std::endl;
	}
};

void CustomziedTraderSpi::OnRtnTrade(CThostFtdcTradeField* pTrade) {
	std::cout << "=====�����ɹ��ɽ�=====" << std::endl;
	std::cout << "�ɽ�ʱ�䣺 " << pTrade->TradeTime << std::endl;
	std::cout << "��Լ���룺 " << pTrade->InstrumentID << std::endl;
	std::cout << "�ɽ��۸� " << pTrade->Price << std::endl;
	std::cout << "�ɽ����� " << pTrade->Volume << std::endl;
	std::cout << "��ƽ�ַ��� " << pTrade->Direction << std::endl;
};

bool CustomziedTraderSpi::isMyOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->FrontID == trade_front_id) &&
		(pOrder->SessionID == session_id) &&
		(strcmp(pOrder->OrderRef, order_ref) == 0));
}

bool CustomziedTraderSpi::isTradingOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}