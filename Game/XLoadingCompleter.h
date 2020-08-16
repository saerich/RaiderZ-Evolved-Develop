#pragma once

class XLoadingCompleter
{
private:

public:
	XLoadingCompleter() {}
	virtual ~XLoadingCompleter() {}
	virtual void WorldLoadingFinish() = 0;			///< World �ε� �Ϸ�
	virtual void Complete() = 0;				///< �������� �غ� �Ϸ�
};

class XGame;
class XLoadingFrame;

class XLoadingCompleter_Game : public XLoadingCompleter
{
private:
	XGame*			m_pGame;
	XLoadingFrame*	m_pLoadingFrame;
public:
	XLoadingCompleter_Game(XGame* pGame, XLoadingFrame* pLoadingFrame);
	virtual ~XLoadingCompleter_Game();
	virtual void WorldLoadingFinish();
	virtual void Complete();
};