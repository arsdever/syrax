#include "editormgr.h"
#include <QTabWidget>
#include <QPlainTextEdit>

CEditorManager::CEditorManager(QObject* pParent)
	: CPluginManager(pParent)
	, m_nCurrentEditor(-1)
{
}

CEditorManager::~CEditorManager()
{
}

CEditor* CEditorManager::GetCurrentEditor() const
{
	return m_lstEditors.at(m_nCurrentEditor);
}

CEditor* CEditorManager::GetEditor(QPlainTextEdit* pEditor) const
{
	int index = GetTabIndex(pEditor);

	if (index > -1)
		return m_lstEditors.at(index);

	return nullptr;
}

void CEditorManager::NewEditor(QString const& strPath)
{
	CEditor* pNew = new CEditor(strPath);
	m_lstEditors.push_back(pNew);
	SetCurrentEditor(m_lstEditors.size() - 1);

	if (m_pTabWidget == nullptr)
		return;

	m_pTabWidget->addTab(pNew->GetCore(), pNew->GetFileName());
}

void CEditorManager::SetCurrentEditor(int nIndex)
{
	if (nIndex >= m_lstEditors.size())
		return;

	m_nCurrentEditor = nIndex;

	emit CurrentEditorChanged(nIndex);
}

void CEditorManager::SetCurrentEditor(CEditor* pEditor)
{
	SetCurrentEditor(GetTabIndex(pEditor));
}

void CEditorManager::SetCurrentEditor(QPlainTextEdit* pEditor)
{
	SetCurrentEditor(GetTabIndex(pEditor));
}

void CEditorManager::SetTabWidget(QTabWidget* pTabWidget)
{
	m_pTabWidget = pTabWidget;
}

void CEditorManager::CloseEditor(int nIndex)
{
}

void CEditorManager::CloseEditor(CEditor* pEditor)
{
	CloseEditor(GetTabIndex(pEditor));
}

void CEditorManager::CloseEditor(QPlainTextEdit* pEditor)
{
	CloseEditor(GetTabIndex(pEditor));
}

int CEditorManager::GetTabIndex(CEditor* pEditor) const
{
	return m_lstEditors.indexOf(pEditor);
}

int CEditorManager::GetTabIndex(QPlainTextEdit* pEditor) const
{
	auto it = m_lstEditors.cbegin();
	int i = 0;
	for (; it != m_lstEditors.cend(); ++i, ++it)
	{
		if ((*it)->GetCore() == pEditor)
			return i;
	}
}