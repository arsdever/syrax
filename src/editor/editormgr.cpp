#include "editormgr.h"

#include <tabwidget.h>

#include <QPlainTextEdit>

CEditorManager* CEditorManager::s_pGlobalInstance = nullptr;

CEditorManager::CEditorManager(QObject* pParent)
	: QObject(pParent)
	, m_pTabWidget(new CTabWidget())
	, m_nCurrentEditor(-1)
{
	m_pTabWidget->setTabsClosable(true);
	connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(OnClose(int)));
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(OnCurrentChanged(int)));
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

CEditor* CEditorManager::GetEditorAt(int nIndex) const
{
	return m_lstEditors.at(nIndex);
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

QTabWidget* CEditorManager::GetTabWidget() const
{
	return m_pTabWidget;
}

void CEditorManager::SaveCurrent()
{
}

void CEditorManager::SaveAll()
{

}

void CEditorManager::CloseEditor(CEditor* pEditor)
{
	Close(GetTabIndex(pEditor));
}

void CEditorManager::CloseEditor(QPlainTextEdit* pEditor)
{
	Close(GetTabIndex(pEditor));
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
		if ((*it)->GetCoreWidget() == pEditor)
			return i;
	}
	return -1;
}

void CEditorManager::XFileManipulator::Open(QStringList const& strPaths)
{
	for (QString path : strPaths)
	{
		CEditor* pNew = new CEditor(path);
		if (!pNew->IsValid())
		{
			delete pNew;
			continue;
		}

		m_pThis->m_lstEditors.push_back(pNew);

		if (m_pThis->m_pTabWidget == nullptr)
			return;

		int newTabIndex = m_pThis->m_pTabWidget->addTab(pNew->GetCoreWidget(), pNew->GetFileName());
		m_pThis->m_pTabWidget->setTabToolTip(newTabIndex, pNew->GetFilePath());
	}
	//m_pThis->SetCurrentEditor();
}

void CEditorManager::XFileManipulator::CloseAll(int nIndex)
{
	for(int index = m_pThis->m_lstEditors.size() - 1; index >= nIndex; --index)
	{
		m_pThis->Close(index);
	}
}

void CEditorManager::XFileManipulator::SaveAll()
{}

void CEditorManager::XFileManipulator::Close(int nIndex)
{
	m_pThis->Close(nIndex);
}

void CEditorManager::XFileManipulator::Save(QString const&) {}

void CEditorManager::XFileManipulator::New()
{
	CEditor* pNew = new CEditor();
	m_pThis->m_lstEditors.push_back(pNew);

	if (m_pThis->m_pTabWidget == nullptr)
		return;

	int newTabIndex = m_pThis->m_pTabWidget->addTab(pNew->GetCoreWidget(), pNew->GetFileName());
	m_pThis->m_pTabWidget->setTabToolTip(newTabIndex, pNew->GetFilePath());
}

CEditorManager* CEditorManager::GlobalInstance()
{
	return s_pGlobalInstance == nullptr ? s_pGlobalInstance = new CEditorManager(GetCore()) : s_pGlobalInstance;
}

void CEditorManager::Close(int nIndex)
{
	if (nIndex < 0)
	{
		if (m_nCurrentEditor < 0)
			return;

		nIndex = m_nCurrentEditor;
	}

	CEditor* pClosing = GetEditorAt(nIndex);
	if (!pClosing->AskForSave())
		return;

	m_pTabWidget->removeTab(nIndex);
	delete pClosing;
	m_lstEditors.removeAt(nIndex);
}

void CEditorManager::OnClose(int nIndex)
{
	QSet<IUnknown*> plgList = GetCore()->QueryInterface(IFileManipulator::UUID);
	for (IUnknown* plg : plgList)
	{
		IFileManipulator* plugin = dynamic_cast<IFileManipulator*> (plg);

		if (plugin == nullptr)
			continue;

		plugin->Close(nIndex);
	}
}

void CEditorManager::OnCurrentChanged(int nIndex)
{
	m_nCurrentEditor = nIndex;
}

extern "C" EDITOR_EXPORT void LoadPlugin()
{
	CEditorManager::GlobalInstance();
}

extern "C" EDITOR_EXPORT QTabWidget* Widget()
{
	return CEditorManager::GlobalInstance()->GetTabWidget();
}